#include "HardwareProfile.h"
#include "TCPIPConfig.h"

#include "pubnub.h"
#include "pubnubDemo.h"

static const char pubkey[] = "demo";
static const char subkey[] = "demo";
static const char channel[] = "hello_world";

static struct pubnub pn;


/* We have an internal state machine - if a timer is set,
 * when it reaches completion, the action in PubnubDemoNext
 * is executed. This is used to implement delayed start and
 * delayed retry. */

void PubnubDemoStart(void);
void PubnubDemoSubscribe(void);
void PubnubDemoResubscribe(void); // delayed subscirbe

#define START_DELAY 10
#define RESUB_DELAY 2 // TODO: Exponential backoff
static DWORD timer;
static enum PubnubDemoNext {
    PDN_START,
    PDN_SUBSCRIBE,
} next;


/* Some LED utility functions. */
static void
flip_led(int n)
{
    switch (n) {
        case 0: LED0_IO ^= 1; break;
        case 1: LED1_IO ^= 1; break;
        case 2: LED2_IO ^= 1; break;
    }
}
static void
set_led(int n, int s)
{
    switch (n) {
        case 0: LED0_IO = s; break;
        case 1: LED1_IO = s; break;
        case 2: LED2_IO = s; break;
    }
}

/* Print error() to UART and switch a given LED to indicate error. */
static void
error(int led, const char *ctx, enum pubnub_res result)
{
#if defined(STACK_USE_UART)
    static const char *errmsg[] = {
        "Success.",
        "Time out before the request has completed.",
        "Communication error (network or HTTP response format).",
        "HTTP error.",
        "Unexpected input in received JSON.",
        "Initial call failed."
    };
    putrsUART("\r\nPubNubDemo error ");
    putrsUART(ctx);
    putrsUART(": ");
    putrsUART(errmsg[result+1]);
    putrsUART("\r\n");
#endif
    flip_led(led);
}


static void subscribe_cb(struct pubnub *p, enum pubnub_res result, int http_code, const char *channel, char *response, void *cb_data);
static void
publish_cb(struct pubnub *p, enum pubnub_res result, int http_code,
        char *response, void *cb_data)
{
    if (result != PNR_OK)
        error(1, "publish", result);

    /* Ok, now subscribe. */
    PubnubDemoSubscribe();
}

static void
subscribe_cb(struct pubnub *p, enum pubnub_res result, int http_code,
        const char *channel, char *response, void *cb_data)
{
    if (result == PNR_TIMEOUT) {
        /* Nothing happenned, just retry to stay tuned. */
        PubnubDemoSubscribe();
        return;
    }
    if (result != PNR_OK) {
        error(2, "subscribe", result);
        PubnubDemoResubscribe(); // delayed retry
        return;
    }

    /* XXX: The following pattern does not take into account a situation
     * when we get multiple messages at once in the [] array, which is
     * entirely possible. */
    int ledno, ledval;
    if (sscanf(response, "{\"led\":{\"%d\":%d}}", &ledno, &ledval) == 2) {
        /* Switch the given LED. */
        set_led(ledno, ledval);
    }

    PubnubDemoSubscribe();
}


void
PubnubDemoStart(void)
{
    if (!pubnub_publish(&pn, channel, "\"\\\"Hello world!\\\" she said from the PIC.\"", publish_cb, NULL))
        error(1, "publish initial", 5);
}

void
PubnubDemoSubscribe(void)
{
    if (!pubnub_subscribe(&pn, channel, subscribe_cb, NULL)) {
        error(2, "subscribe initial", 5);
        PubnubDemoResubscribe();
    }
}

void
PubnubDemoResubscribe(void)
{
    /* Set a delayed retry. */
    next = PDN_SUBSCRIBE;
    timer = TickGet() + START_DELAY * TICK_SECOND;
}


void
PubnubDemoInit(void)
{
    pubnub_init(&pn, pubkey, subkey);

    /* We will not do a pubnub call right away as the network
     * may not be configured properly at this moment yet (e.g.
     * DHCP address configuration still ongoing etc.). We will
     * therefore perform a delayed call of PubnubDemoStart()
     * after START_DELAY seconds. */
    next = PDN_START;
    timer = TickGet() + START_DELAY * TICK_SECOND;
}

void PubnubDemoProcess(void)
{
    pubnub_update(&pn);

    /* XXX: What if the timer overflows? */
    if (timer != 0 && TickGet() > timer) {
        timer = 0;
        switch (next) {
            case PDN_START: PubnubDemoStart(); break;
            case PDN_SUBSCRIBE: PubnubDemoSubscribe(); break;
        }
    }
}
