/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "set_hddm_func.h"


extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;

extern int nstart ;
extern int nend ;
extern int nrepeat ;

extern HDDM_SW hddm_sw ;
extern WORK_TIMES work_times ;
extern double s_time ;


void HDDM_set_HDDM_Opt( HDDM_Opt *hddm_opt, OPTIONS options )
{
  /*+ Function: set options for HDDM +*/
  hddm_opt->hddm_solver = options.hddm_opt.hddm_solver ;
  hddm_opt->hddm_pc = options.hddm_opt.hddm_pc ;
  hddm_opt->hddm_eps = options.hddm_opt.hddm_eps ;
  hddm_opt->hddm_nl_eps = options.hddm_opt.hddm_nl_eps ;
  hddm_opt->hddm_div = options.hddm_opt.hddm_div ;
  hddm_opt->hddm_nl_div = options.hddm_opt.hddm_nl_div ;
  hddm_opt->hddm_max_loop = options.hddm_opt.hddm_max_loop ;
  hddm_opt->hddm_log = options.hddm_opt.hddm_log ;
}

/***********************************************************************/

void HDDM_set_HDDM_Opt_log( HDDM_Opt *hddm_opt, OPTIONS options,
                            char *mode, char *mess )
{
  /*+ Function: set options for HDDM +*/
  if( COM_GID.rootid == COM_ID.myprocid ){
    if( hddm_opt->hddm_log == NEED ) {
      char fname[FILENAME_MAX] ;
      make_calclog_filename( fname, options.datasw, -2, hddm_sw.t_loop ) ;
      hddm_opt->fp = Fopen( fname, mode ) ;
      fprintf( hddm_opt->fp, "\n" ) ;
      fprintf( hddm_opt->fp, "\n" ) ;
      Fprintf( hddm_opt->fp, "%s\n", mess ) ;
    }
  }
}

/***********************************************************************/

void HDDM_set_SolvOpt( SolvOpt *solvopt, OPTIONS options )
{
  /*+ Function: set options for Solver +*/
  solvopt->solver = options.solvopt.solver ;
  solvopt->solver_pc = options.solvopt.solver_pc ;
  solvopt->solver_eps = options.solvopt.solver_eps ;
  solvopt->solver_div = options.solvopt.solver_div ;
  solvopt->solver_max_loop = options.solvopt.solver_max_loop ;
  solvopt->solver_ic_shift = options.solvopt.solver_ic_shift ;
  solvopt->solver_log = options.solvopt.solver_log ;
}

/***********************************************************************/

void HDDM_set_SolvOpt_log( SolvOpt *solvopt, OPTIONS options, char *name )
{
  /*+ Function: set options for Solver +*/
  #ifdef __ADVFW_HIERARCHICAL_MODE__
    if( COM_ID.myprocid >= COM_GID.nparent ) {
  #endif
      if( solvopt->solver_log == NEED ) {
        char   fname[FILENAME_MAX] ;
        sprintf( fname, "%s/%s/%s_HDDM_DOM_%s_%d",
                 options.datasw.data_dir, options.datasw.calclog_dir,
                 options.datasw.calclog_file, name,
                 COM_ID.myprocid );
        solvopt->fp = Fopen( fname, "w" ) ;
      }
  #ifdef __ADVFW_HIERARCHICAL_MODE__
    }
  #endif
}

/***********************************************************************/

