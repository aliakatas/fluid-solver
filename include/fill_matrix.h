#pragma once

void fill_inner_nodes(double* coeff_mat, double* rhs, int rows, int cols, double dx, double dy, double* source);

void fill_dirichlet_nodes(double* coeff_mat, double* rhs, int* bcrows, int* bccols, double* bc_val, int N, int rows, int cols);

void fill_dirichlet_nodes(double* coeff_mat, double* rhs, int* bcrows, int* bccols, double bc_val, int N, int rows, int cols);
