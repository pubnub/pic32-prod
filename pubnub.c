#include <stdbool.h>

#include "pubnub.h"


void
pubnub_init(struct pubnub *p, const char *publish_key, const char *subscribe_key)
{
    p->publish_key = publish_key;
    p->subscribe_key = subscribe_key;
    p->timeout = 10;
    p->sub_timeout = 310;
    pubnub_set_origin(p, "http://pubsub.pubnub.com/");
    strcpy(p->timetoken, "0");
}

void
pubnub_done(struct pubnub *p)
{
    if (p->state != PS_IDLE)
        TCPDisconnect(p->socket);
    if (p->http_reply) {
        free(p->http_reply); p->http_reply = NULL;
    }
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

static void
pubnub_callback_and_idle(struct pubnub *p, enum pubnub_res result)
{
    /* Reset the context *before* the callback, as the callback may
     * typically want to issue another API call. */
    /* N.B. We ignore p->channel and p->timetoken; it's internal_cb's
     * job to handle these if necessary (shouldn't be necessary). */

    p->state = PS_IDLE;
    p->http_substate = 0;

    if (p->socket != INVALID_SOCKET) {
        TCPDisconnect(p->socket);
        p->socket = INVALID_SOCKET;
    }

    char *http_reply = p->http_reply;
    p->http_reply = NULL;
    int replylen = p->http_content_length;
    p->http_content_length = 0;
    void *cb = p->cb, *cbdata = p->cbdata;
    p->cb = p->cbdata = NULL;

    p->internal_cb(p, result, http_reply, replylen, cb, cbdata);

    if (http_reply)
        free(http_reply);
}

static bool
pubnub_update_test_timeout(struct pubnub *p)
{
    if (TickGet() - p->timer > p->com_timeout * TICK_SECOND) {
        pubnub_callback_and_idle(p, PNR_TIMEOUT);
        return true;
    }
    return false;
}

static bool
pubnub_update_sendrequest(struct pubnub *p)
{
    if (!TCPIsConnected(p->socket)) {
        pubnub_callback_and_idle(p, PNR_IO_ERROR);
        return false;
    } else if (pubnub_update_test_timeout(p))
        return false;

    if (p->http_substate <= 0) {
#define S (ROM BYTE *) "GET "
        if (TCPIsPutReady(p->socket) < sizeof(S)-1)
            return true;
        TCPPutROMString(p->socket, S);
#undef S
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
#define S (ROM BYTE *) " HTTP/1.1\r\nHost: "
        if (TCPIsPutReady(p->socket) < sizeof(S)-1)
            return true;
        TCPPutROMString(p->socket, S);
#undef S
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
#define S (ROM BYTE *) "\r\nUser-Agent: PubNub-Arduino/1.0\r\nConnection: close\r\n\r\n"
        if (TCPIsPutReady(p->socket) < sizeof(S)-1)
            return true;
        TCPPutROMString(p->socket, S);
#undef S
        p->state = PS_HTTPREPLY;
        p->http_substate = 0;
    }
    return true;
}

static void
pubnub_update_recvreply(struct pubnub *p)
{
    int readylen = TCPIsGetReady(p->socket);
    if (!readylen) {
        if (!TCPIsConnected(p->socket)) {
            pubnub_callback_and_idle(p, PNR_IO_ERROR);
            return;
        }
        pubnub_update_test_timeout(p);
        return;
    }

    /* http_substate:
     * 0 first line (status code etc.)
     * 1 plain header
     * 2 plain header, chunked encoding detected
     * 3 chunk size line
     * 4 body follows now (transient) */
    while (p->state == PS_HTTPREPLY && readylen > 0) {
        if (p->http_buf_len == sizeof (p->http_buf.line)) {
            /* Our buffer is already full and we did not extract a line.
             * Normally, hitting a long line in the HTTP header should
             * not happen, let's just flush the buffer. */
            p->http_buf_len = 0;
        }
        int gotlen = TCPGetArray(p->socket,
                (BYTE *) (p->http_buf.line + p->http_buf_len),
                sizeof(p->http_buf.line)-1 - p->http_buf_len);
        p->http_buf_len += gotlen;
        readylen -= gotlen;

        p->http_buf.line[p->http_buf_len] = 0;
        char *bufptr = p->http_buf.line;
        char *newline;
        while (p->state == PS_HTTPREPLY
               && (newline = strstr(bufptr, "\r\n"))) {
            *newline = 0;

            if (newline - bufptr == 0) {
                /* Empty line handling. */
                switch (p->http_substate) {
                    case 1: /* Content follows. */
                        p->http_substate = 4;
                        break;
                    case 2: /* Chunked encoding, another line
                             * with content length follows. */
                        p->http_substate = 3;
                        break;
                    case 0:
                    case 3: /* Error. */
io_error:
                        pubnub_callback_and_idle(p, PNR_IO_ERROR);
                        return; /* straight out */
                }

            } else if (p->http_substate == 0) {
                /* An HTTP status line. */
                if (strncmp(bufptr, "HTTP/1.", 7) || !bufptr[7] || !bufptr[8])
                    goto io_error;
                int http_code = atoi(bufptr+9);
                if (http_code / 100 != 2) {
                    pubnub_callback_and_idle(p, PNR_HTTP_ERROR);
                    return;
                }
                p->http_substate = 1;

            } else if (p->http_substate < 3) {
                /* An HTTP header line. */
                char h_chunked[] = "Transfer-Encoding: chunked";
                char h_length[] = "Content-Length: ";
                if (!strncmp(bufptr, h_chunked, sizeof(h_chunked)-1)) {
                    p->http_substate = 2;

                } else if (!strncmp(bufptr, h_length, sizeof(h_length)-1)) {
                    p->http_content_length = atoi(bufptr + sizeof(h_length)-1);
                }

            } else if (p->http_substate == 3) {
                /* A chunk size line. */
                p->http_content_length = strtoul(bufptr, NULL, 16);
                p->http_substate = 4;
            }

            if (p->http_substate == 4) {
                p->state = PS_HTTPBODY;
                p->http_reply = NULL;
            }

            p->http_buf_len -= (newline+2 - bufptr);
            bufptr = newline+2;
        }
        memmove(p->http_buf.line, bufptr, p->http_buf_len + 1);
    }
}

static void
pubnub_update_recvbody(struct pubnub *p)
{
    if (!p->http_reply) {
        if (p->http_content_length > PUBNUB_REPLY_MAXLEN) {
            /* Too large reply. Abort and report format error. */
            pubnub_callback_and_idle(p, PNR_FORMAT_ERROR);
            return;
        }
        p->http_reply = malloc(p->http_content_length);
        if (p->http_buf_len > p->http_content_length)
            goto io_error;
        memcpy(p->http_reply, p->http_buf.line, p->http_buf_len);
        p->http_content_length -= p->http_buf_len;
    }
    if (p->http_content_length == 0) {
        p->state = PS_PROCESS;
	return;
    }

    int readylen = TCPIsGetReady(p->socket);
    if (!readylen) {
        if (!TCPIsConnected(p->socket)) {
io_error:
            pubnub_callback_and_idle(p, PNR_IO_ERROR);
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
                (BYTE *) (p->http_reply + p->http_buf_len),
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
            pubnub_callback_and_idle(p, PNR_OK);
            break;
    }
}


void
pubnub_publish_icb(struct pubnub *p, enum pubnub_res result, char *reply, int replylen, void *cb, void *cbdata)
{
    if (cb)
        ((pubnub_publish_cb) cb)(p, result, reply, cbdata);
}

bool
pubnub_publish(struct pubnub *p, const char *channel, const char *message,
        pubnub_publish_cb cb, void *cb_data)
{
    if (p->state != PS_IDLE)
        return false;
    p->http_reply = NULL;
    p->http_content_length = 0;

    p->http_buf_len = snprintf(p->http_buf.url, sizeof(p->http_buf.url),
            "/publish/%s/%s/0/%s/0/", p->publish_key, p->subscribe_key,
            channel);

    const char *pmessage = message;
    while (pmessage[0]) {
        /* RFC 3986 Unreserved characters plus few
         * safe reserved ones. */
        size_t okspan = strspn(pmessage, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.~" ",=:;@[]");
        if (okspan > 0) {
            if (okspan > sizeof(p->http_buf.url)-1 - p->http_buf_len) {
                p->http_buf_len = 0;
                return false;
            }
            memcpy(&p->http_buf.url[p->http_buf_len], pmessage, okspan);
            p->http_buf_len += okspan;
            p->http_buf.url[p->http_buf_len] = 0;
            pmessage += okspan;
        }
        if (pmessage[0]) {
            /* %-encode a non-ok character. */
            char enc[4] = {'%'};
            enc[1] = "0123456789ABCDEF"[pmessage[0] / 16];
            enc[2] = "0123456789ABCDEF"[pmessage[0] % 16];
            if (3 > sizeof(p->http_buf.url)-1 - p->http_buf_len) {
                p->http_buf_len = 0;
                return false;
            }
            memcpy(&p->http_buf.url[p->http_buf_len], enc, 4);
            p->http_buf_len += 3;
            pmessage++;
        }
    }

    p->cb = cb; p->cbdata = cb_data;
    p->internal_cb = pubnub_publish_icb;
    p->channel = channel;
    p->com_timeout = p->timeout;
    return pubnub_http_connect(p);
}


void
pubnub_subscribe_icb(struct pubnub *p, enum pubnub_res result, char *reply, int replylen, void *cb, void *cbdata)
{
    if (result != PNR_OK) {
error:
        if (result == PNR_FORMAT_ERROR) {
            /* In case of PubNub protocol error, abort an ongoing subscribe
             * and start over. This means some messages were lost, but allows
             * us to recover from bad situations, e.g. too many messages
             * queued or unexpected problem caused by a particular message. */
            strcpy(p->timetoken, "0");
        }
        if (cb)
            ((pubnub_subscribe_cb) cb)(p, result, p->channel, reply, cbdata);
        return;
    }
    if (reply[0] != '[' || reply[replylen-1] != ']'
        || reply[replylen-2] != '"') {
        result = PNR_FORMAT_ERROR;
        goto error;
    }

    /* Extract timetoken. */
    reply[replylen-2] = 0;
    int i;
    for (i = replylen-3; i > 0; i--)
        if (reply[i] == '"')
            break;
    if (!i || reply[i-1] != ',' || replylen-2 - (i+1) >= 64) {
        result = PNR_FORMAT_ERROR;
        goto error;
    }
    strcpy(p->timetoken, &reply[i+1]);
    reply[i-1] = 0; // terminate the [] message array

    /* Here, as @reply we pass only the [msg1,msg2,...] array. */
    if (cb)
        ((pubnub_subscribe_cb) cb)(p, PNR_OK, p->channel, reply+1, cbdata);
}

bool
pubnub_subscribe(struct pubnub *p, const char *channel,
        pubnub_subscribe_cb cb, void *cb_data)
{
    if (p->state != PS_IDLE)
        return false;
    p->http_reply = NULL;
    p->http_content_length = 0;

    p->http_buf_len = snprintf(p->http_buf.url, sizeof(p->http_buf.url),
            "/subscribe/%s/%s/0/%s", p->subscribe_key,
            channel, p->timetoken);

    p->cb = cb; p->cbdata = cb_data;
    p->internal_cb = pubnub_subscribe_icb;
    p->com_timeout = p->sub_timeout;
    return pubnub_http_connect(p);
}
