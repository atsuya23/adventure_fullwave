/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "make_MESH.h"



void make_MESH( MESH *mesh, OPTIONS options )
{
  /*+ Function: make structure MESH +*/
  char   str[128] ;


  make_MESH_check_flag( mesh->nel, mesh->flag, mesh->mtrl, options ) ;


  switch( options.field ) {
    /* magnetic analysis */
    case FIELD_ELECTROMAGNETIC :
      make_MESH_FW( mesh, options ) ;
      break ;


    default :
      sprintf( str, "%d", options.field ) ;
      emessage( 152, __ABORT_LIBFEM__, str ) ;
  }
}

/********************************************************************/

void make_MESH_check_flag( int nel, int *flag, MTRL mtrl,
                           OPTIONS options )
{
  /*+ Function: check flag +*/
  int    nflag = 0 ;
  int*   flag_i ;


  make_MESH_check_flag_1st( nel, flag, &nflag, &flag_i ) ;
  sort_i_bubble_sort( nflag, flag_i ) ;
  make_MESH_check_flag_2nd( nflag, flag_i, mtrl, options ) ;
  Free( sizeof(int)*nflag, flag_i ) ;
}

/********************************************************************/

void make_MESH_check_flag_1st( int nel, int *flag,
                               int *nflag, int **flag_i )
{
  /*+ Function: check flag 1st step +*/
  int    i, j ;


  /* check nflag & *flag_i */
  for( i=0 ; i<nel ; i++ ) {
    int    ii = 0 ;
    for( j=0 ; j<(*nflag) ; j++ ) {
      if( (*flag_i)[j] == flag[i] ) {
        ii++ ;
        break ;
      }
    }
    if( ii == 0 ) {
      (*flag_i) = (int *)Realloc( (*flag_i), sizeof(int),
                                  (*nflag), (*nflag)+1 ) ;
      (*flag_i)[(*nflag)] = flag[i] ;
      (*nflag)++ ;
    }
  }
}

/********************************************************************/

void make_MESH_check_flag_2nd( int nflag, int *flag_i, MTRL mtrl,
                               OPTIONS options )
{
  /*+ Function: check flag 2nd step +*/
  int    i, j ;

  char   str[128] ;


  if( nflag != mtrl.nflag ) {
    sprintf( str, "nflag %d", nflag ) ;
    emessage( 2000, __ABORT_LIBFEM__, str ) ;
  } else {
    for( i=0 ; i<nflag ; i++ ) {
      if( flag_i[i] != mtrl.flag_i[i] ) {
        sprintf( str, "flag_i[%d] %d", i, flag_i[i] ) ;
        emessage( 2000, __ABORT_LIBFEM__, str ) ;
      }
    }
  }


  /** Magnetic Field **/
  if( options.field == FIELD_ELECTROMAGNETIC ) {
    /* check nantenna & antenna_def.flag */
    if( mtrl.nantenna == 0 ) {
      sprintf( str, "number of antenna 0" ) ;
      emessage( 2000, __ABORT_LIBFEM__, str ) ;
    } else {
      for( i=0 ; i<mtrl.nantenna ; i++ ) {
        int    ii = 0 ;
        for( j=0 ; j<nflag ; j++ ) {
          if( flag_i[j] == mtrl.antenna_def[i].flag ) {
            ii++ ;
            break ;
          }
        }
        if( ii == 0 ) {
          sprintf( str, "antenna_def[%d].flag %d", i, mtrl.antenna_def[i].flag ) ;
          emessage( 2000, __ABORT_LIBFEM__, str ) ;
        }
      }
    }

    /* check nantenna_omega & antenna_omega */
    if( options.analysis == ANALYSIS_FULL_WAVECURRENT ) {
      if( mtrl.nantenna_omega != 1 ) {
        sprintf( str, "nantenna_omega %d", mtrl.nantenna_omega ) ;
        emessage( 2000, __ABORT_LIBFEM__, str ) ;
      } else {
        if( mtrl.antenna_omega <= 0.0 ) {
          sprintf( str, "antenna_omega is unsuitable \"%lg\"",
                   mtrl.antenna_omega ) ;
          emessage( 2000, __ABORT_LIBFEM__, str ) ;
        }
      }
    }

    /* check nR & *R_i */
    if( options.analysis == ANALYSIS_FULL_WAVECURRENT ) {
      if( mtrl.nR != 0 ) {
        for( i=0 ; i<mtrl.nR ; i++ ) {
          int    ii = 0 ;
          for( j=0 ; j<nflag ; j++ ) {
            if( flag_i[j] == mtrl.R_i[i] ) {
              ii++ ;
              break ;
            }
          }
          if( ii == 0 ) {
            sprintf( str, "R_i[%d] %d", i, mtrl.R_i[i] ) ;
            emessage( 2000, __ABORT_LIBFEM__, str ) ;
          }
        }
      }
    }
  }
}

