/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "struct.h"


extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;


void init_FaceGroup( FaceGroup *fgr )
{
  /*+ Funciton: initialize struct FaceGroup +*/
  fgr->n = 0 ;
  fgr->nfgr = 0 ;
  fgr->nface = NULL ;
  fgr->el = NULL ;
  fgr->face = NULL ;
  fgr->nop = NULL ;
}

/********************************************************************/

void free_FaceGroup( FaceGroup *fgr )
{
  /*+ Funciton: free struct FaceGroup +*/
  int    i ;

  int    nd_face = 0 ;


  if( fgr->n == 4 ) {
    nd_face = 3 ;
  } else if( fgr->n == 10 ) {
    nd_face = 6 ;
  } else if( fgr->n == 8 ) {
    nd_face = 4 ;
  } else if( fgr->n == 20 ) {
    nd_face = 8 ;
  } else {
    char str[1024] ;
    sprintf( str, "%d", fgr->n ) ;
    emessage( 126, __ABORT_ADVFW_LIB__, str ) ;
  }



  for( i=0 ; i<fgr->nfgr ; i++ ) {
    if( fgr->el[i] != NULL ) {
      Free( sizeof(int)*fgr->nface[i], fgr->el[i] ) ;
    }
    if( fgr->face[i] != NULL ) {
      Free( sizeof(int)*fgr->nface[i], fgr->face[i] ) ;
    }
    if( fgr->nop[i] != NULL ) {
      Free( sizeof(int)*fgr->nface[i]*nd_face, fgr->nop[i] ) ;
    }
  }
  if( fgr->nface != NULL ) {
    Free( sizeof(int)*fgr->nfgr, fgr->nface ) ;
  }
  if( fgr->el != NULL ) {
    Free( sizeof(int*)*fgr->nfgr, fgr->el ) ;
  }
  if( fgr->face != NULL ) {
    Free( sizeof(int*)*fgr->nfgr, fgr->face ) ;
  }
  if( fgr->nop != NULL ) {
    Free( sizeof(int*)*fgr->nfgr, fgr->nop ) ;
  }
  fgr->n = 0 ;
  fgr->nfgr = 0 ;
}

/********************************************************************/

void init_MTRL_ANTENNA_DEF_PP( MTRL_ANTENNA_DEF_PP *pp )
{
  /*+ Funciton: initialize struct MTRL_ANTENNA_DEF_PP +*/
  int    i, j ;
  pp->n = 0 ;
  for( i=0 ; i<5 ; i++ ) {
    for( j=0 ; j<3 ; j++ ) {
      pp->ap[i][j] = 0.0 ;
    }
  }
  strcpy( pp->analysis, "" ) ;
  for( i=0 ; i<3 ; i++ ) {
    pp->Jor[i] = 0.0 ;
    pp->Joi[i] = 0.0 ;
    for( j=0 ; j<3 ; j++ ) {
      pp->abc[i][j] = 0.0 ;
    }
  }
  for( i=0 ; i<2 ; i++ ) {
    for( j=0 ; j<3 ; j++ ) {
      pp->d[i][j] = 0.0 ;
    }
  }
  pp->tolerance = 0.0 ;
}

/********************************************************************/

void free_MTRL_ANTENNA_DEF_PP( MTRL_ANTENNA_DEF_PP *pp )
{
  /*+ Funciton: free struct MTRL_ANTENNA_DEF_PP +*/
  init_MTRL_ANTENNA_DEF_PP( pp ) ;
}

/********************************************************************/

void Copy_MTRL_ANTENNA_DEF_PP( MTRL_ANTENNA_DEF_PP *a, MTRL_ANTENNA_DEF_PP b )
{
  /*+ Funciton: Copy struct MTRL_ANTENNA_DEF_PP b to *a +*/
  int    i, j ;
  init_MTRL_ANTENNA_DEF_PP( a ) ;
  a->n = b.n ;
  for( i=0 ; i<5 ; i++ ) {
    for( j=0 ; j<3 ; j++ ) {
      a->ap[i][j] = b.ap[i][j] ;
    }
  }
  sprintf( a->analysis, "%s", b.analysis ) ;
  for( i=0 ; i<3 ; i++ ) {
    a->Jor[i] = b.Jor[i] ;
    a->Joi[i] = b.Joi[i] ;
    for( j=0 ; j<3 ; j++ ) {
      a->abc[i][j] = b.abc[i][j] ;
    }
  }
  for( i=0 ; i<2 ; i++ ) {
    for( j=0 ; j<3 ; j++ ) {
      a->d[i][j] = b.d[i][j] ;
    }
  }
  a->tolerance = b.tolerance ;
}

/********************************************************************/

void Bcast_Grp_MTRL_ANTENNA_DEF_PP( MTRL_ANTENNA_DEF_PP *pp )
{
  /*+ Funciton: Bcast struct MTRL_ANTENNA_DEF_PP +*/
  int    i ;
  if( COM_GID.rootid != COM_ID.myprocid ) {
    init_MTRL_ANTENNA_DEF_PP( pp ) ;
  }
  COM_Grp_Bcast_Int( &(pp->n), 1, COM_GID.rootid ) ;
  for( i=0 ; i<5 ; i++ ) {
    COM_Grp_Bcast_Dbl( pp->ap[i], 3, COM_GID.rootid ) ;
  }
  COM_Grp_Bcast_Chr( pp->analysis, 128, COM_GID.rootid ) ;
  COM_Grp_Bcast_Dbl( pp->Jor, 3, COM_GID.rootid ) ;
  COM_Grp_Bcast_Dbl( pp->Joi, 3, COM_GID.rootid ) ;
  for( i=0 ; i<3 ; i++ ) {
    COM_Grp_Bcast_Dbl( pp->abc[i], 3, COM_GID.rootid ) ;
  }
  for( i=0 ; i<2 ; i++ ) {
    COM_Grp_Bcast_Dbl( pp->d[i], 3, COM_GID.rootid ) ;
  }
  COM_Grp_Bcast_Dbl( &(pp->tolerance), 1, COM_GID.rootid ) ;
}

/********************************************************************/

void Bcast_MTRL_ANTENNA_DEF_PP( MTRL_ANTENNA_DEF_PP *pp )
{
  /*+ Funciton: Bcast struct MTRL_ANTENNA_DEF_PP +*/
  int    i ;
  if( COM_GID.rootid != COM_ID.myprocid ) {
    init_MTRL_ANTENNA_DEF_PP( pp ) ;
  }
  COM_Bcast_Int( &(pp->n), 1, COM_GID.rootid ) ;
  for( i=0 ; i<5 ; i++ ) {
    COM_Bcast_Dbl( pp->ap[i], 3, COM_GID.rootid ) ;
  }
  COM_Bcast_Chr( pp->analysis, 128, COM_GID.rootid ) ;
  COM_Bcast_Dbl( pp->Jor, 3, COM_GID.rootid ) ;
  COM_Bcast_Dbl( pp->Joi, 3, COM_GID.rootid ) ;
  for( i=0 ; i<3 ; i++ ) {
    COM_Bcast_Dbl( pp->abc[i], 3, COM_GID.rootid ) ;
  }
  for( i=0 ; i<2 ; i++ ) {
    COM_Bcast_Dbl( pp->d[i], 3, COM_GID.rootid ) ;
  }
  COM_Bcast_Dbl( &(pp->tolerance), 1, COM_GID.rootid ) ;
}

/********************************************************************/

void init_MTRL_ANTENNA_DEF_DSC( MTRL_ANTENNA_DEF_DSC *dsc )
{
  /*+ Funciton: initialize struct MTRL_ANTENNA_DEF_DSC +*/
  int    i ;
  dsc->n = 0 ;
  dsc->xyz = 0 ;
  for( i=0 ; i<4 ; i++ ) {
    dsc->ap[i] = 0.0 ;
  }
  for( i=0 ; i<2 ; i++ ) {
    dsc->theta[i] = 0.0 ;
    dsc->r[i] = 0.0 ;
  }
  strcpy( dsc->analysis, "" ) ;
  dsc->Jor = 0.0 ;
  dsc->Joi = 0.0 ;
}

/********************************************************************/

void free_MTRL_ANTENNA_DEF_DSC( MTRL_ANTENNA_DEF_DSC *dsc )
{
  /*+ Funciton: free struct MTRL_ANTENNA_DEF_DSC +*/
  init_MTRL_ANTENNA_DEF_DSC( dsc ) ;
}

/********************************************************************/

void Copy_MTRL_ANTENNA_DEF_DSC( MTRL_ANTENNA_DEF_DSC *a, MTRL_ANTENNA_DEF_DSC b )
{
  /*+ Funciton: Copy struct MTRL_ANTENNA_DEF_DSC b to *a +*/
  int    i ;
  init_MTRL_ANTENNA_DEF_DSC( a ) ;
  a->n = b.n ;
  a->xyz = b.xyz ;
  for( i=0 ; i<4 ; i++ ) {
    a->ap[i] = b.ap[i] ;
  }
  for( i=0 ; i<2 ; i++ ) {
    a->theta[i] = b.theta[i] ;
    a->r[i] = b.r[i] ;
  }
  sprintf( a->analysis, "%s", b.analysis ) ;
  a->Jor = b.Jor ;
  a->Joi = b.Joi ;
}

/********************************************************************/

void Bcast_Grp_MTRL_ANTENNA_DEF_DSC( MTRL_ANTENNA_DEF_DSC *dsc )
{
  /*+ Funciton: Bcast struct MTRL_ANTENNA_DEF_DSC +*/
  if( COM_GID.rootid != COM_ID.myprocid ) {
    init_MTRL_ANTENNA_DEF_DSC( dsc ) ;
  }
  COM_Grp_Bcast_Int( &(dsc->n), 1, COM_GID.rootid ) ;
  COM_Grp_Bcast_Int( &(dsc->xyz), 1, COM_GID.rootid ) ;
  COM_Grp_Bcast_Dbl( dsc->ap, 4, COM_GID.rootid ) ;
  COM_Grp_Bcast_Dbl( dsc->theta, 2, COM_GID.rootid ) ;
  COM_Grp_Bcast_Dbl( dsc->r, 2, COM_GID.rootid ) ;
  COM_Grp_Bcast_Chr( dsc->analysis, 128, COM_GID.rootid ) ;
  COM_Grp_Bcast_Dbl( &(dsc->Jor), 1, COM_GID.rootid ) ;
  COM_Grp_Bcast_Dbl( &(dsc->Joi), 1, COM_GID.rootid ) ;
}

/********************************************************************/

