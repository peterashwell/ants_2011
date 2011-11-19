#!/usr/bin/env python
import sys
from GameState import GameState
from ants import *
import utils
import Genes
# define a class with a do_turn method
# the Ants.run method will parse and update bot input
# it will also run the do_turn method for us
class GeneticBot:
	def __init__(self):
		sys.stderr = open('errors.txt', 'w')
	
	# This method is called in ants.py (can be removed)
	def do_setup(self, ants):
		self.gs = GameState(ants.rows, ants.cols) # Keep track of important game state stuff
		# GENOME IS DEFINED HERE
		self.genome = [Genes.ExploreGene()]

	# Called once a turn in ants.py, orders called from here
	def do_turn(self, ants):
		sys.stderr.write('rem: ' + str(ants.time_remaining()) + '\n')
		sys.stderr.flush()
		self.gs.updateAll(ants)
		sys.stderr.write('rem (building af): ' + str(ants.time_remaining()) + '\n')
		sys.stderr.flush()
		afs = [] # see Utils.py -> AntForce
		# Build default ant force holders based on current ants
		# TODO replace this with just dispersion fields
		for ant_loc in ants.my_ants():
			row, col = ant_loc
			afs.append(utils.AntForce(row, col))
		sys.stderr.write('rem (expressing genome...): ' + str(ants.time_remaining()) + '\n')
		sys.stderr.flush()
		
		# Get the hill protectors to do their business
		# This is a necessary evil as it would be far messier to screw around with the Genes
		self.gs.hp.protect() # does dispersion while staying near the hill

		# Express the genes (not related to hill protectors)
		for gene in self.genome:
			gene.express(ants, self.gs, afs)
		sys.stderr.write('rem (applying forces): ' + str(ants.time_remaining()) + '\n')
		sys.stderr.flush()
		#Resolve the forces to produce actions
		for af in afs:
			af.applyForces(ants, self.gs)
		sys.stderr.write('rem: ' + str(ants.time_remaining()) + '\n')
		sys.stderr.flush()
				
if __name__ == '__main__':
	# psyco will speed up python a little, but is not needed
	try:
		import psyco
		psyco.full()
	except ImportError:
		pass	
	try:
		# if run is passed a class with a do_turn method, it will do the work
		# this is not needed, in which case you will need to write your own
		# parsing function and your own game state class
		Ants.run(GeneticBot())
	except KeyboardInterrupt:
		print('ctrl-c, leaving ...')
