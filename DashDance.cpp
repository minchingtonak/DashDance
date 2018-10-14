#include "stdafx.h"
#include "icsneo40DLLAPI.h"
#include "DashDance.h"
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <istream>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;
using std::chrono::high_resolution_clock;
using std::printf;

void * m_hObject; //handle for device
bool m_bPortOpen; //tells the port status of the device
byte NetworkIDConvert[64]; //Stores the netIDs used

vector<int> channel;

HINSTANCE hDLL;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	char cInputCommand;
	bool bKeepRunning = 1;
	int iVerNumber;

	ifstream leftAudio;
	leftAudio.open("unitedwedance.txt");

	if (!leftAudio.is_open()) return 1;

	string left, right;
	while (leftAudio >> left >> right) {
		// Multiply by 100 to avoid integer truncation to 0
		channel.push_back(static_cast<int>(stof(left) * 100.0));
	}

	cout << "Song size: " << channel.size() << endl;

	//Load DLL functions
	if (!LoadDLLAPI(hDLL))
	{
		//problem, close the application
		printf("Problem loading Library\r\nMake sure icsneo40.dll is installed and accessable\r\nPress any key to Exit");
		cInputCommand = _getwch();
		return(1);
	}

	system("Color A"); //Change window color

	//Get the DLL Version number
	iVerNumber = icsneoGetDLLVersion();
	printf("ICS icsneo40.dll version %d\r\n\r\n", iVerNumber);

	while (bKeepRunning)
	{
		//Main Menu
		printf("\r\n\r\n");
		printf("Press the letter next to the function group you want to use\r\n");
		printf("A - List Attached devices\r\n");
		printf("B - Connect to the first\r\n");
		printf("C - Get Messages\r\n");
		printf("D - Play Song\r\n");
		printf("H - Disconnect\r\n");
		printf("X - Exit\r\n");
		printf("\r\n\r\n");
		//Get User intput
		cInputCommand = toupper(_getwch());

		//Find command chosen
		switch (cInputCommand)
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
			SendMessageFromDeviceCAN();
			break;
		case 'H':  //H - Disconnect from device
			DisconnectFromDevice();
			break;
		case 'X': //X - Exit
			if (m_bPortOpen == true) DisconnectFromDevice();
			UnloadDLLAPI(hDLL);
			bKeepRunning = 0;
			break;
		default:
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
	if (m_bPortOpen == false)
	{
		printf("neoVI not opened\r\n");
		return;
	}

	//Call Get Messages
	lResult = icsneoGetMessages(m_hObject, stMessages, &lNumberOfMessages, &iNumberOfErrors); //Call get message function
	if (lResult == 0)//See if Call was successful
	{
		printf("Problem Getting Messages\r\n");
		return;
	}

	//if there are no messages get out of function
	if (lNumberOfMessages == 0) return;

	//Loop to acquire all of the messages in the buffer
	for (lCount = 0; lCount <= lNumberOfMessages - 1; lCount++)
	{

		//Get the number of bytes in the message
		lByteCount = stMessages[lCount].NumberBytesData;

		//Calc the Time stamp for the message
		lResult = icsneoGetTimeStampForMsg(m_hObject, &stMessages[lCount], &dTime);
		printf("%f ", dTime);

		//check if the message is Tx or RX
		if ((stMessages[lCount].StatusBitField & SPY_STATUS_TX_MSG) != 0)
		{
			printf("Tx ");
		}
		else
		{
			printf("Rx ");
		}

		//Get the network of the message
		switch (stMessages[lCount].NetworkID)
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
		switch (stMessages[lCount].Protocol)
		{
		case SPY_PROTOCOL_CAN:
			//(Classic) CAN messages, get the ArbID and data bytes
			printf(" ArbID-%X ", stMessages[lCount].ArbIDOrHeader);
			if (lByteCount >= 1) printf("D %X ", stMessages[lCount].Data[0]);
			if (lByteCount >= 2) printf(" %X ", stMessages[lCount].Data[1]);
			if (lByteCount >= 3) printf(" %X ", stMessages[lCount].Data[2]);
			if (lByteCount >= 4) printf(" %X ", stMessages[lCount].Data[3]);
			if (lByteCount >= 5) printf(" %X ", stMessages[lCount].Data[4]);
			if (lByteCount >= 6) printf(" %X ", stMessages[lCount].Data[5]);
			if (lByteCount >= 7) printf(" %X ", stMessages[lCount].Data[6]);
			if (lByteCount >= 8) printf(" %X ", stMessages[lCount].Data[7]);
			break;
		case SPY_PROTOCOL_CANFD:
			//CAN FD 
			//Get the ID
			printf(" ArbID-%X ", stMessages[lCount].ArbIDOrHeader);
			if (stMessages[lCount].ExtraDataPtrEnabled == 0)
			{
				//8 bytes of data, messages with 0 to 8 bytes of data are different than Longer frames.
				//Here is how to acquire 0 to 8
				if (lByteCount >= 1) printf("D %X ", stMessages[lCount].Data[0]);
				if (lByteCount >= 2) printf(" %X ", stMessages[lCount].Data[1]);
				if (lByteCount >= 3) printf(" %X ", stMessages[lCount].Data[2]);
				if (lByteCount >= 4) printf(" %X ", stMessages[lCount].Data[3]);
				if (lByteCount >= 5) printf(" %X ", stMessages[lCount].Data[4]);
				if (lByteCount >= 6) printf(" %X ", stMessages[lCount].Data[5]);
				if (lByteCount >= 7) printf(" %X ", stMessages[lCount].Data[6]);
				if (lByteCount >= 8) printf(" %X ", stMessages[lCount].Data[7]);
			}
			else
			{
				//More bytes are done using Extended Data pointer.
				ptr = (const unsigned char*)stMessages[lCount].ExtraDataPtr;
				for (int j = 0; j < stMessages[lCount].NumberBytesData; j++)
				{
					printf("%0x\t", *(ptr++));
				}
				printf("\n");
			}

			break;
		default:
			//Other non CAN message types.  
			memcpy((void*)&icsJ1850Msg, (void*)&stMessages[lCount], sizeof(icsSpyMessageJ1850));

			printf(" Header-%X %X %X ", icsJ1850Msg.Header[0], icsJ1850Msg.Header[1], icsJ1850Msg.Header[2]);
			if (lByteCount >= 1) printf("D %X ", icsJ1850Msg.Data[0]);
			if (lByteCount >= 2) printf(" %X", icsJ1850Msg.Data[1]);
			if (lByteCount >= 3) printf(" %X ", icsJ1850Msg.Data[2]);
			if (lByteCount >= 4) printf(" %X ", icsJ1850Msg.Data[3]);
			if (lByteCount >= 5) printf(" %X ", icsJ1850Msg.Data[4]);
			if (lByteCount >= 6) printf(" %X ", icsJ1850Msg.Data[5]);
			if (lByteCount >= 7) printf(" %X ", icsJ1850Msg.Data[6]);
			if (lByteCount >= 8) printf(" %X ", icsJ1850Msg.Data[7]);
			break;
		}
		printf("\r\n");
	}
	//Display the number of messages and errors
	printf("Number Read : %d\r\n", lNumberOfMessages);
	printf("Number Errors : %d\r\n", iNumberOfErrors);
	printf("\r\n");
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void SendMessageFromDeviceCAN()
{
	long lResult;
	icsSpyMessage stMessagesTx;
	long lNetworkID;

	//make sure the device is open
	if (m_bPortOpen == false)
	{
		printf("neoVI not opened\r\n");
		return;
	}

	//Send on HS CAN
	lNetworkID = NETID_HSCAN;

	//Set the ID
	stMessagesTx.ArbIDOrHeader = 0x101;


	// The number of Data Bytes
	stMessagesTx.NumberBytesData = 8;

	stMessagesTx.StatusBitField = 0;	// not extended or remote frame
	stMessagesTx.StatusBitField2 = 0;

	const double dur = 46.39858; //msecs

	printf("Starting song...\r\n");

	for (auto i : channel) {
		auto start = high_resolution_clock::now();
		// Load all of the data bytes in the structure

		uint16_t speed = (int)((double)i * 1.6);
		uint8_t speed1 = (speed >> 8) & 0xFF;
		uint8_t speed2 = speed & 0xFF;
		stMessagesTx.Data[0] = speed2;
		stMessagesTx.Data[1] = speed1; // Spedometer

		uint16_t rpm = i * 50;
		uint8_t rpm1 = (rpm >> 8) & 0xFF;
		uint8_t rpm2 = rpm & 0xFF;
		stMessagesTx.Data[2] = rpm2;
		stMessagesTx.Data[3] = rpm1; // RPM

		uint16_t fuel = i;
		uint8_t fuel1 = (fuel >> 8) & 0xFF;
		uint8_t fuel2 = fuel & 0xFF;
		stMessagesTx.Data[4] = fuel2;
		stMessagesTx.Data[5] = fuel1; // Fuel level

		uint16_t light = (int)((double)i * 6553.5);
		uint8_t light1 = (light >> 8) & 0xFF;
		uint8_t light2 = light & 0xFF;
		stMessagesTx.Data[6] = light2;
		stMessagesTx.Data[7] = light1; // Brightness

		// Transmit the assembled message
		lResult = icsneoTxMessages(m_hObject, &stMessagesTx, lNetworkID, 1);
		auto elapsed = high_resolution_clock::now() - start;
		long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
		Sleep(dur - milliseconds);
	}

	printf("SONG SENDING FINISHED!\r\n");

}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void DisconnectFromDevice(void)
{
	int iResult;
	int iNumberOfErrors;

	//Call close port
	iResult = icsneoClosePort(m_hObject, &iNumberOfErrors);
	if (iResult == 0)
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
//----------------------------------------------------------------------------
void ConnectToFirstDevice(void)
{
	int iResult;
	NeoDevice ndNeoToOpen[10];
	int iNumberOfDevices;
	string sTempString;
	char cSNString[8] = { NULL };

	//Make sure the device is NOT open
	if (m_bPortOpen == true)
	{
		printf("Device already opened\r\n");
		return;
	}

	//search for the first device
	iNumberOfDevices = 1;
	iResult = icsneoFindNeoDevices(NEODEVICE_ALL, ndNeoToOpen, &iNumberOfDevices);
	if (iResult == false)
	{
		printf("Problem Finding Device\r\n");
		return;
	}

	//Check for the number of devices
	if (iNumberOfDevices < 1)
	{
		printf("No Devices Found\r\n");
		return;
	}

	m_hObject = NULL;
	m_hObject = 0;
	//Connect to the first device found
	iResult = icsneoOpenNeoDevice(&ndNeoToOpen[0], &m_hObject, NULL, 1, 0);
	if (iResult == false)
	{
		printf("Problem Opening Port\r\n");
		return;
	}
	printf("Port Opened OK!\r\n");

	//Display the type of device
	switch (ndNeoToOpen[0].DeviceType)
	{
	case NEODEVICE_BLUE:
		sTempString = "neoVI Blue SN ";
		printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_DW_VCAN:
		sTempString = "ValueCAN 2 SN ";
		printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_FIRE:
		sTempString = "neoVI FIRE SN ";
		printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_VCAN3:
		sTempString = "ValueCAN 3 SN ";
		printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_YELLOW:
		sTempString = "neoVI Yellow SN ";
		printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_FIRE2:
		sTempString = "neoVI FIRE2 SN ";
		icsneoSerialNumberToString(ndNeoToOpen[0].SerialNumber, cSNString, 6);
		printf("Device %s %s is connected\r\n", sTempString.c_str(), cSNString);
		break;
	case NEODEVICE_RADGALAXY:
		sTempString = "RADGalaxy SN ";
		icsneoSerialNumberToString(ndNeoToOpen[0].SerialNumber, cSNString, 6);
		printf("Device %s %s is connected\r\n", sTempString.c_str(), cSNString);
		break;
	case NEODEVICE_VCANRF:
		sTempString = "ValueCAN.rf SN ";
		icsneoSerialNumberToString(ndNeoToOpen[0].SerialNumber, cSNString, 6);
		printf("Device %s %s is connected\r\n", sTempString.c_str(), cSNString);
		break;
	case NEODEVICE_PLASMA_1_11:
	case NEODEVICE_PLASMA_1_12:
	case NEODEVICE_PLASMA_1_13:
		sTempString = "neoVI Plasma SN ";
		printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_ION_2:
	case NEODEVICE_ION_3:
		sTempString = "neoVI ION SN ";
		printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_VCAN412:
		sTempString = "VCAN4-12 SN ";
		icsneoSerialNumberToString(ndNeoToOpen[0].SerialNumber, cSNString, 6);
		printf("Device %s %s is connected\r\n", sTempString.c_str(), cSNString);
		break;
	default:
		sTempString = "Unknown neoVI SN ";
		printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
	}

	m_bPortOpen = true;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void ListDevices(void)
{
	int iResult;
	NeoDevice ndNeoToOpen[10];
	int iNumberOfDevices;
	int iCounter;
	string sTempString;
	char cSNString[8] = { NULL };


	iNumberOfDevices = 10;
	//Search for attached devices
	iResult = icsneoFindNeoDevices(NEODEVICE_ALL, ndNeoToOpen, &iNumberOfDevices);
	if (iResult == false)
	{
		printf("Problem Finding Device\r\n");
		return;
	}

	if (iNumberOfDevices < 1)
	{
		printf("No Devices Found\r\n");
		return;
	}

	//Print list of devices
	for (iCounter = 0; iCounter < iNumberOfDevices; iCounter++)
	{
		switch (ndNeoToOpen[iCounter].DeviceType)
		{
		case NEODEVICE_BLUE:
			sTempString = "neoVI Blue SN ";
			printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_DW_VCAN:
			sTempString = "ValueCAN 2 SN ";
			printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_FIRE:
			sTempString = "neoVI FIRE SN ";
			printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_VCAN3:
			sTempString = "ValueCAN 3 SN ";
			printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_YELLOW:
			sTempString = "neoVI Yellow SN ";
			printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_FIRE2:
			sTempString = "neoVI FIRE2 SN ";
			icsneoSerialNumberToString(ndNeoToOpen[iCounter].SerialNumber, cSNString, 6);
			printf("%s %s\r\n", sTempString.c_str(), cSNString);
			break;
		case NEODEVICE_RADGALAXY:
			sTempString = "RADGalaxy SN ";
			icsneoSerialNumberToString(ndNeoToOpen[iCounter].SerialNumber, cSNString, 6);
			printf("%s %s\r\n", sTempString.c_str(), cSNString);
			break;
		case NEODEVICE_VCANRF:
			sTempString = "ValueCAN.rf SN ";
			icsneoSerialNumberToString(ndNeoToOpen[iCounter].SerialNumber, cSNString, 6);
			printf("%s %s\r\n", sTempString.c_str(), cSNString);
			break;
		case NEODEVICE_PLASMA_1_11:
		case NEODEVICE_PLASMA_1_12:
		case NEODEVICE_PLASMA_1_13:
			sTempString = "neoVI Plasma SN ";
			printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_ION_2:
		case NEODEVICE_ION_3:
			sTempString = "neoVI ION SN ";
			printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_VCAN412:
			sTempString = "VCAN4-12 SN ";
			icsneoSerialNumberToString(ndNeoToOpen[iCounter].SerialNumber, cSNString, 6);
			printf("%s %s\r\n", sTempString.c_str(), cSNString);
			break;
		default:
			sTempString = "Unknown neoVI SN ";
			printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
		}

	}
}
//----------------------------------------------------------------------------

