#include "HardwareProfile.h"
#include "TCPIPConfig.h"

#include "pubnub.h"
#include "pubnubDemo.h"

static const char pubkey[] = "demo";
static const char subkey[] = "demo";
static const char channel[] = "hello_world";

static struct pubnub pn;

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

static void subscribe_cb(struct pubnub *p, enum pubnub_res result, const char *channel, char *response, void *cb_data);
static void
publish_cb(struct pubnub *p, enum pubnub_res result,
        char *response, void *cb_data)
{
    if (result != PNR_OK)
        error(1, "publish", result);

    /* Ok, now subscribe. */
    if (!pubnub_subscribe(&pn, channel, subscribe_cb, NULL))
        error(2, "subscribe initial", 5);
}

static void
subscribe_cb(struct pubnub *p, enum pubnub_res result,
        const char *channel, char *response, void *cb_data)
{
    if (result == PNR_TIMEOUT) {
        /* Nothing happenned, just retry to stay tuned. */
        goto subscribe_again;
    }
    if (result != PNR_OK) {
        error(2, "subscribe", result);
        return;
    }

    int ledno, ledval;
    if (sscanf(response, "{\"led\":{\"%d\":%d}}", &ledno, &ledval) == 2) {
        /* Switch the given LED. */
        set_led(ledno, ledval);
    }

subscribe_again:
    pubnub_subscribe(&pn, channel, subscribe_cb, NULL);
}


void
PubnubDemoStart(void)
{
    if (!pubnub_publish(&pn, channel, "\"\\\"Hello world!\\\" she said from the PIC.\"", publish_cb, NULL))
        error(1, "publish initial", 5);
}


#define START_DELAY 10
static DWORD timer;

void
PubnubDemoInit(void)
{
    pubnub_init(&pn, pubkey, subkey);

    /* We will not do a pubnub call right away as the network
     * may not be configured properly at this moment yet (e.g.
     * DHCP address configuration still ongoing etc.). We will
     * therefore perform a delayed call of PubnubDemoStart()
     * after START_DELAY seconds. */
    timer = TickGet();
}

void PubnubDemoProcess(void)
{
    if (timer != 0 && TickGet() - timer > START_DELAY * TICK_SECOND) {
        timer = 0;
        PubnubDemoStart();
    }

    pubnub_update(&pn);
}
