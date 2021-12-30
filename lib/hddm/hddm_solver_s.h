/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

void __HDDM_SOLVER_PARENT_LOOP_1ST__( PartMesh *pm,
                                      OPTIONS options,
                                      HDDM_Opt hddm_opt,
                                      SolvOpt solvopt,
                                      HDDM_VEC *hddm_vec,
                                      HDDM_FUNC hddm_func,
                                      FILE *fp_log )
{
  /*+ Function: 1st hddm loop for Parent +*/
  int    part ;
  int    npart = COM_GID.nparent ;


  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "Loop %d\n", hddm_sw.loop ) ;
  }
  for( part=0 ; part<npart ; part++ ) {
    int    dom ;
    int    ndomain = pm[part].ndomain ;
    for( dom=0 ; dom<ndomain ; dom++ ) {
      HDDM_DOM_VEC hddm_dom_vec ;
      if( solvopt.solver_log == NEED ) {
        Pfprintf( fp_log, "\n" ) ;
        Pfprintf( fp_log, "\n" ) ;
        Pfprintf( fp_log, "Part %d, Dom %d\n", part, dom ) ;
      }
      hddm_func.init_bc( &(pm[part].dm[dom]) ) ;
      hddm_func.make_TMP_AIJ( &(pm[part].dm[dom]), pm[part].mtrl ) ;
      init_HDDM_DOM_VEC( &hddm_dom_vec ) ;
      hddm_func.solve_dom_1st( &(pm[part].dm[dom]), hddm_opt, solvopt,
                               hddm_func.dom_solver, &hddm_dom_vec,
                               hddm_func.makePC, fp_log ) ;
      hddm_func.COM_res_sp_1st( &(hddm_vec[part]), hddm_dom_vec,
                                pm[part].dm[dom], hddm_opt ) ;
      free_HDDM_DOM_VEC( &hddm_dom_vec ) ;
    }
  }
  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "\n" ) ;
    Pfprintf( fp_log, "\n" ) ;
  }


  hddm_func.iteration_s_1st( pm, hddm_vec, hddm_opt, hddm_func.setPC,
                             hddm_func.gatherPC_s ) ;
}

/********************************************************************/

void __HDDM_SOLVER_PARENT_LOOP_RESTART__( PartMesh *pm,
                                          OPTIONS options,
                                          HDDM_Opt hddm_opt,
                                          SolvOpt solvopt,
                                          HDDM_VEC *hddm_vec,
                                          HDDM_FUNC hddm_func,
                                          FILE *fp_log )
{
  /*+ Function: hddm loop for restart for Parent +*/
}

/********************************************************************/

void __HDDM_SOLVER_PARENT_LOOP__( PartMesh *pm,
                                  OPTIONS options,
                                  HDDM_Opt hddm_opt,
                                  SolvOpt solvopt,
                                  HDDM_VEC *hddm_vec,
                                  HDDM_FUNC hddm_func,
                                  FILE *fp_log )
{
  /*+ Function: hddm loop for Parent +*/
  int    part ;
  int    npart = COM_GID.nparent ;


  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "Loop %d\n", hddm_sw.loop ) ;
  }
  for( part=0 ; part<npart ; part++ ) {
    int    dom ;
    int    ndomain = pm[part].ndomain ;
    for( dom=0 ; dom<ndomain ; dom++ ) {
      HDDM_DOM_VEC hddm_dom_vec ;
      if( solvopt.solver_log == NEED ) {
        Pfprintf( fp_log, "\n" ) ;
        Pfprintf( fp_log, "\n" ) ;
        Pfprintf( fp_log, "Part %d, Dom %d\n", part, dom ) ;
      }
      hddm_func.renewal_bc_sp( &(pm[part].dm[dom]), hddm_vec[part] ) ;
      init_HDDM_DOM_VEC( &hddm_dom_vec ) ;
      hddm_func.solve_dom( &(pm[part].dm[dom]), solvopt,
                           hddm_func.dom_solver_nmpcm, &hddm_dom_vec,
                           fp_log ) ;
      hddm_func.COM_res_sp( &(hddm_vec[part]), hddm_dom_vec,
                            pm[part].dm[dom], hddm_opt ) ;
      free_HDDM_DOM_VEC( &hddm_dom_vec ) ;
    }
  }
  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "\n" ) ;
    Pfprintf( fp_log, "\n" ) ;
  }


  hddm_func.iteration_s( pm, hddm_vec, hddm_opt, hddm_func.setPC ) ;
}

/********************************************************************/