void set_hddm_func_correct_Jo( HDDM_FUNC *hddm_func, OPTIONS options )
{
  /*+ Function: set HDDM functions +*/
  hddm_sw.Jo = 0 ;


  /* set options */
  hddm_func->set_HDDM_Opt = HDDM_set_HDDM_Opt_correct_Jo ;
  hddm_func->set_SolvOpt = HDDM_set_SolvOpt_correct_Jo ;

  /* initialize */
  hddm_func->make_HDDM_VEC = HDDM_make_HDDM_VEC_correct_Jo ;
  hddm_func->init_bc = HDDM_init_bc_correct_Jo ;

  /* renewal interface B.C. */
  #ifndef __ADVFW_HIERARCHICAL_MODE__
    hddm_func->renewal_bc_sp = HDDM_renewal_bc_sp_correct_Jo ;
  #else
    hddm_func->renewal_bc_h_send_dom = HDDM_renewal_bc_h_send_dom_correct_Jo ;
    hddm_func->renewal_bc_h_recv_dom = HDDM_renewal_bc_h_recv_dom_correct_Jo ;
  #endif

  /* TMP_AIJ */
  hddm_func->make_TMP_AIJ = HDDM_make_TMP_AIJ_correct_Jo ;
  hddm_func->free_TMP_AIJ = HDDM_free_TMP_AIJ_correct_Jo ;

  /* solver for subdomain */
  if( options.solvopt.coJo_sol == SOLVER_CGMETHOD ) {
    hddm_func->dom_solver = mymt_RS_AIJ_Solver_CG ;
    hddm_func->dom_solver_nmpcm = mymt_RS_AIJ_Solver_CG_nmpcm ;
  } else {
    hddm_func->dom_solver = mymt_RS_AIJ_Solver_CG ;
    hddm_func->dom_solver_nmpcm = mymt_RS_AIJ_Solver_CG_nmpcm ;
  }

  /* preconditioner for HDDM */
  if( options.hddm_opt.coJo_hddm_pc == HDDM_PC_NONE ) {
    hddm_func->makePC =  hddm_pc_RS_NONE_make ;
    hddm_func->setPC =  hddm_pc_RS_NONE_set ;
    #ifdef __ADVFW_SINGLE_MODE__
      hddm_func->gatherPC_s = hddm_pc_RS_NONE_gather_s ;
    #else
      hddm_func->gatherPC_ph = hddm_pc_RS_NONE_gather_ph ;
    #endif
  } else if( options.hddm_opt.coJo_hddm_pc == HDDM_PC_DIAG ) {
    hddm_func->makePC =  hddm_pc_RS_DIAG_make ;
    hddm_func->setPC =  hddm_pc_RS_DIAG_set ;
    #ifdef __ADVFW_SINGLE_MODE__
      hddm_func->gatherPC_s = hddm_pc_RS_DIAG_gather_s ;
    #else
      hddm_func->gatherPC_ph = hddm_pc_RS_DIAG_gather_ph ;
    #endif
  } else {
    hddm_func->makePC =  hddm_pc_RS_DIAG_make ;
    hddm_func->setPC =  hddm_pc_RS_DIAG_set ;
    #ifdef __ADVFW_SINGLE_MODE__
      hddm_func->gatherPC_s = hddm_pc_RS_DIAG_gather_s ;
    #else
      hddm_func->gatherPC_ph = hddm_pc_RS_DIAG_gather_ph ;
    #endif
  }

  /* solve subdomain */
  hddm_func->solve_dom_1st = HDDM_solve_dom_1st_correct_Jo ;
  hddm_func->solve_dom = HDDM_solve_dom_correct_Jo ;
  hddm_func->solve_dom_last = HDDM_solve_dom_last_correct_Jo ;

  /* communicate each iteration results */
  #ifndef __ADVFW_HIERARCHICAL_MODE__
    hddm_func->COM_res_sp_1st = HDDM_COM_res_sp_1st_correct_Jo ;
    hddm_func->COM_res_sp = HDDM_COM_res_sp_correct_Jo ;
  #else
    hddm_func->COM_res_h_recv_1st = HDDM_COM_res_h_recv_1st_correct_Jo ;
    hddm_func->COM_res_h_recv = HDDM_COM_res_h_recv_correct_Jo ;
  #endif

  /* iterations */
  #ifdef __ADVFW_SINGLE_MODE__
    hddm_func->iteration_s_1st = HDDM_iteration_s_1st_correct_Jo ;
    hddm_func->iteration_s = HDDM_iteration_s_correct_Jo ;
  #else
    hddm_func->iteration_ph_1st = HDDM_iteration_ph_1st_correct_Jo ;
    hddm_func->iteration_ph = HDDM_iteration_ph_correct_Jo ;
  #endif

  /* get result */
  #ifndef __ADVFW_HIERARCHICAL_MODE__
    hddm_func->get_result_sp = HDDM_get_result_sp_correct_Jo ;
    hddm_func->post_sp = HDDM_post_sp_correct_Jo ;
  #else
    hddm_func->post_h_recv_res = HDDM_post_h_recv_res_correct_Jo ;
    hddm_func->post_h_send_res = HDDM_post_h_send_res_correct_Jo ;
  #endif /* #ifndef __ADVFW_HIERARCHICAL_MODE__ */
  hddm_func->out_result = HDDM_out_result_correct_Jo ;

  /* general functions */
  hddm_func->general_func_1st = HDDM_general_func_none_correct_Jo ;
  hddm_func->general_func_2nd = HDDM_general_func_make_Jo_correct_Jo ;
  hddm_func->general_func_3rd = HDDM_general_func_none_correct_Jo ;
  hddm_func->general_func_4th = HDDM_general_func_none_correct_Jo ;
  hddm_func->general_func_5th = HDDM_general_func_none_correct_Jo ;
}

/***********************************************************************/

void set_hddm_func_correct_Jor( HDDM_FUNC *hddm_func, OPTIONS options )
{
  /*+ Function: set HDDM functions +*/
  set_hddm_func_correct_Jo( hddm_func, options ) ;


  hddm_sw.Jo = -1 ;


  hddm_func->set_HDDM_Opt = HDDM_set_HDDM_Opt_correct_Jor ;
  #ifndef __ADVFW_HIERARCHICAL_MODE__
    hddm_func->get_result_sp = HDDM_get_result_sp_correct_Jor ;
  #else
    hddm_func->post_h_recv_res = HDDM_post_h_recv_res_correct_Jor ;
    hddm_func->post_h_send_res = HDDM_post_h_send_res_correct_Jor ;
  #endif /* #ifndef __ADVFW_HIERARCHICAL_MODE__ */
}