void Bcast_MTRL_ANTENNA_DEF_DSC( MTRL_ANTENNA_DEF_DSC *dsc )
{
  /*+ Funciton: Bcast struct MTRL_ANTENNA_DEF_DSC +*/
  if( COM_GID.rootid != COM_ID.myprocid ) {
    init_MTRL_ANTENNA_DEF_DSC( dsc ) ;
  }
  COM_Bcast_Int( &(dsc->n), 1, COM_GID.rootid ) ;
  COM_Bcast_Int( &(dsc->xyz), 1, COM_GID.rootid ) ;
  COM_Bcast_Dbl( dsc->ap, 4, COM_GID.rootid ) ;
  COM_Bcast_Dbl( dsc->theta, 2, COM_GID.rootid ) ;
  COM_Bcast_Dbl( dsc->r, 2, COM_GID.rootid ) ;
  COM_Bcast_Chr( dsc->analysis, 128, COM_GID.rootid ) ;
  COM_Bcast_Dbl( &(dsc->Jor), 1, COM_GID.rootid ) ;
  COM_Bcast_Dbl( &(dsc->Joi), 1, COM_GID.rootid ) ;
}

/********************************************************************/

void init_MTRL_ANTENNA_DEFINITION( MTRL_ANTENNA_DEFINITION *antenna_def )
{
  /*+ Funciton: initialize struct MTRL_ANTENNA_DEFINITION +*/
  antenna_def->flag = 0 ;
  antenna_def->mode = 0 ;
  strcpy( antenna_def->fname, "" ) ;

  antenna_def->ndef = 0 ;
  antenna_def->npp = 0 ;
  antenna_def->ndsc = 0 ;
  antenna_def->pp = NULL ;
  antenna_def->dsc = NULL ;
}

/********************************************************************/

void free_MTRL_ANTENNA_DEFINITION( MTRL_ANTENNA_DEFINITION *antenna_def )
{
  /*+ Funciton: free struct MTRL_ANTENNA_DEFINITION +*/
  int    i ;
  antenna_def->flag = 0 ;
  antenna_def->mode = 0 ;
  strcpy( antenna_def->fname, "" ) ;
  if( antenna_def->pp != NULL ) {
    for( i=0 ; i<antenna_def->npp ; i++ ) {
      free_MTRL_ANTENNA_DEF_PP( &(antenna_def->pp[i]) ) ;
    }
    Free( sizeof(MTRL_ANTENNA_DEF_PP)*antenna_def->npp, antenna_def->pp ) ;
  }
  if( antenna_def->dsc != NULL ) {
    for( i=0 ; i<antenna_def->ndsc ; i++ ) {
      free_MTRL_ANTENNA_DEF_DSC( &(antenna_def->dsc[i]) ) ;
    }
    Free( sizeof(MTRL_ANTENNA_DEF_DSC)*antenna_def->ndsc, antenna_def->dsc ) ;
  }
  antenna_def->ndef = 0 ;
  antenna_def->npp = 0 ;
  antenna_def->ndsc = 0 ;
}

/********************************************************************/

void Copy_MTRL_ANTENNA_DEFINITION( MTRL_ANTENNA_DEFINITION *a,
                                MTRL_ANTENNA_DEFINITION b )
{
  /*+ Funciton: Copy struct MTRL_ANTENNA_DEFINITION b to *a +*/
  int    i ;
  init_MTRL_ANTENNA_DEFINITION( a ) ;
  a->flag = b.flag ;
  a->mode = b.mode ;
  sprintf( a->fname, "%s", b.fname ) ;

  a->ndef = b.ndef ;
  a->npp = b.npp ;
  a->ndsc = b.ndsc ;
  if( b.npp != 0 ) {
    a->pp = (MTRL_ANTENNA_DEF_PP *)Malloc( sizeof(MTRL_ANTENNA_DEF_PP),
                                        b.npp ) ;
    for( i=0 ; i<b.npp ; i++ ) {
      Copy_MTRL_ANTENNA_DEF_PP( &(a->pp[i]), b.pp[i] ) ;
    }
  }
  if( b.ndsc != 0 ) {
    a->dsc = (MTRL_ANTENNA_DEF_DSC *)Malloc( sizeof(MTRL_ANTENNA_DEF_DSC),
                                          b.ndsc ) ;
    for( i=0 ; i<b.ndsc ; i++ ) {
      Copy_MTRL_ANTENNA_DEF_DSC( &(a->dsc[i]), b.dsc[i] ) ;
    }
  }
}

/********************************************************************/

void Bcast_Grp_MTRL_ANTENNA_DEFINITION( MTRL_ANTENNA_DEFINITION *antenna_def )
{
  /*+ Funciton: Bcast struct MTRL_ANTENNA_DEFINITION +*/
  int    i ;
  if( COM_GID.rootid != COM_ID.myprocid ) {
    init_MTRL_ANTENNA_DEFINITION( antenna_def ) ;
  }
  COM_Grp_Bcast_Int( &(antenna_def->flag), 1, COM_GID.rootid ) ;
  COM_Grp_Bcast_Int( &(antenna_def->mode), 1, COM_GID.rootid ) ;
  COM_Grp_Bcast_Chr( antenna_def->fname, FILENAME_MAX, COM_GID.rootid ) ;
  COM_Grp_Bcast_Int( &(antenna_def->ndef), 1, COM_GID.rootid ) ;
  COM_Grp_Bcast_Int( &(antenna_def->npp), 1, COM_GID.rootid ) ;
  COM_Grp_Bcast_Int( &(antenna_def->ndsc), 1, COM_GID.rootid ) ;
  if( antenna_def->npp != 0 ) {
    if( COM_GID.rootid != COM_ID.myprocid ) {
      antenna_def->pp =
              (MTRL_ANTENNA_DEF_PP *)Malloc( sizeof(MTRL_ANTENNA_DEF_PP),
                                          antenna_def->npp ) ;
    }
    for( i=0 ; i<antenna_def->npp ; i++ ) {
      Bcast_Grp_MTRL_ANTENNA_DEF_PP( &(antenna_def->pp[i]) ) ;
    }
  }
  if( antenna_def->ndsc != 0 ) {
    if( COM_GID.rootid != COM_ID.myprocid ) {
      antenna_def->dsc =
              (MTRL_ANTENNA_DEF_DSC *)Malloc( sizeof(MTRL_ANTENNA_DEF_DSC),
                                           antenna_def->ndsc ) ;
    }
    for( i=0 ; i<antenna_def->ndsc ; i++ ) {
      Bcast_Grp_MTRL_ANTENNA_DEF_DSC( &(antenna_def->dsc[i]) ) ;
    }
  }
}

/********************************************************************/

void Bcast_MTRL_ANTENNA_DEFINITION( MTRL_ANTENNA_DEFINITION *antenna_def )
{
  /*+ Funciton: Bcast struct MTRL_ANTENNA_DEFINITION +*/
  int    i ;
  if( COM_GID.rootid != COM_ID.myprocid ) {
    init_MTRL_ANTENNA_DEFINITION( antenna_def ) ;
  }
  COM_Bcast_Int( &(antenna_def->flag), 1, COM_GID.rootid ) ;
  COM_Bcast_Int( &(antenna_def->mode), 1, COM_GID.rootid ) ;
  COM_Bcast_Chr( antenna_def->fname, FILENAME_MAX, COM_GID.rootid ) ;
  COM_Bcast_Int( &(antenna_def->ndef), 1, COM_GID.rootid ) ;
  COM_Bcast_Int( &(antenna_def->npp), 1, COM_GID.rootid ) ;
  COM_Bcast_Int( &(antenna_def->ndsc), 1, COM_GID.rootid ) ;
  if( antenna_def->npp != 0 ) {
    if( COM_GID.rootid != COM_ID.myprocid ) {
      antenna_def->pp =
              (MTRL_ANTENNA_DEF_PP *)Malloc( sizeof(MTRL_ANTENNA_DEF_PP),
                                          antenna_def->npp ) ;
    }
    for( i=0 ; i<antenna_def->npp ; i++ ) {
      Bcast_MTRL_ANTENNA_DEF_PP( &(antenna_def->pp[i]) ) ;
    }
  }
  if( antenna_def->ndsc != 0 ) {
    if( COM_GID.rootid != COM_ID.myprocid ) {
      antenna_def->dsc =
              (MTRL_ANTENNA_DEF_DSC *)Malloc( sizeof(MTRL_ANTENNA_DEF_DSC),
                                           antenna_def->ndsc ) ;
    }
    for( i=0 ; i<antenna_def->ndsc ; i++ ) {
      Bcast_MTRL_ANTENNA_DEF_DSC( &(antenna_def->dsc[i]) ) ;
    }
  }
}

/********************************************************************/

void init_MTRL( MTRL *mtrl )
{
  /*+ Funciton: initialize struct MTRL +*/
  mtrl->nvol = 0 ;
  mtrl->vol = NULL ;

  mtrl->nbc = 0 ;
  mtrl->bci = NULL ;

  mtrl->nflag = 0 ;
  mtrl->flag_i = NULL ;
  mtrl->flag_nu = NULL ;

  mtrl->nantenna = 0 ;
  mtrl->antenna_def = NULL ;
  mtrl->nantenna_omega = 0 ;
  mtrl->antenna_omega = 0.0 ;

  mtrl->nR = 0 ;
  mtrl->R_i = NULL ;
  mtrl->R_sigma = NULL ;
}

/********************************************************************/

void free_MTRL( MTRL *mtrl )
{
  /*+ Funciton: free struct MTRL +*/
  if( mtrl->vol != NULL ) {
    Free( sizeof(int)*mtrl->nvol, mtrl->vol ) ;
  }
  mtrl->nvol = 0 ;

  if( mtrl->bci != NULL ) {
    Free( sizeof(int)*mtrl->nbc, mtrl->bci ) ;
  }
  mtrl->nbc = 0 ;

  if( mtrl->flag_i != NULL ) {
    Free( sizeof(int)*mtrl->nflag, mtrl->flag_i ) ;
  }
  if( mtrl->flag_nu != NULL ) {
    Free( sizeof(double)*mtrl->nflag, mtrl->flag_nu ) ;
  }
  mtrl->nflag = 0 ;


  if( mtrl->antenna_def != NULL ) {
    int    ii ;
    for( ii=0 ; ii<mtrl->nantenna ; ii++ ) {
      free_MTRL_ANTENNA_DEFINITION( &(mtrl->antenna_def[ii]) ) ;
    }
    Free( sizeof(MTRL_ANTENNA_DEFINITION)*mtrl->nantenna, mtrl->antenna_def ) ;
  }
  mtrl->nantenna = 0 ;

  if( mtrl->nantenna_omega != 0 ) {
    mtrl->nantenna_omega = 0 ;
    mtrl->antenna_omega = 0.0 ;
  }

  if( mtrl->R_i != NULL ) {
    Free( sizeof(int)*mtrl->nR, mtrl->R_i ) ;
  }
  if( mtrl->R_sigma != NULL ) {
    Free( sizeof(double)*mtrl->nR, mtrl->R_sigma ) ;
  }
  mtrl->nR = 0 ;
}

/********************************************************************/

