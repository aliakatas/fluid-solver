#include "write_results_netcdf.h"
#include "netcdf.h"

#include <stdio.h>
#include <string>

const char* get_writer_netcdf_version() {
    char* msg = new char[128];
    memset(msg, 0, 128);
    sprintf(msg, "netCDF %s", nc_inq_libvers());
    return msg;
}

const char* get_writer_netcdf_comp_version() {
    char* compVer = new char[32];
    memset(compVer, 0, 32);

#ifdef __INTEL_COMPILER
    char compiler[] = "Intel C++ ";
    sprintf(compVer, "%d.%d", __INTEL_COMPILER / 10000, __INTEL_COMPILER % 10000);
#else
    char compiler[] = "MSVC ";
    sprintf(compVer, "%d.%d", _MSC_VER / 100, _MSC_VER % 100);
#endif

    char* compVersion = new char[32];
    memset(compVersion, 0, 32);
    if (compVersion) {
        strncat(compVersion, compiler, 12);
        strncat(compVersion, compVer, 12);
    }

    delete[] compVer;
    return compVersion;
}