/***********************************************************************/

void set_hddm_func_correct_Joi( HDDM_FUNC *hddm_func, OPTIONS options )
{
  /*+ Function: set HDDM functions +*/
  set_hddm_func_correct_Jo( hddm_func, options ) ;


  hddm_sw.Jo = -2 ;


  hddm_func->set_HDDM_Opt = HDDM_set_HDDM_Opt_correct_Joi ;
  #ifndef __ADVFW_HIERARCHICAL_MODE__
    hddm_func->get_result_sp = HDDM_get_result_sp_correct_Joi ;
  #else
    hddm_func->post_h_recv_res = HDDM_post_h_recv_res_correct_Joi ;
    hddm_func->post_h_send_res = HDDM_post_h_send_res_correct_Joi ;
  #endif /* #ifndef __ADVFW_HIERARCHICAL_MODE__ */
}

/***********************************************************************/

void HDDM_set_HDDM_Opt_correct_Jo_sub( HDDM_Opt *hddm_opt, OPTIONS options )
{
  /*+ Function: set options for HDDM +*/
  hddm_opt->hddm_solver = options.hddm_opt.coJo_hddm_sol ;
  hddm_opt->hddm_pc = options.hddm_opt.coJo_hddm_pc ;
  hddm_opt->hddm_eps = options.hddm_opt.coJo_hddm_eps ;
  hddm_opt->hddm_div = options.hddm_opt.coJo_hddm_div ;
  hddm_opt->hddm_max_loop = options.hddm_opt.coJo_hddm_max_loop ;
  hddm_opt->hddm_log = options.hddm_opt.coJo_hddm_log ;
}

/***********************************************************************/

void HDDM_set_HDDM_Opt_correct_Jo( HDDM_Opt *hddm_opt, OPTIONS options )
{
  /*+ Function: set options for HDDM +*/
  HDDM_set_HDDM_Opt_correct_Jo_sub( hddm_opt, options ) ;
  if( COM_GID.rootid == COM_ID.myprocid ){
    if( hddm_sw.antenna == 0 ) {
      HDDM_set_HDDM_Opt_log( hddm_opt, options, "w", "Correction Jo\n" ) ;
    } else {
      HDDM_set_HDDM_Opt_log( hddm_opt, options, "a", "Correction Jo\n" ) ;
    }
    if( hddm_opt->hddm_log == NEED ) {
      Fprintf( hddm_opt->fp, "  Antenna number = %d\n", hddm_sw.antenna ) ;
    }
  }
}

/***********************************************************************/

void HDDM_set_HDDM_Opt_correct_Jor( HDDM_Opt *hddm_opt, OPTIONS options )
{
  /*+ Function: set options for HDDM +*/
  HDDM_set_HDDM_Opt_correct_Jo_sub( hddm_opt, options ) ;
  if( COM_GID.rootid == COM_ID.myprocid ){
    if( hddm_sw.antenna == 0 ) {
      HDDM_set_HDDM_Opt_log( hddm_opt, options, "w",
                             "Correction Jo : Real part\n" ) ;
    } else {
      HDDM_set_HDDM_Opt_log( hddm_opt, options, "a",
                             "Correction Jo : Real part\n" ) ;
    }
    if( hddm_opt->hddm_log == NEED ) {
      Fprintf( hddm_opt->fp, "  Antenna number = %d\n", hddm_sw.antenna ) ;
    }
  }
}

/***********************************************************************/

void HDDM_set_HDDM_Opt_correct_Joi( HDDM_Opt *hddm_opt, OPTIONS options )
{
  /*+ Function: set options for HDDM +*/
  HDDM_set_HDDM_Opt_correct_Jo_sub( hddm_opt, options ) ;
  if( COM_GID.rootid == COM_ID.myprocid ){
    HDDM_set_HDDM_Opt_log( hddm_opt, options, "a",
                           "Correction Jo : Imaginary part\n" ) ;
    if( hddm_opt->hddm_log == NEED ) {
      Fprintf( hddm_opt->fp, "  Antenna number = %d\n", hddm_sw.antenna ) ;
    }
  }
}

/***********************************************************************/

