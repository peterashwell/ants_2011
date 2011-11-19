import sys
import utils
import HillProtector
# records important data which is not a part of the ants.py program
class GameState:
	def __init__(self, ants_instance):
		self.num_rows = ants_instance.rows
		self.num_cols = ants_instance.cols
		self.turn = 0
		# record turn upon which squares were last visible
		self.last_visible = [[-100 for i in xrange(self.num_cols)] for j in xrange(self.num_rows)]
		self.moving = set()
		self.vacated = set()
		sys.stderr.write('assbuck: ' + str(ants_instance.hill_list) + '\n')
		# Assign ants to be hill protectors (see HillProtector.py)
		# record turn upon which squares were last occupied by an opponent
		#self.last_occupied = [[0 for i in num_rows] for j in num_cols]

	# Called in doTurn(), does everything with gamestate
	def updateAll(self, ants_instance):
		if self.turn == 0: # do stuff on first turn
			self.hp = HillProtector.HillProtector(ants_instance)
		self.hp.updateTurn(ants_instance)
		self.moving = set()
		self.vacated = set()
		# update all visible squares
		for r in xrange(self.num_rows):
			for c in xrange(self.num_cols):
				if ants_instance.visible((r,c)):
					self.updateVisible(r,c)
	
	# Tick over to next turn
	def updateTurn(self):
		self.turn += 1

	# Visit a square on current turn
	def updateVisible(self, row, col):
		#sys.stderr.write(str(self.last_visible) + '\n')
		#sys.stderr.write(str(row) + ' ' + str(col) + '\n')
		self.last_visible[row][col] = self.turn

	def updateMoving(self, location):
		self.moving.add(location)
	
	def updateVacated(self, location):
		self.vacated.add(location)
	# Mark a square as having been occupied by enemy this turn
	#def updateEnemy(self, row, col):
	#	self.last_occupied[row][col] = self.turn
	
	def getTurn(self):
		return self.turn
	
	def movingTo(self, location):
		return location in self.moving
	
	def vacatedNow(self, location):
		return location in self.vacated
	# Return turns since visited for a cell
	def turnsSinceVisible(self, row, col):
		return self.turn - self.last_visible[row][col]
