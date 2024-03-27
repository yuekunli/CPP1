#include <iostream>

namespace CheckDotNetVersion {
    void check_dotnet_version();
}

namespace OpensslInstallFips {
    void install_fips();
}

namespace FileModification {
    void modify_file();
}


namespace GetProcessName {
    void get_current_process_name();
}

int main()
{
    //CheckDotNetVersion::check_dotnet_version();
    //OpensslInstallFips::install_fips();
    //FileModification::modify_file();
    GetProcessName::get_current_process_name();
}