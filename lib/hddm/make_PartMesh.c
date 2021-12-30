/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "make_PartMesh.h"

extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;

int    nstart = 0 ;
int    nend = 0 ;
int    nrepeat = 0 ;

HDDM_SW hddm_sw ;
WORK_TIMES work_times ;
double s_time ;


void hddm_time_Init( void )
{
  /*+ Function: Initialize time for HDDM +*/
  s_time = 0.0 ;
  work_times.work = 0.0 ;
  work_times.comm = 0.0 ;
  work_times.idle = 0.0 ;
}

/********************************************************************/

void hddm_time_Finalize( void )
{
  /*+ Function: Finalize time for HDDM +*/
  int    i ;

  double all_time = 100.0 / nowTime() ;


  Fprintf( stdout, "\n" ) ;
  Fprintf( stdout, "Efficiency\n" ) ;
  for( i=0 ; i<COM_ID.nproc ; i++ ) {
    if( i == COM_ID.myprocid ) {
      Pfprintf( stdout, "  CPU efficiency (%%) = %lg\n",
                work_times.work*all_time ) ;
      Pfprintf( stdout, "  Communication rate (%%) = %lg\n",
                work_times.comm*all_time ) ;
      Pfprintf( stdout, "    work time %lg\n", work_times.work ) ;
      Pfprintf( stdout, "    comm time %lg\n", work_times.comm ) ;
      Pfprintf( stdout, "    idle time %lg\n", work_times.idle ) ;
    }
    COM_Synchronize() ;
  }
}

/********************************************************************/

void hddm_Init( PartMesh **pm, OPTIONS options )
{
  /*+ Function: Initialize for HDDM +*/
  int    i, j ;
  char   fname[FILENAME_MAX] ;


  Fprintf( stdout, "\n" ) ;
  Fprintf( stdout, "Start reading data   time %g[s]\n", nowTime() ) ;
  if( COM_ID.myprocid < COM_GID.nparent ) {
    #ifndef __ADVFW_SINGLE_MODE__
      nstart = COM_GID.mygprocid ;
      nend = nstart + 1 ;
      nrepeat = 1 ;
    #else
      nstart = 0 ;
      nend = COM_GID.nparent ;
      nrepeat = COM_GID.nparent ;
    #endif
    (*pm) = (PartMesh *)Malloc( sizeof(PartMesh), nrepeat ) ;
    for( i=nstart,j=0 ; i<nend ; i++,j++ ) {
      /* read modeldata */
      init_PartMesh( &((*pm)[j]), options ) ;
      make_modeldata_filename( fname, options.datasw, i ) ;
      read_modeldata_file( fname, &((*pm)[j]), options ) ;
    }
  } else {
    nstart = 0 ;
    nend = 1 ;
    nrepeat = 1 ;
    (*pm) = (PartMesh *)Malloc( sizeof(PartMesh), nrepeat ) ;
    init_PartMesh( &((*pm)[0]), options ) ;
  }

  /* read mtrldat */
  #ifdef __ADVFW_SINGLE_MODE__
    make_mtrldat_filename( fname, options.datasw, 0 ) ;
    read_mtrldat_file( fname, &((*pm)[0].mtrl), options ) ;
    for( i=1 ; i<COM_GID.nparent ; i++ ) {
      Copy_MTRL( &((*pm)[i].mtrl), (*pm)[0].mtrl ) ;
    }
  #else
    if( COM_GID.rootid == COM_ID.myprocid ) {
      make_mtrldat_filename( fname, options.datasw, 0 ) ;
      read_mtrldat_file( fname, &((*pm)[0].mtrl), options ) ;
    }
    work_times.work += plusTime( &s_time ) ;
    Bcast_MTRL( &((*pm)[0].mtrl) ) ;
    work_times.comm += plusTime( &s_time ) ;
  #endif

  Fprintf( stdout, "Finish reading data   time %g[s]\n", nowTime() ) ;

  if( COM_ID.myprocid < COM_GID.nparent ) {
    make_PartMesh( (*pm), options ) ;
  } 
  work_times.work += plusTime( &s_time ) ;
  COM_Synchronize() ;
  work_times.idle += plusTime( &s_time ) ;
}

/********************************************************************/

void make_PartMesh( PartMesh *pm, OPTIONS options )
{
  /*+ Function: make structure PartMesh +*/
  char   str[128] ;

  make_PartMesh_check_flag( pm, options ) ;

  switch( options.field ) {
    /* magnetic analysis */
    case FIELD_ELECTROMAGNETIC :
      make_PartMesh_FW( pm, options ) ;
      break ;

    default :
      sprintf( str, "%d", options.field ) ;
      emessage( 152, __ABORT_LIBHDDM__, str ) ;
  }
}

/********************************************************************/

void make_PartMesh_check_flag( PartMesh *pm, OPTIONS options )
{
  /*+ Function: check flag for PartMesh +*/
  #ifdef __ADVFW_SINGLE_MODE__ /* s-mode */
    make_PartMesh_check_flag_s( pm, options ) ;
  #else
    #ifdef __ADVFW_PARALLEL_MODE__ /* p-mode */
      make_PartMesh_check_flag_p( pm[0], options ) ;
    #else /* h-mode */
      make_PartMesh_check_flag_h( pm[0], options ) ;
    #endif /* __ADVFW_PARALLEL_MODE__ */
  #endif /* __ADVFW_SINGLE_MODE__ */
}

/********************************************************************/

void make_PartMesh_change_innerbc_func( int *infree, int *outfree,
                                        OPinfo **op,
                                        int **ninbc, Inbc ***inbc,
                                        PartMesh *pm, OPTIONS options,
                                        void (*make_nf)( int  **nf,
                                                         DomMesh dm,
                                                         MTRL mtrl,
                                                         int kk ),
                                        int kk )
{
  /*+ Function: change inner B.C. function +*/
  int    i, k ;

  int    part ;

  Pinbc** pinbc = NULL ;
  int*    tfree = NULL ;
  int***  nf = NULL ;


  /* make Pinbc */
  tfree = (int *)Malloc( sizeof(int), nrepeat ) ;
  pinbc = (void *)Malloc( sizeof(Pinbc*), nrepeat ) ;
  for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
    tfree[k] = infree[k] + outfree[k] ;
    pinbc[k] = (Pinbc *)Malloc( sizeof(Pinbc), tfree[k] ) ;
    for( i=0 ; i<tfree[k] ; i++ ) {
      init_Pinbc( &(pinbc[k][i]) ) ;
    }
  }


  /* exclude Essential B.C. */
  make_PartMesh_change_innerbc_change_Pinbc_1st( pm, ninbc, inbc, pinbc ) ;


  /* make nf & add nf to Pinbc */
  nf = (void *)Malloc( sizeof(int**), nrepeat ) ;
  for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
    int    dom ;
    int    ndomain = pm[k].ndomain ;
    nf[k] = (void *)Malloc( sizeof(int*), ndomain ) ;
    for( dom=0 ; dom<ndomain ; dom++ ) {
      DomMesh* dm = &(pm[k].dm[dom]) ;
      make_nf( &(nf[k][dom]), *dm, pm[k].mtrl, kk ) ;
      
      make_PartMesh_change_innerbc_add_nf( pinbc[k], tfree[k],
                                           nf[k][dom], dm->nnd,
                                           ninbc[k][dom], inbc[k][dom] ) ;
    }
  }

  make_PartMesh_change_innerbc_change_Pinbc_2nd( pm, tfree, op, pinbc ) ;
  /* make bc_flag */
  make_PartMesh_change_innerbc_change_Pinbc_3rd( tfree, pinbc ) ;

  /* change infree & outfree */
  /* change charge part & local interface B.C. number in part */
  make_PartMesh_change_innerbc_change_Pinbc_4th( pm, tfree, infree,
                                                 outfree, pinbc ) ; 
 /* change local interface B.C. number in charge part */
  make_PartMesh_change_innerbc_change_Pinbc_5th( pm, tfree, op, pinbc ) ;


  for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
    int    partid = pm[k].partid ;
    int    dom ;
    int    ndomain = pm[k].ndomain ;

    for( dom=0 ; dom<ndomain ; dom++ ) {
      make_PartMesh_change_innerbc_change_Inbc( pinbc[k], tfree[k],
                                                nf[k][dom],
                                                &(ninbc[k][dom]),
                                                &(inbc[k][dom]) ) ;
      Free( sizeof(int)*pm[k].dm[dom].nnd, nf[k][dom] ) ;
    }
    Free( sizeof(int*)*ndomain, nf[k] ) ;

    make_PartMesh_change_innerbc_change_OPinfo( pinbc[k], tfree[k],
                                                infree[k], outfree[k],
                                                partid, &(op[k]) ) ;

    for( i=0 ; i<tfree[k] ; i++ ) {
      free_Pinbc( &(pinbc[k][i]) ) ;
    }
    Free( sizeof(Pinbc)*tfree[k], pinbc[k] ) ;
  }
  Free( sizeof(int)*nrepeat, tfree ) ;
  Free( sizeof(Pinbc*)*nrepeat, pinbc ) ;
  Free( sizeof(int**)*nrepeat, nf ) ;
}

