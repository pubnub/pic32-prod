/*******************************************************************************
  System Initialization File

  File Name:
    system_init.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, configuration bits, and allocates
    any necessary global system resources, such as the systemObjects structure
    that contains the object handles to all the MPLAB Harmony module objects in
    the system.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "app.h"
#include "system_definitions.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************

/*** DEVCFG0 ***/

#pragma config DEBUG =      ON
#pragma config ICESEL =     ICS_PGx2
#pragma config PWP =        0xff
#pragma config BWP =        OFF
#pragma config CP =         OFF

/*** DEVCFG1 ***/

#pragma config FNOSC =      PRIPLL
#pragma config FSOSCEN =    ON
#pragma config IESO =       ON
#pragma config POSCMOD =    XT
#pragma config OSCIOFNC =   OFF
#pragma config FPBDIV =     DIV_1
#pragma config FCKSM =      CSDCMD
#pragma config WDTPS =      PS1048576
#pragma config FWDTEN =     OFF

/*** DEVCFG2 ***/

#pragma config FPLLIDIV =   DIV_2
#pragma config FPLLMUL =    MUL_20
#pragma config FPLLODIV =   DIV_1
#pragma config UPLLIDIV =   DIV_12
#pragma config UPLLEN =     OFF

/*** DEVCFG3 ***/

#pragma config USERID =     0xffff
#pragma config FSRSSEL =    PRIORITY_7
#pragma config FMIIEN =     OFF
#pragma config FETHIO =     OFF
#pragma config FCANIO =     OFF
#pragma config FUSBIDIO =   OFF
#pragma config FVBUSONIO =  OFF


// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************/



// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************


/*** TMR Driver Initialization Data ***/

const DRV_TMR_INIT drvTmr0InitData =
{
    .moduleInit.sys.powerState = DRV_TMR_POWER_STATE_IDX0,
    .tmrId = DRV_TMR_PERIPHERAL_ID_IDX0,
    .clockSource = DRV_TMR_CLOCK_SOURCE_IDX0, 
    .prescale = DRV_TMR_PRESCALE_IDX0,
    .mode = DRV_TMR_OPERATION_MODE_IDX0,
    .interruptSource = DRV_TMR_INTERRUPT_SOURCE_IDX0,
    .asyncWriteEnable = false,
};

/*** TMR Service Initialization Data ***/
const SYS_TMR_INIT sysTmrInitData =
{
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},
    .drvIndex = DRV_TMR_INDEX_0,
    .tmrFreq = 1000, 
};





// *****************************************************************************
// *****************************************************************************
// Section: TCPIP Data
// *****************************************************************************
// *****************************************************************************


/*** ARP Service Initialization Data ***/
const TCPIP_ARP_MODULE_CONFIG tcpipARPInitData =
{ 
    .cacheEntries       = 5, 
    .deleteOld          = true, 
    .entrySolvedTmo     = 1200, 
    .entryPendingTmo    = 60, 
    .entryRetryTmo      = 2, 
    .permQuota          = 50, 
    .purgeThres         = 75, 
    .purgeQuanta        = 1, 
    .retries            = 3, 
    .gratProbeCount     = 1,
};







/*** UDP Sockets Initialization Data ***/
const TCPIP_UDP_MODULE_CONFIG tcpipUDPInitData =
{
    .nSockets       = 10,
    .sktTxBuffSize  = 512, 
    .poolBuffers    = 4,
    .poolBufferSize = 512,
};


/*** TCP Sockets Initialization Data ***/
const TCPIP_TCP_MODULE_CONFIG tcpipTCPInitData =
{
    .nSockets       = 10,
    .sktTxBuffSize  = 512, 
    .sktRxBuffSize  = 512,
};








/*** DHCP client Initialization Data ***/
const TCPIP_DHCP_MODULE_CONFIG tcpipDHCPInitData =
{ 
    .dhcpEnable		= true,
    .dhcpTmo		= 2,
    .dhcpCliPort    	= 68,
    .dhcpSrvPort	= 67,

};







/*** NBNS Server Initialization Data ***/
const TCPIP_NBNS_MODULE_CONFIG tcpipNBNSInitData =
{ 
};
/*** ETH MAC Initialization Data ***/
const TCPIP_MODULE_MAC_PIC32INT_CONFIG tcpipMACPIC32INTInitData =
{ 
    .nTxDescriptors		= 8,
    .rxBuffSize		    	= 1536,
    .nRxDescriptors     	= 6,
    .ethFlags		    	= 0x11f,
    .phyFlags		   	= 0x10,
    .linkInitDelay		= 500,
    .phyAddress		    	= 1,
};


