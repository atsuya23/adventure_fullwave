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
  int    i ;

  int    dom = 0 ;
  int    ndomain = pm[0].ndomain ;
  MTRL   mtrl = pm[0].mtrl ;

  int*   nodom = NULL ;
  int    nbreak = 0 ;


  if( COM_GID.rootid == COM_ID.myprocid ) {
    nodom = (int *)Calloc( sizeof(int), COM_GID.nparent ) ;
  }


  while( 1 ) {
    int    compid = -1 ;
    int    type = -1 ;
    int    comm_sig = __HDDM_COMM_SIG_NONE__ ;

    work_times.work += plusTime( &s_time ) ;
    COM_ReadInt( &comm_sig, 1, &compid, &type ) ;
    work_times.idle += plusTime( &s_time ) ;


    if( comm_sig == __HDDM_COMM_SIG_RECV_DOM__ ) {
      if( dom < ndomain ) {
        comm_sig = __HDDM_COMM_SIG_SEND_DOM__ ;
        work_times.work += plusTime( &s_time ) ;
        COM_WriteInt( &comm_sig, 1, compid, type ) ;
        send_DomMesh( &(pm[0].dm[dom]), mtrl, options, compid, type ) ;
        work_times.comm += plusTime( &s_time ) ;
        dom++ ;

        if( dom == ndomain ) {
          if( COM_GID.rootid == COM_ID.myprocid ) {
            nodom[0]++ ;
          } else {
            type = 6000 ;
            compid = COM_GID.rootid ;
            comm_sig = __HDDM_COMM_SIG_NOTHING_DOM__ ;
            work_times.work += plusTime( &s_time ) ;
            COM_WriteInt( &comm_sig, 1, compid, type ) ;
            work_times.comm += plusTime( &s_time ) ;
          }
        }
      } else {
        comm_sig = __HDDM_COMM_SIG_NOTHING_DOM__ ;
        work_times.work += plusTime( &s_time ) ;
        COM_WriteInt( &comm_sig, 1, compid, type ) ;
        work_times.comm += plusTime( &s_time ) ;
      }


    } else if( comm_sig == __HDDM_COMM_SIG_SEND_RES__ ) {
      int    domid = 0 ;
      comm_sig = __HDDM_COMM_SIG_RECV_RES__ ;
      work_times.work += plusTime( &s_time ) ;
      COM_WriteInt( &comm_sig, 1, compid, type ) ;
      COM_ReadInt( &domid, 1, &compid, &type ) ;
      hddm_func.COM_res_h_recv_1st( &(hddm_vec[0]), pm[0].dm[domid],
                                    hddm_opt, compid, type ) ;
      work_times.comm += plusTime( &s_time ) ;


    } else if( comm_sig == __HDDM_COMM_SIG_NOTHING_DOM__ ) {
      nodom[compid]++ ;


    } else if( comm_sig == __HDDM_COMM_SIG_CHANGE_PARENT__ ) {
      int    current = 0 ;
      int    next = 0 ;
      type = 7000 ;
      work_times.work += plusTime( &s_time ) ;
      COM_ReadInt( &current, 1, &compid, &type ) ;
      work_times.comm += plusTime( &s_time ) ;
      next = __HDDM_SOLVER_CHANGE_PARENT_H__( nodom, current ) ;
      if( next > -1 ) {
        work_times.work += plusTime( &s_time ) ;
        COM_WriteInt( &next, 1, compid, type ) ;
        work_times.comm += plusTime( &s_time ) ;
      } else {
        comm_sig = __HDDM_COMM_SIG_BREAK__ ;
        nbreak++ ;
        work_times.work += plusTime( &s_time ) ;
        COM_WriteInt( &comm_sig, 1, compid, type ) ;
        if( nbreak == COM_GID.nchild ) {
          for( i=1 ; i<COM_GID.nparent ; i++ ) {
            COM_WriteInt( &comm_sig, 1, i, type ) ;
          }
          work_times.comm += plusTime( &s_time ) ;
          break ;
        }
        work_times.comm += plusTime( &s_time ) ;
      }


    } else if( comm_sig == __HDDM_COMM_SIG_BREAK__ ) {
      break ;


    } else {
      char   str[128] ;
      sprintf( str, "%d", comm_sig ) ;
      emessage( 3100, __ABORT_LIBHDDM__, str ) ;
    }
  }


  if( COM_GID.rootid == COM_ID.myprocid ) {
    Free( sizeof(int)*COM_GID.nparent, nodom ) ;
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
  int    dom = 0 ;
  int    ndomain = pm[0].ndomain ;


  while( 1 ) {
    int    compid = -1 ;
    int    type = -1 ;
    int    comm_sig = __HDDM_COMM_SIG_NONE__ ;

    work_times.work += plusTime( &s_time ) ;
    COM_ReadInt( &comm_sig, 1, &compid, &type ) ;
    work_times.idle += plusTime( &s_time ) ;


    if( comm_sig == __HDDM_COMM_SIG_RECV_DOM__ ) {
      int    domid = 0 ;
      comm_sig = __HDDM_COMM_SIG_SEND_DOM__ ;
      work_times.work += plusTime( &s_time ) ;
      COM_WriteInt( &comm_sig, 1, compid, type ) ;
      COM_ReadInt( &domid, 1, &compid, &type ) ;
      work_times.comm += plusTime( &s_time ) ;
      hddm_func.renewal_bc_h_send_dom( pm[0].dm[domid], hddm_vec[0],
                                       compid, type ) ;


    } else if( comm_sig == __HDDM_COMM_SIG_SEND_RES__ ) {
      int    domid = 0 ;
      comm_sig = __HDDM_COMM_SIG_RECV_RES__ ;
      work_times.work += plusTime( &s_time ) ;
      COM_WriteInt( &comm_sig, 1, compid, type ) ;
      COM_ReadInt( &domid, 1, &compid, &type ) ;
      hddm_func.COM_res_h_recv( &(hddm_vec[0]), pm[0].dm[domid],
                                hddm_opt, compid, type ) ;
      work_times.comm += plusTime( &s_time ) ;
      dom++ ;
      if( dom == ndomain ) {
        break ;
      }


    } else {
      char   str[128] ;
      sprintf( str, "%d", comm_sig ) ;
      emessage( 3100, __ABORT_LIBHDDM__, str ) ;
    }
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
  int    dom = 0 ;
  int    ndomain = pm[0].ndomain ;


  while( 1 ) {
    int    compid = -1 ;
    int    type = -1 ;
    int    comm_sig = __HDDM_COMM_SIG_NONE__ ;

    work_times.work += plusTime( &s_time ) ;
    COM_ReadInt( &comm_sig, 1, &compid, &type ) ;
    work_times.idle += plusTime( &s_time ) ;


    if( comm_sig == __HDDM_COMM_SIG_RECV_DOM__ ) {
      int    domid = 0 ;
      comm_sig = __HDDM_COMM_SIG_SEND_DOM__ ;
      work_times.work += plusTime( &s_time ) ;
      COM_WriteInt( &comm_sig, 1, compid, type ) ;
      COM_ReadInt( &domid, 1, &compid, &type ) ;
      work_times.comm += plusTime( &s_time ) ;
      hddm_func.renewal_bc_h_send_dom( pm[0].dm[domid], hddm_vec[0],
                                       compid, type ) ;


    } else if( comm_sig == __HDDM_COMM_SIG_SEND_RES__ ) {
      int    domid = 0 ;
      comm_sig = __HDDM_COMM_SIG_RECV_RES__ ;
      work_times.work += plusTime( &s_time ) ;
      COM_WriteInt( &comm_sig, 1, compid, type ) ;
      COM_ReadInt( &domid, 1, &compid, &type ) ;
      hddm_func.COM_res_h_recv( &(hddm_vec[0]), pm[0].dm[domid],
                                hddm_opt, compid, type ) ;
      hddm_func.post_h_recv_res( &(pm[0].dm[domid]), compid, type ) ;
      work_times.comm += plusTime( &s_time ) ;
      dom++ ;
      if( dom == ndomain ) {
        break ;
      }


    } else {
      char   str[128] ;
      sprintf( str, "%d", comm_sig ) ;
      emessage( 3100, __ABORT_LIBHDDM__, str ) ;
    }
  }
}

/********************************************************************/

void __HDDM_SOLVER_CHILD_LOOP_1ST__( DomMesh **dm,
                                     MTRL mtrl,
                                     OPTIONS options,
                                     HDDM_Opt hddm_opt,
                                     SolvOpt solvopt,
                                     HDDM_FUNC hddm_func,
                                     FILE* fp_log )
{
  /*+ Function: hddm loop for Child +*/
  int    nparent = COM_GID.nparent ;
  int    parent = COM_GID.parid[COM_ID.myprocid % nparent] ;
  int    dom = 0 ;
  int    rootid = COM_GID.rootid ;


  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "Loop %d\n", hddm_sw.loop ) ;
  }
  while( 1 ) {
    int    type = 8000 ;
    int    comm_sig = __HDDM_COMM_SIG_RECV_DOM__ ;


    work_times.work += plusTime( &s_time ) ;
    COM_WriteInt( &comm_sig, 1, parent, type ) ;
    COM_ReadInt( &comm_sig, 1, &parent, &type ) ;
    work_times.comm += plusTime( &s_time ) ;


    if( comm_sig == __HDDM_COMM_SIG_SEND_DOM__ ) {
      (*dm) = (DomMesh *)Realloc( (*dm), sizeof(DomMesh), dom, dom+1 ) ;
      init_DomMesh( &((*dm)[dom]), options ) ;
      work_times.work += plusTime( &s_time ) ;
      recv_DomMesh( &((*dm)[dom]), mtrl, options, parent, type ) ;
      work_times.comm += plusTime( &s_time ) ;


    } else if( comm_sig == __HDDM_COMM_SIG_NOTHING_DOM__ ) {
      comm_sig = __HDDM_COMM_SIG_CHANGE_PARENT__ ;
      work_times.work += plusTime( &s_time ) ;
      COM_WriteInt( &comm_sig, 1, rootid, type ) ;
      type = 7000 ;
      COM_WriteInt( &parent, 1, rootid, type ) ;
      COM_ReadInt( &comm_sig, 1, &rootid, &type ) ;
      work_times.comm += plusTime( &s_time ) ;
      if( comm_sig == __HDDM_COMM_SIG_BREAK__ ) {
        break ;
      } else {
        parent = COM_GID.parid[comm_sig] ;
        continue ;
      }


    } else {
      char   str[128] ;
      sprintf( str, "%d", comm_sig ) ;
      emessage( 3100, __ABORT_LIBHDDM__, str ) ;
    }


    {
      HDDM_DOM_VEC hddm_dom_vec ;
      if( solvopt.solver_log == NEED ) {
        Pfprintf( fp_log, "\n" ) ;
        Pfprintf( fp_log, "\n" ) ;
        Pfprintf( fp_log, "Part %d, Dom %d\n", parent, dom ) ;
      }
      hddm_func.init_bc( &((*dm)[dom]) ) ;
      hddm_func.make_TMP_AIJ( &((*dm)[dom]), mtrl ) ;
      init_HDDM_DOM_VEC( &hddm_dom_vec ) ;
      hddm_func.solve_dom_1st( &((*dm)[dom]), hddm_opt, solvopt,
                               hddm_func.dom_solver, &hddm_dom_vec,
                               hddm_func.makePC, fp_log ) ;
      type = 9000 ;
      comm_sig = __HDDM_COMM_SIG_SEND_RES__ ;
      work_times.work += plusTime( &s_time ) ;
      COM_WriteInt( &comm_sig, 1, parent, type ) ;
      COM_ReadInt( &comm_sig, 1, &parent, &type ) ;
      if( comm_sig == __HDDM_COMM_SIG_RECV_RES__ ) {
        COM_WriteInt( &((*dm)[dom].domid), 1, parent, type ) ;
        __HDDM_SOLVER_COM_RES_H_SEND_RES_1ST__( hddm_dom_vec, hddm_opt,
                                                parent, type ) ;
      }
      work_times.comm += plusTime( &s_time ) ;
      free_HDDM_DOM_VEC( &hddm_dom_vec ) ;
    }


    dom++ ;
  }


  hddm_sw.ndomain = dom ;


  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "\n" ) ;
    Pfprintf( fp_log, "\n" ) ;
  }
}

