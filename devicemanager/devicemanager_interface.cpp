#include "stdafx.h"
#include "devicemanager_interface.h"

vector<DeviceInfo> ICdevicemanager::EnumDeviceInfo(GUID ClassGuid,bool isAllInfo)
{
	return vector<DeviceInfo>();
}

bool ICdevicemanager::SetDeviceStatus(DeviceInfo& theDevice, bool bStatusFlag)
{
	return false;
}

wstring ICdevicemanager::vid(wstring deviceID)
{
	return wstring();
}

wstring ICdevicemanager::pid(wstring deviceID)
{
	return wstring();
}

BOOL ICdevicemanager::DoRegisterDeviceInterfaceToHwnd(IN GUID InterfaceClassGuid, IN HWND hWnd, OUT HDEVNOTIFY* hDeviceNotify)
{
	return 0;
}
