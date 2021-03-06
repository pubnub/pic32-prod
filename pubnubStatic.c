#include "HardwareProfile.h"
#include "TCPIPConfig.h"

#include "pubnub.h"
#include "pubnubStatic.h"

char pubMsgBuf[256];
bool bPublish;
static bool bPublishOn;

char subMsgBuf[256];
bool bSubscribe;
static bool bSubscribeOn;
static bool bLeave;

static struct pubnub pCtx, sCtx;
static char pubChan[64], subChan[64];
static DWORD pubTimer, subTimer;


#define START_DELAY 10
#define RETRY_DELAY 2 // TODO: Exponential backoff


/* Print error() to UART. */
static void
error(int led, const char *ctx, enum pubnub_res result)
{
    /* You may want to also comment out the following completely to save
     * memory, or implement your own error reporting mechanism. */
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
}


static void
publish_cb(struct pubnub *p, enum pubnub_res result, int http_code,
        char *response, void *cb_data)
{
    if (result != PNR_OK) {
        /* TODO: Error reporting, let user decide whether to reply?
         * (incl. http_code). */
        error(1, "publish", result);
        // We do not enforce a cool-down as we do not actually
        // retry the publish ourselves?
        // pubTimer = TickGet() + RETRY_DELAY * TICK_SECOND;
    }
    bPublish = bPublishOn = false;
}

static void
subscribe_cb(struct pubnub *p, enum pubnub_res result, int http_code,
        const char *channel, char *response, void *cb_data)
{
    bSubscribeOn = false;
    if (result == PNR_TIMEOUT || result == PNR_CANCELLED)
        return;
    if (result != PNR_OK) {
        /* TODO: Error reporting, let user decide whether to reply?
         * (incl. http_code). */
        error(2, "subscribe", result);
        subTimer = TickGet() + RETRY_DELAY * TICK_SECOND;
        return;
    }

    /* Truncate if response is too long. */
    snprintf(subMsgBuf, sizeof(subMsgBuf), "%s", response);
    bSubscribe = true;
}

static void
leave_cb(struct pubnub *p, enum pubnub_res result, int http_code,
        char *response, void *cb_data)
{
    bLeave = false;
    /* The result does not matter. */
    /* We will automatically subscribe now. */
}


void
PubnubStaticProcPublish(struct pubnub *pn)
{
    pubnub_update(pn);

    if (pubTimer != 0 && pubTimer > TickGet())
        return; // cool-down
    pubTimer = 0;

    if (bPublish == false || bPublishOn == true)
        return; // nothing new to do

    if (!pubnub_publish(pn, pubChan, pubMsgBuf, publish_cb, NULL)) {
        error(1, "publish initial", 5);
        pubTimer = TickGet() + RETRY_DELAY * TICK_SECOND;
    }
    bPublishOn = true;
}

void
PubnubStaticProcSubscribe(struct pubnub *pn)
{
    pubnub_update(pn);

    if (subTimer != 0 && subTimer > TickGet())
        return; // cool-down
    subTimer = 0;

    if (bSubscribe == true || bSubscribeOn == true || bLeave == true)
        return; // nothing new to do

    bSubscribeOn = true;
    if (!pubnub_subscribe(pn, subChan, subscribe_cb, NULL)) {
        error(2, "subscribe initial", 5);
        subTimer = TickGet() + RETRY_DELAY * TICK_SECOND;
    }
}


void
PubnubStaticInit(const char *pubkey, const char *subkey,
                 const char *pubChan_, const char *subChan_)
{
    pubnub_init(&pCtx, pubkey, subkey);
    pubnub_init(&sCtx, pubkey, subkey);
    strncpy(pubChan, subChan_, sizeof(pubChan));
    pubChan[sizeof(pubChan)-1] = 0;
    strncpy(subChan, subChan_, sizeof(subChan));
    subChan[sizeof(subChan)-1] = 0;

    /* We will not do a pubnub call right away as the network
     * may not be configured properly at this moment yet (e.g.
     * DHCP address configuration still ongoing etc.). We will
     * therefore perform a delayed call of PubnubStaticStart()
     * after START_DELAY seconds. */
    pubTimer = subTimer = TickGet() + START_DELAY * TICK_SECOND;
}

void PubnubStaticProcess(void)
{
    PubnubStaticProcPublish(&pCtx);
    PubnubStaticProcSubscribe(&sCtx);
}

struct pubnub *
PubnubStaticPubCtx(void)
{
	return &pCtx;
}

struct pubnub *
PubnubStaticSubCtx(void)
{
	return &sCtx;
}

void
PubnubStaticSubChan(const char *subChan_)
{
    pubnub_cancel(&sCtx);
    if (!pubnub_leave(&sCtx, subChan, leave_cb, NULL)) {
        error(2, "leave", 5);
    } else {
        bLeave = true;
    }

    /* pubnub_leave() consumes the subChan buffer immediately so we can
     * replace it now. */
    strncpy(subChan, subChan_, sizeof(subChan));
    subChan[sizeof(subChan)-1] = 0;

    /* At this point, bSubscribeOn == false, bSubscribe == false.
     * Therefore, the moment bLeave drops to false, we will resubscribe
     * again with the new subChan set. */
}
