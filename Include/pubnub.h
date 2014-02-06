#ifndef PUBNUB__H
#define	PUBNUB__H

#include <stdbool.h>

#include "TCPIP Stack/TCPIP.h"

/* Maximum length of the HTTP buffer. This is a major component of the
 * memory size of the whole pubnub context, but it is also an upper
 * bound on URL-encoded form of published message, so if you need to
 * construct big messages, you may need to raise this.  N.B. you also
 * need to appropriately raise TX buffer size in PUBNUB_SOCKET_INITIALIZER
 * of pubnubConfig.h. */
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

/* TODO: Also use the same error policy handling as in the C library. */


/** PubNub API Requests */

/* Publish the @message (in JSON format) on @channel. The response
 * will usually be just a success confirmation. Use NULL as cb if you
 * are not interested in result notification. The call itself returns
 * false on immediate error (e.g. too long message). */
typedef void (*pubnub_publish_cb)(struct pubnub *p, enum pubnub_res result,
        char *response, void *cb_data);
bool pubnub_publish(struct pubnub *p, const char *channel, const char *message,
        pubnub_publish_cb cb, void *cb_data);

/* Subscribe to @channel. The response will be a JSON-formatted string
 * with an array that has one received message per item.
 *
 * Messages published on @channel since the last subscribe call
 * are returned.  The first call will typically just establish
 * the context and return immediately with an empty response array.
 * Usually, you would issue the subscribe request in a loop, i.e.
 * call another pubnub_subscribe() from your pubnub_subscribe() callback.
 *
 * Note that the @channel pointer here must stay valid throughout the
 * subscribe call (until the callback). You can release it in the callback
 * if you need to.
 *
 * The call itself returns false on immediate error. */
typedef void (*pubnub_subscribe_cb)(struct pubnub *p, enum pubnub_res result,
        const char *channel, char *response, void *cb_data);
bool pubnub_subscribe(struct pubnub *p, const char *channel,
        pubnub_subscribe_cb cb, void *cb_data);


/** Internal Definitions */

/* The contents of struct pubnub is described here so that you do not have
 * to create contexts just on heap, but please treat the struct as a black box;
 * do not access any of these members directly! */
struct pubnub {
    /* Configuration and global state. */
    const char *publish_key, *subscribe_key;
    int timeout, sub_timeout;
    char origin[64];
    char timetoken[64];

    /* API call state */
    void *cb, *cbdata;
    const char *channel;
    void (*internal_cb)(struct pubnub *p, enum pubnub_res result, char *reply, int replylen, void *cb, void *cbdata);
    int com_timeout;

    /* Network communication state */
    enum pubnub_state {
        PS_IDLE,
        PS_CONNECT,
        PS_HTTPREQUEST,
        PS_HTTPREPLY,
        PS_HTTPBODY,
        PS_PROCESS
    } state;
    DWORD timer;
    TCP_SOCKET socket;
    union { char url[PUBNUB_BUF_MAXLEN]; char line[PUBNUB_BUF_MAXLEN]; } http_buf;
    BYTE http_substate; // PS_HTTP* internal state
    int http_buf_len;
    int http_content_length;
    char *http_reply;
};

#endif	/* PUBNUB__H */
