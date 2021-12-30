/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __SET_HDDM_FUNC_H__
#define __SET_HDDM_FUNC_H__


#include "libhddm.h"


/* set options */
void HDDM_set_HDDM_Opt( HDDM_Opt *hddm_opt, OPTIONS options ) ;
void HDDM_set_HDDM_Opt_log( HDDM_Opt *hddm_opt, OPTIONS options,
                            char *mode, char *mess ) ;
void HDDM_set_SolvOpt( SolvOpt *solvopt, OPTIONS options ) ;
void HDDM_set_SolvOpt_log( SolvOpt *solvopt, OPTIONS options, char *name ) ;



/*** correct Jo ***/
void set_hddm_func_correct_Jo( HDDM_FUNC *hddm_func, OPTIONS options ) ;
void set_hddm_func_correct_Jor( HDDM_FUNC *hddm_func, OPTIONS options ) ;
void set_hddm_func_correct_Joi( HDDM_FUNC *hddm_func, OPTIONS options ) ;

/* set options */
void HDDM_set_HDDM_Opt_correct_Jo_sub( HDDM_Opt *hddm_opt, OPTIONS options ) ;
void HDDM_set_HDDM_Opt_correct_Jo( HDDM_Opt *hddm_opt, OPTIONS options ) ;
void HDDM_set_HDDM_Opt_correct_Jor( HDDM_Opt *hddm_opt, OPTIONS options ) ;
void HDDM_set_HDDM_Opt_correct_Joi( HDDM_Opt *hddm_opt, OPTIONS options ) ;
void HDDM_set_SolvOpt_correct_Jo( SolvOpt *solvopt, OPTIONS options ) ;

/* initialize */
void HDDM_make_HDDM_VEC_correct_Jo( HDDM_VEC *hddm_vec, PartMesh pm,
                                    HDDM_Opt hddm_opt ) ;
void HDDM_init_bc_correct_Jo( DomMesh *dm ) ;

/* renewal interface B.C. */
#ifndef __ADVFW_HIERARCHICAL_MODE__
  void HDDM_renewal_bc_sp_correct_Jo( DomMesh *dm, HDDM_VEC hddm_vec ) ;
#else
  void HDDM_renewal_bc_h_send_dom_correct_Jo( DomMesh dm, HDDM_VEC hddm_vec,
                                              int compid, int type ) ;
  void HDDM_renewal_bc_h_recv_dom_correct_Jo( DomMesh *dm,
                                              int compid, int type ) ;
#endif

/* TMP_AIJ */
void HDDM_make_TMP_AIJ_correct_Jo( DomMesh *dm, MTRL mtrl ) ;
void HDDM_free_TMP_AIJ_correct_Jo( DomMesh *dm ) ;

/* solve subdomain */
void HDDM_solve_dom_1st_correct_Jo( DomMesh *dm, HDDM_Opt hddm_opt,
                                    SolvOpt solvopt,
                                    int (*dom_solver)( AIJ_MAT *aij_mat,
                                                       SolvOpt solvopt,
                                                       FILE *monitor ),
                                    HDDM_DOM_VEC *hddm_dom_vec,
                                    int (*makePC)( TMP_AIJ tmp_aij,
                                                   HDDM_DOM_VEC *hddm_dom_vec ),
                                    FILE *fp_log ) ;
void HDDM_solve_dom_correct_Jo( DomMesh *dm, SolvOpt solvopt,
                                int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                         SolvOpt solvopt,
                                                         FILE *monitor ),
                                HDDM_DOM_VEC *hddm_dom_vec, FILE *fp_log ) ;
void HDDM_solve_dom_last_correct_Jo( DomMesh *dm, SolvOpt solvopt,
                                     int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                              SolvOpt solvopt,
                                                              FILE *monitor ),
                                     HDDM_DOM_VEC *hddm_dom_vec,
                                     FILE *fp_log ) ;

/* communicate each iteration results */
#ifndef __ADVFW_HIERARCHICAL_MODE__
  void HDDM_COM_res_sp_1st_correct_Jo( HDDM_VEC *hddm_vec,
                                       HDDM_DOM_VEC hddm_dom_vec,
                                       DomMesh dm, HDDM_Opt hddm_opt ) ;
  void HDDM_COM_res_sp_correct_Jo( HDDM_VEC *hddm_vec,
                                   HDDM_DOM_VEC hddm_dom_vec,
                                   DomMesh dm, HDDM_Opt hddm_opt ) ;
