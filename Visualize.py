import pygame, sys, os, copy, math
from pygame.locals import *

# Constants
_BLACK = [  0,  0,  0]
_WHITE = [255,255,255]
_BLUE  = [0  ,  0,255]
_GREEN = [0  ,255,  0]
_RED   = [255,  0,  0]
# Threshold for dispersion values considerednon-zero
_THRESHOLD = 1e-10
_CELL_WIDTH = 8 # Cell width to draw, in pixels

class DispersionGrid: 
  def __init__(self, fname, iteration, gene_id):
    self.gene_id = gene_id
    self.iteration = iteration

    grid_file = open(fname)
    lines = grid_file.readlines()

    # Read grid and get dimensions
    self.rows = len(lines)
    self.cols = len(lines[0].split(","))
    #print "self.rows: ", self.rows, "self.cols:", self.cols
    self.grid = []
    self.max_disp = None
    self.min_disp = None
    for r in xrange(self.rows):
      #print r, lines[r]
      if lines[r][0] == '#':
        continue
      #for c, obj in enumerate(lines[r].split(',')):
       # print c, obj
      line_data = [float(obj) for obj in lines[r].split(',')]
      self.grid.append(line_data)
      # Check min/max stuff
      line_max = max(line_data)
      line_min = min(line_data)
      if self.max_disp is None:
        self.max_disp = line_max
      elif self.max_disp < line_max:
        self.max_disp = line_max
      if self.min_disp is None:
        self.min_disp = line_min
      elif self.min_disp > line_min:
        self.min_disp = line_min
 
  def __add__(self, other_dg):
    new_dg = copy.copy(other_dg)
    new_dg.grid = [[new_dg.grid[r][c] + self.grid[r][c] for c in xrange(self.cols)] for r in xrange(self.rows)]
    return new_dg

  # Dump this dispersion grid to screen
  def dump(self, screen, cell_width, max_scaled_energy, scale_base=2):
    for r in xrange(self.rows):
      for c in xrange(self.cols):
        # Distribute evenly according to maximum
        value = self.grid[r][c]
        # See top of file for constants
        intensity = 1 # Defaults for when
        color = _BLUE # inside threshold
        if value > _THRESHOLD:
          value += 1 # keep in range (-inf, -1]
          color = _GREEN
          intensity = math.log(math.fabs(value), scale_base) / max_scaled_energy
        elif value < -1 * _THRESHOLD: 
          value -= 1 # keep in range [1, inf)
          color = _RED
          intensity = math.log(math.fabs(value), scale_base) / max_scaled_energy
        # SWAP ROWS AND COLS TO GET CORRECT ORIENTATION
        xpos = c * cell_width
        ypos = r * cell_width
        #print "color:", [obj * intensity for obj in color]
        pygame.draw.rect(screen, [obj * intensity for obj in color], (xpos, ypos, cell_width, cell_width))
        #pygame.draw.rect(screen, [255, 255, 255], (50, 50, 100, 100))
    pygame.display.update()

