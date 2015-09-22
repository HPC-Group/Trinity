#pragma once

#ifdef GATE_STATIC
#define DLLEXPORT
#else
#if defined(_MSC_VER)
// MSVC
#ifdef GATEAPIDLL_EXPORTS
		#define DLLEXPORT __declspec(dllexport) 
#else
		#define DLLEXPORT __declspec(dllimport) 
#endif
#else
//  Unix
#ifdef GATEAPIDLL_EXPORTS
		#define DLLEXPORT __attribute__((visibility("default")))
#else
#define DLLEXPORT
#endif
#endif
#endif


extern "C"
{
enum DLLEXPORT GATE_ERROR_STATE {
    GATE_OK,
    GATE_ERROR,
    GATE_NO_EVENT
};

DLLEXPORT GATE_ERROR_STATE createEvent(unsigned int& handle);
DLLEXPORT GATE_ERROR_STATE destroyEvent(unsigned int handle);

DLLEXPORT GATE_ERROR_STATE eventSetType(unsigned int handle, const char* value);
DLLEXPORT GATE_ERROR_STATE eventSetX(unsigned int handle, float value);
DLLEXPORT GATE_ERROR_STATE eventSetY(unsigned int handle, float value);
DLLEXPORT GATE_ERROR_STATE eventSetZ(unsigned int handle, float value);
DLLEXPORT GATE_ERROR_STATE eventSetW(unsigned int handle, float value);
DLLEXPORT GATE_ERROR_STATE eventSetText(unsigned int handle, const char* value);
DLLEXPORT GATE_ERROR_STATE eventSetToggle(unsigned int handle, bool value);


DLLEXPORT GATE_ERROR_STATE eventHasType(unsigned int handle, bool& hasValue);
DLLEXPORT GATE_ERROR_STATE eventHasX(unsigned int handle, bool& hasValue);
DLLEXPORT GATE_ERROR_STATE eventHasY(unsigned int handle, bool& hasValue);
DLLEXPORT GATE_ERROR_STATE eventHasZ(unsigned int handle, bool& hasValue);
DLLEXPORT GATE_ERROR_STATE eventHasW(unsigned int handle, bool& hasValue);
DLLEXPORT GATE_ERROR_STATE eventHasText(unsigned int handle, bool& hasValue);
DLLEXPORT GATE_ERROR_STATE eventHasToggle(unsigned int handle, bool& hasValue);


DLLEXPORT GATE_ERROR_STATE eventGetType(unsigned int handle, const char*& value);
DLLEXPORT GATE_ERROR_STATE eventGetX(unsigned int handle, float& value);
DLLEXPORT GATE_ERROR_STATE eventGetY(unsigned int handle, float& value);
DLLEXPORT GATE_ERROR_STATE eventGetZ(unsigned int handle, float& value);
DLLEXPORT GATE_ERROR_STATE eventGetW(unsigned int handle, float& value);
DLLEXPORT GATE_ERROR_STATE eventGetText(unsigned int handle, const char*& value);
DLLEXPORT GATE_ERROR_STATE eventGetToggle(unsigned int handle, bool& value);


DLLEXPORT GATE_ERROR_STATE startService(const char* gateInfoFile);


DLLEXPORT GATE_ERROR_STATE stopService();

DLLEXPORT GATE_ERROR_STATE reset();

DLLEXPORT GATE_ERROR_STATE appSendEvent(unsigned int handle,
                                        const char* requirementName,
                                        const char* roleName);

DLLEXPORT GATE_ERROR_STATE appReceiveEvent(unsigned int& handle,
                                           const char* requirementName,
                                           const char* roleName);

DLLEXPORT GATE_ERROR_STATE deviceSendEvent(unsigned int handle,
                                           const char* deviceName,
                                           const char* capabilityName);

DLLEXPORT GATE_ERROR_STATE deviceReceiveEvent(unsigned int& handle,
                                              const char* deviceName,
                                              const char* capabilityName);


// \todo dmc this function is just for debug purposes; remove it when not needed anymore!
DLLEXPORT GATE_ERROR_STATE subscribeEvents(const char* ip, int port, unsigned int& handle);

DLLEXPORT const char* lastErrorText();
}