/********************************************************************/

void make_PartMesh_change_innerbc_make_temp_struct( PartMesh *pm,
                                                    int **infree,
                                                    int **outfree,
                                                    OPinfo ***op,
                                                    int ***ninbc,
                                                    Inbc ****inbc )
{
  /*+ Function: change inner B.C. +*/
  /*+           make temporary OPinfo & Inbc +*/
  int    k ;

  int    part ;


  *infree = (int *)Malloc( sizeof(int), nrepeat ) ;
  *outfree = (int *)Malloc( sizeof(int), nrepeat ) ;
  *op = (void *)Malloc( sizeof(OPinfo*), nrepeat ) ;
  *ninbc = (void *)Malloc( sizeof(int*), nrepeat ) ;
  *inbc = (void *)Malloc( sizeof(Inbc**), nrepeat ) ;
  for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
    make_PartMesh_change_innerbc_make_temp_struct_sub( pm[k],
                                                       &((*infree)[k]),
                                                       &((*outfree)[k]),
                                                       &((*op)[k]),
                                                       &((*ninbc)[k]),
                                                       &((*inbc)[k]) ) ;
  }
}

/********************************************************************/

void make_PartMesh_change_innerbc_make_temp_struct_sub( PartMesh pm,
                                                        int *infree,
                                                        int *outfree,
                                                        OPinfo **op,
                                                        int **ninbc,
                                                        Inbc ***inbc )
{
  /*+ Function: change inner B.C. +*/
  /*+           make temporary OPinfo & Inbc +*/
  int    i ;

  int    part ;
  int    dom ;
  int    npart = COM_GID.nparent ;
  int    ndomain = pm.ndomain ;


  (*infree) = pm.infree ;
  (*outfree) = pm.outfree ;
  (*op) = (OPinfo *)Malloc( sizeof(OPinfo), npart ) ;
  for( part=0 ; part<npart ; part++ ) {
    Copy_OPinfo( &((*op)[part]), pm.op[part] ) ;
  }
  (*ninbc) = (int *)Malloc( sizeof(int), ndomain ) ;
  (*inbc) = (void *)Malloc( sizeof(Inbc*), ndomain ) ;
  for( dom=0 ; dom<ndomain ; dom++ ) {
    int    ii = pm.dm[dom].ninbc ;
    (*ninbc)[dom] = ii ;
    (*inbc)[dom] = (Inbc *)Malloc( sizeof(Inbc), ii ) ;
    for( i=0 ; i<ii ; i++ ) {
      Copy_Inbc( &((*inbc)[dom][i]), pm.dm[dom].inbc[i] ) ;
    }
  }
}

/********************************************************************/

void make_PartMesh_change_innerbc_free_temp_struct( PartMesh pm,
                                                    int *infree,
                                                    int *outfree,
                                                    OPinfo **op,
                                                    int **ninbc,
                                                    Inbc ***inbc )
{
  /*+ Function: change inner B.C. +*/
  /*+           free temporary OPinfo & Inbc +*/
  int    i ;

  int    part ;
  int    dom ;
  int    npart = COM_GID.nparent ;
  int    ndomain = pm.ndomain ;


  (*infree) = 0 ;
  (*outfree) = 0 ;
  for( part=0 ; part<npart ; part++ ) {
    free_OPinfo( &((*op)[part]) ) ;
  }
  Free( sizeof(OPinfo)*npart, (*op) ) ;
  for( dom=0 ; dom<ndomain ; dom++ ) {
    int    ii = (*ninbc)[dom] ;
    for( i=0 ; i<ii ; i++ ) {
      free_Inbc( &((*inbc)[dom][i]) ) ;
    }
    Free( sizeof(Inbc)*ii, (*inbc)[dom] ) ;
  }
  Free( sizeof(int)*ndomain, (*ninbc) ) ;
  Free( sizeof(Inbc*)*ndomain, (*inbc) ) ;
}

/********************************************************************/

void make_PartMesh_change_innerbc_add_nf( Pinbc *pinbc, int tfree,
                                          int *nf, int nnd,
                                          int ninbc, Inbc *inbc )
{
  /*+ Function: add nf to Pinbc for Magnetic field +*/
  int    i ;


  for( i=0 ; i<ninbc ; i++ ) {
    int    ii = inbc[i].ln_part ;
    if( pinbc[ii].bc_flag == __ADVFW_HDDM_BC_FLAG_ESSENTIAL_BC__ ) {
      pinbc[ii].nf = 0 ;
    } else {
      if( nf[inbc[i].snid] == __ADVFW_HDDM_CHANGE_INNER_BC_CHANGE__ ) {
        if( pinbc[ii].nf == __ADVFW_HDDM_CHANGE_INNER_BC_NOT_CHANGE__ ) {
          /* error */
        } else {
            pinbc[ii].nf++ ;
            pinbc[ii].charge_part = inbc[i].charge_part ;
            pinbc[ii].ln_chpar = inbc[i].ln_chpar ;
            pinbc[ii].ln_part = inbc[i].ln_part ;
        }
      } else if( nf[inbc[i].snid] == __ADVFW_HDDM_CHANGE_INNER_BC_NOT_CHANGE__ ) {
        pinbc[ii].nf = __ADVFW_HDDM_CHANGE_INNER_BC_NOT_CHANGE__ ;
        pinbc[ii].charge_part = inbc[i].charge_part ;
        pinbc[ii].ln_chpar = inbc[i].ln_chpar ;
        pinbc[ii].ln_part = inbc[i].ln_part ;
      } else {
        /* error */
      }
    }
   
  }
}

/********************************************************************/

void make_PartMesh_change_innerbc_change_Pinbc_1st( PartMesh *pm,
                                                    int **ninbc, Inbc ***inbc,
                                                    Pinbc **pinbc )
{
  /*+ Function: 1st step of changing Pinbc +*/
  /*+           exclude Essential B.C. +*/
  int    i, k ;

  int    part ;


  if( pm[0].dbc_Axn0.need == NEED ) {
    for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
      int    dom ;
      int    ndomain = pm[k].ndomain ;
      for( dom=0 ; dom<ndomain ; dom++ ) {
        DomMesh* dm = &(pm[k].dm[dom]) ;
        int    bc ;
        int    nbc = dm->dbc_Axn0.nbc ;
        int*   ibc = dm->dbc_Axn0.ibc ;
        for( bc=0 ; bc<nbc ; bc++ ) {
          for( i=0 ; i<ninbc[k][dom] ; i++ ) {
            if( ibc[bc] == inbc[k][dom][i].snid ) {
              pinbc[k][inbc[k][dom][i].ln_part].bc_flag =
                          __ADVFW_HDDM_BC_FLAG_ESSENTIAL_BC__ ;
            }
          }
        }
      }
    }
  }
}

/********************************************************************/

void make_PartMesh_change_innerbc_change_Pinbc_2nd( PartMesh *pm,
                                                    int *tfree,
                                                    OPinfo **op,
                                                    Pinbc **pinbc )
{
  /*+ Function: 2nd step of changing Pinbc +*/
  #ifdef __ADVFW_SINGLE_MODE__ /* s-mode */
    make_PartMesh_change_innerbc_change_Pinbc_2nd_s( pm, tfree, op,
                                                     pinbc ) ;
  #else
    #ifdef __ADVFW_PARALLEL_MODE__ /* p-mode */
      make_PartMesh_change_innerbc_change_Pinbc_2nd_p( pm[0], tfree[0],
                                                       op[0], pinbc[0] ) ;
    #else /* h-mode */
      make_PartMesh_change_innerbc_change_Pinbc_2nd_h( pm[0], tfree[0],
                                                       op[0], pinbc[0] ) ;
    #endif /* __ADVFW_PARALLEL_MODE__ */
  #endif /* __ADVFW_SINGLE_MODE__ */
}