void Copy_MTRL( MTRL *a, MTRL b )
{
  /*+ Funciton: Copy struct MTRL b to *a +*/
  init_MTRL( a ) ;
  a->nvol = b.nvol ;
  if( b.vol != NULL ) {
    a->vol = (int *)Malloc( sizeof(int), b.nvol ) ;
    memcpy( a->vol, b.vol, sizeof(int)*b.nvol ) ;
  }

  a->nbc = b.nbc ;
  if( b.bci !=NULL ) {
    a->bci = (int *)Malloc( sizeof(int), b.nbc ) ;
    memcpy( a->bci, b.bci, sizeof(int)*b.nbc ) ;
  }

  a->nflag = b.nflag ;
  if( b.flag_i !=NULL ) {
    a->flag_i = (int *)Malloc( sizeof(int), b.nflag ) ;
    memcpy( a->flag_i, b.flag_i, sizeof(int)*b.nflag ) ;
  }
  if( b.flag_nu !=NULL ) {
    a->flag_nu = (double *)Malloc( sizeof(double), b.nflag ) ;
    memcpy( a->flag_nu, b.flag_nu, sizeof(double)*b.nflag ) ;
  }

  a->nantenna = b.nantenna ;
  if( b.antenna_def != NULL ) {
    int    ii ;
    a->antenna_def = (MTRL_ANTENNA_DEFINITION *)Malloc( sizeof(MTRL_ANTENNA_DEFINITION),
                                                  b.nantenna ) ;
    for( ii=0 ; ii<b.nantenna ; ii++ ) {
      Copy_MTRL_ANTENNA_DEFINITION( &(a->antenna_def[ii]), b.antenna_def[ii] ) ;
    }
  }

  a->nantenna_omega = b.nantenna_omega ;
  a->antenna_omega = b.antenna_omega ;

  a->nR = b.nR ;
  if( b.R_i !=NULL ) {
    a->R_i = (int *)Malloc( sizeof(int), b.nR ) ;
    memcpy( a->R_i, b.R_i, sizeof(int)*b.nR ) ;
  }
  if( b.R_sigma !=NULL ) {
    a->R_sigma = (double *)Malloc( sizeof(double), b.nR ) ;
    memcpy( a->R_sigma, b.R_sigma, sizeof(double)*b.nR ) ;
  }
}

/********************************************************************/

void Bcast_Grp_MTRL( MTRL *mtrl )
{
  /*+ Funciton: Bcast struct MTRL +*/
  if( COM_GID.rootid != COM_ID.myprocid ) {
    init_MTRL( mtrl ) ;
  }
  COM_Grp_Bcast_Int( &(mtrl->nvol), 1, COM_GID.rootid ) ;
  if( mtrl->nvol != 0 ) {
    if( COM_GID.rootid != COM_ID.myprocid ) {
      mtrl->vol = (int *)Malloc( sizeof(int), mtrl->nvol ) ;
    }
    COM_Grp_Bcast_Int( mtrl->vol, mtrl->nvol, COM_GID.rootid ) ;
  }


  COM_Grp_Bcast_Int( &(mtrl->nbc), 1, COM_GID.rootid ) ;
  if( mtrl->nbc != 0 ) {
    if( COM_GID.rootid != COM_ID.myprocid ) {
      mtrl->bci = (int *)Malloc( sizeof(int), mtrl->nbc ) ;
    }
    COM_Grp_Bcast_Int( mtrl->bci, mtrl->nbc, COM_GID.rootid ) ;
  }


  COM_Grp_Bcast_Int( &(mtrl->nflag), 1, COM_GID.rootid ) ;
  if( mtrl->nflag != 0 ) {
    if( COM_GID.rootid != COM_ID.myprocid ) {
      mtrl->flag_i = (int *)Malloc( sizeof(int), mtrl->nflag ) ;
      mtrl->flag_nu = (double *)Malloc( sizeof(double), mtrl->nflag ) ;
    }
    COM_Grp_Bcast_Int( mtrl->flag_i, mtrl->nflag, COM_GID.rootid ) ;
    COM_Grp_Bcast_Dbl( mtrl->flag_nu, mtrl->nflag, COM_GID.rootid ) ;
  }


  COM_Grp_Bcast_Int( &(mtrl->nantenna), 1, COM_GID.rootid ) ;
  if( mtrl->nantenna != 0 ) {
    int    ii ;
    if( COM_GID.rootid != COM_ID.myprocid ) {
      mtrl->antenna_def =
               (MTRL_ANTENNA_DEFINITION *)Malloc( sizeof(MTRL_ANTENNA_DEFINITION),
                                               mtrl->nantenna ) ;
    }
    for( ii=0 ; ii<mtrl->nantenna ; ii++ ) {
      Bcast_Grp_MTRL_ANTENNA_DEFINITION( &(mtrl->antenna_def[ii]) ) ;
    }
  }

  COM_Grp_Bcast_Int( &(mtrl->nantenna_omega), 1, COM_GID.rootid ) ;
  if( mtrl->nantenna_omega != 0 ) {
    COM_Grp_Bcast_Dbl( &(mtrl->antenna_omega), 1, COM_GID.rootid ) ;
  }



  COM_Grp_Bcast_Int( &(mtrl->nR), 1, COM_GID.rootid ) ;
  if( mtrl->nR != 0 ) {
    if( COM_GID.rootid != COM_ID.myprocid ) {
      mtrl->R_i = (int *)Malloc( sizeof(int), mtrl->nR ) ;
      mtrl->R_sigma = (double *)Malloc( sizeof(double), mtrl->nR ) ;
    }
    COM_Grp_Bcast_Int( mtrl->R_i, mtrl->nR, COM_GID.rootid ) ;
    COM_Grp_Bcast_Dbl( mtrl->R_sigma, mtrl->nR, COM_GID.rootid ) ;
  }
}

/********************************************************************/

void Bcast_MTRL( MTRL *mtrl )
{
  /*+ Funciton: Bcast struct MTRL +*/
  if( COM_GID.rootid != COM_ID.myprocid ) {
    init_MTRL( mtrl ) ;
  }
  COM_Bcast_Int( &(mtrl->nvol), 1, COM_GID.rootid ) ;
  if( mtrl->nvol != 0 ) {
    if( COM_GID.rootid != COM_ID.myprocid ) {
      mtrl->vol = (int *)Malloc( sizeof(int), mtrl->nvol ) ;
    }
    COM_Bcast_Int( mtrl->vol, mtrl->nvol, COM_GID.rootid ) ;
  }


  COM_Bcast_Int( &(mtrl->nbc), 1, COM_GID.rootid ) ;
  if( mtrl->nbc != 0 ) {
    if( COM_GID.rootid != COM_ID.myprocid ) {
      mtrl->bci = (int *)Malloc( sizeof(int), mtrl->nbc ) ;
    }
    COM_Bcast_Int( mtrl->bci, mtrl->nbc, COM_GID.rootid ) ;
  }


  COM_Bcast_Int( &(mtrl->nflag), 1, COM_GID.rootid ) ;
  if( mtrl->nflag != 0 ) {
    if( COM_GID.rootid != COM_ID.myprocid ) {
      mtrl->flag_i = (int *)Malloc( sizeof(int), mtrl->nflag ) ;
      mtrl->flag_nu = (double *)Malloc( sizeof(double), mtrl->nflag ) ;
    }
    COM_Bcast_Int( mtrl->flag_i, mtrl->nflag, COM_GID.rootid ) ;
    COM_Bcast_Dbl( mtrl->flag_nu, mtrl->nflag, COM_GID.rootid ) ;
  }


  COM_Bcast_Int( &(mtrl->nantenna), 1, COM_GID.rootid ) ;
  if( mtrl->nantenna != 0 ) {
    int    ii ;
    if( COM_GID.rootid != COM_ID.myprocid ) {
      mtrl->antenna_def =
               (MTRL_ANTENNA_DEFINITION *)Malloc( sizeof(MTRL_ANTENNA_DEFINITION),
                                               mtrl->nantenna ) ;
    }
    for( ii=0 ; ii<mtrl->nantenna ; ii++ ) {
      Bcast_MTRL_ANTENNA_DEFINITION( &(mtrl->antenna_def[ii]) ) ;
    }
  }

  COM_Bcast_Int( &(mtrl->nantenna_omega), 1, COM_GID.rootid ) ;
  if( mtrl->nantenna_omega != 0 ) {
    COM_Bcast_Dbl( &(mtrl->antenna_omega), 1, COM_GID.rootid ) ;
  }




  COM_Bcast_Int( &(mtrl->nR), 1, COM_GID.rootid ) ;
  if( mtrl->nR != 0 ) {
    if( COM_GID.rootid != COM_ID.myprocid ) {
      mtrl->R_i = (int *)Malloc( sizeof(int), mtrl->nR ) ;
      mtrl->R_sigma = (double *)Malloc( sizeof(double), mtrl->nR ) ;
    }
    COM_Bcast_Int( mtrl->R_i, mtrl->nR, COM_GID.rootid ) ;
    COM_Bcast_Dbl( mtrl->R_sigma, mtrl->nR, COM_GID.rootid ) ;
  }
}

/********************************************************************/

void init_DirichletBC( DirichletBC *dbc )
{
  /*+ Funciton: initialize struct DirichletBC +*/
  dbc->need = NO_NEED ;
  dbc->nbc = 0 ;
  dbc->ibc = NULL ;
  dbc->vbc = NULL ;
  dbc->vbc_c = NULL ;
}

/********************************************************************/

void free_DirichletBC( DirichletBC *dbc )
{
  /*+ Funciton: free struct DirichletBC +*/
  if( dbc->ibc != NULL ) {
    Free( sizeof(int)*dbc->nbc, dbc->ibc ) ;
  }
  if( dbc->vbc != NULL ) {
    Free( sizeof(double)*dbc->nbc, dbc->vbc ) ;
  }
  if( dbc->vbc_c != NULL ) {
    Free( sizeof(Complex)*dbc->nbc, dbc->vbc_c ) ;
  }
  dbc->need = NO_NEED ;
  dbc->nbc = 0 ;
}

/********************************************************************/

