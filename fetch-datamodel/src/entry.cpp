#include <iostream>
#include <print>
#include <Windows.h>
#include <Psapi.h>
class offsets {
public:
	static const uint64_t fake_dm_ptr = 0x6E854F8;
	static const uint64_t fake_dm_2_dm = 0x1C0;
};
class helpers {
public:
	// https://github.com/PierreCiholas/GetBaseAddress/blob/master/main.cpp
	static uint64_t get_base_addr(const HANDLE process) {
		HMODULE module_list[1024];
		DWORD bytes(NULL);
		TCHAR module_name[MAX_PATH];
		if (!EnumProcessModules(process, module_list, sizeof(module_list), &bytes))
			return NULL;
		if (!GetModuleFileNameEx(process, module_list[0], module_name, sizeof(module_name) / sizeof(TCHAR)))
			return NULL;
		return (uint64_t)module_list[0];
	}
	static uint64_t read(HANDLE proc, uint64_t addr) {
		uint64_t buffer{};
		if (ReadProcessMemory(proc, (PVOID)addr, &buffer, sizeof(buffer), NULL)) {
			return buffer;
		}
		else {
			return 0;
		}
	}
};
int main() {
	DWORD pid;
	HWND roblox = FindWindowA(NULL, "Roblox");
	GetWindowThreadProcessId(roblox, &pid);
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
	if (process) {
		uint64_t base = helpers::get_base_addr(process);
		if (!base) return 0;
		uint64_t fake_dm = helpers::read(process, base + offsets::fake_dm_ptr);
		uint64_t real_dm = helpers::read(process, fake_dm + offsets::fake_dm_2_dm);
		std::println("0x{:X}", real_dm);
		std::cin.get();
	}
}
