#ifndef PUBNUB__H
#define	PUBNUB__H

#include <stdbool.h>

#include <tcpip/tcpip.h>

/* Maximum length of the HTTP buffer. This is a major component of the
 * memory size of the whole pubnub context, but it is also an upper
 * bound on URL-encoded form of published message, so if you need to
 * construct big messages, you may need to raise this.  */
#ifndef PUBNUB_BUF_MAXLEN
#define PUBNUB_BUF_MAXLEN 256
#endif

/* Maximum length of the HTTP reply. This buffer is dynamically allocated
 * only when loading the reply from the network. */
/* XXX: Replies of API calls longer than this will be discarded and instead,
 * PNR_FORMAT_ERROR will be reported. Specifically, this may cause lost
 * messages returned by subscribe if too many too large messages got queued. */
#ifndef PUBNUB_REPLY_MAXLEN
#define PUBNUB_REPLY_MAXLEN (4096-8)
#endif

/* If #defined, the PubNub implementation will not try to catch-up on
 * messages it could miss while subscribe() failed with an IO error or such.
 * Use this if missing some messages is not a problem. */
/* Note that messages may be sometimes lost anyway due to potential
 * PUBNUB_REPLY_MAXLEN overrun described above. */
#define PUBNUB_MISSMSG_OK 1

/* If #defined to anything but 0, the PubNub implementation will support SSL
 * (https) connections*/
#define PUBNUB_SSL 0

/* If #defined to anything but 0, instead of the Harmony TCP/IP, we'll be using
 * the CyaSSL (provided as "third-party" in Harmony).
 * The *.pubnub.com server certificate is currently
 * hardcoded in pubnub.c; a more future-proof SSL certificate verification
 * is still a TODO. */
//#define PUBNUB_CYASSL 1

#if !defined(PUBNUB_SSL) && PUBNUB_CYASSL
#define PUBNUB_SSL 1
#endif

#if PUBNUB_CYASSL
#include <cyassl/cyassl.h>
#endif

/* The PubNub library is designed for cooperative multi-tasking as is the
 * standard in the PIC32 environment. You can have multiple pubnub contexts
 * established; in each context, up to one API call may be ongoing (typically
 * one "publish" or one "subscribe").
 *
 * The call is initiated by a function call as described below, but is not
 * executed in a blocking way; instead, you specify a callback function that
 * is called when the callback finishes, i.e. new messages are received on
 * the channel where you are subscribed.
 *
 * To facilitate cooperative multitasking, please call pubnub_update() for
 * each of your contexts in your main loop. */

/* A typical configuration may consist of a single pubnub context for
 * channel subscription and another pubnub context that will periodically
 * publish messages about device status (with timeout lower than message
 * generation frequency).
 *
 * Another typical setup may have a single subscription context and
 * create (or maintain a fixed-size pool of) a context for each publish
 * call triggered by an external event (e.g. a button push). */

/* A pubnub context. */
struct pubnub;

/* Result codes for PubNub methods. */
enum pubnub_res {
    /* Success. */
    PNR_OK,
    /* Time out before the request has completed. */
    PNR_TIMEOUT,
    /* Communication error (network or HTTP response format). */
    PNR_IO_ERROR,
    /* HTTP error. */
    PNR_HTTP_ERROR,
    /* Unexpected input in received JSON. */
    PNR_FORMAT_ERROR,
    /* Request cancelled by user. */
    PNR_CANCELLED,
    /* SSL error. */
    PNR_SSL_ERROR,
};


/** Pubnub Context Methods */

/** Lifetime Methods */

/* Initialize a given pubnub context. You can customize other parameters
 * of the context by the configuration function calls below.
 * 
 * Precondition: Call this after TCP initialization. */
void pubnub_init(struct pubnub *p, const char *publish_key, const char *subscribe_key);

/* Update a given pubnub context, checking for new events and maintaining
 * network communication and executing ongoing API calls. Please call this
 * function from your main loop and frequently. */
void pubnub_update(struct pubnub *p);

/* Deinitialize a given pubnub context, freeing all its associated resources.
 * Needs to be called only if you manage multiple pubnub contexts dynamically. */
void pubnub_done(struct pubnub *p);

/** Configuration Methods */

/* Set the origin server name. By default, http://pubsub.pubnub.com/
 * is used. TODO: Allow https origin. */
void pubnub_set_origin(struct pubnub *p, const char *origin);

/* Set the timeout for API requests execution (except subscribe),
 * in seconds. Default: 10s */
void pubnub_set_timeout(struct pubnub *p, int timeout);

/* Set the timeout for subscribe execution, in seconds. Default: 310s */
void pubnub_set_sub_timeout(struct pubnub *p, int timeout);

/* Set the secret key that is used for signing published messages
 * to confirm they are genuine. Using the secret key is optional. */
/* TODO */
void pubnub_set_secret_key(struct pubnub *p, const char *secret_key);

/* Set the cipher key that is used for symmetric encryption of messages
 * passed over the network (publish, subscribe, history). Using the
 * cipher key is optional. */
