/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, Universit of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "fullwave.h"


extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;

extern int nstart ;
extern int nend ;
extern int nrepeat ;

extern HDDM_SW hddm_sw ;

extern WORK_TIMES work_times ;
extern double s_time ;


void set_hddm_func_fullwave( HDDM_FUNC *hddm_func, OPTIONS options )
{

/* Controler for main and post processing */

  /*+ Function: set HDDM functions +*/
  /* set options */
  hddm_func->set_HDDM_Opt = HDDM_set_HDDM_Opt_fullwave ;
  hddm_func->set_SolvOpt = HDDM_set_SolvOpt_fullwave ;

  /* initialize */
  hddm_func->make_HDDM_VEC = HDDM_make_HDDM_VEC_fullwave ;
  hddm_func->init_bc = HDDM_init_bc_fullwave ;

  /* renewal interface B.C. */
  #ifndef __ADVFW_HIERARCHICAL_MODE__
    hddm_func->renewal_bc_sp = HDDM_renewal_bc_sp_fullwave ;
  #else
    hddm_func->renewal_bc_h_send_dom = HDDM_renewal_bc_h_send_dom_fullwave ;
    hddm_func->renewal_bc_h_recv_dom = HDDM_renewal_bc_h_recv_dom_fullwave ;
  #endif

  /* TMP_AIJ */
    if( options.formulation == FORMULATION_FW_EMETHOD ) {
      hddm_func->make_TMP_AIJ = HDDM_make_TMP_AIJ_em_E ;
    } 
    
  hddm_func->free_TMP_AIJ = HDDM_free_TMP_AIJ_fullwave ;

  /* solver for subdomain */
  if( options.solvopt.solver == SOLVER_COCG ) {
    hddm_func->dom_solver = mymt_CS_AIJ_Solver_COCG ;
    hddm_func->dom_solver_nmpcm = mymt_CS_AIJ_Solver_COCG_nmpcm ;
  } else { 
    if( options.solvopt.solver_dom == SOLVER_LU ) {
      hddm_func->dom_solver = mymt_CS_AIJ_Solver_COCG ;
      hddm_func->dom_solver_nmpcm = mymt_CS_AIJ_Solver_COCG_nmpcm ; 
    } else {
    hddm_func->dom_solver = mymt_CS_AIJ_Solver_COCG ;
    hddm_func->dom_solver_nmpcm = mymt_CS_AIJ_Solver_COCG_nmpcm ;
    }
  }

  /* preconditioner for HDDM */

  if( options.hddm_opt.coJo_hddm_pc == HDDM_PC_NONE ) {
    hddm_func->makePC =  hddm_pc_CS_NONE_make ;
    hddm_func->setPC =  hddm_pc_CS_NONE_set ;
    #ifdef __ADVFW_SINGLE_MODE__
      hddm_func->gatherPC_s = hddm_pc_CS_NONE_gather_s ;
    #else
      hddm_func->gatherPC_ph = hddm_pc_CS_NONE_gather_ph ;
    #endif
  } else if( options.hddm_opt.coJo_hddm_pc == HDDM_PC_DIAG ) {
    hddm_func->makePC =  hddm_pc_CS_DIAG_make ;
    hddm_func->setPC =  hddm_pc_CS_DIAG_set ;
    #ifdef __ADVFW_SINGLE_MODE__
      hddm_func->gatherPC_s = hddm_pc_CS_DIAG_gather_s ;
    #else
      hddm_func->gatherPC_ph = hddm_pc_CS_DIAG_gather_ph ;
    #endif
  } else {
    hddm_func->makePC =  hddm_pc_CS_DIAG_make ;
    hddm_func->setPC =  hddm_pc_CS_DIAG_set ;
    #ifdef __ADVFW_SINGLE_MODE__
      hddm_func->gatherPC_s = hddm_pc_CS_DIAG_gather_s ;
    #else
      hddm_func->gatherPC_ph = hddm_pc_CS_DIAG_gather_ph ;
    #endif
  }

  /* solve subdomain */

  if( options.solvopt.solver_dom == SOLVER_LU ) {
    hddm_func->solve_dom_1st = HDDM_solve_dom_1st_fullwave_d ;
    hddm_func->solve_dom = HDDM_solve_dom_fullwave_d ;
    hddm_func->solve_dom_last = HDDM_solve_dom_last_fullwave_d ;
  } else {
    hddm_func->solve_dom_1st = HDDM_solve_dom_1st_fullwave ;
    hddm_func->solve_dom = HDDM_solve_dom_fullwave ;
    hddm_func->solve_dom_last = HDDM_solve_dom_last_fullwave ;
  }
  
  /* communicate each iteration results */
  #ifndef __ADVFW_HIERARCHICAL_MODE__
    hddm_func->COM_res_sp_1st = HDDM_COM_res_sp_1st_fullwave ;
    hddm_func->COM_res_sp = HDDM_COM_res_sp_fullwave ;
  #else
    hddm_func->COM_res_h_recv_1st = HDDM_COM_res_h_recv_1st_fullwave ;
    hddm_func->COM_res_h_recv = HDDM_COM_res_h_recv_fullwave ;
  #endif

  /* iterations */
  #ifdef __ADVFW_SINGLE_MODE__
    hddm_func->iteration_s_1st = HDDM_iteration_s_1st_fullwave ;
    hddm_func->iteration_s = HDDM_iteration_s_fullwave ;
  #else
    hddm_func->iteration_ph_1st = HDDM_iteration_ph_1st_fullwave ;
    hddm_func->iteration_ph = HDDM_iteration_ph_fullwave ;
  #endif

  /* get result */
  #ifndef __ADVFW_HIERARCHICAL_MODE__
    hddm_func->get_result_sp = HDDM_get_result_sp_fullwave ;
    if( options.formulation == FORMULATION_FW_EMETHOD ) {
      hddm_func->post_sp = HDDM_post_sp_em_E ;
    }

  #else
    hddm_func->post_h_recv_res = HDDM_post_h_recv_res_fullwave ;
    if( options.formulation == FORMULATION_FW_EMETHOD ) {
      hddm_func->post_h_send_res = HDDM_post_h_send_res_em_E ;
    }

  #endif /* #ifndef __ADVFW_HIERARCHICAL_MODE__ */
  hddm_func->out_result = HDDM_out_result_fullwave ;

  /* general functions */
  hddm_func->general_func_1st = HDDM_general_func_none_fullwave ;
  hddm_func->general_func_2nd = HDDM_general_func_none_fullwave ;
  hddm_func->general_func_3rd = HDDM_general_func_none_fullwave ;
  hddm_func->general_func_4th = HDDM_general_func_none_fullwave ;
  hddm_func->general_func_5th = HDDM_general_func_none_fullwave ;
}