class Visualize:
  def __init__(self):
    pygame.init()
    # Mouse data
    self.grid_square_clicked = None
    # Grid, gene and visible parameters
    self.cell_width = 8
    self.genes = []
    self.genenum_genename = {} # maps gene names to gene numbers in 'visible' array
    self.iter_gene_dg = {} # maps iteration to gene to DispersionGrid
    self.rows = None
    self.cols = None
    self.max_energy = None # Maximum energy, positive or negative (sets scale)
    self.max_iters = 0
    # Loading data
    self.loadData()
    self.base = 1.3 # base for logarithmic scale
    self.max_energy += 1 # keep in range [1, inf)
    self.max_scaled_energy = math.log(self.max_energy, self.base)
    self.log_scale_max = math.pow(int(math.ceil(math.log(self.max_energy, self.base))), self.base)
    #print "mse: {0} lsm: {1}".format(self.max_scaled_energy, self.log_scale_max)
    self.genes_visible = [True for g in xrange(len(self.genes))]
    # SWAP ROWS AND COLS TO GET CORRECT ORIENTATION
    self.screen = pygame.display.set_mode([self.cols * self.cell_width, self.rows * self.cell_width])
  
  def run(self):
    self.clock = pygame.time.Clock()
    self.selected_iteration = 1
    self.render() # Draw first grid
    pygame.display.update()
    while True:
      self.checkEvents()
      self.clock.tick(10) 
  
  def loadData(self):
    dir = sys.argv[1]
    fname = "No file loaded yet"
    try:
      for fname in os.listdir(dir):
        print "loading: {0}".format(fname)
        if fname.split('.')[-1] != "df":
          #print "continuing"
          continue
        gene_id, iter = fname.split('.')[0].split('_')
        iter = int(iter)
        self.max_iters = max(iter, self.max_iters)
        # Add unseen genes to set
        if gene_id not in self.genes:
          print "mapped gene {0} to key {1}".format(gene_id, len(self.genes))
          self.genenum_genename[len(self.genes)] = gene_id
          self.genes.append(gene_id)

        grid_path = "{0}/{1}".format(dir, fname)
        # Load the file with necessary info
        new_grid = DispersionGrid(grid_path, iter, gene_id)
        if self.max_energy is None:
          self.max_energy = max(math.fabs(new_grid.min_disp), new_grid.max_disp)
        else:
          self.max_energy = max(math.fabs(new_grid.min_disp), new_grid.max_disp, self.max_energy)
        if iter not in self.iter_gene_dg.keys():
          new_dict = {gene_id : new_grid}
          self.iter_gene_dg[iter] = new_dict
        else:
          self.iter_gene_dg[iter][gene_id] = new_grid
        # Get row data if needed and check it makes sense
        if self.rows is None or self.cols is None:
          self.rows = new_grid.rows
          self.cols = new_grid.cols
        if self.rows != new_grid.rows or self.cols != new_grid.cols:
          raise Error("Inconsistent rows and cols in file {0}".format(fname))
    except IOError as e:
      print "Error loading file {0}: {1}".format(fname, str(e))
      exit(1)
  
  def render(self):
    grids_to_draw = []
    #print self.iter_gene_dg
    #print self.genenum_genename
    for gene_num in xrange(len(self.genes_visible)):
      if not self.genes_visible[gene_num]:
        continue
      grids_to_draw.append(self.iter_gene_dg[self.selected_iteration][self.genenum_genename[gene_num]])
    if len(grids_to_draw) == 0:
      self.screen.fill((0,0,0))
      pygame.display.update()
    else:
     sum_grid = reduce( self.combine_grids, grids_to_draw)
     sum_grid.dump(self.screen, self.cell_width, self.log_scale_max, self.base)

  def checkEvents(self):
    mouse_pos = pygame.mouse.get_pos()
    for event in pygame.event.get():
      if event.type == QUIT:
        pygame.quit()
        sys.exit()
      elif event.type == KEYDOWN:
        if event.key == K_ESCAPE:
          pygame.event.post(pygame.event.Event(QUIT))
        elif event.key in xrange(K_0, K_0 + len(self.genes_visible)):
          self.toggle_gene_visible(event.key - K_0) 
        elif event.key == K_d:
          self.select_iteration(1)
        elif event.key == K_a:
          self.select_iteration(-1)      
        self.render()
      elif event.type == pygame.MOUSEBUTTONDOWN:
        self.clicked = mouse_pos
    
  def toggle_gene_visible(self, index):
    print "len gv: {0}".format(len(self.genes_visible))
    print "index: {0}".format(index)
    if(index >= 0 and index < len(self.genes_visible)):
      self.genes_visible[index] = not self.genes_visible[index]
      print("{0} visible: {1}".format(self.genes[index], self.genes_visible[index]))

  def select_iteration(self, change):
    # Clamp selected iteration
    self.selected_iteration += change
    if self.selected_iteration < 1:
      self.selected_iteration = 1
    elif self.selected_iteration > self.max_iters:
      self.selected_iteration = self.max_iters
    print("Iteration change: {0}".format(self.selected_iteration))

  def combine_grids(self, grid_a, grid_b):
    return grid_a + grid_b



v = Visualize()
v.run()

#while done == False:
#  # Get mouse info
#  mouse_pos = pygame.mouse.get_pos()
#  for event in pygame.event.get():
#    if event.type == QUIT:
#      pygame.quit()
#      sys.exit()
#    elif event.type == KEYDOWN:
#      if event.key == K_ESCAPE:
#        pygame.event.post(pygame.event.Event(QUIT))
#      elif event.key == K_0:
#        v.toggle_gene_visible(0)
#      elif event.key == K_1:
#        v.toggle_gene_visible(1)
#      elif event.key == K_d:
#        v.select_iteration(1)
#      elif event.key == K_a:
#        v.select_iteration(-1)      
#    elif event.type == pygame.MOUSEBUTTONDOWN:
#     v.clicked = mouse_pos
#    v.render()