/* TODO */
void pubnub_set_cipher_key(struct pubnub *p, const char *cipher_key);

/* Set the UUID identification of PubNub client. Pass NULL to unset.
 * The string is not copied over. */
void pubnub_set_uuid(struct pubnub *p, const char *uuid);

/* Set the auth information of PubNub client. Pass NULL to unset.
 * The string is not copied over. */
void pubnub_set_auth(struct pubnub *p, const char *auth);

/* TODO: Also use the same error policy handling as in the C library. */

/* Cancel an ongoing API request. The callback will be called with
 * PNR_CANCELLED result. */
void pubnub_cancel(struct pubnub *p);


/** PubNub API Requests */

/* Publish the @message (in JSON format) on @channel. The response
 * will usually be just a success confirmation. Use NULL as cb if you
 * are not interested in result notification. The call itself returns
 * false on immediate error (e.g. too long message). */
typedef void (*pubnub_publish_cb)(struct pubnub *p, enum pubnub_res result,
        int http_code, char *response, void *cb_data);
bool pubnub_publish(struct pubnub *p, const char *channel, const char *message,
        pubnub_publish_cb cb, void *cb_data);

/* Subscribe to @channel. The response will be a JSON-formatted string
 * with an array that has one received message per item.
 *
 * Messages published on @channel since the last subscribe call
 * are returned.  One message is returned at a time; sometimes, retrieving
 * a single message may correspond to several HTTP requests, sometimes
 * a single request will yield many messages; a callback will always
 * be called only for a single message at a time. Usually, you would issue
 * the subscribe request in a loop, i.e. call another pubnub_subscribe()
 * from your pubnub_subscribe() callback.
 *
 * Note that the @channel pointer here must stay valid throughout the
 * subscribe call (until the callback). You can release it in the callback
 * if you need to.
 * Also note that the @channel string may contain multiple comma-separated
 * channel names.
 *
 * Also note that some of the subscribed messages may be lost when
 * calling publish() after a subscribe() on the same context or
 * subscribe() on different channels in turn on the same context.
 * But typically, you will want two separate contexts for publish and
 * subscribe anyway. If you are changing the set of channels you are
 * subscribed to, you should first call pubnub_leave() on the old set.
 *
 * The call itself returns false on immediate error. */
typedef void (*pubnub_subscribe_cb)(struct pubnub *p, enum pubnub_res result,
        int http_code, const char *channel, char *response, void *cb_data);
bool pubnub_subscribe(struct pubnub *p, const char *channel,
        pubnub_subscribe_cb cb, void *cb_data);

/* Leave the @channel. The response will usually be just a success
 * confirmation. Use NULL as cb if you are not interested in result
 * notification. The call itself returns false on immediate error. */
/* You can use p->channel as the channel parameter if you called
 * subscribe beforehand and want to leave all those channels (but don't
 * forget to pubnub_cancel() the subscribe first). */
typedef void (*pubnub_leave_cb)(struct pubnub *p, enum pubnub_res result,
        int http_code, char *response, void *cb_data);
bool pubnub_leave(struct pubnub *p, const char *channel,
        pubnub_leave_cb cb, void *cb_data);


/** Internal Definitions */
enum pubnub_state {
    PS_IDLE,
    PS_WAIT_DNS,
    PS_CONNECT,
    PS_SSLCONNECT,
    PS_HTTPREQUEST,
    PS_HTTPREPLY,
    PS_HTTPBODY,
    PS_PROCESS
};

/* The contents of struct pubnub is described here so that you do not have
 * to create contexts just on heap, but please treat the struct as a black box;
 * do not access any of these members directly! */
struct pubnub {
    /* Configuration and global state. */
    const char *publish_key, *subscribe_key;
    int timeout, sub_timeout;
    char origin[64];
    const char *uuid, *auth;
    char timetoken[64];

    /* API call state */
    void *cb, *cbdata;
    const char *channel;
    void (*internal_cb)(struct pubnub *p, enum pubnub_res result, int http_code, char *reply, void *cb, void *cbdata);
    int com_timeout;

    /* Network communication state */
    enum pubnub_state state;
    uint32_t timer;
    TCP_SOCKET socket;
    union { char url[PUBNUB_BUF_MAXLEN]; char line[PUBNUB_BUF_MAXLEN]; } http_buf;
    uint8_t http_substate; // PS_HTTP* internal state
    int http_code;
    unsigned http_buf_len;
    unsigned http_content_length;
    char *http_reply;
    /* These in-string offsets are used for yielding messages received
     * by subscribe - the beginning of last yielded message and total
     * length of message buffer, and the same for channels.
     * These values are valid only when http_reply != NULL. */
    unsigned short msg_ofs, msg_end, chan_ofs, chan_end;

    /* SSL state */
    uint8_t use_ssl;
#if PUBNUB_CYASSL
    CyaSSL *ssl;
#endif
};

#endif	/* PUBNUB__H */
