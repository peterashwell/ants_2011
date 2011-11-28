import pygame, sys, os
from pygame.locals import *

class Visualize:
  def __init__(self):
    pygame.init()

    self.black = [  0,  0,  0]
    self.white = [255,255,255]
    self.blue  = [0  ,  0,255]
    self.green = [0  ,255,  0]
    self.red   = [255,  0,  0]
    self.gene_colours = [self.red, self.blue, self.green]
    self.map_rows = 200
    self.map_cols = 200
    self.grid_size = 8
    self.clock = pygame.time.Clock()
    self.dispersion_maps = {}
    self.dispersion_maxs = {}
    self.dispersion_mins = {}
    self.genes = ["ExploreGene"]
    self.genes_visible = [True for gene in xrange(len(self.genes))]
    self.map_rows = 0
    self.map_cols = 0
    self.load_files()
    print "Rows: {0}, Cols: {1}".format(self.map_rows, self.map_cols)
    print "Genes: {0}".format(", ".join(self.genes))
    self.window_size = [self.grid_size * self.map_cols, self.grid_size * self.map_rows]
    self.screen = pygame.display.set_mode(self.window_size)
    self.selected_iteration = 0
    if self.iterations <= 0:
      pygame.quit()
      return

  def load_files(self):
    # Check how many iteration logs we have
    self.iterations = 1 # If it's -1, there were no iterations in the folder
    while True:
      try:
        f = open("{0}/cppbot/dflogs/{1}_{2}.df".format(os.getcwd(), self.genes[0], self.iterations))
        f.close()
        self.iterations += 1
      except IOError as e:
        print "Iterations: {0}".format(self.iterations)
        break

    
    self.dispersion_maps = {}
    self.dispersion_maxs = {}
    self.dispersion_mins = {}
    
    for gene in self.genes:
      print(gene)
      self.dispersion_maps[gene] = [[] for iter in xrange(self.iterations)]
      self.dispersion_maxs[gene] = [[] for iter in xrange(self.iterations)]
      self.dispersion_mins[gene] = [[] for iter in xrange(self.iterations)]
      for iteration in xrange(0, self.iterations - 1):
        f = open("{0}/cppbot/dflogs/{1}_{2}.df".format(os.getcwd(), gene, iteration + 1))
        lines = f.readlines()
        
        # Store Map Size information
        self.map_rows = len(lines)
        self.map_cols = len(lines[0].split(","))
        # Create dispersion matrix for this gene
        self.dispersion_maps[gene][iteration] = [[0 for i in xrange(self.map_cols)] for j in xrange(self.map_rows)]
        self.dispersion_maxs[gene][iteration] = 0

        # Read in values
        for row, line in enumerate(lines):
          vals = line.split(",")
          for col, val in enumerate(vals):
                  self.dispersion_maps[gene][iteration][row][col] = float(val)
                  if float(val) > self.dispersion_maxs[gene][iteration]:
                    self.dispersion_maxs[gene][iteration] = float(val)
                  if float(val) < self.dispersion_mins[gene][iteration]:
                    self.dispersion_mins[gene][iteration] = float(val)
          #print(self.dispersion_maps[gene][iteration][row])
        print("Max = {0}".format(self.dispersion_maxs[gene][iteration]))

  def render(self):     
    self.screen.fill(self.black)

    for gene_index, gene in enumerate(self.genes):
      if self.genes_visible[gene_index] == True:
        surface = pygame.Surface(self.window_size, flags=pygame.SRCALPHA)
        for row in xrange(self.map_rows):
          for col in xrange(self.map_cols):
            opacity = self.dispersion_maps[gene][self.selected_iteration][row][col] - self.dispersion_mins[gene][self.selected_iteration]
            dilution = (self.dispersion_maxs[gene][self.selected_iteration] - self.dispersion_mins[gene][self.selected_iteration])

            if dilution > 1e-10:  
              opacity = opacity / (self.dispersion_maxs[gene][self.selected_iteration] - self.dispersion_mins[gene][self.selected_iteration])
            
            opacity = 128 * opacity
            #print("val {0}".format(self.dispersion_maps[gene][self.selected_iteration][row][col]))
            #print("min {0} max {1}".format(self.dispersion_mins[gene][self.selected_iteration], self.dispersion_maxs[gene][self.selected_iteration]))
            #print(opacity)
            pygame.draw.rect(surface, self.gene_colours[gene_index] + [opacity], (col * self.grid_size, row * self.grid_size, self.grid_size, self.grid_size))
        self.screen.blit(surface, (0, 0))
        
    pygame.display.update()
    self.clock.tick(10)

  def toggle_gene_visible(self, index):
    if(index >= 0 and index < len(self.genes_visible)):
      self.genes_visible[index] = not self.genes_visible[index]
      print("{0} visible: {1}".format(self.genes[index], self.genes_visible[index]))

  def select_iteration(self, change):
    # Clamp selected iteration
    self.selected_iteration += change
    if self.selected_iteration < 0:
      self.selected_iteration = 0
    elif self.selected_iteration >= self.iterations:
      self.selected_iteration = self.iterations - 1
    print("Iteration change: {0}".format(self.selected_iteration))
    



done = False
v = Visualize()

while done == False:
  for event in pygame.event.get():
    if event.type == QUIT:
      pygame.quit()
      sys.exit()
    elif event.type == KEYDOWN:
      if event.key == K_ESCAPE:
        pygame.event.post(pygame.event.Event(QUIT))
      elif event.key == K_0:
        v.toggle_gene_visible(0)
      elif event.key == K_1:
        v.toggle_gene_visible(1)
      elif event.key == K_d:
        v.select_iteration(1)
      elif event.key == K_a:
        v.select_iteration(-1)      

    v.render()
        
                             
                
    
      
    