/********************************************************************/

void __HDDM_SOLVER_CHILD_LOOP_RESTART__( DomMesh **dm,
                                         MTRL mtrl,
                                         OPTIONS options,
                                         HDDM_Opt hddm_opt,
                                         SolvOpt solvopt,
                                         HDDM_FUNC hddm_func,
                                         FILE* fp_log )
{
  /*+ Function: hddm loop for Child +*/
}

/********************************************************************/

void __HDDM_SOLVER_CHILD_LOOP__( DomMesh **dm,
                                 MTRL mtrl,
                                 OPTIONS options,
                                 HDDM_Opt hddm_opt,
                                 SolvOpt solvopt,
                                 HDDM_FUNC hddm_func,
                                 FILE* fp_log )
{
  /*+ Function: hddm loop for Child +*/
  int    dom = 0 ;


  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "Loop %d\n", hddm_sw.loop ) ;
  }
  while( 1 ) {
    int    parent = (*dm)[dom].partid ;
    int    type = 10000 ;
    int    comm_sig = __HDDM_COMM_SIG_RECV_DOM__ ;


    work_times.work += plusTime( &s_time ) ;
    COM_WriteInt( &comm_sig, 1, parent, type ) ;
    COM_ReadInt( &comm_sig, 1, &parent, &type ) ;
    work_times.comm += plusTime( &s_time ) ;


    if( comm_sig == __HDDM_COMM_SIG_SEND_DOM__ ) {
      COM_WriteInt( &((*dm)[dom].domid), 1, parent, type ) ;
      work_times.comm += plusTime( &s_time ) ;
      hddm_func.renewal_bc_h_recv_dom( &((*dm)[dom]), parent, type ) ;


    } else {
      char   str[128] ;
      sprintf( str, "%d", comm_sig ) ;
      emessage( 3100, __ABORT_LIBHDDM__, str ) ;
    }


    {
      HDDM_DOM_VEC hddm_dom_vec ;
      if( solvopt.solver_log == NEED ) {
        Pfprintf( fp_log, "\n" ) ;
        Pfprintf( fp_log, "\n" ) ;
        Pfprintf( fp_log, "Part %d, Dom %d\n", parent, dom ) ;
      }
      init_HDDM_DOM_VEC( &hddm_dom_vec ) ;
      hddm_func.solve_dom( &((*dm)[dom]), solvopt,
                           hddm_func.dom_solver_nmpcm, &hddm_dom_vec,
                           fp_log ) ;
      type = 11000 ;
      comm_sig = __HDDM_COMM_SIG_SEND_RES__ ;
      work_times.work += plusTime( &s_time ) ;
      COM_WriteInt( &comm_sig, 1, parent, type ) ;
      COM_ReadInt( &comm_sig, 1, &parent, &type ) ;
      if( comm_sig == __HDDM_COMM_SIG_RECV_RES__ ) {
        COM_WriteInt( &((*dm)[dom].domid), 1, parent, type ) ;
        __HDDM_SOLVER_COM_RES_H_SEND_RES__( hddm_dom_vec, hddm_opt,
                                            parent, type ) ;
      }
      work_times.comm += plusTime( &s_time ) ;
      free_HDDM_DOM_VEC( &hddm_dom_vec ) ;
    }


    dom++ ;
    if( dom == hddm_sw.ndomain ) {
      break ;
    }
  }


  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "\n" ) ;
    Pfprintf( fp_log, "\n" ) ;
  }
}

