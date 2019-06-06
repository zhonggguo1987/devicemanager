// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DEVICEMANAGER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DEVICEMANAGER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DEVICEMANAGER_EXPORTS
#define DEVICEMANAGER_API __declspec(dllexport)
#else
#define DEVICEMANAGER_API __declspec(dllimport)
#endif
#pragma once


#include "devicemanager_interface.h"

// This class is exported from the devicemanager.dll
#if 1
class Cdevicemanager:public ICdevicemanager {
public:
	Cdevicemanager(void);
	// TODO: add your methods here.
	~Cdevicemanager();
	vector<DeviceInfo> EnumDeviceInfo(GUID ClassGuid, bool isAllInfo = false);
	//设置设备状态(禁用/停用),true 禁用,false 启用
	bool setDeviceStatus(DeviceInfo &theDevice, bool bStatusFlag);
	//pid
	wstring vid(std::wstring deviceID);
	//vid
	wstring pid(std::wstring deviceID);
	void destroy();
	BOOL DoRegisterDeviceInterfaceToHwnd(IN GUID InterfaceClassGuid, IN HWND hWnd, OUT HDEVNOTIFY *hDeviceNotify);
};

extern "C" __declspec(dllexport) ICdevicemanager* __cdecl create_devicemanager()
{
	return new Cdevicemanager;
}
#else
class MyKlass : public IKlass {
public:
	MyKlass()
		: m_data(0)
	{
		
	}

	~MyKlass()
	{
		
	}

	void destroy()
	{
		delete this;
	}

	int do_stuff(int param)
	{
		m_data += param;
		return m_data;
	}

	void do_something_else(double f)
	{
		int intpart = static_cast<int>(f);
		m_data += intpart;
	}
private:
	int m_data;
};

extern "C" __declspec(dllexport) IKlass* __cdecl create_klass()
{
	return new MyKlass;
}



#endif