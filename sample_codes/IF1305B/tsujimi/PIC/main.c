//Microchip Solutions v2011-12-05
/******************************************************************************
            USB Custom Demo, Host

This file provides the main entry point to the Microchip USB Custom
Host demo.  This demo shows how a PIC24F system could be used to
act as the host, controlling a USB device running the Microchip Custom
Device demo.

******************************************************************************/

/******************************************************************************
* Filename:        main.c
* Dependancies:    USB Host Driver with Generic Client Driver
* Processor:       PIC24F256GB1xx
* Hardware:        Explorer 16 with USB PICtail Plus
* Compiler:        C30 v2.01/C32 v0.00.18
* Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the ìCompanyÅE for its PICmicroÆ Microcontroller is intended and
supplied to you, the Companyís customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
software is owned by the Company and/or its supplier, and is
protected under applicable copyright laws. All rights are reserved.
Any use in violation of the foregoing restrictions may subject the
user to criminal sanctions under applicable laws, as well as to
civil liability for the breach of the terms and conditions of this
license.

THIS SOFTWARE IS PROVIDED IN AN ìAS ISÅECONDITION. NO WARRANTIES,
WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.


*******************************************************************************/
//0x0004 = Attribute Protocol
//0x0005 = LE L2CAP Signaling channel
//0x0006 = Security Manager Protocol

#include <stdlib.h>
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_host_generic.h"
#include "user.h"
#include "timer.h"

//#define DEBUG_MODE


#define PROG_ADDR1 0xA700
#define PROG_ADDR2 0xA702
int addrOffset;

// *****************************************************************************
// *****************************************************************************
// Configuration Bits
// *****************************************************************************
// *****************************************************************************

        _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_NODIV & IESO_ON)
        _CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
        _CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
 
// *****************************************************************************

// Application States
typedef enum
{
    BT_INITIALIZE = 0,                // Initialize the app when a device is attached
    BT_STATE_IDLE,                    // Inactive State
    BT_STATE_PROCESS,
	BT_STATE_WRITE_CLASS,
	BT_STATE_READ_EP1,
	BT_STATE_READ_CLASS_WAITING,
	BT_STATE_WRITE_ACL,
	BT_STATE_READ_ACL_HCI,
	BT_STATE_READ_ACL_WAITING,
	BT_STATE_READ_HCI_WAITING,
	BT_STATE_READ_HCI,

    BT_STATE_ERROR                    // An error has occured
} BT_STATE;

// Hci States
typedef enum
{
	HCI_CMD_RESET = 0,                // Initialize the hci when a device is attached
		HCI_CMD_RESET_END,
	HCI_CMD_READ_BD_ADDR,
		HCI_CMD_READ_BD_ADDR_END,
	HCI_CMD_MASK,
		HCI_CMD_MASK_END,

	HCI_CMD_ADVERT_DATA,
		HCI_CMD_ADVERT_DATA_END,
	HCI_CMD_ADVERT_PARAM,
		HCI_CMD_ADVERT_PARAM_END,
	HCI_CMD_ADVERT_ENABLE,
		HCI_CMD_ADVERT_ENABLE_END,


	L2CAP_DISCONNECT_REQ,
		L2CAP_DISCONNECT_REQ_END,


	GATT_SARVICE_CHANGE,
		GATT_SARVICE_CHANGE_RESP,
	GATT_DEVICE_NAME,
		GATT_DEVICE_NAME_RESP,
	GATT_PRIMARY_SERVICE1,
		GATT_PRIMARY_SERVICE1_RESP,
	GATT_PRIMARY_SERVICE2,
		GATT_PRIMARY_SERVICE2_RESP,

	GATT_HID_CHAR1_REQ,
		GATT_HID_CHAR1_RES,
	GATT_HID_CHAR2_REQ,
		GATT_HID_CHAR2_RES,
	GATT_HID_CHAR3_REQ,
		GATT_HID_CHAR4_RES,

	GATT_HID_REPORT_REQ1,
		GATT_HID_REPORT_RES1,
	GATT_HID_REPORT_REQ2,
		GATT_HID_REPORT_RES2,
	GATT_HID_DESC_REQ1,
		GATT_HID_DESC_RES1,
	GATT_HID_DESC_REQ2,
		GATT_HID_DESC_RES2,
	GATT_HID_INFO_REQ,
		GATT_HID_INFO_RESP,
	GATT_REP_REF_REQ1,
		GATT_REP_REF_RES1,
	GATT_REP_REF_REQ2,
		GATT_REP_REF_RES2,
	GATT_HID_CCC_REQ,
		GATT_HID_CCC_RES,

	GATT_ACCESS_CHAR1_REQ,
		GATT_ACCESS_CHAR1_RES,
	GATT_ACCESS_CHAR2_REQ,
		GATT_ACCESS_CHAR2_RES,

	GATT_DEVICEE_CHAR1_REQ,
		GATT_DEVICEE_CHAR1_RES,
	GATT_DEVICEE_CHAR2_REQ,
		GATT_DEVICEE_CHAR2_RES,

	GATT_BATTERY_CHAR1_REQ,
		GATT_BATTERY_CHAR1_RES,
	GATT_BATTERY_CHAR2_REQ,
		GATT_BATTERY_CHAR2_RES,

	GATT_ACCESS_NAME_REQ,
		GATT_ACCESS_NAME_RES,
	GATT_ACCESS_ICON_REQ,
		GATT_ACCESS_ICON_RES,
	GATT_PNP_REQ,
		GATT_PNP_RES,

	GATT_BATTERY_DESC_REQ,
		GATT_BATTERY_DESC_RES,

	GATT_BATTERY_NOW_REQ,
		GATT_BATTERY_NOW_RES,

	GATT_BATTERY_CCC_REQ,
		GATT_BATTERY_CCC_RES,

	GATT_BATTERY_CCC_RES1,//FOR SECOND CONNECTION
		GATT_BATTERY_NOW_REQ1,//FOR SECOND CONNECTION
	GATT_BATTERY_NOW_RES1,//FOR SECOND CONNECTION

	GATT_CCC_REQ,//FOR SECOND CONNECTION
		GATT_CCC_RES,//FOR SECOND CONNECTION

	SMP_PAIR_REQ,
		SMP_PAIR_RESP,
	SMP_FUNC_E,
		SMP_FUNC_E_END,
	SMP_FUNC_C1,
		SMP_FUNC_C1_END,
	SMP_FUNC_S1,
		SMP_FUNC_S1_END,
	SMP_PAIR_CONF_IN,
		SMP_PAIR_CONF_OUT,
	SMP_RANDOM_IN,
		SMP_RANDOM_OUT,
	SMP_RANDOM1_IN,
		SMP_RANDOM1_OUT,
	SMP_LTK_REQ,
		SMP_LTK_RES,
		SMP_LTK_RES_END,
	SMP_LTK_OUT,
		SMP_LTK_IN,

	HID_READ_DATA,
	HID_WRITE_RES,
	HID_WRITE_DATA,
	PROG_END
} HCI_STATE;


// *****************************************************************************
// *****************************************************************************
// Global Variables
// *****************************************************************************
// *****************************************************************************

BYTE	deviceAddress;  // Address of the device on the USB
BT_STATE  DemoState;      // Current state of the demo application
HCI_STATE  HciState;      // Current state of the demo application
WORD data_size;
int data_num;

unsigned char buf[64];
unsigned char buf1[64];
unsigned char handle[2];//a handle for ACL 
unsigned char hid_flag=0;
unsigned char EP2busy_flag=0;

int step;
char message[30];
int end_num;
unsigned char ConnectSwitch;

