// devicemanager.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "devicemanager.h"

#pragma comment(lib,"setupapi.lib")

std::string utf8_encode(const std::wstring & wstr)
{

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

#if 1
// This is the constructor of a class that has been exported.
// see devicemanager.h for the class definition
Cdevicemanager::Cdevicemanager()
{
    
}

Cdevicemanager::~Cdevicemanager()
{

}

vector<DeviceInfo> Cdevicemanager::EnumDeviceInfo(GUID ClassGuid, bool isAllInfo)
{
	//结果集
	std::vector<DeviceInfo> result_set;

	HDEVINFO device_info_set;
	//https://docs.microsoft.com/zh-cn/windows/desktop/api/setupapi/nf-setupapi-setupdigetclassdevsw
	if (isAllInfo) {
		//获取本地计算机所有设备信息集 
		device_info_set = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
	}
	else {
		//仅串口
		device_info_set = SetupDiGetClassDevs(&ClassGuid, NULL, NULL,DIGCF_PRESENT);
	}

	if (device_info_set == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "GetLastError = %lu\r\n", GetLastError());
		return result_set;
	}

	SP_DEVINFO_DATA device_info_data;
	SecureZeroMemory(&device_info_data, sizeof(SP_DEVINFO_DATA));
	device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
	unsigned long device_info_set_index = 0;


	//枚举设备
	while (SetupDiEnumDeviceInfo(device_info_set, device_info_set_index, &device_info_data))
	{
		++device_info_set_index;

		TCHAR szFriendlyName[MAX_PATH] = { 0 };
		TCHAR szDeviceClass[MAX_PATH] = { 0 };
		TCHAR szDeviceDesc[MAX_PATH] = { 0 };
		TCHAR szDriverName[MAX_PATH] = { 0 };
		DWORD DataT;
		LPTSTR szDeviceID = NULL;
		DWORD buffersize = 0;

		//SPDRP_HARDWAREID

		//SPDRP_HARDWAREID
		DeviceInfo device_info;
		//获取友好名称
		if (!SetupDiGetDeviceRegistryProperty(device_info_set, &device_info_data, SPDRP_FRIENDLYNAME, NULL, (PBYTE)szFriendlyName, MAX_PATH - 1, NULL)) {
			fprintf(stderr, "%2d %s\r\n", device_info_set_index, utf8_encode(L"Get SPDRP_FRIENDLYNAME Failed").c_str());
		}

		//获取设备类
		if (!SetupDiGetDeviceRegistryProperty(device_info_set, &device_info_data, SPDRP_CLASS, NULL, (PBYTE)szDeviceClass, MAX_PATH - 1, NULL)) {
			fprintf(stderr, "%2d %s\r\n", device_info_set_index,utf8_encode(L"Get SPDRP_CLASS Failed").c_str());
		}

		//获取设备描述
		if (!SetupDiGetDeviceRegistryProperty(device_info_set, &device_info_data, SPDRP_DEVICEDESC, NULL, (PBYTE)szDeviceDesc, MAX_PATH - 1, NULL)) {
			fprintf(stderr, "%2d %s\r\n", device_info_set_index, utf8_encode(L"Get SPDRP_DEVICEDESC Failed").c_str());
		}
		
		while (!SetupDiGetDeviceRegistryProperty(device_info_set,
			&device_info_data,
			SPDRP_HARDWAREID,
			&DataT,
			(PBYTE)szDeviceID,
			buffersize, 
			&buffersize))
		{
			if (GetLastError() == ERROR_INVALID_DATA) {
				break;
			}
			else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
				if (szDeviceID) {
					LocalFree(szDeviceID);
				}
				szDeviceID = (wchar_t*)LocalAlloc(LPTR, buffersize);
			}
			else {

			}
		}

		////获取驱动名称
		//if(!SetupDiGetDeviceRegistryProperty(device_info_set, &device_info_data, SPDRP_HARDWAREID, NULL, (PBYTE)szDeviceID, MAX_PATH - 1, NULL)) {
		//	fprintf(stderr, "%2d %s\r\n", device_info_set_index, utf8_encode(L"Get SPDRP_HARDWAREID Failed").c_str());
		//}

		//获取驱动名称
		if (!SetupDiGetDeviceRegistryProperty(device_info_set, &device_info_data, SPDRP_DRIVER, NULL, (PBYTE)szDriverName, MAX_PATH - 1, NULL)) {
			fprintf(stderr, "%2d %s\r\n", device_info_set_index, utf8_encode(L"Get SPDRP_DRIVER Failed").c_str());
		}

		device_info.szFriendlyName = szFriendlyName;
		device_info.szDeviceClass = szDeviceClass;
		device_info.szDeviceDesc = szDeviceDesc;
		device_info.szDeviceID = szDeviceID;
		device_info.szDriverName = szDriverName;
		device_info.dwDevIns = device_info_data.DevInst;//实例
		device_info.Guid = device_info_data.ClassGuid;//GUID
		
		if (szDeviceID) {
			LocalFree(szDeviceID);
		}
		result_set.push_back(device_info);
	}

	if (device_info_set) {
		SetupDiDestroyDeviceInfoList(device_info_set);
	}

	return result_set;
}

