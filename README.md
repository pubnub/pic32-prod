PubNub PIC32 Library
====================

To add PubNub support to your PIC application that uses the PIC TCP/IP stack:

  * Copy over the ``*.c`` and the ``Include/*.h`` files to your project.
    Add these files to the "Source" and "Headers" sets respectively in the
    MPLABX IDE.

  * In **Include/TCPIPConfig.h**, find the Application Options section (near
    the top) and at the *end* of this section, add:

	#define STACK_USE_PUBNUB                        // PubNub client

	#ifdef STACK_USE_PUBNUB
	#include "pubnubConfig.h"
	#endif

  * In the same file, find the definition of TCPSocketInitializer and at the
    end of the initializer (before the closing bracket), insert a line

	PUBNUB_SOCKET_INITIALIZER

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
or the PIC32 Ethernet Kit Demo:

  * Follow the library setup instructions above:

  * At the top of your main file (**MainDemo.c** in Microchip's "Demo App"),
    after the ``#include "TCPIP Stack/TCPIP.h"`` line, add this code:

	#if defined(STACK_USE_PUBNUB)
	#include "pubnubDemo.h"
	#endif

  * To the late initialization stage of your main() function, e.g. after
    the ``StackInit();`` line, add this code:

	#if defined(STACK_USE_PUBNUB)
	    PubnubDemoInit();
	#endif

  * To your main loop, e.g. after the ``StackApplications();`` line, add:

	#if defined(STACK_USE_PUBNUB)
	       PubnubDemoProcess();
	#endif

  * Build and upload. If LED 1 or 2 is lit, there was an error, look at
    UART output. Otherwise, use demo/demo keys and watch the hello_world
    channel - you should see a welcome message when the device starts
    and you can send a message like

	{"led":{0:1}}
	{"led":{0:0}}

to turn on the LED 0 and then turn it off again.

You can then use pubnubDemo.c as a base for your own usage of the PubNub
cloud. Enjoy!
