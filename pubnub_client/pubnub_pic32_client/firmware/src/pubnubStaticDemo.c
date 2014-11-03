#include "pubnub.h"
#include "pubnubStaticDemo.h"
#include "pubnubStatic.h"


static const char pubkey[] = "demo";
static const char subkey[] = "demo";
static const char channel[] = "hello_world";

static uint32_t pubTimer;


static void set_led(int n, int s)
{
    switch (n) {
    case 0: BSP_LEDStateSet(BSP_LED_1, s ? BSP_LED_STATE_ON : BSP_LED_STATE_OFF); break;
    case 1: BSP_LEDStateSet(BSP_LED_2, s ? BSP_LED_STATE_ON : BSP_LED_STATE_OFF); break;
    case 2: BSP_LEDStateSet(BSP_LED_3, s ? BSP_LED_STATE_ON : BSP_LED_STATE_OFF); break;
    }
}


/* Unfortunately, there is no (easy) way to find out how many buttons/switches
 * are there, so we shall assume 3.
 */
#define SWITCH_COUNT 3

/* Debouncing count */
#define DEBOUNCE 5

/* Info for detecting the switch state changes (events) */
struct SwitchState {
    BSP_SWITCH_STATE state;
    uint8_t count;
};

/* Switch detector states */
static struct SwitchState m_SwState[SWITCH_COUNT];

/* Detects switch events. Will return 1 if a event is detected, 0 otherwise. */
int SwitchEventDetector(struct SwitchState *p, BSP_SWITCH_STATE state)
{
    if ((state != p->state) && (++p->count > DEBOUNCE)) {
        p->state = state;
        p->count = 0;
        return 1;
    }
    return 0;
}


void PubnubStaticDemoInit(void)
{
    int i;
    for (i = 0; i < SWITCH_COUNT; ++i) {
        m_SwState[i].state = BSP_SWITCH_STATE_RELEASED;
        m_SwState[i].count = 0;
    }
    PubnubStaticInit(pubkey, subkey, channel, channel);
    pubTimer = 1; // ASAP
}


/* Helper function to map between the switch as an index and the BSP
 * symbol (enum), which need not be contiguous or start at a particular
 * value.
 */
static BSP_SWITCH BSP_SWITCH_FromIndex(int i)
{
    switch (i) {
    case 0: return BSP_SWITCH_1;
    case 1: return BSP_SWITCH_2;
    default: case 2: return BSP_SWITCH_3;
    }
}


/* Helper function to handle a single switch */
static void HandleSwitch(int i)
{
    BSP_SWITCH sw = BSP_SWITCH_FromIndex(i);
    BSP_SWITCH_STATE state = BSP_SwitchStateGet(sw);
    if (SwitchEventDetector(m_SwState + i, state)) {
        PubnubStaticPublishf("{\"switch\":%d, \"event\":\"%s\"}",
            i,
            (state == BSP_SWITCH_STATE_RELEASED) ? "released" : "pressed"
            );
    }
}


void PubnubStaticDemoProcess(void)
{
    char const *inmsg;
    static int i;

    if (pubTimer == 0) {
        return;
    }
    PubnubStaticProcess();

    /* Publish a ping message every 10 seconds. */
    if ((pubTimer < SYS_TMR_TickCountGet()) && (PubnubStaticPublishf("%s", "{\"ping, Mile, ping\":1}") == 0)) {
        pubTimer = SYS_TMR_TickCountGet() + 10*SYS_TMR_TickCounterFrequencyGet();
    }

    /* Publish button (switch) presses (and releases) */
    HandleSwitch(i);
    if (++i >= SWITCH_COUNT) {
        i = 0;
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
