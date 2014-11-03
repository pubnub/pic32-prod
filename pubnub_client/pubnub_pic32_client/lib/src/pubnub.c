#include <stdbool.h>

#include "pubnub.h"

#if PUBNUB_SSL && !PUBNUB_CYASSL
#include <tcpip/ssl.h>
#endif

#if !defined(PUBNUB_SSL)
#define PUBNUB_SSL 0
#endif
#if !defined(PUBNUB_CYASSL)
#define PUBNUB_CYASSL 0
#endif


#if PUBNUB_CYASSL
static const unsigned char ssl_cert_ca[] = {
    /* The self-signed certificate for petr.devbuild.pubnub.com */
    0x30, 0x82, 0x02, 0xa5, 0x30, 0x82, 0x02, 0x0e, 0x02, 0x09, 0x00, 0xd1,
    0x1b, 0xbe, 0x8d, 0x9e, 0x3d, 0x6a, 0xcd, 0x30, 0x0d, 0x06, 0x09, 0x2a,
    0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x30, 0x81,
    0x96, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02,
    0x55, 0x53, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c,
    0x02, 0x43, 0x41, 0x31, 0x16, 0x30, 0x14, 0x06, 0x03, 0x55, 0x04, 0x07,
    0x0c, 0x0d, 0x53, 0x61, 0x6e, 0x20, 0x46, 0x72, 0x61, 0x6e, 0x63, 0x69,
    0x73, 0x63, 0x6f, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55, 0x04, 0x0a,
    0x0c, 0x06, 0x50, 0x75, 0x62, 0x4e, 0x75, 0x62, 0x31, 0x0f, 0x30, 0x0d,
    0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x06, 0x44, 0x65, 0x76, 0x4f, 0x70,
    0x73, 0x31, 0x21, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x18,
    0x70, 0x65, 0x74, 0x72, 0x2e, 0x64, 0x65, 0x76, 0x62, 0x75, 0x69, 0x6c,
    0x64, 0x2e, 0x70, 0x75, 0x62, 0x6e, 0x75, 0x62, 0x2e, 0x63, 0x6f, 0x6d,
    0x31, 0x1d, 0x30, 0x1b, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
    0x01, 0x09, 0x01, 0x16, 0x0e, 0x6a, 0x61, 0x79, 0x40, 0x70, 0x75, 0x62,
    0x6e, 0x75, 0x62, 0x2e, 0x63, 0x6f, 0x6d, 0x30, 0x1e, 0x17, 0x0d, 0x31,
    0x34, 0x30, 0x34, 0x32, 0x33, 0x30, 0x32, 0x34, 0x35, 0x32, 0x32, 0x5a,
    0x17, 0x0d, 0x31, 0x35, 0x30, 0x34, 0x32, 0x33, 0x30, 0x32, 0x34, 0x35,
    0x32, 0x32, 0x5a, 0x30, 0x81, 0x96, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03,
    0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x0b, 0x30, 0x09, 0x06,
    0x03, 0x55, 0x04, 0x08, 0x0c, 0x02, 0x43, 0x41, 0x31, 0x16, 0x30, 0x14,
    0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x0d, 0x53, 0x61, 0x6e, 0x20, 0x46,
    0x72, 0x61, 0x6e, 0x63, 0x69, 0x73, 0x63, 0x6f, 0x31, 0x0f, 0x30, 0x0d,
    0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x06, 0x50, 0x75, 0x62, 0x4e, 0x75,
    0x62, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x06,
    0x44, 0x65, 0x76, 0x4f, 0x70, 0x73, 0x31, 0x21, 0x30, 0x1f, 0x06, 0x03,
    0x55, 0x04, 0x03, 0x0c, 0x18, 0x70, 0x65, 0x74, 0x72, 0x2e, 0x64, 0x65,
    0x76, 0x62, 0x75, 0x69, 0x6c, 0x64, 0x2e, 0x70, 0x75, 0x62, 0x6e, 0x75,
    0x62, 0x2e, 0x63, 0x6f, 0x6d, 0x31, 0x1d, 0x30, 0x1b, 0x06, 0x09, 0x2a,
    0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x01, 0x16, 0x0e, 0x6a, 0x61,
    0x79, 0x40, 0x70, 0x75, 0x62, 0x6e, 0x75, 0x62, 0x2e, 0x63, 0x6f, 0x6d,
    0x30, 0x81, 0x9f, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7,
    0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x81, 0x8d, 0x00, 0x30, 0x81,
    0x89, 0x02, 0x81, 0x81, 0x00, 0xb1, 0x63, 0xb1, 0x85, 0x02, 0x74, 0x3e,
    0x8c, 0x80, 0x8f, 0x1e, 0xfd, 0xc1, 0x79, 0x1a, 0xbf, 0xf0, 0xa0, 0x50,
    0x7f, 0xae, 0x6f, 0x6f, 0x78, 0x4c, 0xae, 0xc8, 0x8e, 0xe0, 0x12, 0x94,
    0x77, 0x69, 0x02, 0x50, 0xd5, 0x32, 0x66, 0x8a, 0x8e, 0x0d, 0xd2, 0xf1,
    0x22, 0x37, 0x50, 0x1d, 0x55, 0x91, 0x5d, 0xfd, 0x0e, 0xb5, 0xb7, 0x4f,
    0x95, 0x40, 0x7d, 0xe2, 0x52, 0x58, 0x01, 0x59, 0x04, 0x0b, 0x82, 0x5c,
    0x72, 0xca, 0xcb, 0x35, 0x9c, 0xe6, 0x87, 0xc4, 0x89, 0x01, 0xe3, 0x74,
    0xf6, 0x02, 0xfa, 0x7b, 0x98, 0x0d, 0x20, 0x68, 0xab, 0x15, 0xc8, 0x8a,
    0x5b, 0x89, 0xc4, 0x30, 0xa5, 0xbf, 0x81, 0xea, 0x38, 0x2a, 0x36, 0x83,
    0xa3, 0x04, 0x84, 0x87, 0xda, 0x6c, 0xc3, 0xc2, 0x51, 0x5d, 0x04, 0xfd,
    0x86, 0xb1, 0x9a, 0x71, 0xa9, 0xf4, 0x70, 0x62, 0xbf, 0x5f, 0x22, 0x2c,
    0xef, 0x02, 0x03, 0x01, 0x00, 0x01, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86,
    0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x03, 0x81, 0x81,
    0x00, 0xa6, 0x6f, 0x54, 0x5f, 0x63, 0xf8, 0x73, 0xc7, 0x73, 0xb0, 0xcd,
    0xb2, 0x9b, 0x1c, 0xe2, 0xe2, 0x87, 0x67, 0x51, 0x11, 0x56, 0xe2, 0xea,
    0xae, 0xf9, 0x6b, 0x14, 0xb3, 0x8c, 0xaf, 0xaa, 0x33, 0x9b, 0xca, 0xf2,
    0x8e, 0xfb, 0x4b, 0x8b, 0xeb, 0xa2, 0x1f, 0x12, 0x96, 0x2c, 0xf8, 0x21,
    0x3b, 0x9b, 0xd0, 0xa9, 0x4c, 0x70, 0xb6, 0x17, 0xfb, 0xe2, 0x7c, 0x8a,
    0x82, 0x5c, 0x73, 0x17, 0xb7, 0xca, 0xf6, 0x4c, 0xcd, 0xee, 0x3a, 0x56,
    0x49, 0x2e, 0x58, 0xcc, 0x58, 0xe6, 0x31, 0x76, 0x0c, 0xd8, 0x3f, 0xaf,
    0x73, 0xdb, 0x2a, 0xa4, 0x7a, 0xc2, 0x79, 0x00, 0x8c, 0x35, 0x95, 0x2a,
    0xb5, 0xf3, 0xf3, 0xb4, 0x88, 0x45, 0xfb, 0x02, 0xe5, 0x06, 0xde, 0xe1,
    0xc1, 0x62, 0xfc, 0xa6, 0x6a, 0x1a, 0x3e, 0x48, 0xd1, 0x4c, 0x59, 0xd0,
    0xc1, 0x9a, 0xf9, 0x2e, 0x75, 0xcc, 0x5a, 0x28, 0x32,
};


