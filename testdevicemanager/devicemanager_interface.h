#pragma once

#include <string>
#include <vector>
#include <setupapi.h>
#include <devguid.h>
#include <stringapiset.h>
#include <Dbt.h>
#include <Usbiodef.h>

using namespace std;

typedef struct tagDeviceInfo {

	//�豸�Ѻ�����
	std::wstring szFriendlyName;
	//�豸��
	std::wstring szDeviceClass;
	//�豸����
	std::wstring szDeviceDesc;
	//�豸Ӳ��ID
	std::wstring szDeviceID;
	//�豸����
	std::wstring szDriverName;
	//�豸ʵ��
	DWORD dwDevIns;
	//�豸���־
	GUID Guid;
}DeviceInfo, *pDeviceInfo;

static GUID WceusbshGUID = { 0x25dbce51, 0x6c8f, 0x4a72,0x8a,0x6d,0xb5,0x4c,0x2b,0x4f,0xc8,0x35 };

std::string utf8_encode(const std::wstring& wstr);


#if 1
class ICdevicemanager {
public:
	virtual void destroy() = 0;
	//ö�����е��豸��Ĭ��Ϊfalse
	virtual vector<DeviceInfo> EnumDeviceInfo(GUID ClassGuid,bool isAllInfo = false);
	//�����豸״̬(����/ͣ��),true ����,false ����
	virtual bool SetDeviceStatus(DeviceInfo &theDevice, bool bStatusFlag);	
	virtual wstring vid(wstring deviceID);
	virtual wstring pid(wstring deviceID);
	//ע���豸�Ȱβ�֪ͨ win8 ���Ͽ�ʹ�� CM_Register_Notification ���� 
	//https://docs.microsoft.com/zh-cn/windows-hardware/drivers/install/registering-for-notification-of-device-interface-arrival-and-device-removal
	virtual BOOL DoRegisterDeviceInterfaceToHwnd(
		IN GUID InterfaceClassGuid,
		IN HWND hWnd,
		OUT HDEVNOTIFY *hDeviceNotify);
};
#else
class IKlass {
public:
	virtual void destroy() = 0;
	virtual int do_stuff(int param) = 0;
	virtual void do_something_else(double f) = 0;
};
#endif