/***********************************************************************/

void HDDM_set_HDDM_Opt_fullwave( HDDM_Opt *hddm_opt, OPTIONS options )
{
  /*+ Function: set options for HDDM +*/
  HDDM_set_HDDM_Opt( hddm_opt, options ) ;
  HDDM_set_HDDM_Opt_log( hddm_opt, options, "a", "Full Wave Analysis" ) ;
}

/***********************************************************************/

void HDDM_set_SolvOpt_fullwave( SolvOpt *solvopt, OPTIONS options )
{
  /*+ Function: set options for Solver +*/
  HDDM_set_SolvOpt( solvopt, options ) ;
  HDDM_set_SolvOpt_log( solvopt, options, "Full_Wave" ) ;
}

/***********************************************************************/

void HDDM_make_HDDM_VEC_fullwave( HDDM_VEC *hddm_vec, PartMesh pm,
                                 HDDM_Opt hddm_opt )
{
  /*+ Function: make struct HDDM_VEC +*/
  hddm_solver_CS_make_HDDM_VEC( hddm_vec, hddm_opt, pm.infree,
                                pm.outfree, pm.tfree, pm.total_free ) ;
}

/***********************************************************************/

void HDDM_init_bc_fullwave( DomMesh *dm )
{
  /*+ Function: initialize bc for HDDM +*/
  DirichletBC* dbc = &(dm->dbc_Axn0) ;


  dbc->vbc_c = (Complex *)Calloc( sizeof(Complex), dbc->nbc ) ;
}

/***********************************************************************/

