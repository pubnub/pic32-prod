/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
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

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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

#include "app.h"
#include "app_commands.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback funtions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

int32_t _APP_ParseUrl(char *uri, char **host, char **path);
int8_t _APP_PumpDNS(const char * hostname, IPV4_ADDR *ipv4Addr);



// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

/* dodao istvan */
#include "pubnubDemo.h"

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    
    APP_Commands_Init();

    /* doda istvan */
    PubnubDemoInit();
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    SYS_STATUS          tcpipStat;
    const char          *netName, *netBiosName;
    static IPV4_ADDR    dwLastIP[2] = { {-1}, {-1} };
    IPV4_ADDR           ipAddr;
    TCPIP_NET_HANDLE    netH;
    int                 i, nNets;

    /* dodao istvan */
    PubnubDemoProcess();

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            tcpipStat = TCPIP_STACK_Status(sysObj.tcpip);
            if(tcpipStat < 0)
            {   // some error occurred
                SYS_CONSOLE_MESSAGE(" APP: TCP/IP stack initialization failed!\r\n");
                appData.state = APP_TCPIP_ERROR;
            }
            else if(tcpipStat == SYS_STATUS_READY)
            {
                // now that the stack is ready we can check the
                // available interfaces
                nNets = TCPIP_STACK_NumberOfNetworksGet();
                for(i = 0; i < nNets; i++)
                {

                    netH = TCPIP_STACK_IndexToNet(i);
                    netName = TCPIP_STACK_NetNameGet(netH);
                    netBiosName = TCPIP_STACK_NetBIOSName(netH);

#if defined(TCPIP_STACK_USE_NBNS)
                    SYS_CONSOLE_PRINT("    Interface %s on host %s - NBNS enabled\r\n", netName, netBiosName);
#else
                    SYS_CONSOLE_PRINT("    Interface %s on host %s - NBNS disabled\r\n", netName, netBiosName);
#endif  // defined(TCPIP_STACK_USE_NBNS)

                }
                appData.state = APP_TCPIP_WAIT_FOR_IP;

            }
            break;
        }
        case APP_TCPIP_WAIT_FOR_IP:

            // if the IP address of an interface has changed
            // display the new value on the system console
            nNets = TCPIP_STACK_NumberOfNetworksGet();

            for (i = 0; i < nNets; i++)
            {
                netH = TCPIP_STACK_IndexToNet(i);
                ipAddr.Val = TCPIP_STACK_NetAddress(netH);
                if(dwLastIP[i].Val != ipAddr.Val)
                {
                    dwLastIP[i].Val = ipAddr.Val;

                    SYS_CONSOLE_MESSAGE(TCPIP_STACK_NetNameGet(netH));
                    SYS_CONSOLE_MESSAGE(" IP Address: ");
                    SYS_CONSOLE_PRINT("%d.%d.%d.%d \r\n", ipAddr.v[0], ipAddr.v[1], ipAddr.v[2], ipAddr.v[3]);
                    if (ipAddr.v[0] != 0 && ipAddr.v[0] != 169) // Wait for a Valid IP
                    {
                        appData.state = APP_TCPIP_WAITING_FOR_COMMAND;
                        APP_URL_Buffer[0] = '/';
                        APP_URL_Buffer[1] = '/';
                        APP_URL_Buffer[2] = 'w';
                        APP_URL_Buffer[3] = 'w';
                        APP_URL_Buffer[4] = 'w';
                        APP_URL_Buffer[5] = '.';
                        APP_URL_Buffer[6] = 'g';
                        APP_URL_Buffer[7] = 'o';
                        APP_URL_Buffer[8] = 'o';
                        APP_URL_Buffer[9] = 'g';
                        APP_URL_Buffer[10] = 'l';
                        APP_URL_Buffer[11] = 'e';
                        APP_URL_Buffer[12] = '.';
                        APP_URL_Buffer[13] = 'c';
                        APP_URL_Buffer[14] = 'o';
                        APP_URL_Buffer[15] = 'm';
                        APP_URL_Buffer[16] = '\0';
                        SYS_CONSOLE_MESSAGE("Waiting for command type: openurl <url>\r\n");
                    }
                }
            }
            break;

        case APP_TCPIP_WAITING_FOR_COMMAND:
        {
            if (APP_URL_Buffer[0] != '\0')
            {
                TCPIP_DNS_RESULT result;
                if (_APP_ParseUrl(APP_URL_Buffer, &appData.host, &appData.path))
                {
                    SYS_CONSOLE_PRINT("Could not parse URL '%s'\r\n", APP_URL_Buffer);
                    APP_URL_Buffer[0] = '\0';
                    break;
                }
                result = TCPIP_DNS_Resolve(appData.host, DNS_TYPE_A);
                if (result == DNS_RES_NAME_IS_IPADDRESS)
                {
                    IPV4_ADDR addr;
                    TCPIP_Helper_StringToIPAddress(appData.host, &addr);
                    appData.socket = TCPIP_TCP_ClientOpen(IP_ADDRESS_TYPE_IPV4,
                                                          TCPIP_HTTP_SERVER_PORT,
                                                          (IP_MULTI_ADDRESS*) &addr);
                    if (appData.socket == INVALID_SOCKET)
                    {
                        SYS_CONSOLE_MESSAGE("Could not start connection\r\n");
                        appData.state = APP_TCPIP_WAITING_FOR_COMMAND;
                    }
                    SYS_CONSOLE_MESSAGE("Starting connection\r\n");
                    appData.state = APP_TCPIP_WAIT_FOR_CONNECTION;
                    break;
                }
                if (result != DNS_RES_OK)
                {
                    SYS_CONSOLE_MESSAGE("Error in DNS aborting\r\n");
                    APP_URL_Buffer[0] = '\0';
                    break;
                }
                appData.state = APP_TCPIP_WAIT_ON_DNS;
                APP_URL_Buffer[0] = '\0';
            }
        }
        break;

        case APP_TCPIP_WAIT_ON_DNS:
        {
            IPV4_ADDR addr;
            switch (_APP_PumpDNS(appData.host, &addr))
            {
                case -1:
                {
                    // Some sort of error, already reported
                    appData.state = APP_TCPIP_WAITING_FOR_COMMAND;
                }
                break;
                case 0:
                {
                    // Still waiting
                }
                break;
                case 1:
                {
                    appData.socket = TCPIP_TCP_ClientOpen(IP_ADDRESS_TYPE_IPV4,
                                                          TCPIP_HTTP_SERVER_PORT,
                                                          (IP_MULTI_ADDRESS*) &addr);
                    if (appData.socket == INVALID_SOCKET)
                    {
                        SYS_CONSOLE_MESSAGE("Could not start connection\r\n");
                        appData.state = APP_TCPIP_WAITING_FOR_COMMAND;
                    }
                    SYS_CONSOLE_MESSAGE("Starting connection\r\n");
                    appData.state = APP_TCPIP_WAIT_FOR_CONNECTION;
                }
                break;
            }
        }
        break;

        case APP_TCPIP_WAIT_FOR_CONNECTION:
        {
            char buffer[MAX_URL_SIZE];
            if (!TCPIP_TCP_IsConnected(appData.socket))
            {
                break;
            }
            if(TCPIP_TCP_PutIsReady(appData.socket) == 0)
            {
                break;
            }
            sprintf(buffer, "GET /%s HTTP/1.1\r\n"
                    "Host: %s\r\n"
                    "Connection: close\r\n\r\n", appData.path, appData.host);
            TCPIP_TCP_ArrayPut(appData.socket, (uint8_t*)buffer, strlen(buffer));
            appData.state = APP_TCPIP_WAIT_FOR_RESPONSE;
        }
        break;

        case APP_TCPIP_WAIT_FOR_RESPONSE:
        {
            char buffer[180];
            memset(buffer, 0, sizeof(buffer));
            if (!TCPIP_TCP_IsConnected(appData.socket))
            {
                SYS_CONSOLE_MESSAGE("\r\nConnection Closed\r\n");
                appData.state = APP_TCPIP_WAITING_FOR_COMMAND;
                break;
            }
            if (TCPIP_TCP_GetIsReady(appData.socket))
            {
                TCPIP_TCP_ArrayGet(appData.socket, (uint8_t*)buffer, sizeof(buffer) - 1);
                SYS_CONSOLE_MESSAGE(buffer);
            }
        }
        break;

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}
 int32_t _APP_ParseUrl(char *uri, char **host, char **path)
{
    char * pos;
    pos = strstr(uri, "//"); //Check to see if its a proper URL

    if ( !pos )
    {
        return -1;
    }
    *host = pos + 2; // This is where the host should start

    pos = strchr( * host, '/');

    if ( !pos )
    {
        *path = NULL;
    }
    else
    {
        * pos = '\0';
        *path = pos + 1;
    }
    return 0;
}

int8_t _APP_PumpDNS(const char * hostname, IPV4_ADDR *ipv4Addr)
{
    TCPIP_DNS_RESULT result = TCPIP_DNS_IsResolved(hostname, ipv4Addr);
    switch (result)
    {
        case DNS_RES_OK:
        {
            // We now have an IPv4 Address
            // Open a socket
            return 1;
        }
        case DNS_RES_PENDING:
            return 0;
        case DNS_RES_SERVER_TMO:
        case DNS_RES_NO_ENTRY:
        default:
            SYS_CONSOLE_MESSAGE("TCPIP_DNS_IsResolved returned failure\r\n");
            return -1;
    }
    // Should not be here!
    return -1;

}

/*******************************************************************************
 End of File
 */

