import utils
import sys

# Class Gene is the superclass of all other genes
# - Specifies id, size parameters, that's about it
class Gene:
	def __init__(self, id, size, param_names):
		self.id = id
		self.size = size
		self.param_names = param_names

# This gene controls parameters governing exploratory behavior
# - unex_attract is how attractive unexplored squares are
# - unex_decay is the order of the polynomial function f
#		where f(time since visited t_u) ~ attraction = f_u^unex_decay
# - dispersion_coeff is the dispersion coefficient which governs
#		how attractive unexplored squares appear from a distance
class ExploreGene(Gene):
	def __init__(self):
		# Set up super class
		Gene.__init__(self, 'explore', 3, ['unex_attract', 'unex_decay', 'disp_coeff'])
			
		# Initialize gene parameter(s)
		self.params =\
		{\
			'unex_attract' : 1.0,\
			'dispersion_coeff' : 0.9,\
			'unex_decay' : 1\
		}
	
	# Dispersion function to use in the grid filling dispersion algorithm
	# Writes updated values from grid_old to grid_new
	# Includes information from ants instance, and local gamestate data
	# active_func says is a particular square is actually to cause forces
	def disperse_once(self, ants, gs, grid_old, active_func):
		# Default cell energy is 0, cell has no attractive force
		grid_new = [[-1 for i in xrange(ants.cols)] for j in xrange(ants.rows)]
		cell_energy = 0
		# Cell energy for unexplored squares is unex_attract
		for row in xrange(ants.rows):
			for col in xrange(ants.cols):
				if not active_func((row, col)) or not ants.passable((row, col)):
					continue
				
				# Compute the energy for the center square
				# Square is more attractive the longer it hasn't been seen
				last_seen = gs.turnsSinceVisible(row, col)
				center_energy = 0 # assume center energy is 0 (unless otherwise)
				if last_seen != 0:
					center_energy = min(1, self.params['unex_attract']\
						 * (1.0 - 1.0 / last_seen) ** self.params['unex_decay'])
					grid_new[row][col] = center_energy
					#sys.stderr.write('setting center to: ' + str(center_energy) + '\n')
					continue
				# Compute the sum of the adjacent squares and how many were used
				adj_sum = 0
				dirs_used = 0
				for d in utils.DIRECTIONS:
					adjrow, adjcol = ants.destination((row, col), d)
					if ants.passable((adjrow, adjcol)) and active_func((adjrow, adjcol)):
						#sys.stderr.write('inadjsum (go): ' + str(grid_old[adjrow][adjcol]) + '\n')
						#sys.stderr.write('inadjsum (ce): ' + str(center_energy) + '\n')
						adj_sum += (grid_old[adjrow][adjcol] - center_energy)
						dirs_used += 1
				adjenergy = 0
				#sys.stderr.write('adj_sum: ' + str(adj_sum) + '\n')
				#sys.stderr.write('center_energy: ' + str(center_energy) + '\n')
				#sys.stderr.write('dirs_used:' + str(dirs_used) + '\n')
				if dirs_used > 0:
					adjenergy = adj_sum / dirs_used

				# Finally compute the energy of the new grid square
				new_entry = self.params['dispersion_coeff'] * (center_energy + adjenergy)
				grid_new[row][col] = new_entry
		return grid_new

	def express(self, ants, gs, afs, active_func=lambda o: True):
		#sys.stderr.write('expressing ' + self.id + '\n')
		# include the activity function for the map squares into the dispersion function
		disperse_func = lambda ants, gs, grid_old : self.disperse_once(ants, gs, grid_old, active_func)
		return utils.disperse(ants, gs, utils.DISP_ITERS, afs, disperse_func)

class FoodGene(Gene):
	def __init__(self):
		# Set up super class
		Gene.__init__(self, 'food', 1, ['food_attract'])
			
		# Initialize gene parameter(s)
		self.params =\
		{\
			'food_attract' : 1.0,\
		}
	
	# Dispersion function to use in the grid filling dispersion algorithm
	# Writes updated values from grid_old to grid_new
	# Includes information from ants instance, and local gamestate data
	def disperse_once(self, ants_instance, gs, grid_old, active_func):
		# Default cell energy is 0, cell has no attractive force
		grid_new = [[0 for i in xrange(ants_instance.cols)] for j in xrange(ants_instance.rows)]
		cell_energy = 0
		# Cell energy for unexplored squares is unex_attract
		#sys.stderr.write('food locations: ' + str(ants_instance.food()) + '\n')
		for row in xrange(ants_instance.rows):
			for col in xrange(ants_instance.cols):
				if not active_func((row, col)):
					grid_new[row][col] = -1
					continue
				if (row, col) in ants_instance.food():
					grid_new[row][col] = self.params['food_attract']
					continue
				# Dispersion function
				adjenergy_sum = 0
				dirs_used = 0
				for d in utils.DIRECTIONS:
					adjrow, adjcol = ants_instance.destination((row, col), d)
					if ants_instance.passable((adjrow, adjcol)) and active_func((adjrow, adjcol)):
						adjenergy_sum += (grid_old[adjrow][adjcol] - grid_old[row][col])
						dirs_used += 1
				adjenergy = 0
				if dirs_used > 0:
					adjenergy = adjenergy_sum / dirs_used
				##sys.stderr.write('center energy:' + str(center_energy) + '\n')
				#sys.stderr.write('adj energy:' + str(adjenergy) + '\n')
				#sys.stderr.write('disp coeff:' + str(self.params['dispersion_coeff']) + '\n')
				#sys.stderr.write('new entry: ' + str(new_entry) + '\n')
				#sys.stderr.flush()
				grid_new[row][col] = grid_old[row][col] + adjenergy
		return grid_new

	def express(self, ants_instance, gs, afs, active_func=lambda o: True):
		#sys.stderr.write('expressing ' + self.id + '\n')
		# include the activity function for the map squares into the dispersion function
		disperse_func = lambda ants_instance, gs, grid_old : self.disperse_once(ants_instance, gs, grid_old, active_func)
		return utils.disperse(ants_instance, gs, utils.DISP_ITERS, afs, disperse_func)
