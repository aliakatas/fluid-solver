#include "write_results_netcdf.h"
#include "netcdf.h"

#include <stdio.h>
#include <string>

#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}

#define XNAME "x"
#define YNAME "y"
#define TNAME "t"
#define UNITNAME "units"
#define XDEFINITION "m from x=0"
#define YDEFINITION "m from y=0"
#define TEMPNAME "temperature"
#define TEMPUNITS "Kelvin"

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

bool open_nc_file_to_write(const char* ncFileName, const int nrows, const int ncols, double xll, double yll, double dx, double nodata, NCIDS& ncids) {
    // Create the file
    int retval = nc_create(ncFileName, NC_NETCDF4 | NC_CLOBBER, &(ncids.ncid));
    if (retval)
        ERR(retval);

    // Define dimensions
    retval = nc_def_dim(ncids.ncid, XNAME, ncols, &(ncids.x_dimid));
    if (retval)
        ERR(retval);

    retval = nc_def_dim(ncids.ncid, YNAME, nrows, &(ncids.y_dimid));
    if (retval)
        ERR(retval);
    
    retval = nc_def_dim(ncids.ncid, TNAME, NC_UNLIMITED, &(ncids.t_dimid));
    if (retval)
        ERR(retval);

    // Define coordinate variables
    int x_varid;
    int y_varid;
    retval = nc_def_var(ncids.ncid, XNAME, NC_DOUBLE, 1, &(ncids.x_dimid), &x_varid);
    if (retval)
        ERR(retval);

    retval = nc_def_var(ncids.ncid, YNAME, NC_DOUBLE, 1, &(ncids.y_dimid), &y_varid);
    if (retval)
        ERR(retval);

    // Assign attributes to coordinates
    retval = nc_put_att_text(ncids.ncid, x_varid, UNITNAME, strlen(XDEFINITION), XDEFINITION);
    if (retval)
        ERR(retval);
    
    retval = nc_put_att_text(ncids.ncid, y_varid, UNITNAME, strlen(YDEFINITION), YDEFINITION);
    if (retval)
        ERR(retval);

    ncids.dimids[0] = ncids.t_dimid;
    ncids.dimids[1] = ncids.y_dimid;
    ncids.dimids[2] = ncids.x_dimid;

    // Define the netCDF variable for temperature data.
    retval = nc_def_var(ncids.ncid, TEMPNAME, NC_DOUBLE, NDIMS, ncids.dimids, &(ncids.temp_varid));
    if (retval)
        ERR(retval);
    /*
    if ((retval = nc_def_var(ncid, TEMP_NAME, NC_FLOAT, NDIMS,
        dimids, &temp_varid)))
        ERR(retval);
    */

    // Assign units attributes to the netCDF variable
    retval = nc_put_att_text(ncids.ncid, ncids.temp_varid, UNITNAME, strlen(TEMPUNITS), TEMPUNITS);
    if (retval)
        ERR(retval);
    retval = nc_put_att_double(ncids.ncid, ncids.temp_varid, "missing_value", NC_DOUBLE, 1, &nodata);
    if (retval)
        ERR(retval);
    /*
    if ((retval = nc_put_att_text(ncid, temp_varid, UNITS,
        strlen(TEMP_UNITS), TEMP_UNITS)))
        ERR(retval);
    */

    // Leave definition mode
    retval = nc_enddef(ncids.ncid);
    if (retval)
        ERR(retval);

    // Write the coordinate variable data.
    double* x = (double*)malloc(ncols * sizeof(double));
    double* y = (double*)malloc(nrows * sizeof(double));
    for (auto i = 0; i < ncols; ++i)
        x[i] = xll + i * dx;
    for (auto i = 0; i < nrows; ++i)
        y[i] = yll + (nrows - i - 1) * dx;

    retval = nc_put_var_double(ncids.ncid, x_varid, x);
    if (retval)
        ERR(retval);

    retval = nc_put_var_double(ncids.ncid, y_varid, y);
    if (retval)
        ERR(retval);

    free(x);
    free(y);
    return true;
}

bool write_nc_record(NCIDS& ncids, int rec, int rows, int cols, const double* temperature) {
    size_t count[NDIMS];
    size_t start[NDIMS];
    int retval;

    count[0] = 1;
    count[1] = rows;
    count[2] = cols;
    
    start[0] = rec;
    start[1] = 0;
    start[2] = 0;

    /*for (auto row = 0; row < count[1]; ++row) {
        for (auto col = 0; col < count[2]; ++col) {
            printf("%f ", temperature[row * cols + col]);
        }
        printf("\n");
    }*/

    retval = nc_put_vara_double(ncids.ncid, ncids.temp_varid, start, count, temperature);
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