/********************************************************************/

void make_MESH_FW( MESH *mesh, OPTIONS options )
{
  /*+ Function: make structure MESH for Magnetic Field +*/
  if( !strcmp(mesh->elem_type, ELEM_TYPE_3DQT) ) {
    make_MESH_FW_TetQ( mesh, options ) ;
  } else {
    emessage( 126, __ABORT_LIBFEM__, mesh->elem_type ) ;
  }
}

/********************************************************************/

void make_MESH_FW_TetQ( MESH *mesh, OPTIONS options )
{
  /*+ Function: make structure MESH for Magnetic Field
                                    with 3DQuadraticTetrhedron +*/
  char   str[128] ;

  int    nel = mesh->nel ;
  int    nnd = mesh->nnd ;
  int    ap_elem = mesh->ap_elem ;
  int    nd_elem = mesh->nd_elem ;
  int    dimension = mesh->dimension ;
  int    nap ;
  int    nmp = 0 ;
  int    nphi = 0 ;
  int*   nop = mesh->nop ;
  int*   flag = mesh->flag ;
  double* crd = mesh->crd ;
  int*   ap ;
  int*   apid2gnid ;

  DirichletBC dbc_Axn0_tmp ;


  make_MESH_make_ap( &nap, &ap, nel, nnd, nop, ap_elem, nd_elem ) ;
  make_MESH_change_crd( nnd, dimension, &crd, nap, ap ) ;
  nmp = nnd - nap ;
  if( options.work == WORK_POST_FEM_CALC ) {
    if( nmp != mesh->nmp ) {
      sprintf( str, "%d", nmp ) ;
      emessage( 123, __ABORT_LIBFEM__, str ) ;
    }
  }
 
  if( options.work == WORK_POST_FEM_CALC ) {
    if( nphi != mesh->nphi ) {
      sprintf( str, "%d", nphi ) ;
      emessage( 140, __ABORT_LIBFEM__, str ) ;
    }
  }
  make_MESH_make_apid2gnid( &apid2gnid, nnd, nap, ap ) ;
  make_MESH_make_mpid2gnid( &(mesh->mpid2gnid), nnd, nap, nmp,
                             nphi, ap, mesh->apid2phiid, apid2gnid ) ;
  make_MESH_change_dbc_Axn0_1st( &dbc_Axn0_tmp, &(mesh->dbc_Axn0),
                                 ap ) ;
  make_MESH_make_ANTENNA( &(mesh->antenna), nel, nop, flag, ap_elem, nd_elem,
                       nap, ap, dbc_Axn0_tmp, apid2gnid, mesh->mtrl ) ;
  make_MESH_change_ap( ap, nnd ) ;
  make_MESH_change_nop( nel, nd_elem, nop, ap, mesh->elem_type, options ) ;
  make_MESH_change_dbc_Axn0_2nd( &dbc_Axn0_tmp, &(mesh->dbc_Axn0),
                                 ap, mesh->apid2phiid, &(mesh->nbc_A),
                                 options ) ;


  Free( sizeof(int)*nnd, ap ) ;
  mesh->nnd = nap ;
  mesh->nap = nap ;
  mesh->nmp = nmp ;
  mesh->nphi = nphi ;
  mesh->nop = nop ;
  mesh->crd = crd ;
  mesh->apid2gnid = apid2gnid ;

}