#else
  void HDDM_COM_res_h_recv_1st_correct_Jo( HDDM_VEC *hddm_vec, DomMesh dm,
                                           HDDM_Opt hddm_opt,
                                           int compid, int type ) ;
  void HDDM_COM_res_h_recv_correct_Jo( HDDM_VEC *hddm_vec, DomMesh dm,
                                       HDDM_Opt hddm_opt,
                                       int compid, int type ) ;
#endif /* __ADVFW_HIERARCHICAL_MODE__ */

/* iterations */
#ifdef __ADVFW_SINGLE_MODE__
  void HDDM_iteration_s_1st_correct_Jo( PartMesh *pm, HDDM_VEC *hddm_vec,
                                        HDDM_Opt hddm_opt,
                                        int (*setPC)( HDDM_VEC *hddm_vec ),
                                        void (*gatherPC_s)( OPinfo **oop,
                                                            HDDM_VEC *hddm_vec ) ) ;
  void HDDM_iteration_s_correct_Jo( PartMesh *pm, HDDM_VEC *hddm_vec,
                                    HDDM_Opt hddm_opt,
                                    int (*setPC)( HDDM_VEC *hddm_vec ) ) ;
  void HDDM_iteration_s_make_oop_correct_Jo( OPinfo ***ooop, PartMesh *pm ) ;
#else
  void HDDM_iteration_ph_1st_correct_Jo( PartMesh pm, HDDM_VEC *hddm_vec,
                                         HDDM_Opt hddm_opt,
                                         int (*setPC)( HDDM_VEC *hddm_vec ),
                                         void (*gatherPC_ph)( OPinfo *op,
                                                              HDDM_VEC *hddm_vec ) ) ;
  void HDDM_iteration_ph_correct_Jo( PartMesh pm, HDDM_VEC *hddm_vec,
                                     HDDM_Opt hddm_opt,
                                     int (*setPC)( HDDM_VEC *hddm_vec ) ) ;
#endif /* __ADVFW_SINGLE_MODE__ */

/* get result */
#ifndef __ADVFW_HIERARCHICAL_MODE__
  void HDDM_get_result_sp_correct_Jo( PartMesh *pm ) ;
  void HDDM_get_result_sp_correct_Jor( PartMesh *pm ) ;
  void HDDM_get_result_sp_correct_Joi( PartMesh *pm ) ;
  void HDDM_post_sp_correct_Jo( PartMesh *pm ) ;
#else
  void HDDM_post_h_recv_res_correct_Jo( DomMesh *dm, int compid, int type ) ;
  void HDDM_post_h_send_res_correct_Jo( DomMesh *dm, MTRL mtrl,
                                        int compid, int type ) ;
  void HDDM_post_h_recv_res_correct_Jor( DomMesh *dm, int compid, int type ) ;
  void HDDM_post_h_send_res_correct_Jor( DomMesh *dm, MTRL mtrl,
                                         int compid, int type ) ;
  void HDDM_post_h_recv_res_correct_Joi( DomMesh *dm, int compid, int type ) ;
  void HDDM_post_h_send_res_correct_Joi( DomMesh *dm, MTRL mtrl,
                                         int compid, int type ) ;
#endif /* #ifndef __ADVFW_HIERARCHICAL_MODE__ */
void HDDM_out_result_correct_Jo( int t_loop, PartMesh pm, OPTIONS options ) ;

/* general functions */
void HDDM_general_func_none_correct_Jo( PartMesh *pm, HDDM_VEC *hddm_vec,
                                        OPTIONS options,
                                        HDDM_Opt hddm_opt, SolvOpt solvopt ) ;
void HDDM_general_func_make_Jo_correct_Jo( PartMesh *pm, HDDM_VEC *hddm_vec,
                                           OPTIONS options,
                                           HDDM_Opt hddm_opt,
                                           SolvOpt solvopt ) ;

#endif /* __SET_HDDM_FUNC_H__ */