/********************************************************************/

void __HDDM_SOLVER_CHILD_LOOP_LAST__( DomMesh **dm,
                                      MTRL mtrl,
                                      OPTIONS options,
                                      HDDM_Opt hddm_opt,
                                      SolvOpt solvopt,
                                      HDDM_FUNC hddm_func,
                                      FILE* fp_log )
{
  /*+ Function: hddm loop for Child +*/
  int    dom = 0 ;


  if( solvopt.solver_log == NEED ) {
    Pfprintf( fp_log, "Last step\n" ) ;
  }
  while( 1 ) {
    int    parent = (*dm)[dom].partid ;
    int    type = 12000 ;
    int    comm_sig = __HDDM_COMM_SIG_RECV_DOM__ ;


    work_times.work += plusTime( &s_time ) ;
    COM_WriteInt( &comm_sig, 1, parent, type ) ;
    COM_ReadInt( &comm_sig, 1, &parent, &type ) ;
    work_times.comm += plusTime( &s_time ) ;


    if( comm_sig == __HDDM_COMM_SIG_SEND_DOM__ ) {
      COM_WriteInt( &((*dm)[dom].domid), 1, parent, type ) ;
      work_times.comm += plusTime( &s_time ) ;
      hddm_func.renewal_bc_h_recv_dom( &((*dm)[dom]), parent, type ) ;


    } else {
      char   str[128] ;
      sprintf( str, "%d", comm_sig ) ;
      emessage( 3100, __ABORT_LIBHDDM__, str ) ;
    }


    {
      HDDM_DOM_VEC hddm_dom_vec ;
      if( solvopt.solver_log == NEED ) {
        Pfprintf( fp_log, "\n" ) ;
        Pfprintf( fp_log, "\n" ) ;
        Pfprintf( fp_log, "Part %d, Dom %d\n", parent, dom ) ;
      }
      init_HDDM_DOM_VEC( &hddm_dom_vec ) ;
      hddm_func.solve_dom_last( &((*dm)[dom]), solvopt,
                                hddm_func.dom_solver_nmpcm, &hddm_dom_vec,
                                fp_log ) ;
      type = 13000 ;
      comm_sig = __HDDM_COMM_SIG_SEND_RES__ ;
      work_times.work += plusTime( &s_time ) ;
      COM_WriteInt( &comm_sig, 1, parent, type ) ;
      COM_ReadInt( &comm_sig, 1, &parent, &type ) ;
      if( comm_sig == __HDDM_COMM_SIG_RECV_RES__ ) {
        COM_WriteInt( &((*dm)[dom].domid), 1, parent, type ) ;
        __HDDM_SOLVER_COM_RES_H_SEND_RES__( hddm_dom_vec, hddm_opt,
                                            parent, type ) ;
      }
      work_times.comm += plusTime( &s_time ) ;
      free_HDDM_DOM_VEC( &hddm_dom_vec ) ;
      work_times.work += plusTime( &s_time ) ;
      hddm_func.post_h_send_res( &((*dm)[dom]), mtrl, parent, type ) ;
      work_times.comm += plusTime( &s_time ) ;
      free_DomMesh( &((*dm)[dom]), mtrl, options ) ;
    }



    dom++ ;
    if( dom == hddm_sw.ndomain ) {
      break ;
    }
  }


  Free( sizeof(DomMesh)*hddm_sw.ndomain, (*dm) ) ;
}