bool Cdevicemanager::setDeviceStatus(DeviceInfo & theDevice, bool bStatusFlag)
{
	//获取设备信息集
	HDEVINFO device_info_set = SetupDiGetClassDevs(&theDevice.Guid, 0, 0, DIGCF_PRESENT /*| DIGCF_ALLCLASSES */);
	if (device_info_set == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "SetupDiGetClassDevs ERR!");
		return false;
	}

	SP_DEVINFO_DATA device_info_data;
	SecureZeroMemory(&device_info_data, sizeof(SP_DEVINFO_DATA));
	device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
	unsigned long device_info_set_index = 0;
	bool bFlag = false;

	//枚举设备判断指定的设备是否存在
	while (SetupDiEnumDeviceInfo(device_info_set, device_info_set_index, &device_info_data)) {
		++device_info_set_index;
		if (theDevice.dwDevIns == device_info_data.DevInst) {
			bFlag = true;
			break;
		}
	}

	if (bFlag) {
		//https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/ns-setupapi-_sp_propchange_params
		SP_PROPCHANGE_PARAMS change;
		change.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
		change.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
		change.Scope = DICS_FLAG_GLOBAL;
		change.StateChange = bStatusFlag ? DICS_ENABLE : DICS_DISABLE;
		change.HwProfile = 0;

		if (SetupDiSetClassInstallParams(device_info_set, &device_info_data, (SP_CLASSINSTALL_HEADER*)&change, sizeof(change))) {
			if (!SetupDiChangeState(device_info_set, &device_info_data)) {
				fprintf(stderr, "SetupDiChangeState ERR!");
				bFlag = false;
			}
		}
		else {
			fprintf(stderr, "SetupDiSetClassInstallParams ERR!");
			bFlag = false;
		}
	}
	else {
		fprintf(stderr, "Device not found!");
	}

	//释放资源
	SetupDiDestroyDeviceInfoList(device_info_set);

	return bFlag;
}

wstring Cdevicemanager::vid(std::wstring deviceID)
{
	auto pos = deviceID.rfind(L"vid_");
	if (pos == std::wstring::npos) {
		return std::wstring();
	}
	return deviceID.substr(pos + 4, 4);
}

wstring Cdevicemanager::pid(std::wstring deviceID)
{
	auto pos = deviceID.rfind(L"pid_");
	if (pos == std::wstring::npos) {
		return std::wstring();
	}
	return deviceID.substr(pos + 4, 4);
}

void Cdevicemanager::destroy()
{
	delete this;
}

BOOL Cdevicemanager::DoRegisterDeviceInterfaceToHwnd(IN GUID InterfaceClassGuid, IN HWND hWnd, OUT HDEVNOTIFY * hDeviceNotify)
{
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;

	SecureZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid = InterfaceClassGuid;

	*hDeviceNotify = RegisterDeviceNotification(
		hWnd,                       // events recipient
		&NotificationFilter,        // type of device
		DEVICE_NOTIFY_WINDOW_HANDLE // type of recipient handle
	);

	if (NULL == *hDeviceNotify)
	{
		fprintf(stderr, "RegisterDeviceNotification");
		return FALSE;
	}

	return TRUE;
}
#endif