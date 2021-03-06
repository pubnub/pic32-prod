PubNub PIC32 Library for Standalone and MPLH
====================

### Standalone
For the standalone library, use master branch

### MPLH
For MPLab Harmony, use the harmony branch

To add PubNub support to your PIC application that uses the PIC TCP/IP stack:

  * Copy over the ``*.c`` and the ``Include/*.h`` files to your project.
    Add these files to the "Source" and (that's optional) "Headers" sets
    respectively in the MPLABX IDE.

  * In **Include/TCPIPConfig.h**, find the Application Options section (near
    the top) and at the *end* of this section, add:

```c
	#define STACK_USE_PUBNUB                        // PubNub client

	#ifdef STACK_USE_PUBNUB
	#include "pubnubConfig.h"
	#endif
```
  * (OPTIONAL) In the same file, find the definition of TCPSocketInitializer
    and at the end of the initializer (before the closing bracket), insert
    a line

```c
	PUBNUB_SOCKET_INITIALIZER
```	

  * (OPTIONAL) If you want SSL support, download CyaSSL, the CyaSSL SDK
    PIC32_cyassl_example and follow the attached MPLABx setup instructions.

(multiple times if you want to use multiple PubNub context simultaneously).
If you defined a custom TCP socket type of id 13, change the id in
pubnubConfig.h. (If you aren't sure, you probably don't need to do this.)

PubNub Demo
-----------

Unfortunately, we are unable to ship a self-contained PubNub demo app you
could build and upload straight away due to Microchip licencing restrictions.
However, setting up your own demo app within the PIC32 MPLABX development
environment is a piece of cake!

We will show you how to incorporate a simple PubNub demo (pubnubDemo.c)
in an already made TCP/IP stack application.  To get started, add this to
the "Demo App" provided by Microchip either as part of their TCP/IP stack
or the PIC32 Ethernet Kit Demo ("PIC32 ESK TCPIP Demo"):

  * We assume you already have some demo app running, and the MPLABX IDE
    and compiler set up.  A nice Linux howto for setting up the PIC32
    Ethernet Kit Demo from scratch is at: http://jap.hu/electronic/pic32esk/

  * First, follow the library setup instructions above.

  * At the top of your main file (**MainDemo.c** in Microchip's "Demo App"),
    after the ``#include "TCPIP Stack/TCPIP.h"`` line, add this code:

```c
	#if defined(STACK_USE_PUBNUB)
	#include "pubnubDemo.h"
	#endif
```	

  * To the late initialization stage of your main() function, e.g. after
    the ``StackInit();`` line, add this code:

```c
	#if defined(STACK_USE_PUBNUB)
	    PubnubDemoInit();
	#endif
```	

  * To your main loop, e.g. after the ``StackApplications();`` line, add:

```c
	#if defined(STACK_USE_PUBNUB)
	       PubnubDemoProcess();
	#endif
```

  * Build and upload. If LED 1 or 2 is lit, there was an error. Otherwise,
    use demo/demo keys and watch the hello_world channel - you should see
    a welcome message when the device starts and you can send a message like

```javascript
	{"led":{"2":1}}
	{"led":{"1":1}}
	{"led":{"1":0}}
```

to turn on the LED 2, then turn on and back off LED 1. Blinking lights!

  * If there was an error, to start debugging, place a breakpoint in the
    error() function of pubnubDemo.c and go from there. Try to visit the
    website provided by the PIC32 (just access it via HTTP) to verify its
    network stack is all right.

You can then use pubnubDemo.c as a base for your own usage of the PubNub
cloud. Enjoy!

Resource Usage
--------------

What is the footprint of the PubNub interface on PIC32 devices?
(As of version @ Git commit 1f3cdbdae.)

The pure footprint of the pubnub library itself is 14680 bytes in flash
and the static RAM footprint is 252 bytes; several kilobytes of heap
RAM memory are required during network communication.  The pubnubDemo
shipped in the SDK is a typical example of a pubnub library user
(that performs a single publish and then runs a continuous subscribe);
its flash footprint is 4388 bytes, RAM footprint is 476 bytes.

An alternative API pubnubStatic makes the library simpler to use by
reducing some of its flexibility; the API itself has flash footprint
of 1736 bytes and RAM footprint 1580 bytes.  An example app based
on this API that allows continuous subscribe and continuous publishing
would take extra 3428 bytes of flash and 8 bytes of RAM.  The total
footprint of pubnubStatic-based app on top of pubnub library would
be 5164 bytes of flash and 1588 bytes of RAM.

Optionally, SSL communication is supported using the third-party
CyaSSL embedded library. Unfortunately, it comes with a big footprint
of 242592 bytes of flash (236 kilobytes!) and 4324 bytes of RAM. The
SSL layer in the pubnub library then takes 2140 bytes of flash and
16 bytes of RAM.