#ifndef __ADVFW_HIERARCHICAL_MODE__
void HDDM_renewal_bc_sp_fullwave( DomMesh *dm, HDDM_VEC hddm_vec )
{
  /*+ Function: renewal bc for HDDM +*/
  hddm_solver_CS_renewal_bc_sp( dm->dbc_Axn0.nbc, dm->dbc_Axn0.vbc_c,
                                hddm_vec, dm->ninbc, dm->inbc ) ;
}

/***********************************************************************/

#else
void HDDM_renewal_bc_h_send_dom_fullwave( DomMesh dm, HDDM_VEC hddm_vec,
                                         int compid, int type )
{
  /*+ Function: renewal bc for HDDM +*/
  hddm_solver_CS_renewal_bc_h_send_dom( hddm_vec.p_c, dm.ninbc, dm.inbc,
                                        compid, type ) ;
}

/***********************************************************************/

void HDDM_renewal_bc_h_recv_dom_fullwave( DomMesh *dm,
                                         int compid, int type )
{
  /*+ Function: renewal bc for HDDM +*/
  hddm_solver_CS_renewal_bc_h_recv_dom( dm->dbc_Axn0.nbc, dm->dbc_Axn0.vbc_c,
                                        dm->ninbc, compid, type ) ;
}
#endif

/***********************************************************************/

void HDDM_make_TMP_AIJ_em_E( DomMesh *dm, MTRL mtrl )
{
  /*+ Function: make TMP_AIJ for HDDM +*/
  int    el ;
  int    nel = dm->nel ;
  int*   nop = dm->nop ;
  int*   flag = dm->flag ;
  double* crd = dm->crd ;
  ANTENNA*  antenna = dm->antenna ;
  int    nf = dm->nmp ;

  TMP_AIJ* tmp_aij = &(dm->tmp_aij) ;


  mymt_CS_AIJ_Init( nf, tmp_aij, dm->ap_elem ) ;
  for( el=0 ; el<nel ; el++ ) {
    tet_MakeElement_EM_E( el, nop, crd, flag, antenna, mtrl, tmp_aij ) ;
  }
  HDDM_make_TMP_AIJ_fullwave_sub( tmp_aij ) ;
}

/***********************************************************************/

void HDDM_make_TMP_AIJ_fullwave_sub( TMP_AIJ *tmp_aij )
{
  /*+ Function: make TMP_AIJ for HDDM +*/
  int    i ;

  int    nf = tmp_aij->nf ;
  int*   tai = tmp_aij->tmp_ai ;
  int**  taij = tmp_aij->tmp_aij ;
  Complex** tvij = tmp_aij->tmp_vij_c ;


  for( i=0 ; i<nf ; i++ ) {
    sort_ic( tai[i], taij[i], tvij[i], 1 ) ;
  }
  mymt_CS_AIJ_DeleteZero( tmp_aij ) ;
}

/***********************************************************************/

void HDDM_free_TMP_AIJ_fullwave( DomMesh *dm )
{
  /*+ Function: free TMP_AIJ for HDDM +*/
  mymt_CS_AIJ_Free_TMP_AIJ( &(dm->tmp_aij) ) ;
}


/***********************************************************************/

void HDDM_free_fullwave( DomMesh *dm )
{
  /*+ Function: free TMP_AIJ for HDDM +*/
  dmat_free( &(dm->dmat) ) ;
}

/***********************************************************************/

void HDDM_solve_dom_1st_fullwave( DomMesh *dm, HDDM_Opt hddm_opt,
                                 SolvOpt solvopt,
                                 int (*dom_solver)( AIJ_MAT *aij_mat,
                                                    SolvOpt solvopt,
                                                    FILE *monitor ),
                                 HDDM_DOM_VEC *hddm_dom_vec,
                                 int (*makePC)( TMP_AIJ tmp_aij,
                                                HDDM_DOM_VEC *hddm_dom_vec ),
                                 FILE *fp_log )
{
  /*+ Function: make TMP_AIJ for HDDM +*/
  hddm_solver_CS_solve_dom_1st( &(dm->tmp_aij), dm->dbc_Axn0.nbc,
                                dm->dbc_Axn0.ibc, dm->dbc_Axn0.vbc_c,
                                solvopt, dom_solver, hddm_dom_vec,
                                makePC, fp_log ) ;
}