/********************************************************************/

void make_PartMesh_change_innerbc_change_Pinbc_2nd_sub( PartMesh pm,
                                                        int tfree,
                                                        OPinfo *op,
                                                        Pinbc *pinbc )
{
  /*+ Function: 2nd step of changing Pinbc (sub) +*/
  int    i, j ;

  int    part ;
  int    partid = pm.partid ;
  int    npart = COM_GID.nparent ;


  /* trade data within part */
  for( part=0 ; part<partid ; part++ ) {
    int    n_mofree = op[part].n_mofree ;
    int*   ibid = op[part].ibid ;
    for( i=0 ; i<n_mofree ; i++ ) {
      pinbc[ibid[i]].yng_npart++ ;
    }
  }
  for( part=partid+1 ; part<npart ; part++ ) {
    int    n_mofree = op[part].n_mofree ;
    int*   ibid = op[part].ibid ;
    for( i=0 ; i<n_mofree ; i++ ) {
      pinbc[ibid[i]].old_npart++ ;
    }
  }


  for( i=0 ; i<tfree ; i++ ) {
    pinbc[i].yng_part = (int *)Malloc( sizeof(int), pinbc[i].yng_npart ) ;
    for( j=0 ; j<pinbc[i].yng_npart ; j++ ) {
      pinbc[i].yng_part[j] = -1 ;
    }
    pinbc[i].yng_nf_part = (int *)Calloc( sizeof(int), pinbc[i].yng_npart ) ;
    pinbc[i].yng_npart = 0 ;

    pinbc[i].old_part = (int *)Malloc( sizeof(int), pinbc[i].old_npart ) ;
    for( j=0 ; j<pinbc[i].old_npart ; j++ ) {
      pinbc[i].old_part[j] = -1 ;
    }
    pinbc[i].old_nf_part = (int *)Calloc( sizeof(int), pinbc[i].old_npart ) ;
    pinbc[i].old_npart = 0 ;
  }
}

/********************************************************************/

void make_PartMesh_change_innerbc_change_Pinbc_3rd( int *tfree,
                                                    Pinbc **pinbc )
{
  /*+ Function: 3rd step of changing Pinbc +*/
  /*+           make bc_flag +*/
  int    i, j, k ;

  int    part ;


  for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
    for( i=0 ; i<tfree[k] ; i++ ) {
      int    nf = pinbc[k][i].nf ;
      if( pinbc[k][i].bc_flag == __ADVFW_HDDM_BC_FLAG_ESSENTIAL_BC__ ) {
        /* Nothing to do */
      } else {
        if( nf == __ADVFW_HDDM_CHANGE_INNER_BC_NOT_CHANGE__ ) {
          pinbc[k][i].bc_flag = __ADVFW_HDDM_BC_FLAG_NOT_CHANGE__ ;
        } else if( nf == 0 ) {
          pinbc[k][i].bc_flag = __ADVFW_HDDM_BC_FLAG_NO_DOF__ ;
        } else {

          /* only one */
          if( nf == 1 ) {
            int    ii = -1 ;
            for( j=0 ; j<pinbc[k][i].yng_npart ; j++ ) {
              if( pinbc[k][i].yng_nf_part[j] != 0 ) {
                ii = 0 ;
                break ;
              }
            }
            for( j=0 ; j<pinbc[k][i].old_npart ; j++ ) {
              if( pinbc[k][i].old_nf_part[j] != 0 ) {
                ii = 0 ;
                break ;
              }
            }
            if( ii == -1 ) {
              pinbc[k][i].bc_flag = __ADVFW_HDDM_BC_FLAG_ONLY_ONE__ ;
              continue ;
            }

          /* only this part */
          } else {
            int    ii = -1 ;
            for( j=0 ; j<pinbc[k][i].yng_npart ; j++ ) {
              if( pinbc[k][i].yng_nf_part[j] != 0 ) {
                ii = 0 ;
                break ;
              }
            }
            for( j=0 ; j<pinbc[k][i].old_npart ; j++ ) {
              if( pinbc[k][i].old_nf_part[j] != 0 ) {
                ii = 0 ;
                break ;
              }
            }
            if( ii == -1 ) {
              pinbc[k][i].bc_flag = __ADVFW_HDDM_BC_FLAG_ONLY_THIS_PART__ ;
              continue ;
            }
          }

          {
            int    ii = 0 ;
            for( j=0 ; j<pinbc[k][i].yng_npart ; j++ ) {
              if( pinbc[k][i].yng_nf_part[j] == 0 ) {
                ii = -1 ;
                break ;
              }
            }
            for( j=0 ; j<pinbc[k][i].old_npart ; j++ ) {
              if( pinbc[k][i].old_nf_part[j] == 0 ) {
                ii = -1 ;
                break ;
              }
            }
            if( ii == 0 ) {
              pinbc[k][i].bc_flag = __ADVFW_HDDM_BC_FLAG_NOT_CHANGE__ ;
              continue ;
            }
          }

          /* infree & outfree */
          {
            int    ii = -1 ;
            for( j=0 ; j<pinbc[k][i].yng_npart ; j++ ) {
              if( pinbc[k][i].yng_nf_part[j] != 0 ) {
                ii = 0 ;
                break ;
              }
            }
            if( ii == -1 ) {
              pinbc[k][i].bc_flag = __ADVFW_HDDM_BC_FLAG_INFREE__ ;
            } else {
              pinbc[k][i].bc_flag = __ADVFW_HDDM_BC_FLAG_OUTFREE__ ;
            }
          }
        }
      }
    }
  }
}

/********************************************************************/

void make_PartMesh_change_innerbc_change_Pinbc_4th( PartMesh *pm,
                                                    int *tfree,
                                                    int *infree,
                                                    int *outfree,
                                                    Pinbc **pinbc )
{
  /*+ Function: 4th step of changing Pinbc +*/
  /*+           change infree & outfree +*/
  /*+           change charge part
                     & local interface B.C. number in part +*/
  int    i, j, k ;

  int    part ;


  for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
    int    partid = pm[k].partid ;
    int    new_infree = 0 ;
    int    new_outfree = 0 ;
    for( i=0 ; i<tfree[k] ; i++ ) {
      int    bc_flag = pinbc[k][i].bc_flag ;
      if( bc_flag == __ADVFW_HDDM_BC_FLAG_NOT_CHANGE__ ) {
        if( pinbc[k][i].charge_part == partid ) {
          pinbc[k][i].ln_part = new_infree ;
          new_infree++ ;
        } else {
          pinbc[k][i].ln_part = new_outfree ;
          new_outfree++ ;
        }
      } else if( bc_flag == __ADVFW_HDDM_BC_FLAG_ONLY_THIS_PART__ ) {
        pinbc[k][i].charge_part = partid ;
        pinbc[k][i].ln_part = new_infree ;
        new_infree++ ;
      } else if( bc_flag == __ADVFW_HDDM_BC_FLAG_INFREE__ ) {
        pinbc[k][i].charge_part = partid ;
        pinbc[k][i].ln_part = new_infree ;
        new_infree++ ;
      } else if( bc_flag == __ADVFW_HDDM_BC_FLAG_OUTFREE__ ) {
        for( j=0 ; j<pinbc[k][i].yng_npart ; j++ ) {
          if( pinbc[k][i].yng_nf_part[j] != 0 ) {
            pinbc[k][i].charge_part = pinbc[k][i].yng_part[j] ;
            break ;
          }
        }
        pinbc[k][i].ln_part = new_outfree ;
        new_outfree++ ;
      }
    }
    for( i=0 ; i<tfree[k] ; i++ ) {
      int    bc_flag = pinbc[k][i].bc_flag ;
      if( bc_flag == __ADVFW_HDDM_BC_FLAG_NOT_CHANGE__ ) {
        if( pinbc[k][i].charge_part == partid ) {
          pinbc[k][i].ln_part += new_outfree ;
        }
      } else if( bc_flag == __ADVFW_HDDM_BC_FLAG_ONLY_THIS_PART__ ) {
        pinbc[k][i].ln_part += new_outfree ;
      } else if( bc_flag == __ADVFW_HDDM_BC_FLAG_INFREE__ ) {
        pinbc[k][i].ln_part += new_outfree ;
      }
    }
    infree[k] = new_infree ;
    outfree[k] = new_outfree ;
  }
}