/* Custom recv/send callbacks for SSL. */
static int ssl_recvf(CYASSL* ssl, char* buf, int sz, void* vp) {
    int sockfd = *(int*) vp;
    if (TCPIP_TCP_GetIsReady(sockfd)) {
        uint16_t got = TCPIP_TCP_ArrayGet(sockfd, (uint8_t*) buf, sz);
        return got;
    } else
        return -2; // WANT_READ
}

static int ssl_sendf(CYASSL* ssl, char* buf, int sz, void* vp) {
    int sockfd = *(int*) vp;
    if (TCPIP_TCP_PutIsReady(sockfd)) {
        uint16_t put = TCPIP_TCP_ArrayPut(sockfd, (uint8_t*) buf, sz);
        return put;
    } else
        return -2; // WANT_WRITE
}

/* All pubnub contexts share a common CyaSSL context. */
CYASSL_CTX *ssl_ctx;
static CYASSL_CTX *
get_ssl_ctx()
{
    if (!ssl_ctx) {
        ssl_ctx = CyaSSL_CTX_new(CyaTLSv1_2_client_method());
        CyaSSL_CTX_load_verify_buffer(ssl_ctx, ssl_cert_ca,
                sizeof(ssl_cert_ca), SSL_FILETYPE_ASN1);

        CyaSSL_SetIORecv(ssl_ctx, ssl_recvf);
        CyaSSL_SetIOSend(ssl_ctx, ssl_sendf);
    }
    return ssl_ctx;
}

