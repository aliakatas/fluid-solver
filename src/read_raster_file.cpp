#include "read_raster_file.h"
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>


Grid::Grid() {
    header.ncols = 0;
    header.nrows = 0;
    header.xllCorner = 0.0;
    header.yllCorner = 0.0;
    header.cellSize = 0.0;
    header.NODATA_d = -9999.9;
    data = nullptr; 
    x = nullptr;
    y = nullptr;
    state = 1;
}

Grid::Grid(const char* fname) {
    std::ifstream asciiFile(fname);
    if (asciiFile.is_open()) {
        
        header.ncols = get_int_val(asciiFile, "ncols");
        header.nrows = get_int_val(asciiFile, "nrows");

        header.xllCorner = get_double_val(asciiFile, "xllcorner");
        header.yllCorner = get_double_val(asciiFile, "yllcorner");
        header.cellSize = get_double_val(asciiFile, "cellsize");
        header.NODATA_d = get_double_val(asciiFile, "NODATA_value");



        asciiFile.close();
    }
    else
        state = -1;
}

Grid::~Grid() {
    if (data)
        free(data);
        
    if (x)
        free(x);
        
    if (y)
        free(y);
}

bool Grid::prepare_data_storage() {
    int rows, cols;
    rows = header.nrows;
    cols = header.ncols;

    data = (double*)malloc(rows * cols * sizeof(double));
    if (!data) {
        printf("ERROR: Memory could not be allocated for Grid::data...\n");
        state = -1;
        return false;
    }

    x = (double*)malloc(rows * cols * sizeof(double));
    if (!x) {
        printf("ERROR: Memory could not be allocated for Grid::x...\n");
        state = -1;
        return false;
    }
    y = (double*)malloc(rows * cols * sizeof(double));
    if (!y) {
        printf("ERROR: Memory could not be allocated for Grid::y...\n");
        state = -1;
        return false;
    }
    
    state = 0;
    return true;
}

bool Grid::prepare_coordinates() {
    int idx = 0;
    
    for (auto irow = 0; irow < header.nrows; ++irow) {
        for (auto icol = 0; icol < header.ncols; ++icol) {
            idx = irow * header.ncols + icol;
            x[idx] = header.xllCorner + icol * header.cellSize;
            y[idx] = header.yllCorner + (header.nrows - 1 - irow) * header.cellSize;
        }
    }
    return true;
}

void Grid::print() {
    printf("_____ Grid _____ \n");
    
    if (state < 0) {
        printf("Errors occured, Grid is not ready to use... \n ");
        return;
    }
        
    printf("   Columns :: %d \n", header.ncols);
    printf("      Rows :: %d \n", header.nrows);
    printf("Lower-left :: (%f, %f) \n", header.xllCorner, header.yllCorner);
    printf(" Cell size :: %f \n", header.cellSize);
    printf("    NODATA :: %f \n", header.NODATA_d);
    if (state > 0)
        printf("Grid may not ready to use... \n ");
}

int get_int_val(std::ifstream& fileStream, const char* field) {
    std::string line;
    std::getline(fileStream, line);
    std::stringstream ss(line);
    ss.ignore(strlen(field), ' ');
    int out = 0;
    ss >> out;
    return out;
}

double get_double_val(std::ifstream& fileStream, const char* field) {
    std::string line;
    std::getline(fileStream, line);
    std::stringstream ss(line);
    ss.ignore(strlen(field), ' ');
    double out = 0.0;
    ss >> out;
    return out;
}