/*** Zeroconfig initialization data ***/
const ZCLL_MODULE_CONFIG tcpipZCLLInitData =
{
};



/*** DNS Client Initialization Data ***/
const TCPIP_DNS_CLIENT_MODULE_CONFIG tcpipDNSClientInitData =
{ 
    .deleteOldLease			= true,
    .cacheEntries			= 5,
    .entrySolvedTmo			= 0,    
    .IPv4EntriesPerDNSName 	= 5,
	.dnsIpAddressType 		= IP_ADDRESS_TYPE_IPV4,
};



const TCPIP_NETWORK_CONFIG __attribute__((unused))  TCPIP_HOSTS_CONFIGURATION[] =
{
/*** Network Configuration Index 0 ***/
    {
        TCPIP_NETWORK_DEFAULT_INTERFACE_NAME,       // interface
        TCPIP_NETWORK_DEFAULT_HOST_NAME,            // hostName
        TCPIP_NETWORK_DEFAULT_MAC_ADDR,             // macAddr
        TCPIP_NETWORK_DEFAULT_IP_ADDRESS,           // ipAddr
        TCPIP_NETWORK_DEFAULT_IP_MASK,              // ipMask
        TCPIP_NETWORK_DEFAULT_GATEWAY,              // gateway
        TCPIP_NETWORK_DEFAULT_DNS,                  // priDNS
        TCPIP_NETWORK_DEFAULT_SECOND_DNS,           // secondDNS
        TCPIP_NETWORK_DEFAULT_POWER_MODE,           // powerMode
        TCPIP_NETWORK_DEFAULT_INTERFACE_FLAGS,      // startFlags
    },
};

const TCPIP_STACK_MODULE_CONFIG TCPIP_STACK_MODULE_CONFIG_TBL [] =
{
    {TCPIP_MODULE_IPV4,          0},
    {TCPIP_MODULE_ICMP,          0},                           // TCPIP_MODULE_ICMP
    {TCPIP_MODULE_ARP,           &tcpipARPInitData},              // TCPIP_MODULE_ARP
    {TCPIP_MODULE_UDP,           &tcpipUDPInitData},              // TCPIP_MODULE_UDP,
    {TCPIP_MODULE_TCP,           &tcpipTCPInitData},              // TCPIP_MODULE_TCP,
    {TCPIP_MODULE_DHCP_CLIENT,   &tcpipDHCPInitData},             // TCPIP_MODULE_DHCP_CLIENT,
    {TCPIP_MODULE_DNS_CLIENT,&tcpipDNSClientInitData}, // TCPIP_MODULE_DNS_CLIENT,
    {TCPIP_MODULE_NBNS,          &tcpipNBNSInitData},                           // TCPIP_MODULE_NBNS

    {TCPIP_MODULE_ZCLL, 0},                                    // TCPIP_MODULE_ZCLL,

    // MAC modules
    {TCPIP_MODULE_MAC_PIC32INT, &tcpipMACPIC32INTInitData},     // TCPIP_MODULE_MAC_PIC32INT
};

/*********************************************************************
 * Function:        SYS_MODULE_OBJ TCPIP_STACK_Init()
 *
 * PreCondition:    None
 *
 * Input:
 *
 * Output:          valid system module object if Stack and its componets are initialized
 *                  SYS_MODULE_OBJ_INVALID otherwise
 *
 * Overview:        The function starts the initialization of the stack.
 *                  If an error occurs, the SYS_ERROR() is called
 *                  and the function de-initialize itself and will return false.
 *
 * Side Effects:    None
 *
 * Note:            This function must be called before any of the
 *                  stack or its component routines are used.
 *
 ********************************************************************/


SYS_MODULE_OBJ TCPIP_STACK_Init()
{
    TCPIP_STACK_INIT    tcpipInit;

    tcpipInit.moduleInit.sys.powerState = SYS_MODULE_POWER_RUN_FULL;
    tcpipInit.pNetConf = TCPIP_HOSTS_CONFIGURATION;
    tcpipInit.nNets = sizeof (TCPIP_HOSTS_CONFIGURATION) / sizeof (*TCPIP_HOSTS_CONFIGURATION);
    tcpipInit.pModConfig = TCPIP_STACK_MODULE_CONFIG_TBL;
    tcpipInit.nModules = sizeof (TCPIP_STACK_MODULE_CONFIG_TBL) / sizeof (*TCPIP_STACK_MODULE_CONFIG_TBL);

    return TCPIP_STACK_Initialize(0, &tcpipInit.moduleInit);
}

/*********************************************************************
 * Function:        static void APP_ETHMAC_PinInitialize(void)
 *
 * PreCondition:    None
 *
 * Input:	    None
 *
 * Output:          Sets the Ethernet MAC pins
 *
 * Overview:        This function sets the MAC pin configurations 
 ********************************************************************/
