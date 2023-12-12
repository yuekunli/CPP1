#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

using namespace std;

namespace OpensslInstallFips {
    static std::string& get_command_string()
    {
        char const* openssl_path = "C:\\ws\\openssl_output\\lib\\x32\\release\\bin\\openssl.exe";
        char const* fips_module_path = "C:\\ws\\openssl_output\\lib\\x32\\release\\lib\\ossl-modules\\fips.dll";
        char const* out_path = "C:\\ws\\openssl_output\\SSL_static_32_release\\fipsmodule.cnf";

        //char const* command = "cmd /c C:\\ws\\openssl_output\\lib\\x32\\release\\bin\\openssl.exe fipsinstall -module C:\\ws\\openssl_output\\lib\\x32\\release\\lib\\ossl-modules\\fips.dll -out C:\\ws\\openssl_output\\SSL_static_32_release\\fipsmodule.cnf -self_test_oninstall";
        //char const* command = "dir *.c /on /p";

        //char const* command = "%OPENSSL_OUTPUT%\\lib\\x32\\release\\bin\\openssl.exe fipsinstall -module %OPENSSL_OUTPUT%\\lib\\x32\\release\\lib\\ossl-modules\\fips.dll -out %OPENSSL_OUTPUT%\\SSL_static_32_release\\fipsmodule.cnf -self_test_oninstall";

        //char const* command = "echo %OPENSSL_OUTPUT%";

        char const* openssl_cmd = "fipsinstall";
        char const* module_arg = "-module";
        char const* out_arg = "-out";
        char const* self_test_arg = "-self_test_oninstall";

        static std::string command;

        command.append(openssl_path).append(1, ' ').append(openssl_cmd).append(1, ' ').append(module_arg).append(1, ' ').append(fips_module_path).append(1, ' ').append(out_arg).append(1, ' ').append(out_path).append(1, ' ').append(self_test_arg);
        return command;
    }

    static std::string& get_command_string_with_env_var()
    {
        char* openssl_output_env_var;
        size_t len;
        errno_t err = _dupenv_s(&openssl_output_env_var, &len, "OPENSSL_OUTPUT");

        //= getenv("OPENSSL_OUTPUT");

        char const* openssl_path = "\\lib\\x32\\release\\bin\\openssl.exe";
        char const* fips_module_path = "\\lib\\x32\\release\\lib\\ossl-modules\\fips.dll";
        char const* out_path = "\\SSL_static_32_release\\fipsmodule.cnf";
        char const* openssl_cmd = "fipsinstall";
        char const* module_arg = "-module";
        char const* out_arg = "-out";
        char const* self_test_arg = "-self_test_oninstall";

        static std::string command;

        command.append(openssl_output_env_var).append(openssl_path).append(1, ' ')
            .append(openssl_cmd).append(1, ' ')
            .append(module_arg).append(1, ' ').append(openssl_output_env_var).append(fips_module_path).append(1, ' ')
            .append(out_arg).append(1, ' ').append(openssl_output_env_var).append(out_path).append(1, ' ')
            .append(self_test_arg);

        free(openssl_output_env_var);

        return command;
    }

    void install_fips()
    {
        FILE* pPipe;
        //char buf[128];

        //std::string const& command = get_command_string();
        std::string const& command = get_command_string_with_env_var();

        if ((pPipe = _popen(command.c_str(), "rt")) == NULL) // the reason this doesn't work is because the fipsinstall command sends its output to stderr and _popen can only read from stdout
        {
            exit(1);
        }

        /*
            while (fgets(buf, 128, pPipe))
            {
                if (strcmp(buf, "INSTALL PASSED")==0)
                {
                    std::cout << "\nfound the string\n";
                    break;
                }
            }

            int endOfFileVal = feof(pPipe);
            int closeReturnVal = _pclose(pPipe);

            std::cout << "\nEnd of file: " << endOfFileVal << ", closing pipe: " << closeReturnVal << '\n';
        */
    }
}