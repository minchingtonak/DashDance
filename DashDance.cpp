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
#include <mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "Winmm.lib")

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

	ifstream audio;
	audio.open("unitedwedance.txt");

	if (!audio.is_open()) return 1;

	string left, right;
	while (audio >> left >> right) {
		// Multiply by 100 to avoid integer truncation to 0
		channel.push_back(static_cast<int>(stof(left) * 100.0));
	}

	cout << "Song size: " << channel.size() << endl;

	//Load DLL functions
	if (!LoadDLLAPI(hDLL))
	{
		//problem, close the application
		std::printf("Problem loading Library\r\nMake sure icsneo40.dll is installed and accessable\r\nPress any key to Exit");
		cInputCommand = _getwch();
		return(1);
	}

	system("Color A"); //Change window color

	//Get the DLL Version number
	iVerNumber = icsneoGetDLLVersion();
	std::printf("ICS icsneo40.dll version %d\r\n\r\n", iVerNumber);

	while (bKeepRunning)
	{
		//Main Menu
		std::printf("\r\n\r\n");
		std::printf("Press the letter next to the function group you want to use\r\n");
		std::printf("A - List Attached devices\r\n");
		std::printf("B - Connect to the first\r\n");
		std::printf("C - Get Messages\r\n");
		std::printf("D - Play Song\r\n");
		std::printf("H - Disconnect\r\n");
		std::printf("X - Exit\r\n");
		std::printf("\r\n\r\n");
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
		std::printf("Press Key to Continue");
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
		std::printf("neoVI not opened\r\n");
		return;
	}

	//Call Get Messages
	lResult = icsneoGetMessages(m_hObject, stMessages, &lNumberOfMessages, &iNumberOfErrors); //Call get message function
	if (lResult == 0)//See if Call was successful
	{
		std::printf("Problem Getting Messages\r\n");
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
		std::printf("%f ", dTime);

		//check if the message is Tx or RX
		if ((stMessages[lCount].StatusBitField & SPY_STATUS_TX_MSG) != 0)
		{
			std::printf("Tx ");
		}
		else
		{
			std::printf("Rx ");
		}

		//Get the network of the message
		switch (stMessages[lCount].NetworkID)
		{
		case NETID_DEVICE:
			std::printf("DEVICE");
			break;
		case NETID_HSCAN:
			std::printf("HSCAN");
			break;
		case NETID_MSCAN:
			std::printf("MSCAN");
			break;
		case NETID_SWCAN:
			std::printf("SWCAN");
			break;
		case NETID_LSFTCAN:
			std::printf("LSFTCAN");
			break;
		case NETID_FORDSCP:
			std::printf("FORDSCP");
			break;
		case NETID_J1708:
			std::printf("J1708");
			break;
		case NETID_JVPW:
			std::printf("JVPW");
			break;
		case NETID_ISO:
			std::printf("ISO");
			break;
		case NETID_ISO2:
			std::printf("ISO2");
			break;
		case NETID_ISO14230:
			std::printf("ISO14230");
			break;
		case NETID_LIN:
			std::printf("LIN");
			break;
		case NETID_ISO3:
			std::printf("ISO3");
			break;
		case NETID_HSCAN2:
			std::printf("HSCAN2");
			break;
		case NETID_HSCAN3:
			std::printf("HSCAN3");
			break;
		case NETID_ISO4:
			std::printf("ISO4");
			break;
		case NETID_LIN2:
			std::printf("LIN2");
			break;
		case NETID_LIN3:
			std::printf("LIN3");
			break;
		case NETID_LIN4:
			std::printf("LIN4");
			break;
		case NETID_LIN5:
			std::printf("LIN5");
			break;
		case NETID_MOST:
			std::printf("MOST");
			break;
		case NETID_CGI:
			std::printf("CGI");
			break;
		case NETID_HSCAN4:
			std::printf("HSCAN4");
			break;
		case NETID_HSCAN5:
			std::printf("HSCAN5");
			break;
		case NETID_UART:
			std::printf("UART");
			break;
		case NETID_UART2:
			std::printf("UART2");
			break;
		case NETID_UART3:
			std::printf("UART3");
			break;
		case NETID_UART4:
			std::printf("UART4");
			break;
		case NETID_SWCAN2:
			std::printf("SWCAN2");
			break;
		case NETID_ETHERNET_DAQ:
			std::printf("ETHERNE");
			break;
		case NETID_TEXTAPI_TO_HOST:
			std::printf("TEXTAP");
			break;
		case NETID_FLEXRAY1A:
			std::printf("FLEXRAY1A");
			break;
		case NETID_FLEXRAY1B:
			std::printf("FLEXRAY1B");
			break;
		case NETID_FLEXRAY2A:
			std::printf("FLEXRAY2A");
			break;
		case NETID_FLEXRAY2B:
			std::printf("FLEXRAY2B");
			break;
		case NETID_FLEXRAY:
			std::printf("FLEXRAY");
			break;
		case NETID_MOST25:
			std::printf("MOST25");
			break;
		case NETID_MOST50:
			std::printf("MOST50");
			break;
		case NETID_MOST150:
			std::printf("MOST150");
			break;
		case NETID_ETHERNET:
			std::printf("ETHERNET");
			break;
		case NETID_GMFSA:
			std::printf("GMFSA");
			break;
		case NETID_TCP:
			std::printf("TCP");
			break;
		case NETID_HSCAN6:
			std::printf("HSCAN6");
			break;
		case NETID_HSCAN7:
			std::printf("HSCAN7");
			break;
		case NETID_LIN6:
			std::printf("LIN6");
			break;
		case NETID_LSFTCAN2:
			std::printf("LSFTCAN2");
			break;
		case NETID_OP_ETHERNET1:
			std::printf("OP_ETHERNET1");
			break;
		case NETID_OP_ETHERNET2:
			std::printf("OP_ETHERNET2");
			break;
		case NETID_OP_ETHERNET3:
			std::printf("OP_ETHERNET3");
			break;
		case NETID_OP_ETHERNET4:
			std::printf("OP_ETHERNET4");
			break;
		case NETID_OP_ETHERNET5:
			std::printf("OP_ETHERNET5");
			break;
		case NETID_OP_ETHERNET6:
			std::printf("OP_ETHERNET6");
			break;
		case NETID_OP_ETHERNET7:
			std::printf("OP_ETHERNET7");
			break;
		case NETID_OP_ETHERNET8:
			std::printf("OP_ETHERNET8");
			break;
		case NETID_OP_ETHERNET9:
			std::printf("OP_ETHERNET9");
			break;
		case NETID_OP_ETHERNET10:
			std::printf("OP_ETHERNET10");
			break;
		case NETID_OP_ETHERNET11:
			std::printf("OP_ETHERNET11");
			break;
		case NETID_OP_ETHERNET12:
			std::printf("OP_ETHERNET12");
			break;
		}

		//Decode the databytes
		switch (stMessages[lCount].Protocol)
		{
		case SPY_PROTOCOL_CAN:
			//(Classic) CAN messages, get the ArbID and data bytes
			std::printf(" ArbID-%X ", stMessages[lCount].ArbIDOrHeader);
			if (lByteCount >= 1) std::printf("D %X ", stMessages[lCount].Data[0]);
			if (lByteCount >= 2) std::printf(" %X ", stMessages[lCount].Data[1]);
			if (lByteCount >= 3) std::printf(" %X ", stMessages[lCount].Data[2]);
			if (lByteCount >= 4) std::printf(" %X ", stMessages[lCount].Data[3]);
			if (lByteCount >= 5) std::printf(" %X ", stMessages[lCount].Data[4]);
			if (lByteCount >= 6) std::printf(" %X ", stMessages[lCount].Data[5]);
			if (lByteCount >= 7) std::printf(" %X ", stMessages[lCount].Data[6]);
			if (lByteCount >= 8) std::printf(" %X ", stMessages[lCount].Data[7]);
			break;
		case SPY_PROTOCOL_CANFD:
			//CAN FD 
			//Get the ID
			std::printf(" ArbID-%X ", stMessages[lCount].ArbIDOrHeader);
			if (stMessages[lCount].ExtraDataPtrEnabled == 0)
			{
				//8 bytes of data, messages with 0 to 8 bytes of data are different than Longer frames.
				//Here is how to acquire 0 to 8
				if (lByteCount >= 1) std::printf("D %X ", stMessages[lCount].Data[0]);
				if (lByteCount >= 2) std::printf(" %X ", stMessages[lCount].Data[1]);
				if (lByteCount >= 3) std::printf(" %X ", stMessages[lCount].Data[2]);
				if (lByteCount >= 4) std::printf(" %X ", stMessages[lCount].Data[3]);
				if (lByteCount >= 5) std::printf(" %X ", stMessages[lCount].Data[4]);
				if (lByteCount >= 6) std::printf(" %X ", stMessages[lCount].Data[5]);
				if (lByteCount >= 7) std::printf(" %X ", stMessages[lCount].Data[6]);
				if (lByteCount >= 8) std::printf(" %X ", stMessages[lCount].Data[7]);
			}
			else
			{
				//More bytes are done using Extended Data pointer.
				ptr = (const unsigned char*)stMessages[lCount].ExtraDataPtr;
				for (int j = 0; j < stMessages[lCount].NumberBytesData; j++)
				{
					std::printf("%0x\t", *(ptr++));
				}
				std::printf("\n");
			}

			break;
		default:
			//Other non CAN message types.  
			memcpy((void*)&icsJ1850Msg, (void*)&stMessages[lCount], sizeof(icsSpyMessageJ1850));

			std::printf(" Header-%X %X %X ", icsJ1850Msg.Header[0], icsJ1850Msg.Header[1], icsJ1850Msg.Header[2]);
			if (lByteCount >= 1) std::printf("D %X ", icsJ1850Msg.Data[0]);
			if (lByteCount >= 2) std::printf(" %X", icsJ1850Msg.Data[1]);
			if (lByteCount >= 3) std::printf(" %X ", icsJ1850Msg.Data[2]);
			if (lByteCount >= 4) std::printf(" %X ", icsJ1850Msg.Data[3]);
			if (lByteCount >= 5) std::printf(" %X ", icsJ1850Msg.Data[4]);
			if (lByteCount >= 6) std::printf(" %X ", icsJ1850Msg.Data[5]);
			if (lByteCount >= 7) std::printf(" %X ", icsJ1850Msg.Data[6]);
			if (lByteCount >= 8) std::printf(" %X ", icsJ1850Msg.Data[7]);
			break;
		}
		std::printf("\r\n");
	}
	//Display the number of messages and errors
	std::printf("Number Read : %d\r\n", lNumberOfMessages);
	std::printf("Number Errors : %d\r\n", iNumberOfErrors);
	std::printf("\r\n");
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
		std::printf("neoVI not opened\r\n");
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

	std::printf("Starting song...\r\n");

	thread th(playSong);

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

	std::printf("SONG SENDING FINISHED!\r\n");

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
		std::printf("Problem Closing Port\r\n");
	}
	else
	{
		std::printf("Port Closed\r\n");
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
		std::printf("Device already opened\r\n");
		return;
	}

	//search for the first device
	iNumberOfDevices = 1;
	iResult = icsneoFindNeoDevices(NEODEVICE_ALL, ndNeoToOpen, &iNumberOfDevices);
	if (iResult == false)
	{
		std::printf("Problem Finding Device\r\n");
		return;
	}

	//Check for the number of devices
	if (iNumberOfDevices < 1)
	{
		std::printf("No Devices Found\r\n");
		return;
	}

	m_hObject = NULL;
	m_hObject = 0;
	//Connect to the first device found
	iResult = icsneoOpenNeoDevice(&ndNeoToOpen[0], &m_hObject, NULL, 1, 0);
	if (iResult == false)
	{
		std::printf("Problem Opening Port\r\n");
		return;
	}
	std::printf("Port Opened OK!\r\n");

	//Display the type of device
	switch (ndNeoToOpen[0].DeviceType)
	{
	case NEODEVICE_BLUE:
		sTempString = "neoVI Blue SN ";
		std::printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_DW_VCAN:
		sTempString = "ValueCAN 2 SN ";
		std::printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_FIRE:
		sTempString = "neoVI FIRE SN ";
		std::printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_VCAN3:
		sTempString = "ValueCAN 3 SN ";
		std::printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_YELLOW:
		sTempString = "neoVI Yellow SN ";
		std::printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_FIRE2:
		sTempString = "neoVI FIRE2 SN ";
		icsneoSerialNumberToString(ndNeoToOpen[0].SerialNumber, cSNString, 6);
		std::printf("Device %s %s is connected\r\n", sTempString.c_str(), cSNString);
		break;
	case NEODEVICE_RADGALAXY:
		sTempString = "RADGalaxy SN ";
		icsneoSerialNumberToString(ndNeoToOpen[0].SerialNumber, cSNString, 6);
		std::printf("Device %s %s is connected\r\n", sTempString.c_str(), cSNString);
		break;
	case NEODEVICE_VCANRF:
		sTempString = "ValueCAN.rf SN ";
		icsneoSerialNumberToString(ndNeoToOpen[0].SerialNumber, cSNString, 6);
		std::printf("Device %s %s is connected\r\n", sTempString.c_str(), cSNString);
		break;
	case NEODEVICE_PLASMA_1_11:
	case NEODEVICE_PLASMA_1_12:
	case NEODEVICE_PLASMA_1_13:
		sTempString = "neoVI Plasma SN ";
		std::printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_ION_2:
	case NEODEVICE_ION_3:
		sTempString = "neoVI ION SN ";
		std::printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
		break;
	case NEODEVICE_VCAN412:
		sTempString = "VCAN4-12 SN ";
		icsneoSerialNumberToString(ndNeoToOpen[0].SerialNumber, cSNString, 6);
		std::printf("Device %s %s is connected\r\n", sTempString.c_str(), cSNString);
		break;
	default:
		sTempString = "Unknown neoVI SN ";
		std::printf("Device %s %d is connected\r\n", sTempString.c_str(), ndNeoToOpen[0].SerialNumber);
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
		std::printf("Problem Finding Device\r\n");
		return;
	}

	if (iNumberOfDevices < 1)
	{
		std::printf("No Devices Found\r\n");
		return;
	}

	//Print list of devices
	for (iCounter = 0; iCounter < iNumberOfDevices; iCounter++)
	{
		switch (ndNeoToOpen[iCounter].DeviceType)
		{
		case NEODEVICE_BLUE:
			sTempString = "neoVI Blue SN ";
			std::printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_DW_VCAN:
			sTempString = "ValueCAN 2 SN ";
			std::printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_FIRE:
			sTempString = "neoVI FIRE SN ";
			std::printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_VCAN3:
			sTempString = "ValueCAN 3 SN ";
			std::printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_YELLOW:
			sTempString = "neoVI Yellow SN ";
			std::printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_FIRE2:
			sTempString = "neoVI FIRE2 SN ";
			icsneoSerialNumberToString(ndNeoToOpen[iCounter].SerialNumber, cSNString, 6);
			std::printf("%s %s\r\n", sTempString.c_str(), cSNString);
			break;
		case NEODEVICE_RADGALAXY:
			sTempString = "RADGalaxy SN ";
			icsneoSerialNumberToString(ndNeoToOpen[iCounter].SerialNumber, cSNString, 6);
			std::printf("%s %s\r\n", sTempString.c_str(), cSNString);
			break;
		case NEODEVICE_VCANRF:
			sTempString = "ValueCAN.rf SN ";
			icsneoSerialNumberToString(ndNeoToOpen[iCounter].SerialNumber, cSNString, 6);
			std::printf("%s %s\r\n", sTempString.c_str(), cSNString);
			break;
		case NEODEVICE_PLASMA_1_11:
		case NEODEVICE_PLASMA_1_12:
		case NEODEVICE_PLASMA_1_13:
			sTempString = "neoVI Plasma SN ";
			std::printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_ION_2:
		case NEODEVICE_ION_3:
			sTempString = "neoVI ION SN ";
			std::printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
			break;
		case NEODEVICE_VCAN412:
			sTempString = "VCAN4-12 SN ";
			icsneoSerialNumberToString(ndNeoToOpen[iCounter].SerialNumber, cSNString, 6);
			std::printf("%s %s\r\n", sTempString.c_str(), cSNString);
			break;
		default:
			sTempString = "Unknown neoVI SN ";
			std::printf("%s %d\r\n", sTempString.c_str(), ndNeoToOpen[iCounter].SerialNumber);
		}

	}
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void playSong() {
	PlaySound("unitedwedance.wav", NULL, SND_FILENAME);
}