void __HDDM_SOLVER_PARENT_LOOP_LAST__( PartMesh *pm,
                                       OPTIONS options,
                                       HDDM_Opt hddm_opt,
                                       SolvOpt solvopt,
                                       HDDM_VEC *hddm_vec,
                                       HDDM_FUNC hddm_func,
                                       FILE *fp_log )
{
  /*+ Function: last hddm loop for Parent +*/
  int    part ;
  int    npart = COM_GID.nparent ;


  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "Last step\n" ) ;
  }
  for( part=0 ; part<npart ; part++ ) {
    int    dom ;
    int    ndomain = pm[part].ndomain ;
    for( dom=0 ; dom<ndomain ; dom++ ) {
      HDDM_DOM_VEC hddm_dom_vec ;
      if( solvopt.solver_log == NEED ) {
        Pfprintf( fp_log, "\n" ) ;
        Pfprintf( fp_log, "\n" ) ;
        Pfprintf( fp_log, "Part %d, Dom %d\n", part, dom ) ;
      }
      hddm_func.renewal_bc_sp( &(pm[part].dm[dom]), hddm_vec[part] ) ;
      init_HDDM_DOM_VEC( &hddm_dom_vec ) ;
      hddm_func.solve_dom_last( &(pm[part].dm[dom]), solvopt,
                                hddm_func.dom_solver_nmpcm, &hddm_dom_vec,
                                fp_log ) ;
      hddm_func.COM_res_sp( &(hddm_vec[part]), hddm_dom_vec,
                            pm[part].dm[dom], hddm_opt ) ;
      free_HDDM_DOM_VEC( &hddm_dom_vec ) ;
    }
    hddm_func.get_result_sp( &(pm[part]) ) ;
    hddm_func.post_sp( &(pm[part]) ) ;
  }
}

/********************************************************************/

void __HDDM_SOLVER_ITERATION_S_1ST__( OPinfo ***ooop, HDDM_VEC *hddm_vec,
                                      HDDM_Opt hddm_opt,
                                      int (*setPC)( HDDM_VEC *hddm_vec),
                                      void (*gatherPC_s)( OPinfo **oop,
                                                          HDDM_VEC *hddm_vec ) )
{
  /*+ Function: 1st HDDM iteration +*/
  int    i, j ;

  int    part ;
  int    npart = COM_GID.nparent ;
  OPinfo** oop = (*ooop) ;


  /*** Gather ***/
  /* r0 = SuB0 - g */
  __HDDM_SOLVER_ITERATION_S_GATHER__( oop, hddm_vec ) ;
  for( part=0 ; part<npart ; part++ ) {
    memcpy( hddm_vec[part].HDDM_VEC_R,
            &(hddm_vec[part].HDDM_VEC_Q[hddm_vec[part].outfree]),
            sizeof(MTYPE)*hddm_vec[part].infree ) ;
  }
  /* make M^-1 */
  gatherPC_s( oop, hddm_vec ) ;


  {
    double resi = 0.0 ;
    MTYPE  rz = M_ZERO ;
    for( part=0 ; part<npart ; part++ ) {
      int    infree = hddm_vec[part].infree ;
      int    outfree = hddm_vec[part].outfree ;
      /* z(0) = M^-1 * r(0) */
      setPC( &(hddm_vec[part]) ) ;
      /* p(0) = z(0) */
      memcpy( &(hddm_vec[part].HDDM_VEC_P[outfree]), hddm_vec[part].HDDM_VEC_Z,
              sizeof(MTYPE)*infree ) ;
      /* ||r0|| */
      for( i=0 ; i<infree ; i++ ) {
        double tmp = MABS( hddm_vec[part].HDDM_VEC_R[i] ) ;
        if( tmp > resi ) {
          resi = tmp ;
        }
      }
    }
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
    resi = 1.0 / resi ;

    /* (r(0), z(0)) */
    for( part=0 ; part<npart ; part++ ) {
      int    infree = hddm_vec[part].infree ;
      /*rz = __LOCAL_INNER__( infree, hddm_vec[part].HDDM_VEC_R,
                             hddm_vec[part].HDDM_VEC_Z ) ;*/
      /*rz = MINNER( infree, hddm_vec[part].HDDM_VEC_R,
                     hddm_vec[part].HDDM_VEC_Z ) ;*/
      /*rz = MBILINEAR( infree, hddm_vec[part].HDDM_VEC_R,
                        hddm_vec[part].HDDM_VEC_Z ) ;*/
      for( i=0 ; i<infree ; i++ ) {
        rz = MPLUS( rz, MMULTI(hddm_vec[part].HDDM_VEC_R[i],
                               hddm_vec[part].HDDM_VEC_Z[i]) ) ;
      }
    }

    for( part=0 ; part<npart ; part++ ) {
      hddm_vec[part].r0 = resi ;
      hddm_vec[part].HDDM_VEC_RZ = rz ;
    }
  }


  for( part=0 ; part<npart ; part++ ) {
    for( i=part+1 ; i<npart ; i++ ) {
      int    size = oop[part][i].n_mofree ;
      for( j=0 ; j<size ; j++ ) {
        int    ii = oop[i][part].ibid[j] ;
        int    jj = oop[part][i].ibid[j] ;
        hddm_vec[i].HDDM_VEC_P[ii] = hddm_vec[part].HDDM_VEC_P[jj] ;
      }
    }
  }
  for( part=0 ; part<npart ; part++ ) {
    int    ii = hddm_vec[part].tfree ;
    for( j=0 ; j<ii ; j++ ) {
      hddm_vec[part].HDDM_VEC_Q[j] = M_ZERO ;
    }
  }


  __HDDM_SOLVER_ITERATION_S_FREE_OOP__( ooop ) ;
}

