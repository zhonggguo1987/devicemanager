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

	//设备友好名称
	std::wstring szFriendlyName;
	//设备类
	std::wstring szDeviceClass;
	//设备描述
	std::wstring szDeviceDesc;
	//设备硬件ID
	std::wstring szDeviceID;
	//设备驱动
	std::wstring szDriverName;
	//设备实例
	DWORD dwDevIns;
	//设备类标志
	GUID Guid;
}DeviceInfo, *pDeviceInfo;

static GUID WceusbshGUID = { 0x25dbce51, 0x6c8f, 0x4a72,0x8a,0x6d,0xb5,0x4c,0x2b,0x4f,0xc8,0x35 };

std::string utf8_encode(const std::wstring& wstr);


#if 1
class ICdevicemanager {
public:
	virtual void destroy() = 0;
	//枚举所有的设备，默认为false
	virtual vector<DeviceInfo> EnumDeviceInfo(GUID ClassGuid,bool isAllInfo = false);
	//设置设备状态(禁用/停用),true 禁用,false 启用
	virtual bool SetDeviceStatus(DeviceInfo &theDevice, bool bStatusFlag);	
	virtual wstring vid(wstring deviceID);
	virtual wstring pid(wstring deviceID);
	//注册设备热拔插通知 win8 以上可使用 CM_Register_Notification 函数 
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