/********************************************************************/

void make_PartMesh_change_innerbc_change_Pinbc_5th( PartMesh *pm,
                                                    int *tfree,
                                                    OPinfo **op,
                                                    Pinbc **pinbc )
{
  /*+ Function: 5th step of changing Pinbc +*/
  /*+           change local interface B.C. number in charge part +*/
  int    i, k ;

  int    part ;


  for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
    int    ii = tfree[k] ;
    for( i=0 ; i<ii ; i++ ) {
      pinbc[k][i].ln_chpar = pinbc[k][i].ln_part ;
    }
  }


  #ifdef __ADVFW_SINGLE_MODE__ /* s-mode */
    make_PartMesh_change_innerbc_change_Pinbc_5th_s( op, pinbc ) ;
  #else
    #ifdef __ADVFW_PARALLEL_MODE__ /* p-mode */
      make_PartMesh_change_innerbc_change_Pinbc_5th_p( pm[0], op[0],
                                                       pinbc[0] ) ;
    #else /* h-mode */
      make_PartMesh_change_innerbc_change_Pinbc_5th_h( pm[0], op[0],
                                                       pinbc[0] ) ;
    #endif /* __ADVFW_PARALLEL_MODE__ */
  #endif /* __ADVFW_SINGLE_MODE__ */
}

/********************************************************************/

void make_PartMesh_change_innerbc_change_Inbc( Pinbc *pinbc, int tfree,
                                               int *nf,
                                               int *ninbc, Inbc **inbc )
{
  /*+ Function: change Inbc +*/
  int    i ;

  int    new_ninbc = 0 ;
  Inbc*  new_inbc = NULL ;


  for( i=0 ; i<(*ninbc) ; i++ ) {
    int    ii = (*inbc)[i].ln_part ;
    int    bc_flag = pinbc[ii].bc_flag ;
    if( (bc_flag != __ADVFW_HDDM_BC_FLAG_ESSENTIAL_BC__)
        && (bc_flag != __ADVFW_HDDM_BC_FLAG_NO_DOF__)
        && (bc_flag != __ADVFW_HDDM_BC_FLAG_ONLY_ONE__)
        && (nf[(*inbc)[i].snid] != 0) ) {
      new_ninbc++ ;
    }
  }
  new_inbc = (Inbc *)Malloc( sizeof(Inbc), new_ninbc ) ;
  new_ninbc = 0 ;
  for( i=0 ; i<(*ninbc) ; i++ ) {
    int    ii = (*inbc)[i].ln_part ;
    int    bc_flag = pinbc[ii].bc_flag ;
    if( (bc_flag != __ADVFW_HDDM_BC_FLAG_ESSENTIAL_BC__)
        && (bc_flag != __ADVFW_HDDM_BC_FLAG_NO_DOF__)
        && (bc_flag != __ADVFW_HDDM_BC_FLAG_ONLY_ONE__)
        && (nf[(*inbc)[i].snid] != 0) ) {
      new_inbc[new_ninbc].snid = (*inbc)[i].snid ;
      new_inbc[new_ninbc].coordinate = (*inbc)[i].coordinate ;
      new_inbc[new_ninbc].charge_part = pinbc[ii].charge_part ;
      new_inbc[new_ninbc].ln_chpar = pinbc[ii].ln_chpar ;
      new_inbc[new_ninbc].ln_part = pinbc[ii].ln_part ;
      new_ninbc++ ;
    }
  }
  Free( sizeof(Inbc)*(*ninbc), (*inbc) ) ;
  (*ninbc) = new_ninbc ;
  (*inbc) = new_inbc ;
}

/********************************************************************/

void make_PartMesh_change_innerbc_change_OPinfo( Pinbc *pinbc, int tfree,
                                                 int infree, int outfree,
                                                 int partid, OPinfo **op )
{
  /*+ Function: change OPinfo +*/
  int    i, j ;

  int    part ;
  int    npart = COM_GID.nparent ;
  OPinfo* new_op = NULL ;


  new_op = (OPinfo *)Malloc( sizeof(OPinfo), npart ) ;
  for( part=0 ; part<npart ; part++ ) {
    int    n_mofree = (*op)[part].n_mofree ;
    int*   ibid = (*op)[part].ibid ;
    int    new_n_mofree = 0 ;
    init_OPinfo( &(new_op[part]) ) ;
    for( i=0 ; i<n_mofree ; i++ ) {
      int    ii = ibid[i] ;
      int    bc_flag = pinbc[ii].bc_flag ;
      if( (bc_flag != __ADVFW_HDDM_BC_FLAG_ESSENTIAL_BC__)
          && (bc_flag != __ADVFW_HDDM_BC_FLAG_NO_DOF__)
          && (bc_flag != __ADVFW_HDDM_BC_FLAG_ONLY_ONE__)
          && (bc_flag != __ADVFW_HDDM_BC_FLAG_ONLY_THIS_PART__) ) {
        if( bc_flag == __ADVFW_HDDM_BC_FLAG_INFREE__ ) {
          int    old_npart = pinbc[ii].old_npart ;
          int*   old_part = pinbc[ii].old_part ;
          int*   old_nf_part = pinbc[ii].old_nf_part ;
          for( j=0 ; j<old_npart ; j++ ) {
            if( old_part[j] == part ) {
              if( old_nf_part[j] != 0 ) {
                new_n_mofree++ ;
              }
              break ;
            }
          }
        } else if( bc_flag == __ADVFW_HDDM_BC_FLAG_OUTFREE__ ) {
          int    yng_npart = pinbc[ii].yng_npart ;
          int*   yng_part = pinbc[ii].yng_part ;
          int*   yng_nf_part = pinbc[ii].yng_nf_part ;
          for( j=0 ; j<yng_npart ; j++ ) {
            if( yng_part[j] == part ) {
              if( yng_nf_part[j] != 0 ) {
                new_n_mofree++ ;
              }
              break ;
            }
          }
        } else {
          new_n_mofree++ ;
        }
      }
    }
    new_op[part].n_mofree = new_n_mofree ;
    new_op[part].ibid = (int *)Malloc( sizeof(int), new_n_mofree ) ;
  }


  for( part=0 ; part<npart ; part++ ) {
    int    n_mofree = (*op)[part].n_mofree ;
    int*   ibid = (*op)[part].ibid ;
    int    new_n_mofree = 0 ;
    for( i=0 ; i<n_mofree ; i++ ) {
      int    ii = ibid[i] ;
      int    bc_flag = pinbc[ii].bc_flag ;
      if( (bc_flag != __ADVFW_HDDM_BC_FLAG_ESSENTIAL_BC__)
          && (bc_flag != __ADVFW_HDDM_BC_FLAG_NO_DOF__)
          && (bc_flag != __ADVFW_HDDM_BC_FLAG_ONLY_ONE__)
          && (bc_flag != __ADVFW_HDDM_BC_FLAG_ONLY_THIS_PART__) ) {
        if( bc_flag == __ADVFW_HDDM_BC_FLAG_INFREE__ ) {
          int    old_npart = pinbc[ii].old_npart ;
          int*   old_part = pinbc[ii].old_part ;
          int*   old_nf_part = pinbc[ii].old_nf_part ;
          for( j=0 ; j<old_npart ; j++ ) {
            if( old_part[j] == part ) {
              if( old_nf_part[j] != 0 ) {
                new_op[part].ibid[new_n_mofree] = pinbc[ii].ln_part ;
                new_n_mofree++ ;
              }
              break ;
            }
          }
        } else if( bc_flag == __ADVFW_HDDM_BC_FLAG_OUTFREE__ ) {
          int    yng_npart = pinbc[ii].yng_npart ;
          int*   yng_part = pinbc[ii].yng_part ;
          int*   yng_nf_part = pinbc[ii].yng_nf_part ;
          for( j=0 ; j<yng_npart ; j++ ) {
            if( yng_part[j] == part ) {
              if( yng_nf_part[j] != 0 ) {
                new_op[part].ibid[new_n_mofree] = pinbc[ii].ln_part ;
                new_n_mofree++ ;
              }
              break ;
            }
          }
        } else {
          new_op[part].ibid[new_n_mofree] = pinbc[ii].ln_part ;
          new_n_mofree++ ;
        }
      }
    }
    free_OPinfo( &((*op)[part]) ) ;
  }
  Free( sizeof(OPinfo)*npart, (*op) ) ;
  (*op) = new_op ;
}