/********************************************************************/

void __HDDM_SOLVER_ITERATION_S__( OPinfo ***ooop, HDDM_VEC *hddm_vec,
                                  HDDM_Opt hddm_opt,
                                  int (*setPC)( HDDM_VEC *hddm_vec) )
{
  /*+ Function: HDDM iteration +*/
  int    i, j ;

  int    part ;
  int    npart = COM_GID.nparent ;
  OPinfo** oop = (*ooop) ;


  /* q(n) = Sp(n) */
  __HDDM_SOLVER_ITERATION_S_GATHER__( oop, hddm_vec ) ;


  {
    MTYPE  rz = hddm_vec[0].HDDM_VEC_RZ ;
    MTYPE  pq = M_ZERO ;
    MTYPE  malpha = M_ZERO ;
    MTYPE  beta = M_ZERO ;
    double resi = 0.0 ;
    /* -alpha = - (r(n),z(n)) / (p(n),q(n)) */
    for( part=0 ; part<npart ; part++ ) {
      int    infree = hddm_vec[part].infree ;
      int    outfree = hddm_vec[part].outfree ;
      for( i=0,j=outfree ; i<infree ; i++,j++ ) {
        pq = MPLUS( pq, MMULTI(hddm_vec[part].HDDM_VEC_P[j],
                               hddm_vec[part].HDDM_VEC_Q[j]) ) ;
      }
    }
    if( (MABS(rz) < NEARLY_ZERO) && (MABS(pq) < NEARLY_ZERO) ) {
      malpha = M_ZERO ;
    } else {
      malpha = MMINUS( M_ZERO, MDIVIDE(rz, pq) ) ;
    }

    /* u(n+1) = u(n) - alpha * p(n) */
    /* r(n+1) = r(n) - alpha * q(n) */
    /* ||r(n+1)|| */
    for(  part=0 ; part<npart ; part++ ) {
      int    infree = hddm_vec[part].infree ;
      int    outfree = hddm_vec[part].outfree ;
      for( i=0,j=outfree ; i<infree ; i++,j++ ) {
        double tmp ;
        hddm_vec[part].HDDM_VEC_U[i]
              = MPLUS( hddm_vec[part].HDDM_VEC_U[i],
                       MMULTI( malpha, hddm_vec[part].HDDM_VEC_P[j]) ) ;
        hddm_vec[part].HDDM_VEC_R[i]
              = MPLUS( hddm_vec[part].HDDM_VEC_R[i],
                       MMULTI( malpha, hddm_vec[part].HDDM_VEC_Q[j]) ) ;
        tmp = MABS( hddm_vec[part].HDDM_VEC_R[i] ) ;
        if( tmp > resi ) {
          resi = tmp ;
        }
      }
    }

    resi *= hddm_vec[0].r0 ;
    if( COM_GID.rootid == COM_ID.myprocid ){
      if( hddm_opt.hddm_log == NEED ) {
        Rfprintf( hddm_opt.fp, "%d %le\n", hddm_sw.loop, resi ) ;
      }
    }

    /* z(n+1) = M^-1 * r(n+1) */
    for( part=0 ; part<npart ; part++ ) {
      setPC( &(hddm_vec[part]) ) ;
    }

    /* beta(n) = (r(n+1), z(n+1)) / (r(n), z(n)) */
    rz = M_ZERO ;
    for( part=0 ; part<npart ; part++ ) {
    int    infree = hddm_vec[part].infree ;
      /*rz = __LOCAL_INNER__( infree, hddm_vec[part].HDDM_VEC_R,
                             hddm_vec[part].HDDM_VEC_Z ) ;*/
      /*rz = MINNER( infree, hddm_vec[part].HDDM_VEC_R,
                     hddm_vec[part].HDDM_VEC_Z ) ;*/
      /*rz = MBILINEAR( infree, hddm_vec[part].HDDM_VEC_R,
                        hddm_vec[part].HDDM_VEC_Z ) ;*/
      for( i=0 ; i<infree ; i++ ) {
        rz = MPLUS( rz, MMULTI(hddm_vec[part].HDDM_VEC_R[i],
                               hddm_vec[part].HDDM_VEC_Z[i]) ) ;
      }
    }
    if( (MABS(rz) < NEARLY_ZERO)
        && (MABS(hddm_vec[0].HDDM_VEC_RZ) < NEARLY_ZERO) ) {
      beta = M_ZERO ;
    } else {
      beta = MDIVIDE( rz, hddm_vec[0].HDDM_VEC_RZ ) ;
    }

    for( part=0 ; part<npart ; part++ ) {
      hddm_vec[part].HDDM_VEC_RZ = rz ;
    }

    /* p(n+1) = z(n+1) + beta * p(n) */
    for( part=0 ; part<npart ; part++ ) {
      int    infree = hddm_vec[part].infree ;
      int    outfree = hddm_vec[part].outfree ;
      memcpy( &(hddm_vec[part].HDDM_VEC_Q[outfree]),
              &(hddm_vec[part].HDDM_VEC_P[outfree]), sizeof(MTYPE)*infree ) ;
      for( i=0,j=outfree ; i<infree ; i++, j++ ) {
        hddm_vec[part].HDDM_VEC_P[j]
              = MPLUS( hddm_vec[part].HDDM_VEC_Z[i],
                       MMULTI(beta, hddm_vec[part].HDDM_VEC_P[j]) ) ;
      }
    }


    if( resi < hddm_opt.hddm_eps ) {
      hddm_sw.status = __HDDM_SW_LAST__ ;
      for( part=0 ; part<npart ; part++ ) {
        memcpy( &(hddm_vec[part].HDDM_VEC_P[hddm_vec[part].outfree]),
                hddm_vec[part].HDDM_VEC_U,
                sizeof(MTYPE)*hddm_vec[part].infree ) ;
      }
    } else {
      hddm_sw.status = __HDDM_SW_ITERATION__ ;
    }
  }

  for( part=0 ; part<npart ; part++ ) {
    for( i=part+1 ; i<npart ; i++ ) {
      int    size = oop[part][i].n_mofree ;
      for( j=0 ; j<size ; j++ ) {
        int    ii = oop[i][part].ibid[j] ;
        int    jj = oop[part][i].ibid[j] ;
        hddm_vec[i].HDDM_VEC_P[ii] = hddm_vec[part].HDDM_VEC_P[jj] ;
      }
    }
  }
  for( part=0 ; part<npart ; part++ ) {
    int    ii = hddm_vec[part].tfree ;
    for( j=0 ; j<ii ; j++ ) {
      hddm_vec[part].HDDM_VEC_Q[j] = M_ZERO ;
    }
  }

  __HDDM_SOLVER_ITERATION_S_FREE_OOP__( ooop ) ;
}