void Bcast_Grp_DirichletBC( DirichletBC *dbc )
{
  /*+ Funciton: Bcast struct DirichletBC +*/
  int    i = NO_NEED ;


  COM_Grp_Bcast_Int( &(dbc->need), 1, COM_GID.rootid ) ;
  if( dbc->need == NEED ) {
    COM_Grp_Bcast_Int( &(dbc->nbc), 1, COM_GID.rootid ) ;

    if( dbc->nbc != 0 ) {
      if( COM_GID.rootid == COM_ID.myprocid ) {
        if( dbc->ibc == NULL ) i = NO_NEED ;
        else i = NEED ;
      }
      COM_Grp_Bcast_Int( &i, 1, COM_GID.rootid ) ;
      if( i == NEED ) {
        if( COM_GID.rootid != COM_ID.myprocid ) {
          dbc->ibc = (int *)Malloc( sizeof(int), dbc->nbc ) ;
        }
        COM_Grp_Bcast_Int( dbc->ibc, dbc->nbc, COM_GID.rootid ) ;
      }

      if( COM_GID.rootid == COM_ID.myprocid ) {
        if( dbc->vbc == NULL ) i = NO_NEED ;
        else i = NEED ;
      }
      COM_Grp_Bcast_Int( &i, 1, COM_GID.rootid ) ;
      if( i == NEED ) {
        if( COM_GID.rootid != COM_ID.myprocid ) {
          dbc->vbc = (double *)Malloc( sizeof(double), dbc->nbc ) ;
        }
        COM_Grp_Bcast_Dbl( dbc->vbc, dbc->nbc, COM_GID.rootid ) ;
      }

      if( COM_GID.rootid == COM_ID.myprocid ) {
        if( dbc->vbc_c == NULL ) i = NO_NEED ;
        else i = NEED ;
      }
      COM_Grp_Bcast_Int( &i, 1, COM_GID.rootid ) ;
      if( i == NEED ) {
        if( COM_GID.rootid != COM_ID.myprocid ) {
          dbc->vbc_c = (Complex *)Malloc( sizeof(Complex), dbc->nbc ) ;
        }
        COM_Grp_Bcast_Cpl( dbc->vbc_c, dbc->nbc, COM_GID.rootid ) ;
      }
    }
  }
}

/********************************************************************/

void Bcast_DirichletBC( DirichletBC *dbc )
{
  /*+ Funciton: Bcast struct DirichletBC +*/
  int    i = NO_NEED ;


  COM_Bcast_Int( &(dbc->need), 1, COM_GID.rootid ) ;
  if( dbc->need == NEED ) {
    COM_Bcast_Int( &(dbc->nbc), 1, COM_GID.rootid ) ;

    if( dbc->nbc != 0 ) {
      if( COM_GID.rootid == COM_ID.myprocid ) {
        if( dbc->ibc == NULL ) i = NO_NEED ;
        else i = NEED ;
      }
      COM_Bcast_Int( &i, 1, COM_GID.rootid ) ;
      if( i == NEED ) {
        if( COM_GID.rootid != COM_ID.myprocid ) {
          dbc->ibc = (int *)Malloc( sizeof(int), dbc->nbc ) ;
        }
        COM_Bcast_Int( dbc->ibc, dbc->nbc, COM_GID.rootid ) ;
      }

      if( COM_GID.rootid == COM_ID.myprocid ) {
        if( dbc->vbc == NULL ) i = NO_NEED ;
        else i = NEED ;
      }
      COM_Bcast_Int( &i, 1, COM_GID.rootid ) ;
      if( i == NEED ) {
        if( COM_GID.rootid != COM_ID.myprocid ) {
          dbc->vbc = (double *)Malloc( sizeof(double), dbc->nbc ) ;
        }
        COM_Bcast_Dbl( dbc->vbc, dbc->nbc, COM_GID.rootid ) ;
      }

      if( COM_GID.rootid == COM_ID.myprocid ) {
        if( dbc->vbc_c == NULL ) i = NO_NEED ;
        else i = NEED ;
      }
      COM_Bcast_Int( &i, 1, COM_GID.rootid ) ;
      if( i == NEED ) {
        if( COM_GID.rootid != COM_ID.myprocid ) {
          dbc->vbc_c = (Complex *)Malloc( sizeof(Complex), dbc->nbc ) ;
        }
        COM_Bcast_Cpl( dbc->vbc_c, dbc->nbc, COM_GID.rootid ) ;
      }
    }
  }
}

/********************************************************************/

void send_DirichletBC( DirichletBC *dbc, OPTIONS options,
                       int compid, int type )
{
  /*+ Funciton: send struct DirichletBC +*/
  COM_WriteInt( &(dbc->need), 1, compid, type ) ;
  if( dbc->need == NEED ) {
    COM_WriteInt( &(dbc->nbc), 1, compid, type ) ;
    if( dbc->ibc != NULL ) {
      COM_WriteInt( &(dbc->nbc), 1, compid, type ) ;
      COM_WriteInt( dbc->ibc, dbc->nbc, compid, type ) ;
    } else {
      int    ii = 0 ;
      COM_WriteInt( &ii, 1, compid, type ) ;
    }
    if( dbc->vbc != NULL ) {
      COM_WriteInt( &(dbc->nbc), 1, compid, type ) ;
      COM_WriteDbl( dbc->vbc, dbc->nbc, compid, type ) ;
    } else {
      int    ii = 0 ;
      COM_WriteInt( &ii, 1, compid, type ) ;
    }
    if( dbc->vbc_c != NULL ) {
      COM_WriteInt( &(dbc->nbc), 1, compid, type ) ;
      COM_WriteCpl( dbc->vbc_c, dbc->nbc, compid, type ) ;
    } else {
      int    ii = 0 ;
      COM_WriteInt( &ii, 1, compid, type ) ;
    }
  }
}

/********************************************************************/

void recv_DirichletBC( DirichletBC *dbc, OPTIONS options,
                       int compid, int type )
{
  /*+ Funciton: recieve struct DirichletBC +*/
  COM_ReadInt( &(dbc->need), 1, &compid, &type ) ;
  if( dbc->need == NEED ) {
    int    ii = 0 ;
    COM_ReadInt( &(dbc->nbc), 1, &compid, &type ) ;
    COM_ReadInt( &ii, 1, &compid, &type ) ;
    if( ii > 0 ) {
      dbc->ibc = (int *)Malloc( sizeof(int), ii ) ;
      COM_ReadInt( dbc->ibc, ii, &compid, &type ) ;
      ii = 0 ;
    }
    COM_ReadInt( &ii, 1, &compid, &type ) ;
    if( ii > 0 ) {
      dbc->vbc = (double *)Malloc( sizeof(double), ii ) ;
      COM_ReadDbl( dbc->vbc, ii, &compid, &type ) ;
      ii = 0 ;
    }
    COM_ReadInt( &ii, 1, &compid, &type ) ;
    if( ii > 0 ) {
      dbc->vbc_c = (Complex *)Malloc( sizeof(Complex), ii ) ;
      COM_ReadCpl( dbc->vbc_c, ii, &compid, &type ) ;
      ii = 0 ;
    }
  }
}

/********************************************************************/

void init_ANTENNA( ANTENNA *antenna )
{
  /*+ Funciton: initialize struct ANTENNA +*/
  antenna->antenna = 0 ;

  antenna->nf = 0 ;
  antenna->enf = NULL ;
  antenna->apid2gnid = NULL ;

  antenna->Jo = NULL ;
  antenna->Jor = NULL ;
  antenna->Ihr = NULL ;
  antenna->Ihi = NULL ;

  init_DirichletBC( &(antenna->dbc) ) ;

  antenna->ninbc = 0 ;
  antenna->inbc = NULL ;
}

/********************************************************************/

void free_ANTENNA( ANTENNA *antenna, int nap, int dimension )
{
  /*+ Funciton: free struct ANTENNA +*/
  if( antenna->enf != NULL ) {
    Free( sizeof(int)*nap, antenna->enf ) ;
  }
  #ifndef __ADVFW_HIERARCHICAL_MODE__
    antenna->apid2gnid = NULL ;
  #else
    if( COM_ID.myprocid < COM_GID.nparent ) {
      antenna->apid2gnid = NULL ;
    } else {
      if( antenna->apid2gnid != NULL ) {
        Free( sizeof(int)*nap, antenna->apid2gnid ) ;
      }
    }
  #endif

  if( antenna->Jo != NULL ) {
    Free( sizeof(double)*antenna->nf*dimension, antenna->Jo ) ;
  }
  if( antenna->Jor != NULL ) {
    Free( sizeof(double)*antenna->nf*dimension, antenna->Jor ) ;
  }
  if( antenna->Ihr != NULL ) {
    Free( sizeof(double)*antenna->nf, antenna->Ihr ) ;
  }
  if( antenna->Ihi != NULL ) {
    Free( sizeof(double)*antenna->nf, antenna->Ihi ) ;
  }

  free_DirichletBC( &(antenna->dbc) ) ;

  #ifndef __ADVFW_HIERARCHICAL_MODE__
    antenna->ninbc = 0 ;
    antenna->inbc = NULL ;
  #else
    if( COM_ID.myprocid < COM_GID.nparent ) {
      antenna->ninbc = 0 ;
      antenna->inbc = NULL ;
    } else {
      if( antenna->inbc != NULL ) {
        int    ii ;
        for( ii=0 ; ii<antenna->ninbc ; ii++ ) {
          free_Inbc( &(antenna->inbc[ii]) ) ;
        }
        Free( sizeof(Inbc)*antenna->ninbc, antenna->inbc ) ;
        antenna->ninbc = 0 ;
      }
    }
  #endif

  antenna->antenna = 0 ;
  antenna->nf = 0 ;
}

/********************************************************************/

