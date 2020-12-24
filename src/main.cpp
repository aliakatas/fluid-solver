#include "read_raster_file.h"
#include <stdio.h>


int main(int argc, char* argv[]) {
    printf("Starting... \n");

    if (argc < 2)
        return 1;

    Grid tempGrid(argv[1]);

    tempGrid.print();
    printf("\n");
    tempGrid.show_active();
    printf("\n");
    printf("Goodbye!\n\n");
    return 0;
}