void HDDM_set_SolvOpt_correct_Jo( SolvOpt *solvopt, OPTIONS options )
{
  /*+ Function: set options for Solver +*/
  solvopt->solver = options.solvopt.coJo_sol ;
  solvopt->solver_pc = options.solvopt.coJo_sol_pc ;
  solvopt->solver_eps = options.solvopt.coJo_sol_eps ;
  solvopt->solver_div = options.solvopt.coJo_sol_div ;
  solvopt->solver_max_loop = options.solvopt.coJo_sol_max_loop ;
  solvopt->solver_ic_shift = options.solvopt.coJo_sol_ic_shift ;
  solvopt->solver_log = options.solvopt.coJo_sol_log ;
  #ifdef __ADVFW_HIERARCHICAL_MODE__
    if( COM_ID.myprocid >= COM_GID.nparent ) {
  #endif
      if( solvopt->solver_log == NEED ) {
        char   name[FILENAME_MAX] ;
        if( hddm_sw.Jo == 0 ) {
          sprintf( name, "CorrectingJo_%d", hddm_sw.antenna ) ;
        } else if( hddm_sw.Jo == -1 ) {
          sprintf( name, "CorrectingJor_%d", hddm_sw.antenna ) ;
        } else {
          sprintf( name, "CorrectingJoi_%d", hddm_sw.antenna ) ;
        }
        HDDM_set_SolvOpt_log( solvopt, options, name ) ;
        Pfprintf( solvopt->fp, "Antenna number %d\n", hddm_sw.antenna ) ;
      }
  #ifdef __ADVFW_HIERARCHICAL_MODE__
    }
  #endif
}

/***********************************************************************/

void HDDM_make_HDDM_VEC_correct_Jo( HDDM_VEC *hddm_vec, PartMesh pm,
                                    HDDM_Opt hddm_opt )
{
  /*+ Function: make struct HDDM_VEC +*/
  hddm_solver_RS_make_HDDM_VEC( hddm_vec, hddm_opt,
                                pm.pantenna[hddm_sw.antenna].infree,
                                pm.pantenna[hddm_sw.antenna].outfree,
                                pm.pantenna[hddm_sw.antenna].tfree,
                                pm.pantenna[hddm_sw.antenna].total_free ) ;
}

/***********************************************************************/

void HDDM_init_bc_correct_Jo( DomMesh *dm )
{
  /*+ Function: initialize bc for HDDM of correct Jo +*/
  DirichletBC* dbc = &(dm->antenna[hddm_sw.antenna].dbc) ;


  dbc->vbc = (double *)Calloc( sizeof(double), dbc->nbc ) ;
}

/***********************************************************************/

#ifndef __ADVFW_HIERARCHICAL_MODE__
void HDDM_renewal_bc_sp_correct_Jo( DomMesh *dm, HDDM_VEC hddm_vec )
{
  /*+ Function: renewal bc for HDDM of correct Jo +*/
  hddm_solver_RS_renewal_bc_sp( dm->antenna[hddm_sw.antenna].dbc.nbc,
                                dm->antenna[hddm_sw.antenna].dbc.vbc, hddm_vec,
                                dm->antenna[hddm_sw.antenna].ninbc,
                                dm->antenna[hddm_sw.antenna].inbc ) ;
}

/***********************************************************************/

#else
void HDDM_renewal_bc_h_send_dom_correct_Jo( DomMesh dm, HDDM_VEC hddm_vec,
                                            int compid, int type )
{
  /*+ Function: renewal bc for HDDM of correct Jo +*/
  hddm_solver_RS_renewal_bc_h_send_dom( hddm_vec.p_d,
                                        dm.antenna[hddm_sw.antenna].ninbc,
                                        dm.antenna[hddm_sw.antenna].inbc,
                                        compid, type ) ;
}

/***********************************************************************/

void HDDM_renewal_bc_h_recv_dom_correct_Jo( DomMesh *dm,
                                            int compid, int type )
{
  /*+ Function: renewal bc for HDDM of correct Jo +*/
  hddm_solver_RS_renewal_bc_h_recv_dom( dm->antenna[hddm_sw.antenna].dbc.nbc,
                                        dm->antenna[hddm_sw.antenna].dbc.vbc,
                                        dm->antenna[hddm_sw.antenna].ninbc,
                                        compid, type ) ;
}
#endif

/***********************************************************************/

void HDDM_make_TMP_AIJ_correct_Jo( DomMesh *dm, MTRL mtrl )
{
  /*+ Function: make TMP_AIJ for HDDM of correct Jo +*/
  int    i ;

  int    el ;
  int    nel = dm->nel ;
  int*   nop = dm->nop ;
  int*   flag = dm->flag ;
  double* crd = dm->crd ;
  int    ap_elem = dm->ap_elem ;

  ANTENNA*  antenna = &(dm->antenna[hddm_sw.antenna]) ;
  int    nf = antenna->nf ;
  int*   enf = antenna->enf ;
  double* Jo = antenna->Jo ;

  TMP_AIJ* tmp_aij = &(antenna->tmp_aij) ;


  mymt_RS_AIJ_Init( nf, tmp_aij, ap_elem ) ;
  for( el=0 ; el<nel ; el++ ) {
    tet_MakeElement_correct_Jo( hddm_sw.antenna, el, nop, crd, enf,
                                flag[el], mtrl, Jo, tmp_aij ) ;
  }
  {
    int*   tai = tmp_aij->tmp_ai ;
    int**  taij = tmp_aij->tmp_aij ;
    double** tvij = tmp_aij->tmp_vij_d ;
    for( i=0 ; i<nf ; i++ ) {
      sort_id( tai[i], taij[i], tvij[i], 1 ) ;
    }
  }
  mymt_RS_AIJ_DeleteZero( tmp_aij ) ;
}

