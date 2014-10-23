#include "pubnub.h"
#include "pubnubStaticDemo.h"
#include "pubnubStatic.h"

static const char pubkey[] = "demo";
static const char subkey[] = "demo";
static const char channel[] = "hello_world";

static uint32_t pubTimer;


static void
set_led(int n, int s)
{
    switch (n) {
        case 0: BSP_LEDStateSet(BSP_LED_1, s ? BSP_LED_STATE_ON : BSP_LED_STATE_OFF); break;
        case 1: BSP_LEDStateSet(BSP_LED_2, s ? BSP_LED_STATE_ON : BSP_LED_STATE_OFF); break;
        case 2: BSP_LEDStateSet(BSP_LED_3, s ? BSP_LED_STATE_ON : BSP_LED_STATE_OFF); break;
    }
}


void PubnubStaticDemoInit(void)
{
    PubnubStaticInit(pubkey, subkey, channel, channel);
    pubTimer = 1; // ASAP
}

void PubnubStaticDemoProcess(void)
{
    char const *inmsg;

    if (pubTimer == 0) {
        return;
    }
    PubnubStaticProcess();

    /* Publish a ping message every 10 seconds. */
    if ((pubTimer < SYS_TMR_TickCountGet()) && (PubnubStaticPublishf("%s", "{\"ping, Mile, ping\":1}") == 0)) {
        pubTimer = SYS_TMR_TickCountGet() + 10*SYS_TMR_TickCounterFrequencyGet();
    }

    /* Process any received message. */
    inmsg = PubnubStaticGetMsg();
    if (inmsg != NULL) {
        int ledno, ledval;
        if (sscanf(inmsg, "{\"led\":{\"%d\":%d}}", &ledno, &ledval) == 2) {
            /* Switch the given LED. */
            set_led(ledno, ledval);
        }
        PubnubStaticMarkMsgRead();
    }
}
