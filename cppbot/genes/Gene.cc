#include <sstream>
#include <fstream>
#include "Gene.h"
Gene::Gene(string id, int rows, int cols, float default_value)
{
	this->id = id;
	this->rows = rows;
	this->cols = cols;
	this->default_value = default_value;
	disp_field_curr = new float*[rows];
	disp_field_prev = new float*[rows];

	for(int i = 0; i < rows; ++i)
	{
		disp_field_curr[i] = new float[cols];
		disp_field_prev[i] = new float[cols];
		for(int j = 0; j < cols; ++j)
		{
			disp_field_curr[i][j] = default_value;
			disp_field_prev[i][j] = default_value;
		}
	}
}

Gene::~Gene()
{
	for(int i = 0; i < rows; ++i)
	{
		delete [] disp_field_curr[i];
		delete [] disp_field_prev[i];
	}
	delete [] disp_field_curr;
	delete [] disp_field_prev;
}

// Switch the dispersion buffers around so we have the prev and curr is to be overwritten
void Gene::swap_disp_buffer()
{
	float **temp = disp_field_curr;
	disp_field_curr = disp_field_prev;
	disp_field_prev = temp;

	// Reset curr to default_values to avoid seemingly random numbers
	/* depricated - every value is overwritten and has no impact on future results
	there is no need to overwrite them here as well
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < cols; ++j)
		{
			disp_field_curr[i][j] = default_value;
		}
	}
	*/
}

void Gene::dump_current_df(int turn) {
	ofstream out;
	stringstream fname;
	fname << "dflogs/" << id << "_" << turn << ".df";		
	out.open(fname.str().c_str());
	for (int r = 0; r < rows; ++r) {
		out << disp_field_curr[r][0];
		for (int c = 1; c < cols; ++c) {
			out << "," << disp_field_curr[r][c];
		}	
		out << endl;
	}
	out.close();
}