void send_ANTENNA( ANTENNA *antenna, OPTIONS options, int nap, int dimension,
                int compid, int type )
{
  /*+ Funciton: send struct ANTENNA +*/
  int    i ;


  COM_WriteInt( &(antenna->antenna), 1, compid, type ) ;

  COM_WriteInt( &(antenna->nf), 1, compid, type ) ;
  if( antenna->enf != NULL ) {
    COM_WriteInt( &nap, 1, compid, type ) ;
    COM_WriteInt( antenna->enf, nap, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( antenna->apid2gnid != NULL ) {
    COM_WriteInt( &nap, 1, compid, type ) ;
    COM_WriteInt( antenna->apid2gnid, nap, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }

  if( antenna->Jo != NULL ) {
    COM_WriteInt( &(antenna->nf), 1, compid, type ) ;
    COM_WriteDbl( antenna->Jo, antenna->nf*dimension, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( antenna->Jor != NULL ) {
    COM_WriteInt( &(antenna->nf), 1, compid, type ) ;
    COM_WriteDbl( antenna->Jor, antenna->nf*dimension, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( antenna->Ihr != NULL ) {
    COM_WriteInt( &(antenna->nf), 1, compid, type ) ;
    COM_WriteDbl( antenna->Ihr, antenna->nf, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( antenna->Ihi != NULL ) {
    COM_WriteInt( &(antenna->nf), 1, compid, type ) ;
    COM_WriteDbl( antenna->Ihi, antenna->nf, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }

  send_DirichletBC( &(antenna->dbc), options, compid, type ) ;

  COM_WriteInt( &(antenna->ninbc), 1, compid, type ) ;
  if( antenna->inbc != NULL ) {
    COM_WriteInt( &(antenna->ninbc), 1, compid, type ) ;
    for( i=0 ; i<antenna->ninbc ; i++ ) {
      send_Inbc( &(antenna->inbc[i]), options, compid, type ) ;
    }
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
}

/********************************************************************/

void recv_ANTENNA( ANTENNA *antenna, OPTIONS options, int nap, int dimension,
                int compid, int type )
{
  /*+ Funciton: recieve struct ANTENNA +*/
  int    i ;

  int    ii = 0 ;

  COM_ReadInt( &(antenna->antenna), 1, &compid, &type ) ;

  COM_ReadInt( &(antenna->nf), 1, &compid, &type ) ;
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    antenna->enf = (int *)Malloc( sizeof(int), ii ) ;
    COM_ReadInt( antenna->enf, ii, &compid, &type ) ;
    ii = 0 ;
  }
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    antenna->apid2gnid = (int *)Malloc( sizeof(int), ii ) ;
    COM_ReadInt( antenna->apid2gnid, ii, &compid, &type ) ;
    ii = 0 ;
  }
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    antenna->Jo = (double *)Malloc( sizeof(double), ii*dimension ) ;
    COM_ReadDbl( antenna->Jo, ii*dimension, &compid, &type ) ;
    ii = 0 ;
  }
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    antenna->Jor = (double *)Malloc( sizeof(double), ii*dimension ) ;
    COM_ReadDbl( antenna->Jor, ii*dimension, &compid, &type ) ;
    ii = 0 ;
  }
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    antenna->Ihr = (double *)Malloc( sizeof(double), ii ) ;
    COM_ReadDbl( antenna->Ihr, ii, &compid, &type ) ;
    ii = 0 ;
  }
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    antenna->Ihi = (double *)Malloc( sizeof(double), ii ) ;
    COM_ReadDbl( antenna->Ihi, ii, &compid, &type ) ;
    ii = 0 ;
  }

  recv_DirichletBC( &(antenna->dbc), options, compid, type ) ;

  COM_ReadInt( &(antenna->ninbc), 1, &compid, &type ) ;
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    antenna->inbc = (Inbc *)Malloc( sizeof(Inbc), ii ) ;
    for( i=0 ; i<antenna->ninbc ; i++ ) {
      recv_Inbc( &(antenna->inbc[i]), options, compid, type ) ;
    }
    ii = 0 ;
  }
}

/********************************************************************/

void init_DomANTENNA( DomANTENNA *dantenna )
{
  /*+ Funciton: initialize struct DomANTENNA +*/
  dantenna->ninbc = 0 ;
  dantenna->inbc = NULL ;
}

/********************************************************************/

void free_DomANTENNA( DomANTENNA *dantenna, int dimension )
{
  /*+ Funciton: free struct DomANTENNA +*/
  int    i ;
  if( dantenna->inbc != NULL ) {
    for( i=0 ; i<dantenna->ninbc ; i++ ) {
      free_Inbc( &(dantenna->inbc[i]) ) ;
    }
    Free( sizeof(Inbc)*dantenna->ninbc, dantenna->inbc ) ;
  }
  dantenna->ninbc = 0 ;
}

/********************************************************************/

void init_PartANTENNA( PartANTENNA *pantenna )
{
  /*+ Funciton: initialize struct PartANTENNA +*/
  pantenna->npart = 0 ;
  pantenna->ndomain = 0 ;
  pantenna->infree = 0 ;
  pantenna->outfree = 0 ;
  pantenna->tfree = 0 ;
  pantenna->total_free = 0 ;
  pantenna->op = NULL ;
  pantenna->dantenna = NULL ;
}

/********************************************************************/

void free_PartANTENNA( PartANTENNA *pantenna, int dimension )
{
  /*+ Funciton: free struct PartANTENNA +*/
  int    i ;
  if( pantenna->op != NULL ) {
    for( i=0 ; i<pantenna->npart ; i++ ) {
      free_OPinfo( &(pantenna->op[i]) ) ;
    }
    Free( sizeof(OPinfo)*pantenna->npart, pantenna->op ) ;
  }
  if( pantenna->dantenna != NULL ) {
    for( i=0 ; i<pantenna->ndomain ; i++ ) {
      free_DomANTENNA( &(pantenna->dantenna[i]), dimension ) ;
    }
    Free( sizeof(DomANTENNA)*pantenna->ndomain, pantenna->dantenna ) ;
  }
  pantenna->npart = 0 ;
  pantenna->ndomain = 0 ;
  pantenna->infree = 0 ;
  pantenna->outfree = 0 ;
  pantenna->tfree = 0 ;
  pantenna->total_free = 0 ;
}

/********************************************************************/

void init_ResultValue( ResultValue *rv, OPTIONS options )
{
  /*+ Funciton: initialize struct ResultValue +*/
  rv->B_d = NULL ;
  rv->B_c = NULL ;

  rv->E_c = NULL ;

  rv->Je_d = NULL ;
  rv->Je_c = NULL ;

  rv->el_B_d = NULL ;
  rv->el_B_c = NULL ;

  rv->el_E_c = NULL ;

  rv->el_Je_d = NULL ;
  rv->el_Je_c = NULL ;


}

/********************************************************************/

void free_ResultValue( ResultValue *rv, int nel, int nnd, int dimension,
                       OPTIONS options )
{
  /*+ Funciton: free struct ResultValue +*/


  if( rv->B_d != NULL ) {
    Free( sizeof(double)*nnd*dimension, rv->B_d ) ;
  }
  if( rv->B_c != NULL ) {
    Free( sizeof(Complex)*nnd*dimension, rv->B_c ) ;
  }
 

  if( rv->E_c != NULL ) {
    Free( sizeof(Complex)*nnd*dimension, rv->E_c ) ;
  }


  if( rv->Je_d != NULL ) {
    Free( sizeof(double)*nnd*dimension, rv->Je_d ) ;
  }
  if( rv->Je_c != NULL ) {
    Free( sizeof(Complex)*nnd*dimension, rv->Je_c ) ;
  }


  if( rv->el_B_d != NULL ) {
    Free( sizeof(double)*nel*dimension, rv->el_B_d ) ;
  }
  if( rv->el_B_c != NULL ) {
    Free( sizeof(Complex)*nel*dimension, rv->el_B_c ) ;
  }

 
  if( rv->el_E_c != NULL ) {
    Free( sizeof(Complex)*nel*dimension, rv->el_E_c ) ;
  }

  if( rv->el_Je_d != NULL ) {
    Free( sizeof(double)*nel*dimension, rv->el_Je_d ) ;
  }
  if( rv->el_Je_c != NULL ) {
    Free( sizeof(Complex)*nel*dimension, rv->el_Je_c ) ;
  }


}

/********************************************************************/

void send_ResultValue( ResultValue *rv, OPTIONS options,
                       int nel, int nap, int dimension,
                       int compid, int type )
{
  /*+ Funciton: send struct ResultValue +*/
  if( rv->B_d != NULL ) {
    COM_WriteInt( &(nap), 1, compid, type ) ;
    COM_WriteDbl( rv->B_d, nap*dimension, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( rv->B_c != NULL ) {
    COM_WriteInt( &(nap), 1, compid, type ) ;
    COM_WriteCpl( rv->B_c, nap*dimension, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }

  if( rv->E_c != NULL ) {
    COM_WriteInt( &(nap), 1, compid, type ) ;
    COM_WriteCpl( rv->E_c, nap*dimension, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }

  if( rv->Je_d != NULL ) {
    COM_WriteInt( &(nap), 1, compid, type ) ;
    COM_WriteDbl( rv->Je_d, nap*dimension, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( rv->Je_c != NULL ) {
    COM_WriteInt( &(nap), 1, compid, type ) ;
    COM_WriteCpl( rv->Je_c, nap*dimension, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }

  if( rv->el_B_d != NULL ) {
    COM_WriteInt( &(nel), 1, compid, type ) ;
    COM_WriteDbl( rv->el_B_d, nel*dimension, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( rv->el_B_c != NULL ) {
    COM_WriteInt( &(nel), 1, compid, type ) ;
    COM_WriteCpl( rv->el_B_c, nel*dimension, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }

  if( rv->el_E_c != NULL ) {
    COM_WriteInt( &(nel), 1, compid, type ) ;
    COM_WriteCpl( rv->el_E_c, nel*dimension, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }

  if( rv->el_Je_d != NULL ) {
    COM_WriteInt( &(nel), 1, compid, type ) ;
    COM_WriteDbl( rv->el_Je_d, nel*dimension, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( rv->el_Je_c != NULL ) {
    COM_WriteInt( &(nel), 1, compid, type ) ;
    COM_WriteCpl( rv->el_Je_c, nel*dimension, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
}

/********************************************************************/

void recv_ResultValue( ResultValue *rv, OPTIONS options,
                       int nel, int nap, int dimension,
                       int compid, int type )
{
  /*+ Funciton: recieve struct ResultValue +*/
  int    ii = 0 ;


  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    rv->B_d = (double *)Malloc( sizeof(double), ii*dimension ) ;
    COM_ReadDbl( rv->B_d, ii*dimension, &compid, &type ) ;
    ii = 0 ;
  }
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    rv->B_c = (Complex *)Malloc( sizeof(Complex), ii*dimension ) ;
    COM_ReadCpl( rv->B_c, ii*dimension, &compid, &type ) ;
    ii = 0 ;
  }

  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    rv->E_c = (Complex *)Malloc( sizeof(Complex), ii*dimension ) ;
    COM_ReadCpl( rv->E_c, ii*dimension, &compid, &type ) ;
    ii = 0 ;
  }


  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    rv->Je_d = (double *)Malloc( sizeof(double), ii*dimension ) ;
    COM_ReadDbl( rv->Je_d, ii*dimension, &compid, &type ) ;
    ii = 0 ;
  }
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    rv->Je_c = (Complex *)Malloc( sizeof(Complex), ii*dimension ) ;
    COM_ReadCpl( rv->Je_c, ii*dimension, &compid, &type ) ;
    ii = 0 ;
  }

  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    rv->el_B_d = (double *)Malloc( sizeof(double), ii*dimension ) ;
    COM_ReadDbl( rv->el_B_d, ii*dimension, &compid, &type ) ;
    ii = 0 ;
  }
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    rv->el_B_c = (Complex *)Malloc( sizeof(Complex), ii*dimension ) ;
    COM_ReadCpl( rv->el_B_c, ii*dimension, &compid, &type ) ;
    ii = 0 ;
  }

  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    rv->el_E_c = (Complex *)Malloc( sizeof(Complex), ii*dimension ) ;
    COM_ReadCpl( rv->el_E_c, ii*dimension, &compid, &type ) ;
    ii = 0 ;
  }

  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    rv->el_Je_d = (double *)Malloc( sizeof(double), ii*dimension ) ;
    COM_ReadDbl( rv->el_Je_d, ii*dimension, &compid, &type ) ;
    ii = 0 ;
  }
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    rv->el_Je_c = (Complex *)Malloc( sizeof(Complex), ii*dimension ) ;
    COM_ReadCpl( rv->el_Je_c, ii*dimension, &compid, &type ) ;
    ii = 0 ;
  }
}

/********************************************************************/

void init_MESH( MESH *mesh, OPTIONS options )
{
  /*+ Funciton: initialize struct MESH +*/
  mesh->nel = 0 ;
  mesh->nnd = 0 ;
  mesh->nap = 0 ;
  mesh->nmp = 0 ;
  mesh->nop = NULL ;
  mesh->flag = NULL ;
  mesh->crd = NULL ;
  mesh->ap_elem = 0 ;
  mesh->nd_elem = 0 ;
  strcpy( mesh->elem_type, "" ) ;
  mesh->dimension = 0 ;

  init_MTRL( &(mesh->mtrl) ) ;

  mesh->x_d = NULL ;
  mesh->x_c = NULL ;
  mesh->b_d_mem = NULL ;
  mesh->b_c_mem = NULL ;



  init_DirichletBC( &(mesh->dbc_Axn0) ) ;
  mesh->antenna = NULL ;


  mesh->nphi = 0 ;
  mesh->apid2phiid = NULL ;
  mesh->apid2gnid = NULL ;
  mesh->mpid2gnid = NULL ;

  mesh->nini = 0 ;
  mesh->ini = NULL ;
  mesh->MagField = NULL ;
  mesh->nbc_A = 0 ;
 

  init_ResultValue( &(mesh->rvalue), options ) ;
}

/********************************************************************/

void free_MESH( MESH *mesh, OPTIONS options )
{
  /*+ Funciton: free struct MESH +*/
  int    i ;

  int    nel = mesh->nel ;
  int    nnd = mesh->nnd ;
  int    nap = mesh->nap ;
  int    nmp = mesh->nmp ;
  int    nphi = mesh->nphi ;
  int    nf ;
  int    nd_elem = mesh->nd_elem ;
  int    dimension = mesh->dimension ;


  if( options.field == FIELD_ELECTROMAGNETIC ) {
    nf = nmp + nphi ;
  } else {
    nf = 0 ;
  }


  if( mesh->nop != NULL ) {
    Free( sizeof(int)*nel*nd_elem, mesh->nop ) ;
  }
  if( mesh->flag != NULL ) {
    Free( sizeof(int)*nel, mesh->flag ) ;
  }
  if( mesh->crd != NULL ) {
    Free( sizeof(double)*nnd*dimension, mesh->crd ) ;
  }
  if( mesh->x_d != NULL ) {
    Free( sizeof(double)*nf, mesh->x_d ) ;
  }
  if( mesh->x_c != NULL ) {
    Free( sizeof(Complex)*nf, mesh->x_c ) ;
  }
  if( mesh->b_d_mem != NULL ) {
    Free( sizeof(double)*nf, mesh->b_d_mem ) ;
  }
  if( mesh->b_c_mem != NULL ) {
    Free( sizeof(Complex)*nf, mesh->b_c_mem ) ;
  }


  free_DirichletBC( &(mesh->dbc_Axn0) ) ;

  if( mesh->antenna != NULL ) {
    for( i=0 ; i<mesh->mtrl.nantenna ; i++ ) {
      free_ANTENNA( &(mesh->antenna[i]), nap, dimension ) ;
    }
    Free( sizeof(ANTENNA)*mesh->mtrl.nantenna, mesh->antenna ) ;
  }

  if( mesh->apid2phiid != NULL ) {
    Free( sizeof(int)*nap, mesh->apid2phiid ) ;
  }
  if( mesh->apid2gnid != NULL ) {
    Free( sizeof(int)*nap, mesh->apid2gnid ) ;
  }
  if( mesh->mpid2gnid != NULL ) {
    Free( sizeof(int)*nf, mesh->mpid2gnid ) ;
  }

  if( mesh->ini != NULL ) {
    for( i=0 ; i<mesh->nini ; i++ ) {
      Free( sizeof(double)*nf, mesh->ini[i] ) ;
    }
    Free( sizeof(double*)*mesh->nini, mesh->ini ) ;
  }

  if( mesh->MagField != NULL ) {
    Free( sizeof(double)*nel*dimension, mesh->MagField ) ;
  }
  
  free_ResultValue( &(mesh->rvalue), nel, nnd, dimension, options ) ;

  free_MTRL( &(mesh->mtrl) ) ;

  mesh->nel = 0 ;
  mesh->nnd = 0 ;
  mesh->nap = 0 ;
  mesh->nmp = 0 ;
  mesh->ap_elem = 0 ;
  mesh->nd_elem = 0 ;
  strcpy( mesh->elem_type, "" ) ;
  mesh->dimension = 0 ;
  mesh->nphi = 0 ;
  mesh->nini = 0 ;
  mesh->nbc_A = 0 ;
}

/********************************************************************/

void init_Inbc( Inbc *inbc )
{
  /*+ Funciton: initialize struct Inbc +*/
  inbc->snid = 0 ;
  inbc->nfid = 0 ;
  inbc->coordinate = 0 ;
  inbc->charge_part = 0 ;
  inbc->ln_chpar = 0 ;
  inbc->ln_part = 0 ;
}

/********************************************************************/

void free_Inbc( Inbc *inbc )
{
  /*+ Funciton: free struct Inbc +*/
  inbc->snid = 0 ;
  inbc->nfid = 0 ;
  inbc->coordinate = 0 ;
  inbc->charge_part = 0 ;
  inbc->ln_chpar = 0 ;
  inbc->ln_part = 0 ;
}

/********************************************************************/

void Copy_Inbc( Inbc *a, Inbc b )
{
  /*+ Funciton: copy struct Inbc +*/
  a->snid = b.snid ;
  a->nfid = b.nfid ;
  a->coordinate = b.coordinate ;
  a->charge_part = b.charge_part ;
  a->ln_chpar = b.ln_chpar ;
  a->ln_part = b.ln_part ;
}

/********************************************************************/

void send_Inbc( Inbc *inbc, OPTIONS options, int compid, int type )
{
  /*+ Funciton: send struct Inbc +*/
  COM_WriteInt( &(inbc->snid), 1, compid, type ) ;
  COM_WriteInt( &(inbc->nfid), 1, compid, type ) ;
  COM_WriteInt( &(inbc->coordinate), 1, compid, type ) ;
  COM_WriteInt( &(inbc->charge_part), 1, compid, type ) ;
  COM_WriteInt( &(inbc->ln_chpar), 1, compid, type ) ;
  COM_WriteInt( &(inbc->ln_part), 1, compid, type ) ;
}

/********************************************************************/

void recv_Inbc( Inbc *inbc, OPTIONS options, int compid, int type )
{
  /*+ Funciton: recieve struct Inbc +*/
  COM_ReadInt( &(inbc->snid), 1, &compid, &type ) ;
  COM_ReadInt( &(inbc->nfid), 1, &compid, &type ) ;
  COM_ReadInt( &(inbc->coordinate), 1, &compid, &type ) ;
  COM_ReadInt( &(inbc->charge_part), 1, &compid, &type ) ;
  COM_ReadInt( &(inbc->ln_chpar), 1, &compid, &type ) ;
  COM_ReadInt( &(inbc->ln_part), 1, &compid, &type ) ;
}

/********************************************************************/

void init_DomMesh( DomMesh *dm, OPTIONS options )
{
  /*+ Funciton: initialize struct DomMesh +*/
  dm->partid = 0 ;
  dm->domid = 0 ;
  dm->npart = 0 ;
  dm->ndomain = 0 ;

  dm->nel = 0 ;
  dm->nnd = 0 ;
  dm->nap = 0 ;
  dm->nmp = 0 ;
  dm->nphi = 0 ;
  dm->nop = NULL ;
  dm->flag = NULL ;
  dm->crd = NULL ;
  dm->ap_elem = 0 ;
  dm->nd_elem = 0 ;
  strcpy( dm->elem_type, "" ) ;
  dm->dimension = 0 ;
  dm->ifd_dimension = 0 ;
  dm->ninbc = 0 ;
  dm->inbc = NULL ;

  dm->snid2pnid = NULL ;
  dm->seid2geid = NULL ;

  dm->apid2phiid = NULL ;
  dm->apid2snid = NULL ;
  dm->mpid2snid = NULL ;

  dm->x_d = NULL ;
  dm->x_c = NULL ;
  dm->b_d_mem = NULL ;
  dm->b_c_mem = NULL ;

  init_DirichletBC( &(dm->dbc_Axn0) ) ;
  dm->nbc_A = 0 ;
  dm->antenna = NULL ;

  init_ResultValue( &(dm->rvalue), options ) ;


}

/********************************************************************/

void free_DomMesh( DomMesh *dm, MTRL mtrl, OPTIONS options )
{
  /*+ Funciton: free struct DomMesh +*/
  int    i ;

  int    nel = dm->nel ;
  int    nnd = dm->nnd ;
  int    nap = dm->nap ;
  int    nmp = dm->nmp ;
  int    nphi = dm->nphi ;
  int    nf = 0 ;
  int    nd_elem = dm->nd_elem ;
  int    dimension = dm->dimension ;


  if( options.field == FIELD_ELECTROMAGNETIC ) {
    nf = nmp + nphi ;
  }


  if( dm->nop != NULL ) {
    Free( sizeof(int)*nel*nd_elem, dm->nop ) ;
  }
  if( dm->flag != NULL ) {
    Free( sizeof(int)*nel, dm->flag ) ;
  }
  if( dm->crd != NULL ) {
    Free( sizeof(double)*nap*dimension, dm->crd ) ;
  }
  if( dm->inbc != NULL ) {
    for( i=0 ; i<dm->ninbc ; i++ ) {
      free_Inbc( &(dm->inbc[i]) ) ;
    }
    Free( sizeof(Inbc)*dm->ninbc, dm->inbc ) ;
  }
  if( dm->snid2pnid != NULL ) {
    Free( sizeof(int)*nnd, dm->snid2pnid ) ;
  }
  if( dm->seid2geid != NULL ) {
    Free( sizeof(int)*nel, dm->seid2geid ) ;
  }
  if( dm->apid2phiid != NULL ) {
    Free( sizeof(int)*nap, dm->apid2phiid ) ;
  }
  if( dm->apid2snid != NULL ) {
    Free( sizeof(int)*nap, dm->apid2snid ) ;
  }
  if( dm->mpid2snid != NULL ) {
    Free( sizeof(int)*nf, dm->mpid2snid ) ;
  }


  if( dm->x_d != NULL ) {
    Free( sizeof(double)*nf, dm->x_d ) ;
  }
  if( dm->x_c != NULL ) {
    Free( sizeof(Complex)*nf, dm->x_c ) ;
  }
  if( dm->b_d_mem != NULL ) {
    Free( sizeof(double)*nf, dm->b_d_mem ) ;
  }
  if( dm->b_c_mem != NULL ) {
    Free( sizeof(Complex)*nf, dm->b_c_mem ) ;
  }

  free_DirichletBC( &(dm->dbc_Axn0) ) ;

  if( dm->antenna != NULL ) {
    for( i=0 ; i<mtrl.nantenna ; i++ ) {
      free_ANTENNA( &(dm->antenna[i]), nap, dimension ) ;
    }
    Free( sizeof(ANTENNA)*mtrl.nantenna, dm->antenna ) ;
  }

  free_ResultValue( &(dm->rvalue), nel, nnd, dimension, options ) ;


  dm->partid = 0 ;
  dm->domid = 0 ;
  dm->npart = 0 ;
  dm->ndomain = 0 ;

  dm->nel = 0 ;
  dm->nnd = 0 ;
  dm->nap = 0 ;
  dm->nmp = 0 ;
  dm->ap_elem = 0 ;
  dm->nd_elem = 0 ;
  strcpy( dm->elem_type, "" ) ;
  dm->dimension = 0 ;
  dm->ifd_dimension = 0 ;
  dm->ninbc = 0 ;
  dm->nbc_A = 0 ;

}

/********************************************************************/

void send_DomMesh( DomMesh *dm, MTRL mtrl, OPTIONS options,
                   int compid, int type )
{
  /*+ Funciton: send struct DomMesh +*/
  int    i ;

  int    nf = 0 ;


  if( options.field == FIELD_ELECTROMAGNETIC ) {
    nf = dm->nmp + dm->nphi ;
  }


  COM_WriteInt( &(dm->partid), 1, compid, type ) ;
  COM_WriteInt( &(dm->domid), 1, compid, type ) ;
  COM_WriteInt( &(dm->npart), 1, compid, type ) ;
  COM_WriteInt( &(dm->ndomain), 1, compid, type ) ;

  COM_WriteInt( &(dm->nel), 1, compid, type ) ;
  COM_WriteInt( &(dm->nnd), 1, compid, type ) ;
  COM_WriteInt( &(dm->nap), 1, compid, type ) ;
  COM_WriteInt( &(dm->nmp), 1, compid, type ) ;
  COM_WriteInt( &(dm->nphi), 1, compid, type ) ;
  COM_WriteInt( &(dm->ap_elem), 1, compid, type ) ;
  COM_WriteInt( &(dm->nd_elem), 1, compid, type ) ;
  COM_WriteChr( dm->elem_type, 128, compid, type ) ;
  COM_WriteInt( &(dm->dimension), 1, compid, type ) ;
  COM_WriteInt( &(dm->ifd_dimension), 1, compid, type ) ;

  COM_WriteInt( dm->nop, dm->nel*dm->nd_elem, compid, type ) ;
  COM_WriteInt( dm->flag, dm->nel, compid, type ) ;
  COM_WriteDbl( dm->crd, dm->nap*dm->dimension, compid, type ) ;

  COM_WriteInt( &(dm->ninbc), 1, compid, type ) ;
  for( i=0 ; i<dm->ninbc ; i++ ) {
    send_Inbc( &(dm->inbc[i]), options, compid, type ) ;
  }

  if( dm->snid2pnid != NULL ) {
    COM_WriteInt( &(dm->nnd), 1, compid, type ) ;
    COM_WriteInt( dm->snid2pnid, dm->nnd, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( dm->seid2geid != NULL ) {
    COM_WriteInt( &(dm->nel), 1, compid, type ) ;
    COM_WriteInt( dm->seid2geid, dm->nel, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }

  if( dm->apid2phiid != NULL ) {
    COM_WriteInt( &(dm->nap), 1, compid, type ) ;
    COM_WriteInt( dm->apid2phiid, dm->nap, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( dm->apid2snid != NULL ) {
    COM_WriteInt( &(dm->nap), 1, compid, type ) ;
    COM_WriteInt( dm->apid2snid, dm->nap, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( dm->mpid2snid != NULL ) {
    COM_WriteInt( &(nf), 1, compid, type ) ;
    COM_WriteInt( dm->mpid2snid, nf, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }

  if( dm->x_d != NULL ) {
    COM_WriteInt( &nf, 1, compid, type ) ;
    COM_WriteDbl( dm->x_d, nf, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( dm->x_c != NULL ) {
    COM_WriteInt( &nf, 1, compid, type ) ;
    COM_WriteCpl( dm->x_c, nf, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( dm->b_d_mem != NULL ) {
    COM_WriteInt( &nf, 1, compid, type ) ;
    COM_WriteDbl( dm->b_d_mem, nf, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }
  if( dm->b_c_mem != NULL ) {
    COM_WriteInt( &nf, 1, compid, type ) ;
    COM_WriteCpl( dm->b_c_mem, nf, compid, type ) ;
  } else {
    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;
  }

  send_DirichletBC( &(dm->dbc_Axn0), options, compid, type ) ;
  COM_WriteInt( &(dm->nbc_A), 1, compid, type ) ;
  for( i=0 ; i<mtrl.nantenna ; i++ ) {
    send_ANTENNA( &(dm->antenna[i]), options, dm->nap, dm->dimension,
               compid, type ) ;
  }

    int    ii = 0 ;
    COM_WriteInt( &ii, 1, compid, type ) ;

  send_ResultValue( &(dm->rvalue), options, dm->nel, dm->nap, dm->dimension,
                    compid, type ) ;
}

/********************************************************************/

void recv_DomMesh( DomMesh *dm, MTRL mtrl, OPTIONS options,
                   int compid, int type )
{
  /*+ Funciton: recieve struct DomMesh +*/
  int    i ;

  int    nf = 0 ;
  int    ii = 0 ;


  COM_ReadInt( &(dm->partid), 1, &compid, &type ) ;
  COM_ReadInt( &(dm->domid), 1, &compid, &type ) ;
  COM_ReadInt( &(dm->npart), 1, &compid, &type ) ;
  COM_ReadInt( &(dm->ndomain), 1, &compid, &type ) ;

  COM_ReadInt( &(dm->nel), 1, &compid, &type ) ;
  COM_ReadInt( &(dm->nnd), 1, &compid, &type ) ;
  COM_ReadInt( &(dm->nap), 1, &compid, &type ) ;
  COM_ReadInt( &(dm->nmp), 1, &compid, &type ) ;
  COM_ReadInt( &(dm->nphi), 1, &compid, &type ) ;
  COM_ReadInt( &(dm->ap_elem), 1, &compid, &type ) ;
  COM_ReadInt( &(dm->nd_elem), 1, &compid, &type ) ;
  COM_ReadChr( dm->elem_type, 128, &compid, &type ) ;
  COM_ReadInt( &(dm->dimension), 1, &compid, &type ) ;
  COM_ReadInt( &(dm->ifd_dimension), 1, &compid, &type ) ;

  dm->nop = (int *)Malloc( sizeof(int), dm->nel*dm->nd_elem ) ;
  COM_ReadInt( dm->nop, dm->nel*dm->nd_elem, &compid, &type ) ;
  dm->flag = (int *)Malloc( sizeof(int), dm->nel ) ;
  COM_ReadInt( dm->flag, dm->nel, &compid, &type ) ;
  dm->crd = (double *)Malloc( sizeof(double), dm->nap*dm->dimension ) ;
  COM_ReadDbl( dm->crd, dm->nap*dm->dimension, &compid, &type ) ;

  COM_ReadInt( &(dm->ninbc), 1, &compid, &type ) ;
  dm->inbc = (Inbc *)Malloc( sizeof(Inbc), dm->ninbc ) ;
  for( i=0 ; i<dm->ninbc ; i++ ) {
    init_Inbc( &(dm->inbc[i]) ) ;
    recv_Inbc( &(dm->inbc[i]), options, compid, type ) ;
  }

  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    dm->snid2pnid = (int *)Malloc( sizeof(int), ii ) ;
    COM_ReadInt( dm->snid2pnid, ii, &compid, &type ) ;
    ii = 0 ;
  }
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    dm->seid2geid = (int *)Malloc( sizeof(int), ii ) ;
    COM_ReadInt( dm->seid2geid, ii, &compid, &type ) ;
    ii = 0 ;
  }

  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    dm->apid2phiid = (int *)Malloc( sizeof(int), ii ) ;
    COM_ReadInt( dm->apid2phiid, ii, &compid, &type ) ;
    ii = 0 ;
  }
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    dm->apid2snid = (int *)Malloc( sizeof(int), ii ) ;
    COM_ReadInt( dm->apid2snid, ii, &compid, &type ) ;
    ii = 0 ;
  }
  COM_ReadInt( &ii, 1, &compid, &type ) ;
  if( ii > 0 ) {
    dm->mpid2snid = (int *)Malloc( sizeof(int), ii ) ;
    COM_ReadInt( dm->mpid2snid, ii, &compid, &type ) ;
    ii = 0 ;
  }

  COM_ReadInt( &nf, 1, &compid, &type ) ;
  if( nf > 0 ) {
    dm->x_d = (double *)Malloc( sizeof(double), nf ) ;
    COM_ReadDbl( dm->x_d, nf, &compid, &type ) ;
    nf = 0 ;
  }
  COM_ReadInt( &nf, 1, &compid, &type ) ;
  if( nf > 0 ) {
    dm->x_c = (Complex *)Malloc( sizeof(Complex), nf ) ;
    COM_ReadCpl( dm->x_c, nf, &compid, &type ) ;
    nf = 0 ;
  }
  COM_ReadInt( &nf, 1, &compid, &type ) ;
  if( nf > 0 ) {
    dm->b_d_mem = (double *)Malloc( sizeof(double), nf ) ;
    COM_ReadDbl( dm->b_d_mem, nf, &compid, &type ) ;
    nf = 0 ;
  }
  COM_ReadInt( &nf, 1, &compid, &type ) ;
  if( nf > 0 ) {
    dm->b_c_mem = (Complex *)Malloc( sizeof(Complex), nf ) ;
    COM_ReadCpl( dm->b_c_mem, nf, &compid, &type ) ;
    nf = 0 ;
  }

  recv_DirichletBC( &(dm->dbc_Axn0), options, compid, type ) ;
  COM_ReadInt( &(dm->nbc_A), 1, &compid, &type ) ;
  dm->antenna = (ANTENNA *)Malloc( sizeof(ANTENNA), mtrl.nantenna ) ;
  for( i=0 ; i<mtrl.nantenna ; i++ ) {
    init_ANTENNA( &(dm->antenna[i]) ) ;
    recv_ANTENNA( &(dm->antenna[i]), options, dm->nap, dm->dimension,
               compid, type ) ;
  }

  COM_ReadInt( &nf, 1, &compid, &type ) ;

  recv_ResultValue( &(dm->rvalue), options, dm->nel, dm->nap, dm->dimension,
                    compid, type ) ;
}

/********************************************************************/

void init_OPinfo( OPinfo *op )
{
  /*+ Funciton: initialize struct OPinfo +*/
  op->n_mofree = 0 ;
  op->ibid = NULL ;
}

/********************************************************************/

void free_OPinfo( OPinfo *op )
{
  /*+ Funciton: free struct OPinfo +*/
  if( op->ibid != NULL ) {
    Free( sizeof(int)*(op->n_mofree), op->ibid ) ;
  }
  op->n_mofree = 0 ;
}

/********************************************************************/

void Copy_OPinfo( OPinfo *a, OPinfo b )
{
  /*+ Funciton: copy struct OPinfo +*/
  init_OPinfo( a ) ;
  a->n_mofree = b.n_mofree ;
  if( b.ibid != NULL ) {
    a->ibid = (int *)Malloc( sizeof(int), b.n_mofree ) ;
    memcpy( a->ibid, b.ibid, sizeof(int)*b.n_mofree ) ;
  }
}

/********************************************************************/

void init_Pinbc( Pinbc *pinbc )
{
  /*+ Funciton: initialize struct Pinbc +*/
  pinbc->nf = 0 ;
  pinbc->yng_npart = 0 ;
  pinbc->yng_part = NULL ;
  pinbc->yng_nf_part = NULL;
  pinbc->old_npart = 0 ;
  pinbc->old_part = NULL ;
  pinbc->old_nf_part = NULL;
  pinbc->charge_part = 0 ;
  pinbc->ln_chpar = 0 ;
  pinbc->ln_part = 0 ;
  pinbc->bc_flag = 0 ;
}

/********************************************************************/

void free_Pinbc( Pinbc *pinbc )
{
  /*+ Funciton: free struct Pinbc +*/
  if( pinbc->yng_part != NULL ) {
    Free( sizeof(int)*pinbc->yng_npart, pinbc->yng_part ) ;
  }
  if( pinbc->yng_nf_part != NULL ) {
    Free( sizeof(int)*pinbc->yng_npart, pinbc->yng_nf_part ) ;
  }
  if( pinbc->old_part != NULL ) {
    Free( sizeof(int)*pinbc->old_npart, pinbc->old_part ) ;
  }
  if( pinbc->old_nf_part != NULL ) {
    Free( sizeof(int)*pinbc->old_npart, pinbc->old_nf_part ) ;
  }
  pinbc->nf = 0 ;
  pinbc->yng_npart = 0 ;
  pinbc->old_npart = 0 ;
  pinbc->charge_part = 0 ;
  pinbc->ln_chpar = 0 ;
  pinbc->ln_part = 0 ;
  pinbc->bc_flag = 0 ;
}

/********************************************************************/

void init_HDDM_VEC( HDDM_VEC *hddm_vec )
{
  /*+ Funciton: initialize struct HDDM_VEC +*/
  hddm_vec->infree = 0 ;
  hddm_vec->outfree = 0 ;
  hddm_vec->tfree = 0 ;
  hddm_vec->total_free = 0 ;

  hddm_vec->p_d = NULL ;
  hddm_vec->p_c = NULL ;

  hddm_vec->q_d = NULL ;
  hddm_vec->q_c = NULL ;

  hddm_vec->r_d = NULL ;
  hddm_vec->r_c = NULL ;

  hddm_vec->z_d = NULL ;
  hddm_vec->z_c = NULL ;

  hddm_vec->u_d = NULL ;
  hddm_vec->u_c = NULL ;

  hddm_vec->pc_size = 0 ;
  hddm_vec->pc_d = NULL ;
  hddm_vec->pc_c = NULL ;
}

/********************************************************************/

void free_HDDM_VEC( HDDM_VEC *hddm_vec )
{
  /*+ Funciton: free struct HDDM_VEC +*/
  int    infree = hddm_vec->infree ;
  int    tfree = hddm_vec->tfree ;
  int    pc_size = hddm_vec->pc_size ;


  if( hddm_vec->p_d != NULL ) {
    Free( sizeof(double)*tfree, hddm_vec->p_d ) ;
  }
  if( hddm_vec->p_c != NULL ) {
    Free( sizeof(Complex)*tfree, hddm_vec->p_c ) ;
  }

  if( hddm_vec->q_d != NULL ) {
    Free( sizeof(double)*tfree, hddm_vec->q_d ) ;
  }
  if( hddm_vec->q_c != NULL ) {
    Free( sizeof(Complex)*tfree, hddm_vec->q_c ) ;
  }

  if( hddm_vec->r_d != NULL ) {
    Free( sizeof(double)*infree, hddm_vec->r_d ) ;
  }
  if( hddm_vec->r_c != NULL ) {
    Free( sizeof(Complex)*infree, hddm_vec->r_c ) ;
  }

  if( hddm_vec->z_d != NULL ) {
    Free( sizeof(double)*infree, hddm_vec->z_d ) ;
  }
  if( hddm_vec->z_c != NULL ) {
    Free( sizeof(Complex)*infree, hddm_vec->z_c ) ;
  }

  if( hddm_vec->u_d != NULL ) {
    Free( sizeof(double)*infree, hddm_vec->u_d ) ;
  }
  if( hddm_vec->u_c != NULL ) {
    Free( sizeof(Complex)*infree, hddm_vec->u_c ) ;
  }

  if( hddm_vec->pc_d != NULL ) {
    Free( sizeof(double)*pc_size, hddm_vec->pc_d ) ;
  }
  if( hddm_vec->pc_c != NULL ) {
    Free( sizeof(Complex)*pc_size, hddm_vec->pc_c ) ;
  }
  hddm_vec->pc_size = 0 ;

  hddm_vec->infree = 0 ;
  hddm_vec->outfree = 0 ;
  hddm_vec->tfree = 0 ;
  hddm_vec->total_free = 0 ;
}

/********************************************************************/

void init_PartMesh( PartMesh *pm, OPTIONS options )
{
  /*+ Funciton: initialize struct PartMesh +*/
  pm->partid = 0 ;
  pm->npart = 0 ;
  pm->ndomain = 0 ;

  pm->nel = 0 ;
  pm->nel_all =0 ;
  pm->nnd = 0 ;
  pm->nnd_all = 0 ;
  pm->nap = 0 ;
  pm->nmp = 0 ;
  pm->crd = NULL ;
  pm->ap_elem = 0 ;
  pm->nd_elem = 0 ;
  strcpy( pm->elem_type, "" ) ;
  pm->dimension = 0 ;
  pm->pnid2gnid = NULL ;
  pm->infree = 0 ;
  pm->outfree = 0 ;
  pm->midfree =0 ;
  pm->tfree =0 ;
  pm->total_free =0 ;
  pm->op = NULL ;
  pm->ifd_dimension = 0 ;
  pm->pinbc = NULL ;

  init_MTRL( &(pm->mtrl) ) ;
  pm->pantenna = NULL ;

  init_DirichletBC( &(pm->dbc_Axn0) ) ;

  pm->hddm_vec_u_d_mem = NULL ;
  pm->hddm_vec_u_c_mem = NULL ;

  pm->dm = NULL ;


}

/********************************************************************/

void free_PartMesh( PartMesh *pm, OPTIONS options )
{
  /*+ Funciton: free struct PartMesh +*/
  int    i ;

  int    npart = pm->npart ;
  int    ndomain = pm->ndomain ;
  int    nnd = pm->nnd ;
  int    dimension = pm->dimension ;



  if( pm->crd != NULL ) {
    Free( sizeof(double)*nnd*dimension, pm->crd ) ;
  }
  if( pm->pnid2gnid != NULL ) {
    Free( sizeof(int)*nnd, pm->pnid2gnid ) ;
  }
  if( pm->op != NULL ) {
    for( i=0 ; i<npart ; i++ ) {
      free_OPinfo( &(pm->op[i]) ) ;
    }
    Free( sizeof(OPinfo)*npart, pm->op ) ;
  }
  if( pm->pinbc != NULL ) {
    for( i=0 ; i<pm->tfree ; i++ ) {
      free_Pinbc( &(pm->pinbc[i]) ) ;
    }
    Free( sizeof(Pinbc)*pm->tfree, pm->pinbc ) ;
  }
  if( pm->hddm_vec_u_d_mem != NULL ) {
    Free( sizeof(double)*pm->infree, pm->hddm_vec_u_d_mem ) ;
  }
  if( pm->hddm_vec_u_c_mem != NULL ) {
    Free( sizeof(Complex)*pm->infree, pm->hddm_vec_u_c_mem ) ;
  }
  if( pm->dm != NULL ) {
    for( i=0 ; i<ndomain ; i++ ) {
      free_DomMesh( &(pm->dm[i]), pm->mtrl, options ) ;
    }
    Free( sizeof(DomMesh)*ndomain, pm->dm ) ;
  }
 


  if( pm->pantenna != NULL ) {
    for( i=0 ; i<pm->mtrl.nantenna ; i++ ) {
      free_PartANTENNA( &(pm->pantenna[i]), dimension ) ;
    }
    Free( sizeof(PartANTENNA)*pm->mtrl.nantenna, pm->pantenna ) ;
  }

  free_DirichletBC( &(pm->dbc_Axn0) ) ;

 

  free_MTRL( &(pm->mtrl) ) ;

  pm->partid = 0 ;
  pm->npart = 0 ;
  pm->ndomain = 0 ;

  pm->nel = 0 ;
  pm->nel_all =0 ;
  pm->nnd = 0 ;
  pm->nnd_all = 0 ;
  pm->nap = 0 ;
  pm->nmp = 0 ;
  pm->ap_elem = 0 ;
  pm->nd_elem = 0 ;
  strcpy( pm->elem_type, "" ) ;
  pm->dimension = 0 ;
  pm->infree = 0 ;
  pm->outfree = 0 ;
  pm->midfree =0 ;
  pm->tfree =0 ;
  pm->total_free =0 ;
  pm->ifd_dimension = 0 ;

}

/********************************************************************/

void init_HDDM_DOM_VEC( HDDM_DOM_VEC *hddm_dom_vec )
{
  /*+ Funciton: initialize struct HDDM_DOM_VEC +*/
  hddm_dom_vec->nf = 0 ;

  hddm_dom_vec->vec_d = NULL ;
  hddm_dom_vec->vec_c = NULL ;


  hddm_dom_vec->pc_size = 0 ;
  hddm_dom_vec->pc_d = NULL ;
  hddm_dom_vec->pc_c = NULL ;
}

/********************************************************************/

void free_HDDM_DOM_VEC( HDDM_DOM_VEC *hddm_dom_vec )
{
  /*+ Funciton: free struct HDDM_DOM_VEC +*/
  int    nf = hddm_dom_vec->nf ;
  int    pc_size = hddm_dom_vec->pc_size ;


  if( hddm_dom_vec->vec_d != NULL ) {
    Free( sizeof(double)*nf, hddm_dom_vec->vec_d ) ;
  }
  if( hddm_dom_vec->vec_c != NULL ) {
    Free( sizeof(Complex)*nf, hddm_dom_vec->vec_c ) ;
  }


  if( hddm_dom_vec->pc_d != NULL ) {
    Free( sizeof(double)*pc_size, hddm_dom_vec->pc_d ) ;
  }
  if( hddm_dom_vec->pc_c != NULL ) {
    Free( sizeof(Complex)*pc_size, hddm_dom_vec->pc_c ) ;
  }

  hddm_dom_vec->nf = 0 ;
  hddm_dom_vec->pc_size = 0 ;
}