/********************************************************************/

int __HDDM_SOLVER_CHANGE_PARENT_H__( int *nodom, int current )
{
  /*+ Function: change parent +*/
  int    i ;

  int    nparent = COM_GID.nparent ;


  for( i=0 ; i<current ; i++ ) {
    if( nodom[i] == 0 ) {
      return i ;
    }
  }


  for( i=current+1 ; i<nparent ; i++ ) {
    if( nodom[i] == 0 ) {
      return i ;
    }
  }


  return -1 ;
}

/********************************************************************/

void __HDDM_SOLVER_COM_RES_H_RECV_RES_1ST__( HDDM_VEC *hddm_vec,
                                             int ninbc, Inbc *inbc,
                                             HDDM_Opt hddm_opt,
                                             int compid, int type )
{
  /*+ Function: recieve result of domain from Child +*/
  __HDDM_SOLVER_COM_RES_H_RECV_RES_FUNC__( hddm_vec->HDDM_VEC_Q,
                                           ninbc, inbc, compid, type ) ;


  if( hddm_opt.hddm_pc == HDDM_PC_DIAG ) {
    __HDDM_SOLVER_COM_RES_H_RECV_RES_FUNC__( hddm_vec->HDDM_VEC_PC,
                                             ninbc, inbc, compid, type ) ;
  }
}

