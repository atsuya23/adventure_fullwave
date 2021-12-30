/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

int __HDDM_PC_NONE_MAKE__( TMP_AIJ tmp_aij, HDDM_DOM_VEC *hddm_dom_vec )
{
  /*+ Function: make preconditioner matrix (NONE) +*/
  hddm_dom_vec->pc_size = 0 ;
  hddm_dom_vec->HDDM_DOM_VEC_PC = NULL ;


  return 0 ;
}

/********************************************************************/

int __HDDM_PC_NONE_SET__( HDDM_VEC *hddm_vec )
{
  /*+ Function: set preconditioner matrix (NONE) +*/
  if( hddm_vec->HDDM_VEC_P != hddm_vec->HDDM_VEC_Z ) {
    memcpy( hddm_vec->HDDM_VEC_Z, hddm_vec->HDDM_VEC_P,
            sizeof(MTYPE)*hddm_vec->infree ) ;
  }


  return 0 ;
}

/********************************************************************/

#ifdef __ADVFW_SINGLE_MODE__ /* s-mode */
void __HDDM_PC_NONE_GATHER_S__( OPinfo **oop, HDDM_VEC *hddm_vec )
{
  /*+ Function: gather pc +*/
  return ;
}

/********************************************************************/

#else
void __HDDM_PC_NONE_GATHER_PH__( OPinfo *op, HDDM_VEC *hddm_vec )
{
  /*+ Function: gather pc +*/
  return ;
}
#endif /* __ADVFW_SINGLE_MODE__ */

/********************************************************************/

int __HDDM_PC_DIAG_MAKE__( TMP_AIJ tmp_aij, HDDM_DOM_VEC *hddm_dom_vec )
{
  /*+ Function: make preconditioner matrix (Diagonal scaling) +*/
  int    i, j ;

  int*    tai = tmp_aij.tmp_ai ;
  int**   taij = tmp_aij.tmp_aij ;
  MTYPE** tvij = tmp_aij.TMP_VIJ ;
  int    pc_size = tmp_aij.nf ;
  MTYPE* pc = (MTYPE *)Calloc( sizeof(MTYPE), pc_size ) ;


  for( i=0 ; i<pc_size ; i++ ) {
    int*   aaij = taij[i] ;
    MTYPE* vvij = tvij[i] ;
    for( j=0 ; j<tai[i] ; j++ ) {
      if( aaij[j] == i ) {
        if( !ISZERO(vvij[j]) ) {
          pc[i] = vvij[j] ;
        } else {
          pc[i] = M_ONE ;
        }
        break ;
      }
    }
    if( ISZERO(pc[i]) ) {
      pc[i] = M_ONE ;
    }
  }
  hddm_dom_vec->pc_size = pc_size ;
  hddm_dom_vec->HDDM_DOM_VEC_PC = pc ;


  return 0 ;
}

/********************************************************************/

int __HDDM_PC_DIAG_SET__( HDDM_VEC *hddm_vec )
{
  /*+ Function: set preconditioner matrix (Diagonal scaling) +*/
  int    i, j ;


  for( i=0,j=hddm_vec->outfree ; i<hddm_vec->infree ; i++,j++ ) {
    hddm_vec->HDDM_VEC_Z[i] = MMULTI( hddm_vec->HDDM_VEC_R[i],
                                      hddm_vec->HDDM_VEC_PC[j] ) ;
  }


  return 0 ;
}

/********************************************************************/

#ifdef __ADVFW_SINGLE_MODE__ /* s-mode */
void __HDDM_PC_DIAG_GATHER_S__( OPinfo **oop, HDDM_VEC *hddm_vec )
{
  /*+ Function: gather pc +*/
  int    i, j ;

  int    part ;
  int    npart = COM_GID.nparent ;


  for( part=0 ; part<npart ; part++ ) {
    for( i=part+1 ; i<npart ; i++ ) {
      int    size = oop[part][i].n_mofree ;
      for( j=0 ; j<size ; j++ ) {
        int    ii = oop[part][i].ibid[j] ;
        int    jj = oop[i][part].ibid[j] ;
        hddm_vec[part].HDDM_VEC_PC[ii]
                            = MPLUS( hddm_vec[part].HDDM_VEC_PC[ii],
                                     hddm_vec[i].HDDM_VEC_PC[jj] ) ;
      }
    }
    for( i=0 ; i<hddm_vec[part].pc_size ; i++ ) {
      MTYPE tmp = hddm_vec[part].HDDM_VEC_PC[i] ;
      if( !ISZERO(tmp) ) {
        hddm_vec[part].HDDM_VEC_PC[i] = MDIVIDE( M_ONE, tmp ) ;
      } else {
        hddm_vec[part].HDDM_VEC_PC[i] = M_ONE ;
      }
    }
  }
}

/********************************************************************/

#else
void __HDDM_PC_DIAG_GATHER_PH__( OPinfo *op, HDDM_VEC *hddm_vec )
{
  /*+ Function: gather pc +*/
  int    i ;


  work_times.work += plusTime( &s_time ) ;


  __HDDM_SOLVER_ITERATION_PH_GATHER__( op, hddm_vec->HDDM_VEC_PC ) ;


  work_times.comm += plusTime( &s_time ) ;


  for( i=0 ; i<hddm_vec->pc_size ; i++ ) {
    MTYPE tmp = hddm_vec->HDDM_VEC_PC[i] ;
    if( !ISZERO(tmp) ) {
      hddm_vec->HDDM_VEC_PC[i] = MDIVIDE( M_ONE, tmp ) ;
    } else {
      hddm_vec->HDDM_VEC_PC[i] = M_ONE ;
    }
  }
}
#endif /* __ADVFW_SINGLE_MODE__ */
