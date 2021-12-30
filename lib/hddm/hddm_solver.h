/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/
#include "advlas_skyline.h"

#ifdef __ADVFW_SINGLE_MODE__ /* s-mode */
  #include "hddm_solver_s.h"
  #include "hddm_solver_sp.h"
#else
  #ifdef __ADVFW_PARALLEL_MODE__ /* p-mode */
    #include "hddm_solver_p.h"
    #include "hddm_solver_sp.h"
    #include "hddm_solver_ph.h"
  #else /* h-mode */
    #include "hddm_solver_h.h"
    #include "hddm_solver_ph.h"
  #endif /* __ADVFW_PARALLEL_MODE__ */

#endif /* __ADVFW_SINGLE_MODE__ */

#include "hddm_pc.h"

void __HDDM_SOLVER__( PartMesh *pm, OPTIONS options,
                      void (*set_hddm_func)( HDDM_FUNC *hddm_func,
                                             OPTIONS options ) )
{
  /*+ Function: hddm solver +*/
  char   fname[FILENAME_MAX] ;


  if( COM_GID.rootid == COM_ID.myprocid ){
    if( hddm_sw.antenna == -1 ) {
      make_outdata_filename( fname, options.datasw, -1, -1 ) ;
      write_outdata_file_HDDM( fname, -2, pm[0], options ) ;
    }
  }


  #ifdef __ADVFW_SINGLE_MODE__      /* s-mode */
    __HDDM_SOLVER_PARENT__( pm, options, set_hddm_func ) ;
  #else
    #ifdef __ADVFW_PARALLEL_MODE__  /* p-mode */
    //printf("__HDDM_SOLVER__/* p-mode */\n");
    __HDDM_SOLVER_PARENT__( pm, options, set_hddm_func ) ;
    #else                            /* h-mode */
      if( COM_ID.myprocid < COM_GID.nparent ) {
        __HDDM_SOLVER_PARENT__( pm, options, set_hddm_func ) ;
      } else {
        __HDDM_SOLVER_CHILD__( options, pm[0].mtrl, set_hddm_func ) ;
      }
    #endif
  #endif
}

/********************************************************************/