/********************************************************************/

void make_PartMesh_make_ap( int *nap, int **ap,
                            int nel, int nnd, int *nop,
                            int ap_elem, int nd_elem )
{
  /*+ Function: make nap & ap +*/
  make_MESH_make_ap( nap, ap, nel, nnd, nop, ap_elem, nd_elem ) ;
}

/********************************************************************/

double* make_PartMesh_change_crd( int nnd, int dimension,
                                  double *part_crd, int *snid2pnid,
                                  int nap, int *ap )
{
  /*+ Function: change crd +*/
  int     i, j, k ;

  int     nd ;
  double* crd ;


  crd = (double *)Malloc( sizeof(double), nap*dimension ) ;
  for( nd=0,j=0 ; nd<nnd ;nd++ ) {
    if( ap[nd] > -1 ) {
      for( i=0,k=snid2pnid[nd]*dimension ; i<dimension ; i++,j++,k++ ) {
        crd[j] = part_crd[k] ;
      }
    }
  }
  return crd ;
}

/********************************************************************/

void make_PartMesh_make_apid2phiid( int *nphi, int **apid2phiid,
                                    int nel, int nap, int nmp,
                                     int *nop, int *flag, int *ap,
                                    int ap_elem, int nd_elem,
                                    MTRL mtrl )
{
  /*+ Function: make nphi & apid2phiid for A-phi method +*/
  make_MESH_make_apid2phiid( nphi, apid2phiid,
                             nel, nap, nmp,  nop, flag, ap,
                             ap_elem, nd_elem, mtrl ) ;
}


/********************************************************************/

void make_PartMesh_make_apid2phiid_with_Disp( int *nphi, int **apid2phiid,
                                              int nap, int nmp )
{
  /*+ Function: make nphi & apid2phiid for A-phi method +*/
  make_MESH_make_apid2phiid_with_Disp( nphi, apid2phiid, nap, nmp ) ;
}

/********************************************************************/

void make_PartMesh_make_apid2snid( int **apid2snid,
                                   int nnd, int nap, int *ap )
{
  /*+ Function: make apid to snid +*/
  make_MESH_make_apid2gnid( apid2snid, nnd, nap, ap ) ;
}

/********************************************************************/

void make_PartMesh_make_mpid2snid( int **mpid2snid,
                                   int nnd, int nap, int nmp, int nphi,
                                   int *ap, int *apid2phiid, int *apid2snid )
{
  /*+ Function: make mpid to snid +*/
  make_MESH_make_mpid2gnid( mpid2snid, nnd, nap, nmp, nphi, ap,
                            apid2phiid, apid2snid ) ;
}

/********************************************************************/

void make_PartMesh_change_dbc_Axn0_1st( DirichletBC *dbc_Axn0_tmp,
                                        DirichletBC *dbc_Axn0,
                                        int *ap )
{
  /*+ Function: change DirichletBC for Axn=0 1st step +*/
  make_MESH_change_dbc_Axn0_1st( dbc_Axn0_tmp, dbc_Axn0, ap ) ;
}

/********************************************************************/

void make_PartMehs_make_ANTENNA( ANTENNA **antenna,
                              int nel, int *nop, int *flag,
                              int ap_elem, int nd_elem,
                              int nap, int *ap, DirichletBC dbc_tmp,
                              int *apid2snid, MTRL mtrl )
{
  /*+ Function: 1st step of make struct ANTENNA +*/
  int    i ;


  (*antenna) = (ANTENNA *)Malloc( sizeof(ANTENNA), mtrl.nantenna ) ;


  for( i=0 ; i<mtrl.nantenna ; i++ ) {
    make_PartMesh_make_ANTENNA_make_enf( &((*antenna)[i]), nel, nop, flag,
                                      ap_elem, nd_elem, nap, ap, apid2snid,
                                      mtrl.antenna_def[i].flag ) ;
    make_PartMesh_make_ANTENNA_change_bc( &((*antenna)[i]), dbc_tmp ) ;
  }
}

/********************************************************************/

void make_PartMesh_make_ANTENNA_make_enf( ANTENNA *antenna,
                                       int nel, int *nop, int *flag,
                                       int ap_elem, int nd_elem,
                                       int nap, int *ap,
                                       int *apid2snid, int antenna_flag )
{
  /*+ Function: make struct ANTENNA (make enf) +*/
  make_MESH_make_ANTENNA_make_enf( antenna, nel, nop, flag, ap_elem, nd_elem,
                                nap, ap, apid2snid, antenna_flag ) ;
}

/********************************************************************/

void make_PartMesh_make_ANTENNA_change_bc( ANTENNA *antenna, DirichletBC dbc_tmp )
{
  /*+ Function: make struct ANTENNA (change bc) +*/
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
  antenna->dbc.nbc = nbc ;
  ibc = (int *)Malloc( sizeof(int), nbc ) ;
  nbc = 0 ;
  for( bc=0 ; bc<dbc_tmp.nbc ; bc++ ) {
    if( enf[dbc_tmp.ibc[bc]] > -1 ) {
      ibc[nbc] = enf[dbc_tmp.ibc[bc]] ;
      nbc++ ;
    }
  }
  sort_i( nbc, ibc ) ;
  antenna->dbc.ibc = ibc ;
}

/********************************************************************/

void make_PartMesh_make_ANTENNA_add_inbc( ANTENNA *antenna,
                                       int nnd, int nap,
                                       int ninbc, Inbc *inbc )
{
  /*+ Function: make struct ANTENNA (make dbc_inbc) +*/
  if( ninbc != 0 ) {
    int    i, j, k ;

    int    nd ;
    int*   enf = antenna->enf ;
    int*   apid2snid = antenna->apid2gnid ;
    int*   snid2apid = (int *)Malloc( sizeof(int), nnd ) ;
    for( nd=0 ; nd<nnd ; nd++ ) {
      snid2apid[nd] = -1 ;
    }
    for( i=0 ; i<nap ; i++ ) {
      snid2apid[apid2snid[i]] = i ;
    }
    for( i=0 ; i<ninbc ; i++ ) {
      inbc[i].nfid = enf[snid2apid[inbc[i].snid]] ;
    }
    make_PartMesh_sort_inbc( ninbc, inbc ) ;


    k = antenna->dbc.nbc + ninbc ;
    antenna->dbc.ibc = (int *)Realloc( antenna->dbc.ibc, sizeof(int),
                                    antenna->dbc.nbc, k ) ;
    for( i=0,j=antenna->dbc.nbc ; i<ninbc ; i++,j++ ) {
      antenna->dbc.ibc[j] = inbc[i].nfid ;
    }
    antenna->dbc.nbc = k ;
    antenna->ninbc = ninbc ;
    antenna->inbc = inbc ;


    Free( sizeof(int)*nnd, snid2apid ) ;


  } else if( (antenna->dbc.nbc == 0) && (antenna->nf > 0) ) {
    antenna->dbc.nbc = 1 ;
    antenna->dbc.ibc = (int *)Malloc( sizeof(int), antenna->dbc.nbc ) ;
    antenna->dbc.ibc[0] = 0 ;
  }
}



/********************************************************************/

void make_PartMesh_change_ap( int *ap, int nnd )
{
  /*+ Function: change ap +*/
  make_MESH_change_ap( ap, nnd ) ;
}

/********************************************************************/

void make_PartMesh_change_nop( int nel, int nd_elem, int *nop,
                               int *ap, char *elem_type, OPTIONS options )
{
  /*+ Function: change ap +*/
  make_MESH_change_nop( nel, nd_elem, nop, ap, elem_type, options ) ;
}

