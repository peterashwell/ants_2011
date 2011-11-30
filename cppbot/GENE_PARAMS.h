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
#endif
