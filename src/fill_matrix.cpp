#include "fill_matrix.h"

void fill_inner_nodes(double* coeff_mat, double* rhs, int rows, int cols, double dx, double dy, double* source) {
	int coef_mat_size = rows * cols;
	int idx = 0;
	int kidx = 0;
	double dx2 = dx * dx;
	double dy2 = dy * dy;

	for (auto row = 1; row < rows - 1; ++row) {
		for (auto col = 1; col < cols - 1; ++col) {
			idx = row * cols + col;
			kidx = idx * coef_mat_size + idx;
			coeff_mat[kidx] = -(2. / dx2 + 2. / dy2); // diagonal
			
			coeff_mat[kidx - 1] = 1. / dx2; // Western node link
			coeff_mat[kidx + 1] = 1. / dx2; // Eastern node link
			coeff_mat[kidx - cols] = 1. / dy2; // Southern node link
			coeff_mat[kidx + cols] = 1. / dy2; // Northern node link
			if (source)
				rhs[idx] = source[idx];
		}
	}
}

void fill_dirichlet_nodes(double* coeff_mat, double* rhs, int* bcrows, int* bccols, double* bc_val, int N, int rows, int cols) {
	int coef_mat_size = rows * cols;
	int idx = 0;
	int kidx = 0;

	for (auto ibc = 0; ibc < N; ++ibc) {
		idx = bcrows[ibc] * cols + bccols[ibc];
		kidx = idx * coef_mat_size + idx;
		coeff_mat[kidx] = 1.; // diagonal
		rhs[idx] = bc_val[ibc];
	}
}

void fill_dirichlet_nodes(double* coeff_mat, double* rhs, int* bcrows, int* bccols, double bc_val, int N, int rows, int cols) {
	int coef_mat_size = rows * cols;
	int idx = 0;
	int kidx = 0;

	for (auto ibc = 0; ibc < N; ++ibc) {
		idx = bcrows[ibc] * cols + bccols[ibc];
		kidx = idx * coef_mat_size + idx;
		coeff_mat[kidx] = 1.; // diagonal
		rhs[idx] = bc_val;
	}
}

