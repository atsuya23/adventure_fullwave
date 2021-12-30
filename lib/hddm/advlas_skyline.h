#ifndef __ADVLAS_SKYLINE_H__
#define __ADVLAS_SKYLINE_H__
#include "libhddm.h"

//void advlas_copy_nzindex_to_skyindex(int  *NzIdx_ia, int  *NzIdx_jcn, DMatrix *dmat) ;
//void advlas_copy_nzmat_to_skymat(int *NzIdx_ia, int *NzIdx_jcn, Complex *NzMat, DMatrix *dmat) ;

void advlas_copy_nzindex_to_skyindex(int** taij, DMatrix *dmat) ;
void advlas_copy_nzmat_to_skymat(int *ai, int **taij, Complex **tvij, DMatrix *dmat) ;

void advlas_skyline_ldl_decomposite(DMatrix dmat, double _Complex *work) ;
void advlas_skyline_ldl_solve(DMatrix dmat, double _Complex *solution);

void dmat_init(DMatrix *dmat) ;
void dmat_free(DMatrix *dmat) ;
void skyline_copy(DMatrix*  dmat,  DMatrix  dmat_orig) ;
void skyline_copy_1(DMatrix  dmat_temp,  DMatrix  *dmat ) ;
void change_aij_2_skyline_index(TMP_AIJ tmp_aij, DMatrix *dmat) ;

void temp( DMatrix dmat ) ;
void change_skyline(DMatrix dmat) ;

void aij_setbc_for_rhs( TMP_AIJ *tmp_aij,int nbc, int *indbc, double *val ) ;

#endif