/********************************************************************/

void make_PartMesh_change_dbc_Axn0_2nd( DirichletBC *dbc_Axn0_tmp,
                                        DirichletBC *dbc_Axn0,
                                        int *ap, int *apid2phiid,
                                        int *nbc_A, OPTIONS options )
{
  /*+ Function: change DirichletBC 2nd step +*/
  make_MESH_change_dbc_Axn0_2nd( dbc_Axn0_tmp, dbc_Axn0, ap, apid2phiid,
                                 nbc_A, options ) ;
  sort_i( dbc_Axn0->nbc, dbc_Axn0->ibc ) ;
}

/********************************************************************/

void make_PartMesh_change_dbc_Axn0_3rd( DirichletBC *dbc,
                                        int nnd, int nap, int nmp, int nphi,
                                        int ninbc, Inbc *inbc,
                                        int *apid2phiid, int *apid2snid,
                                        int *mpid2snid )
{
  /*+ Function: change DirichletBC 3rd step +*/
  int    i, j ;

  int    nd ;
  int    mnmp = -nmp ;
  int    nbc = dbc->nbc + ninbc ;
  int*   snid2mpid = (int *)Malloc( sizeof(int), nnd ) ;
  int*   snid2phiid = NULL ;
  int*   phiid2apid = NULL ;


  for( nd=0 ; nd<nnd ; nd++ ) {
    snid2mpid[nd] = -1 ;
  }
  for( i=0 ; i<nmp ; i++ ) {
    snid2mpid[mpid2snid[i]] = i ;
  }


  if( nphi > 0 ) {
    snid2phiid = (int *)Malloc( sizeof(int), nnd ) ;
    phiid2apid = (int *)Malloc( sizeof(int), nphi ) ;

    for( nd=0 ; nd<nnd ; nd++ ) {
      snid2phiid[nd] = -1 ;
    }
    for( i=0 ; i<nphi ; i++ ) {
      phiid2apid[i] = -1 ;
    }
    for( i=0 ; i<nap ; i++ ) {
      if( apid2phiid[i] > -1 ) {
        phiid2apid[apid2phiid[i]+mnmp] = i ;
      }
    }
    for( i=0 ; i<nphi ; i++ ) {
      snid2phiid[apid2snid[phiid2apid[i]]] = i ;
    }
    Free( sizeof(int)*nphi, phiid2apid ) ;
  }


  for( i=0 ; i<ninbc ; i++ ) {
    if( snid2mpid[inbc[i].snid] > -1 ) {
      inbc[i].nfid = snid2mpid[inbc[i].snid] ;
    } else {
      inbc[i].nfid = snid2phiid[inbc[i].snid] + nmp ;
    }
  }
  make_PartMesh_sort_inbc( ninbc, inbc ) ;


  dbc->ibc = (int *)Realloc( dbc->ibc, sizeof(int), dbc->nbc, nbc ) ;
  for( i=0,j=dbc->nbc ; i<ninbc ; i++,j++ ) {
    dbc->ibc[j] = inbc[i].nfid ;
  }
  dbc->nbc = nbc ;


  Free( sizeof(int)*nnd, snid2mpid ) ;
  if( nphi > 0 ) {
    Free( sizeof(int)*nnd, snid2phiid ) ;
  }
}

/********************************************************************/

void make_PartMesh_sort_inbc( int ninbc, Inbc *inbc )
{
  /*+ Function: sort struct Inbc +*/
  int    i, j ;


  for( i=1 ; i<ninbc ; i++ ) {
    for( j=ninbc-1 ; j>=i ; j-- ) {
      if( inbc[j-1].nfid > inbc[j].nfid ) {
        int    ii ;
        int    jj = j - 1 ;
        ii = inbc[jj].snid ; inbc[jj].snid = inbc[j].snid ;
                             inbc[j].snid = ii ;
        ii = inbc[jj].nfid ; inbc[jj].nfid = inbc[j].nfid ;
                             inbc[j].nfid = ii ;
        ii = inbc[jj].coordinate ; inbc[jj].coordinate = inbc[j].coordinate ;
                                   inbc[j].coordinate = ii ;
        ii = inbc[jj].charge_part ;
             inbc[jj].charge_part = inbc[j].charge_part ;
             inbc[j].charge_part = ii ;
        ii = inbc[jj].ln_chpar ; inbc[jj].ln_chpar = inbc[j].ln_chpar ;
                                 inbc[j].ln_chpar = ii ;
        ii = inbc[jj].ln_part ; inbc[jj].ln_part = inbc[j].ln_part ;
                                inbc[j].ln_part = ii ;
      }
    }
  }
}

/********************************************************************/


/*** 2008.2.10 ***/

void make_PartMesh_FW( PartMesh *pm, OPTIONS options )
{
  /*+ Function: make structure PartMesh for Magnetic Field +*/
  int    i, k ;

  int    part ;


  for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
    int    nantenna = pm[k].mtrl.nantenna ;
    int    dom ;
    int    ndomain = pm[k].ndomain ;
    if( nantenna != 0 ) {
      pm[k].pantenna = (PartANTENNA *)Malloc( sizeof(PartANTENNA), nantenna ) ;
      for( i=0 ; i<nantenna ; i++ ) {
        init_PartANTENNA( &(pm[k].pantenna[i]) ) ;
        pm[k].pantenna[i].dantenna = (DomANTENNA *)Malloc( sizeof(DomANTENNA), ndomain ) ;
        for( dom=0 ; dom<ndomain ; dom++ ) {
          init_DomANTENNA( &(pm[k].pantenna[i].dantenna[dom]) ) ;
	  
        }
      }
    }

    pm[k].presult.dantenna = (DomANTENNA *)Malloc( sizeof(DomANTENNA), ndomain ) ;
    for( dom=0 ; dom<ndomain ; dom++ ) {
      init_DomANTENNA( &(pm[k].presult.dantenna[dom]) ) ;
    }

  }


  make_PartMesh_FW_change_innerbc( pm, options ) ;


  if( !strcmp(pm[0].elem_type, ELEM_TYPE_3DQT) ) {
    make_PartMesh_FW_TetQ( pm, options ) ;
  } else {
    emessage( 126, __ABORT_LIBHDDM__, pm[0].elem_type ) ;
  }
}

/**********************************************************/

void make_PartMesh_FW_change_innerbc( PartMesh *pm,
                                       OPTIONS options )
{
  /*+ Function: change inner B.C. for Mangetic field +*/
  #ifdef __ADVFW_SPECIAL_FUNCTIONS_PERIODICITY_BC__
    if( pm[0].mtrl.pbc == 1 ) {
      if( COM_am_I_parent() ) {
        make_PartMesh_add_innerbc_PBC( pm ) ;
      }
    }
  #endif /* __ADVFW_SPECIAL_FUNCTIONS_PERIODICITY_BC__ */

    make_PartMesh_FW_change_innerbc_ANTENNA( pm, options ) ;	
 
    make_PartMesh_FW_change_innerbc_whole( pm, options ) ;
  
}

/********************************************************************/