void __HDDM_SOLVER_PARENT__( PartMesh *pm, OPTIONS options,
                             void (*set_hddm_func)( HDDM_FUNC *hddm_func,
                                                    OPTIONS options ) )
{
  /*+ Function: hddm solver for Parent +*/
  int    i, k ;
  FILE*  fp_log = NULL ;

  HDDM_Opt hddm_opt ;
  SolvOpt solvopt ;
  HDDM_FUNC hddm_func ;
  HDDM_VEC* hddm_vec = NULL ;


  hddm_sw.status = __HDDM_SW_FIRST__ ;
  hddm_sw.loop = 0 ;
  set_hddm_func( &hddm_func, options ) ;

  hddm_func.general_func_1st( pm, hddm_vec, options, hddm_opt, solvopt ) ;

  /*** Initialize ***/
  hddm_func.set_HDDM_Opt( &hddm_opt, options ) ;
  hddm_func.set_SolvOpt( &solvopt, options ) ;
  fp_log = solvopt.fp ;
  hddm_vec = (HDDM_VEC *)Malloc( sizeof(HDDM_VEC), nrepeat ) ;
  for( i=nstart,k=0 ; i<nend ; i++,k++ ) {
    init_HDDM_VEC( &(hddm_vec[k]) ) ;
    hddm_func.make_HDDM_VEC( &(hddm_vec[k]), pm[k], hddm_opt ) ;
  }
  if( COM_GID.rootid == COM_ID.myprocid ){
    if( hddm_opt.hddm_log == NEED ) {
      Fprintf( hddm_opt.fp, "DOF on interface %d\n", hddm_vec[0].total_free ) ;
    }
  }

  hddm_func.general_func_2nd( pm, hddm_vec, options, hddm_opt, solvopt ) ;

  /*** HDDM loop ***/
  work_times.work += plusTime( &s_time ) ;
  COM_Bcast_Int( &(hddm_sw.status), 1, COM_GID.rootid ) ;
  work_times.comm += plusTime( &s_time ) ;


  /** 1st step of HDDM loop **/
  if( hddm_opt.hddm_max_loop == 0 ) {
    Fprintf( stdout, "HDDM Solver was not executed!!!\n" ) ;
    return ;
  } else if( hddm_sw.status == __HDDM_SW_FIRST__ ) {
    __HDDM_SOLVER_PARENT_LOOP_1ST__( pm, options, hddm_opt, solvopt,
                                     hddm_vec, hddm_func, fp_log ) ;
  } else if( hddm_sw.status == __HDDM_SW_RESTART__ ) {
    __HDDM_SOLVER_PARENT_LOOP_RESTART__( pm, options, hddm_opt, solvopt,
                                         hddm_vec, hddm_func, fp_log ) ;
  } else {
    char   str[128] ;
    sprintf( str, "%d", hddm_sw.status ) ;
    emessage( 154, __ABORT_LIBHDDM__, str ) ;
  }
  hddm_sw.loop++ ;


  hddm_func.general_func_3rd( pm, hddm_vec, options, hddm_opt, solvopt ) ;


  /** main HDDM loop **/
 
  while( 1 ) {
    work_times.work += plusTime( &s_time ) ;
    COM_Synchronize() ;
    work_times.idle += plusTime( &s_time ) ;

    COM_Bcast_Int( &(hddm_sw.status), 1, COM_GID.rootid ) ;
    work_times.comm += plusTime( &s_time ) ;
    if( hddm_sw.status == __HDDM_SW_LAST__ ) {
      break ;
    }

   __HDDM_SOLVER_PARENT_LOOP__( pm, options, hddm_opt, solvopt,
                                 hddm_vec, hddm_func, fp_log ) ;
    if( hddm_sw.loop == hddm_opt.hddm_max_loop ) {
      hddm_sw.status = __HDDM_SW_LAST__ ;
      /* write restart file */
    }
    hddm_sw.loop++ ;
  }


  hddm_func.general_func_4th( pm, hddm_vec, options, hddm_opt, solvopt ) ;


  /** Last step of HDDM loop **/
  __HDDM_SOLVER_PARENT_LOOP_LAST__( pm, options, hddm_opt, solvopt,
                                    hddm_vec, hddm_func, fp_log ) ;


  hddm_func.general_func_5th( pm, hddm_vec, options, hddm_opt, solvopt ) ;



  /** out result **/
  for( i=nstart,k=0 ; i<nend ; i++,k++ ) {
    hddm_func.out_result( hddm_sw.t_loop, pm[k], options ) ;
  }

  /*** Finalize ***/
  for( i=nstart,k=0 ; i<nend ; i++,k++ ) {
    free_HDDM_VEC( &(hddm_vec[k]) ) ;
  }
  Free( sizeof(HDDM_VEC)*nrepeat, hddm_vec ) ;


  #ifndef __ADVFW_HIERARCHICAL_MODE__
    if( solvopt.solver_log == NEED ) {
      Fclose( fp_log ) ;
    }
  #endif
  if( COM_GID.rootid == COM_ID.myprocid ){
    if( hddm_opt.hddm_log == NEED ) {
      Fclose( hddm_opt.fp ) ;
    }
  }


  work_times.work += plusTime( &s_time ) ;
}

/********************************************************************/

