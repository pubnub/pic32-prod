#ifndef PUBNUB_STATIC__H
#define	PUBNUB_STATIC__H

/* This is an alternative interface to the pubnub library. It uses pubnub.h
 * as a backend, but the frontend are just a few simple functions. It has a
 * drawback of being able to handle only one incoming and one outgoing message
 * at a time.
 */

/* N.B. You need to have _two_ sockets of the appropriate type in your
 * TCPSocketInitializer. One will be used for publish, the other for
 * subscribe. */

/** Publish the message. Only one message can be published at a time.
 * So, to publish next message, the previous one has to be sent.
 * Usage is like stdio's printf.
 * @param format The format string of the message - same as for #printf. The
 * rest of the parameters are also same as for printf.
 * @return 0: OK, -1: Busy - previous message not published yet, try later
 */
int PubnubStaticPublishf(char const *format, ...);


/** Get an incoming message, if one is received. If you get a message here,
 * when done processing it, call #PubnubStaticMarkMsgRead().
 * @return NULL: No message received; otherwise: string of the received message
 */
char const * PubnubStaticGetMsg();

/** Mark the incoming message as read. If there is no incoming message, has
 * no effect. If you don't call this after you finish processing the message, 
 * no new message will be received.
 */
void PubnubStaticMarkMsgRead();

/** Call during initialization, after all the system initializations, but
 * before client code initialization.
 */
void PubnubStaticInit(const char *pubkey, const char *subkey, const char *pubChan, const char *subChan);

/** Call from main loop, or similar place, often. Any processing is done in
 * this function.
 */
void PubnubStaticProcess(void);

/* Use these interfaces if you need to change Pub/Sub parameters,
 * e.g. the UUID. */
struct pubnub;
struct pubnub *PubnubStaticPubCtx(void);
struct pubnub *PubnubStaticSubCtx(void);

/* Use this interface to change the set of subscribed channels.
 * You can use multiple channel names delimited by a comma.
 * N.B. the list of channels may be at most 64 bytes long. */
void PubnubStaticSubChan(const char *subChan);

#endif	/* PUBNUB_STATIC__H */
