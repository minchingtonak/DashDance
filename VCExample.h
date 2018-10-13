#ifndef ADD_H  
#define ADD_H  

bool Load_icsneo40DLL(void); 
void ListDevices(void); 
void ConnectToFirstDevice(void);
void DisconnectFromDevice(void);
void GetMessagesFromDevice(void);
void SendMessageFromDeviceCAN(void);
void SendMessageFromDeviceCANFD(int ByteCount);
void GetErrorsFromDevice(void);
void SetHSCANBaudRatetoDevice(int iRateToUse);
void SendMasterFrame(void);
void SendHeaderOnlyFrame(void);
void SendSlaveFrame(void);
void SendISO9141KW2K(void);
void SendEthernet(void);
void ConfigureISO15765Tx(void);
void ConfigureISO15765Rx(void);
void DisableISO15765(void);
#endif 
