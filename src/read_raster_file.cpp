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

        if (!prepare_data_storage()) {
            state = -1;
            return;
        }

        prepare_coordinates();
        read_data_from_ascii(asciiFile);
        asciiFile.close();
        state = 0;
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
        return false;
    }

    x = (double*)malloc(rows * cols * sizeof(double));
    if (!x) {
        printf("ERROR: Memory could not be allocated for Grid::x...\n");
        return false;
    }
    y = (double*)malloc(rows * cols * sizeof(double));
    if (!y) {
        printf("ERROR: Memory could not be allocated for Grid::y...\n");
        return false;
    }
    
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

void Grid::fill_with_nodata() {
    int N = header.ncols * header.nrows;
    // may worth exploring an alternative to memset()?
    if (data) {
        for (auto i = 0; i < N; ++i)
            data[i] = header.NODATA_d;
    }
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

void Grid::show_active() {
    int idx = 0;
    if (data) {
        for (auto irow = 0; irow < header.nrows; ++irow) {
            for (auto icol = 0; icol < header.ncols; ++icol) {
                idx = header.ncols * irow + icol;
                if (data[idx] > header.NODATA_d)
                    printf("%s", "+");
                else 
                    printf("%s", "O");
            }
            printf("\n");
        }
    }
    else
        printf("Data not allocated...\n");
}

void Grid::read_line_to_data(std::ifstream& fileStream, double* row) {
    std::string line;
    std::getline(fileStream, line);
    std::stringstream ss(line);

    for (auto icol = 0; icol < header.ncols; ++icol)
        ss >> row[icol];
}

void Grid::read_data_from_ascii(std::ifstream& fileStream) {
    for (auto irow = 0; irow < header.nrows; ++irow)
        read_line_to_data(fileStream, &(data[header.ncols * irow]));
}

void Grid::get_size(int* rows, int* cols) {
    *rows = header.nrows;
    *cols = header.ncols;
}

const double* Grid::get_data() {
    const double* out = data;
    return out;
}

double Grid::get_xll() {
    return header.xllCorner;
}

double Grid::get_yll() {
    return header.yllCorner;
}

double Grid::get_cellsize() {
    return header.cellSize;
}

double Grid::get_nodata_val() {
    return header.NODATA_d;
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

const char* get_ascii_reader_version() {
    char date[] = __DATE__;
    char time[] = __TIME__;

    char* buildTime = new char[32];
    memset(buildTime, 0, 32);
    if (buildTime) {
        strncat(buildTime, date, 12);
        strncat(buildTime, " ", 1);
        strncat(buildTime, time, 12);
    }

    return buildTime;
}

const char* get_ascii_reader_comp_version() {
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
