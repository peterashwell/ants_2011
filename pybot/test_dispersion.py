import psyco
import random
import time

psyco.full()
# apply simple dispersion function to grid
rows = 200
cols = 200
grid = [[random.random() for i in xrange(cols)] for j in xrange(rows)]
def disperse(grid):
	new_grid = [[0 for c in xrange(cols)] for r in xrange(rows)]
	for row in xrange(rows):
		for col in xrange(cols):
			if row > 0 and row < (rows - 1) and col > 0 and col < (cols - 1):
				center_energy = grid[row][col]
				adjsum = 0
				adjsum += grid[row - 1][col] - center_energy
				adjsum += grid[row + 1][col] - center_energy
				adjsum += grid[row][col + 1] - center_energy
				adjsum += grid[row][col - 1] - center_energy
				adjsum /= 4.0
				new_val = 0.5 * (center_energy + adjsum)
				new_grid[row][col] = new_val
	return new_grid


start = time.time()
for i in xrange(10):
	#for r in xrange(rows):
	#	for c in xrange(cols):
	#		x = grid[r][c]
	grid = disperse(grid)
print "elapsed: {0}".format(time.time() - start)
