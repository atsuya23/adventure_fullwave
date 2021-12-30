void __HDDM_SOLVER_PARENT_LOOP_1ST__( PartMesh *pm,
                                      OPTIONS options,
                                      HDDM_Opt hddm_opt,
                                      SolvOpt solvopt,
                                      HDDM_VEC *hddm_vec,
                                      HDDM_FUNC hddm_func,
                                      FILE *fp_log )
{
  /*+ Function: 1st hddm loop for Parent +*/
  int    dom ;
  int    ndomain = pm[0].ndomain ;

  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "Loop %d\n", hddm_sw.loop ) ;
  }
  for( dom=0 ; dom<ndomain ; dom++ ) {

    HDDM_DOM_VEC hddm_dom_vec ;
    if( solvopt.solver_log == NEED ) {
      Pfprintf( fp_log, "\n" ) ;
      Pfprintf( fp_log, "\n" ) ;
      Pfprintf( fp_log, "Part %d, Dom %d\n", pm[0].partid, dom ) ;
    }

    hddm_func.init_bc( &(pm[0].dm[dom]) ) ;

    hddm_func.make_TMP_AIJ( &(pm[0].dm[dom]), pm[0].mtrl ) ;

    init_HDDM_DOM_VEC( &hddm_dom_vec ) ;
    hddm_func.solve_dom_1st( &(pm[0].dm[dom]), hddm_opt, solvopt,
                             hddm_func.dom_solver, &hddm_dom_vec,
                             hddm_func.makePC, fp_log ) ;

    hddm_func.COM_res_sp_1st( &(hddm_vec[0]), hddm_dom_vec,
                              pm[0].dm[dom], hddm_opt ) ;
    free_HDDM_DOM_VEC( &hddm_dom_vec ) ;
  }
  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "\n" ) ;
    Pfprintf( fp_log, "\n" ) ;
  }

  hddm_func.iteration_ph_1st( pm[0], &(hddm_vec[0]), hddm_opt, hddm_func.setPC,
                              hddm_func.gatherPC_ph ) ;
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
  int    dom ;
  int    ndomain = pm[0].ndomain ;


  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "Loop %d\n", hddm_sw.loop ) ;
  }
  for( dom=0 ; dom<ndomain ; dom++ ) {
    HDDM_DOM_VEC hddm_dom_vec ;
    if( solvopt.solver_log == NEED ) {
      Pfprintf( fp_log, "\n" ) ;
      Pfprintf( fp_log, "\n" ) ;
      Pfprintf( fp_log, "Part %d, Dom %d\n", pm[0].partid, dom ) ;
    }
    hddm_func.renewal_bc_sp( &(pm[0].dm[dom]), hddm_vec[0] ) ;
    init_HDDM_DOM_VEC( &hddm_dom_vec ) ;
    hddm_func.solve_dom( &(pm[0].dm[dom]), solvopt,
                         hddm_func.dom_solver_nmpcm, &hddm_dom_vec,
                         fp_log ) ;
    hddm_func.COM_res_sp( &(hddm_vec[0]), hddm_dom_vec,
                          pm[0].dm[dom], hddm_opt ) ;
    free_HDDM_DOM_VEC( &hddm_dom_vec ) ;
  }
  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "\n" ) ;
    Pfprintf( fp_log, "\n" ) ;
  }


  hddm_func.iteration_ph( pm[0], &(hddm_vec[0]), hddm_opt,
                          hddm_func.setPC ) ;
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
  /*+ Function: hddm loop for Parent +*/
  int    dom ;
  int    ndomain = pm[0].ndomain ;


  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "Last step\n" ) ;
  }
  for( dom=0 ; dom<ndomain ; dom++ ) {
    HDDM_DOM_VEC hddm_dom_vec ;
    if( solvopt.solver_log == NEED ) {
      Pfprintf( fp_log, "\n" ) ;
      Pfprintf( fp_log, "\n" ) ;
      Pfprintf( fp_log, "Part %d, Dom %d\n", pm[0].partid, dom ) ;
    }
    hddm_func.renewal_bc_sp( &(pm[0].dm[dom]), hddm_vec[0] ) ;
    init_HDDM_DOM_VEC( &hddm_dom_vec ) ;
    hddm_func.solve_dom_last( &(pm[0].dm[dom]), solvopt,
                              hddm_func.dom_solver_nmpcm, &hddm_dom_vec,
                              fp_log ) ;
    hddm_func.COM_res_sp( &(hddm_vec[0]), hddm_dom_vec,
                          pm[0].dm[dom], hddm_opt ) ;
    free_HDDM_DOM_VEC( &hddm_dom_vec ) ;
  }


  hddm_func.get_result_sp( &(pm[0]) ) ;
  hddm_func.post_sp( &(pm[0]) ) ;
}
