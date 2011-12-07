#ifndef GENE_PARAMS_H
#define GENE_PARAMS_H
/* GENERIC PARAMETERS */
static float ENERGY_THRESHOLD = 1e-10; // threshold for attraction working
/* EXPLORE PARAMTERS */
static float EXPGENE_DISPERSION_ITERATIONS = 20;
static float EXPGENE_UNEX_ATTRACT = 2;
static float EXPGENE_UNEX_DECAY = 1;
static float EXPGENE_DISP_COEFF = 0.95;
static float EXPGENE_UNSEEN_LIMIT = 10;
/* FOOD PARAMETERS */
static float FOODGENE_DISPERSION_ITERATIONS = 30;
static float FOODGENE_FOOD_ATTRACT = 10;
static float FOODGENE_COLLAB_COEFF = 0.05;
static float FOODGENE_DISP_COEFF = 0.9;
/* HILL PARAMETERS */
static float HILLGENE_ENEMY_ATTRACT = -0.9;
static float HILLGENE_HILL_ATTRACT = 1;
static float HILLGENE_DISP_COEFF = 0.95;
static float HILLGENE_DISP_ITERS = 30;
/* POSITIONING PARAMETERS */
static float POSGENE_CLUSTER_DISP_COEFF = 0.95;
static int POSGENE_CLUSTER_DISP_ITERS = 4;
static int POSGENE_CLUSTER_THRESHOLD = 4;
static float POSGENE_CLUSTER_ATTRACTIVENESS = 2;
static int POSGENE_STRAT_DISP_ITERS = 5; // TODO should these be the same
static float POSGENE_STRAT_OVERLAP_RADIUS = 5; // (as this ^)
static float POSGENE_STRAT_COVERAGE_BOUND = 0.5; // Need outnumbering of at least +1 to give force
static float POSGENE_STRAT_DISP_COEFF = 0.95;
// TODO make the above a parameterized function rather than a constant
#endif