int rerr;
static int
pubnub_tcp_read(struct pubnub *p, uint8_t *buf, size_t buf_size, size_t buf_pos)
{
    int r = CyaSSL_read(p->ssl, buf + buf_pos, buf_size - buf_pos);
    rerr = r;
    if (r < 0) {
        int error = CyaSSL_get_error(p->ssl, 0);
        if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE)
            return 0;
        else
            return -1;
    }
    return r;
}

static bool
pubnub_tcp_writestr(struct pubnub *p, char *string, int len)
{
    int r = CyaSSL_write(p->ssl, string, len);
    if (r < 0)
        return false; // even if it's WANT_WRITE; we retry later
    return true;
}

#else

static int
pubnub_tcp_read(struct pubnub *p, uint8_t *buf, int buf_size, int buf_pos)
{
    int readylen = TCPIP_TCP_GetIsReady(p->socket);
    if (!readylen)
        return TCPIP_TCP_IsConnected(p->socket) ? 0 : -1;

    return TCPIP_TCP_ArrayGet(p->socket, buf + buf_pos, buf_size - buf_pos);
}

static bool
pubnub_tcp_writestr(struct pubnub *p, char *string, int len)
{
    if (TCPIP_TCP_PutIsReady(p->socket) < len)
        return false;
    TCPIP_TCP_StringPut(p->socket, (uint8_t *) string);
    return true;
}
#endif

/* pubnub_tcp_read() returns #of bytes in case of a read, 0 in case
 * there is nothing to read, and -1 in case of IO error. */


/* Find the beginning of a JSON string that ends at &buf[len].
 * Return -1 on error. */
static int
extract_last_string(char *buf, int len)
{
    buf[len] = 0;
    int i;
    for (i = len-1; i > 0; i--)
        if (buf[i] == '"')
            break;
    if (!i || buf[i-1] != ',')
        return -1;
    return i;
}

/* Split a JSON array (with arbitrary contents) to multiple NUL-terminated
 * C strings. */