static void APP_ETHMAC_PinInitialize(void)
{
    //MDC
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_OUTPUT,
                                 PORT_CHANNEL_D,
                                 PORTS_BIT_POS_11);
    //MDIO
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_INPUT,
                                 PORT_CHANNEL_D,
                                 PORTS_BIT_POS_8);
    //TXEN
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_OUTPUT,
                                 PORT_CHANNEL_A,
                                 PORTS_BIT_POS_15);
    //TXD0
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_OUTPUT,
                                 PORT_CHANNEL_D,
                                 PORTS_BIT_POS_14);
    //TXD1
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_OUTPUT,
                                 PORT_CHANNEL_D,
                                 PORTS_BIT_POS_15);
    //RXCLK
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_INPUT,
                                 PORT_CHANNEL_G,
                                 PORTS_BIT_POS_9);
    //RXDV
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_INPUT,
                                 PORT_CHANNEL_G,
                                 PORTS_BIT_POS_8);
    //RXD0
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_INPUT,
                                 PORT_CHANNEL_E,
                                 PORTS_BIT_POS_8);
    //RXD1
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_INPUT,
                                 PORT_CHANNEL_E,
                                 PORTS_BIT_POS_9);

    //RXERR
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_INPUT,
                                 PORT_CHANNEL_G,
                                 PORTS_BIT_POS_15);
}


// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************

/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Module Initialization Data
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* System Clock Initialization Data
*/

const SYS_CLK_INIT sysClkInit =
{
    .moduleInit = {0},
    .systemClockSource = SYS_CLK_SOURCE,
    .systemClockFrequencyHz = SYS_CLK_FREQ,
    .waitTillComplete = true,
    .secondaryOscKeepEnabled = true,
    .onWaitInstruction = SYS_CLK_ON_WAIT,
};


/*** System Device Control Initialization Data ***/

const SYS_DEVCON_INIT sysDevconInit =
{
    .moduleInit = {0},
};


// *****************************************************************************
// *****************************************************************************
// Section: Static Initialization Functions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Initialize ( SYS_INIT_DATA *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
    See prototype in system/common/sys_module.h.
 */

void SYS_Initialize ( void* data )
{
    /* Core Processor Initialization */
    SYS_CLK_Initialize(&sysClkInit);
    sysObj.sysDevcon = SYS_DEVCON_Initialize(SYS_DEVCON_INDEX_0, (SYS_MODULE_INIT*)&sysDevconInit);
    SYS_DEVCON_PerformanceConfig(SYS_DEVCON_SYSTEM_CLOCK);

    /* Board Support Package Initialization */
    BSP_Initialize();

    /* System Services Initialization */    
    SYS_INT_Initialize();  

    /* Initialize Drivers */
    sysObj.drvTmr0 = DRV_TMR_Initialize(DRV_TMR_INDEX_0, (SYS_MODULE_INIT *)&drvTmr0InitData);

    SYS_INT_VectorPrioritySet(INT_VECTOR_T1, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_T1, INT_SUBPRIORITY_LEVEL0);
 
 
    /* Initialize System Services */

/*** TMR Service Initialization Code ***/

    sysObj.sysTmr  = SYS_TMR_Initialize(SYS_TMR_INDEX_0, (const SYS_MODULE_INIT  * const)&sysTmrInitData);



    SYS_RANDOM_Initialize(0, 0);

    /* Initialize Middleware */

    /* MAC Pin Initialize Call */
    APP_ETHMAC_PinInitialize();
    
    /* set priority for ETHERNET interrupt source */
    SYS_INT_VectorPrioritySet(INT_VECTOR_ETH, INT_PRIORITY_LEVEL5);

    /* set sub-priority for ETHERNET interrupt source */
    SYS_INT_VectorSubprioritySet(INT_VECTOR_ETH, INT_SUBPRIORITY_LEVEL3);
    
    if(!_SYS_CONSOLE_INIT(SYS_CONSOLE_PORT))
    {
        return;
    }

    if (!_SYS_DEBUG_INIT(SYS_DEBUG_PORT))
    {
        return;
    }

    if (!SYS_COMMAND_INIT())
    {
        return;
    }

    /* TCPIP Stack Initialization */
    sysObj.tcpip = TCPIP_STACK_Init();
    if (sysObj.tcpip == SYS_MODULE_OBJ_INVALID)
    {
       return;
    }


    /* Enable Global Interrupts */
    SYS_INT_Enable();

    /* Initialize the Application */
    APP_Initialize();
}

/*******************************************************************************
 End of File
*/