void make_PartMesh_FW_change_innerbc_ANTENNA( PartMesh *pm,
                                            OPTIONS options )
{
  /*+ Function: change inner B.C. for Mangetic field for ANTENNA +*/
  int    i, k ;

  int    part ;
  int    npart = COM_GID.nparent ;
  int    nantenna = pm[0].mtrl.nantenna ;


  for( i=0 ; i<nantenna ; i++ ) {
    int*   infree = NULL ;
    int*   outfree = NULL ;
    int    total_free = 0 ;
    OPinfo** op = NULL ;
    int**    ninbc = NULL ;
    Inbc*** inbc = NULL ;


    make_PartMesh_change_innerbc_make_temp_struct( pm, &infree, &outfree,
                                                   &op, &ninbc, &inbc ) ;


    make_PartMesh_change_innerbc_func( infree, outfree, op, ninbc, inbc,
                                       pm, options,
                                       make_PartMesh_FW_change_innerbc_ANTENNA_make_nf,
                                       i ) ;



    #ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
      work_times.comm += plusTime( &s_time ) ;
      if( COM_GID.rootid == COM_ID.myprocid ) {
        total_free += infree[0] ;
        for( part=1 ; part<npart ; part++ ) {
          int    compid = COM_GID.parid[part] ;
          int    type = 30000 + part ;
          int    tmp ;
          COM_ReadInt( &tmp, 1, &compid, &type ) ;
          total_free += tmp ;
        }
      } else {
        int    compid = COM_GID.rootid ;
        int    type = 30000 + COM_ID.myprocid ;
        COM_WriteInt( &(infree[0]), 1, compid, type ) ;
      }
      COM_Grp_Bcast_Int( &total_free, 1, COM_GID.rootid ) ;
      work_times.comm += plusTime( &s_time ) ;
    #else
      for( part=0 ; part<npart ; part++ ) {
        total_free += infree[part] ;
      }
    #endif


    for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
      int    dom ;
      int    ndomain = pm[k].ndomain ;
      pm[k].pantenna[i].npart = npart ;
      pm[k].pantenna[i].ndomain = ndomain ;
      pm[k].pantenna[i].infree = infree[k] ;
      pm[k].pantenna[i].outfree = outfree[k] ;
      pm[k].pantenna[i].tfree = infree[k] + outfree[k] ;
      pm[k].pantenna[i].total_free = total_free ;
      pm[k].pantenna[i].op = op[k] ;
      for( dom=0 ; dom<ndomain ; dom++ ) {
        pm[k].pantenna[i].dantenna[dom].ninbc = ninbc[k][dom] ;
        pm[k].pantenna[i].dantenna[dom].inbc = inbc[k][dom] ;
      }
      Free( sizeof(int*)*ndomain, ninbc[k] ) ;
      Free( sizeof(Inbc*)*ndomain, inbc[k] ) ;
    }
    Free( sizeof(int)*nrepeat, infree ) ;
    Free( sizeof(int)*nrepeat, outfree ) ;
    Free( sizeof(OPinfo*)*nrepeat, op ) ;
    Free( sizeof(int*)*nrepeat, ninbc ) ;
    Free( sizeof(Inbc**)*nrepeat, inbc ) ;
  }
}

/********************************************************************/

void make_PartMesh_FW_change_innerbc_ResultValue( PartMesh *pm,
						   OPTIONS options )
{
  /*+ Function: change inner B.C. for Mangetic field for ResultValue +*/
  int    i=0 , j, k ;
  
  int    part ;
  int    npart = COM_GID.nparent ;

  int*   infree = NULL ;
  int*   outfree = NULL ;
  int    total_free = 0 ;
  OPinfo** op = NULL ;
  int**    ninbc = NULL ;
  Inbc*** inbc = NULL ;
  
  int    need = pm[0].dbc_Axn0.need ;
  
  make_PartMesh_change_innerbc_make_temp_struct( pm, &infree, &outfree,
                                                 &op, &ninbc, &inbc ) ;
  
  pm[0].dbc_Axn0.need = NO_NEED ;
  make_PartMesh_change_innerbc_func( infree, outfree, op, ninbc, inbc,
                                     pm, options,
                                     make_PartMesh_FW_change_innerbc_ANTENNA_make_nf,
                                     i ) ;
  pm[0].dbc_Axn0.need = need ;
  
  /**** ****/  
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  work_times.work += plusTime( &s_time ) ;
  COM_Grp_Synchronize() ;
  work_times.idle += plusTime( &s_time ) ;
  if( COM_GID.rootid == COM_ID.myprocid ) {
    total_free += infree[0] ;
    for( part=1 ; part<npart ; part++ ) {
      int    compid = COM_GID.parid[part] ;
      int    type = 30000 + part ;
      int    tmp ;
      COM_ReadInt( &tmp, 1, &compid, &type ) ;
      total_free += tmp ;
    }
  } else {
    int    compid = COM_GID.rootid ;
    int    type = 30000 + COM_ID.myprocid ;
    COM_WriteInt( &(infree[0]), 1, compid, type ) ;
  }
  COM_Grp_Bcast_Int( &total_free, 1, COM_GID.rootid ) ;
  work_times.comm += plusTime( &s_time ) ;
#else
  for( part=0 ; part<npart ; part++ ) {
    total_free += infree[part] ;
  }
#endif
  
  for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
    int    dom ;
    int    ndomain = pm[k].ndomain ;
    pm[k].presult.npart = npart ;
    pm[k].presult.ndomain = ndomain ;
    pm[k].presult.infree = infree[k] ;
    pm[k].presult.outfree = outfree[k] ;
    pm[k].presult.tfree = infree[k] + outfree[k] ;

  for( dom=0 ; dom<ndomain ; dom++ ) {
      DomMesh* dm = &(pm[k].dm[dom]) ;
      int    nel = dm->nel ;
      int    nnd = dm->nnd ;
      int    ap_elem = dm->ap_elem ;
      int    mp_elem = dm->nd_elem - ap_elem ;
      int*   nop = dm->nop ;
      int    ii = ninbc[k][dom] ;
      int    jj ;
      int*   snid2apid = (int *)Malloc( sizeof(int), nnd ) ;
      for( i=0 ; i<nnd ; i++ ) {
        snid2apid[i] = -1 ;
      }
      for( i=0,jj=0 ; i<nel ; i++ ) {
        for( j=0 ; j<ap_elem ; j++,jj++ ) {
          snid2apid[nop[jj]] =0 ;
        }
        jj += mp_elem ;
      }
      for( i=0,jj=0 ; i<nnd ; i++ ) {
        if( snid2apid[i] > -1 ) {
          snid2apid[i] = jj ;
          jj++ ;
        }
      }
      for( i=0 ; i<ii ; i++ ) {
        inbc[k][dom][i].nfid = snid2apid[inbc[k][dom][i].snid] ;
      }
      Free( sizeof(int)*nnd, snid2apid ) ;
      pm[k].presult.dantenna[dom].ninbc = ii ;
      pm[k].presult.dantenna[dom].inbc = inbc[k][dom] ;
    }
    Free( sizeof(int*)*ndomain, ninbc[k] ) ;
    Free( sizeof(Inbc*)*ndomain, inbc[k] ) ;
  }
  Free( sizeof(int)*nrepeat, infree ) ;
  Free( sizeof(int)*nrepeat, outfree ) ;
  Free( sizeof(OPinfo*)*nrepeat, op ) ;
  Free( sizeof(int*)*nrepeat, ninbc ) ;
  Free( sizeof(Inbc**)*nrepeat, inbc ) ;
}

/********************************************************************/

void make_PartMesh_FW_change_innerbc_whole( PartMesh *pm,
                                             OPTIONS options )
{
  /*+ Function: change inner B.C. for Mangetic field for whole domain +*/
  int    i, k ;

  int    part ;
  int    npart = COM_GID.nparent ;

  int*   infree = NULL ;
  int*   outfree = NULL ;
  int    total_free = 0 ;
  OPinfo** op = NULL ;
  int**    ninbc = NULL ;
  Inbc*** inbc = NULL ;


  make_PartMesh_change_innerbc_make_temp_struct( pm, &infree, &outfree,
                                                 &op, &ninbc, &inbc ) ;

  if( options.formulation == FORMULATION_FW_EMETHOD ) {
    make_PartMesh_change_innerbc_func( infree, outfree, op, ninbc, inbc,
                                       pm, options,
                                       make_PartMesh_FW_change_innerbc_make_nf_A_Method,
                                       0 ) ;
  } else {
    char   str[128] ;
    sprintf( str, "%d", options.formulation ) ;
    emessage( 153, __ABORT_LIBHDDM__, str ) ;
  }


  #ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
    work_times.comm += plusTime( &s_time ) ;
    if( COM_GID.rootid == COM_ID.myprocid ) {
      total_free += infree[0] ;
      for( part=1 ; part<npart ; part++ ) {
        int    compid = COM_GID.parid[part] ;
        int    type = 30000 + part ;
        int    tmp ;
        COM_ReadInt( &tmp, 1, &compid, &type ) ;
        total_free += tmp ;
      }
    } else {
      int    compid = COM_GID.rootid ;
      int    type = 30000 + COM_ID.myprocid ;
      COM_WriteInt( &(infree[0]), 1, compid, type ) ;
    }
    COM_Grp_Bcast_Int( &total_free, 1, COM_GID.rootid ) ;
    work_times.comm += plusTime( &s_time ) ;
  #else
    for( part=0 ; part<npart ; part++ ) {
      total_free += infree[part] ;
    }
  #endif


  for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
    int    dom ;
    int    ndomain = pm[k].ndomain ;
    pm[k].infree = infree[k] ;
    pm[k].outfree = outfree[k] ;
    pm[k].tfree = infree[k] + outfree[k] ;
    pm[k].total_free = total_free ;
    for( i=0 ; i<npart ; i++ ) {
      free_OPinfo( &(pm[k].op[i]) ) ;
    }
    Free( sizeof(OPinfo)*npart, pm[k].op ) ;
    pm[k].op = op[k] ;
    for( dom=0 ; dom<ndomain ; dom++ ) {
      int    ii = pm[k].dm[dom].ninbc ;
      for( i=0 ; i<ii ; i++ ) {
        free_Inbc( &(pm[k].dm[dom].inbc[i]) ) ;
      }
      Free( sizeof(Inbc)*ii, pm[k].dm[dom].inbc ) ;
      pm[k].dm[dom].ninbc = ninbc[k][dom] ;
      pm[k].dm[dom].inbc = inbc[k][dom] ;
    }
    Free( sizeof(int*)*ndomain, ninbc[k] ) ;
    Free( sizeof(Inbc*)*ndomain, inbc[k] ) ;
  }
  Free( sizeof(int)*nrepeat, infree ) ;
  Free( sizeof(int)*nrepeat, outfree ) ;
  Free( sizeof(OPinfo*)*nrepeat, op ) ;
  Free( sizeof(int*)*nrepeat, ninbc ) ;
  Free( sizeof(Inbc**)*nrepeat, inbc ) ;
}