/***********************************************************************/

void HDDM_free_TMP_AIJ_correct_Jo( DomMesh *dm )
{
  /*+ Function: free TMP_AIJ for HDDM of correct Jo +*/
  mymt_RS_AIJ_Free_TMP_AIJ( &(dm->antenna[hddm_sw.antenna].tmp_aij) ) ;
}

/***********************************************************************/

void HDDM_solve_dom_1st_correct_Jo( DomMesh *dm, HDDM_Opt hddm_opt,
                                    SolvOpt solvopt,
                                    int (*dom_solver)( AIJ_MAT *aij_mat,
                                                       SolvOpt solvopt,
                                                       FILE *monitor ),
                                    HDDM_DOM_VEC *hddm_dom_vec,
                                    int (*makePC)( TMP_AIJ tmp_aij,
                                                   HDDM_DOM_VEC *hddm_dom_vec ),
                                    FILE *fp_log )
{
  /*+ Function: make TMP_AIJ for HDDM of correct Jo +*/
  hddm_solver_RS_solve_dom_1st( &(dm->antenna[hddm_sw.antenna].tmp_aij),
                                dm->antenna[hddm_sw.antenna].dbc.nbc,
                                dm->antenna[hddm_sw.antenna].dbc.ibc,
                                dm->antenna[hddm_sw.antenna].dbc.vbc,
                                solvopt, dom_solver,
                                hddm_dom_vec, makePC, fp_log ) ;
}

/***********************************************************************/

void HDDM_solve_dom_correct_Jo( DomMesh *dm, SolvOpt solvopt,
                                int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                         SolvOpt solvopt,
                                                         FILE *monitor ),
                                HDDM_DOM_VEC *hddm_dom_vec, FILE *fp_log )
{
  /*+ Function: make TMP_AIJ for HDDM of correct Jo +*/
  hddm_solver_RS_solve_dom( &(dm->antenna[hddm_sw.antenna].tmp_aij),
                            dm->antenna[hddm_sw.antenna].dbc.nbc,
                            dm->antenna[hddm_sw.antenna].dbc.ibc,
                            dm->antenna[hddm_sw.antenna].dbc.vbc,
                            solvopt, dom_solver_nmpcm,
                            hddm_dom_vec, fp_log ) ;
}

/***********************************************************************/

void HDDM_solve_dom_last_correct_Jo( DomMesh *dm, SolvOpt solvopt,
                                     int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                              SolvOpt solvopt,
                                                              FILE *monitor ),
                                     HDDM_DOM_VEC *hddm_dom_vec,
                                     FILE *fp_log )
{
  /*+ Function: make TMP_AIJ for HDDM of correct Jo +*/
  hddm_solver_RS_solve_dom_last( &(dm->antenna[hddm_sw.antenna].tmp_aij),
                                 dm->antenna[hddm_sw.antenna].dbc.nbc,
                                 dm->antenna[hddm_sw.antenna].dbc.ibc,
                                 dm->antenna[hddm_sw.antenna].dbc.vbc,
                                 solvopt, dom_solver_nmpcm,
                                 hddm_dom_vec, fp_log ) ;
}

/***********************************************************************/

#ifndef __ADVFW_HIERARCHICAL_MODE__
void HDDM_COM_res_sp_1st_correct_Jo( HDDM_VEC *hddm_vec,
                                     HDDM_DOM_VEC hddm_dom_vec,
                                     DomMesh dm, HDDM_Opt hddm_opt )
{
  /*+ Function: add result of domain to HDDM_VEC +*/
  hddm_solver_RS_COM_res_sp_1st( hddm_vec, hddm_dom_vec,
                                 dm.antenna[hddm_sw.antenna].ninbc,
                                 dm.antenna[hddm_sw.antenna].inbc,
                                 hddm_opt ) ;
}

/***********************************************************************/

void HDDM_COM_res_sp_correct_Jo( HDDM_VEC *hddm_vec,
                                 HDDM_DOM_VEC hddm_dom_vec,
                                 DomMesh dm, HDDM_Opt hddm_opt )
{
  /*+ Function: add result of domain to HDDM_VEC +*/
  hddm_solver_RS_COM_res_sp( hddm_vec, hddm_dom_vec,
                             dm.antenna[hddm_sw.antenna].ninbc,
                             dm.antenna[hddm_sw.antenna].inbc,
                             hddm_opt ) ;
}

/***********************************************************************/