unsigned char PAIR_CONF[16];
unsigned char PAIR_RANDOM[16]={0x94,0x58,0x7f,0x05,0xdc,0x5d,0x8f,0xe6,0xdd,0x26,0x3e,0x6d,0x2b,0x16,0xb4,0x7a};
unsigned char INIT_RANDOM[16];
unsigned char STK[16];

unsigned char INI_ADDRESS_TYPE=0,RES_ADDRESS_TYPE=0;
unsigned char INI_BT_ADDRESS[6],RES_BT_ADDRESS[6];
unsigned char PAIR_RES[7]={0x02,0x03,0x00,0x01,0x10,0x01,0x01},PAIR_REQ[7];
unsigned char P1[16],P2[16];

unsigned char EDIV_RANDOM[10]={0x3C,0x03,0x5D,0x83,0xA8,0xDF,0x97,0xE4,0xA1,0xC2};
unsigned char LTK[16]={0xA4,0x21,0x5D,0xB1,0xEE,0x4D,0x1A,0x74,0xD2,0xB8,0x47,0xCA,0xDE,0xBB,0xEE,0xE9};
unsigned char M_EDIV_RANDOM[10];
unsigned char M_LTK[16];

unsigned char HID_CHAR[6][7]=
{
{0x07,0x00,0x12,0x08,0x00,0x4d,0x2a},
{0x0b,0x00,0x0e,0x0c,0x00,0x4d,0x2a},
{0x0e,0x00,0x02,0x0f,0x00,0x4b,0x2a},
{0x10,0x00,0x02,0x11,0x00,0x4a,0x2a},
{0x12,0x00,0x04,0x13,0x00,0x4c,0x2a},
{0x08,0x13,0x00,0x0a,0xFF,0xFF,0xFF}
};

