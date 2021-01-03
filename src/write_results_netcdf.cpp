#include "write_results_netcdf.h"
#include "netcdf.h"

#include <stdio.h>
#include <string>

#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}

#define XNAME "x"
#define YNAME "y"
#define TNAME "t"
#define UNITNAME "m"
#define XDEFINITION "from x=0"
#define YDEFINITION "from y=0"

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

bool open_nc_file_to_write(const char* ncFileName, const int nrows, const int ncols, NCIDS& ncids) {
    int retval = nc_create(ncFileName, NC_CLOBBER, &(ncids.ncid));
    if (retval)
        ERR(retval);

    retval = nc_def_dim(ncids.ncid, XNAME, ncols, &(ncids.x_dimid));
    if (retval)
        ERR(retval);

    retval = nc_def_dim(ncids.ncid, YNAME, nrows, &(ncids.y_dimid));
    if (retval)
        ERR(retval);
    
    retval = nc_def_dim(ncids.ncid, TNAME, NC_UNLIMITED, &(ncids.t_dimid));
    if (retval)
        ERR(retval);

    int x_varid;
    int y_varid;
    retval = nc_def_var(ncids.ncid, XNAME, NC_FLOAT, 1, &(ncids.x_dimid), &x_varid);
    if (retval)
        ERR(retval);

    retval = nc_def_var(ncids.ncid, YNAME, NC_FLOAT, 1, &(ncids.y_dimid), &y_varid);
    if (retval)
        ERR(retval);

    retval = nc_put_att_text(ncids.ncid, x_varid, UNITNAME, strlen(XDEFINITION), XDEFINITION);
    if (retval)
        ERR(retval);
    
    retval = nc_put_att_text(ncids.ncid, y_varid, UNITNAME, strlen(YDEFINITION), YDEFINITION);
    if (retval)
        ERR(retval);

    ncids.dimids[0] = ncids.t_dimid;
    ncids.dimids[1] = ncids.y_dimid;
    ncids.dimids[2] = ncids.x_dimid;

    retval = nc_enddef(ncids.ncid);
    if (retval)
        ERR(retval);

    return true;
}

bool close_nc_file(int ncid) {
    int retval = nc_close(ncid);
    if (retval)
        ERR(retval);
    return true;
}