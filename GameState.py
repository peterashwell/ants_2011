import sys
import utils
# records important data which is not a part of the ants.py program
class GameState:
	def __init__(self, ants):
		self.num_rows = ants.rows
		self.num_cols = ants.cols
		self.turn = 0
		# record turn upon which squares were last visible
		self.last_visible = [[-100 for i in xrange(self.num_cols)] for j in xrange(self.num_rows)]
		self.moving = set()
		self.vacated = set()
		self.hp = utils.HillProtector(ants) # assigns ants to protect hills
		# record turn upon which squares were last occupied by an opponent
		#self.last_occupied = [[0 for i in num_rows] for j in num_cols]

	# Called in doTurn(), does everything with gamestate
	def updateAll(self, ants):
		self.hp.updateTurn()
		self.moving = set()
		self.vacated = set()
		# update all visible squares
		for r in xrange(self.num_rows):
			for c in xrange(self.num_cols):
				if ants.visible((r,c)):
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