static bool
split_array(char *buf, int len)
{
    bool escaped = false, in_string = false;
    int bracket_level = 0;
    int i;
    for (i = 0; i < len; i++) {
        if (escaped) {
            escaped = false;
        } else if (in_string) {
            switch (buf[i]) {
                case '\\': escaped = true; break;
                case '"': in_string = false; break;
                default: break;
            }
        } else {
            switch (buf[i]) {
                case '"': in_string = true; break;
                case '[': case '{': bracket_level++; break;
                case ']': case '}': bracket_level--; break;
                /* if at root, split! */
                case ',': if (bracket_level == 0) buf[i] = 0; break;
                default: break;
            }
        }
    }
    if (escaped || in_string || bracket_level > 0)
        return false;
    return true;
}


void
pubnub_init(struct pubnub *p, const char *publish_key, const char *subscribe_key)
{
    p->publish_key = publish_key;
    p->subscribe_key = subscribe_key;
    p->timeout = 10;
    p->sub_timeout = 310;
    if (PUBNUB_SSL) {
        pubnub_set_origin(p, "https://pubsub.pubnub.com/");
#if PUBNUB_CYASSL
            p->ssl = NULL;
#endif
    }
    else {
        pubnub_set_origin(p, "http://pubsub.pubnub.com/");
    }

    strcpy(p->timetoken, "0");
    p->uuid = NULL;
    p->auth = NULL;
    p->socket = INVALID_SOCKET;
}

void
pubnub_done(struct pubnub *p)
{
    if (p->socket != INVALID_SOCKET) {
        TCPIP_TCP_Disconnect(p->socket);
        TCPIP_TCP_Disconnect(p->socket); // we may want to reuse the socket, RST!
        p->socket = INVALID_SOCKET;
    }
#if PUBNUB_CYASSL
    if (p->ssl) {
        CyaSSL_shutdown(p->ssl);
        CyaSSL_free(p->ssl);
        p->ssl = NULL;
    }
#endif
    if (p->http_reply) {
        free(p->http_reply);
        p->http_reply = NULL;
    }
}

