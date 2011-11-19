import ants
import sys

# Some important statics
DIRECTIONS = ['n', 's', 'e', 'w']
DISP_ITERS = 20

# TODO replace this with a simple for loop iterating over each of the dispersion fields from the separate genomes
# AntForce accumulates the forces on a particular ant
#class AntForce:
#	def __init__(self, row, col):
#		self.ant_row = row
#		self.ant_col = col
#		# initialize forces in all directions
#		self.forces = {}
#		for d in ants.AIM.keys(): # 'n', 's', etc
#			self.forces[d] = 0 
#		self.THRESHOLD = 1e-10
#
#	def getPosition(self):
#		return (self.ant_row, self.ant_col)
#	
#	# Add a force in a particular direction to the ant
#	def addForce(self, direction, amt): 
#		self.forces[direction] += amt
#	
#	# Choose the best action available on the map for the ant	
#	def applyForces(self, ants, gs):
#		sorted_forces = sorted(self.forces.items(), key=lambda o: o[1], reverse=True)
#		pos = self.getPosition()
#		for dir_force in sorted_forces:
#			direction, force = dir_force
#			dest = ants.destination(pos, direction)
#			if force < self.THRESHOLD \
#				or not ants.passable(dest) \
#				or (not ants.unoccupied(dest) and not gs.vacatedNow(dest)) \
#				or gs.movingTo(dest):
#				continue
#			# actually issue the order according to strongest actionable force
#			ants.issue_order((pos, direction))
#			gs.updateMoving(dest)
#			gs.updateVacated(dest)
#		# no valid or worthwhile action can be made

# Class to manage the protection of hills by at least one ant
# from the start of the game to the end
class HillProtector:
	# assign a particular ant to protecting each of our hills
	# until he dies, at which point replacements are found
	def __init__(self, ants):
		# There should be one ant near each of our hills. Assign the closest
		# ant to each hill as its protector
		self.hill_protectors = [] # hill <--> protector tuples
		for hill in ants.my_hills():
			protector = findNewProtector(ants, hill)
			self.hill_protectors.append((hill, protector))
	
	def findNewProtector(self, ants, hill):
		closest = None
		closest_dist = None
		for ant in ants.my_ants():
			distance = (ant[0] - hill[0]) ** 2 + (ant[1] - hill[1]) ** 2
			if ant in [obj[0] for obj in self.hill_protectors]:
				continue # ant already assigned as protector
			if closest is None:
				closest = ant
				closest_dist = distance
			elif distance < closest_dist:
				closest = ant
				closest_dist = distance

		return closest

	# Express the genome while protecting the hills
	def protect(self, ants, gs, genome):
		update_position_for = []
		for hill_prot in self.hill_protectors:
			hill, protector = hill_prot
			disp_fields = []
			# Express modified genes
			for gene in genome:
				# cansee returns True for squares that can see the hill
				canseehill = lambda ant: utils.canSee(ant, hill, ants.viewdist2)
				disp_fields.append(gene.express(ants, self.gs, canseehill))
			# apply the modified dispersion fields to the protector
			dest = utils.applyForces(protector, disp_fields)
			# Update the position of the hill protector in our local environment
			update_position_for.append((hill_prot, dest))
		# Update our understandings of the positions of the hill protectors
		for hp_new in update_position_for:
			hill_prot, new_pos = hp_new
			self.hill_protectors.remove(hill_prot)
			self.hill_protectors.append(new_pos)

	# Update the hill protectors at new turn (in case they died, or hill died)
	def updateTurn(self, ants, gs):
		# check if one died
		find_new = []
		for hill_prot in self.hill_protectors:
			hill, protector = hill_prot
			found = False
			for ant in ants.my_ants():
				if ant == protector:
					found = True
			if not found:
				find_new.append(protector)
		new_hps = []
		for hill_prot in self.hill_protectors:
			hill, protector = hill_prot
			if protector in find_new:
				new_hps.append(findNewProtector(hill, ants, gs))
			else:
				new_hps.append(hill_prot)
		self.hill_protectors = new_hps
	
# Compute the dispersions across the map and apply them to our ants
#def disperse(ants, gs, disp_function, iterations):
	# declare the map we will work with
#	disp_grid = [[0 for i in xrange(gs.map_cols)] for j in gs.map_rows]
	# disperse for i iterations
#	for iter in xrange(iterations):
#		for row in xrange(gs.map_rows):
#			for col in xrange(gs.map_cols):
#				# Call dispersion function from the gene (see Genes.py)
#				disp_grid[row][col] = disp_function(row, col, disp_grid, gs, ants_state)

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
	pprintdg(grid, ants)
	pprintlm(ants)
	# apply dispersion forces to the AntForce objects
	for af in afs:
		for d in DIRECTIONS:
			dest_x, dest_y = ants.destination(af.getPosition(), d) # see ants.py -> destination
			af.addForce(d, grid[dest_x][dest_y]) # add force from particular direction

# apply dispersion fields to ant
def applyForce(ant, dfs):
	forces = {}
	# initialize forces to 0
	for d in utils.DIRECTIONS:
		forces[d] =  # TODO accumulate df forces
	sorted_forces = sorted(self.forces.items(), key=lambda o: o[1], reverse=True)
	for dir_force in sorted_forces:
		direction, force = dir_force
		dest = ants.destination(ant, direction)
		if force < self.THRESHOLD \
			or not ants.passable(dest) \
			or (not ants.unoccupied(dest) and not gs.vacatedNow(dest)) \
			or gs.movingTo(dest):
			continue
		# actually issue the order according to strongest actionable force
		ants.issue_order((pos, direction))
		gs.updateMoving(dest)
		gs.updateVacated(dest)
		break
	
# Checks is query is visible from center according to current ants instance
def canSee(center, query, viewdist2):
	# compute distance from center to query (squared)
	d = (center[0] - query[0]) ** 2 + (center[1] - query[1]) ** 2
	if d <= viewdist2:
		return True
	return False
	 
