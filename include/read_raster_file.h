#pragma once
#include <sstream>

struct ASCIIGrid {
    int ncols;
    int nrows;
    double xllCorner;
    double yllCorner;
    double cellSize;
    double NODATA_d;
};

class Grid {
private:
    ASCIIGrid header;
    double* data;
    double* x;
    double* y;

    int state;

public:
    Grid();

    Grid(const char* fname);

    ~Grid();

    bool prepare_data_storage();

    bool prepare_coordinates();

    void print();
};


int get_int_val(std::ifstream& fileStream, const char* field);

double get_double_val(std::ifstream& fileStream, const char* field);