#else
void HDDM_COM_res_h_recv_1st_correct_Jo( HDDM_VEC *hddm_vec, DomMesh dm,
                                         HDDM_Opt hddm_opt,
                                         int compid, int type )
{
  /*+ Function: add result of domain to HDDM_VEC +*/
  hddm_solver_RS_COM_res_h_recv_res_1st( hddm_vec,
                                         dm.antenna[hddm_sw.antenna].ninbc,
                                         dm.antenna[hddm_sw.antenna].inbc,
                                         hddm_opt, compid, type ) ;
}

/***********************************************************************/

void HDDM_COM_res_h_recv_correct_Jo( HDDM_VEC *hddm_vec, DomMesh dm,
                                     HDDM_Opt hddm_opt,
                                     int compid, int type )
{
  /*+ Function: add result of domain to HDDM_VEC +*/
  hddm_solver_RS_COM_res_h_recv_res( hddm_vec,
                                     dm.antenna[hddm_sw.antenna].ninbc,
                                     dm.antenna[hddm_sw.antenna].inbc,
                                     hddm_opt, compid, type ) ;
}
#endif /* __ADVFW_HIERARCHICAL_MODE__ */

/***********************************************************************/

#ifdef __ADVFW_SINGLE_MODE__
void HDDM_iteration_s_1st_correct_Jo( PartMesh *pm, HDDM_VEC *hddm_vec,
                                      HDDM_Opt hddm_opt,
                                      int (*setPC)( HDDM_VEC *hddm_vec ),
                                      void (*gatherPC_s)( OPinfo **oop,
                                                          HDDM_VEC *hddm_vec ) )
{
  OPinfo** oop ;


  HDDM_iteration_s_make_oop_correct_Jo( &oop, pm ) ;
  hddm_solver_RS_iteration_s_1st( &oop, hddm_vec, hddm_opt,
                                  setPC, gatherPC_s ) ;
}

/***********************************************************************/

void HDDM_iteration_s_correct_Jo( PartMesh *pm, HDDM_VEC *hddm_vec,
                                  HDDM_Opt hddm_opt,
                                  int (*setPC)( HDDM_VEC *hddm_vec ) )
{
  OPinfo** oop ;


  HDDM_iteration_s_make_oop_correct_Jo( &oop, pm ) ;
  hddm_solver_RS_iteration_s( &oop, hddm_vec, hddm_opt, setPC ) ;
}

/***********************************************************************/

void HDDM_iteration_s_make_oop_correct_Jo( OPinfo ***ooop, PartMesh *pm )
{
  int    i ;

  int    part ;
  int    npart = COM_GID.nparent ;


  (*ooop) = (void *)Malloc( sizeof(OPinfo*), npart ) ;
  for( part=0 ; part<npart ; part++ ) {
    (*ooop)[part] = (OPinfo *)Malloc( sizeof(OPinfo), npart ) ;
    for( i=0 ; i<npart ; i++ ) {
      (*ooop)[part][i] = pm[part].pantenna[hddm_sw.antenna].op[i] ;
    }
  }
}

/***********************************************************************/

#else
void HDDM_iteration_ph_1st_correct_Jo( PartMesh pm, HDDM_VEC *hddm_vec,
                                      HDDM_Opt hddm_opt,
                                      int (*setPC)( HDDM_VEC *hddm_vec ),
                                      void (*gatherPC_ph)( OPinfo *op,
                                                           HDDM_VEC *hddm_vec ) )
{
  hddm_solver_RS_iteration_ph_1st( pm.pantenna[hddm_sw.antenna].op, hddm_vec,
                                   hddm_opt, setPC, gatherPC_ph ) ;
}

/***********************************************************************/

void HDDM_iteration_ph_correct_Jo( PartMesh pm, HDDM_VEC *hddm_vec,
                                   HDDM_Opt hddm_opt,
                                   int (*setPC)( HDDM_VEC *hddm_vec ) )
{
  hddm_solver_RS_iteration_ph( pm.pantenna[hddm_sw.antenna].op, hddm_vec,
                               hddm_opt, setPC ) ;
}
#endif /* __ADVFW_SINGLE_MODE__ */

/***********************************************************************/

#ifndef __ADVFW_HIERARCHICAL_MODE__
void HDDM_get_result_sp_correct_Jo( PartMesh *pm )
{
  /*+ Function: get result for HDDM of correct Jo +*/
  int    dom ;
  int    ndomain = pm->ndomain ;


  for( dom=0 ; dom<ndomain ; dom++ ) {
    ANTENNA* antenna = &(pm->dm[dom].antenna[hddm_sw.antenna]) ;
    antenna->Ihr = antenna->tmp_aij.tmp_x_d ;
    antenna->tmp_aij.tmp_x_d = NULL ;
    Free( sizeof(double)*antenna->dbc.nbc, antenna->dbc.vbc ) ;
    mymt_RS_AIJ_Free_TMP_AIJ( &(antenna->tmp_aij) ) ;
  }
}

