#include "HardwareProfile.h"
#include "TCPIPConfig.h"

#include "pubnub.h"
#include "pubnubDemo.h"

static const char pubkey[] = "demo";
static const char subkey[] = "demo";
static const char channel[] = "hello_world";

static struct pubnub pn;

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
    };
    putrsUART("\r\nPubNubDemo error ");
    putrsUART(ctx);
    putrsUART(": ");
    putrsUART(errmsg[result]);
    putrsUART("\r\n");
#endif

    LED0_IO ^= 1<<led;
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
        error(2, "subscribe initial", 0);
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
        LED0_IO &= ~(1 << ledno);
        LED0_IO |= (ledval&1) << ledno;
    }

subscribe_again:
    pubnub_subscribe(&pn, channel, subscribe_cb, NULL);
}


void
PubnubDemoInit(void)
{
    pubnub_init(&pn, pubkey, subkey);

    if (!pubnub_publish(&pn, channel, "\"\\\"Hello world!\\\" she said from the PIC.\"", publish_cb, NULL))
        error(1, "publish initial", 0);
}

void PubnubDemoProcess(void)
{
    pubnub_update(&pn);
}
