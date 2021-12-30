/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __FULL_WAVE_H__
#define __FULL_WAVE_H__

#include "advfullwave_lib.h"
#include "mymt_aij_solver.h"
#include "libfem.h"
#include "libhddm.h"
#include "advlas_skyline.h"


void set_hddm_func_fullwave( HDDM_FUNC *hddm_func, OPTIONS options ) ;

/* set options */
void HDDM_set_HDDM_Opt_fullwave( HDDM_Opt *hddm_opt, OPTIONS options ) ;
void HDDM_set_SolvOpt_fullwave( SolvOpt *solvopt, OPTIONS options ) ;

/* initialize */
void HDDM_make_HDDM_VEC_fullwave( HDDM_VEC *hddm_vec, PartMesh pm,
                                 HDDM_Opt hddm_opt ) ;
void HDDM_init_bc_fullwave( DomMesh *dm ) ;

/* renewal interface B.C. */
#ifndef __ADVFW_HIERARCHICAL_MODE__
  void HDDM_renewal_bc_sp_fullwave( DomMesh *dm, HDDM_VEC hddm_vec ) ;
#else
  void HDDM_renewal_bc_h_send_dom_fullwave( DomMesh dm, HDDM_VEC hddm_vec,
                                           int compid, int type ) ;
  void HDDM_renewal_bc_h_recv_dom_fullwave( DomMesh *dm,
                                           int compid, int type ) ;
#endif

/* TMP_AIJ */
void HDDM_make_TMP_AIJ_em_E( DomMesh *dm, MTRL mtrl ) ;
void HDDM_make_TMP_AIJ_fullwave_sub( TMP_AIJ *tmp_aij ) ;
void HDDM_free_TMP_AIJ_fullwave( DomMesh *dm ) ;

/* solve subdomain */
void HDDM_solve_dom_1st_fullwave( DomMesh *dm, HDDM_Opt hddm_opt,
                                 SolvOpt solvopt,
                                 int (*dom_solver)( AIJ_MAT *aij_mat,
                                                    SolvOpt solvopt,
                                                    FILE *monitor ),
                                 HDDM_DOM_VEC *hddm_dom_vec,
                                 int (*makePC)( TMP_AIJ tmp_aij,
                                                HDDM_DOM_VEC *hddm_dom_vec ),
                                 FILE *fp_log ) ;
void HDDM_solve_dom_fullwave( DomMesh *dm, SolvOpt solvopt,
                             int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                      SolvOpt solvopt,
                                                      FILE *monitor ),
                             HDDM_DOM_VEC *hddm_dom_vec, FILE *fp_log ) ;
void HDDM_solve_dom_last_fullwave( DomMesh *dm, SolvOpt solvopt,
                                  int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                           SolvOpt solvopt,
                                                           FILE *monitor ),
                                  HDDM_DOM_VEC *hddm_dom_vec,
                                  FILE *fp_log ) ;

void HDDM_solve_dom_1st_fullwave_d( DomMesh *dm, HDDM_Opt hddm_opt,
                                 SolvOpt solvopt,
                                 int (*dom_solver)( AIJ_MAT *aij_mat,
                                                    SolvOpt solvopt,
                                                    FILE *monitor ),
                                 HDDM_DOM_VEC *hddm_dom_vec,
                                 int (*makePC)( TMP_AIJ tmp_aij,
                                                HDDM_DOM_VEC *hddm_dom_vec ),
				   FILE *fp_log ) ;
void HDDM_solve_dom_fullwave_d( DomMesh *dm, SolvOpt solvopt,
                             int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                      SolvOpt solvopt,
                                                      FILE *monitor ),
                             HDDM_DOM_VEC *hddm_dom_vec, FILE *fp_log ) ;
void HDDM_solve_dom_last_fullwave_d( DomMesh *dm, SolvOpt solvopt,
                                  int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                           SolvOpt solvopt,
                                                           FILE *monitor ),
                                  HDDM_DOM_VEC *hddm_dom_vec,
                                  FILE *fp_log ) ;