/***********************************************************************/

void HDDM_solve_dom_fullwave( DomMesh *dm, SolvOpt solvopt,
                             int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                      SolvOpt solvopt,
                                                      FILE *monitor ),
                             HDDM_DOM_VEC *hddm_dom_vec, FILE *fp_log )
{
  /*+ Function: make TMP_AIJ for HDDM +*/
  hddm_solver_CS_solve_dom( &(dm->tmp_aij), dm->dbc_Axn0.nbc,
                            dm->dbc_Axn0.ibc, dm->dbc_Axn0.vbc_c,
                            solvopt, dom_solver_nmpcm, hddm_dom_vec,
                            fp_log ) ;
}

/***********************************************************************/

void HDDM_solve_dom_last_fullwave( DomMesh *dm, SolvOpt solvopt,
                                  int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                           SolvOpt solvopt,
                                                           FILE *monitor ),
                                  HDDM_DOM_VEC *hddm_dom_vec,
                                  FILE *fp_log )
{
  /*+ Function: make TMP_AIJ for HDDM +*/
  hddm_solver_CS_solve_dom_last( &(dm->tmp_aij), dm->dbc_Axn0.nbc,
                                 dm->dbc_Axn0.ibc, dm->dbc_Axn0.vbc_c,
                                 solvopt, dom_solver_nmpcm, hddm_dom_vec,
                                 fp_log ) ;
}

/***********************************************************************/

void HDDM_solve_dom_1st_fullwave_d( DomMesh *dm, HDDM_Opt hddm_opt,
				   SolvOpt solvopt,
				  int (*dom_solver)( AIJ_MAT *aij_mat,
						     SolvOpt solvopt,
						     FILE *monitor ),
				   HDDM_DOM_VEC *hddm_dom_vec,
				   int (*makePC)( TMP_AIJ tmp_aij,
						  HDDM_DOM_VEC *hddm_dom_vec),
				   FILE *fp_log )
{
  /*+ Function: make TMP_AIJ for HDDM +*/
  hddm_solver_CS_solve_dom_1st_d( &(dm->tmp_aij),&(dm->dmat), dm->dbc_Axn0.nbc,
				  dm->dbc_Axn0.ibc, dm->dbc_Axn0.vbc_c,
				  solvopt, dom_solver, hddm_dom_vec,makePC, fp_log ) ;
}

/***********************************************************************/
void HDDM_solve_dom_fullwave_d( DomMesh *dm, SolvOpt solvopt,
			       int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
							SolvOpt solvopt,
							FILE *monitor ),
			       HDDM_DOM_VEC *hddm_dom_vec, 
			       FILE *fp_log )
{
  /*+ Function: make TMP_AIJ for HDDM +*/
  hddm_solver_CS_solve_dom_d( &(dm->tmp_aij), &(dm->dmat), dm->dbc_Axn0.nbc,
			      dm->dbc_Axn0.ibc, dm->dbc_Axn0.vbc_c,
			      solvopt, dom_solver_nmpcm, hddm_dom_vec,
			      fp_log ) ;
}

/***********************************************************************/
void HDDM_solve_dom_last_fullwave_d( DomMesh *dm,              
				    SolvOpt solvopt,            
				    int (*dom_solver_nmpcm)       
				    ( AIJ_MAT *aij_mat,SolvOpt   
				      solvopt,FILE *monitor ),    
				    HDDM_DOM_VEC *hddm_dom_vec,  
				    FILE *fp_log )
{
  /*+ Function: make TMP_AIJ for HDDM +*/
  hddm_solver_CS_solve_dom_last_d( &(dm->tmp_aij), &(dm->dmat), dm->dbc_Axn0.nbc,
				   dm->dbc_Axn0.ibc, dm->dbc_Axn0.vbc_c, 
				   solvopt, dom_solver_nmpcm, hddm_dom_vec,
				  fp_log ) ;
}

/***********************************************************************/