/***********************************************************************/

void HDDM_get_result_sp_correct_Jor( PartMesh *pm )
{
  /*+ Function: get result for HDDM of correct Jo +*/
  HDDM_get_result_sp_correct_Jo( pm ) ;
}

/***********************************************************************/

void HDDM_get_result_sp_correct_Joi( PartMesh *pm )
{
  /*+ Function: get result for HDDM of correct Jo +*/
  int    dom ;
  int    ndomain = pm->ndomain ;


  for( dom=0 ; dom<ndomain ; dom++ ) {
    ANTENNA* antenna =&(pm->dm[dom].antenna[hddm_sw.antenna]) ;
    antenna->Ihi = antenna->tmp_aij.tmp_x_d ;
    antenna->tmp_aij.tmp_x_d = NULL ;
    Free( sizeof(double)*antenna->dbc.nbc, antenna->dbc.vbc ) ;
    mymt_RS_AIJ_Free_TMP_AIJ( &(antenna->tmp_aij) ) ;
  }
}

/***********************************************************************/

void HDDM_post_sp_correct_Jo( PartMesh *pm )
{
  /*+ Function: post for HDDM of correct Jo +*/
  return ;
}

/***********************************************************************/

#else
void HDDM_post_h_recv_res_correct_Jo( DomMesh *dm, int compid, int type )
{
  /*+ Function: recieve result for HDDM of correct Jo +*/
  ANTENNA* antenna = &(dm->antenna[hddm_sw.antenna]) ;


  antenna->Ihr = (double *)Malloc( sizeof(double), antenna->nf ) ;
  work_times.work += plusTime( &s_time ) ;
  COM_ReadDbl( antenna->Ihr, antenna->nf, &compid, &type ) ;
  work_times.comm += plusTime( &s_time ) ;
}

/***********************************************************************/

void HDDM_post_h_send_res_correct_Jo( DomMesh *dm, MTRL mtrl,
                                      int compid, int type )
{
  /*+ Function: send result for HDDM of correct Jo +*/
  ANTENNA* antenna = &(dm->antenna[hddm_sw.antenna]) ;


  work_times.work += plusTime( &s_time ) ;
  COM_WriteDbl( antenna->tmp_aij.tmp_x_d, antenna->nf, compid, type ) ;
  work_times.comm += plusTime( &s_time ) ;
  Free( sizeof(double)*antenna->tmp_aij.nf, antenna->tmp_aij.tmp_x_d ) ;
  Free( sizeof(double)*antenna->dbc.nbc, antenna->dbc.vbc ) ;
  mymt_RS_AIJ_Free_TMP_AIJ( &(antenna->tmp_aij) ) ;
}

/***********************************************************************/

void HDDM_post_h_recv_res_correct_Jor( DomMesh *dm, int compid, int type )
{
  /*+ Function: recieve result for HDDM of correct Jo +*/
  HDDM_post_h_recv_res_correct_Jo( dm, compid, type ) ;
}

/***********************************************************************/

void HDDM_post_h_send_res_correct_Jor( DomMesh *dm, MTRL mtrl,
                                       int compid, int type )
{
  /*+ Function: send result for HDDM of correct Jo +*/
  HDDM_post_h_send_res_correct_Jo( dm, mtrl, compid, type ) ;
}

/***********************************************************************/

void HDDM_post_h_recv_res_correct_Joi( DomMesh *dm, int compid, int type )
{
  /*+ Function: recieve result for HDDM of correct Jo +*/
  ANTENNA* antenna = &(dm->antenna[hddm_sw.antenna]) ;


  antenna->Ihi = (double *)Malloc( sizeof(double), antenna->nf ) ;
  work_times.work += plusTime( &s_time ) ;
  COM_ReadDbl( antenna->Ihi, antenna->nf, &compid, &type ) ;
  work_times.comm += plusTime( &s_time ) ;
}

/***********************************************************************/

void HDDM_post_h_send_res_correct_Joi( DomMesh *dm, MTRL mtrl,
                                       int compid, int type )
{
  /*+ Function: send result for HDDM of correct Jo +*/
  HDDM_post_h_send_res_correct_Jo( dm, mtrl, compid, type ) ;
}
#endif /* #ifndef __ADVFW_HIERARCHICAL_MODE__ */

/***********************************************************************/

void HDDM_out_result_correct_Jo( int t_loop, PartMesh pm, OPTIONS options )
{
  /*+ Function: post for HDDM of correct Jo +*/
  return ;
}

/***********************************************************************/

void HDDM_general_func_none_correct_Jo( PartMesh *pm, HDDM_VEC *hddm_vec,
                                        OPTIONS options,
                                        HDDM_Opt hddm_opt, SolvOpt solvopt )
{
  /*+ Function: general func (NONE) for HDDM of correct Jo +*/
  return ;
}

/***********************************************************************/

