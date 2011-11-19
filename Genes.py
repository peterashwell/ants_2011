import utils
import sys

# Class Gene is the superclass of all other genes
# - Specifies id, size parameters, that's about it
class Gene:
	def __init__(self, id, size, param_names):
		self.id = id
		self.size = size
		self.param_names = param_names
		
	# Express the genome on an ants instance (with gamestate)
	# Called from the derived class with its own disperse_once
	def express(self, ants, gs, afs, disp_func):
		utils.disperse(ants, gs, utils.DISP_ITERS, afs, disp_func)

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
	def disperse_once(self, ants, gs, grid_old):
		# Default cell energy is 0, cell has no attractive force
		grid_new = [[0 for i in xrange(ants.cols)] for j in xrange(ants.rows)]
		cell_energy = 0
		# Cell energy for unexplored squares is unex_attract
		for row in xrange(ants.rows):
			for col in xrange(ants.cols):
				if not ants.passable((row, col)):
					continue
				last_seen = gs.turnsSinceVisible(row, col)
				if last_seen == 0:
					center_energy = 0
				else:
					# Square is more attractive the longer it hasn't been seen
					center_energy = min(1, self.params['unex_attract']\
						 * (1.0 - 1.0 / last_seen) ** self.params['unex_decay'])
					grid_new[row][col] = center_energy
					continue # skip computing diffusion for this cell
				# Average influence from adjacent squares
				#utils.pprintvg(gs)
				adjenergy_sum = 0
				dirs_used = 0
				for d in utils.DIRECTIONS:
					adjrow, adjcol = ants.destination((row, col), d)
					if ants.passable((adjrow, adjcol)):
						adjenergy_sum += (grid_old[adjrow][adjcol] - center_energy)
						dirs_used += 1
				if dirs_used > 0:
					adjenergy = adjenergy_sum / float(len(utils.DIRECTIONS))
				# Finally compute the energy of the new grid square
				new_entry = self.params['dispersion_coeff'] * (center_energy + adjenergy)
				#sys.stderr.write('center energy:' + str(center_energy) + '\n')
				#sys.stderr.write('adj energy:' + str(adjenergy) + '\n')
				#sys.stderr.write('disp coeff:' + str(self.params['dispersion_coeff']) + '\n')
				#sys.stderr.write('new entry: ' + str(new_entry) + '\n')
				#sys.stderr.flush()
				grid_new[row][col] = new_entry
		return grid_new

	def express(self, ants, gs, afs):
		return Gene.express(self, ants, gs, afs, self.disperse_once)

def HillStayGene(Gene):
	def __init__(self):
		super(Gene, self).__init__('hillstay', 0, [])
	
	# Pick an ant in hill visibility, flag it as hill master to keep it near
	def express(self, ants, gs, afs):
		#
		for ant in ants.my_ants():

		hillmaster = random.choice(ants.my_ants())
		self.gs.setHillMaster(hillmaster)

def FoodGene(Gene):
	def __init__(self):
		super(DispersionGene, self).__init__('food', 1, ['food_attract'])
		self.params =\
		{\
			'food_attract' : 0.9\
		}
	
	def disperse_once(self, grid_old, grid_new, ants, gs):
		pass