/* communicate each iteration results */
#ifndef __ADVFW_HIERARCHICAL_MODE__
  void HDDM_COM_res_sp_1st_fullwave( HDDM_VEC *hddm_vec,
                                    HDDM_DOM_VEC hddm_dom_vec,
                                    DomMesh dm, HDDM_Opt hddm_opt ) ;
  void HDDM_COM_res_sp_fullwave( HDDM_VEC *hddm_vec,
                                HDDM_DOM_VEC hddm_dom_vec,
                                DomMesh dm, HDDM_Opt hddm_opt ) ;
#else
  void HDDM_COM_res_h_recv_1st_fullwave( HDDM_VEC *hddm_vec, DomMesh dm,
                                        HDDM_Opt hddm_opt,
                                        int compid, int type ) ;
  void HDDM_COM_res_h_recv_fullwave( HDDM_VEC *hddm_vec, DomMesh dm,
                                    HDDM_Opt hddm_opt,
                                    int compid, int type ) ;
#endif /* __ADVFW_HIERARCHICAL_MODE__ */

/* iterations */
#ifdef __ADVFW_SINGLE_MODE__
  void HDDM_iteration_s_1st_fullwave( PartMesh *pm, HDDM_VEC *hddm_vec,
                                     HDDM_Opt hddm_opt,
                                     int (*setPC)( HDDM_VEC *hddm_vec ),
                                     void (*gatherPC_s)( OPinfo **oop,
                                                         HDDM_VEC *hddm_vec ) ) ;
  void HDDM_iteration_s_fullwave( PartMesh *pm, HDDM_VEC *hddm_vec,
                                 HDDM_Opt hddm_opt,
                                 int (*setPC)( HDDM_VEC *hddm_vec ) ) ;
  void HDDM_iteration_s_make_oop_fullwave( OPinfo ***ooop, PartMesh *pm ) ;
#else
  void HDDM_iteration_ph_1st_fullwave( PartMesh pm, HDDM_VEC *hddm_vec,
                                      HDDM_Opt hddm_opt,
                                      int (*setPC)( HDDM_VEC *hddm_vec ),
                                      void (*gatherPC_ph)( OPinfo *op,
                                                           HDDM_VEC *hddm_vec ) ) ;
  void HDDM_iteration_ph_fullwave( PartMesh pm, HDDM_VEC *hddm_vec,
                                  HDDM_Opt hddm_opt,
                                  int (*setPC)( HDDM_VEC *hddm_vec ) ) ;

#endif /* __ADVFW_SINGLE_MODE__ */

/* get result */
#ifndef __ADVFW_HIERARCHICAL_MODE__
  void HDDM_get_result_sp_fullwave( PartMesh *pm )  ;
  void HDDM_post_sp_em_E( PartMesh *pm ) ;

#else
  void HDDM_post_h_recv_res_fullwave( DomMesh *dm, int compid, int type ) ;
  void HDDM_post_h_send_res_em_E( DomMesh *dm, MTRL mtrl,
                                       int compid, int type ) ;
  void HDDM_post_h_send_res_fullwave_func( DomMesh *dm, MTRL mtrl,
                                          int compid, int type ) ;
  void HDDM_post_h_send_res_em_func( DomMesh *dm, MTRL mtrl,
					       int compid, int type );

#endif /* #ifndef __ADVFW_HIERARCHICAL_MODE__ */

void HDDM_post_dom_em_E( DomMesh *dm, MTRL mtrl ) ;
void HDDM_post_em_E( int el, int *nop, double *crd,
                          int *flag, int *apid2phiid,
			Complex *x_c, Complex *rotE, Complex *ET,
			Complex *Jee, MTRL mtrl ) ;
void HDDM_out_result_fullwave( int t_loop, PartMesh pm, OPTIONS options ) ;

/* general functions */
void HDDM_general_func_none_fullwave( PartMesh *pm, HDDM_VEC *hddm_vec,
                                     OPTIONS options,
                                     HDDM_Opt hddm_opt, SolvOpt solvopt ) ;

#endif /* __FULL_WAVE_H__ */
