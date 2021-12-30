/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/


void __HDDM_SOLVER_ITERATION_PH_1ST__( OPinfo *op, HDDM_VEC *hddm_vec,
                                       HDDM_Opt hddm_opt,
                                       int (*setPC)( HDDM_VEC *hddm_vec),
                                       void (*gatherPC_ph)( OPinfo *op,
                                                            HDDM_VEC *hddm_vec ) )
{
  /*+ Function: 1st HDDM iteration +*/
  int    i ;

  int    infree = hddm_vec->infree ;
  int    outfree = hddm_vec->outfree ;
  int    tfree = hddm_vec->tfree ;
  MTYPE* r = hddm_vec->HDDM_VEC_R ;
  MTYPE* p = hddm_vec->HDDM_VEC_P ;
  MTYPE* q = hddm_vec->HDDM_VEC_Q ;
  double resi = 0.0 ;


  /* GatherCGVector */
  /* r(0) = SuB0 - g */
  __HDDM_SOLVER_ITERATION_PH_GATHER__( op, q ) ;
  memcpy( r, &(q[outfree]), sizeof(MTYPE)*infree ) ;
  /* make M^-1 */
  gatherPC_ph( op, hddm_vec ) ;


  /* renew_inner_first */
  /* z(0) = M^-1 * r(0) */
  setPC( hddm_vec ) ;
  /* p(0) = z(0) */
  memcpy( &(p[outfree]), hddm_vec->HDDM_VEC_Z, sizeof(MTYPE)*infree ) ;


  for( i=0 ; i<infree ; i++ ) {
    double tmp = MABS( r[i] ) ;
    if( resi < tmp ) {
      resi = tmp ;
    }
  }
  work_times.work += plusTime( &s_time ) ;
  {
    double dbuf = resi ;
    COM_Grp_MaxDbl( &dbuf, &resi, 1 ) ;
  }
  work_times.comm += plusTime( &s_time ) ;
  if( COM_GID.rootid == COM_ID.myprocid ){
    if( hddm_opt.hddm_log == NEED ) {
      Rfprintf( hddm_opt.fp, "%d %le  # Norm %le\n", 0, 1.0, resi ) ;
    }
  }
  if( resi < NEARLY_ZERO ) {
    hddm_sw.status = __HDDM_SW_LAST__ ;
  } else {
    hddm_sw.status = __HDDM_SW_ITERATION__ ;
  }
  hddm_vec->r0 = 1.0 / resi ;


  __HDDM_SOLVER_ITERATION_PH_RENEWAL__( op, p ) ;


  for( i=0 ; i<tfree ; i++ ) {
    q[i] = M_ZERO ;
  }
}

/********************************************************************/