unsigned char HID_REPORT[2][22]=
{
{0x06,0x00,0xff,0x09,0x01,0xa1,0x01,0x19,0x01,0x29,0x13,0x15,0x00,0x26,0xff,
0x00,0x75,0x08,0x95,0x13,0x81,0x00},
{0x19,0x01,0x29,0x13,0x91,0x00,0xc0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};


unsigned char ACCESS_SERVICE[3][7]=
{
{0x02,0x00,0x0a,0x03,0x00,0x00,0x2a},
{0x04,0x00,0x02,0x05,0x00,0x01,0x2a},
{0x08,0x05,0x00,0x0a,0xFF,0xFF,0xFF}
};

unsigned char PNP_SERVICE[2][7]=
{
{0x19,0x00,0x02,0x1a,0x00,0x50,0x2a},
{0x08,0x1a,0x00,0x0a,0xFF,0xFF,0xFF}
};

//******************************************************************************
//******************************************************************************
// Local Routines
//******************************************************************************
//******************************************************************************

/*************************************************************************
 * Function:        InitializeSystem
 *
 * Preconditions:   None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Returns:         TRUE if successful, FALSE if not.
 *
 * Side Effects:    See below
 *
 * Overview:        This routine initializes the processor and peripheral,
 *                  setting clock speeds and enabling any required
 *                  features.
 *************************************************************************/

BOOL InitializeSystem ( void )
{

    unsigned int pll_startup_counter = 600;
    CLKDIVbits.PLLEN = 1;
    while(pll_startup_counter--);

	// Configure U2RX - put on pin 17 (RP8)
	RPINR19bits.U2RXR = 8;
	// Configure U2TX - put on pin 16 (RP7)
	RPOR3bits.RP7R = 5;
    // Init UART
    UART2Init();

	AD1PCFG = 0xFFFF; /*set AN pins to digital pin */
	TRISA=0x0003;	/* set PORTB(RA0 and RA1 pins) to input*/
					/* "0" corresponds to "o"utput and "1" to "i"nput */
	TRISB=0x001C;	/* set PORTB(RB2 and RB4 pins) to input*/
					/* "0" corresponds to "o"utput and "1" to "i"nput */
					/* SOSCSEL_IO setting in CONFIG3 to use RA4 and RB4 pins as digital ones */ 
	CNPU1=0x00CE;	/* Pull up CN1, 2, 3, 6, 7, pins  */



    // Set Default demo state
    DemoState = BT_INITIALIZE;
	HciState=HCI_CMD_RESET;

	LATBbits.LATB15=0;
	ConnectSwitch=PORTBbits.RB4;//YTS

    return TRUE;
} // InitializeSystem


/*************************************************************************
 * Function:        CheckForNewAttach
 *
 * Preconditions:   None
 *
 * Input:           None
 *
 * Output:          deviceAddress (global)
 *                  Updates the device address when an attach is found.
 *
 * Returns:         TRUE if a new device has been attached.  FALSE,
 *                  otherwise.
 *
 * Side Effects:    Prints attach message
 *
 * Overview:        This routine checks to see if a new device has been
 *                  attached.  If it has, it records the address.
 *************************************************************************/

BOOL CheckForNewAttach ( void )
{
    // Try to get the device address, if we don't have one.
    if (deviceAddress == 0)
    {
        GENERIC_DEVICE_ID DevID;

        DevID.vid   = 0x04D8;//NO meaning: Any number is OK.
        DevID.pid   = 0x000C;//NO meaning: Any number is OK.
        #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
            DevID.serialNumberLength = 0;
            DevID.serialNumber = NULL;
        #endif

        if (USBHostGenericGetDeviceAddress(&DevID))
        {
            deviceAddress = DevID.deviceAddress;
            UART2PrintString( "Generic demo device attached - polled, deviceAddress=" );
            UART2PutDec( deviceAddress );
            UART2PrintString( "\r\n" );
            return TRUE;
        }
    }

    return FALSE;

} // CheckForNewAttach


/*************************************************************************
 * Function:        ManageDemoState
 *
 * Preconditions:   The DemoState global variable must be initialized to
 *                  DEMO_STATE_IDLE (0).  (This occurs on reset.)
 *
 * Input:           DemoState (global)
 *                  Actions selected based value of DemoState on function
 *                  entry.
 *
 *                  deviceAddress (global)
 *                  May use device address to access device, depending on
 *                  state.
 *
 *                  DataPacket (global)
 *                  May read data from packet buffer, depending on state.
 *
 * Output:          DemoState (global)
 *                  Updates demo state as appropriate.
 *
 *                  DataPacket (global)
 *                  May cause data in the packet buffer to be updated,
 *                  depending on state.
 *
 * Returns:         None
 *
 * Side Effects:    Depend on state transition
 *
 * Overview:        This routine maintains the state of the application,
 *                  updateing global data and taking actions as necessary
 *                  to maintain the custom demo operations.
 *************************************************************************/
void ManageDemoState ( void )
{
    BYTE RetVal;

    if (USBHostGenericDeviceDetached(deviceAddress) && deviceAddress != 0)
    {
        UART2PrintString( "Generic demo device detached - polled\r\n" );
        DemoState = BT_INITIALIZE;
		HciState=HCI_CMD_RESET;
        deviceAddress   = 0;
    }

    switch (DemoState)
    {
    case BT_INITIALIZE:
        DemoState = BT_STATE_IDLE;
        break;

    /** Idle State:  Loops here until attach **/
    case BT_STATE_IDLE:
        if (CheckForNewAttach())
        {
			DemoState = BT_STATE_PROCESS;
			HciState = HCI_CMD_RESET;
        }
        break;

    case BT_STATE_PROCESS:
		switch (HciState)
		{
//HCI layer***********************************************************************
		case HCI_CMD_RESET:
			buf1[0]=0x03;
			buf1[1]=0x0c;
			buf1[2]=0;
			data_size=3;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_RESET_END;
		    break;

		case HCI_CMD_RESET_END:
			end_num=0x0e;// When buf1[0]==end_num, reading process ends. See BT_STATE_READ_EP1
			strcpy(message,"HCI_CMD_RESET: ");//message for BT_STATE_READ_EP1
			DemoState = BT_STATE_READ_EP1;
			HciState = HCI_CMD_READ_BD_ADDR;
		    break;

//********************************************************************************
		case HCI_CMD_READ_BD_ADDR:
			buf1[0]=0x09;
			buf1[1]=0x10;
			buf1[2]=0;
			data_size=3;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_READ_BD_ADDR_END;
			break;

		case HCI_CMD_READ_BD_ADDR_END:
			end_num=0x0e;strcpy(message,"HCI_CMD_READ_BD_ADDR: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = HCI_CMD_MASK;
		    break;


//********************************************************************************
		case HCI_CMD_MASK:

			for(step=0;step<6;step++) RES_BT_ADDRESS[step]=buf1[6+step];

			#ifdef DEBUG_MODE
			for(data_num=0;data_num<6;data_num++)
      	          {UART2PutHex(RES_BT_ADDRESS[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif

			buf1[0]=0x01;
			buf1[1]=0x0c;
			buf1[2]=0x08;
			buf1[3]=0x2f;
			buf1[4]=0xff;
			buf1[5]=0xff;
			buf1[6]=0xff;
			buf1[7]=0xff;
			buf1[8]=0xff;
			buf1[9]=0xbf;
			buf1[10]=0x3d;
			data_size=11;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_MASK_END;
			break;

		case HCI_CMD_MASK_END:
			end_num=0x0e;strcpy(message,"HCI_CMD_MASK: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = HCI_CMD_ADVERT_DATA;
		    break;

//********************************************************************************
		case HCI_CMD_ADVERT_DATA:
//LE Set Advertising Data Command
			buf1[0]=0x08;
			buf1[1]=0x20;
			buf1[2]=0x20;//PDU length
			buf1[3]=0x13;// data length 3+4+4+8=19

			buf1[4]=0x02;//length
			buf1[5]=0x01;//flags
			buf1[6]=0x05;
			buf1[7]=0x03;//length
			buf1[8]=0x03;//UUID 
			buf1[9]=0x12;//0x1812 HID service
			buf1[10]=0x18;
			buf1[11]=0x03;//length
			buf1[12]=0x19;
			buf1[13]=0xc0;//appearance(HID)
			buf1[14]=0x03;
			buf1[15]=0x07;//length
			buf1[16]=0x09;//local name
			buf1[17]='Y';
			buf1[18]='T';
			buf1[19]='S';
			buf1[20]='-';
			buf1[21]='L';
			buf1[22]='E';
			for(step=23;step<35;step++)
			buf1[step]=0x00;

			data_size=35;
			DemoState =BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_ADVERT_DATA_END;
			break;

		case HCI_CMD_ADVERT_DATA_END:
			end_num=0x0e;strcpy(message,"HCI_CMD_SCAN_ENABLE: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = HCI_CMD_ADVERT_PARAM;
		    break;

//********************************************************************************
		case HCI_CMD_ADVERT_PARAM:
//LE Set Advertising Parameters Command
			buf1[0]=0x06;
			buf1[1]=0x20;
			buf1[2]=0x0f;//length
			buf1[3]=0x00;//0xa0;// Advertising_Interval_Min
			buf1[4]=0x01;//0x08;//0x00;
			buf1[5]=0x00;//0x00;//Advertising_Interval_Max
			buf1[6]=0x01;//0x08;//0x40;
			if(0==ConnectSwitch) buf1[7]=0x00;//Connectable undirected advertising
			else buf1[7]=0x01;

			buf1[8]=0x00;//Own_Address_Type
			buf1[9]=0x00;//Direct_Address_Type

			if(1==ConnectSwitch){

			//get storde BD_address of PC
			TBLPAG=((PROG_ADDR1 & 0x7f0000)>>16);
			addrOffset=(PROG_ADDR1 & 0x00ffff);

			asm("tblrdl.b [%1], %0": "=r"(buf1[10]) : "r"(addrOffset));// Read low byte
			asm("tblrdl.b [%1], %0": "=r"(buf1[11]) : "r"(addrOffset+1));// Read middle byte
			asm("tblrdh.b [%1], %0": "=r"(buf1[12]) : "r"(addrOffset));// Read high byte

			TBLPAG=((PROG_ADDR2 & 0x7f0000)>>16);
			addrOffset=(PROG_ADDR2 & 0x00ffff);

			asm("tblrdl.b [%1], %0": "=r"(buf1[13]) : "r"(addrOffset));// Read low byte
			asm("tblrdl.b [%1], %0": "=r"(buf1[14]) : "r"(addrOffset+1));// Read middle byte
			asm("tblrdh.b [%1], %0": "=r"(buf1[15]) : "r"(addrOffset));// Read high byte
			}
			else {
				buf1[10]=0x00;//Direct_Address
				buf1[11]=0x00;
				buf1[12]=0x00;
				buf1[13]=0x00;
				buf1[14]=0x00;
				buf1[15]=0x00;
			}
			buf1[16]=0x07;//Advertising_Channel_Map
			buf1[17]=0x00;//Advertising_Filter_Policy

			data_size=18;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_ADVERT_PARAM_END;
			break;

		case HCI_CMD_ADVERT_PARAM_END:
			end_num=0x0e;strcpy(message,"HCI_CMD_ADVERT_PARAM: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = HCI_CMD_ADVERT_ENABLE;
		    break;

//********************************************************************************
		case HCI_CMD_ADVERT_ENABLE:
//Set Advertise Enable Command
			buf1[0]=0x0a;
			buf1[1]=0x20;
			buf1[2]=0x01;//length
			buf1[3]=0x01;//Advertising is enabled.
			data_size=4;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = HCI_CMD_ADVERT_ENABLE_END;
			break;

		case HCI_CMD_ADVERT_ENABLE_END:
			end_num=0x3e;strcpy(message,"HCI_CMD_ADVERT_ENABLE: ");
			DemoState = BT_STATE_READ_EP1;
			HciState = GATT_SARVICE_CHANGE;
		    break;

//********************************************************************************
//********************************************************************************
// GATT LAYER (FIRST HALF)
//********************************************************************************
//********************************************************************************
		case  GATT_SARVICE_CHANGE:

			handle[0]=buf1[4];handle[1]=buf1[5];

			INI_ADDRESS_TYPE=buf1[7];
			for(step=0;step<6;step++) INI_BT_ADDRESS[step]=buf1[8+step];

//////////////////
			NVMCON=0x4042;

			TBLPAG=((PROG_ADDR1 & 0x7f0000)>>16);
			addrOffset=(PROG_ADDR1 & 0x00ffff);

			__builtin_tblwtl(addrOffset,0x0000);

			asm("DISI #5");
			__builtin_write_NVM();

			//store BD_address of PC in program memory (memory addresses are PROG_ADDR1 and PROG_ADDR2 )
			NVMCON=0x4003;

			TBLPAG=((PROG_ADDR1 & 0x7f0000)>>16);
			addrOffset=(PROG_ADDR1 & 0x00ffff);

			asm("tblwtl.b %1, [%0]": "=r"(addrOffset) : "d"(buf1[8]));// Write low byte
			addrOffset++;
			asm("tblwtl.b %1, [%0]": "=r"(addrOffset) : "d"(buf1[9]));// Write middle byte
			addrOffset--;
			asm("tblwth.b %1, [%0]": "=r"(addrOffset) : "d"(buf1[10]));// Write high byte
			asm("DISI #5");
			__builtin_write_NVM();

			NVMCON=0x4003;

			TBLPAG=((PROG_ADDR2 & 0x7f0000)>>16);
			addrOffset=(PROG_ADDR2 & 0x00ffff);

			asm("tblwtl.b %1, [%0]": "=r"(addrOffset) : "d"(buf1[11]));// Write low byte
			addrOffset++;
			asm("tblwtl.b %1, [%0]": "=r"(addrOffset) : "d"(buf1[12]));// Write middle byte
			addrOffset--;
			asm("tblwth.b %1, [%0]": "=r"(addrOffset) : "d"(buf1[13]));// Write high byte
			asm("DISI #5");
			__builtin_write_NVM();
//////////////////


			#ifdef DEBUG_MODE
			for(data_num=0;data_num<6;data_num++)
      	          {UART2PutHex(INI_BT_ADDRESS[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif

			strcpy(message,"GATT_SARVICE_CHANGE:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_SARVICE_CHANGE_RESP;

			break;

//********************************************************************************
		case  GATT_SARVICE_CHANGE_RESP:

			buf[2]=0x09;
			buf[3]=0x00;
			buf[4]=0x05;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x01;//ERROR
			buf[9]=0x08;
			buf[10]=0x01;
			buf[11]=0x00;
			buf[12]=0x0a;
			data_size=13;		

			DemoState = BT_STATE_WRITE_ACL;
			HciState =  GATT_DEVICE_NAME;

			break;

//********************************************************************************
		case  GATT_DEVICE_NAME:
			strcpy(message,"GATT_DEVICE_NAME:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_DEVICE_NAME_RESP;
			break;

		case  GATT_DEVICE_NAME_RESP:

			buf[2]=0x0e;
			buf[3]=0x00;
			buf[4]=0x0a;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x09;//Read By Type Response
			buf[9]=0x08;//length
			buf[10]=0x03;//handle
			buf[11]=0x00;
			buf[12]='Y';
			buf[13]='T';
			buf[14]='S';
			buf[15]='-';
			buf[16]='L';
			buf[17]='E';
			data_size=18;		

			DemoState = BT_STATE_WRITE_ACL;
			HciState =  GATT_PRIMARY_SERVICE1;

			break;

//********************************************************************************
		case  GATT_PRIMARY_SERVICE1:
			strcpy(message,"GATT_PRIMARY_SERVICE1:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			if(1==ConnectSwitch) HciState =GATT_BATTERY_CCC_RES1;
			else HciState = GATT_PRIMARY_SERVICE1_RESP;//GO TO FIRST CONNECTION
			break;

//********************************************************************************
//********************************************************************************
// SECOND OR LATER CONNECTION STAGE
//********************************************************************************
//********************************************************************************
		case  GATT_BATTERY_CCC_RES1:

			buf[2]=0x05;
			buf[3]=0x00;
			buf[4]=0x01;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x13;

			data_size=9;	

			DemoState = BT_STATE_WRITE_ACL;
			HciState =  GATT_BATTERY_NOW_REQ1;

			break;

		case  GATT_BATTERY_NOW_REQ1:
			strcpy(message,"GATT_BATTERY_NOW_RES1:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_BATTERY_NOW_RES1;
			break;

//********************************************************************************
		case  GATT_BATTERY_NOW_RES1:

			buf[2]=0x06;
			buf[3]=0x00;
			buf[4]=0x02;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x0b;
			buf[9]=0x64;

			data_size=10;	

			DemoState = BT_STATE_WRITE_ACL;
			HciState =  GATT_CCC_REQ;
			step=0;
			break;

		case  GATT_CCC_REQ:
			step++;
			strcpy(message,"GATT_CCC_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_CCC_RES;

			break;

//********************************************************************************
		case  GATT_CCC_RES:

			buf[2]=0x05;
			buf[3]=0x00;
			buf[4]=0x01;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x13;

			data_size=9;	

			DemoState = BT_STATE_WRITE_ACL;
			HciState =  GATT_CCC_REQ;
			if(step==2) HciState = HID_READ_DATA;
			break;

//********************************************************************************
//********************************************************************************
// FIRST CONNECTION STAGE
//********************************************************************************
//********************************************************************************
		case  GATT_PRIMARY_SERVICE1_RESP:

			buf[2]=0x18;
			buf[3]=0x00;
			buf[4]=0x14;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x11;//Read by Group Type Response
			buf[9]=0x06;//lendth par type
			buf[10]=0x01;//handle
			buf[11]=0x00;
			buf[12]=0x05;//end group (from 0x0001 to 0x0007
			buf[13]=0x00;
			buf[14]=0x00;//type
			buf[15]=0x18;
			buf[16]=0x06;//handle
			buf[17]=0x00;
			buf[18]=0x13;//end group (from 0x0008 to 0x0008)
			buf[19]=0x00;
			buf[20]=0x12;//type
			buf[21]=0x18;
			buf[22]=0x14;//handle
			buf[23]=0x00;
			buf[24]=0x17;//0x1c;//end group (from 0x0009 to 0x001c)
			buf[25]=0x00;
			buf[26]=0x0f;//type
			buf[27]=0x18;

			data_size=28;		

			DemoState = BT_STATE_WRITE_ACL;
			HciState =  GATT_PRIMARY_SERVICE2;

			break;

//********************************************************************************
		case  GATT_PRIMARY_SERVICE2://0x2800:GATT Primary Service Declaration
			strcpy(message,"GATT_PRIMARY_SERVICE2:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_PRIMARY_SERVICE2_RESP;
			break;

		case  GATT_PRIMARY_SERVICE2_RESP:

			buf[2]=0x0c;
			buf[3]=0x00;
			buf[4]=0x08;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x11;//Read by Group Type Response
			buf[9]=0x06;//lendth par type
			buf[10]=0x18;//handle
			buf[11]=0x00;
			buf[12]=0xff;//end group
			buf[13]=0xff;
			buf[14]=0x0a;//type
			buf[15]=0x18;

			data_size=16;		

			DemoState = BT_STATE_WRITE_ACL;
			HciState =  SMP_PAIR_REQ;

			break;

//********************************************************************************
		case  SMP_PAIR_REQ://0x2803:GATT Characteristic Declaration
			strcpy(message,"SMP_PAIR_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SMP_PAIR_RESP;
			break;

//********************************************************************************
//********************************************************************************
// SMP LAYER
//********************************************************************************
//********************************************************************************
		case  SMP_PAIR_RESP:

			for(step=0;step<7;step++) PAIR_REQ[step]=buf[8+step];

			#ifdef DEBUG_MODE
			for(data_num=0;data_num<7;data_num++)
      	          {UART2PutHex(PAIR_REQ[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif

			buf[2]=0x0b;
			buf[3]=0x00;
			buf[4]=0x07;
			buf[5]=0x00;
			buf[6]=0x06;
			buf[7]=0x00;
			for(step=0;step<7;step++) buf[8+step]=PAIR_RES[step];//Pairing Response

			data_size=15;		

			DemoState = BT_STATE_WRITE_ACL;
			HciState =  SMP_PAIR_CONF_IN;

			break;

//********************************************************************************
		case  SMP_PAIR_CONF_IN:
			strcpy(message,"SMP_PAIR_CONF_IN:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SMP_FUNC_E;
			break;


//********************************************************************************
//Pair config calculation
		case SMP_FUNC_E:

			P1[0]=INI_ADDRESS_TYPE;
			P1[1]=RES_ADDRESS_TYPE;
			for(step=0;step<7;step++) {
				P1[2+step]=PAIR_REQ[step];
				P1[9+step]=PAIR_RES[step];
			}

			buf1[0]=0x17;
			buf1[1]=0x20;
			buf1[2]=0x20;
			for(step=0;step<16;step++) buf1[3+step]=0x00;
			for(step=0;step<16;step++) buf1[19+step]=  PAIR_RANDOM [step] ^ P1[step];

			data_size=35;

			#ifdef DEBUG_MODE
			for(data_num=0;data_num<35;data_num++)
      	          {UART2PutHex(buf1[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif

			DemoState = BT_STATE_WRITE_CLASS;
			HciState = SMP_FUNC_E_END;
			break;

		case SMP_FUNC_E_END:
			end_num=0x0e;strcpy(message,"SMP_FUNC_E_END:\r\n");
			DemoState = BT_STATE_READ_EP1;
			HciState = SMP_FUNC_C1;
		    break;

		case SMP_FUNC_C1:

			for(step=0;step<6;step++) {
				P2[step]=RES_BT_ADDRESS[step];
				P2[6+step]=INI_BT_ADDRESS[step];
			}
			for(step=0;step<4;step++) P2[12+step]=0x00;

			buf1[0]=0x17;
			buf1[1]=0x20;
			buf1[2]=0x20;		
			for(step=0;step<16;step++) buf1[34-step]= buf1[21-step] ^ P2[15-step];
			for(step=0;step<16;step++) buf1[3+step]=0x00;
			data_size=35;

			#ifdef DEBUG_MODE
			for(data_num=0;data_num<35;data_num++)
      	          {UART2PutHex(buf1[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif

			DemoState = BT_STATE_WRITE_CLASS;
			HciState = SMP_FUNC_C1_END;
			break;

		case SMP_FUNC_C1_END:
			end_num=0x0e;strcpy(message,"SMP_FUNC_C1_END:\r\n");
			DemoState = BT_STATE_READ_EP1;
			HciState = SMP_PAIR_CONF_OUT;
		    break;


		case SMP_PAIR_CONF_OUT:

			for(step=0;step<16;step++) PAIR_CONF[step]=buf1[6+step];

			#ifdef DEBUG_MODE
			for(data_num=0;data_num<16;data_num++)
      	          {UART2PutHex(PAIR_CONF[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif

			buf[2]=0x15;
			buf[3]=0x00;
			buf[4]=0x11;
			buf[5]=0x00;
			buf[6]=0x06;
			buf[7]=0x00;
			buf[8]=0x03;//Pairing Confirm
			for(step=0;step<16;step++) buf[9+step]=PAIR_CONF[step];

			data_size=25;
			DemoState = BT_STATE_WRITE_ACL;
			HciState = SMP_RANDOM_IN;
			break;

//********************************************************************************
		case  SMP_RANDOM_IN:
			strcpy(message,"SMP_RANDOM_IN:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SMP_RANDOM_OUT;
			break;

		case  SMP_RANDOM_OUT:

			for(step=0;step<16;step++) INIT_RANDOM[step]=buf[9+step];

			#ifdef DEBUG_MODE
			for(data_num=0;data_num<16;data_num++)
      	          {UART2PutHex(INIT_RANDOM[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif

			buf[2]=0x15;
			buf[3]=0x00;
			buf[4]=0x11;
			buf[5]=0x00;
			buf[6]=0x06;
			buf[7]=0x00;
			buf[8]=0x04;//
			for(step=0;step<16;step++) buf[9+step]=PAIR_RANDOM[step];

			data_size=25;
			DemoState = BT_STATE_WRITE_ACL;
			HciState = SMP_FUNC_S1;
			break;

//********************************************************************************
//STK = s1(TK, Srand, Mrand)
		case SMP_FUNC_S1:

			buf1[0]=0x17;
			buf1[1]=0x20;
			buf1[2]=0x20;
			for(step=0;step<16;step++) buf1[3+step]=0x00;
			for(step=0;step<8;step++) {
				buf1[19+step]=  INIT_RANDOM[step];
				buf1[27+step]=  PAIR_RANDOM[step];
			}
			data_size=35;

			#ifdef DEBUG_MODE
			for(data_num=0;data_num<35;data_num++)
      	          {UART2PutHex(buf1[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif

			DemoState = BT_STATE_WRITE_CLASS;
			HciState = SMP_FUNC_S1_END;
			break;

		case SMP_FUNC_S1_END:
			end_num=0x0e;strcpy(message,"SMP_FUNC_S1_END:\r\n");
			DemoState = BT_STATE_READ_EP1;
			HciState = SMP_LTK_REQ;
		    break;

//********************************************************************************
		case SMP_LTK_REQ:

			for(step=0;step<16;step++) STK[step]=buf1[6+step];

			end_num=0x3e;strcpy(message,"SMP_LTK_REQ:\r\n");
			DemoState = BT_STATE_READ_EP1;
			HciState = SMP_LTK_RES;
		    break;

//********************************************************************************
		case SMP_LTK_RES:

			buf1[0]=0x1a;
			buf1[1]=0x20;
			buf1[2]=0x12;
//			buf1[3]=0x00;
//			buf1[4]=0x00;
			for(step=0;step<16;step++) buf1[5+step]=STK[step];

			#ifdef DEBUG_MODE
			for(data_num=0;data_num<21;data_num++)
      	          {UART2PutHex(buf1[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif

			data_size=21;
			DemoState = BT_STATE_WRITE_CLASS;
			HciState = SMP_LTK_RES_END;
			break;


		case SMP_LTK_RES_END:

			end_num=0x0e;strcpy(message,"SMP_LTK_RES_END:\r\n");
			DemoState = BT_STATE_READ_EP1;
			HciState = SMP_LTK_OUT;
		    break;

//********************************************************************************
//********************************************************************************
//********************************************************************************
//********************************************************************************
		case  SMP_LTK_OUT:

			buf[2]=0x15;
			buf[3]=0x00;
			buf[4]=0x11;
			buf[5]=0x00;
			buf[6]=0x06;
			buf[7]=0x00;
			buf[8]=0x06;//
			for(step=0;step<16;step++) buf[9+step]=LTK[step];

			data_size=25;
			DemoState = BT_STATE_WRITE_ACL;
			HciState = SMP_RANDOM1_OUT;
			break;

		case  SMP_RANDOM1_OUT:

			buf[2]=0x0f;
			buf[3]=0x00;
			buf[4]=0x0b;
			buf[5]=0x00;
			buf[6]=0x06;
			buf[7]=0x00;
			buf[8]=0x07;//
			for(step=0;step<10;step++) buf[9+step]=EDIV_RANDOM[step];

			data_size=19;
			DemoState = BT_STATE_WRITE_ACL;
			HciState = SMP_LTK_IN;
			break;

		case  SMP_LTK_IN:

			strcpy(message,"SMP_LTK_IN:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = SMP_RANDOM1_IN;
			break;

		case  SMP_RANDOM1_IN:
			for(step=0;step<10;step++) M_EDIV_RANDOM[step]=buf[9+step];
			strcpy(message,"SMP_RANDOM1_IN:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_HID_CHAR1_REQ;
			break;

//********************************************************************************
//********************************************************************************
// GATT LAYER (LATTER HALF)
//********************************************************************************
//********************************************************************************
		case  GATT_HID_CHAR1_REQ:

			strcpy(message,"GATT_HID_CHAR1_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_HID_CHAR1_RES;
			break;

//********************************************************************************
		case  GATT_HID_CHAR1_RES:

			buf[2]=0x1b;//length
			buf[3]=0x00;
			buf[4]=0x17;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x09;
			buf[9]=0x07;
			for(step=0;step<7;step++) {
				buf[10+step]=HID_CHAR[0][step];
				buf[17+step]=HID_CHAR[1][step];
				buf[24+step]=HID_CHAR[2][step];
			}

			data_size=31;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_HID_CHAR2_REQ;
			break;

		case GATT_HID_CHAR2_REQ:

			strcpy(message,"GATT_HID_CHAR2_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_HID_CHAR2_RES;
			break;

//********************************************************************************
		case  GATT_HID_CHAR2_RES:

			buf[2]=0x14;//length
			buf[3]=0x00;
			buf[4]=0x10;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x09;
			buf[9]=0x07;
			for(step=0;step<7;step++) 
				for(step=0;step<7;step++) {
				buf[10+step]=HID_CHAR[3][step];
				buf[17+step]=HID_CHAR[4][step];
			}

			data_size=24;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_HID_CHAR3_REQ;
			break;

		case GATT_HID_CHAR3_REQ:

			strcpy(message,"GATT_HID_CHAR3_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_HID_CHAR4_RES;
			break;

		case  GATT_HID_CHAR4_RES:

			buf[2]=0x09;//length
			buf[3]=0x00;
			buf[4]=0x05;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x01;
			for(step=0;step<4;step++) buf[9+step]=HID_CHAR[5][step];

			data_size=13;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_HID_REPORT_REQ1;
			break;

		case GATT_HID_REPORT_REQ1:

			strcpy(message,"GATT_HID_REPORT_REQ1:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_HID_REPORT_RES1;
			break;

//********************************************************************************
		case  GATT_HID_REPORT_RES1:

			buf[2]=0x1b;//length
			buf[3]=0x00;
			buf[4]=0x17;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x0b;
			for(step=0;step<22;step++) buf[9+step]=HID_REPORT[0][step];

			data_size=31;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_HID_REPORT_REQ2;
			break;

		case GATT_HID_REPORT_REQ2:

			strcpy(message,"GATT_HID_REPORT_REQ2:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_HID_REPORT_RES2;
			break;

//********************************************************************************
		case  GATT_HID_REPORT_RES2:

			buf[2]=0x0c;//length
			buf[3]=0x00;
			buf[4]=0x08;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x0d;
			for(step=0;step<7;step++) buf[9+step]=HID_REPORT[1][step];

			data_size=16;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_HID_DESC_REQ1;
			break;

		case GATT_HID_DESC_REQ1:

			strcpy(message,"GATT_HID_DESC_REQ1:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_HID_DESC_RES1;
			break;

//********************************************************************************
		case  GATT_HID_DESC_RES1:

			buf[2]=0x0e;//length
			buf[3]=0x00;
			buf[4]=0x0a;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x05;
			buf[9]=0x01;
			buf[10]=0x09;
			buf[11]=0x00;
			buf[12]=0x08;
			buf[13]=0x29;
			buf[14]=0x0a;
			buf[15]=0x00;
			buf[16]=0x02;
			buf[17]=0x29;

			data_size=18;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_HID_DESC_REQ2;
			break;

		case GATT_HID_DESC_REQ2:

			strcpy(message,"GATT_HID_DESC_REQ2:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_HID_DESC_RES2;
			break;

//********************************************************************************
		case  GATT_HID_DESC_RES2:

			buf[2]=0x0a;//length
			buf[3]=0x00;
			buf[4]=0x06;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x05;
			buf[9]=0x01;
			buf[10]=0x0d;
			buf[11]=0x00;
			buf[12]=0x08;
			buf[13]=0x29;

			data_size=14;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_HID_INFO_REQ;
			break;

		case GATT_HID_INFO_REQ:

			strcpy(message,"GATT_HID_INFO_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_HID_INFO_RESP;
			break;

//********************************************************************************
		case  GATT_HID_INFO_RESP:

			buf[2]=0x09;//length
			buf[3]=0x00;
			buf[4]=0x05;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x0b;
			buf[9]=0x02;
			buf[10]=0x00;
			buf[11]=0x00;
			buf[12]=0x10;
			data_size=13;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_REP_REF_REQ1;
			break;

		case GATT_REP_REF_REQ1:

			strcpy(message,"GATT_REP_REF_REQ1:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_REP_REF_RES1;
			break;

//********************************************************************************
		case  GATT_REP_REF_RES1:

			buf[2]=0x07;//length
			buf[3]=0x00;
			buf[4]=0x03;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x0b;
			buf[9]=0x00;
			buf[10]=0x01;

			data_size=11;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_REP_REF_REQ2;
			break;

		case GATT_REP_REF_REQ2:

			strcpy(message,"GATT_REP_REF_REQ2:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_REP_REF_RES2;
			break;

//********************************************************************************
		case  GATT_REP_REF_RES2:

			buf[2]=0x07;//length
			buf[3]=0x00;
			buf[4]=0x03;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x0b;
			buf[9]=0x00;
			buf[10]=0x02;

			data_size=11;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_HID_CCC_REQ;
			break;

		case GATT_HID_CCC_REQ:

			strcpy(message,"GATT_HID_CCC_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_HID_CCC_RES;
			break;

//********************************************************************************
		case  GATT_HID_CCC_RES:

			buf[2]=0x05;//length
			buf[3]=0x00;
			buf[4]=0x01;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x13;

			data_size=9;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_ACCESS_CHAR1_REQ;
			break;

		case GATT_ACCESS_CHAR1_REQ:

			strcpy(message,"GATT_ACCESS_CHAR1_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_ACCESS_CHAR1_RES;
			break;

//********************************************************************************
		case  GATT_ACCESS_CHAR1_RES:

			buf[2]=0x14;//length
			buf[3]=0x00;
			buf[4]=0x10;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x09;
			buf[9]=0x07;
			for(step=0;step<7;step++){
				buf[10+step]= ACCESS_SERVICE[0][step];
				buf[17+step]= ACCESS_SERVICE[1][step];
			}
			data_size=24;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_ACCESS_CHAR2_REQ;
			break;

		case GATT_ACCESS_CHAR2_REQ:

			strcpy(message,"GATT_ACCESS_CHAR2_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_ACCESS_CHAR2_RES;
			break;

//********************************************************************************
		case  GATT_ACCESS_CHAR2_RES:

			buf[2]=0x09;//length
			buf[3]=0x00;
			buf[4]=0x05;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x01;
			for(step=0;step<4;step++) buf[9+step]= ACCESS_SERVICE[2][step];

			data_size=13;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_DEVICEE_CHAR1_REQ;
			break;

//********************************************************************************
		case GATT_DEVICEE_CHAR1_REQ:

			strcpy(message,"GATT_DEVICEE_CHAR1_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_DEVICEE_CHAR1_RES;
			break;

//********************************************************************************
		case  GATT_DEVICEE_CHAR1_RES:

			buf[2]=0x0d;//length
			buf[3]=0x00;
			buf[4]=0x09;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x09;
			buf[9]=0x07;
			for(step=0;step<7;step++){
				buf[10+step]= PNP_SERVICE[0][step];
			}

			data_size=17;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_DEVICEE_CHAR2_REQ;
			break;

		case GATT_DEVICEE_CHAR2_REQ:

			strcpy(message,"GATT_DEVICEE_CHAR2_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_DEVICEE_CHAR2_RES;
			break;

//********************************************************************************
		case  GATT_DEVICEE_CHAR2_RES:

			buf[2]=0x09;//length
			buf[3]=0x00;
			buf[4]=0x05;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x01;
			for(step=0;step<4;step++) buf[9+step]= PNP_SERVICE[1][step];

			data_size=13;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_BATTERY_CHAR1_REQ;
			break;

		case GATT_BATTERY_CHAR1_REQ:
			strcpy(message,"GATT_BATTERY_CHAR1_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_BATTERY_CHAR1_RES;
			break;

//********************************************************************************
		case  GATT_BATTERY_CHAR1_RES:

			buf[2]=0x0d;//length
			buf[3]=0x00;
			buf[4]=0x09;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x09;
			buf[9]=0x07;
			buf[10]=0x15;
			buf[11]=0x00;
			buf[12]=0x12;
			buf[13]=0x16;
			buf[14]=0x00;
			buf[15]=0x19;
			buf[16]=0x2a;

			data_size=17;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_BATTERY_CHAR2_REQ;
			break;

		case GATT_BATTERY_CHAR2_REQ:

			strcpy(message,"GATT_BATTERY_CHAR2_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_BATTERY_CHAR2_RES;
			break;

//********************************************************************************
		case  GATT_BATTERY_CHAR2_RES:

			buf[2]=0x09;//length
			buf[3]=0x00;
			buf[4]=0x05;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x01;
			buf[9]=0x08;
			buf[10]=0x16;
			buf[11]=0x00;
			buf[12]=0x0a;

			data_size=13;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_ACCESS_NAME_REQ;
			break;

		case GATT_ACCESS_NAME_REQ:
			strcpy(message,"GATT_ACCESS_NAME_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_ACCESS_NAME_RES;
			break;

//********************************************************************************
		case  GATT_ACCESS_NAME_RES:

			buf[2]=0x0b;//length
			buf[3]=0x00;
			buf[4]=0x07;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x0b;
			buf[9]='Y';
			buf[10]='T';
			buf[11]='S';
			buf[12]='_';
			buf[13]='L';
			buf[14]='E';

			data_size=15;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_ACCESS_ICON_REQ;
			break;

		case GATT_ACCESS_ICON_REQ:
			strcpy(message,"GATT_ACCESS_ICON_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_ACCESS_ICON_RES;
			break;

//********************************************************************************
		case  GATT_ACCESS_ICON_RES:

			buf[2]=0x07;//length
			buf[3]=0x00;
			buf[4]=0x03;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x0b;
			buf[9]=0xc0;
			buf[10]=0x03;

			data_size=11;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_PNP_REQ;
			break;

		case GATT_PNP_REQ:
			strcpy(message,"GATT_PNP_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_PNP_RES;
			break;

//********************************************************************************
		case  GATT_PNP_RES:

			buf[2]=0x0c;//length
			buf[3]=0x00;
			buf[4]=0x08;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x0b;
			buf[9]=0x01;
			buf[10]=0xd8;
			buf[11]=0x04;
			buf[12]=0x3f;
			buf[13]=0x00;
			buf[14]=0x11;
			buf[15]=0x01;

			data_size=16;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_BATTERY_DESC_REQ;
			break;

		case GATT_BATTERY_DESC_REQ:
			strcpy(message,"GATT_BATTERY_DESC_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_BATTERY_DESC_RES;
			break;

//********************************************************************************
		case  GATT_BATTERY_DESC_RES:

			buf[2]=0x0a;//length
			buf[3]=0x00;
			buf[4]=0x06;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x05;
			buf[9]=0x01;
			buf[10]=0x17;
			buf[11]=0x00;
			buf[12]=0x02;
			buf[13]=0x29;

			data_size=14;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_BATTERY_NOW_REQ;
			break;

		case GATT_BATTERY_NOW_REQ:
			strcpy(message,"GATT_BATTERY_NOW_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_BATTERY_NOW_RES;
			break;

//********************************************************************************
		case  GATT_BATTERY_NOW_RES:

			buf[2]=0x06;//length
			buf[3]=0x00;
			buf[4]=0x02;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x0b;
			buf[9]=0x64;

			data_size=10;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = GATT_BATTERY_CCC_REQ;
			break;

		case GATT_BATTERY_CCC_REQ:
			strcpy(message,"GATT_BATTERY_CCC_REQ:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = GATT_BATTERY_CCC_RES;
			break;

//********************************************************************************
		case  GATT_BATTERY_CCC_RES:

			buf[2]=0x05;//length
			buf[3]=0x00;
			buf[4]=0x01;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x13;

			data_size=9;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = HID_READ_DATA;
			break;

//********************************************************************************
//********************************************************************************
		case L2CAP_DISCONNECT_REQ:
			buf1[0]=0x06;
			buf1[1]=0x04;
			buf1[2]=0x03;
			buf1[3]=handle[0];
			buf1[4]=handle[1]-0x20;
			buf1[5]=0x13;

			DemoState = BT_STATE_WRITE_CLASS;
			HciState = L2CAP_DISCONNECT_REQ_END;
			break;

		case L2CAP_DISCONNECT_REQ_END:
			#ifdef DEBUG_MODE
            UART2PrintString( "HCI_DISCONNECT !:\r\n" );
			#endif

			HciState = PROG_END;
		break;

//********************************************************************************
//********************************************************************************
// HOGP (HID OVER GATT PROFILE)
//********************************************************************************
//********************************************************************************

		case  HID_READ_DATA:

			LATBbits.LATB15=1;

			strcpy(message,"HID_READ_DATA:\r\n");
			DemoState = BT_STATE_READ_ACL_HCI;
			HciState = HID_WRITE_RES;
			break;


		case  HID_WRITE_RES:

			buf[2]=0x05;
			buf[3]=0x00;
			buf[4]=0x01;
			buf[5]=0x00;
			buf[6]=0x04;
			buf[7]=0x00;
			buf[8]=0x13;

			data_size=9;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = HID_WRITE_DATA;
			break;


		case  HID_WRITE_DATA:

			buf[2]=0x1a;
			buf[3]=0x00;
			buf[4]=0x16;
			buf[5]=0x00;

			buf[8]=0x1b;
			buf[9]=0x08;
			buf[10]=0x00;
			buf[11]+=1;
			data_size=30;

			DemoState = BT_STATE_WRITE_ACL;
			HciState = HID_READ_DATA;
			break;

		case  PROG_END:
			break;

		}
		break;

//********************************************************************************
//Å@READ-WRITE FUNCTIONS
//********************************************************************************
//WRITE ENDPOINT 0
    case BT_STATE_WRITE_CLASS:
        if (!USBHostGenericTxIsBusy(deviceAddress))
		{
            if ( (RetVal=USBHostGenericClassRequest( deviceAddress, buf1, data_size )) == USB_SUCCESS )
            {
			//UART2PrintString( "HCI COMMAND SENT\r\n" );	
            DemoState = BT_STATE_PROCESS;
            }
            else
            {
			UART2PrintString( "Write Class Error !\r\n" );	
            }
        }
        break;


//READ ENDPOINT 1
    case BT_STATE_READ_EP1:
        if (!USBHostGenericRx1IsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericRead(deviceAddress, buf1, DATA_PACKET_LENGTH)) == USB_SUCCESS )
            {
                DemoState = BT_STATE_READ_CLASS_WAITING;
                //UART2PrintString( "READ EP1\r\n" );
            }
            else
            {
                UART2PrintString( "Device Read Error !\r\n" );
            }
        }
        break;

    case BT_STATE_READ_CLASS_WAITING:
        if (!USBHostGenericRx1IsBusy(deviceAddress)){

			#ifdef DEBUG_MODE
			UART2PrintString(message);
			for(data_num=0;data_num<buf1[1]+2;data_num++)
      	          {UART2PutHex(buf1[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif
			if(buf1[0]!=end_num){ DemoState =BT_STATE_READ_EP1; break;}



           DemoState = BT_STATE_PROCESS;
		}
        break;


//WRITE ENDPOINT 2
    case BT_STATE_WRITE_ACL:

			#ifdef DEBUG_MODE
			for(data_num=0;data_num<buf[2]+4;data_num++)
	        {UART2PutHex(buf[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif

        if (!USBHostGenericTxIsBusy(deviceAddress))
		{
            if ( (RetVal=USBHostGenericAclWrite( deviceAddress, buf, data_size )) == USB_SUCCESS )
            {
			//UART2PrintString( "HCI COMMAND SENT\r\n" );	
            DemoState = BT_STATE_PROCESS;
            }
            else
            {
			UART2PrintString( "Write Acl Error !\r\n" );	
            }
        }
        break;

//READ ENDPOINT 2 & 1 
    case BT_STATE_READ_ACL_HCI:
		if(EP2busy_flag==1) {DemoState = BT_STATE_READ_ACL_WAITING;break;}
        if (!USBHostGenericRx2IsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericAclRead(deviceAddress, buf, DATA_PACKET_LENGTH)) == USB_SUCCESS )
            {
                DemoState = BT_STATE_READ_ACL_WAITING;
            }
            else
            {
                UART2PrintString( "Read Acl Error !\r\n" );
            }
        }
        break;

    case BT_STATE_READ_ACL_WAITING:
        if (!USBHostGenericRx2IsBusy(deviceAddress)){

			#ifdef DEBUG_MODE
			UART2PrintString(message);
			for(data_num=0;data_num<buf[2]+4;data_num++)
   	  	        {UART2PutHex(buf[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif
			EP2busy_flag=0;			
           DemoState = BT_STATE_READ_HCI;
		}
		else if(hid_flag!=3){
			EP2busy_flag=1;
			DemoState = BT_STATE_READ_HCI;
		}
        break;

	case BT_STATE_READ_HCI:
		if(EP2busy_flag==0) buf1[0]=0xff;
        if (!USBHostGenericRx1IsBusy(deviceAddress))
        {
            if ( (RetVal=USBHostGenericRead(deviceAddress, buf1, DATA_PACKET_LENGTH)) == USB_SUCCESS )
            {
                DemoState = BT_STATE_READ_HCI_WAITING;
                //UART2PrintString( "READ EP1\r\n" );
            }
            else
            {
                UART2PrintString( "Device Read Error !\r\n" );
            }
        }
        break;

    case BT_STATE_READ_HCI_WAITING:
        if (!USBHostGenericRx1IsBusy(deviceAddress)){

			#ifdef DEBUG_MODE
			UART2PrintString( "HCI: " );
			for(data_num=0;data_num<buf1[1]+2;data_num++)
      	          {UART2PutHex(buf1[data_num]);UART2PutChar(' ');}
			UART2PrintString( "\r\n" );
			#endif

			if(EP2busy_flag==0){
				if(buf1[0]==0xff) {DemoState = BT_STATE_PROCESS;}
				else {DemoState = BT_STATE_READ_HCI;}
			}
			else{
				if(buf1[0]==0x05){HciState = L2CAP_DISCONNECT_REQ;DemoState = BT_STATE_PROCESS;}
				else {DemoState = BT_STATE_READ_ACL_WAITING;}
			}
		}
        break;

//********************************************************************************

    /** Error state:  Hold here until detached **/
    case BT_STATE_ERROR:                          // To Do: Flash LEDs
        break;

    default:
        DemoState = BT_INITIALIZE;
        break;
    }

    DelayMs(1); // 1ms delay

} // ManageDemoState


//******************************************************************************
//******************************************************************************
// USB Support Functions
//******************************************************************************
//******************************************************************************

/*************************************************************************
 * Function:        USB_ApplicationEventHandler
 *
 * Preconditions:   The USB must be initialized.
 *
 * Input:           event       Identifies the bus event that occured
 *
 *                  data        Pointer to event-specific data
 *
 *                  size        Size of the event-specific data
 *
 * Output:          deviceAddress (global)
 *                  Updates device address when an attach or detach occurs.
 *
 *                  DemoState (global)
 *                  Updates the demo state as appropriate when events occur.
 *
 * Returns:         TRUE if the event was handled, FALSE if not
 *
 * Side Effects:    Event-specific actions have been taken.
 *
 * Overview:        This routine is called by the Host layer or client
 *                  driver to notify the application of events that occur.
 *                  If the event is recognized, it is handled and the
 *                  routine returns TRUE.  Otherwise, it is ignored (or
 *                  just "sniffed" and the routine returns FALSE.
 *************************************************************************/

BOOL USB_ApplicationEventHandler ( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
        BYTE i;
    #endif

    // Handle specific events.
    switch (event)
    {
        case EVENT_GENERIC_ATTACH:
            if (size == sizeof(GENERIC_DEVICE_ID))
            {
                deviceAddress   = ((GENERIC_DEVICE_ID *)data)->deviceAddress;
                DemoState = BT_STATE_PROCESS; HciState=HCI_CMD_RESET;//YTS !!!!!!!!!!!!!!!!!
                UART2PrintString( "Generic demo device attached - event, deviceAddress=" );
                UART2PutDec( deviceAddress );
                UART2PrintString( "\r\n" );
                #ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
                    for (i=1; i<((GENERIC_DEVICE_ID *)data)->serialNumberLength; i++)
                    {
                        UART2PutChar( ((GENERIC_DEVICE_ID *)data)->serialNumber[i] );
                    }
                #endif
                UART2PrintString( "\r\n" );
                return TRUE;
            }
            break;

        case EVENT_GENERIC_DETACH:
            deviceAddress   = 0;
            DemoState = BT_INITIALIZE;	//YTS
            UART2PrintString( "Generic demo device detached - event\r\n" );
            return TRUE;

        case EVENT_GENERIC_TX_DONE:           // The main state machine will poll the driver.
        case EVENT_GENERIC_RX1_DONE:	//YTS
        case EVENT_GENERIC_RX2_DONE:	//YTS
            return TRUE;

        case EVENT_VBUS_REQUEST_POWER:
            // We'll let anything attach.
            return TRUE;

        case EVENT_VBUS_RELEASE_POWER:
            // We aren't keeping track of power.
            return TRUE;

        case EVENT_HUB_ATTACH:
            UART2PrintString( "\r\n***** USB Error - hubs are not supported *****\r\n" );
            return TRUE;
            break;

        case EVENT_UNSUPPORTED_DEVICE:
            UART2PrintString( "\r\n***** USB Error - device is not supported *****\r\n" );
            return TRUE;
            break;

        case EVENT_CANNOT_ENUMERATE:
            UART2PrintString( "\r\n***** USB Error - cannot enumerate device *****\r\n" );
            return TRUE;
            break;

        case EVENT_CLIENT_INIT_ERROR:
            UART2PrintString( "\r\n***** USB Error - client driver initialization error *****\r\n" );
            return TRUE;
            break;

        case EVENT_OUT_OF_MEMORY:
            UART2PrintString( "\r\n***** USB Error - out of heap memory *****\r\n" );
            return TRUE;
            break;

        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
            UART2PrintString( "\r\n***** USB Error - unspecified *****\r\n" );
            return TRUE;
            break;

        case EVENT_SUSPEND:
        case EVENT_DETACH:
        case EVENT_RESUME:
        case EVENT_BUS_ERROR:
            return TRUE;
            break;

        default:
            break;
    }

    return FALSE;

} // USB_ApplicationEventHandler


//******************************************************************************
//******************************************************************************
// Main
//******************************************************************************
//******************************************************************************

/*************************************************************************
 * Function:        main
 *
 * Preconditions:   None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Returns:         Never exits
 *
 * Side Effects:    Runs the application
 *
 * Overview:        This is the USB Custom Demo Application's main entry
 *                  point.
 *************************************************************************/

int main ( void )
{
    // Initialize the processor and peripherals.
    if ( InitializeSystem() != TRUE )
    {
        UART2PrintString( "\r\n\r\nCould not initialize USB Custom Demo App - system.  Halting.\r\n\r\n" );
        while (1);
    }
    if ( USBHostInit(0) == TRUE )
    {
        UART2PrintString( "\r\n\r\n***** USB Custom Demo App Initialized *****\r\n\r\n" );
    }
    else
    {
        UART2PrintString( "\r\n\r\nCould not initialize USB Custom Demo App - USB.  Halting.\r\n\r\n" );
        while (1);
    }

    // Main Processing Loop
    while (1)
    {
        // This demo does not check for overcurrent conditions.  See the
        // USB Host - Data Logger for an example of overcurrent detection
        // with the PIC24F and the USB PICtail Plus.

        // Maintain USB Host State
        USBHostTasks();
        // Maintain Demo Application State
        ManageDemoState();
    }

    return 0;

} // main


/*************************************************************************
 * EOF main.c
 */

