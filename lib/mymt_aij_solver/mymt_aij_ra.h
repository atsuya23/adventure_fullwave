/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __MYMT_AIJ_RA_H__
#define __MYMT_AIJ_RA_H__


#include "mymt_aij_solver.h"


/*** aij_matrix_func.h ***/
void mymt_RA_AIJ_Init( int nf, TMP_AIJ *tmp_aij, int nMargin ) ;
void mymt_RA_AIJ_Set( TMP_AIJ *tmp_aij,
                      int szRow, int *indRow,
                      int szCol, int *indCol,
                      double *ae, double *fe ) ;
void mymt_RA_AIJ_SetBC( TMP_AIJ *tmp_aij,
                        int nbc, int *indbc,
                        double *val ) ;
void mymt_RA_AIJ_Assem( AIJ_MAT *aij_mat, TMP_AIJ *tmp_aij ) ;
void mymt_RA_AIJ_Free( AIJ_MAT *aij_mat ) ;


void mymt_RA_AIJ_SetAe( TMP_AIJ *tmp_aij,
                        int szRow, int *indRow,
                        int szCol, int *indCol,
                        double *ae ) ;
void mymt_RA_AIJ_SetFe( double *tmp_b_d,
                        int szCol, int *indCol,
                        double *fe ) ;
void mymt_RA_AIJ_DeleteZero( TMP_AIJ *tmp_aij ) ;
void mymt_RA_AIJ_Copy_TMP_AIJ( TMP_AIJ *a, TMP_AIJ b ) ;
void mymt_RA_AIJ_Copy_TMP_AIJ_A( TMP_AIJ *a, TMP_AIJ b ) ;
void mymt_RA_AIJ_Copy_TMP_AIJ_B( TMP_AIJ *a, TMP_AIJ b ) ;
void mymt_RA_AIJ_Print_TMP_AIJ( FILE *fp, TMP_AIJ tmp_aij ) ;
void mymt_RA_AIJ_Print_AIJ_MAT( FILE *fp, AIJ_MAT aij_mat ) ;
void mymt_RA_AIJ_MxV( int m, int *ai, int *aij, double *vij,
                      double *vec, double *x ) ;
void mymt_RA_AIJ_MxV_TMP_AIJ( TMP_AIJ tmp_aij, double *vec, double *x ) ;
double mymt_RA_AIJ_getAIJ( int pos_i, int pos_j,
                           int *ai, int *aij, double *vij ) ;
void mymt_RA_AIJ_Free_TMP_AIJ( TMP_AIJ *tmp_aij ) ;



/*** aij_pc_func.h ***/
int mymt_RA_AIJ_PC_NONE_make( AIJ_MAT *aij_mat, SolvOpt solvopt,
                              FILE *monitor ) ;
int mymt_RA_AIJ_PC_NONE_set( AIJ_MAT *aij_mat, double *Mr, double *r,
                             SolvOpt solvopt, FILE *monitor ) ;



/*** aij_solver_func.h ***/
int mymt_RA_AIJ_Solver_Judge( int solver_status ) ;

#endif /* __MYMT_AIJ_RA_H__ */
