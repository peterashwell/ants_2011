int main() {
int ROWS = 150;
int COLS = 150;

float grid[ROWS][COLS];
int blah;

int iter = 0;
int r = 0;
int c = 0;
for (iter = 0; iter < 20; iter++) {
	for (r = 0; r < ROWS; r++) {
		for (c = 0; c < COLS; c++) {
			float center_energy = grid[r][c];
			float nigadj = 0;
			if (r > 0 && r < (ROWS - 1) && c > 0 && c < (COLS - 1)) {
			nigadj += grid[r - 1][c] - center_energy;
			nigadj += grid[r + 1][c] - center_energy;
			nigadj += grid[r][c + 1] - center_energy;
			nigadj += grid[r][c - 1] - center_energy;
			nigadj /= 4.0;
			float new_val = 0.5 * (center_energy + nigadj);
			grid[r][c] = new_val;
			}
		}
	}
}

return 0;
}