void HDDM_general_func_make_Jo_correct_Jo( PartMesh *pm, HDDM_VEC *hddm_vec,
                                           OPTIONS options,
                                           HDDM_Opt hddm_opt,
                                           SolvOpt solvopt )
{
  /*+ Function: general func (make Jo) for HDDM of correct Jo +*/
  char   fname[FILENAME_MAX] ;

  int    dimension = pm[0].dimension ;
  MTRL_ANTENNA_DEFINITION* antenna_def = &(pm[0].mtrl.antenna_def[hddm_sw.antenna]) ;


  if( antenna_def->mode == MTRL_ANTENNA_RF_NUMBER ) {
    int    k ;
    int    part ;
    int    nJo ;
    int*   iJo = NULL ;
    double* Jo = NULL ;

    if( COM_GID.rootid == COM_ID.myprocid ) {
      make_Jodata_filename( fname, options.datasw, antenna_def->fname,
                            hddm_sw.Jo ) ;
      read_Jodata_file_read( fname, &nJo, &iJo, &Jo, dimension ) ;
      sort_id( nJo, iJo, Jo, dimension ) ;
      COM_Grp_Bcast_Int( &nJo, 1, COM_GID.rootid ) ;
    } else {
      COM_Grp_Bcast_Int( &nJo, 1, COM_GID.rootid ) ;
      iJo = (int *)Malloc( sizeof(int), nJo ) ;
      Jo = (double *)Malloc( sizeof(double), nJo*dimension ) ;
    }
    COM_Grp_Bcast_Int( iJo, nJo, COM_GID.rootid ) ;
    COM_Grp_Bcast_Dbl( Jo, nJo*dimension, COM_GID.rootid ) ;

    if( hddm_sw.Jo == -2 ) {
      for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
        int    dom ;
        int    ndomain = pm[k].ndomain ;
        for( dom=0 ; dom<ndomain ; dom++ ) {
          pm[k].dm[dom].antenna[hddm_sw.antenna].Jor
                           = pm[k].dm[dom].antenna[hddm_sw.antenna].Jo ;
        }
      }
    }
    for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
      int    dom ;
      int    ndomain = pm[k].ndomain ;
      int*   pnid2gnid = pm[k].pnid2gnid ;
      for( dom=0 ; dom<ndomain ; dom++ ) {
        int    i, j ;
        int    nap = pm[k].dm[dom].nap ;
        ANTENNA*  antenna = &(pm[k].dm[dom].antenna[hddm_sw.antenna]) ;
        int*   snid2pnid = pm[k].dm[dom].snid2pnid ;
        int*   apid2Joid ;
        int*   apid2snid = antenna->apid2gnid ;
        double* dJo ;


        if( antenna->nf == 0 ) {
          continue ;
        }
        dJo = (double *)Malloc( sizeof(double), antenna->nf*dimension ) ;
        apid2Joid = (int *)Malloc( sizeof(int), nap ) ;
        for( i=0 ; i<nap ; i++ ) {
          apid2Joid[i] = -1 ;
        }
        for( i=0 ; i<nJo ; i++ ) {
          for( j=0 ; j<nap ; j++ ) {
            int    ii = pnid2gnid[snid2pnid[apid2snid[j]]] ;
            if( ii == iJo[i] ) {
              apid2Joid[j] = i ;
              break ;
            }
          }
        }
        for( i=0 ; i<nap ; i++ ) {
          int    ii = antenna->enf[i] ;
          if( ii > -1 ) {
            int    jj = apid2Joid[i] ;
            if( jj > -1 ) {
              int    iii = ii * dimension ;
              int    kk = jj * dimension ;
              for( j=0 ; j<dimension ; j++,iii++,kk++ ) {
                dJo[iii] = Jo[kk] ;
              }
            } else {
              int    iii = ii * dimension ;
              for( j=0 ; j<dimension ; j++,iii++ ) {
                dJo[iii] = 0.0 ;
              }
            }
          }
        }
        Free( sizeof(int)*nap, apid2Joid ) ;
        antenna->Jo = dJo ;
      }
    }

    Free( sizeof(int)*nJo, iJo ) ;
    Free( sizeof(double)*nJo*dimension, Jo ) ;

  } else if( antenna_def->mode == MTRL_ANTENNA_MD_NUMBER ) {
    int    k ;
    int    part ;
    for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
      int    dom ;
      int    ndomain = pm[k].ndomain ;
      for( dom=0 ; dom<ndomain ; dom++ ) {
        make_Jo_from_definition( (*antenna_def),
                                 &(pm[k].dm[dom].antenna[hddm_sw.antenna]),
                                 pm[k].dm[dom].nap, dimension,
                                 pm[k].dm[dom].crd, hddm_sw.Jo ) ;
      }
    }

  } else {
    char   str[128] ;
    sprintf( str, "%d for antenna", antenna_def->mode ) ;
    emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
  }
}
