Pubnub PIC32 Harmony library
============================

To add Pubnub PIC32 Harmony library to your project, you have several options:

1. Add the library project
2. Add the generated library file
3. Add the files from the library

These are listed in order of preference.

To add the library project, just add it to the libraries of you App project
in MPLAB X IDE. This has the benefit of "tracking" configurations of your
app project, although it also means the library will be built for each
of the configurations (which may or may not be what you want).

To add the generated library file, build the library from the library
project or get a ready-made one, then add the library (.a) to your
libraries in the MPLAB X IDE. In this case, library will not be rebuilt
for each of your configurations. Also, you will have to add the headers
to your project yourself.

And as the last option, you may copy the files from the library
(at the moment of this writing, those are: pubnub.c pubnub.h
pubnubStatic.c pubnubStatic.h) to your project. This is similar to
option 1., but more work.

In any case, you have to link against the Harmony library, because
that is what Pubnub PIC32 Harmony library uses. We use the TCPIP,
TCPIP_DNS, SYS_TMR, and, optionally, TCPIP_SSL modules from Harmony.
Keep in mind that these modules use other Harmony modules.

The way you link against Harmony is up to you - most Harmony examples
have the source files included in the project, but you may define your
only MPLABX library projects for those files.


Configuring the library
-----------------------

The library has three possible configurations, defined by two preprocessor
symbols:

    PUBNUB_SSL
    PUBNUB_CYASSL

Refer to pubnub.h for detailed description of those. The configurations
are:

1. no SSL
2. Use Harmony SSL - at the time of this writing, this is not useful,
	as Harmony SSL only supports SSLv3, which Pubnub doesn't support
	(any more)
3. Use CyaSSL - this is the library supplied with Harmony under Third
	Party directory, is full featured, but, has a rather large
	memory footprint

But, keep in mind that if you use a pre-built library (.a - the option
2. mentioned above) it will be compiled for one of the three 
configurations, and your changes to pubnub.h and the two mentioned
symbols *will not* affect the library.

Besides these two symbols for general configuration, there are few more
"fine grained" configuration symbols, you will find them at the top of
pubnub.h.


Pubnub Demo
-----------

Depending on licensing, we may or may not be able to ship a 
self contained demo.

Here's how you can make the APP yourself:

1. Start with the TCPIP Client example from Harmony
2. Add the Pubnub in any of the 3 options mentioned above
3. Add the pubnubDemo.h pubnubDemo.c and/or pubnubStaticDemo.h pubnubStaticDemo.c
4. Replace the App.c with "our" App.c - it uses one of the interfaces, you may easily
	switch to the other
5. If you wish, you may delete AppCommands.h AppCommands.c

You may choose the pubnubDemo or pubnubStatic demo, each demonstrating the use
of the API "variant". Static is easier to use, but less flexible. Also, since it
uses the more general "dynamic" API, it uses a little more memory.

The easiest way to use the demo is to open the Web developer console on Pubnub
website. But please be sure to choose the right channel.

The message you send will be seen on the console, and you may send the following
message:

    {"led":{"2":1}}

to turn the LED number 2 ON. Of course, this expects that your board is supported
by Harmony and it has the LED number 2.

So, in general, the first number (quoted) is the number of the LED (on most boards
there are LEDS 0 - 2), the second is 0 to turn OFF, or 1 to turn ON. So this should
blink the LED 1:

    {"led":{"1", 0}}
    {"led":{"1", 1}}
    {"led":{"1", 0}}
    {"led":{"1", 1}}

Please refer to the header (and source) files for more information.


Memory footprint
----------------

Memory footprint depends on configuration, compiler options, etc.

But, this is what you may expect, at the time of this writing:

- General (dynamic) API program memory: 8KB, +0,5KB heap per (Pubnub) connection
- Static API program memory: +1 KB, +1KB static 
- Harmony SSL: +1 KB program memory
- CyaSSL: +2 KB program memory, 0.5KB static (for the certificate) + CyaSSL footprint
	(you may expect around 200KB)
