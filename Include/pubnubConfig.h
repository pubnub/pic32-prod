#ifndef PUBNUB_CONFIG__H
#define	PUBNUB_CONFIG__H

#ifndef STACK_USE_TCP
#define STACK_USE_TCP
#endif
#ifndef STACK_USE_DNS
#define STACK_USE_DNS
#endif
#ifndef STACK_CLIENT_MODE
#define STACK_CLIENT_MODE
#endif
#ifndef TCP_PURPOSE_PUBNUB_CLIENT
#define TCP_PURPOSE_PUBNUB_CLIENT TCP_PURPOSE_DEFAULT /* 13 XXX */
#endif
#define PUBNUB_SOCKET_INITIALIZER {TCP_PURPOSE_PUBNUB_CLIENT, TCP_PIC_RAM, 256, 256},

#endif	/* PUBNUB_CONFIG__H */