/********************************************************************/

void make_MESH_make_ap( int *nap, int **ap,
                        int nel, int nnd, int *nop,
                        int ap_elem, int nd_elem )
{
  /*+ Function: make nap & ap +*/
  int    i ;


  if( ap_elem == nd_elem ) {
    (*ap) = (int *)Malloc( sizeof(int), nnd ) ;
    for( i=0 ; i<nnd ; i++ ) {
      (*ap)[i] = i ;
    }
    (*nap) = nnd ;
  } else {
    int    nd ;
    int    el ;
    (*ap) = (int *)Malloc( sizeof(int), nnd ) ;
    for( nd=0 ; nd<nnd ; nd++ ) {
      (*ap)[nd] = -1 ;
    }
    for( el=0 ; el<nel ; el++ ) {
      int     ii = el * nd_elem ;
      for( i=0 ; i<ap_elem ; i++,ii++ ) {
        (*ap)[nop[ii]] = 0 ;
      }
    }
    (*nap) = 0 ;
    for( nd=0 ; nd<nnd ;nd++ ) {
      if( (*ap)[nd] > -1 ) {
        (*ap)[nd] = (*nap) ;
        (*nap)++ ;
      }
    }
  }
}

/********************************************************************/

void make_MESH_change_crd( int nnd, int dimension, double **crd,
                           int nap, int *ap )
{
  /*+ Function: change crd +*/
  int     i, j, k ;

  int     nd ;
  double* tmp_crd ;


  tmp_crd = (double *)Malloc( sizeof(double), nap*dimension ) ;
  for( nd=0,j=0,k=0 ; nd<nnd ; nd++ ) {
    if( ap[nd] > -1 ) {
      for( i=0 ; i<dimension ; i++,j++,k++ ) {
        tmp_crd[j] = (*crd)[k] ;
      }
    } else {
      k += dimension ;
    }
  }
  Free( sizeof(double)*nnd*dimension, (*crd) ) ;
  *crd = tmp_crd ;
}

/********************************************************************/

void make_MESH_make_apid2phiid_with_Disp( int *nphi, int **apid2phiid,
                                          int nap, int nmp )
{
  /*+ Function: make nphi & apid2phiid
                for A-phi method with displacement current +*/
  int    nd ;


  (*apid2phiid) = (int *)Malloc( sizeof(int), nap ) ;
  (*nphi) = nap ;
  for( nd=0 ; nd<nap ;nd++ ) {
    (*apid2phiid)[nd] = nmp ;
    nmp++ ;
  }
}

/********************************************************************/

void make_MESH_make_apid2phiid( int *nphi, int **apid2phiid,
                                int nel, int nap, int nmp,
                                int *nop, int *flag, int *ap,
                                int ap_elem, int nd_elem,
                                MTRL mtrl )
{
  /*+ Function: make nphi & apid2phiid for A-phi method +*/
  int    i ;

  int    el ;
  int    nd ;


  (*apid2phiid) = (int *)Malloc( sizeof(int), nap ) ;
  for( nd=0 ; nd<nap ; nd++ ) {
    (*apid2phiid)[nd] = -1 ;
  }
  for( el=0 ; el<nel ; el++ ) {
    int    ii = el * nd_elem ;
    int    je = ap_elem ;
    for( i=0 ; i<mtrl.nR ; i++ ) {
      if( flag[el] == mtrl.R_i[i] ) {
        je = 0 ;
        break ;
      }
    }
    for( i=je ; i<ap_elem ; i++,ii++ ) {
      (*apid2phiid)[ap[nop[ii]]] = 0 ;
    }
  }
  (*nphi) = 0 ;
  for( nd=0 ; nd<nap ;nd++ ) {
    if( (*apid2phiid)[nd] > -1 ) {
      (*apid2phiid)[nd] = nmp ;
      nmp++ ;
      (*nphi)++ ;
    }
  }
}

