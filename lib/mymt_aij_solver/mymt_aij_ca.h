/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __MYMT_AIJ_CA_H__
#define __MYMT_AIJ_CA_H__


#include "mymt_aij_solver.h"


/*** aij_matrix_func.h ***/
void mymt_CA_AIJ_Init( int nf, TMP_AIJ *tmp_aij, int nMargin ) ;
void mymt_CA_AIJ_Set( TMP_AIJ *tmp_aij,
                      int szRow, int *indRow,
                      int szCol, int *indCol,
                      Complex *ae, Complex *fe ) ;
void mymt_CA_AIJ_SetBC( TMP_AIJ *tmp_aij,
                        int nbc, int *indbc,
                        Complex *val ) ;
void mymt_CA_AIJ_Assem( AIJ_MAT *aij_mat, TMP_AIJ *tmp_aij ) ;
void mymt_CA_AIJ_Free( AIJ_MAT *aij_mat ) ;


void mymt_CA_AIJ_SetAe( TMP_AIJ *tmp_aij,
                        int szRow, int *indRow,
                        int szCol, int *indCol,
                        Complex *ae ) ;
void mymt_CA_AIJ_SetFe( Complex *tmp_b_d,
                        int szCol, int *indCol,
                        Complex *fe ) ;
void mymt_CA_AIJ_DeleteZero( TMP_AIJ *tmp_aij ) ;
void mymt_CA_AIJ_Copy_TMP_AIJ( TMP_AIJ *a, TMP_AIJ b ) ;
void mymt_CA_AIJ_Copy_TMP_AIJ_A( TMP_AIJ *a, TMP_AIJ b ) ;
void mymt_CA_AIJ_Copy_TMP_AIJ_B( TMP_AIJ *a, TMP_AIJ b ) ;
void mymt_CA_AIJ_Print_TMP_AIJ( FILE *fp, TMP_AIJ tmp_aij ) ;
void mymt_CA_AIJ_Print_AIJ_MAT( FILE *fp, AIJ_MAT aij_mat ) ;
void mymt_CA_AIJ_MxV( int m, int *ai, int *aij, Complex *vij,
                      Complex *vec, Complex *x ) ;
void mymt_CA_AIJ_MxV_TMP_AIJ( TMP_AIJ tmp_aij, Complex *vec, Complex *x ) ;
Complex mymt_CA_AIJ_getAIJ( int pos_i, int pos_j,
                            int *ai, int *aij, Complex *vij ) ;
void mymt_CA_AIJ_Free_TMP_AIJ( TMP_AIJ *tmp_aij ) ;



/*** aij_pc_func.h ***/
int mymt_CA_AIJ_PC_NONE_make( AIJ_MAT *aij_mat, SolvOpt solvopt,
                              FILE *monitor ) ;
int mymt_CA_AIJ_PC_NONE_set( AIJ_MAT *aij_mat, Complex *Mr, Complex *r,
                             SolvOpt solvopt, FILE *monitor ) ;



/*** aij_solver_func.h ***/
int mymt_CA_AIJ_Solver_Judge( int solver_status ) ;

#endif /* _MYMT__AIJ_CA_H__ */