/********************************************************************/

void __HDDM_SOLVER_COM_RES_H_SEND_RES_1ST__( HDDM_DOM_VEC hddm_dom_vec,
                                             HDDM_Opt hddm_opt,
                                             int compid, int type )
{
  /*+ Function: send result of domain to Parent +*/
  __HDDM_SOLVER_COM_RES_H_SEND_RES_FUNC__( hddm_dom_vec.nf,
                                           hddm_dom_vec.HDDM_DOM_VEC_VEC,
                                           compid, type ) ;


  if( hddm_opt.hddm_pc == HDDM_PC_DIAG ) {
    __HDDM_SOLVER_COM_RES_H_SEND_RES_FUNC__( hddm_dom_vec.pc_size,
                                             hddm_dom_vec.HDDM_DOM_VEC_PC,
                                             compid, type ) ;
  }
}

/********************************************************************/

void __HDDM_SOLVER_COM_RES_H_RECV_RES__( HDDM_VEC *hddm_vec,
                                         int ninbc, Inbc *inbc,
                                         HDDM_Opt hddm_opt,
                                         int compid, int type )
{
  /*+ Function: recieve result of domain from Child +*/
  __HDDM_SOLVER_COM_RES_H_RECV_RES_FUNC__( hddm_vec->HDDM_VEC_Q,
                                           ninbc, inbc, compid, type ) ;
}