// メインループ
void __HDDM_SOLVER_ITERATION_PH__( OPinfo *op, HDDM_VEC *hddm_vec,
                                   HDDM_Opt hddm_opt,
                                   int (*setPC)( HDDM_VEC *hddm_vec) )
{
  /*+ Function: HDDM iteration +*/
  int    i, j ;

  int    infree = hddm_vec->infree ;
  int    outfree = hddm_vec->outfree ;
  int    tfree = hddm_vec->tfree ;
  MTYPE* r = hddm_vec->HDDM_VEC_R ;
  MTYPE* p = hddm_vec->HDDM_VEC_P ;
  MTYPE* q = hddm_vec->HDDM_VEC_Q ;
  MTYPE* u = hddm_vec->HDDM_VEC_U ;
  double resi = 0.0 ;
  MTYPE  rz = M_ZERO ;
  MTYPE  pq = M_ZERO ;
  MTYPE  malpha ;
  MTYPE  beta ;


  /* GatherCGVector */
  __HDDM_SOLVER_ITERATION_PH_GATHER__( op, q ) ;


  /* renew_inner_first */
  setPC( hddm_vec ) ;


  for( i=0,j=outfree ; i<infree ; i++,j++ ) {
    rz = MPLUS( rz, MMULTI(r[i], hddm_vec->HDDM_VEC_Z[i]) ) ;
    pq = MPLUS( pq, MMULTI( p[j], q[j]) ) ;
  }

  work_times.work += plusTime( &s_time ) ;
  if( COM_GID.rootid == COM_ID.myprocid ) {
    int    part ;
    int    npart = COM_GID.nparent ;
    for( part=1 ; part<npart ; part++ ) {
      int    compid = COM_GID.parid[part] ;
      int    type = 5000 + part ;
      MTYPE  tmp ;
      COM_ReadMTYPE( &tmp, 1, &compid, &type ) ;
      rz = MPLUS( rz, tmp ) ;
      COM_ReadMTYPE( &tmp, 1, &compid, &type ) ;
      pq = MPLUS( pq, tmp ) ;
    }
  } else {
    int    compid = COM_GID.rootid ;
    int    type = 5000 + COM_ID.myprocid ;
    COM_WriteMTYPE( &rz, 1, compid, type ) ;
    COM_WriteMTYPE( &pq, 1, compid, type ) ;
  }
  COM_Grp_Bcast_MTYPE( &rz, 1, COM_GID.rootid ) ;
  COM_Grp_Bcast_MTYPE( &pq, 1, COM_GID.rootid ) ;
  work_times.comm += plusTime( &s_time ) ;


  if( (MABS(rz) < NEARLY_ZERO) && (MABS(pq) < NEARLY_ZERO) ) {
    malpha = M_ZERO ;
  } else {
    malpha = MMINUS( M_ZERO, MDIVIDE(rz, pq) ) ;
  }


  for( i=0,j=outfree ; i<infree ; i++,j++ ) {
    u[i] = MPLUS( u[i], MMULTI(malpha, p[j]) ) ;
    r[i] = MPLUS( r[i], MMULTI(malpha, q[j]) ) ;
  }


  setPC( hddm_vec ) ;


  hddm_vec->HDDM_VEC_RZ = rz ;
  rz = M_ZERO ;
  for( i=0 ; i<infree ; i++ ) {
    rz = MPLUS( rz, MMULTI(r[i], hddm_vec->HDDM_VEC_Z[i]) ) ;
  }


  work_times.work += plusTime( &s_time ) ;
  if( COM_GID.rootid == COM_ID.myprocid ) {
    int    part ;
    int    npart = COM_GID.nparent ;
    for( part=1 ; part<npart ; part++ ) {
      int    compid = COM_GID.parid[part] ;
      int    type = 5000 + part ;
      MTYPE  tmp ;
      COM_ReadMTYPE( &tmp, 1, &compid, &type ) ;
      rz = MPLUS( rz, tmp ) ;
    }
  } else {
    int    compid = COM_GID.rootid ;
    int    type = 5000 + COM_ID.myprocid ;
    COM_WriteMTYPE( &rz, 1, compid, type ) ;
  }
  COM_Grp_Bcast_MTYPE( &rz, 1, COM_GID.rootid ) ;
  work_times.comm += plusTime( &s_time ) ;


  if( (MABS(rz) < NEARLY_ZERO)
      && (MABS(hddm_vec->HDDM_VEC_RZ) < NEARLY_ZERO) ) {
    beta = M_ZERO ;
  } else {
    beta = MDIVIDE( rz, hddm_vec->HDDM_VEC_RZ ) ;
  }


  for( i=0,j=outfree ; i<infree ; i++,j++ ) {
    q[j] =  p[j] ;
    p[j] = MPLUS( hddm_vec->HDDM_VEC_Z[i], MMULTI(beta, p[j]) ) ;
  }


  for( i=0 ; i<infree ; i++ ) {
    double tmp = MABS( r[i] ) ;
    if( resi < tmp ) {
      resi = tmp ;
    }
  }
  work_times.work += plusTime( &s_time ) ;
  {
    double dbuf = resi ;
    COM_Grp_MaxDbl( &dbuf, &resi, 1 ) ;
  }
  work_times.comm += plusTime( &s_time ) ;
  resi *= hddm_vec->r0 ;
  if( COM_GID.rootid == COM_ID.myprocid ){
    if( hddm_opt.hddm_log == NEED ) {
      Rfprintf( hddm_opt.fp, "%d %le\n", hddm_sw.loop, resi ) ;
    }
  }
  if( resi < hddm_opt.hddm_eps ) {
    hddm_sw.status = __HDDM_SW_LAST__ ;
    memcpy( &(p[outfree]), u, sizeof(MTYPE)*infree ) ;
  } else {
    hddm_sw.status = __HDDM_SW_ITERATION__ ;
  }


  __HDDM_SOLVER_ITERATION_PH_RENEWAL__( op, p ) ;


  for( i=0 ; i<tfree ; i++ ) {
    q[i] = M_ZERO ;
  }
}

