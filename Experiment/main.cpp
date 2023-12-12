#include <iostream>

namespace CheckDotNetVersion {
    void check_dotnet_version();
}

namespace OpensslInstallFips {
    void install_fips();
}

int main()
{
    CheckDotNetVersion::check_dotnet_version();
    OpensslInstallFips::install_fips();
}