/********************************************************************/

void __HDDM_SOLVER_ITERATION_S_GATHER__( OPinfo **oop, HDDM_VEC *hddm_vec )
{
  /*+ Function: gather q +*/
  int    i, j ;

  int    part ;
  int    npart = COM_GID.nparent ;


  for( part=0 ; part<npart ; part++ ) {
    for( i=part+1 ; i<npart ; i++ ) {
      int    size = oop[part][i].n_mofree ;
      for( j=0 ; j<size ; j++ ) {
        int    ii = oop[part][i].ibid[j] ;
        int    jj = oop[i][part].ibid[j] ;
        hddm_vec[part].HDDM_VEC_Q[ii] = MPLUS( hddm_vec[part].HDDM_VEC_Q[ii],
                                               hddm_vec[i].HDDM_VEC_Q[jj] ) ;
      }
    }
  }
}

/***********************************************************************/

void __HDDM_SOLVER_ITERATION_S_FREE_OOP__( OPinfo ***ooop )
{
  int    part ;
  int    npart = COM_GID.nparent ;


  for( part=0 ; part<npart ; part++ ) {
    Free( sizeof(OPinfo)*npart, (*ooop)[part] ) ;
  }
  Free( sizeof(OPinfo*)*npart, (*ooop) ) ;
}