#ifdef __ADVFW_HIERARCHICAL_MODE__
void __HDDM_SOLVER_CHILD__( OPTIONS options, MTRL mtrl,
                            void (*set_hddm_func)( HDDM_FUNC *hddm_func,
                                                   OPTIONS options ) )
{
  /*+ Function: hddm solver for Child +*/
  FILE*  fp_log = NULL ;

  HDDM_Opt hddm_opt ;
  SolvOpt solvopt ;
  HDDM_FUNC hddm_func ;

  DomMesh* dm = NULL ;


  hddm_sw.status = __HDDM_SW_FIRST__ ;
  hddm_sw.loop = 0 ;
  set_hddm_func( &hddm_func, options ) ;


  /*** Initialize ***/
  hddm_func.set_HDDM_Opt( &hddm_opt, options ) ;
  hddm_func.set_SolvOpt( &solvopt, options ) ;
  fp_log = solvopt.fp ;


  /*** HDDM loop ***/
  work_times.work += plusTime( &s_time ) ;
  COM_Bcast_Int( &(hddm_sw.status), 1, COM_GID.rootid ) ;
  work_times.comm += plusTime( &s_time ) ;


  /** 1st step of HDDM loop **/
  if( hddm_opt.hddm_max_loop == 0 ) {
    return ;
  } else if( hddm_sw.status == __HDDM_SW_FIRST__ ) {
    __HDDM_SOLVER_CHILD_LOOP_1ST__( &dm, mtrl, options, hddm_opt, solvopt,

                                    hddm_func, fp_log ) ;
  } else if( hddm_sw.status == __HDDM_SW_RESTART__ ) {
    __HDDM_SOLVER_CHILD_LOOP_RESTART__( &dm, mtrl, options, hddm_opt, solvopt,
                                        hddm_func, fp_log ) ;
  } else {
    char   str[128] ;
    sprintf( str, "%d", hddm_sw.status ) ;
    emessage( 154, __ABORT_LIBHDDM__, str ) ;
  }
  hddm_sw.loop++ ;


  /** main HDDM loop **/
  while( 1 ) {
    work_times.work += plusTime( &s_time ) ;
    COM_Synchronize() ;
    work_times.idle += plusTime( &s_time ) ;

    COM_Bcast_Int( &(hddm_sw.status), 1, COM_GID.rootid ) ;
    work_times.comm += plusTime( &s_time ) ;
    if( hddm_sw.status == __HDDM_SW_LAST__ ) {
      break ;
    }

    __HDDM_SOLVER_CHILD_LOOP__( &dm, mtrl, options, hddm_opt, solvopt,
                                hddm_func, fp_log ) ;
    if( hddm_sw.loop == hddm_opt.hddm_max_loop ) {
      hddm_sw.status = __HDDM_SW_LAST__ ;
      /* write restart file */
    }
    hddm_sw.loop++ ;
  }

  /** Last step of HDDM loop **/
  __HDDM_SOLVER_CHILD_LOOP_LAST__( &dm, mtrl, options, hddm_opt, solvopt,
                                   hddm_func, fp_log ) ;


  if( solvopt.solver_log == NEED ) {
    Fclose( fp_log ) ;
  }

  work_times.work += plusTime( &s_time ) ;
}
#endif

/********************************************************************/

void __HDDM_SOLVER_MAKE_HDDM_VEC__( HDDM_VEC *hddm_vec, HDDM_Opt hddm_opt,
                                    int infree, int outfree, int tfree,
                                    int total_free )
{
  /*+ Function: make struct HDDM_VEC +*/
  hddm_vec->infree = infree ;


  hddm_vec->outfree = outfree ;
  hddm_vec->tfree = tfree ;
  hddm_vec->total_free = total_free ;
  hddm_vec->HDDM_VEC_P = (MTYPE *)Calloc( sizeof(MTYPE), tfree ) ;
  hddm_vec->HDDM_VEC_Q = (MTYPE *)Calloc( sizeof(MTYPE), tfree ) ;
  hddm_vec->HDDM_VEC_R = (MTYPE *)Calloc( sizeof(MTYPE), infree ) ;
  hddm_vec->HDDM_VEC_Z = (MTYPE *)Calloc( sizeof(MTYPE), infree ) ;
  hddm_vec->HDDM_VEC_U = (MTYPE *)Calloc( sizeof(MTYPE), infree ) ;
  if( hddm_opt.hddm_pc == HDDM_PC_DIAG ) {
    hddm_vec->pc_size = tfree ;
    hddm_vec->HDDM_VEC_PC = (MTYPE *)Calloc( sizeof(MTYPE), tfree ) ;
  }
}

/********************************************************************/