/********************************************************************/

void make_MESH_make_apid2gnid( int **apid2gnid,
                               int nnd, int nap, int *ap )
{
  /*+ Function: make apid to gnid +*/
  int    i, j ;


  (*apid2gnid) = (int *)Malloc( sizeof(int), nap ) ;
  for( i=0,j=0 ; i<nnd ; i++ ) {
    if( ap[i] > -1 ) {
      (*apid2gnid)[j] = i ;
      j++ ;
    }
  }
}

/********************************************************************/

void make_MESH_make_mpid2gnid( int **mpid2gnid,
                               int nnd, int nap, int nmp, int nphi,
                               int *ap, int *apid2phiid, int *apid2gnid )
{
  /*+ Function: make mpid to gnid +*/
  int    i, j ;

  int    nf = nmp + nphi ;


  (*mpid2gnid) = (int *)Malloc( sizeof(int), nf ) ;
  for( i=0,j=0 ; i<nnd ; i++ ) {
    if( ap[i] < 0 ) {
      (*mpid2gnid)[j] = i ;
      j++ ;
    }
  }
  if( nphi != 0 ) {
    for( i=0 ; i<nap ; i++ ) {
      if( apid2phiid[i] > -1 ) {
        (*mpid2gnid)[j] = apid2gnid[i] ;
        j++ ;
      }
    }
  }
}

/********************************************************************/

void make_MESH_change_dbc_Axn0_1st( DirichletBC *dbc_Axn0_tmp,
                                    DirichletBC *dbc_Axn0,
                                    int *ap )
{
  /*+ Function: change DirichletBC for Axn=0 1st step +*/
  int    bc ;
  int    nbc_tmp ;


  for( bc=0,nbc_tmp=0 ; bc<dbc_Axn0->nbc ; bc++ ) {
    if( ap[dbc_Axn0->ibc[bc]] > -1 ) {
      nbc_tmp++ ;
    }
  }
  dbc_Axn0_tmp->nbc = nbc_tmp ;
  dbc_Axn0_tmp->ibc = (int *)Malloc( sizeof(int), nbc_tmp ) ;
  for( bc=0,nbc_tmp=0 ; bc<dbc_Axn0->nbc ; bc++ ) {
    if( ap[dbc_Axn0->ibc[bc]] > -1 ) {
      dbc_Axn0_tmp->ibc[nbc_tmp] = ap[dbc_Axn0->ibc[bc]] ;
      dbc_Axn0->ibc[bc] = -1 ;
      nbc_tmp++ ;
    }
  }
}

/********************************************************************/

void make_MESH_make_ANTENNA( ANTENNA **antenna,
                          int nel, int *nop, int *flag,
                          int ap_elem, int nd_elem,
                          int nap, int *ap, DirichletBC dbc_tmp,
                          int *apid2gnid, MTRL mtrl )
{
  /*+ Function: make struct ANTENNA +*/
  int    i ;


  (*antenna) = (ANTENNA *)Malloc( sizeof(ANTENNA), mtrl.nantenna ) ;


  for( i=0 ; i<mtrl.nantenna ; i++ ) {
    make_MESH_make_ANTENNA_make_enf( &((*antenna)[i]), nel, nop, flag,
                                  ap_elem, nd_elem, nap, ap, apid2gnid,
                                  mtrl.antenna_def[i].flag ) ;
    make_MESH_make_ANTENNA_change_bc( &((*antenna)[i]), dbc_tmp ) ;
  }
}

/********************************************************************/

