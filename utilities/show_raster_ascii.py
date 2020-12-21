import numpy as np
from matplotlib import pyplot as plt

# by default it will read from ../data folder
filename = 'dummy_raster.asc'
filenameFeature = 'dummy_raster_feature.asc'

############################################
def show_raster_ascii(fname):
    '''
    '''
    with open('../data/' + fname, 'r') as f:
        lines = f.readlines()
    
    ncols = int(lines[0].split()[1])
    nrows = int(lines[1].split()[1])
    xllCorner = float(lines[2].split()[1])
    yllCorner = float(lines[3].split()[1])
    cellSize = float(lines[4].split()[1])
    NODATA_d = float(lines[5].split()[1])

    data = np.zeros((nrows,ncols))
    lineOffset = 6
    for irow in range(nrows):
        line = lines[irow + lineOffset]
        vals = line.split()
        for icol in range(ncols):
            data[irow][icol] = float(vals[icol])
            if data[irow][icol] <= NODATA_d:
                data[irow][icol] = np.nan

    x = np.linspace(xllCorner, xllCorner + cellSize * (ncols - 1), ncols)
    y = np.linspace(yllCorner + cellSize * (nrows - 1), yllCorner, nrows)
    xv, yv = np.meshgrid(x, y)
    
    plt.pcolor(xv, yv, data)
    #plt.pcolor(data)
    plt.show()

############################################
if __name__ == '__main__':
    #show_raster_ascii(filename)
    show_raster_ascii(filenameFeature)
