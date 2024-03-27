#include <windows.h>
#include <processthreadsapi.h>
#include <psapi.h>
#include <iostream>

namespace GetProcessName {
	void get_current_process_name()
	{
		HANDLE h = GetCurrentProcess();

		char buf[MAX_PATH];

		memset(buf, 0, sizeof(buf));

		GetModuleBaseNameA(h, NULL, buf, sizeof(buf));

		std::cout << buf << "\n";


		//const char* adaptivaclient_install_path = std::getenv("ADAPTIVACLIENT");

		//std::cout << adaptivaclient_install_path << "\n";

		char* env_buf = NULL;
		size_t s = 0;

		_dupenv_s(&env_buf, &s, "ADAPTIVACLIENT");

		std::cout << env_buf << "\n";

		free(env_buf);

	}
}