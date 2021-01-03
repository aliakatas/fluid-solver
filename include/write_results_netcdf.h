#pragma once

#define NDIMS 3

struct NCIDS {
	int ncid;
	int x_dimid;
	int y_dimid; 
	int t_dimid;
	int varid;
	int dimids[NDIMS];
	int temp_varid;
};

const char* get_writer_netcdf_version();

const char* get_writer_netcdf_comp_version();

bool open_nc_file_to_write(const char* ncFileName, const int nrows, const int ncols, double xll, double yll, double dx, double nodata, NCIDS& ncids);

bool write_nc_record(NCIDS& ncids, int rec, int rows, int cols, const double* temperature);

bool close_nc_file(int ncid);
