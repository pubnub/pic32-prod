#ifndef PUBNUB_STATIC__H
#define	PUBNUB_STATIC__H

/* This is an alternative interface to the pubnub library. It uses pubnub.h
 * as a backend, but the frontend are just two function calls - one to be
 * called during the firmware startup, another to be called regularly in
 * your main loop. Information is called via a few global variables. */

/* N.B. You need to have _two_ sockets of the appropriate type in your
 * TCPSocketInitializer. One will be used for publish, the other for
 * subscribe. */

/* To publish a message, store it in pubMsgBuf and set the bPublish flag.
 * The flag will be cleared once the message has been sent, you can
 * store another message in pMsgBuf at that point. */
extern char pubMsgBuf[256];
extern bool bPublish;

/* When a set of messages is received, it is stored in subMsgBuf and
 * the bSubscribe flag is set. After reading and processing the message,
 * clear the flag to allow querying for another set of messages. */
extern char subMsgBuf[256];
extern bool bSubscribe;

void PubnubStaticInit(const char *pubkey, const char *subkey, const char *pubChan, const char *subChan);
void PubnubStaticProcess(void);

#endif	/* PUBNUB_STATIC__H */
