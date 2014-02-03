#ifndef PUBNUB__H
#define	PUBNUB__H

#define STACK_USE_TCP
#define STACK_USE_DNS
#define STACK_CLIENT_MODE
#define TCP_PURPOSE_PUBNUB_CLIENT 13 /* TODO: TCPSocketInitializer */
#include "TCPIP Stack/TCPIP.h"

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
    /* Another method already in progress. (Will not retry.) */
    PNR_OCCUPIED,
    /* Time out before the request has completed. */
    PNR_TIMEOUT,
    /* Communication error. response is string object with the error. */
    PNR_IO_ERROR,
    /* HTTP error. response contains number object with the status code. */
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
 * in seconds. Default: 5s */
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
 * are not interested in result notification. */
typedef void (*pubnub_publish_cb)(struct pubnub *p, enum pubnub_res result,
        char *response, void *cb_data);
void pubnub_publish(struct pubnub *p, const char *channel, const char *message,
        pubnub_publish_cb cb, void *cb_data);

/* Subscribe to @channel. The response will be a JSON-formatted string
 * with an array that has one received message per item.
 *
 * Messages published on @channel since the last subscribe call
 * are returned.  The first call will typically just establish
 * the context and return immediately with an empty response array.
 * Usually, you would issue the subscribe request in a loop, i.e.
 * call another pubnub_subscribe() from your pubnub_subscribe() callback. */
typedef void (*pubnub_subscribe_cb)(struct pubnub *p, enum pubnub_res result,
        char *channel, char *response, void *cb_data);
void pubnub_subscribe(struct pubnub *p, const char *channel,
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
    char time_token[64];

    /* API call state */
    void *cb, *cbdata;
    void (*internal_cb)(struct pubnub *p, enum pubnub_res result);
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
    union { char url[256]; char line[256]; } http_buf;
    BYTE http_substate; // PS_HTTP* internal state
    int http_buf_len;
    int http_content_length;
    char *http_reply;
};

#endif	/* PUBNUB__H */