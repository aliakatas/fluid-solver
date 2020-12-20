#include <stdio.h>
#include <stdlib.h>

class Grid {
    private:

    int ncols;
    int nrows;
    double xllCorner;
    double yllCorner;
    double cellSize;
    double NODATA_d;
    double* data;
    double* x;
    double* y;

    public:
    Grid() {
        ncols = 0;
        nrows = 0;
        xllCorner = 0.0;
        yllCorner = 0.0;
        cellSize = 0.0;
        NODATA_d = -9999.9;
        data = nullptr; 
        x = nullptr;
        y = nullptr;
    };

    ~Grid() {
        if (data)
            free(data);

        if (x)
            free(x);

        if (y)
            free(y);
    };

    bool prepare_data_storage(int rows, int cols) {
        data = (double*)malloc(rows * cols * sizeof(double));
        x = (double*)malloc(rows * cols * sizeof(double));
        y = (double*)malloc(rows * cols * sizeof(double));

        if (!data || !x || !y) {
            printf("ERROR: Memory could not be allocated...\n");
            return false;
        }

        return true;
    };

    bool prepare_coordinates() {
        int idx = 0;

        for (auto irow = 0; irow < nrows; ++irow) {
            for (auto icol = 0; icol < ncols; ++icol) {
                idx = irow * ncols + icol;
                x[idx] = xllCorner + icol * cellSize;
                y[idx] = yllCorner + (nrows - 1 - irow) * cellSize;
            }
        }
        return true;
    }





};


int main() {
    printf("Hello World! \n");
    
    printf("Goodbye!\n\n");
    return 0;
}