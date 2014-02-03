#include <stdbool.h>
#include "pubnub.h"


void
pubnub_init(struct pubnub *p, const char *publish_key, const char *subscribe_key)
{
    p->publish_key = publish_key;
    p->subscribe_key = subscribe_key;
    p->timeout = 5;
    p->sub_timeout = 10;
    pubnub_set_origin(p, "http://pubsub.pubnub.com/");
}

void
pubnub_done(struct pubnub *p)
{
    /* TODO tear down network */
}

void
pubnub_set_origin(struct pubnub *p, const char *origin)
{
    /* TODO: More generic URL handling */
    strcpy(p->origin, origin + strlen("http://"));
    int namelen = strlen(p->origin);
    if (p->origin[namelen-1] == '/')
        p->origin[namelen-1] = 0;
}

void
pubnub_set_timeout(struct pubnub *p, int timeout)
{
    p->timeout = timeout;
}

void
pubnub_set_sub_timeout(struct pubnub *p, int timeout)
{
    p->sub_timeout = timeout;
}


static bool
pubnub_http_connect(struct pubnub *p)
{
    p->socket = TCPOpen((PTR_BASE) p->origin, TCP_OPEN_RAM_HOST, 80, TCP_PURPOSE_PUBNUB_CLIENT);
    if (p->socket == INVALID_SOCKET)
        return false;
    p->state = PS_CONNECT;
    p->timer = TickGet();
    p->http_substate = 0;
    p->http_buf_len = 0;
    return true;
}

static bool
pubnub_update_test_timeout(struct pubnub *p)
{
    if (TickGet() - p->timer > p->com_timeout * TICK_SECOND) {
        TCPDisconnect(p->socket);
        p->internal_cb(p, PNR_TIMEOUT);
        p->state = PS_IDLE;
        return true;
    }
    return false;
}

static bool
pubnub_update_sendrequest(struct pubnub *p)
{
    if (!TCPIsConnected(p->socket)) {
        p->internal_cb(p, PNR_IO_ERROR);
        p->state = PS_IDLE;
        return false;
    } else if (pubnub_update_test_timeout(p))
        return false;

    if (p->http_substate <= 0) {
        ROM BYTE *s = (ROM BYTE *) "GET ";
        if (TCPIsPutReady(p->socket) < strlen(s))
            return true;
        TCPPutROMString(p->socket, s);
        p->http_substate++;
    }
    if (p->http_substate <= 1) {
        int url_len = strlen(p->http_buf.url);
        if (TCPIsPutReady(p->socket) < url_len)
            return true;
        TCPPutString(p->socket, (BYTE *) p->http_buf.url);
        p->http_substate++;
    }
    if (p->http_substate <= 2) {
        ROM BYTE *s = (ROM BYTE *) " HTTP/1.1\r\nHost: ";
        if (TCPIsPutReady(p->socket) < strlen(s))
            return true;
        TCPPutROMString(p->socket, s);
        p->http_substate++;
    }
    if (p->http_substate <= 3) {
        int namelen = strlen(p->origin);
        if (TCPIsPutReady(p->socket) < namelen)
            return true;
        TCPPutString(p->socket, (BYTE *) p->origin);
        p->http_substate++;
    }
    if (p->http_substate <= 4) {
        ROM BYTE *s = (ROM BYTE *) "\r\nUser-Agent: PubNub-Arduino/1.0\r\nConnection: close\r\n\r\n";
        if (TCPIsPutReady(p->socket) < strlen(s))
            return true;
        TCPPutROMString(p->socket, s);
        p->state = PS_HTTPREPLY;
    }
    return true;
}