void __HDDM_SOLVER_SOLVE_DOM_1ST__( TMP_AIJ *tmp_aij_orig,

                                    int nbc, int *ibc, MTYPE *vbc,
                                    SolvOpt solvopt,
                                    int (*dom_solver)( AIJ_MAT *aij_mat,
                                                       SolvOpt solvopt,
                                                       FILE *monitor ),
                                    HDDM_DOM_VEC *hddm_dom_vec,
                                    int (*makePC)( TMP_AIJ tmp_aij,
                                                   HDDM_DOM_VEC *hddm_dom_vec ),
                                    FILE *monitor )
{
  /*+ Function: solve subdomain +*/

  int    i ;

  TMP_AIJ tmp_aij ;
  AIJ_MAT aij_mat ;

  int*   tmp_ibc ;
  MTYPE* tmp_vbc ;

  __MYMT_AIJ_COPY_TMP_AIJ__( &tmp_aij, *tmp_aij_orig ) ;

  tmp_ibc = (int *)Malloc( sizeof(int), nbc ) ;
  tmp_vbc = (MTYPE *)Malloc( sizeof(MTYPE), nbc ) ;
  memcpy( tmp_ibc, ibc, sizeof(int)*nbc ) ;
  memcpy( tmp_vbc, vbc, sizeof(MTYPE)*nbc ) ;
  __MYMT_AIJ_SETBC__( &tmp_aij, nbc, tmp_ibc, tmp_vbc ) ;
  Free( sizeof(int)*nbc, tmp_ibc ) ;
  Free( sizeof(MTYPE)*nbc, tmp_vbc ) ;

  __MYMT_AIJ_ASSEM__( &aij_mat, &tmp_aij ) ;

  aij_mat.AIJ_X = (MTYPE *)Calloc( sizeof(MTYPE), aij_mat.nf ) ;
  /*__MYMT_AIJ_SOLVER_JUDGE__( dom_solver(&aij_mat, solvopt, monitor) ) ;*/
  dom_solver( &aij_mat, solvopt, monitor ) ;

  hddm_dom_vec->nf = aij_mat.nf ;
  tmp_aij_orig->TMP_X = aij_mat.AIJ_X ;
  tmp_aij_orig->tmp_pc_size = aij_mat.pc_size ;
  aij_mat.pc_size = 0 ;
  tmp_aij_orig->TMP_PC = aij_mat.AIJ_PC ;
  aij_mat.AIJ_PC = NULL ;
  __MYMT_AIJ_FREE__( &aij_mat ) ;

  hddm_dom_vec->HDDM_DOM_VEC_VEC = (MTYPE *)Calloc( sizeof(MTYPE),
                                                    tmp_aij_orig->nf ) ;
  __MYMT_AIJ_MAIJMXV_TMP_AIJ__( *tmp_aij_orig, tmp_aij_orig->TMP_X,
                                hddm_dom_vec->HDDM_DOM_VEC_VEC ) ;

  for( i=0 ; i<tmp_aij_orig->nf ; i++ ) {
    hddm_dom_vec->HDDM_DOM_VEC_VEC[i]
                            = MMINUS( hddm_dom_vec->HDDM_DOM_VEC_VEC[i],
                                      tmp_aij_orig->TMP_B[i] ) ;
  }

  makePC( *tmp_aij_orig, hddm_dom_vec ) ;
  Free( sizeof(MTYPE)*tmp_aij_orig->nf, tmp_aij_orig->TMP_X ) ;
}

/********************************************************************/