#ifndef __ADVFW_HIERARCHICAL_MODE__
void HDDM_COM_res_sp_1st_fullwave( HDDM_VEC *hddm_vec,
                                  HDDM_DOM_VEC hddm_dom_vec,
                                  DomMesh dm, HDDM_Opt hddm_opt )
{
  /*+ Function: add result of domain to HDDM_VEC +*/
  hddm_solver_CS_COM_res_sp_1st( hddm_vec, hddm_dom_vec,
                                 dm.ninbc, dm.inbc, hddm_opt ) ;
}

/***********************************************************************/

void HDDM_COM_res_sp_fullwave( HDDM_VEC *hddm_vec,
                              HDDM_DOM_VEC hddm_dom_vec,
                              DomMesh dm, HDDM_Opt hddm_opt )
{
  /*+ Function: add result of domain to HDDM_VEC +*/
  hddm_solver_CS_COM_res_sp( hddm_vec, hddm_dom_vec,
                             dm.ninbc, dm.inbc, hddm_opt ) ;
}

/***********************************************************************/

#else
void HDDM_COM_res_h_recv_1st_fullwave( HDDM_VEC *hddm_vec, DomMesh dm,
                                      HDDM_Opt hddm_opt,
                                      int compid, int type )
{
  /*+ Function: add result of domain to HDDM_VEC +*/
  hddm_solver_CS_COM_res_h_recv_res_1st( hddm_vec, dm.ninbc, dm.inbc,
                                         hddm_opt, compid, type ) ;
}

/***********************************************************************/

void HDDM_COM_res_h_recv_fullwave( HDDM_VEC *hddm_vec, DomMesh dm,
                                  HDDM_Opt hddm_opt,
                                  int compid, int type )
{
  /*+ Function: add result of domain to HDDM_VEC +*/
  hddm_solver_CS_COM_res_h_recv_res( hddm_vec, dm.ninbc, dm.inbc,
                                     hddm_opt, compid, type ) ;
}
#endif /* __ADVFW_HIERARCHICAL_MODE__ */

/***********************************************************************/

#ifdef __ADVFW_SINGLE_MODE__
void HDDM_iteration_s_1st_fullwave( PartMesh *pm, HDDM_VEC *hddm_vec,
                                   HDDM_Opt hddm_opt,
                                   int (*setPC)( HDDM_VEC *hddm_vec ),
                                   void (*gatherPC_s)( OPinfo **oop,
                                                       HDDM_VEC *hddm_vec ) )
{
  OPinfo** oop ;


  HDDM_iteration_s_make_oop_fullwave( &oop, pm ) ;
  hddm_solver_CS_iteration_s_1st( &oop, hddm_vec, hddm_opt,
                                  setPC, gatherPC_s ) ;
}

/***********************************************************************/

void HDDM_iteration_s_fullwave( PartMesh *pm, HDDM_VEC *hddm_vec,
                               HDDM_Opt hddm_opt,
                               int (*setPC)( HDDM_VEC *hddm_vec ) )
{
  OPinfo** oop ;


  HDDM_iteration_s_make_oop_fullwave( &oop, pm ) ;
  hddm_solver_CS_iteration_s( &oop, hddm_vec, hddm_opt, setPC ) ;
}

/***********************************************************************/

void HDDM_iteration_s_make_oop_fullwave( OPinfo ***ooop, PartMesh *pm )
{
  int    i ;

  int    part ;
  int    npart = COM_GID.nparent ;


  (*ooop) = (void *)Malloc( sizeof(OPinfo*), npart ) ;
  for( part=0 ; part<npart ; part++ ) {
    (*ooop)[part] = (OPinfo *)Malloc( sizeof(OPinfo), npart ) ;
    for( i=0 ; i<npart ; i++ ) {
      (*ooop)[part][i] = pm[part].op[i] ;
    }
  }
}

/***********************************************************************/

#else
void HDDM_iteration_ph_1st_fullwave( PartMesh pm, HDDM_VEC *hddm_vec,
                                    HDDM_Opt hddm_opt,
                                    int (*setPC)( HDDM_VEC *hddm_vec ),
                                    void (*gatherPC_ph)( OPinfo *op,
                                                         HDDM_VEC *hddm_vec ) )
{
  hddm_solver_CS_iteration_ph_1st( pm.op, hddm_vec, hddm_opt,
                                   setPC, gatherPC_ph ) ;
}