void
pubnub_set_origin(struct pubnub *p, const char *origin)
{
    if (PUBNUB_SSL && origin[4] == 's') {
        p->use_ssl = 1;
        strcpy(p->origin, origin + strlen("https://"));
    } else {
        strcpy(p->origin, origin + strlen("http://"));
    }
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

void
pubnub_set_uuid(struct pubnub *p, const char *uuid)
{
    p->uuid = uuid;
}

void
pubnub_set_auth(struct pubnub *p, const char *auth)
{
    p->auth = auth;
}


static bool
pubnub_http_connect(struct pubnub *p)
{
    TCPIP_DNS_RESULT result = TCPIP_DNS_Resolve(p->origin, DNS_TYPE_A);
    if (result == DNS_RES_NAME_IS_IPADDRESS)
    {
        IPV4_ADDR addr;

        TCPIP_Helper_StringToIPAddress(p->origin, &addr);
        if (p->socket == INVALID_SOCKET || !TCPIP_TCP_IsConnected(p->socket)) {
            p->socket = TCPIP_TCP_ClientOpen(IP_ADDRESS_TYPE_IPV4,
                    (PUBNUB_SSL && p->use_ssl) ? 443 : 80,
                    (IP_MULTI_ADDRESS*)&addr);
            if (p->socket == INVALID_SOCKET)
                return false;
            p->state = PS_CONNECT;
        } else {
            p->state = PS_HTTPREQUEST;
        }
    }
    else if (result != DNS_RES_OK) {
        return false;
    }
    else {
        p->state = PS_WAIT_DNS;
    }
    p->timer = SYS_TMR_TickCountGet();
    p->http_substate = 0;
    p->http_code = 0;
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
    int http_code = p->http_code;
    p->http_code = 0;

    char *http_reply = p->http_reply;
    p->http_reply = NULL;
    void *cb = p->cb, *cbdata = p->cbdata;
    p->cb = p->cbdata = NULL;

    if (http_code > 0 && http_code / 100 != 2) {
        /* We received an error HTTP status code; force PNR_HTTP_ERROR
         * result. This can override PNR_OK or a different error
         * signaled later during parsing of the request. */
        result = PNR_HTTP_ERROR;
    }

    if (result != PNR_OK) {
        /* Disconnect and clean up. In case of PNR_OK, we keep things
         * open for the next request. */

        if (p->socket != INVALID_SOCKET) {
            TCPIP_TCP_Disconnect(p->socket);
            TCPIP_TCP_Disconnect(p->socket); // we need to reuse the socket, RST!
            p->socket = INVALID_SOCKET;
        }

#if PUBNUB_CYASSL
        if (p->ssl) {
            CyaSSL_shutdown(p->ssl);
            CyaSSL_free(p->ssl);
            p->ssl = NULL;
        }
#endif
    }

    p->internal_cb(p, result, http_code, http_reply, cb, cbdata);
}

static bool
pubnub_update_test_timeout(struct pubnub *p)
{
    if (SYS_TMR_TickCountGet() - p->timer > p->com_timeout * SYS_TMR_TickCounterFrequencyGet()) {
        pubnub_callback_and_idle(p, PNR_TIMEOUT);
        return true;
    }
    return false;
}

#if PUBNUB_SSL
static bool pubnub_update_sslconnect(struct pubnub *p)
{
#if !PUBNUB_CYASSL
    if (TCPIP_TCPSSL_StillHandshaking(p->socket)) {
        pubnub_update_test_timeout(p);
        return false;
    }
    return true;
#else
    if (!p->ssl) {
        p->ssl = CyaSSL_new(get_ssl_ctx());
        CyaSSL_SetIOReadCtx(p->ssl, &p->socket);
        CyaSSL_SetIOWriteCtx(p->ssl, &p->socket);
    }
    if (CyaSSL_connect(p->ssl) != SSL_SUCCESS) {
        int error = CyaSSL_get_error(p->ssl, 0);
        if ((error == SSL_ERROR_WANT_READ) || (error == SSL_ERROR_WANT_WRITE)) {
            /* Operation still in progress. */
            pubnub_update_test_timeout(p);
            return false;

        } else {
            pubnub_callback_and_idle(p, PNR_SSL_ERROR);
            return false;
        }
    }

    /* Connected. */
    return true;
#endif
}
#endif

static bool
pubnub_update_sendrequest(struct pubnub *p)
{
    if (!TCPIP_TCP_IsConnected(p->socket)) {
        pubnub_callback_and_idle(p, PNR_IO_ERROR);
        return false;
    } else if (pubnub_update_test_timeout(p))
        return false;

    if (p->http_substate <= 0) {
#define S_ "GET "
        if (!pubnub_tcp_writestr(p, S_, sizeof(S_)-1))
            return true;
#undef S_
        p->http_substate++;
    }
    if (p->http_substate <= 1) {
        int url_len = strlen(p->http_buf.url);
        if (!pubnub_tcp_writestr(p, p->http_buf.url, url_len))
            return true;
        p->http_substate++;
    }
    if (p->http_substate <= 2) {
#define S_ " HTTP/1.1\r\nHost: "
        if (!pubnub_tcp_writestr(p, S_, sizeof(S_)-1))
            return true;
#undef S_
        p->http_substate++;
    }
    if (p->http_substate <= 3) {
        int namelen = strlen(p->origin);
        if (!pubnub_tcp_writestr(p, p->origin, namelen))
            return true;
        p->http_substate++;
    }
    if (p->http_substate <= 4) {
#define S_ "\r\nUser-Agent: PubNub-PIC32/0.1\r\nConnection: Keep-Alive\r\n\r\n"
        if (!pubnub_tcp_writestr(p, S_, sizeof(S_)-1))
            return true;
#undef S_
        p->state = PS_HTTPREPLY;
        p->http_substate = 0;
    }
    return true;
}

static void
pubnub_update_recvreply(struct pubnub *p)
{
    /* http_substate:
     * 0 first line (status code etc.)
     * 1 plain header
     * 2 plain header, chunked encoding detected
     * 3 chunk size line
     * 4 body follows now (transient) */
    while (p->state == PS_HTTPREPLY) {
        if (p->http_buf_len == sizeof (p->http_buf.line)) {
            /* Our buffer is already full and we did not extract a line.
             * Normally, hitting a long line in the HTTP header should
             * not happen, let's just flush the buffer. */
            p->http_buf_len = 0;
        }

        int gotlen = pubnub_tcp_read(p,
                (uint8_t *) p->http_buf.line, sizeof(p->http_buf.line)-1,
                p->http_buf_len);
        if (gotlen < 0) {
            pubnub_callback_and_idle(p, PNR_IO_ERROR);
            return;
        } else if (gotlen == 0) {
            pubnub_update_test_timeout(p);
            return;
        }

        p->http_buf_len += gotlen;

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
                p->http_code = atoi(bufptr+9);
                /* We do not panic in case of non-200 code; instead,
                 * calmly load the response and only then signal
                 * an error from within pubnub_callback_and_idle(). */
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
        p->http_reply = malloc(p->http_content_length+1);
        if (p->http_buf_len > p->http_content_length) {
io_error:
            pubnub_callback_and_idle(p, PNR_IO_ERROR);
            return;
        }
        memcpy(p->http_reply, p->http_buf.line, p->http_buf_len);
    }
    if (p->http_content_length == 0) {
        p->http_reply[p->http_buf_len] = 0;
        p->state = PS_PROCESS;
        return;
    }

    while (p->http_buf_len < p->http_content_length) {
        int gotlen = pubnub_tcp_read(p,
                (uint8_t *) p->http_reply, p->http_content_length,
                p->http_buf_len);
        if (gotlen < 0) {
            goto io_error;
        } else if (gotlen == 0) {
            pubnub_update_test_timeout(p);
            if (p->state == PS_IDLE) {
                free(p->http_reply); p->http_reply = NULL;
            }
            return;
        }

        p->http_buf_len += gotlen;
    }

    if (p->http_buf_len == p->http_content_length) {
        p->http_reply[p->http_buf_len] = 0;
        p->state = PS_PROCESS;
    }
}

void
pubnub_update(struct pubnub *p)
{
    switch (p->state) {
        case PS_IDLE:
            break;

        case PS_WAIT_DNS:
        {
            IPV4_ADDR addr;
            if (DNS_RES_OK == TCPIP_DNS_IsResolved(p->origin, &addr)) {
                if (p->socket == INVALID_SOCKET || !TCPIP_TCP_IsConnected(p->socket)) {
                    p->socket = TCPIP_TCP_ClientOpen(IP_ADDRESS_TYPE_IPV4,
                            (PUBNUB_SSL && p->use_ssl) ? 443 : 80,
                            (IP_MULTI_ADDRESS*)&addr);
                    if (p->socket == INVALID_SOCKET)
                        break;
                    p->state = PS_CONNECT;
                } else {
                    p->state = PS_HTTPREQUEST;
                }
                p->timer = SYS_TMR_TickCountGet();
                p->http_substate = 0;
                p->http_code = 0;
                p->http_buf_len = 0;
            }
            break;
        }
        case PS_CONNECT:
            if (!TCPIP_TCP_IsConnected(p->socket)) {
                pubnub_update_test_timeout(p);
                break;
            }
            if (!PUBNUB_SSL && !p->use_ssl) {
                p->state = PS_HTTPREQUEST;
            }
            else {
                if (PUBNUB_SSL && !PUBNUB_CYASSL && !TCPIP_TCPSSL_ClientStart(p->socket, NULL)) {
                    break;
                }
                p->state = PS_SSLCONNECT;
            }
            break;

#if PUBNUB_SSL
        case PS_SSLCONNECT:
            if (pubnub_update_sslconnect(p)) {
                p->state = PS_HTTPREQUEST;
            }
            break;
#endif

        case PS_HTTPREQUEST:
            if (pubnub_update_sendrequest(p))
                TCPIP_TCP_Flush(p->socket);
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
pubnub_cancel(struct pubnub *p)
{
    pubnub_callback_and_idle(p, PNR_CANCELLED);
}


void
pubnub_publish_icb(struct pubnub *p, enum pubnub_res result, int http_code,
        char *reply, void *cb, void *cbdata)
{
    if (cb)
        ((pubnub_publish_cb) cb)(p, result, http_code, reply, cbdata);
    free(reply);
}

bool
pubnub_publish(struct pubnub *p, const char *channel, const char *message,
        pubnub_publish_cb cb, void *cb_data)
{
    if (p->state != PS_IDLE)
        return false;
    if (p->http_reply)
        free(p->http_reply);
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
pubnub_subscribe_icb(struct pubnub *p, enum pubnub_res result, int http_code,
        char *reply, void *cb, void *cbdata)
{
    if (result != PNR_OK) {
error:
        if (PUBNUB_MISSMSG_OK || (result == PNR_FORMAT_ERROR)) {
            /* In case of PubNub protocol error, abort an ongoing subscribe
             * and start over. This means some messages were lost, but allows
             * us to recover from bad situations, e.g. too many messages
             * queued or unexpected problem caused by a particular message. */
            strcpy(p->timetoken, "0");
        }
        if (cb)
            ((pubnub_subscribe_cb) cb)(p, result, http_code, p->channel, reply, cbdata);
        return;
    }
    int replylen = strlen(reply);
    if (reply[replylen-1] != ']' && replylen > 2)
        replylen -= 2; // XXX: this seems required by Manxiang
    if (reply[0] != '[' || reply[replylen-1] != ']'
        || reply[replylen-2] != '"') {
        result = PNR_FORMAT_ERROR;
        goto error;
    }

    /* Extract the last argument. */
    int i = extract_last_string(reply, replylen-2);
    if (i < 0) {
        result = PNR_FORMAT_ERROR;
        goto error;
    }

    /* Now, the last argument may either be a timetoken or a channel list. */
    if (reply[i-2] == '"') {
        /* It is a channel list, there is another string argument in front
         * of us. Process the channel list ... */
        p->chan_ofs = i+1;
        p->chan_end = replylen;
        int k;
        for (k = p->chan_end-1; k > p->chan_ofs; k--)
            if (reply[k] == ',')
                reply[k] = 0;

        /* ... and look for timetoken again. */
        i = extract_last_string(reply, i-2);
        if (i < 0) {
            result = PNR_FORMAT_ERROR;
            goto error;
        }

    } else {
        p->chan_ofs = 0;
        p->chan_end = 0;
    }

    /* Now, i points at
     * [[1,2,3],"5678"]
     * [[1,2,3],"5678","a,b,c"]
     *          ^-- here */

    /* Setup timetoken. */
    if (replylen-2 - (i+1) >= 64) {
        result = PNR_FORMAT_ERROR;
        goto error;
    }
    strcpy(p->timetoken, &reply[i+1]);
    reply[i-2] = 0; // terminate the [] message array (before the ]!)

    /* As a special case, restart the subscribe if we are dealing
     * with an empty reply. */
    if (i-2 == 2) {
        free(reply);
        if (!pubnub_subscribe(p, p->channel, cb, cbdata) && cb)
            ((pubnub_subscribe_cb) cb)(p, PNR_IO_ERROR, http_code, p->channel, NULL, cbdata);
        return;
    }

    p->http_reply = reply;

    /* Set up the message list - offset, length and NUL-characters splitting
     * the messages. */
    p->msg_ofs = 2;
    p->msg_end = i-2;
    if (!split_array(&p->http_reply[p->msg_ofs], p->msg_end - p->msg_ofs)) {
        p->http_reply = NULL;
        if (cb)
            ((pubnub_subscribe_cb) cb)(p, PNR_FORMAT_ERROR, http_code, p->channel, reply, cbdata);
        free(reply);
    }

    /* Return the first message. */
    if (cb)
        ((pubnub_subscribe_cb) cb)(p, PNR_OK, http_code,
            p->chan_ofs ? &p->http_reply[p->chan_ofs] : p->channel,
            &p->http_reply[p->msg_ofs], cbdata);
}

bool
pubnub_subscribe(struct pubnub *p, const char *channel,
        pubnub_subscribe_cb cb, void *cb_data)
{
    if (p->state != PS_IDLE)
        return false;

    if (p->http_reply) {
        int prevlen = strlen(&p->http_reply[p->msg_ofs]) + 1;
        p->msg_ofs += prevlen;
        if (p->msg_ofs < p->msg_end) {
            /* Next message from stash-away buffer. */

            char *msgchan = "";
            int prevchlen = strlen(&p->http_reply[p->chan_ofs]) + 1;
            if (p->chan_ofs > 0 && p->chan_ofs < p->chan_end) {
                p->chan_ofs += prevchlen;
                if (p->chan_ofs < p->chan_end)
                    msgchan = &p->http_reply[p->chan_ofs];
            }

            if (cb)
                cb(p, PNR_OK, 0,
                    p->chan_ofs ? msgchan : p->channel,
                    &p->http_reply[p->msg_ofs], cb_data);
            return true;
        }
        /* That's all. free() below and fetch new messages. */
    }

    if (p->http_reply)
        free(p->http_reply);
    p->http_reply = NULL;
    p->http_buf_len = 0;

    p->http_content_length = 0;

    p->http_buf_len = snprintf(p->http_buf.url, sizeof(p->http_buf.url),
            "/subscribe/%s/%s/0/%s?" "%s%s" "%s%s%s" "&pnsdk=PubNub-PIC32-%s%%2F%s",
            p->subscribe_key, channel, p->timetoken,
            p->uuid ? "uuid=" : "", p->uuid ? p->uuid : "",
            p->uuid && p->auth ? "&" : "",
            p->auth ? "auth=" : "", p->auth ? p->auth : "",
            "Harmony", "1.1"
            );

    p->cb = cb; p->cbdata = cb_data;
    p->internal_cb = pubnub_subscribe_icb;
    p->channel = channel;
    p->com_timeout = p->sub_timeout;
    return pubnub_http_connect(p);
}


void
pubnub_leave_icb(struct pubnub *p, enum pubnub_res result, int http_code,
        char *reply, void *cb, void *cbdata)
{
    if (cb)
        ((pubnub_leave_cb) cb)(p, result, http_code, reply, cbdata);
    free(reply);
}

bool
pubnub_leave(struct pubnub *p, const char *channel,
        pubnub_leave_cb cb, void *cb_data)
{
    if (p->state != PS_IDLE)
        return false;
    if (p->http_reply)
        free(p->http_reply);
    p->http_reply = NULL;
    p->http_content_length = 0;

    /* Make sure next subscribe() will be a join. */
    strcpy(p->timetoken, "0");

    p->http_buf_len = snprintf(p->http_buf.url, sizeof(p->http_buf.url),
            "/v2/presence/sub-key/%s/channel/%s/leave?" "%s%s" "%s%s%s",
            p->subscribe_key, channel,
            p->uuid ? "uuid=" : "", p->uuid ? p->uuid : "",
            p->uuid && p->auth ? "&" : "",
            p->auth ? "auth=" : "", p->auth ? p->auth : "");

    p->cb = cb; p->cbdata = cb_data;
    p->internal_cb = pubnub_leave_icb;
    p->channel = channel;
    p->com_timeout = p->timeout;
    return pubnub_http_connect(p);
}
