#pragma once

struct NCIDS {
	int ncid;
	int x_dimid;
	int y_dimid; 
	int t_dimid;
	int varid;
	int dimids[4];
};

const char* get_writer_netcdf_version();

const char* get_writer_netcdf_comp_version();

bool open_nc_file_to_write(const char* ncFileName, const int nrows, const int ncols, NCIDS& ncids);

bool close_nc_file(int ncid);
