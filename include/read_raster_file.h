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

    void fill_with_nodata();

    void print();

    void show_active();

    void read_line_to_data(std::ifstream& fileStream, double* row);
    
    void read_data_from_ascii(std::ifstream& fileStream);

    void get_size(int* rows, int* cols);
};

int get_int_val(std::ifstream& fileStream, const char* field);

double get_double_val(std::ifstream& fileStream, const char* field);

const char* get_ascii_reader_version();

const char* get_ascii_reader_comp_version();

