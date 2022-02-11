import numpy as np

# by default it will be created in ../data folder
EXTENSION = '.asc'
BASENAME = 'example_raster_'

# Change at will
ncols = 500
nrows = 800
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
def generate_grid_with_gradient(cols, rows, xll, yll, cell, nodata, low=0.1, high=10., direction=None):
    '''
    '''
    r = high - low
    temp = np.zeros((rows, cols))
    for irow in range(rows):
        for icol in range(cols):
            temp[irow, icol] = produce_grad(irow, icol, rows, cols, low, high, r, direction)
    return temp

############################################
def produce_grad(row_, col_, nrows, ncols, low, high, r, direction=None):
    '''
    '''
    row = row_ / nrows
    col = col_ / ncols

    if direction is None:
        return low + row * r + col * r
    
    if direction.lower() == 'x+':
        return low + col  * r
    elif direction.lower() == 'x-':
        return high - col * r
    
    if direction.lower() == 'y+':
        return low + row * r
    elif direction.lower() == 'y-':
        return high - row * r
    
    if direction.lower() == 'xy+':
        return low + row * r + col * r
    elif direction.lower() == 'xy-':
        return high - row * r - col * r
    
    if direction.lower() == 'yx+':
        return low - row * r + col * r
    elif direction.lower() == 'yx-':
        return high + row * r - col * r

############################################
def embed_feature(cols, rows, data, nodataval, direction='up'):
    '''
    '''
    midcol = int(cols / 2)
    midrow = int(rows / 2)
    # Create the straight line
    if direction.lower() == 'up' or direction.lower() == 'down':
        for irow in range(rows):
            data[irow][midcol] = nodataval
    else:
        for icol in range(cols):
            data[midrow][icol] = nodataval

    # Create an arrow-like shape
    if direction.lower() == 'up':
        for i in range(1, 4):
            data[i][midcol - i] = nodataval
            data[i][midcol + i] = nodataval
    elif direction.lower() == 'down':
        for i in range(1, 4):
            data[-1 - i][midcol - i] = nodataval
            data[-1 - i][midcol + i] = nodataval
    elif direction.lower() == 'right':
        for i in range(1, 4):
            data[midrow - i][-1 - i] = nodataval
            data[midrow + i][-1 - i] = nodataval
    elif direction.lower() == 'left':
        for i in range(1, 4):
            data[midrow - i][i] = nodataval
            data[midrow + i][i] = nodataval
    return data

############################################
def write_to_ascii(fname, cols, rows, xll, yll, cell, nodata, data):
    '''
    '''
    with open('../data/' + fname, 'w') as w:
        w.write(f'ncols \t {cols} \n')
        w.write(f'nrows \t {rows} \n')
        w.write(f'xllcorner \t {xll} \n')
        w.write(f'yllcorner \t {yll} \n')
        w.write(f'cellsize \t {cell} \n')
        w.write(f'NODATA_value \t {nodata} \n')
        for irow in range(rows):
            for icol in range(cols):
                w.write('{:10.3f} '.format(data[irow][icol]))    
            w.write('\n')

############################################
if __name__ == '__main__':
    
    nw_to_se_grad_data = generate_grid_with_gradient(ncols, nrows, xllCorner, yllCorner, cellSize, NODATA_d, direction='xy+')
    se_to_nw_grad_data = generate_grid_with_gradient(ncols, nrows, xllCorner, yllCorner, cellSize, NODATA_d, direction='xy-')
    s_to_n_grad_data = generate_grid_with_gradient(ncols, nrows, xllCorner, yllCorner, cellSize, NODATA_d, direction='y-')
    w_to_e_grad_data = generate_grid_with_gradient(ncols, nrows, xllCorner, yllCorner, cellSize, NODATA_d, direction='x+')
    
    nw_to_se_grad_data = embed_feature(ncols, nrows, nw_to_se_grad_data, NODATA_d, direction='up')
    se_to_nw_grad_data = embed_feature(ncols, nrows, se_to_nw_grad_data, NODATA_d, direction='down')
    s_to_n_grad_data = embed_feature(ncols, nrows, s_to_n_grad_data, NODATA_d, direction='right')
    w_to_e_grad_data = embed_feature(ncols, nrows, w_to_e_grad_data, NODATA_d, direction='left')

    write_to_ascii(BASENAME + 'nw_to_se_arrow_up' + EXTENSION, ncols, nrows, xllCorner, yllCorner, cellSize, NODATA_d, nw_to_se_grad_data)
    write_to_ascii(BASENAME + 'se_to_nw_arrow_down' + EXTENSION, ncols, nrows, xllCorner, yllCorner, cellSize, NODATA_d, se_to_nw_grad_data)
    write_to_ascii(BASENAME + 's_to_n_arrow_right' + EXTENSION, ncols, nrows, xllCorner, yllCorner, cellSize, NODATA_d, s_to_n_grad_data)
    write_to_ascii(BASENAME + 'w_to_e_arrow_left' + EXTENSION, ncols, nrows, xllCorner, yllCorner, cellSize, NODATA_d, w_to_e_grad_data)