/***********************************************************************/

void HDDM_iteration_ph_fullwave( PartMesh pm, HDDM_VEC *hddm_vec,
                                HDDM_Opt hddm_opt,
                                int (*setPC)( HDDM_VEC *hddm_vec ) )
{
  hddm_solver_CS_iteration_ph( pm.op, hddm_vec, hddm_opt, setPC ) ;
}
#endif /* __ADVFW_SINGLE_MODE__ */

/***********************************************************************/

#ifndef __ADVFW_HIERARCHICAL_MODE__
void HDDM_get_result_sp_fullwave( PartMesh *pm )
{
  /*+ Function: get result for HDDM +*/
  int    dom ;
  int    ndomain = pm->ndomain ;


  for( dom=0 ; dom<ndomain ; dom++ ) {
    DomMesh* dm = &(pm->dm[dom]) ;
    dm->x_c = dm->tmp_aij.tmp_x_c ;
    dm->tmp_aij.tmp_x_c = NULL ;
    Free( sizeof(Complex)*dm->dbc_Axn0.nbc, dm->dbc_Axn0.vbc_c ) ;
    mymt_CS_AIJ_Free_TMP_AIJ( &(dm->tmp_aij) ) ;
  }
}

/***********************************************************************/

void HDDM_post_sp_em_E( PartMesh *pm )
{
  /*+ Function: post for HDDM +*/
  int    dom ;
  int    ndomain = pm->ndomain ;

  MTRL   mtrl = pm->mtrl ;

  for( dom=0 ; dom<ndomain ; dom++ ) {
    HDDM_post_dom_em_E( &(pm->dm[dom]), mtrl ) ;
  }
}

/***********************************************************************/

#else
void HDDM_post_h_recv_res_fullwave( DomMesh *dm, int compid, int type )
{
  /*+ Function: recieve result for HDDM +*/
  int    nf = dm->nmp + dm->nphi ;
  int    ii = dm->nel * dm->dimension ;


  dm->x_c = (Complex *)Malloc( sizeof(Complex), nf ) ;
  dm->rvalue.el_B_c = (Complex *)Malloc( sizeof(Complex), ii ) ;
  dm->rvalue.el_Je_c = (Complex *)Malloc( sizeof(Complex), ii ) ;
  work_times.work += plusTime( &s_time ) ;
  COM_ReadCpl( dm->x_c, nf, &compid, &type ) ;
  COM_ReadCpl( dm->rvalue.el_B_c, ii, &compid, &type ) ;
  COM_ReadCpl( dm->rvalue.el_Je_c, ii, &compid, &type ) ;
  work_times.comm += plusTime( &s_time ) ;
}

/***********************************************************************/

void HDDM_post_h_send_res_em_E( DomMesh *dm, MTRL mtrl,
                                     int compid, int type )
{
  /*+ Function: send result for HDDM +*/
  dm->x_c = dm->tmp_aij.tmp_x_c ;
  dm->tmp_aij.tmp_x_c = NULL ;
  HDDM_post_dom_em_E( dm, mtrl ) ;
  work_times.work += plusTime( &s_time ) ;
  HDDM_post_h_send_res_em_func( dm, mtrl, compid, type ) ;
}

/***********************************************************************/

void HDDM_post_h_send_res_fullwave_func( DomMesh *dm, MTRL mtrl,
                                        int compid, int type )
{
  /*+ Function: send result for HDDM +*/
  int    ii = dm->nel * dm->dimension ;


  COM_WriteCpl( dm->x_c, dm->tmp_aij.nf, compid, type ) ;
  COM_WriteCpl( dm->rvalue.el_B_c, ii, compid, type ) ;
  COM_WriteCpl( dm->rvalue.el_Je_c, ii, compid, type ) ;
  work_times.comm += plusTime( &s_time ) ;
  Free( sizeof(Complex)*dm->dbc_Axn0.nbc, dm->dbc_Axn0.vbc_c ) ;
  mymt_CS_AIJ_Free_TMP_AIJ( &(dm->tmp_aij) ) ;
}

/***********************************************************************/

