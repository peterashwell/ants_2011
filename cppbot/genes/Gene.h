#ifndef GENE_H
#define GENE_H

#include <string>
using namespace std;

#include "../utils/State.h"
#include "../utils/AntManager.h"

class Gene
{
	public:
		virtual void express(State& s, LocalData& ld, AntManager& am) = 0;
		Gene(string id, int rows, int cols, float default_value = 0.0f);
		~Gene();
		virtual void disperse_once(State& state, LocalData& local_data) = 0;
		// Switch the dispersion buffers around so we have the prev and curr is to be overwritten
		void swap_disp_buffer();

	protected:
		string id;
		float **disp_field_curr;
		float **disp_field_prev;
		float default_value;

		int rows, cols; // Need to store these so the destructor knows how much to clean up
};

#endif
