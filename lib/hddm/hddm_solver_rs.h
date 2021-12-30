/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __HDDM_SOLVER_RS_H__
#define __HDDM_SOLVER_RS_H__


#include "libhddm.h"


/*** ***/
void hddm_solver_RS( PartMesh *pm, OPTIONS options,
                     void (*set_hddm_func)( HDDM_FUNC *hddm_func,
                                            OPTIONS options ) ) ;
void hddm_solver_RS_Parent( PartMesh *pm, OPTIONS options,
                            void (*set_hddm_func)( HDDM_FUNC *hddm_func,
                                                   OPTIONS options ) ) ;
void hddm_solver_RS_Parent_loop_1st( PartMesh *pm,
                                     OPTIONS options,
                                     HDDM_Opt hddm_opt,
                                     SolvOpt solvopt,
                                     HDDM_VEC *hddm_vec,
                                     HDDM_FUNC hddm_func,
                                     FILE *fp_log ) ;
void hddm_solver_RS_Parent_loop_restart( PartMesh *pm,
                                         OPTIONS options,
                                         HDDM_Opt hddm_opt,
                                         SolvOpt solvopt,
                                         HDDM_VEC *hddm_vec,
                                         HDDM_FUNC hddm_func,
                                         FILE *fp_log ) ;
void hddm_solver_RS_Parent_loop( PartMesh *pm,
                                 OPTIONS options,
                                 HDDM_Opt hddm_opt,
                                 SolvOpt solvopt,
                                 HDDM_VEC *hddm_vec,
                                 HDDM_FUNC hddm_func,
                                 FILE *fp_log ) ;
void hddm_solver_RS_Parent_loop_last( PartMesh *pm,
                                      OPTIONS options,
                                      HDDM_Opt hddm_opt,
                                      SolvOpt solvopt,
                                      HDDM_VEC *hddm_vec,
                                      HDDM_FUNC hddm_func,
                                      FILE *fp_log ) ;

#ifdef __ADVFW_HIERARCHICAL_MODE__
  void hddm_solver_RS_Child( OPTIONS options, MTRL mtrl,
                             void (*set_hddm_func)( HDDM_FUNC *hddm_func,
                                                    OPTIONS options ) ) ;
  void hddm_solver_RS_Child_loop_1st( DomMesh **dm,
                                      MTRL mtrl,
                                      OPTIONS options,
                                      HDDM_Opt hddm_opt,
                                      SolvOpt solvopt,
                                      HDDM_FUNC hddm_func,
                                      FILE* fp_log ) ;
  void hddm_solver_RS_Child_loop_restart( DomMesh **dm,
                                          MTRL mtrl,
                                          OPTIONS options,
                                          HDDM_Opt hddm_opt,
                                          SolvOpt solvopt,
                                          HDDM_FUNC hddm_func,
                                          FILE* fp_log ) ;
  void hddm_solver_RS_Child_loop( DomMesh **dm,
                                  MTRL mtrl,
                                  OPTIONS options,
                                  HDDM_Opt hddm_opt,
                                  SolvOpt solvopt,
                                  HDDM_FUNC hddm_func,
                                  FILE* fp_log ) ;
  void hddm_solver_RS_Child_loop_last( DomMesh **dm,
                                       MTRL mtrl,
                                       OPTIONS options,
                                       HDDM_Opt hddm_opt,
                                       SolvOpt solvopt,
                                       HDDM_FUNC hddm_func,
                                       FILE* fp_log ) ;
  int hddm_solver_RS_change_parent_h( int *nodom, int current ) ;
#endif


void hddm_solver_RS_make_HDDM_VEC( HDDM_VEC *hddm_vec, HDDM_Opt hddm_opt,
                                   int infree, int outfree, int tfree,
                                   int total_free ) ;
void hddm_solver_RS_solve_dom_1st( TMP_AIJ *tmp_aij_orig,
                                   int nbc, int *ibc, double *vbc,
                                   SolvOpt solvopt,
                                   int (*dom_solver)( AIJ_MAT *aij_mat,
                                                      SolvOpt solvopt,
                                                      FILE *monitor ),
                                   HDDM_DOM_VEC *hddm_dom_vec,
                                   int (*makePC)( TMP_AIJ tmp_aij,
                                                  HDDM_DOM_VEC *hddm_dom_vec ),
                                   FILE *monitor ) ;
void hddm_solver_RS_solve_dom( TMP_AIJ *tmp_aij_orig,
                               int nbc, int *ibc, double *vbc,
                               SolvOpt solvopt,
                               int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                        SolvOpt solvopt,
                                                        FILE *monitor ),
                               HDDM_DOM_VEC *hddm_dom_vec, FILE *monitor ) ;
void hddm_solver_RS_solve_dom_last( TMP_AIJ *tmp_aij_orig,
                                    int nbc, int *ibc, double *vbc,
                                    SolvOpt solvopt,
                                    int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                             SolvOpt solvopt,
                                                             FILE *monitor ),
                                    HDDM_DOM_VEC *hddm_dom_vec,
                                    FILE *monitor ) ;




/*** PC ***/
int hddm_pc_RS_NONE_make( TMP_AIJ tmp_aij, HDDM_DOM_VEC *hddm_dom_vec ) ;
int hddm_pc_RS_NONE_set( HDDM_VEC *hddm_vec ) ;
int hddm_pc_RS_DIAG_make( TMP_AIJ tmp_aij, HDDM_DOM_VEC *hddm_dom_vec ) ;
int hddm_pc_RS_DIAG_set( HDDM_VEC *hddm_vec ) ;
#ifdef __ADVFW_SINGLE_MODE__ /* s-mode */
  void hddm_pc_RS_NONE_gather_s( OPinfo **oop, HDDM_VEC *hddm_vec ) ;
  void hddm_pc_RS_DIAG_gather_s( OPinfo **oop, HDDM_VEC *hddm_vec ) ;
