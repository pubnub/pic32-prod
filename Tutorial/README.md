
### Introduction

At PubNub, we are always integrating our realtime software with new hardware, and this time it is with Microchip Technology. The number of platforms we support only keep increasing everyday, and we are super excited to announce this latest hardware addition.   


### What is Microchip Technology, what is PubNub?

[Microchip Technology](http://www.microchip.com) is an American manufacturer of microcontroller, memory and analog semiconductors. Its products include microcontrollers and we are going to be focussing on the PIC32 series. 

[PubNub](http://www.pubnub.com) on the other hand is a global datastream network that lets you build and scale realtime applications for IoT, Mobile and web. 

By running PubNub on Microchip Technology's MCUs, you can build realtime cloud connected embedded systems such as remote control of devices, status updates from sensors,  home security and home automation systems.

### Combining the two

This demo shows you how to run PubNub on Microchip, in order to facilitate realtime communication between Microchip hardware and any other device, be it mobile or web. This will power IoT, allowing for bi-directional flow of data between different endpoints to and from Microchip hardware. I will show you how to control the LEDs on the [Ethernet starter Kit](http://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=DM320004) from a PC or mobile device, and to send messages from the chip to any other device as well. 


![alt text](/images/democon.png)

1. The Microchip Ethernet starter kit runs PubNub. 
3. The PubNub code running on the chip enables it to **publish** the sensor reading(for example) in real time to any other chip or device **subscribing** to the same channel as a data stream.
5. Through the [pubnub developer console](http://www.pubnub.com/console/), you can receive this stream of information from as many sensors as you like in real time. 
6. Since the board is also subscribing, you can send control messages to the board from the console as well. 
7. Using [Project Eon](http://www.pubnub.com/developers/eon/), you can also visualize the data received from the sensor in realtime through graphs and charts. 


IMAGE FOR PubNub + MICROCHIP 



Embed this - http://g.recordit.co/nr3g85PC9O.gif 



## What you will need
 

### Hardware:

 - **PIC32 Ethernet Starter Kit** - The PIC32 Ethernet Starter Kit provides the easiest and lowest cost method to experience 10/100 Ethernet development with PIC32. The specific chip used on the board is PIC32MX795F512L.

 - Standard A to mini B cable for debugger
 - Standard A to micro B cable for USB application development
 
 - Ethernet cable
 - Hardware setup needs to be as follows:
 
 	
	 ![alt text](/images/hwsetup.jpg)
 
### Software:

 - The **[Pubnub client library](https://github.com/pubnub/pic32-prod/tree/harmony)** on Github for the Microchip Harmony framework. You can download the zipped file from the [latest release](https://github.com/pubnub/pic32-prod/releases).

 - **MPLAB® X IDE** is a software program that runs on a PC (Windows®, Mac OS®, Linux®) to develop applications for Microchip microcontrollers and digital signal controllers. It is called an Integrated Development Environment (IDE), because it provides a single integrated "environment" to develop code for embedded microcontrollers. 

 - **MPLAB® Harmony** is a comprehensive, interoperable, tested software development framework for Microchip's PIC32 microcontrollers. The framework integrates both internal and 3rd party middleware, drivers, peripheral libraries and real time operating systems, simplifying and accelerating the 32-bit development process.
 
MPLAB harmony is included to the IDE to simplify the development process for the PIC32 microcontrollers.


## How to run PubNub on the board?

### Prerequisites

On your computer, install the following:

1. [MPLAB X IDE](http://www.microchip.com/mplabx) - v3.00.
2. [MPLAB Harmony](http://www.microchip.com/harmony) - v1.03.01.
3. [MPLAB XC32 C/C++ Compiler](http://www.microchip.com/xc32)

 
#### Including MPLAB Harmony Configurator within MPLAB IDE

MPLAB Harmony provides a MPLAB Harmony Configurator (MHC) MPLAB-X IDE plug-in that can be installed in MPLAB X IDE to help you create your own MPLAB Harmony applications.

To create a new MPLAB Harmony application with MHC, follow these steps:


 - Start MPLAB X IDE and select Tools -> Plugins.
 - Select the Downloaded tab and click Add Plugins..., and then navigate to the MHC com-microchip-mplab-modules-mhc.nbm plug-in file, which is located in <install-dir>/utilities/mhc, and then click Open.

![alt text](/images/step12.png)

 - Ensure that the Install check box for the plug-in is selected and click Install.




![alt text](/images/step1.3.png)

 - Follow the prompts from the installation and continue until the installation completes. (Do not be concerned if the version you're installing is signed but not trusted, simply click Continue). Once the installation has finished you can close the Plugins dialog.
 
 - To verify the installation, select Tools > Plugins and select the Installed tab. The MHC plug-in you installed should be included in the list.


###Running the PubNub PIC32 Harmony project

To add Pubnub PIC32 Harmony library to your project, you have several options:

 - Add the library project
 - Add the generated library file
 - Add the files from the library

I am going to be choosing the **first option**. In any case, you have to link against the Harmony library, because that is what Pubnub PIC32 Harmony library uses. We use the TCPIP, TCPIP_DNS, SYS_TMR, and, optionally, TCPIP_SSL modules from Harmony. 



 - Go to the [Pic32 Github repository](https://github.com/pubnub/pic32-prod/releases) and download the zip file. 

 - Unzip/copy the contents of the library package to your Harmony apps directory. 

	 - For example, on **Linux** this would be:

			`~/microchip/harmony/v1_03_01/apps`


	 - On **Windows**, this would be:

			`c:\microchip\harmony\v1_03_01\apps`

This will create following basic directory structure under apps:

```
pubnub_client
    |
    +--pubnub_pic32_client
        |
        +--lib
        +--firmware

```

The **lib** directory contains the Pubnub client Harmony (static) library. The **firmware** directory contains a sample project you can load to your MPLABX IDE and try it "out of the box".

 - Running the pubnub client directly on the MPLAB IDE:

	 - Choose menu File|Open Project

	 - In the dialog that pops up, go to directory: **c:\microchip\harmony\v1_03_01\apps\pubnub_client\pubnub_pic32_client\firmware**

	- Then just click on the **"pubnub_pic32_client.X"**. 

	- When it loads (it will take a few seconds, you can monitor the progress in the status bar at the bottom of the window).
	
	- First select the right configuration. The easiest way to do it is to go to the "command bar" (just under the menu) and click on the drop down box that is between the "Redo" and "Build Project" icons. You'll see three options:

			* pic32mx_eth_sk : choose this if you're using PIC32 Ethernet Starter Kit
			* pic32mx_eth_sk2 : choose this if you're using PIC32 Ethernet Starter Kit II
			* pic32mz_ec_sk : choose this if you're using PIC32MZ EC Starter Kit


 	- Right click on the project name - `pubnub_pic32_client`, choose properties. Make sure the options chosen are as shown in the image below.

 ![alt text](/images/properties.png)
 
 	- In order to receive and send messages through the PubNub network, you will need to set your publish subscribe keys in the program.
 	
 ![alt text](/images/staticparam.png)

 - You should be all set up. Now choose menu Run|Run Project. It will build and then upload the firmware to the board and then reset it (so that FW starts executing). You can monitor this in the "Output" window, below the code editor window.
 
 ![alt text](/images/run.png)
 
####Static vs Dynamic API

The PubNub Harmony library has two APIs - **static and dynamic**. 

* simple / static for low traffic / low communication complexity setups. This example performs a continuous publish and subscribe.* complex / dynamic for all other setups. The dynamic example performs a single publish and a continuous subscribe.
By default, this demo runs the static version. But changing it from static to dynamic is as simple as linking  pubnubDemo.c instead of pubnubStaticDemo.c and change calls from PubnubStaticDemoInit() to PubnubDemoInit() and from PubnubStaticDemoProcess() to PubnubDemoProcess().

### Output


In order to see the output of this demo, open the [PubNub Developer Console](http://www.pubnub.com/console/), put in the **same** channel and the pub/sub keys as that in the code running in the IDE.

**Note:** You will find the channel and pub/sub keys in `pubnubStaticDemo.c` or `pubnubDemo.c` depending on whether you use the static or dynamic version. 

![alt text](/images/parameter.png)


####PubNub Publish

If all went well, then after a few seconds, you will see constant messages from the MCU. 

![alt text](/images/publish.png)

The MCU publishes messages (purposely set to do every few seconds), which is received by the console. Publish is broadcasting a message onto a specific channel. Options contains channel name, message, and callback values.

This is **PubNub publish** from the board to any other device.

####PubNub Subscribe

You may send the following message:

```
{"led":{"2":1}}
```

to turn the LED number 2 ON. 

So, in general, the first number (quoted) is the number of the LED (on most boards there are LEDS 0 - 2), the second is 0 to turn OFF, or 1 to turn ON. So this should blink the LED 1:

```
{"led":{"1", 0}}
{"led":{"1", 1}}
{"led":{"1", 0}}
{"led":{"1", 1}}
```

This is the board **subscribing** to any other device.

####Complete Output

![alt text](/images/console.png)

As you can see, I have set the channel, pub and sub keys. The messages box shows me the messages I have sent, History gives me the messages sent and received and sent on the `channel` that I have set. Using the message box, I am able to send messages to the MCU to turn on and off the LEDs on the board itself.

###What next?

Being able to talk to your mobile and web devices from your hardware hasn't been this easy. PubNub runs on  70+ SDKs, letting you choose from a wide variety of platforms to communicate with the Microchip hardware. Many companies are using PubNub to solve their IoT use cases like home automation, taxi dispatch services, logistics, fleet management and device control.  All these companies focussed on building what they do best, and left the realtime communication infrastructure maintenance to PubNub. 

