void __HDDM_SOLVER_SOLVE_DOM__( TMP_AIJ *tmp_aij_orig,
                                int nbc, int *ibc, MTYPE *vbc,
                                SolvOpt solvopt,
                                int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                         SolvOpt solvopt,
                                                         FILE *monitor ),
                                HDDM_DOM_VEC *hddm_dom_vec, FILE *monitor )
{
  /*+ Function: solve subdomain +*/
  int    i ;

  TMP_AIJ tmp_aij ;
  AIJ_MAT aij_mat ;

  int*   tmp_ibc ;
  MTYPE* tmp_vbc ;

  __MYMT_AIJ_COPY_TMP_AIJ__( &tmp_aij, *tmp_aij_orig ) ;
  for( i=0 ; i<tmp_aij.nf ; i++ ) {
    tmp_aij.TMP_B[i] = M_ZERO ;
  }

  tmp_ibc = (int *)Malloc( sizeof(int), nbc ) ;
  tmp_vbc = (MTYPE *)Malloc( sizeof(MTYPE), nbc ) ;
  memcpy( tmp_ibc, ibc, sizeof(int)*nbc ) ;
  memcpy( tmp_vbc, vbc, sizeof(MTYPE)*nbc ) ;
  __MYMT_AIJ_SETBC__( &tmp_aij, nbc, tmp_ibc, tmp_vbc ) ;
  Free( sizeof(int)*nbc, tmp_ibc ) ;
  Free( sizeof(MTYPE)*nbc, tmp_vbc ) ;

  __MYMT_AIJ_ASSEM__( &aij_mat, &tmp_aij ) ;

  aij_mat.AIJ_X = (MTYPE *)Calloc( sizeof(MTYPE), aij_mat.nf ) ;
  /*__MYMT_AIJ_SOLVER_JUDGE__( dom_solver(&aij_mat, solvopt, monitor) ) ;*/
  dom_solver_nmpcm( &aij_mat, solvopt, monitor ) ;

  hddm_dom_vec->nf = aij_mat.nf ;
  tmp_aij_orig->TMP_X = aij_mat.AIJ_X ;
  __MYMT_AIJ_FREE__( &aij_mat ) ;

  hddm_dom_vec->HDDM_DOM_VEC_VEC = (MTYPE *)Calloc( sizeof(MTYPE),
                                                    tmp_aij_orig->nf ) ;
  __MYMT_AIJ_MAIJMXV_TMP_AIJ__( *tmp_aij_orig, tmp_aij_orig->TMP_X,
                                hddm_dom_vec->HDDM_DOM_VEC_VEC ) ;
  Free( sizeof(MTYPE)*tmp_aij_orig->nf, tmp_aij_orig->TMP_X ) ;
}

/********************************************************************/

void __HDDM_SOLVER_SOLVE_DOM_LAST__( TMP_AIJ *tmp_aij_orig,
                                     int nbc, int *ibc, MTYPE *vbc,
                                     SolvOpt solvopt,
                                     int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                              SolvOpt solvopt,
                                                              FILE *monitor ),
                                     HDDM_DOM_VEC *hddm_dom_vec,
                                     FILE *monitor )
{
  /*+ Function: solve subdomain +*/
  int    i ;

  TMP_AIJ tmp_aij ;
  AIJ_MAT aij_mat ;

  int*   tmp_ibc ;
  MTYPE* tmp_vbc ;


  __MYMT_AIJ_COPY_TMP_AIJ__( &tmp_aij, *tmp_aij_orig ) ;

  tmp_ibc = (int *)Malloc( sizeof(int), nbc ) ;
  tmp_vbc = (MTYPE *)Malloc( sizeof(MTYPE), nbc ) ;
  memcpy( tmp_ibc, ibc, sizeof(int)*nbc ) ;
  memcpy( tmp_vbc, vbc, sizeof(MTYPE)*nbc ) ;
  __MYMT_AIJ_SETBC__( &tmp_aij, nbc, tmp_ibc, tmp_vbc ) ;
  Free( sizeof(int)*nbc, tmp_ibc ) ;
  Free( sizeof(MTYPE)*nbc, tmp_vbc ) ;

  __MYMT_AIJ_ASSEM__( &aij_mat, &tmp_aij ) ;

  aij_mat.AIJ_X = (MTYPE *)Calloc( sizeof(MTYPE), aij_mat.nf ) ;
  /*__MYMT_AIJ_SOLVER_JUDGE__( dom_solver(&aij_mat, solvopt, monitor) ) ;*/
  dom_solver_nmpcm( &aij_mat, solvopt, monitor ) ;

  hddm_dom_vec->nf = aij_mat.nf ;
  tmp_aij_orig->TMP_X = aij_mat.AIJ_X ;
  __MYMT_AIJ_FREE__( &aij_mat ) ;

  hddm_dom_vec->HDDM_DOM_VEC_VEC = (MTYPE *)Calloc( sizeof(MTYPE),
                                                    tmp_aij_orig->nf ) ;
  __MYMT_AIJ_MAIJMXV_TMP_AIJ__( *tmp_aij_orig, tmp_aij_orig->TMP_X,
                                hddm_dom_vec->HDDM_DOM_VEC_VEC ) ;
  for( i=0 ; i<tmp_aij_orig->nf ; i++ ) {
    hddm_dom_vec->HDDM_DOM_VEC_VEC[i]
                            = MMINUS( hddm_dom_vec->HDDM_DOM_VEC_VEC[i],
                                      tmp_aij_orig->TMP_B[i] ) ;
  }
}