/********************************************************************/

void __HDDM_SOLVER_COM_RES_H_SEND_RES__( HDDM_DOM_VEC hddm_dom_vec,
                                         HDDM_Opt hddm_opt,
                                         int compid, int type )
{
  /*+ Function: send result of domain to Parent +*/
  __HDDM_SOLVER_COM_RES_H_SEND_RES_FUNC__( hddm_dom_vec.nf,
                                           hddm_dom_vec.HDDM_DOM_VEC_VEC,
                                           compid, type ) ;
}

/********************************************************************/

void __HDDM_SOLVER_COM_RES_H_RECV_RES_FUNC__( MTYPE *hddm_vec,
                                              int ninbc, Inbc *inbc,
                                              int compid, int type )
{
  /*+ Function: recieve result of domain from Child +*/
  int    nf = 0 ;


  COM_ReadInt( &nf, 1, &compid, &type ) ;
  if( nf > 0 ) {
    int    i ;
    MTYPE* hddm_dom_vec = (MTYPE *)Malloc( sizeof(MTYPE), nf ) ;
    COM_ReadMTYPE( hddm_dom_vec, nf, &compid, &type ) ;
    for( i=0 ; i<ninbc ; i++ ) {
      int    ii = inbc[i].ln_part ;
      int    jj = inbc[i].nfid ;
      hddm_vec[ii] = MPLUS( hddm_vec[ii], hddm_dom_vec[jj] ) ;
    }
    Free( sizeof(MTYPE)*nf, hddm_dom_vec ) ;
  }
}

/********************************************************************/

void __HDDM_SOLVER_COM_RES_H_SEND_RES_FUNC__( int nf, MTYPE *hddm_dom_vec,
                                              int compid, int type )
{
  /*+ Function: send result of domain to Parent +*/
  COM_WriteInt( &nf, 1, compid, type ) ;
  if( nf > 0 ) {
    COM_WriteMTYPE( hddm_dom_vec, nf, compid, type ) ;
  }
}

/********************************************************************/

void __HDDM_SOLVER_RENEWAL_BC_H_SEND_DOM__( MTYPE *p,
                                            int ninbc, Inbc *inbc,
                                            int compid, int type )
{
  /*+ Function: send dom to Child +*/
  int    i ;

  MTYPE* vec = (MTYPE *)Malloc( sizeof(MTYPE), ninbc ) ;


  for( i=0 ; i<ninbc ; i++ ) {
    vec[i] = p[inbc[i].ln_part] ;
  }
  work_times.work += plusTime( &s_time ) ;
  COM_WriteMTYPE( vec, ninbc, compid, type ) ;
  work_times.comm += plusTime( &s_time ) ;
  Free( sizeof(MTYPE)*ninbc, vec ) ;
}

/********************************************************************/

void __HDDM_SOLVER_RENEWAL_BC_H_RECV_DOM__( int nbc, MTYPE *vbc,
                                            int ninbc,
                                            int compid, int type )
{
  /*+ Function: recieve dom from Parent +*/
  int    i, j ;

  MTYPE* vec = (MTYPE *)Malloc( sizeof(MTYPE), ninbc ) ;


  work_times.work += plusTime( &s_time ) ;
  COM_ReadMTYPE( vec, ninbc, &compid, &type ) ;
  work_times.comm += plusTime( &s_time ) ;
  for( i=nbc-ninbc,j=0 ; i<nbc ; i++,j++ ) {
    vbc[i] = vec[j] ;
  }
  Free( sizeof(MTYPE)*ninbc, vec ) ;
}
