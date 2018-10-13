#include "stdafx.h"
#include "icsneo40DLLAPI.h"
#include "VCExample.h"
#include <Windows.h>
#include <string> 
#include <stdlib.h>
#include <conio.h>
#include <cstdint>
using namespace std;

	
	void * m_hObject; //handle for device
    bool m_bPortOpen; //tells the port status of the device
	byte NetworkIDConvert[64]; //Stores the netIDs used

	HINSTANCE hDLL;


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
char cInputCommand;
bool bKeepRunning = 1;
int iVerNumber;

	//Load DLL functions
	if(!LoadDLLAPI(hDLL)) 
	{	
		//problem, close the application
		printf("Problem loading Library\r\nMake sure icsneo40.dll is installed and accessable\r\nPress any key to Exit");
		cInputCommand = _getwch();
		return(1);
	}

	system("Color A"); //Change window color
	
	//Get the DLL Version number
	iVerNumber=icsneoGetDLLVersion();
	printf("ICS icsneo40.dll version %d\r\n\r\n",iVerNumber);
	
	while (bKeepRunning)
	{

		//Main Menu
		printf("\r\n\r\n");
		printf("Press the letter next to the function group you want to use\r\n");
		printf("A - List Attached devices\r\n");	
		printf("B - Connect to the first\r\n");	
		printf("C - Get Messages\r\n");			
		printf("D - Send Message\r\n");	
		printf("E - ISO15765\r\n");	
		printf("F - Set baud rate\r\n");	
		printf("G - Get Errors\r\n");
		printf("H - Disconnect\r\n");
		printf("X - Exit\r\n");
		printf("\r\n\r\n");
		//Get User intput
		cInputCommand = toupper(_getwch());

		//Find command chosen
		switch(cInputCommand) 
		{
			case 'A': //A - List Attached Devices
				ListDevices();
				break;
			case 'B': //B - Connect to First Device
				ConnectToFirstDevice();
				break;
			case 'C': //C - Get Messages
				GetMessagesFromDevice();
				break;
			case 'D': //D - Send Message
				printf("\r\n\r\n");
				printf("Press the letter next to the function group you want to use\r\n");
				printf("A - Send CAN frame\r\n");	
				printf("-------------------------------------\r\n");
				printf("B - Send CAN FD frame 8 bytes\r\n");	
				printf("C - Send CAN FD frame 64 bytes\r\n");			
				printf("-------------------------------------\r\n");
				printf("D - Send LIN Master Frame\r\n");	
				printf("E - Send LIN Header Only\r\n");	
				printf("F - Send LIN Slave\r\n");	
				printf("-------------------------------------\r\n");
				printf("G - Send ISO9141 KW2K message\r\n");
				printf("-------------------------------------\r\n");
				printf("H - Send Ethernet Frame (FIRE 2)\r\n");
				printf("-------------------------------------\r\n");
				printf("X - Exit\r\n");
				printf("\r\n\r\n");
				//Get User intput
				cInputCommand = toupper(_getwch());
		
				switch(cInputCommand) 
				{
					case 'A': //A - Send CAN Message
						SendMessageFromDeviceCAN();
						break;

					case 'B': //B - Send CAN CANFD 8 Byte
						SendMessageFromDeviceCANFD(8);
						break;
					case 'C': //C - Send CANFD 64 Byte
						SendMessageFromDeviceCANFD(64);
						break;
					case 'D': //C - Send LIN Master frame
						SendMasterFrame();
						break;
					case 'E': //C - Send LIN Header frame
						SendHeaderOnlyFrame();
						break;
					case 'F': //C - Send LIN Slave frame
						SendSlaveFrame();
						break;
					case 'G': //C - ISO9141 frame
						SendISO9141KW2K();
						break;
					case 'H': //C - Ethernet frame
						SendEthernet();
						break;
					default:
						break;
				}

			
				break;
			case 'E': //E - ISO5765
				printf("\r\n\r\n");
				printf("Press the letter next to the function group you want to use\r\n");
				printf("A - Configure Transmit of Multi Frame\r\n");	
				printf("B - Configure Receive of Mulit Frame\r\n");	
				printf("C - Disable\r\n");			
				printf("X - Exit\r\n");
				printf("\r\n\r\n");
				//Get User intput
				cInputCommand = toupper(_getwch());
		
				switch(cInputCommand) 
				{
					case 'A': //A - Send CAN Message
						ConfigureISO15765Tx();
						break;
					case 'B': //B - Send CAN CANFD 8 Byte
						ConfigureISO15765Rx();
						break;
					case 'C': //C - Send CANFD 64 Byte
						DisableISO15765();
						break;
					default:
						break;
				}			
				break;
			case 'F': //F - Set HS CAN to 250K
				printf("\r\n\r\n");
				printf("Press the letter next to the function group you want to use\r\n");
				printf("A - Set HS CAN to 500K\r\n");	
				printf("B - Set HS CAN to 250K\r\n");
				printf("C - Set HS CAN to 125K\r\n");
				printf("X - Exit\r\n");
				printf("\r\n\r\n");
				
				//Get User intput
				cInputCommand = toupper(_getwch());

				switch(cInputCommand) 
				{
					case 'A': //A - Set Baud to 500K
						SetHSCANBaudRatetoDevice(500000);
						break;
					case 'B': //B - Set Baud to 250K
						SetHSCANBaudRatetoDevice(250000);
						break;
					case 'C': //C - Set Baud to 125K
						SetHSCANBaudRatetoDevice(125000);
						break;
					default:
						break;
				}			

				break;	
			case 'G': //E - Get Errors
				GetErrorsFromDevice();
				break;

			case 'H':  //H - Disconnect from device
				DisconnectFromDevice();
				break;
			case 'X': //X - Exit
				if(m_bPortOpen==true) DisconnectFromDevice();
				UnloadDLLAPI(hDLL);
				bKeepRunning = 0;
				break;
			default : 
				// Process for all other cases.
				break;
		}
		printf("Press Key to Continue");
		cInputCommand = _getwch();
		system("cls");
	}
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void GetMessagesFromDevice(void)
{
long lResult;
int iNumberOfErrors;
long lCount;
int lNumberOfMessages;
double dTime;
long lByteCount;
static icsSpyMessage stMessages[20000];
icsSpyMessageJ1850 icsJ1850Msg;
const unsigned char *ptr;

	//check to see if device is open, if so exit
    if (m_bPortOpen==false)
	{
        printf("neoVI not opened\r\n");
        return;  
	}
	
	//Call Get Messages
	lResult = icsneoGetMessages(m_hObject, stMessages, &lNumberOfMessages, &iNumberOfErrors); //Call get message function
	if(lResult==0)//See if Call was successful
	{
		printf("Problem Getting Messages\r\n");
		return;
	}

	//if there are no messages get out of function
	if(lNumberOfMessages==0) return;

	//Loop to acquire all of the messages in the buffer
	for(lCount=0;lCount<=lNumberOfMessages-1;lCount++)   
	{

		//Get the number of bytes in the message
		lByteCount = stMessages[lCount].NumberBytesData;

		//Calc the Time stamp for the message
		lResult = icsneoGetTimeStampForMsg(m_hObject, &stMessages[lCount], &dTime);
		printf("%f ",dTime);
		
		//check if the message is Tx or RX
		if((stMessages[lCount].StatusBitField & SPY_STATUS_TX_MSG)!=0) 
		{
			printf("Tx ");
		}
		else
		{
			printf("Rx ");
		}

		//Get the network of the message
        switch(stMessages[lCount].NetworkID)
        {
            case NETID_DEVICE:
                printf("DEVICE");
                break;
            case NETID_HSCAN:
                printf("HSCAN");
                break;
            case NETID_MSCAN:
                printf("MSCAN");
                break;
            case NETID_SWCAN:
                printf("SWCAN");
                break;
            case NETID_LSFTCAN:
                printf("LSFTCAN");
                break;
            case NETID_FORDSCP:
                printf("FORDSCP");
                break;
            case NETID_J1708:
                printf("J1708");
                break;
            case NETID_JVPW:
                printf("JVPW");
                break;
            case NETID_ISO:
                printf("ISO");
                break;
            case NETID_ISO2:
                printf("ISO2");
                break;
            case NETID_ISO14230:
                printf("ISO14230");
                break;
            case NETID_LIN:
                printf("LIN");
                break;
            case NETID_ISO3:
                printf("ISO3");
                break;
            case NETID_HSCAN2:
                printf("HSCAN2");
                break;
            case NETID_HSCAN3:
                printf("HSCAN3");
                break;
            case NETID_ISO4:
                printf("ISO4");
                break;
            case NETID_LIN2:
                printf("LIN2");
                break;
            case NETID_LIN3:
                printf("LIN3");
                break;
            case NETID_LIN4:
                printf("LIN4");
                break;
            case NETID_LIN5:
                printf("LIN5");
                break;
            case NETID_MOST:
                printf("MOST");
                break;
            case NETID_CGI:
                printf("CGI");
                break;
            case NETID_HSCAN4:
                printf("HSCAN4");
                break;
            case NETID_HSCAN5:
                printf("HSCAN5");
                break;
            case NETID_UART:
                printf("UART");
                break;
            case NETID_UART2:
                printf("UART2");
                break;
            case NETID_UART3:
                printf("UART3");
                break;
            case NETID_UART4:
                printf("UART4");
                break;
            case NETID_SWCAN2:
                printf("SWCAN2");
                break;
            case NETID_ETHERNET_DAQ:
                printf("ETHERNE");
                break;
            case NETID_TEXTAPI_TO_HOST:
                printf("TEXTAP");
                break;
            case NETID_FLEXRAY1A:
                printf("FLEXRAY1A");
                break;
            case NETID_FLEXRAY1B:
                printf("FLEXRAY1B");
                break;
            case NETID_FLEXRAY2A:
                printf("FLEXRAY2A");
                break;
            case NETID_FLEXRAY2B:
                printf("FLEXRAY2B");
                break;
            case NETID_FLEXRAY:
                printf("FLEXRAY");
                break;
            case NETID_MOST25:
                printf("MOST25");
                break;
            case NETID_MOST50:
                printf("MOST50");
                break;
            case NETID_MOST150:
                printf("MOST150");
                break;
            case NETID_ETHERNET:
                printf("ETHERNET");
                break;
            case NETID_GMFSA:
                printf("GMFSA");
                break;
            case NETID_TCP:
                printf("TCP");
                break;
            case NETID_HSCAN6:
                printf("HSCAN6");
                break;
            case NETID_HSCAN7:
                printf("HSCAN7");
                break;
            case NETID_LIN6:
                printf("LIN6");
                break;
            case NETID_LSFTCAN2:
                printf("LSFTCAN2");
                break;
            case NETID_OP_ETHERNET1:
                printf("OP_ETHERNET1");
                break;
            case NETID_OP_ETHERNET2:
                printf("OP_ETHERNET2");
                break;
            case NETID_OP_ETHERNET3:
                printf("OP_ETHERNET3");
                break;
            case NETID_OP_ETHERNET4:
                printf("OP_ETHERNET4");
                break;
            case NETID_OP_ETHERNET5:
                printf("OP_ETHERNET5");
                break;
            case NETID_OP_ETHERNET6:
                printf("OP_ETHERNET6");
                break;
            case NETID_OP_ETHERNET7:
                printf("OP_ETHERNET7");
                break;
            case NETID_OP_ETHERNET8:
                printf("OP_ETHERNET8");
                break;
            case NETID_OP_ETHERNET9:
                printf("OP_ETHERNET9");
                break;
            case NETID_OP_ETHERNET10:
                printf("OP_ETHERNET10");
                break;
            case NETID_OP_ETHERNET11:
                printf("OP_ETHERNET11");
                break;
            case NETID_OP_ETHERNET12:
                printf("OP_ETHERNET12");
                break;
			}

		//Decode the databytes
		switch(stMessages[lCount].Protocol)
		{
		case SPY_PROTOCOL_CAN:
			//(Classic) CAN messages, get the ArbID and data bytes
			printf(" ArbID-%X ",stMessages[lCount].ArbIDOrHeader);
			if(lByteCount>=1) printf("D %X ",stMessages[lCount].Data[0]);
			if(lByteCount>=2) printf(" %X ",stMessages[lCount].Data[1]);
			if(lByteCount>=3) printf(" %X ",stMessages[lCount].Data[2]);
			if(lByteCount>=4) printf(" %X ",stMessages[lCount].Data[3]);
			if(lByteCount>=5) printf(" %X ",stMessages[lCount].Data[4]);
			if(lByteCount>=6) printf(" %X ",stMessages[lCount].Data[5]);
			if(lByteCount>=7) printf(" %X ",stMessages[lCount].Data[6]);
			if(lByteCount>=8) printf(" %X ",stMessages[lCount].Data[7]);
			break;
		case SPY_PROTOCOL_CANFD:
			//CAN FD 
			//Get the ID
			printf(" ArbID-%X ",stMessages[lCount].ArbIDOrHeader);
			if (stMessages[lCount].ExtraDataPtrEnabled == 0)
			{
				//8 bytes of data, messages with 0 to 8 bytes of data are different than Longer frames.
				//Here is how to acquire 0 to 8
				if(lByteCount>=1) printf("D %X ",stMessages[lCount].Data[0]);
				if(lByteCount>=2) printf(" %X ",stMessages[lCount].Data[1]);
				if(lByteCount>=3) printf(" %X ",stMessages[lCount].Data[2]);
				if(lByteCount>=4) printf(" %X ",stMessages[lCount].Data[3]);
				if(lByteCount>=5) printf(" %X ",stMessages[lCount].Data[4]);
				if(lByteCount>=6) printf(" %X ",stMessages[lCount].Data[5]);
				if(lByteCount>=7) printf(" %X ",stMessages[lCount].Data[6]);
				if(lByteCount>=8) printf(" %X ",stMessages[lCount].Data[7]);
			}
			else
			{
				//More bytes are done using Extended Data pointer.
				ptr = (const unsigned char*)stMessages[lCount].ExtraDataPtr;
				for(int j=0;j< stMessages[lCount].NumberBytesData;j++)
				{
					printf("%0x\t",*(ptr++));
				}
					printf("\n");
				}

			break;
		default:
			//Other non CAN message types.  
			memcpy((void*)&icsJ1850Msg,(void*)&stMessages[lCount],sizeof(icsSpyMessageJ1850));
			
			printf(" Header-%X %X %X ",icsJ1850Msg.Header[0],icsJ1850Msg.Header[1],icsJ1850Msg.Header[2]);
			if(lByteCount>=1) printf("D %X ",icsJ1850Msg.Data[0]);
			if(lByteCount>=2) printf(" %X",icsJ1850Msg.Data[1]);
			if(lByteCount>=3) printf(" %X ",icsJ1850Msg.Data[2]);
			if(lByteCount>=4) printf(" %X ",icsJ1850Msg.Data[3]);
			if(lByteCount>=5) printf(" %X ",icsJ1850Msg.Data[4]);
			if(lByteCount>=6) printf(" %X ",icsJ1850Msg.Data[5]);
			if(lByteCount>=7) printf(" %X ",icsJ1850Msg.Data[6]);
			if(lByteCount>=8) printf(" %X ",icsJ1850Msg.Data[7]);
			break;
		}
		printf("\r\n");
	}
	//Display the number of messages and errors
	printf("Number Read : %d\r\n",lNumberOfMessages);
	printf("Number Errors : %d\r\n",iNumberOfErrors);
	printf("\r\n");
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void ConfigureISO15765Tx(void)
{
		long lResult;
        int iNetwork = 1;
		int iCounter = 0;
        
		//make sure the device is open
		if (m_bPortOpen==false)
		{
	        printf("neoVI not opened\r\n");
			return;  
		}

        //Create Message and adjust array to proper size
		stCM_ISO157652_TxMessage tx_msg = {};
		::ZeroMemory(&tx_msg,sizeof(stCM_ISO157652_TxMessage));
        //enable ISO15765 on HS CAN
        lResult = icsneoISO15765_EnableNetworks(m_hObject,NETID_HSCAN);

        tx_msg.id = 0x7E0;  //ArbID of the message
        tx_msg.vs_netid = NETID_HSCAN;  //Network to use
        tx_msg.num_bytes = 20; //The number of data bytes to use
        tx_msg.padding = 0xAA;//Set Padding Byte


		//Set the Flags
		tx_msg.flags = 0;
		tx_msg.paddingEnable = true;
		tx_msg.id_29_bit_enable = false;
		tx_msg.fc_id_29_bit_enable = false;
		tx_msg.ext_address_enable = false;
		tx_msg.fc_ext_address_enable = false;
		tx_msg.overrideSTmin = false;
		tx_msg.overrideBlockSize = false;
		tx_msg.iscanFD = false; //CAN FD
		tx_msg.isBRSEnabled=false; //CAN FD
		tx_msg.tx_dl = 8;

		//Set the Flow control message
        tx_msg.fc_id = 0x7E8; //ArbID for the flow control Frame
        tx_msg.fc_id_mask = 0xFFF; //The flow control arb filter mask (response id from receiver)
        tx_msg.flowControlExtendedAddress = 0xFE; //Extended ID
        tx_msg.fs_timeout = 0x100;  //Flow Control Time out in ms
        tx_msg.fs_wait = 0x3000; //Flow Control FS=Wait Timeout ms
		
        tx_msg.blockSize = 0; //Block size (for sending flow Control)
        tx_msg.stMin = 0;

        //Fill data
		for(iCounter = 0;iCounter<20;iCounter++)
		{
			tx_msg.data[iCounter] = iCounter;
		}

        lResult = icsneoISO15765_TransmitMessage(m_hObject, iNetwork, &tx_msg, 3000);
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void ConfigureISO15765Rx(void)
{

		int lResult;
        stCM_ISO157652_RxMessage flow_cntrl_msg;


		//make sure the device is open
		if (m_bPortOpen==false)
		{
	        printf("neoVI not opened\r\n");
			return;  
		}
		
		memset(&flow_cntrl_msg,0,sizeof(flow_cntrl_msg));

        lResult = icsneoISO15765_EnableNetworks(m_hObject, NETID_HSCAN);

        //Build structure for message
		flow_cntrl_msg.vs_netid = NETID_HSCAN;
        flow_cntrl_msg.padding = 0xAA; //Set Padding Byte
        flow_cntrl_msg.id = 0x7E0;  //ArbID of the message
        flow_cntrl_msg.id_mask = 0xFFF; //The flow control arb filter mask (response id from receiver)
        flow_cntrl_msg.fc_id = 0x7E8; //ArbID for the flow control Frame
        flow_cntrl_msg.blockSize = 100;
        flow_cntrl_msg.stMin = 100;
        flow_cntrl_msg.cf_timeout = 1000;


        //Set flags for Padding and ID information.
		flow_cntrl_msg.paddingEnable = true;
		flow_cntrl_msg.id_29_bit_enable = false;
		flow_cntrl_msg.fc_id_29_bit_enable = false;
		flow_cntrl_msg.ext_address_enable = false;
		flow_cntrl_msg.fc_ext_address_enable = false;
		///flow_cntrl_msg.overrideSTmin = false;
		///flow_cntrl_msg.overrideBlockSize = false;
		flow_cntrl_msg.iscanFD = false;  //CAN FD Enables
		flow_cntrl_msg.isBRSEnabled = false; //CAN FD Enables
		
        lResult = icsneoISO15765_ReceiveMessage(m_hObject, 0, &flow_cntrl_msg);
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void DisableISO15765(void)
{
	long lResult;

	//disable the configuration for icsneoISO15765 in hardware
    lResult = icsneoISO15765_DisableNetworks(m_hObject);
	printf("ISO15765 Disabled!\r\n");

}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void SendMasterFrame(void)
{
	    long lResult;
        icsSpyMessageJ1850 stJMsg;
        int lNetworkID;

		//make sure the device is open
		if (m_bPortOpen==false)
		{
	        printf("neoVI not opened\r\n");
			return;  
		}
        //This message type will send out the break, header, and Data.

        //Set message as Master frame
        stJMsg.StatusBitField = SPY_STATUS_INIT_MESSAGE; //<---- Key difference between master and slave message
		stJMsg.StatusBitField2 = 0;
		stJMsg.StatusBitField3 = 0;
		stJMsg.StatusBitField4 = 0;
        // Set the network (LIN 1 in this case)
        lNetworkID = NETID_LIN;

        //Protected ID, If we have the standard ID, it would have to be converted.
        stJMsg.Header[0] = 0xB1; //The lin ID goes into the First header bytes

		//Data bytes 1 and 2 are in the header section.
        stJMsg.Header[1] = 0x01;
		stJMsg.Header[2] = 0x02;
        //Copy the data
        stJMsg.Data[0] = 0x03;
        stJMsg.Data[1] = 0x04;
        stJMsg.Data[2] = 0x05;
        stJMsg.Data[3] = 0x06;
        stJMsg.Data[4] = 0x07;
        stJMsg.Data[5] = 0x08;
		//Checksum fillows the last data bytes
		stJMsg.Data[6] = 0xDB;

		stJMsg.NumberBytesData = 7;
		stJMsg.NumberBytesHeader = 3;
		
        // Transmit the assembled message
        lResult = icsneoTxMessages(m_hObject, (icsSpyMessage*)&stJMsg, lNetworkID, 1);
        printf("Message Sent!\r\n");
}
//----------------------------------------------------------------------------
void SendHeaderOnlyFrame(void)
{
	    long lResult;
        icsSpyMessageJ1850 stJMsg;
        int lNetworkID;

		//make sure the device is open
		if (m_bPortOpen==false)
		{
	        printf("neoVI not opened\r\n");
			return;  
		}
        //This message type will send out the break and the header.  The data will come back from the slave ECU (if connected)

        //Set message as Master frame
        stJMsg.StatusBitField = SPY_STATUS_INIT_MESSAGE;
        // Set the network (LIN 1 in this case)
        lNetworkID = NETID_LIN;

        stJMsg.NumberBytesHeader = 1;
        // for all the header bytes
        stJMsg.Header[0] = 0xB1;
        
        //No data bytes for this type of message.  Also the slave will have the checksum.
        stJMsg.NumberBytesData = 0;

        // Transmit the assembled message
        lResult = icsneoTxMessages(m_hObject,(icsSpyMessage*)&stJMsg, lNetworkID, 1);
        printf("Message Sent!\r\n");
}
//----------------------------------------------------------------------------
void SendISO9141KW2K(void)
{
		long lResult;
        icsSpyMessageJ1850 stMessagesTxJ1850;
        int lNetworkID;

		//make sure the device is open
		if (m_bPortOpen==false)
		{
	        printf("neoVI not opened\r\n");
			return;  
		}

        // Read the Network we will transmit on
        lNetworkID = NETID_ISO;


        // load the message structure
        //copy headder bytes
        stMessagesTxJ1850.Header[0] = 0xAA;
        stMessagesTxJ1850.Header[1] = 0xBB;
        stMessagesTxJ1850.Header[2] = 0xCC;
        stMessagesTxJ1850.NumberBytesHeader = 3;
        stMessagesTxJ1850.NumberBytesData = 7;      // The number of Data Bytes
        // Load all of the data bytes in the structure
        stMessagesTxJ1850.Data[0] = 0x11;
        stMessagesTxJ1850.Data[1] = 0x22;
        stMessagesTxJ1850.Data[2] = 0x33;
        stMessagesTxJ1850.Data[3] = 0x44;
        stMessagesTxJ1850.Data[4] = 0x55;
        stMessagesTxJ1850.Data[5] = 0x66;
        stMessagesTxJ1850.Data[6] = 0x77;
        
        // Transmit the assembled message
	lResult = icsneoTxMessages(m_hObject, (icsSpyMessage*)&stMessagesTxJ1850, lNetworkID, 1);
	printf("Message Sent!\r\n");
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void SendEthernet(void)
{
	    //This code block is for a FIRE 2.  Using a different network would make this work for RAD Galaxy NETID_OP_ETHERNET1 for example. 
	
        long lResult;
        icsSpyMessage stMessagesTx;
        int lNetworkID;
        unsigned int iNumberTxed;
   		unsigned char iDataBytes[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
			17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38
			,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,
			61,62,63}; //Data to send

		//make sure the device is open
		if (m_bPortOpen==false)
		{
	        printf("neoVI not opened\r\n");
			return;  
		}

        //Send on Ethernet (FIRE 2)
        lNetworkID = NETID_ETHERNET;

        //No ID to set
        stMessagesTx.ArbIDOrHeader = 0x0;

		stMessagesTx.NumberBytesHeader = 0;
		stMessagesTx.StatusBitField = 0;

        stMessagesTx.Protocol = SPY_PROTOCOL_ETHERNET;
        stMessagesTx.StatusBitField3 = 0; //Enable bitrate switch

		//Long Message
		stMessagesTx.NumberBytesData = 64;
		//Enable Extra Data Pointer  and point to the data bytes
        stMessagesTx.ExtraDataPtrEnabled = 1;
		stMessagesTx.ExtraDataPtr = &iDataBytes;
        lResult = icsneoTxMessagesEx(m_hObject, &stMessagesTx, lNetworkID, 1, &iNumberTxed, 0);

		printf("Message Sent!\r\n");
	
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void SendSlaveFrame(void)
{
		long lResult;
        icsSpyMessageJ1850 stJMsg;
        int lNetworkID;

		//make sure the device is open
		if (m_bPortOpen==false)
		{
	        printf("neoVI not opened\r\n");
			return;  
		}

        //This message type will send out the break, header, and Data.

        //Set message as Master frame
        stJMsg.StatusBitField = 0;  //<---- Key difference between master and slave message

        // Set the network (LIN 1 in this case)
        lNetworkID = NETID_LIN;

        //Protected ID, If we have the standard ID, it would have to be converted.
        stJMsg.Header[0] = 0xB1; //The lin ID goes into the First header bytes

		//Data bytes 1 and 2 are in the header section.
        stJMsg.Header[1] = 0x01;
		stJMsg.Header[2] = 0x02;
        //Copy the data
        stJMsg.Data[0] = 0x03;
        stJMsg.Data[1] = 0x04;
        stJMsg.Data[2] = 0x05;
        stJMsg.Data[3] = 0x06;
        stJMsg.Data[4] = 0x07;
        stJMsg.Data[5] = 0x08;
		//Checksum fillows the last data bytes
		stJMsg.Data[6] = 0xDB;

		stJMsg.NumberBytesData = 7;
		stJMsg.NumberBytesHeader = 3; 

        // Transmit the assembled message
        lResult = icsneoTxMessages(m_hObject,(icsSpyMessage*)&stJMsg, lNetworkID, 1);
        printf("Message Sent!\r\n");
}
//----------------------------------------------------------------------------

void SendMessageFromDeviceCAN(void)
{
				     
long lResult; 
icsSpyMessage stMessagesTx;
icsSpyMessage stMessagesTx2;
long lNetworkID;

	//make sure the device is open
	if (m_bPortOpen==false)
	{
        printf("neoVI not opened\r\n");
        return;  
	}

    //Send on HS CAN
	lNetworkID = NETID_HSCAN; 
        
	//Set the ID
	stMessagesTx.ArbIDOrHeader = 0x101; 
	stMessagesTx2.ArbIDOrHeader = 0x102;


	// The number of Data Bytes
	stMessagesTx.NumberBytesData = 8;
	stMessagesTx2.NumberBytesData = 1;


	// Load all of the data bytes in the structure
	stMessagesTx.Data[0] = 0x00;
	stMessagesTx.Data[1] = 0x50; // Spedometer
	stMessagesTx.Data[2] = 0x0B;
	stMessagesTx.Data[3] = 0xB8; // RPM
	stMessagesTx.Data[4] = 0x00;
	stMessagesTx.Data[5] = 0x32; // Fuel lvl
	stMessagesTx.Data[6] = 0xFF;
	stMessagesTx.Data[7] = 0xFF; // Brightness

	stMessagesTx2.Data[0] = 0xFF;
	stMessagesTx2.Data[1] = 0xFF; // Transmission

	stMessagesTx.StatusBitField = 0;	// not extended or remote frame
	stMessagesTx.StatusBitField2 = 0;
	stMessagesTx2.StatusBitField = 0;	// not extended or remote frame
	stMessagesTx2.StatusBitField2 = 0;

	// Transmit the assembled message
	lResult = icsneoTxMessages(m_hObject, &stMessagesTx, lNetworkID, 1);
	lResult = icsneoTxMessages(m_hObject, &stMessagesTx2, lNetworkID, 1);
	printf("Message Sent!\r\n");
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void SendMessageFromDeviceCANFD(int ByteCount)
{
	    //Hardware must have CAN FD support for this tor work. 
        long lResult;
        icsSpyMessage stMessagesTx;
        int lNetworkID;
        unsigned int iNumberTxed;
		unsigned char iDataBytes[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63}; //Data to send

		//make sure the device is open
		if (m_bPortOpen==false)
		{
	        printf("neoVI not opened\r\n");
			return;  
		}

        //Send on HS CAN
        lNetworkID = NETID_HSCAN;

        //Set the ID
        stMessagesTx.ArbIDOrHeader = 0x125;

		stMessagesTx.NumberBytesHeader = 0;
		//stMessagesTx.StatusBitField = icsSpyDataStatusBitfield.SPY_STATUS_XTD_FRAME; //Extended ID
		stMessagesTx.StatusBitField = 0; //11 bit ID

        stMessagesTx.Protocol = SPY_PROTOCOL_CANFD;
        stMessagesTx.StatusBitField3 = 16; //Enable bitrate switch

        // The number of Data Bytes
		//NOTE: CAN FD is limited to lengths of 0,1,2,3,4,5,6,7,8,12,16,20,24,32,48,64
		if(ByteCount==64)
		{
			//Long Message
			stMessagesTx.NumberBytesData = 64;
			//Enable Extra Data Pointer 
            stMessagesTx.ExtraDataPtrEnabled = 1;
			stMessagesTx.ExtraDataPtr = &iDataBytes;
            lResult = icsneoTxMessagesEx(m_hObject, &stMessagesTx, lNetworkID, 1, &iNumberTxed, 0);
			
		}
		else
		{
			//8 bytes
			stMessagesTx.NumberBytesData = 8;
			stMessagesTx.Data[0] = 0x08;
			stMessagesTx.Data[1] = 0x19;
			stMessagesTx.Data[2] = 0x2A;
			stMessagesTx.Data[3] = 0x3B;
			stMessagesTx.Data[4] = 0x4C;
			stMessagesTx.Data[5] = 0x5D;
			stMessagesTx.Data[6] = 0x6E;
			stMessagesTx.Data[7] = 0x7F;
			stMessagesTx.ExtraDataPtrEnabled = 0;
            lResult = icsneoTxMessages(m_hObject, &stMessagesTx, lNetworkID, 1);
		}
		printf("Message Sent!\r\n");
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void GetErrorsFromDevice(void)
{
int lResult=0;
int iErrors[600]={0};
int lNumberOfErrors=0;
int lCount;
char szDescriptionShort[255]={char(0)};
char szDescriptionLong[255]={char(0)};
int lMaxLengthShort,lMaxLengthLong,lErrorSeverity,lRestartNeeded;


	// Read the errors from the DLL
	lResult = icsneoGetErrorMessages(m_hObject,iErrors,&lNumberOfErrors);
	
	if (lResult == 0)
	{
	    printf("Problem Reading errors\r\n");
	}

	// dump the neoVI errors
	if (lNumberOfErrors > 0)
	{    
	    for (lCount=0;lCount <lNumberOfErrors;lCount++)
	    {
			lMaxLengthShort = 255;
			lMaxLengthLong = 255;
			icsneoGetErrorInfo(iErrors[lCount], szDescriptionShort, szDescriptionLong, &lMaxLengthShort,&lMaxLengthLong, &lErrorSeverity, &lRestartNeeded);
			printf("Error - %s\r\n",szDescriptionShort);
	    }
	}
	else
	{
		printf("No Errors to report\r\n");			
	}

}
//----------------------------------------------------------------------------
void SetHSCANBaudRatetoDevice(int iRateToUse)
{

int iNetworkID;
int iResult; 

	//Make sure the device is open
	if (m_bPortOpen==false)
	{
		printf("neoVI not opened\r\n");
		return;   
	}

	//Get the network index to set the baud rate to use
	iNetworkID = NETID_HSCAN;
	
	//Set the bit rate
	iResult = icsneoSetBitRate(m_hObject, iRateToUse, iNetworkID);
	if (iResult == 0)  
	{
		printf("Problem setting bit rate\r\n");
	}
	else
	{
		printf("Baudrate Set\r\n");
	}

}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void DisconnectFromDevice(void)
{
int iResult;
int iNumberOfErrors;

	//Call close port
	iResult = icsneoClosePort(m_hObject,&iNumberOfErrors);
	if(iResult==0)
	{
		printf("Problem Closing Port\r\n");
	}
	else
	{
		printf("Port Closed\r\n");
	}
	//clear open flag
	m_bPortOpen = false;
}
//----------------------------------------------------------------------------
void ConnectToFirstDevice(void)
{
int iResult; 
NeoDevice ndNeoToOpen[10];
int iNumberOfDevices;
string sTempString;
char cSNString [8]={NULL};
		
	//Make sure the device is NOT open
	if (m_bPortOpen==true)
	{
		printf("Device already opened\r\n");
		return;   
	}

	//search for the first device
	iNumberOfDevices = 1;
	iResult = icsneoFindNeoDevices(NEODEVICE_ALL, ndNeoToOpen, &iNumberOfDevices);
	if(iResult == false)
	{
		printf("Problem Finding Device\r\n");
		return;
	}

	//Check for the number of devices
	if(iNumberOfDevices<1)
	{
		printf("No Devices Found\r\n");
		return;
	}
		
	m_hObject = NULL;
	m_hObject = 0;
	//Connect to the first device found
	iResult = icsneoOpenNeoDevice(&ndNeoToOpen[0], &m_hObject, NULL,1,0);
	if(iResult==false)
	{
		printf("Problem Opening Port\r\n");
		return;
	}
	printf("Port Opened OK!\r\n");

	//Display the type of device
	switch(ndNeoToOpen[0].DeviceType)
	{
	case NEODEVICE_BLUE:
		sTempString = "neoVI Blue SN ";
		printf("Device %s %d is connected\r\n",sTempString.c_str(),ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_DW_VCAN:
		sTempString= "ValueCAN 2 SN ";
		printf("Device %s %d is connected\r\n",sTempString.c_str(),ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_FIRE:
		sTempString= "neoVI FIRE SN ";
		printf("Device %s %d is connected\r\n",sTempString.c_str(),ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_VCAN3:
		sTempString= "ValueCAN 3 SN ";
		printf("Device %s %d is connected\r\n",sTempString.c_str(),ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_YELLOW:
		sTempString= "neoVI Yellow SN ";
		printf("Device %s %d is connected\r\n",sTempString.c_str(),ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_FIRE2:
		sTempString= "neoVI FIRE2 SN ";
		icsneoSerialNumberToString(ndNeoToOpen[0].SerialNumber,cSNString,6);
		printf("Device %s %s is connected\r\n",sTempString.c_str(),cSNString);
		break;
	case NEODEVICE_RADGALAXY:
		sTempString= "RADGalaxy SN ";
		icsneoSerialNumberToString(ndNeoToOpen[0].SerialNumber,cSNString,6);
		printf("Device %s %s is connected\r\n",sTempString.c_str(),cSNString);
		break;
	case NEODEVICE_VCANRF:
		sTempString= "ValueCAN.rf SN ";
		icsneoSerialNumberToString(ndNeoToOpen[0].SerialNumber,cSNString,6);
		printf("Device %s %s is connected\r\n",sTempString.c_str(),cSNString);
		break;
	case NEODEVICE_PLASMA_1_11:
	case NEODEVICE_PLASMA_1_12:
	case NEODEVICE_PLASMA_1_13:
		sTempString= "neoVI Plasma SN ";
		printf("Device %s %d is connected\r\n",sTempString.c_str(),ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_ION_2:
	case NEODEVICE_ION_3:
		sTempString= "neoVI ION SN ";
		printf("Device %s %d is connected\r\n",sTempString.c_str(),ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_VCAN412:
		sTempString= "VCAN4-12 SN ";
		icsneoSerialNumberToString(ndNeoToOpen[0].SerialNumber,cSNString,6);
		printf("Device %s %s is connected\r\n",sTempString.c_str(),cSNString);
		break;
	default:
		sTempString = "Unknown neoVI SN ";
		printf("Device %s %d is connected\r\n",sTempString.c_str(),ndNeoToOpen[0].SerialNumber);
	}

	m_bPortOpen = true;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void ListDevices(void)
{
int iResult; 
NeoDevice ndNeoToOpen [10];
int iNumberOfDevices;
int iCounter;
string sTempString;
char cSNString [8]={NULL};
		 

	iNumberOfDevices = 10;
	//Search for attached devices
	iResult = icsneoFindNeoDevices(NEODEVICE_ALL, ndNeoToOpen, &iNumberOfDevices);
	if(iResult == false)
	{
		printf("Problem Finding Device\r\n");
		return;
	}

	if(iNumberOfDevices<1)
	{
		printf("No Devices Found\r\n");
		return;
	}

	//Print list of devices
	for(iCounter = 0;iCounter < iNumberOfDevices;iCounter++)
	{
			switch(ndNeoToOpen[iCounter].DeviceType)
			{
			case NEODEVICE_BLUE:
				sTempString = "neoVI Blue SN ";
				printf("%s %d\r\n",sTempString.c_str(),ndNeoToOpen[iCounter].SerialNumber);
				break;
			case NEODEVICE_DW_VCAN:
				sTempString= "ValueCAN 2 SN ";
				printf("%s %d\r\n",sTempString.c_str(),ndNeoToOpen[iCounter].SerialNumber);
				break;
			case NEODEVICE_FIRE:
				sTempString= "neoVI FIRE SN ";
				printf("%s %d\r\n",sTempString.c_str(),ndNeoToOpen[iCounter].SerialNumber);
				break;
			case NEODEVICE_VCAN3:
				sTempString= "ValueCAN 3 SN ";
				printf("%s %d\r\n",sTempString.c_str(),ndNeoToOpen[iCounter].SerialNumber);
				break;
			case NEODEVICE_YELLOW:
				sTempString= "neoVI Yellow SN ";
				printf("%s %d\r\n",sTempString.c_str(),ndNeoToOpen[iCounter].SerialNumber);
				break;
			case NEODEVICE_FIRE2:
				sTempString= "neoVI FIRE2 SN ";
				icsneoSerialNumberToString(ndNeoToOpen[iCounter].SerialNumber,cSNString,6);
				printf("%s %s\r\n",sTempString.c_str(),cSNString);
				break;
			case NEODEVICE_RADGALAXY:
				sTempString= "RADGalaxy SN ";
				icsneoSerialNumberToString(ndNeoToOpen[iCounter].SerialNumber,cSNString,6);
				printf("%s %s\r\n",sTempString.c_str(),cSNString);
				break;
			case NEODEVICE_VCANRF:
				sTempString= "ValueCAN.rf SN ";
				icsneoSerialNumberToString(ndNeoToOpen[iCounter].SerialNumber,cSNString,6);
				printf("%s %s\r\n",sTempString.c_str(),cSNString);
				break;
			case NEODEVICE_PLASMA_1_11:
			case NEODEVICE_PLASMA_1_12:
			case NEODEVICE_PLASMA_1_13:
				sTempString= "neoVI Plasma SN ";
				printf("%s %d\r\n",sTempString.c_str(),ndNeoToOpen[iCounter].SerialNumber);
				break;
			case NEODEVICE_ION_2:
			case NEODEVICE_ION_3:
				sTempString= "neoVI ION SN ";
				printf("%s %d\r\n",sTempString.c_str(),ndNeoToOpen[iCounter].SerialNumber);
				break;
			case NEODEVICE_VCAN412:
				sTempString= "VCAN4-12 SN ";
				icsneoSerialNumberToString(ndNeoToOpen[iCounter].SerialNumber,cSNString,6);
				printf("%s %s\r\n",sTempString.c_str(),cSNString);
				break;
			default:
				sTempString = "Unknown neoVI SN ";
				printf("%s %d\r\n",sTempString.c_str(),ndNeoToOpen[iCounter].SerialNumber);
			}
			
	}
}

//----------------------------------------------------------------------------