/********************************************************************/

void __HDDM_SOLVER_SOLVE_DOM_1ST_D__( TMP_AIJ *tmp_aij_orig,
				      DMatrix *dmat_orig,
				      int nbc, int *ibc, MTYPE *vbc,
				      SolvOpt solvopt,
				      int (*dom_solver)( AIJ_MAT *aij_mat,
							 SolvOpt solvopt,
							 FILE *monitor ),
				      HDDM_DOM_VEC *hddm_dom_vec,
				      int (*makePC)( TMP_AIJ tmp_aij,
						     HDDM_DOM_VEC *hddm_dom_vec ),
				      FILE *monitor )
{ 
  int  i ;

  TMP_AIJ tmp_aij ;

  int*   tmp_ibc ;
  MTYPE* tmp_vbc ;

  double _Complex *work ;
  
  double _Complex *rhs ;
  Complex *rhs2 ;

  __MYMT_AIJ_COPY_TMP_AIJ__( &tmp_aij, *tmp_aij_orig ) ;

  tmp_ibc = (int *)Malloc( sizeof(int), nbc ) ;
  tmp_vbc = (MTYPE *)Malloc( sizeof(MTYPE), nbc ) ;
  memcpy( tmp_ibc, ibc, sizeof(int)*nbc ) ;
  memcpy( tmp_vbc, vbc, sizeof(MTYPE)*nbc ) ;
  __MYMT_AIJ_SETBC__( &tmp_aij, nbc, tmp_ibc, tmp_vbc ) ;
  Free( sizeof(int)*nbc, tmp_ibc ) ;
  Free( sizeof(MTYPE)*nbc, tmp_vbc ) ;

  dmat_orig->ndim = tmp_aij.nf ;

  dmat_orig->idx1 = (int *)Calloc( sizeof(int),dmat_orig->ndim+1 ) ;
  dmat_orig->idx2 = (int *)Calloc( sizeof(int),dmat_orig->ndim+1 ) ;

  advlas_copy_nzindex_to_skyindex(tmp_aij.tmp_aij, dmat_orig) ;

  dmat_orig->mat_dc = (double _Complex *)Calloc( sizeof(double _Complex),
						 dmat_orig->nlen ) ;

  advlas_copy_nzmat_to_skymat(tmp_aij.tmp_ai, 
			      tmp_aij.tmp_aij, 
			      tmp_aij.tmp_vij_c, 
			      dmat_orig) ;

  work = (double _Complex *)Calloc( sizeof(double _Complex), dmat_orig->ndim ) ;

  advlas_skyline_ldl_decomposite(*dmat_orig, work ) ;
  
  Free( sizeof(double _Complex)*dmat_orig->ndim, work ) ;

  rhs = (double _Complex *)Calloc( sizeof(double _Complex), dmat_orig->ndim ) ;
  rhs2=(Complex *)Calloc( sizeof(Complex), dmat_orig->ndim ) ;

  
  for(i = 0; i < dmat_orig->ndim; i++)
    rhs[i] = tmp_aij.tmp_b_c[i].re + tmp_aij.tmp_b_c[i].im  * I  ;

  advlas_skyline_ldl_solve( *dmat_orig, rhs ) ;
  
  hddm_dom_vec->nf = tmp_aij.nf ;

  for(i = 0; i < dmat_orig->ndim; i++){
    rhs2[i].re = creal(rhs[i]) ; 
    rhs2[i].im = cimag(rhs[i]) ; 
  }
  
  tmp_aij_orig->TMP_X = (MTYPE *)rhs2 ;   

  tmp_aij_orig->tmp_pc_size = tmp_aij.tmp_pc_size ;
  tmp_aij_orig->tmp_pc_c = tmp_aij.tmp_pc_c ;

  __MYMT_AIJ_FREE_TMP_AIJ__( &tmp_aij ) ; 

  hddm_dom_vec->HDDM_DOM_VEC_VEC = (MTYPE *)Calloc( sizeof(MTYPE), tmp_aij_orig->nf ) ;
  
  __MYMT_AIJ_MAIJMXV_TMP_AIJ__( *tmp_aij_orig, tmp_aij_orig->TMP_X,
                                hddm_dom_vec->HDDM_DOM_VEC_VEC ) ;

  for( i = 0; i < tmp_aij_orig->nf ; i++ ) {
    hddm_dom_vec->HDDM_DOM_VEC_VEC[i]
                            = MMINUS( hddm_dom_vec->HDDM_DOM_VEC_VEC[i],
                                      tmp_aij_orig->TMP_B[i] ) ;
  }

  makePC( *tmp_aij_orig, hddm_dom_vec ) ;

  Free( sizeof(double _Complex)*tmp_aij_orig->nf, rhs ) ;
  Free( sizeof(MTYPE)*tmp_aij_orig->nf, tmp_aij_orig->TMP_X ) ;

}

