/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/


void __HDDM_SOLVER_COM_RES_SP_1ST__( HDDM_VEC *hddm_vec,
                                     HDDM_DOM_VEC hddm_dom_vec,
                                     int ninbc, Inbc *inbc,
                                     HDDM_Opt hddm_opt )
{
  /*+ Function: add result of domain to HDDM_VEC +*/
  __HDDM_SOLVER_COM_RES_SP_FUNC__( hddm_vec->HDDM_VEC_Q,
                                   hddm_dom_vec.HDDM_DOM_VEC_VEC,
                                   ninbc, inbc ) ;


  if( hddm_opt.hddm_pc == HDDM_PC_DIAG ) {
    __HDDM_SOLVER_COM_RES_SP_FUNC__( hddm_vec->HDDM_VEC_PC,
                                     hddm_dom_vec.HDDM_DOM_VEC_PC,
                                     ninbc, inbc ) ;
  }
}

/********************************************************************/

void __HDDM_SOLVER_COM_RES_SP__( HDDM_VEC *hddm_vec,
                                 HDDM_DOM_VEC hddm_dom_vec,
                                 int ninbc, Inbc *inbc,
                                 HDDM_Opt hddm_opt )
{
  /*+ Function: add result of domain to HDDM_VEC +*/
  __HDDM_SOLVER_COM_RES_SP_FUNC__( hddm_vec->HDDM_VEC_Q,
                                   hddm_dom_vec.HDDM_DOM_VEC_VEC,
                                   ninbc, inbc ) ;
}

/********************************************************************/

void __HDDM_SOLVER_COM_RES_SP_FUNC__( MTYPE *hddm_vec,
                                      MTYPE *hddm_dom_vec,
                                      int ninbc, Inbc *inbc )
{
  /*+ Function: add result of domain to HDDM_VEC +*/
  int    i ;


  for( i=0 ; i<ninbc ; i++ ) {
    int    ii = inbc[i].ln_part ;
    int    jj = inbc[i].nfid ;
    hddm_vec[ii] = MPLUS( hddm_vec[ii], hddm_dom_vec[jj] ) ;
  }
}

/********************************************************************/

void __HDDM_SOLVER_RENEWAL_BC_SP__( int nbc, MTYPE *vbc,
                                    HDDM_VEC hddm_vec,
                                    int ninbc, Inbc *inbc )
{
  /*+ Function: renewal B.C. +*/
  int    i, j ;


  for( i=0,j=nbc-ninbc ; i<ninbc ; i++,j++ ) {
    vbc[j] = hddm_vec.HDDM_VEC_P[inbc[i].ln_part] ;
  }
}
