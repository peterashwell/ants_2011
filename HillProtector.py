import sys
import utils
# Class to manage the protection of hills by at least one ant
# from the start of the game to the end
class HillProtector:
	# assign a particular ant to protecting each of our hills
	# until he dies, at which point replacements are found
	def __init__(self, ants_instance):
		# There should be one ant near each of our hills. Assign the closest
		# ant to each hill as its protector
		self.hill_protectors = [] # hill <--> protector tuples
		sys.stderr.write('faggot:' + str(ants_instance.hill_list))
		sys.stderr.write('myhills:' + str(utils.my_hills(ants_instance)))
		for hill in utils.my_hills(ants_instance): # The one in ants.py is broken for some reason
			protector = self.findNewProtector(ants_instance, hill)
			sys.stderr.write('protector:' + str(protector))
			self.hill_protectors.append((hill, protector))
	
	def findNewProtector(self, ants_instance, hill):
		closest = None
		closest_dist = None
		for ant in ants_instance.my_ants():
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

	# return list of hill protectors
	def getProtectors(self):
		return [obj[1] for obj in self.hill_protectors]

	# Express the genome while protecting the hills
	# This could be written in a more general way but it should be the only
	# Time in the game that a subset of the ants has to do specific things
	def protect(self, ants_instance, gs, genome):
		afs = []
		sys.stderr.write('hps: ' + str(self.hill_protectors) + '\n')
		for hill_prot in self.hill_protectors:
			hill, protector = hill_prot
			disp_fields = []
			prot_x, prot_y = protector
			hp_af = utils.AntForce(prot_x, prot_y)
			afs.append(hp_af)
			# Express modified genes
			for gene in genome:
				# cansee returns True for squares that can see the hill
				#sys.stderr.write('fagassballs: ' + str(ants_instance.viewradius2) + '\n')
				canseehill = lambda ant: utils.canSee(ant, hill, ants_instance.viewradius2)
				# Get the dispersion field with the restricted dispersion
				df = gene.express(ants_instance, gs, afs, canseehill)
		# Resolve the forces on the hill protectors
		new_hill_protectors = []
		for hp_af in afs:
			# find its item in the hill protectors
			matching_hill = None
			for hp in self.hill_protectors:
				hill, prot = hp
				if prot == hp_af.getPosition:
					matching_hill = hill
			old_pos = hp_af.getPosition()
			new_pos = hp_af.applyForces(ants_instance, gs) # issue order to the ant (utils.py)
			
			if new_pos is None: # no move issued
				new_hill_protectors.append((matching_hill, old_pos))
			else: # update the hill protector list
				new_hill_protectors.append((matching_hill, new_pos))

	# Update the hill protectors at new turn (in case they died, or hill died)
	def updateTurn(self, ants_instance):
		# check if one died
		find_new = []
		for hill_prot in self.hill_protectors:
			hill, protector = hill_prot
			sys.stderr.write('hill protector: ' + str(protector) + '\n')
			found = False
			for ant in ants_instance.my_ants():
				if ant == protector:
					found = True
			if not found:
				find_new.append(protector)
		new_hps = []
		for hill_prot in self.hill_protectors:
			hill, protector = hill_prot
			if protector in find_new:
				new_hps.append((hill, self.findNewProtector(ants_instance, hill)))
			else:
				new_hps.append(hill_prot)
		self.hill_protectors = new_hps
	