/********************************************************************/

void __HDDM_SOLVER_SOLVE_DOM_D__( TMP_AIJ *tmp_aij_orig,
				  DMatrix *dmat_orig,
				  int nbc, int *ibc, MTYPE *vbc,
				  SolvOpt solvopt,
				  int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
							   SolvOpt solvopt,
							   FILE *monitor ),
				  HDDM_DOM_VEC *hddm_dom_vec, FILE *monitor )
{
  /*+ Function: solve subdomain +*/
  
  int    i ;

  int*   tmp_ibc ;
  MTYPE* tmp_vbc ;

  //AIJ_MAT aij_mat ;
  TMP_AIJ tmp_aij ;
  
  double _Complex* rhs ;
  Complex* rhs2 ;

  __MYMT_AIJ_COPY_TMP_AIJ__( &tmp_aij, *tmp_aij_orig ) ;
  for( i=0 ; i<tmp_aij.nf ; i++ ) {
    tmp_aij.TMP_B[i] = M_ZERO ;
  }

  tmp_ibc = (int *)Malloc( sizeof(int), nbc ) ;
  tmp_vbc = (MTYPE *)Malloc( sizeof(MTYPE), nbc ) ;
  memcpy( tmp_ibc, ibc, sizeof(int)*nbc ) ;
  memcpy( tmp_vbc, vbc, sizeof(MTYPE)*nbc ) ;
  __MYMT_AIJ_SETBC__( &tmp_aij, nbc, tmp_ibc, tmp_vbc ) ;
  Free( sizeof(int)*nbc, tmp_ibc ) ;
  Free( sizeof(MTYPE)*nbc, tmp_vbc ) ;

  rhs=(double _Complex *)Calloc( sizeof(double _Complex), tmp_aij.nf) ;
  rhs2=(Complex *)Calloc( sizeof(Complex), tmp_aij.nf) ;

  for(i = 0;i < tmp_aij.nf; i++){
    rhs[i] = tmp_aij.tmp_b_c[i].re + tmp_aij.tmp_b_c[i].im  * I ;
  }

  __MYMT_AIJ_FREE_TMP_AIJ__( &tmp_aij ) ; 

  advlas_skyline_ldl_solve( *dmat_orig, rhs ) ;
   
  hddm_dom_vec->nf = tmp_aij.nf ;

  for(i = 0; i < dmat_orig->ndim; i++){
    rhs2[i].re = creal(rhs[i]) ; 
    rhs2[i].im = cimag(rhs[i]) ; 
  }

  tmp_aij_orig->TMP_X = (MTYPE*) rhs2 ;

  hddm_dom_vec->HDDM_DOM_VEC_VEC = (MTYPE *)Calloc( sizeof(MTYPE),tmp_aij_orig->nf ) ;

  __MYMT_AIJ_MAIJMXV_TMP_AIJ__( *tmp_aij_orig, tmp_aij_orig->TMP_X,
                                hddm_dom_vec->HDDM_DOM_VEC_VEC ) ;

  Free( sizeof(double _Complex)*tmp_aij_orig->nf, rhs ) ;
  Free( sizeof(MTYPE)*tmp_aij_orig->nf, tmp_aij_orig->TMP_X ) ;

}

