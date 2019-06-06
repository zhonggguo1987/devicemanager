// testdevicemanager.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <functional>
#include "devicemanager_interface.h"
using namespace std;

#if 0
class IKlass {
public:
	virtual void destroy() = 0;
	virtual int do_stuff(int param) = 0;
	virtual void do_something_else(double f) = 0;
};
typedef IKlass* (__cdecl *iklass_factory)();
#endif

typedef ICdevicemanager* (__cdecl* icdevicemanager_factory)();

template<typename T> bool compare_no(const T* s1, const T* s2)
{
	return strcmp(s1->no, s2->no) == 0;
}

template<typename T> bool less_no(const T* s1, const T* s2)
{
	return strcmp(s1->no, s2->no) < 0;
}

template<typename T> bool compare_id(const T* s1, const T* s2)
{
	return s1->szDeviceClass == s2->szDeviceClass;
}

template<typename T> bool less_id(const T* s1, const T* s2)
{
	return s1->id < s2->id;
}

int main(int argc,char *argv[])
{
	HMODULE hModule = LoadLibrary(_T("devicemanager.dll"));
	if (hModule == NULL || hModule == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "devicemanager.dll not found");
		return -1;
	}
#if 1
	icdevicemanager_factory factory_func = reinterpret_cast<icdevicemanager_factory>(
		::GetProcAddress(hModule, "create_devicemanager"));
	if (!factory_func) {
		::FreeLibrary(hModule);
		return 1;
	}
	// Ask the factory for a new object implementing the ICdevicemanager
	// interface
	ICdevicemanager* instance = factory_func();

	vector<DeviceInfo> list=instance->EnumDeviceInfo(GUID_DEVCLASS_DISPLAY);


	vector<DeviceInfo>::iterator it_cam;
	it_cam = std::find_if(list.begin(), list.end(), [](DeviceInfo const& obj) {
		return obj.szDeviceClass == TEXT("Display");			
	});

	if (it_cam != list.end()) {
		//wcout.imbue(locale("chs"));
		//fprintf(stderr, "displayname:%s\n", it_cam->szDeviceDesc.c_str());
		wcout << it_cam->szDeviceDesc.c_str() << endl;
	}
	
	

	// Destroy it explicitly
	instance->destroy();
	::FreeLibrary(hModule);
#else
	// Get the function from the DLL
	iklass_factory factory_func = reinterpret_cast<iklass_factory>(
		::GetProcAddress(hModule, "create_klass"));
	if (!factory_func) {
		::FreeLibrary(hModule);
		return 1;
	}

	// Ask the factory for a new object implementing the IKlass
	// interface
	IKlass* instance = factory_func();

	// Play with the object
	int t = instance->do_stuff(5);
	instance->do_something_else(100.3);
	int t2 = instance->do_stuff(0);

	// Destroy it explicitly
	instance->destroy();
	::FreeLibrary(hModule);


#endif
    return 0;
}