void HDDM_post_h_send_res_em_func( DomMesh *dm, MTRL mtrl,
                                        int compid, int type )
{
  /*+ Function: send result for HDDM +*/
  int    ii = dm->nel * dm->dimension ;


  COM_WriteCpl( dm->x_c, dm->tmp_aij.nf, compid, type ) ;
  COM_WriteCpl( dm->rvalue.el_B_c, ii, compid, type ) ;

  COM_WriteCpl( dm->rvalue.el_E_c, ii, compid, type ) ;

  COM_WriteCpl( dm->rvalue.el_Je_c, ii, compid, type ) ;
  work_times.comm +=plusTime( &s_time ) ;
  Free( sizeof(Complex)*dm->dbc_Axn0.nbc, dm->dbc_Axn0.vbc_c ) ;
  mymt_CS_AIJ_Free_TMP_AIJ( &(dm->tmp_aij) ) ;
}

/***********************************************************************/

#endif /* #ifndef __ADVFW_HIERARCHICAL_MODE__ */

/***********************************************************************/

void HDDM_post_dom_em_E( DomMesh *dm, MTRL mtrl )
{
  /*+ Function: post for HDDM +*/
  int    i ;

  int    el ;

  int    nel = dm->nel ;
  int    dimension = dm->dimension ;
  int*   nop = dm->nop ;
  double* crd = dm->crd ;
  int*   flag = dm->flag ;
  int*   apid2phiid = dm->apid2phiid ;
  Complex* x_c = dm->x_c ;

  Complex* B = (Complex *)Calloc( sizeof(Complex), nel*dimension ) ;
  Complex* E = (Complex *)Calloc( sizeof(Complex), nel*dimension ) ;
  Complex* Je = (Complex *)Calloc( sizeof(Complex), nel*dimension ) ;


  for( el=0,i=0 ; el<nel ; el++,i+=dimension ) {
    HDDM_post_em_E( el, nop, crd, flag, apid2phiid,
                         x_c, &(B[i]), &(E[i]), &(Je[i]), mtrl ) ;
  }

  dm->rvalue.el_B_c = B ;
  dm->rvalue.el_E_c = E;
  dm->rvalue.el_Je_c = Je ;
}

/********************************************************************/