/********************************************************************/

void __HDDM_SOLVER_ITERATION_PH_GATHER__( OPinfo *op, MTYPE *vec )
{
  /*+ Function: gather vec +*/
  int    i ;

  int    part ;
  int    partid = COM_ID.myprocid ;
  int    npart = COM_GID.nparent ;

  int    start_id = 1 - partid ;
  int    end_id = 2 * npart - partid - 2 ;


  work_times.work += plusTime( &s_time ) ;


  for( part=start_id ; part<end_id ; part++ ) {
    /*  COM_Grp_Synchronize() ; */

    if( (-1 < part) && (part < partid) ) {
      int    size = op[part].n_mofree ;
      if( size != 0 ) {
        int    type = 3000 + partid ;
        int    compid = COM_GID.parid[part] ;
        int*   ibid = op[part].ibid ;
        MTYPE* d = (MTYPE *)Malloc( sizeof(MTYPE), size ) ;
        for( i=0 ; i<size ; i++ ) {
          d[i] = vec[ibid[i]] ;
        }
        COM_WriteMTYPE( d, size, compid, type ) ;
        Free( sizeof(MTYPE)*size, d ) ;
      }
    } else if( (partid < part) && (part < npart) ) {
      int    size = op[part].n_mofree ;
      if( size != 0 ) {
        int    type = 3000 + part ;
        int    compid = COM_GID.parid[part] ;
        int*   ibid = op[part].ibid ;
        MTYPE* d = (MTYPE *)Malloc( sizeof(MTYPE), size ) ;
        COM_ReadMTYPE( d, size, &compid, &type ) ;
        for( i=0 ; i<size ; i++ ) {
          int    ii = ibid[i] ;
          vec[ii] = MPLUS( vec[ii], d[i] ) ;
        }
        Free( sizeof(MTYPE)*size, d ) ;
      }
    }
  }


  work_times.comm += plusTime( &s_time ) ;
}

/********************************************************************/

void __HDDM_SOLVER_ITERATION_PH_RENEWAL__( OPinfo *op, MTYPE *vec )
{
  /*+ Function: renewal vec (from young parnet to old parent) +*/
  int    i ;

  int    part ;
  int    npart = COM_GID.nparent ;
  int    procid = COM_ID.myprocid ;


  work_times.work += plusTime( &s_time ) ;


  for( part=0 ; part<procid ; part++ ) {
    int    size = op[part].n_mofree ;
    if( size != 0 ) {
      int    compid = COM_GID.parid[part] ;
      int    type = 4000 + part ;
      int*   ibid = op[part].ibid ;
      MTYPE* d = (MTYPE *)Malloc( sizeof(MTYPE), size ) ;
      COM_ReadMTYPE( d, size, &compid, &type ) ;
      for( i=0 ; i<size ; i++ ) {
        vec[ibid[i]] = d[i] ;
      }
      Free( sizeof(MTYPE)*size, d ) ;
    }
  }


  for( part=procid+1 ; part<npart ; part++ ) {
    int    size = op[part].n_mofree ;
    if( size != 0 ) {
      int    compid = COM_GID.parid[part] ;
      int    type = 4000 + procid ;
      int*   ibid = op[part].ibid ;
      MTYPE* d = (MTYPE *)Malloc( sizeof(MTYPE), size ) ;
      for( i=0 ; i<size ; i++ ) {
        d[i] = vec[ibid[i]] ;
      }
      COM_WriteMTYPE( d, size, compid, type ) ;
      Free( sizeof(MTYPE)*size, d ) ;
    }
  }


  work_times.comm += plusTime( &s_time ) ;
}