static void
pubnub_update_recvreply(struct pubnub *p)
{
    int readylen = TCPIsGetReady(p->socket);
    if (!readylen) {
        if (!TCPIsConnected(p->socket)) {
            p->internal_cb(p, PNR_IO_ERROR);
            p->state = PS_IDLE;
            p->http_substate = 0;
            return;
        }
        pubnub_update_test_timeout(p);
        return;
    }

    /* http_substate:
     * 0 first line (status code etc.)
     * 1 plain header
     * 2 plain header, chunked encoding detected
     * 3 chunk size line */
    while (p->state == PS_HTTPREPLY && readylen > 0) {
        if (p->http_buf_len == sizeof (p->http_buf.line)) {
            /* Our buffer is already full and we did not extract a line.
             * Normally, hitting a long line in the HTTP header should
             * not happen, let's just flush the buffer. */
            p->http_buf_len = 0;
        }
        int gotlen = TCPGetArray(p->socket,
                p->http_buf.line + p->http_buf_len,
                sizeof (p->http_buf.line) - p->http_buf_len);
        p->http_buf_len += gotlen;
        readylen -= gotlen;

        char *newline;
        while (p->state == PS_HTTPREPLY
               && (newline = strstr(p->http_buf.line, "\r\n"))) {
            *newline = 0;

            /* Empty line handling. */
            if (newline - p->http_buf.line == 0) {
                switch (p->http_substate) {
                    case 1: /* Content follows. */
                        p->state = PS_PROCESS;
                        break;
                    case 2: /* Chunked encoding, another line
                                     * with content length follows. */
                        p->http_substate = 3;
                        break;
                    case 0:
                    case 3: /* Error. */
                        TCPDisconnect(p->socket);
                        p->internal_cb(p, PNR_IO_ERROR);
                        p->state = PS_IDLE;
                        return; /* straight out */
                }
                if (p->http_substate == 1) {
                    /* In case of Chunked encoding, another line
                     * with content length follows. */
                    p->http_substate = 2;
                } else {
                    /* Content follows. */
                    p->state = PS_HTTPBODY;
                    p->http_reply = NULL;
                }

            } else if (p->http_substate == 0) {
                /* An HTTP status line. */
                /* TODO */

            } else if (p->http_substate < 3) {
                /* An HTTP header line. */
                /* TODO */

            } else if (p->http_substate == 3) {
                /* A chunk size line. */
                /* TODO */
            }
        }
    }
}

static void
pubnub_update_recvbody(struct pubnub *p)
{
    if (!p->http_reply) {
        p->http_reply = malloc(p->http_content_length);
        if (p->http_buf_len < p->http_content_length)
            goto io_error;
        memcpy(p->http_reply, p->http_buf.line, p->http_buf_len);
        p->http_content_length -= p->http_buf_len;
    }
    if (p->http_content_length == 0) {
        p->state = PS_PROCESS;
    }

    int readylen = TCPIsGetReady(p->socket);
    if (!readylen) {
        if (!TCPIsConnected(p->socket)) {
io_error:
            p->internal_cb(p, PNR_IO_ERROR);
            p->state = PS_IDLE;
            p->http_substate = 0;
            free(p->http_reply); p->http_reply = NULL;
            return;
        }
        pubnub_update_test_timeout(p);
        if (p->state == PS_IDLE) {
            free(p->http_reply); p->http_reply = NULL;
        }
        return;
    }

    while (readylen > 0) {
        if (readylen > p->http_content_length)
            goto io_error;

        int gotlen = TCPGetArray(p->socket,
                p->http_reply + p->http_buf_len,
                p->http_content_length - p->http_buf_len);
        p->http_buf_len += gotlen;
        p->http_content_length -= gotlen;
        readylen -= gotlen;
    }

    if (p->http_content_length == 0)
        p->state = PS_PROCESS;
}

void
pubnub_update(struct pubnub *p)
{
    switch (p->state) {
        case PS_IDLE:
            break;

        case PS_CONNECT:
            if (!TCPIsConnected(p->socket)) {
                pubnub_update_test_timeout(p);
                break;
            }
            p->state = PS_HTTPREQUEST;
            /* fall-through */

        case PS_HTTPREQUEST:
            if (pubnub_update_sendrequest(p))
                TCPFlush(p->socket);
            break;

        case PS_HTTPREPLY:
            pubnub_update_recvreply(p);
            if (p->state != PS_HTTPBODY)
                break;
            /* ...otherwise fall-through. */

        case PS_HTTPBODY:
            pubnub_update_recvbody(p);
            break;

        case PS_PROCESS:
            p->internal_cb(p, PNR_OK);
            free(p->http_reply); p->http_reply = NULL;
            p->state = PS_IDLE;
            break;
    }
}


void
pubnub_publish(struct pubnub *p, const char *channel, const char *message,
        pubnub_publish_cb cb, void *cb_data)
{
    /* TODO */
}

void
pubnub_subscribe(struct pubnub *p, const char *channel,
        pubnub_subscribe_cb cb, void *cb_data)
{
    /* TODO */
}