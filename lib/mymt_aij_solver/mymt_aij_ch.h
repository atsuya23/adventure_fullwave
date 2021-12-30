/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __MYMT_AIJ_CH_H__
#define __MYMT_AIJ_CH_H__


#include "mymt_aij_solver.h"


/*** aij_matrix_func.h ***/
void mymt_CH_AIJ_Init( int nf, TMP_AIJ *tmp_aij/*, int nMargin*/ ) ;
void mymt_CH_AIJ_Set( TMP_AIJ *tmp_aij,
                      int szRow, int *indRow,
                      int szCol, int *indCol,
                      Complex *ae, Complex *fe ) ;
void mymt_CH_AIJ_SetBC( TMP_AIJ *tmp_aij,
                        int nbc, int *indbc,
                        Complex *val ) ;
void mymt_CH_AIJ_Assem( AIJ_MAT *aij_mat, TMP_AIJ *tmp_aij ) ;
void mymt_CH_AIJ_Free( AIJ_MAT *aij_mat ) ;


void mymt_CH_AIJ_SetAe( TMP_AIJ *tmp_aij,
                        int szRow, int *indRow,
                        int szCol, int *indCol,
                        Complex *ae ) ;
void mymt_CH_AIJ_SetFe( Complex *tmp_f_d,
                        int szCol, int *indCol,
                        Complex *fe ) ;
void mymt_CH_AIJ_DeleteZero( TMP_AIJ *tmp_aij ) ;
void mymt_CH_AIJ_Copy_TMP_AIJ( TMP_AIJ *a, TMP_AIJ b ) ;
void mymt_CH_AIJ_Copy_TMP_AIJ_A( TMP_AIJ *a, TMP_AIJ b ) ;
void mymt_CH_AIJ_Copy_TMP_AIJ_B( TMP_AIJ *a, TMP_AIJ b ) ;
void mymt_CH_AIJ_Print_TMP_AIJ( FILE *fp, TMP_AIJ tmp_aij ) ;
void mymt_CH_AIJ_Print_AIJ_MAT( FILE *fp, AIJ_MAT aij_mat ) ;
void mymt_CH_AIJ_MxV( int m, int *ai, int *aij, Complex *vij,
                      Complex *vec, Complex *x ) ;
Complex mymt_CH_AIJ_getAIJ( int pos_i, int pos_j,
                            int *ai, int *aij, Complex *vij ) ;



/*** aij_pc_func.h ***/
int mymt_CH_AIJ_PC_NONE_make( AIJ_MAT *aij_mat, SolvOpt solvopt,
                              FILE *monitor ) ;
int mymt_CH_AIJ_PC_NONE_set( AIJ_MAT *aij_mat, Complex *Mr, Complex *r,
                             SolvOpt solvopt, FILE *monitor ) ;
int mymt_CH_AIJ_PC_ICC_make( AIJ_MAT *aij_mat, SolvOpt solvopt,
                             FILE *monitor ) ;
int mymt_CH_AIJ_PC_ICC_set( AIJ_MAT *aij_mat, Complex *Mr, Complex *r,
                            SolvOpt solvopt, FILE *monitor ) ;
int mymt_CH_AIJ_PC_MRIC0_make( AIJ_MAT *aij_mat, SolvOpt solvopt,
                               FILE *monitor ) ;
int mymt_CH_AIJ_PC_MRIC0_set( AIJ_MAT *aij_mat, Complex *Mr, Complex *r,
                              SolvOpt solvopt, FILE *monitor ) ;



/*** aij_solver_func.h ***/
int mymt_CH_AIJ_Solver_Judge( int solver_status ) ;
/** CG method **/
int mymt_CH_AIJ_Solver_CG( AIJ_MAT *aij_mat, SolvOpt solvopt,
                           FILE *monitor ) ;
int mymt_CH_AIJ_Solver_CG_nmpcm( AIJ_MAT *aij_mat, SolvOpt solvopt,
                                 FILE *monitor ) ;
int mymt_CH_AIJ_Solver_CG_func( AIJ_MAT *aij_mat, SolvOpt solvopt,
                                int (*setPC)( AIJ_MAT *aij_mat,
                                              Complex *Mr, Complex *r,
                                              SolvOpt solvopt,
                                              FILE *monitor ),
                                FILE *monitor ) ;
/** COCG method **/
int mymt_CH_AIJ_Solver_COCG( AIJ_MAT *aij_mat, SolvOpt solvopt,
                             FILE *monitor ) ;
int mymt_CH_AIJ_Solver_COCG_nmpcm( AIJ_MAT *aij_mat, SolvOpt solvopt,
                                   FILE *monitor ) ;
int mymt_CH_AIJ_Solver_COCG_func( AIJ_MAT *aij_mat, SolvOpt solvopt,
                                  int (*setPC)( AIJ_MAT *aij_mat,
                                                Complex *Mr, Complex *r,
                                                SolvOpt solvopt,
                                                FILE *monitor ),
                                  FILE *monitor ) ;

#endif /* __MYMT_AIJ_CH_H__ */
