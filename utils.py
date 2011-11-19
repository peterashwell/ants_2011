import ants
import sys

# Some important statics
DIRECTIONS = ['n', 's', 'e', 'w']
DISP_ITERS = 20

# AntForce accumulates the forces on a particular ant
class AntForce:
	def __init__(self, row, col):
		self.ant_row = row
		self.ant_col = col
		# initialize forces in all directions
		self.forces = {}
		for d in ants.AIM.keys(): # 'n', 's', etc
			self.forces[d] = 0 
		self.THRESHOLD = 1e-10

	def getPosition(self):
		return (self.ant_row, self.ant_col)
	
	# Add a force in a particular direction to the ant
	def addForce(self, direction, amt): 
		self.forces[direction] += amt
	
	# Choose the best action available on the map for the ant	
	def applyForces(self, ants_instance, gs):
		sorted_forces = sorted(self.forces.items(), key=lambda o: o[1], reverse=True)
		pos = self.getPosition()
		for dir_force in sorted_forces:
			direction, force = dir_force
			dest = ants_instance.destination(pos, direction)
			if force < self.THRESHOLD \
				or not ants_instance.passable(dest) \
				or (not ants_instance.unoccupied(dest) and not gs.vacatedNow(dest)) \
				or gs.movingTo(dest):
				continue
			# actually issue the order according to strongest actionable force
			ants_instance.issue_order((pos, direction))
			gs.updateMoving(dest)
			gs.updateVacated(dest)
			return dest
		return None
		# no valid or worthwhile action can be made

# Pretty print the visibility grid
def pprintvg(gs):
	row = []
	for r in xrange(gs.num_rows):
		for c in xrange(gs.num_cols):
			row.append(str(gs.turnsSinceVisible(r, c)))
		sys.stderr.write('vg ' + ' '.join(row) + '\n')
		row = []
		 
# Pretty print the dispersion grid
def pprintdg(grid, ants):
	hillrow, hillcol = ants.my_hills()[0]
	lbound = max(0, hillrow - 10)
	rbound = min(ants.rows - 1, hillrow + 10)
	bbound = max(0, hillcol - 10)
	tbound = min(ants.cols - 1, hillcol + 10)
	for rownum, row in enumerate(grid):
		if rownum < lbound or rownum > rbound:
			continue
		new_row = []
		for elnum, elem in enumerate(row):
			if elnum < bbound or elnum > tbound:
				continue
			new_elem = '%.3f' % (elem)
			new_row.append(new_elem)
		sys.stderr.write('df ' + ' '.join(new_row) + '\n')

def pprintlm(ants):
	hillrow, hillcol = ants.my_hills()[0]
	lbound = max(0, hillrow - 10)
	rbound = min(ants.rows - 1, hillrow + 10)
	bbound = max(0, hillcol - 10)
	tbound = min(ants.cols - 1, hillcol + 10)
	for rownum, row in enumerate(xrange(ants.rows)):
		if rownum < lbound or rownum > rbound:
			continue
		new_row = []
		for colnum, col in enumerate(xrange(ants.cols)):
			if colnum < bbound or colnum > tbound:
				continue
			if not ants.passable((row, col)):
				new_elem = '  x  '
			elif (row, col) in ants.my_ants():
				new_elem = '  a  '
			else:
				new_elem = '  -  ' 
			new_row.append(new_elem)			
		sys.stderr.write('m  ' + ' '.join(new_row) + '\n')

# Create an empty dispersion field and iterate iters times to disperse
# Applied the disp_func of a particular genome 
def disperse(ants, gs, iters, afs, disp_func):
	grid = [[0 for i in xrange(ants.cols)] for j in xrange(ants.rows)]
	for i in xrange(iters):
		grid = disp_func(ants, gs, grid)
	#pprintdg(grid, ants)
	#pprintlm(ants)
	# apply dispersion forces to the AntForce objects
	for af in afs:
		for d in DIRECTIONS:
			dest_x, dest_y = ants.destination(af.getPosition(), d) # see ants.py -> destination
			af.addForce(d, grid[dest_x][dest_y]) # add force from particular direction

# Checks is query is visible from center according to current ants instance
def canSee(center, query, viewradius2):
	# compute distance from center to query (squared)
	d = (center[0] - query[0]) ** 2 + (center[1] - query[1]) ** 2
	if d <= viewradius2:
		return True
	return False

def my_hills(ants_instance):
	mine = []
	for h in ants_instance.hill_list.items():
		pos, owner = h
		if owner == ants.MY_ANT:
			mine.append(pos)
	return mine
