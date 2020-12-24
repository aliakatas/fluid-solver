#include "read_raster_file.h"
#include "netcdf.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    printf("2D Fluid Solver starting... \n");

    if (argc < 2)
        return 1;

    printf("ASCII reader version: %s \n", get_ascii_reader_version());
    printf("   built with: %s \n", get_ascii_reader_comp_version());
    printf("netCDF version: %s \n", nc_inq_libvers());


    Grid tempGrid(argv[1]);
    tempGrid.print();
    printf("\n");
    tempGrid.show_active();
    printf("\n");

    
    printf("Goodbye!\n\n");
    return 0;
}