/********************************************************************/

void __HDDM_SOLVER_SOLVE_DOM_LAST_D__( TMP_AIJ *tmp_aij_orig,
				       DMatrix *dmat_orig,
				       int nbc, int *ibc, MTYPE *vbc,
				       SolvOpt solvopt,
				       int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
								SolvOpt solvopt,
								FILE *monitor ),
				       HDDM_DOM_VEC *hddm_dom_vec,
				       FILE *monitor )
{
  /*+ Function: solve subdomain +*/
  
  int    i ;

  int*   tmp_ibc ;
  MTYPE* tmp_vbc ;  

  double _Complex *rhs ;
  Complex *rhs2 ;

  TMP_AIJ tmp_aij ;
  


  __MYMT_AIJ_COPY_TMP_AIJ__( &tmp_aij, *tmp_aij_orig ) ;

  tmp_ibc = (int *)Malloc( sizeof(int), nbc ) ;
  tmp_vbc = (MTYPE *)Malloc( sizeof(MTYPE), nbc ) ;
  memcpy( tmp_ibc, ibc, sizeof(int)*nbc ) ;
  memcpy( tmp_vbc, vbc, sizeof(MTYPE)*nbc ) ;
  __MYMT_AIJ_SETBC__( &tmp_aij, nbc, tmp_ibc, tmp_vbc ) ;
  Free( sizeof(int)*nbc, tmp_ibc ) ;
  Free( sizeof(MTYPE)*nbc, tmp_vbc ) ;


  rhs=(double _Complex *)Calloc( sizeof(double _Complex), tmp_aij.nf) ;
  rhs2=(Complex *)Calloc( sizeof(Complex), tmp_aij.nf) ;

  for(i = 0;i < tmp_aij.nf; i++){
    rhs[i] = tmp_aij.tmp_b_c[i].re + tmp_aij.tmp_b_c[i].im * I ;
  }

  __MYMT_AIJ_FREE_TMP_AIJ__( &tmp_aij ) ; 

  advlas_skyline_ldl_solve( *dmat_orig, rhs ) ;

  hddm_dom_vec->nf = tmp_aij.nf ;

  for(i = 0; i < dmat_orig->ndim; i++){
    rhs2[i].re = creal(rhs[i]) ; 
    rhs2[i].im = cimag(rhs[i]) ; 
  }

  tmp_aij_orig->TMP_X = (MTYPE*)rhs2 ;

  hddm_dom_vec->HDDM_DOM_VEC_VEC = (MTYPE *)Calloc( sizeof(MTYPE),
                                                    tmp_aij_orig->nf ) ;
  __MYMT_AIJ_MAIJMXV_TMP_AIJ__( *tmp_aij_orig, tmp_aij_orig->TMP_X,
                                hddm_dom_vec->HDDM_DOM_VEC_VEC ) ;
  for( i=0 ; i<tmp_aij_orig->nf ; i++ ) {
    hddm_dom_vec->HDDM_DOM_VEC_VEC[i]
                            = MMINUS( hddm_dom_vec->HDDM_DOM_VEC_VEC[i],
                                      tmp_aij_orig->TMP_B[i] ) ;
    
    }

    Free( sizeof(double _Complex)*tmp_aij_orig->nf, rhs ) ;
}

/***************************************************************************/
