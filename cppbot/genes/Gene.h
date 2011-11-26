#ifndef GENE_H
#define GENE_H

#include <string>
using namespace std;

#include "State.h"

class Gene
{
public:
  Gene(string id, int rows, int cols, float default_value = 0.0f)
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

  ~Gene()
  {
    for(int i = 0; i < rows; ++i)
    {
      delete [] disp_field_curr[i];
      delete [] disp_field_prev[i];
    }
    delete [] disp_field_curr;
    delete [] disp_field_prev;
  }

  virtual void disperse_once(State state) = 0;

  // Switch the dispersion buffers around so we have the prev and curr is to be overwritten
  void swap_disp_buffer()
  {
    float **temp = disp_field_curr;
    disp_field_curr = disp_field_prev;
    disp_field_prev = temp;

    // Reset curr to default_values to avoid seemingly random numbers
    for(int i = 0; i < rows; ++i)
    {
      for(int j = 0; j < cols; ++j)
      {
        disp_field_curr[i][j] = default_value;
      }
    }
  }

  // Wrap the index value passed in to the game board
  int wrap_rows(int row)
  {
    while(row < 0)     row += rows;
    while(row >= rows) row -= rows;
    return row;
  }

  int wrap_cols(int col)
  {
    while(col < 0)     col += cols;
    while(col >= cols) col -= cols;
    return col;
  }
protected:
  string id;
  float **disp_field_curr;
  float **disp_field_prev;
  float default_value;

  int rows, cols; // Need to store these so the destructor knows how much to clean up
};

#endif
