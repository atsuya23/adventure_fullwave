/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "make_PartMesh_s.h"


extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;

extern int nstart ;
extern int nend ;
extern int nrepeat ;

extern HDDM_SW hddm_sw ;
extern WORK_TIMES work_times ;
extern double s_time ;


void make_PartMesh_check_flag_s( PartMesh *pm, OPTIONS options )
{
  /*+ Function: check flag for PartMesh (s-mode) +*/
  int    part ;
  int    dom ;

  int    npart = COM_GID.nparent ;
  MTRL   mtrl = pm[0].mtrl ;

  int    nflag = 0 ;
  int*   flag_i ;


  /* check nflag & *flag_i */
  for( part=0 ; part<npart ; part++ ) {
    int    ndomain = pm[part].ndomain ;
    for( dom=0 ; dom<ndomain ; dom++ ) {
      int    nel = pm[part].dm[dom].nel ;
      int*   flag = pm[part].dm[dom].flag ;
      make_MESH_check_flag_1st( nel, flag, &nflag, &flag_i ) ;
    }
  }
  sort_i_bubble_sort( nflag, flag_i ) ;
  make_MESH_check_flag_2nd( nflag, flag_i, mtrl, options ) ;
  Free( sizeof(int)*nflag, flag_i ) ;
}

/********************************************************************/

void make_PartMesh_change_innerbc_change_Pinbc_2nd_s( PartMesh *pm,
                                                      int *tfree,
                                                      OPinfo **op,
                                                      Pinbc **pinbc )
{
  /*+ Function: 2nd step of changing Pinbc (s-mode) +*/
  int    i, j, k ;

  int    part ;
  int    npart = COM_GID.nparent ;


  /* trade data within part */
  for( part=0 ; part<npart ; part++ ) {
    make_PartMesh_change_innerbc_change_Pinbc_2nd_sub( pm[part],
                                                       tfree[part],
                                                       op[part],
                                                       pinbc[part] ) ;
  }


  k = npart - 1 ;
  for( part=0 ; part<k ; part++ ) {
    Pinbc*  pinbc_yng = pinbc[part] ;
    for( i=part+1 ; i<npart ; i++ ) {
      Pinbc*  pinbc_old = pinbc[i] ;
      OPinfo* op_yng = &(op[part][i]) ;
      OPinfo* op_old = &(op[i][part]) ;
      int    n_mofree = op_yng->n_mofree ;
      for( j=0 ; j<n_mofree ; j++ ) {
        int    ii_yng = op_yng->ibid[j] ;
        int    ii_old = op_old->ibid[j] ;
        pinbc_yng[ii_yng].old_part[pinbc_yng[ii_yng].old_npart] = i ;
        pinbc_yng[ii_yng].old_nf_part[pinbc_yng[ii_yng].old_npart] =
                                                    pinbc_old[ii_old].nf; 
        pinbc_yng[ii_yng].old_npart++ ;

        pinbc_old[ii_old].yng_part[pinbc_old[ii_old].yng_npart] = part ;
        pinbc_old[ii_old].yng_nf_part[pinbc_old[ii_old].yng_npart] =
                                                    pinbc_yng[ii_yng].nf; 
        pinbc_old[ii_old].yng_npart++ ;
      }
    }
  }
}

/********************************************************************/

void make_PartMesh_change_innerbc_change_Pinbc_5th_s( OPinfo **op,
                                                      Pinbc **pinbc )
{
  /*+ Function: 5th step of changing Pinbc (s-mode) +*/
  /*+           5th step of change local interface B.C. number
                                                     in charge part +*/
  int    i, j, k ;

  int    part ;
  int    npart = COM_GID.nparent ;


  k = npart - 1 ;
  for( part=0 ; part<k ; part++ ) {
    Pinbc*  pinbc_yng = pinbc[part] ;
    for( i=part+1 ; i<npart ; i++ ) {
      Pinbc*  pinbc_old = pinbc[i] ;
      OPinfo* op_yng = &(op[part][i]) ;
      OPinfo* op_old = &(op[i][part]) ;
      int    n_mofree = op_yng->n_mofree ;
      for( j=0 ; j<n_mofree ; j++ ) {
        int    ii_yng = op_yng->ibid[j] ;
        int    ii_old = op_old->ibid[j] ;
        if( pinbc_old[ii_old].charge_part == part ) {
          pinbc_old[ii_old].ln_chpar = pinbc_yng[ii_yng].ln_part ;
        }
      }
    }
  }
}
