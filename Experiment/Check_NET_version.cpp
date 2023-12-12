#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<iostream>

using namespace std;

namespace CheckDotNetVersion {


	void check_dotnet_version()
	{
		FILE* pPipe;

		char buf[128];

		if ((pPipe = _popen("dotnet --version", "rt")) == NULL)
		{
			exit(1);
		}

		while (fgets(buf, 128, pPipe))
		{
			if (buf[0] == '7')
			{
				std::cout << "\nfound .NET 7\n";
			}
		}
		_pclose(pPipe);
	}
}