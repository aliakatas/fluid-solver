import numpy as np

# by default it will be created in ../data folder
filename = 'dummy_raster.asc'

# Change at will
ncols = 8
nrows = 5
xllCorner = 0.0
yllCorner = 0.0
cellSize = 2.0
NODATA_d = -9999.9

############################################
def generate_raster_ascii(fname, cols, rows, xll, yll, cell, nodata, low=0.1, high=10.0):
    '''
    '''
    with open('../data/' + fname, 'w') as w:
        w.write(f'ncols \t {cols} \n')
        w.write(f'nrows \t {rows} \n')
        w.write(f'xllcorner \t {xll} \n')
        w.write(f'yllcorner \t {yll} \n')
        w.write(f'cellsize \t {cell} \n')
        w.write(f'NODATA_value \t {nodata} \n')

        temp = np.random.uniform(low=low, high=high, size=(rows,cols))

        for irow in range(rows):
            for icol in range(cols):
                if temp[irow][icol] <= (low + (high - low) / 10.0):
                    w.write('%10.3f'.format(nodata))
                elif temp[irow][icol] >= (high - (high - low) / 10.0):
                    w.write('%10.3f'.format(nodata))
                else:
                    w.write('%10.3f'.format(temp[irow][icol]))
                w.write(' ')
            w.write('\n')

if __name__ == '__main__':
    generate_raster_ascii(filename, ncols, nrows, xllCorner, yllCorner, cellsize, NODATA_d)