void make_MESH_make_ANTENNA_make_enf( ANTENNA *antenna,
                                   int nel, int *nop, int *flag,
                                   int ap_elem, int nd_elem,
                                   int nap, int *ap,
                                   int *apid2gnid, int antenna_flag )
{
  /*+ Function: make struct ANTENNA +*/
  int    i ;

  int    el ;
  int    nf = 0 ;
  int*   enf = NULL ;


  init_ANTENNA( antenna ) ;


  /* count degrees of freedom & make enf */
  enf = (int *)Malloc( sizeof(int), nap ) ;
  for( i=0 ; i<nap ; i++ ) {
    enf[i] = -1 ;
  }
  for( el=0 ; el<nel ; el++ ) {
    if( flag[el] == antenna_flag ) {
      int    ii = el * nd_elem ;
      for( i=0 ; i<ap_elem ; i++,ii++ ) {
        enf[ap[nop[ii]]] = 0 ;
      }
    }
  }
  for( i=0 ; i<nap ; i++ ) {
    if( enf[i] > -1 ) {
      enf[i] = nf ;
      nf++ ;
    }
  }
  antenna->antenna = antenna_flag ;
  antenna->nf = nf ;
  antenna->enf = enf ;
  antenna->apid2gnid = apid2gnid ;
}

/********************************************************************/

void make_MESH_make_ANTENNA_change_bc( ANTENNA *antenna, DirichletBC dbc_tmp )
{
  /*+ Function: make struct ANTENNA +*/
  int    bc ;
  int    nbc = 0 ;
  int*   ibc = NULL ;
  int*   enf = antenna->enf ;


  antenna->dbc.need = NEED ;
  for( bc=0 ; bc<dbc_tmp.nbc ; bc++ ) {
    if( enf[dbc_tmp.ibc[bc]] > -1 ) {
      nbc++ ;
    }
  }
  if( nbc == 0 ) {
    nbc = 1 ;
    antenna->dbc.nbc = nbc ;
    ibc = (int *)Malloc( sizeof(int), nbc ) ;
    ibc[0] = 0 ;
  } else {
    antenna->dbc.nbc = nbc ;
    ibc = (int *)Malloc( sizeof(int), nbc ) ;
    nbc = 0 ;
    for( bc=0 ; bc<dbc_tmp.nbc ; bc++ ) {
      if( enf[dbc_tmp.ibc[bc]] > -1 ) {
        ibc[nbc] = enf[dbc_tmp.ibc[bc]] ;
        nbc++ ;
      }
    }
  }
  antenna->dbc.ibc = ibc ;
}

/********************************************************************/

void make_MESH_change_ap( int *ap, int nnd )
{
  /*+ Function: change ap +*/
  int    nd ;
  int    nmp ;


  nmp = 0 ;
  for( nd=0 ; nd<nnd ; nd++ ) {
    if( ap[nd] < 0 ) {
      ap[nd] = nmp ;
      nmp++ ;
    }
  }
}

/********************************************************************/

void make_MESH_change_nop( int nel, int nd_elem, int *nop,
                           int *ap, char *elem_type, OPTIONS options )
{
  /*+ Function: change ap +*/
  int    i, j, k ;

  int    el ;


  for( el=0,j=0 ; el<nel ; el++ ) {
    for( i=0 ; i<nd_elem ; i++,j++ ) {
      nop[j] = ap[nop[j]] ;
    }
  }


  if( (!strcmp(elem_type, ELEM_TYPE_3DQT))
      && (options.field == FIELD_ELECTROMAGNETIC) ) {
    i = 8 ;
    j = 9 ;
    for( el=0 ; el<nel ; el++ ) {
      k = nop[i] ;
      nop[i] = nop[j] ;
      nop[j] = k ;
      i += nd_elem ;
      j += nd_elem ;
    }
  }
}

/********************************************************************/

