// svd.h
#ifndef SVD_H
#define SVD_H

double **dmatrix(int nrl, int nrh, int ncl, int nch);
double *dvector(int nl, int nh);
void free_dvector(double *v, int nl, int nh);
void svdcmp(double **a, int m, int n, double w[], double **v);
double pythag(double a, double b);
void free_dmatrix(double **m, int nrl, int nrh, int ncl, int nch);

#endif
