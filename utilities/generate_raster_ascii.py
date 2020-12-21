import numpy as np

# by default it will be created in ../data folder
filename = 'dummy_raster.asc'
filenameFeature = 'dummy_raster_feature.asc'

# Change at will
ncols = 50
nrows = 80
xllCorner = 0.0
yllCorner = 0.0
cellSize = 2.0
NODATA_d = -9999.9

############################################
def generate_raster_ascii(fname, cols, rows, xll, yll, cell, nodata, low=0.1, high=10.0, tick=20.0):
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
        threshold = (high - low) / tick

        for irow in range(rows):
            for icol in range(cols):
                if temp[irow][icol] <= (low + threshold):
                    w.write('{:10.3f}'.format(nodata))
                elif temp[irow][icol] >= (high - threshold):
                    w.write('{:10.3f}'.format(nodata))
                else:
                    w.write('{:10.3f}'.format(temp[irow][icol]))
                w.write(' ')
            w.write('\n')

############################################
def generate_raster_ascii_with_feature(fname, cols, rows, xll, yll, cell, nodata, low=0.1, high=10.0):
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
        midcol = int(cols / 2)
        
        # Create an arrow-like shape
        for i in range(1, 4):
            temp[i][midcol - i] = nodata
            temp[i][midcol + i] = nodata
        for irow in range(rows):
            temp[irow][midcol] = nodata

        for irow in range(rows):
            for icol in range(cols):
                w.write('{:10.3f} '.format(temp[irow][icol]))    
            w.write('\n')

############################################
if __name__ == '__main__':
    generate_raster_ascii(filename, ncols, nrows, xllCorner, yllCorner, cellSize, NODATA_d)
    generate_raster_ascii_with_feature(filenameFeature, ncols, nrows, xllCorner, yllCorner, cellSize, NODATA_d)