#else
  #ifdef __ADVFW_PARALLEL_MODE__ /* p-mode */
  #else /* h-mode */
  #endif /* __ADVFW_PARALLEL_MODE__ */
  void hddm_pc_RS_NONE_gather_ph( OPinfo *op, HDDM_VEC *hddm_vec ) ;
  void hddm_pc_RS_DIAG_gather_ph( OPinfo *op, HDDM_VEC *hddm_vec ) ;
#endif /* __ADVFW_SINGLE_MODE__ */



#ifdef __ADVFW_SINGLE_MODE__ /* s-mode */
  void hddm_solver_RS_iteration_s_1st( OPinfo ***ooop, HDDM_VEC *hddm_vec,
                                       HDDM_Opt hddm_opt,
                                       int (*setPC)( HDDM_VEC *hddm_vec ),
                                       void (*gatherPC_s)( OPinfo **oop,
                                                           HDDM_VEC *hddm_vec ) ) ;
  void hddm_solver_RS_iteration_s( OPinfo ***ooop, HDDM_VEC *hddm_vec,
                                   HDDM_Opt hddm_opt,
                                   int (*setPC)( HDDM_VEC *hddm_vec) ) ;
  void hddm_solver_RS_iteration_s_gather( OPinfo **oop, HDDM_VEC *hddm_vec ) ;
  void hddm_solver_RS_iteration_s_free_oop( OPinfo ***ooop ) ;
#else
  #ifdef __ADVFW_PARALLEL_MODE__ /* p-mode */
  #else /* h-mode */
  #endif /* __ADVFW_PARALLEL_MODE__ */
  void hddm_solver_RS_iteration_ph_1st( OPinfo *op, HDDM_VEC *hddm_vec,
                                        HDDM_Opt hddm_opt,
                                        int (*setPC)( HDDM_VEC *hddm_vec),
                                        void (*gatherPC_ph)( OPinfo *op,
                                                             HDDM_VEC *hddm_vec ) ) ;
  void hddm_solver_RS_iteration_ph( OPinfo *op, HDDM_VEC *hddm_vec,
                                    HDDM_Opt hddm_opt,
                                    int (*setPC)( HDDM_VEC *hddm_vec) ) ;
  void hddm_solver_RS_iteration_ph_gather( OPinfo *op, double *vec ) ;
  void hddm_solver_RS_iteration_ph_renewal( OPinfo *op, double *vec ) ;
#endif /* __ADVFW_SINGLE_MODE__ */


#ifndef __ADVFW_HIERARCHICAL_MODE__ /* s-mode, p-mode */
  void hddm_solver_RS_COM_res_sp_1st( HDDM_VEC *hddm_vec,
                                      HDDM_DOM_VEC hddm_dom_vec,
                                      int ninbc, Inbc *inbc,
                                      HDDM_Opt hddm_opt ) ;
  void hddm_solver_RS_COM_res_sp( HDDM_VEC *hddm_vec,
                                  HDDM_DOM_VEC hddm_dom_vec,
                                  int ninbc, Inbc *inbc,
                                  HDDM_Opt hddm_opt ) ;
  void hddm_solver_RS_COM_res_sp_func( double *hddm_vec,
                                       double *hddm_dom_vec,
                                       int ninbc, Inbc *inbc ) ;
  void hddm_solver_RS_renewal_bc_sp( int nbc, double *vbc,
                                     HDDM_VEC hddm_vec,
                                     int ninbc, Inbc *inbc ) ;
#else                                /* h-mode */
  void hddm_solver_RS_COM_res_h_recv_res_1st( HDDM_VEC *hddm_vec,
                                              int ninbc, Inbc *inbc,
                                              HDDM_Opt hddm_opt,
                                              int compid, int type ) ;
  void hddm_solver_RS_COM_res_h_send_res_1st( HDDM_DOM_VEC hddm_dom_vec,
                                              HDDM_Opt hddm_opt,
                                              int compid, int type ) ;
  void hddm_solver_RS_COM_res_h_recv_res( HDDM_VEC *hddm_vec,
                                          int ninbc, Inbc *inbc,
                                          HDDM_Opt hddm_opt,
                                          int compid, int type ) ;
  void hddm_solver_RS_COM_res_h_send_res( HDDM_DOM_VEC hddm_dom_vec,
                                          HDDM_Opt hddm_opt,
                                          int compid, int type ) ;
  void hddm_solver_RS_COM_res_h_recv_res_func( double *hddm_vec,
                                               int ninbc, Inbc *inbc,
                                               int compid, int type ) ;
  void hddm_sovler_RS_COM_res_h_send_res_func( int nf, double *hddm_dom_vec,
                                               int compid, int type ) ;
  void hddm_solver_RS_renewal_bc_h_send_dom( double *p,
                                             int ninbc, Inbc *inbc,
                                             int compid, int type ) ;
  void hddm_solver_RS_renewal_bc_h_recv_dom( int nbc, double *vbc,
                                             int ninbc,
                                             int compid, int type ) ;
#endif /* __ADVFW_HIERARCHICAL_MODE__ */





void hddm_solver_RS_temp( PartMesh *pm, OPTIONS options ) ;
void hddm_solver_RS_Parent_temp( PartMesh *pm, OPTIONS options ) ;
#ifdef __ADVFW_HIERARCHICAL_MODE__
void hddm_solver_RS_Child_temp( OPTIONS options, MTRL mtrl ) ;
#endif

#endif /* __HDDM_SOLVER_RS_H__ */
