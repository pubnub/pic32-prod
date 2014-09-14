#include "pubnub.h"
#include "pubnubDemo.h"
#include "pubnubStatic.h"

static const char pubkey[] = "demo";
static const char subkey[] = "demo";
static const char channel[] = "hello_world";

static uint32_t pubTimer;


static void
set_led(int n, int s)
{
    switch (n) {
        /*
        case 0: LED0_IO = s; break;
        case 1: LED1_IO = s; break;
        case 2: LED2_IO = s; break;
         */
    }
}


void
PubnubStaticDemoInit(void)
{
    PubnubStaticInit(pubkey, subkey, channel, channel);
    pubTimer = 0; // ASAP
}

void PubnubStaticsDemoProcess(void)
{
    PubnubStaticProcess();

    /* Publish a ping message every 500 ms. */
    if (pubTimer < SYS_TMR_TickCountGet() && !bPublish) {
        strcpy(pubMsgBuf, "{\"ping\":1}");
        bPublish = true;
        pubTimer = SYS_TMR_TickCountGet() + SYS_TMR_TickCounterFrequencyGet()/2;
    }

    /* Process any received message. */
    if (bSubscribe) {
        int ledno, ledval;
        if (sscanf(subMsgBuf, "{\"led\":{\"%d\":%d}}", &ledno, &ledval) == 2) {
            /* Switch the given LED. */
            set_led(ledno, ledval);
        }
        bSubscribe = false;
    }
}