void make_MESH_change_dbc_Axn0_2nd( DirichletBC *dbc_Axn0_tmp,
                                    DirichletBC *dbc_Axn0,
                                    int *ap, int *apid2phiid, int *nbc_A,
                                    OPTIONS options )
{
  /*+ Function: change DirichletBC 2nd step +*/
  int    i ;

  int    bc ;
  int    nbc ;
  int    *ibc ;


  nbc = dbc_Axn0->nbc - dbc_Axn0_tmp->nbc ;
  ibc = (int *)Malloc( sizeof(int), nbc ) ;
  for( bc=0,i=0 ; bc<dbc_Axn0->nbc ; bc++ ) {
    if( dbc_Axn0->ibc[bc] > -1 ) {
      ibc[i] = ap[dbc_Axn0->ibc[bc]] ;
      i++ ;
    }
  }

  Free( sizeof(int)*dbc_Axn0->nbc, dbc_Axn0->ibc ) ;
  Free( sizeof(int)*dbc_Axn0_tmp->nbc, dbc_Axn0_tmp->ibc ) ;
  dbc_Axn0->nbc = nbc ;
  dbc_Axn0->ibc = ibc ;
}

/********************************************************************/


/********************************************************************/

void correct_Jo( int t_loop, int nel, int nap,
                 int *nop, double *crd, int *flag,
                 int ap_elem, int nd_elem, char *elem_type,
                 int dimension, ANTENNA *antenna, MTRL mtrl,
                 OPTIONS options )
{
  /*+ Function: correct Jo
                  t_loop == -1 : Full Wave +*/
  int    i, j ;
  char   fname[FILENAME_MAX] ;
  FILE*  fp_log = NULL ;

  int    time_step = t_loop ;
  int    nantenna = mtrl.nantenna ;


  options.solvopt.solver = options.solvopt.coJo_sol ;
  options.solvopt.solver_pc = options.solvopt.coJo_sol_pc ;
  options.solvopt.solver_eps = options.solvopt.coJo_sol_eps ;
  options.solvopt.solver_div = options.solvopt.coJo_sol_div ;
  options.solvopt.solver_ic_shift = options.solvopt.coJo_sol_ic_shift ;
  options.solvopt.solver_log = options.solvopt.coJo_sol_log ;

  /*
  if( t_loop < -1 ) {
    emessage( 2500, __ABORT_LIBFEM__, NULL ) ;
  } else if( t_loop == -1 ) {
    time_step = -2 ;
  } else if( t_loop == 0 ) {
    time_step = 0 ;
  }
  */

  if( options.solvopt.solver_log == NEED ) {
    make_calclog_filename( fname, options.datasw, -1, time_step ) ;
    fp_log = Fopen( fname, "w" ) ;
    Fprintf( fp_log, "Correction Jo\n" ) ;
    Fprintf( fp_log, "  Number of antenna = %d\n", nantenna ) ;
    if( t_loop == -1 ) {
      Fprintf( fp_log, "Real part\n" ) ;
    }
  }
  for( i=0 ; i<nantenna ; i++ ) {
    int    el ;
    int    nf = antenna[i].nf ;

    int    nbc = antenna[i].dbc.nbc ;
    double *vbc ;

    TMP_AIJ tmp_aij ;


    if( mtrl.antenna_def[i].mode == MTRL_ANTENNA_RF_NUMBER ) {
      make_Jodata_filename( fname, options.datasw, mtrl.antenna_def[i].fname,
                            t_loop ) ;
      read_Jodata_file( fname, nap, dimension, &(antenna[i]), mtrl ) ;
    } else if( mtrl.antenna_def[i].mode == MTRL_ANTENNA_MD_NUMBER ) {
      make_Jo_from_definition( mtrl.antenna_def[i], &(antenna[i]),
                               nap, dimension, crd, -1 ) ;
    } else {
      char   str[128] ;
      sprintf( str, "%d for antenna", mtrl.antenna_def[i].mode ) ;
      emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
    }


    mymt_RS_AIJ_Init( nf, &tmp_aij, ap_elem ) ;
    for( el=0 ; el<nel ; el++ ) {
      tet_MakeElement_correct_Jo( i, el, nop, crd, antenna[i].enf,
                                  flag[el], mtrl, antenna[i].Jo, &tmp_aij ) ;
    }


    vbc = (double *)Calloc( sizeof(double), nbc ) ;
    mymt_RS_AIJ_SetBC( &tmp_aij, nbc, antenna[i].dbc.ibc, vbc ) ;
    Free( sizeof(double)*nbc, vbc ) ;


    mymt_RS_AIJ_Assem( &(antenna[i].aij_mat),  &tmp_aij ) ;


    antenna[i].Ihr = (double *)Calloc( sizeof(double), nf ) ;
    antenna[i].aij_mat.x_d = antenna[i].Ihr ;


    if( options.solvopt.solver_log == NEED ) {
      Fprintf( fp_log, "  antenna %d\n", i ) ;
    }
    mymt_RS_AIJ_Solver_Judge(
         mymt_RS_AIJ_Solver_CG(&(antenna[i].aij_mat), options.solvopt, fp_log) ) ;
  }


  if( t_loop == -1 ) {
    if( options.solvopt.solver_log == NEED ) {
      Fprintf( fp_log, "\n" ) ;
      Fprintf( fp_log, "Imaginary part\n" ) ;
    }


    for( i=0 ; i<nantenna ; i++ ) {
      int    el ;
      int    nf = antenna[i].nf ;

      int    nbc = antenna[i].dbc.nbc ;


      if( mtrl.antenna_def[i].mode == MTRL_ANTENNA_RF_NUMBER ) {
        antenna[i].Jor = antenna[i].Jo ;
        make_Jodata_filename( fname, options.datasw, mtrl.antenna_def[i].fname,
                              -2 ) ;
        read_Jodata_file( fname, nap, dimension, &(antenna[i]), mtrl ) ;
      } else if( mtrl.antenna_def[i].mode == MTRL_ANTENNA_MD_NUMBER ) {
        make_Jo_from_definition( mtrl.antenna_def[i], &(antenna[i]),
                                 nap, dimension, crd, -2 ) ;
      } else {
        char   str[128] ;
        sprintf( str, "%d for antenna", mtrl.antenna_def[i].mode ) ;
        emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
      }
      /*{
        int ii,jj,kk;
        for(ii=0,jj=0,kk=0;ii<nap;ii++) {
          if(antenna[i].enf[ii]>-1) {
            fprintf(stdout,"%5d  % le % le % le\n",ii,crd[ii*3],crd[ii*3+1],crd[ii*3+2]);
            fprintf(stdout,"       % le % le % le  % le % le % le\n",antenna[i].Jor[jj*3],antenna[i].Jor[jj*3+1],antenna[i].Jor[jj*3+2],antenna[i].Jo[kk*3],antenna[i].Jo[kk*3+1],antenna[i].Jo[kk*3+2]);
            jj++;kk++;
	  }
	}
      }*/

      for( j=0 ; j<nf ; j++ ) {
        antenna[i].aij_mat.b_d[j] = 0.0 ;
      }
      for( el=0 ; el<nel ; el++ ) {
        tet_MakeElement_correct_Jo_b( i, el, nop, crd, antenna[i].enf,
                                      flag[el], mtrl, antenna[i].Jo,
                                      antenna[i].aij_mat.b_d ) ;
      }


      for( j=0 ; j<nbc ; j++ ) {
        antenna[i].aij_mat.b_d[antenna[i].dbc.ibc[j]] = 0.0 ;
      }


      antenna[i].Ihi = (double *)Calloc( sizeof(double), nf ) ;
      antenna[i].aij_mat.x_d = antenna[i].Ihi ;


      if( options.solvopt.solver_log == NEED ) {
        Fprintf( fp_log, "  antenna %d\n", i ) ;
      }
      mymt_RS_AIJ_Solver_Judge(
         mymt_RS_AIJ_Solver_CG_nmpcm(&(antenna[i].aij_mat),
                                     options.solvopt, fp_log) ) ;
    }
  }


  if( options.solvopt.solver_log == NEED ) {
    Fclose( fp_log ) ;
  }
  for( i=0 ; i<nantenna ; i++ ) {
    mymt_RS_AIJ_Free( &(antenna[i].aij_mat) ) ;
  }
}