void HDDM_post_em_E( int el, int *nop, double *crd,
                          int *flag, int *apid2phiid,
                          Complex *x_c, Complex *rotE, Complex *ET,
                          Complex *Jee ,MTRL mtrl )
{

  /*+ Function: post for HDDM
                  E method +*/
  int    i, j ;
  double dd ;

  int    dimension = 3 ;
  int    ap_elem = 4 ;
  int    nd_elem = 10 ;
  int    mp_elem = nd_elem - ap_elem ;

  int    *ne ;
  int    flg = flag[el] ;
  double nu = 0.0 ;
  double sigma = 0.0 ;
  double omega = mtrl.antenna_omega ;
  double x[ap_elem], y[ap_elem], z[ap_elem] ;
  double D ;
  double l[mp_elem] ;
  double tetg[3] ;
  int    si[mp_elem] ;
  double xi[mp_elem], yi[mp_elem], zi[mp_elem] ;
  double X[mp_elem], Y[mp_elem], Z[mp_elem] ;
  double xy[mp_elem], yz[mp_elem], zx[mp_elem] ;
  double Fx[mp_elem], Fy[mp_elem], Fz[mp_elem] ;

  Complex xe[mp_elem] ;

  for( i=0 ; i<mtrl.nflag ; i++ ) {
    if( flg == mtrl.flag_i[i] ) {
      nu = 7.957747e+05;
      break ;
    }
  }
  for( i=0 ; i<mtrl.nR ; i++ ) {
    if( flg == mtrl.R_i[i] ) {
      sigma = mtrl.R_sigma[i] ;
      break ;
    }
  }

  ne = &(nop[el*nd_elem]) ;
  for( i=0,j=ap_elem ; i<mp_elem ; i++,j++ ) {
    xe[i] = x_c[ne[j]] ;
  }

  tet_pickup_coordinate_4vertex( ne, crd, x, y, z ) ;
  D = tet_Volume6( x, y, z ) ;
  tet_SideLength( x, y, z, l ) ;
  tetNedelec_Direction( x, y, z, si ) ;
  tetNedelec_mk( x, y, z, xi, yi, zi ) ;
  tetNedelec_XYZ( l, si, xi, yi, zi, X, Y, Z ) ;


  /*** Calculate E ***/  

  tetNedelec_km_mk( x, y, z, xy, yz, zx ) ;
  tet_Center( x, y, z, tetg ) ;
  tetNedelec_Fxyz( l, si, tetg, xi, yi, zi, xy, yz, zx, Fx, Fy, Fz ) ;
  
  /*atosyori*/
  /* H = i*(nu/omega)*rotE */
  for( i=0 ; i<dimension ; i++ ) {
    rotE[i] = complex_0 ;
  }
  for( i=0 ; i<mp_elem ; i++ ) {
    xe[i] = ComplexMultiScalar(xe[i], nu/omega);
    Complex cc = ComplexMulti( xe[i], complex_i ) ;
    rotE[0] = ComplexPlus( rotE[0], ComplexMultiScalar(cc, X[i]) ) ;
    rotE[1] = ComplexPlus( rotE[1], ComplexMultiScalar(cc, Y[i]) ) ;
    rotE[2] = ComplexPlus( rotE[2], ComplexMultiScalar(cc, Z[i]) ) ;
  }
  dd = 2.0 / D ;
  for( i=0 ; i<dimension ; i++ ) {
    rotE[i] = ComplexMultiScalar( rotE[i], dd ) ;
  }


  /** E→ET **/ 

  /** E*D **/
  for( i=0 ; i<dimension ; i++ ) {
    ET[i] = complex_0 ;
  }

  for( i=0 ; i<mp_elem ; i++ ) {
    ET[0] = ComplexPlus( ET[0], ComplexMultiScalar(xe[i], Fx[i]) ) ;
    ET[1] = ComplexPlus( ET[1], ComplexMultiScalar(xe[i], Fy[i]) ) ;
    ET[2] = ComplexPlus( ET[2], ComplexMultiScalar(xe[i], Fz[i]) ) ;
  }

  /** E **/
  dd = 2.0 / D ;
  for( i=0 ; i<dimension ; i++ ) {
    ET[i] = ComplexMultiScalar( ET[i], dd ) ;
  }
  
  /*** Calculate Eddy Current ***/
  tet_Center( x, y, z, tetg ) ;
  tetNedelec_km_mk( x, y, z, xy, yz, zx ) ;
  tetNedelec_Fxyz( l, si, tetg, xi, yi, zi, xy, yz, zx, Fx, Fy, Fz ) ;


  /** Je = sigma*E  **/ 
  for( i=0 ; i<dimension ; i++ ) {
    Jee[i] = complex_0 ;
  }
  /* E*D */
  for( i=0 ; i<mp_elem ; i++ ) {
    Jee[0] = ComplexPlus( Jee[0], ComplexMultiScalar(xe[i], Fx[i]) ) ;
    Jee[1] = ComplexPlus( Jee[1], ComplexMultiScalar(xe[i], Fy[i]) ) ;
    Jee[2] = ComplexPlus( Jee[2], ComplexMultiScalar(xe[i], Fz[i]) ) ;
  }

  /* sigma*E */
  dd = sigma / D ;
  for( i=0 ; i<dimension ; i++ ) {
    Jee[i] = ComplexMultiScalar( Jee[i], dd ) ;
  }
}

/***********************************************************************/

void HDDM_out_result_fullwave( int t_loop, PartMesh pm, OPTIONS options )
{
  /*+ Function: out result for HDDM +*/
  char   fname[FILENAME_MAX] ;


  make_outdata_filename( fname, options.datasw, pm.partid, -1 ) ;
  write_outdata_file_HDDM( fname, -1, pm, options ) ;
}

/***********************************************************************/

void HDDM_general_func_none_fullwave( PartMesh *pm, HDDM_VEC *hddm_vec,
                                     OPTIONS options,
                                     HDDM_Opt hddm_opt, SolvOpt solvopt )
{
  /*+ Function: general func (NONE) for HDDM +*/

  return ;
}