/********************************************************************/

void make_PartMesh_FW_change_innerbc_ANTENNA_make_nf( int **nf, DomMesh dm,
                                                    MTRL mtrl, int kk )
{
  /*+ Function: make nf for ANTENNA +*/
  int    i ;

  int    el ;
  int    nel = dm.nel ;
  int    nnd = dm.nnd ;
  int    ap_elem = dm.ap_elem ;
  int    nd_elem = dm.nd_elem ;
  int*   nop = dm.nop ;
  int*   flag = dm.flag ;
  int    antenna_flag = mtrl.antenna_def[kk].flag ;


  (*nf) = (int *)Calloc( sizeof(int), nnd ) ;
  for( el=0 ; el<nel ; el++ ) {
    int    flg = flag[el] ;
    if( flg == antenna_flag ) {
      int    ii = el * nd_elem ;
      for( i=0 ; i<ap_elem ; i++,ii++ ) {
        (*nf)[nop[ii]] = __ADVFW_HDDM_CHANGE_INNER_BC_CHANGE__ ;
      }
    }
  }
}

/********************************************************************/

void make_PartMesh_FW_change_innerbc_ResultValue_make_nf( int **nf,
                                                           DomMesh dm,
                                                           MTRL mtrl,
                                                           int kk )
{
  /*+ Function: make nf for ResultValue +*/
  int    i, j ;

  int    el ;
  int    nel = dm.nel ;
  int    nnd = dm.nnd ;
  int    ap_elem = dm.ap_elem ;
  int    mp_elem = dm.nd_elem - ap_elem ;
  int*   nop = dm.nop ;


  (*nf) = (int *)Calloc( sizeof(int), nnd ) ;
  for( el=0,j=0 ; el<nel ; el++ ) {
    for( i=0 ; i<ap_elem ; i++,j++ ) {
      (*nf)[nop[j]] = __ADVFW_HDDM_CHANGE_INNER_BC_NOT_CHANGE__ ;
    }
    j += mp_elem ;
  }
}

/********************************************************************/

void make_PartMesh_FW_change_innerbc_make_nf_A_Method( int **nf, DomMesh dm,
                                                        MTRL mtrl, int kk )
{
  /*+ Function: make nf for A Method +*/
  int    i, j ;

  int    el ;
  int    nel = dm.nel ;
  int    nnd = dm.nnd ;
  int    ap_elem = dm.ap_elem ;
  int    nd_elem = dm.nd_elem ;
  int*   nop = dm.nop ;


  (*nf) = (int *)Calloc( sizeof(int), nnd ) ;
  for( el=0,j=0 ; el<nel ; el++ ) {
    j += ap_elem ;
    for( i=ap_elem ; i<nd_elem ; i++,j++ ) {
      (*nf)[nop[j]] = __ADVFW_HDDM_CHANGE_INNER_BC_NOT_CHANGE__ ;
    }
  }
}

/********************************************************************/

void make_PartMesh_FW_TetQ( PartMesh *pm, OPTIONS options )
{ 
  /*+ Function: mkae structure PartMesh for Magnetic Field
                                    with 3DQuadraticTetrhedron +*/
  int     i, k ;

  int    part ;

  int    ap_elem = pm[0].ap_elem ;
  int    nd_elem = pm[0].nd_elem ;
  int    dimension = pm[0].dimension ;


  for( part=nstart,k=0 ; part<nend ; part++,k++ ) {
    int    dom ;
    int    ndomain = pm[k].ndomain ;
    double* part_crd = pm[k].crd ;
    for( dom=0 ; dom<ndomain ; dom++ ) {
      DomMesh* dm = &(pm[k].dm[dom]) ;
      int    nel = dm->nel ;
      int    nnd = dm->nnd ;
      int    nap = 0 ;
      int    nmp = 0 ;
      int    nphi = 0 ;
      int*   nop = dm->nop ;
      int*   flag = dm->flag ;
      double* crd = NULL ;
      int*   ap = NULL ;
      int*   apid2phiid = NULL ;
      int*   apid2snid = NULL ;

      DirichletBC dbc_Axn0_tmp ;


      make_PartMesh_make_ap( &nap, &ap, nel, nnd, nop, ap_elem, nd_elem ) ;
      crd = make_PartMesh_change_crd( nnd, dimension, part_crd,
                                      dm->snid2pnid, nap, ap ) ;
      nmp = nnd - nap ;
      
      make_PartMesh_make_apid2snid( &apid2snid, nnd, nap, ap ) ;
      make_PartMesh_make_mpid2snid( &(dm->mpid2snid), nnd, nap, nmp,
                                    nphi, ap, apid2phiid, apid2snid ) ;

      make_PartMesh_change_dbc_Axn0_1st( &dbc_Axn0_tmp, &(dm->dbc_Axn0),ap ) ; 
	       
      make_PartMehs_make_ANTENNA( &(pm[k].dm[dom].antenna), nel, nop, flag,
                               ap_elem, nd_elem, nap, ap, dbc_Axn0_tmp,
                               apid2snid, pm[k].mtrl ) ;
      
      for( i=0 ; i<pm[k].mtrl.nantenna ; i++ ) {
        make_PartMesh_make_ANTENNA_add_inbc( &(pm[k].dm[dom].antenna[i]),
                                          nnd, nap,
                                          pm[k].pantenna[i].dantenna[dom].ninbc,
                                          pm[k].pantenna[i].dantenna[dom].inbc ) ;
      }
 
      make_PartMesh_change_ap( ap, nnd ) ;
      make_PartMesh_change_nop( nel, nd_elem, nop, ap, dm->elem_type,
                                options ) ;
      make_PartMesh_change_dbc_Axn0_2nd( &dbc_Axn0_tmp, &(dm->dbc_Axn0),
                                         ap, apid2phiid, &(dm->nbc_A),
                                         options ) ;
      make_PartMesh_change_dbc_Axn0_3rd( &(dm->dbc_Axn0), nnd, nap, nmp, nphi,
                                         dm->ninbc, dm->inbc, apid2phiid,
                                         apid2snid, dm->mpid2snid ) ;


      Free( sizeof(int)*nnd, ap ) ;
      dm->nap = nap ;
      dm->nmp = nmp ;
      dm->nphi = nphi ;
      dm->nop = nop ;
      dm->crd = crd ;
      dm->apid2phiid = apid2phiid ;
      dm->apid2snid = apid2snid ;

    }
    Free( sizeof(double)*pm[k].nnd*dimension, pm[k].crd ) ;
  }
}
