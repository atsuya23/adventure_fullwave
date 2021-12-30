/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "iofunc.h"


extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;


int Fprintf( FILE *fp, const char *format, ... )
{
  /*+ Function: standard fprintf +*/
  static char buf[1024] ;
  va_list ap ;
  int ret = 0 ;


  if( COM_GID.rootid == COM_ID.myprocid ) {
    sprintf( buf, "%s ", PRE_MESSAGE ) ;
    strncat( buf, format, 1023 ) ;

    va_start( ap, format ) ;

    ret = vfprintf( fp, buf, ap ) ;
    va_end( ap ) ;

    fflush( fp ) ;
  }


  return( ret ) ;
}

/***********************************************************************/

int Efprintf( FILE *fp, const char *format, ... )
{
  /*+ Function: error fprintf +*/
  static char buf[1024] ;
  va_list ap ;
  int ret = 0 ;


  if( COM_GID.rootid == COM_ID.myprocid ) {
    sprintf( buf, "error: " ) ;
    strncat( buf, format, 1023 ) ;

    va_start( ap, format ) ;

    ret = vfprintf( fp, buf, ap ) ;
    va_end( ap ) ;

    fflush( fp ) ;
  }


  return( ret ) ;
}

/***********************************************************************/

int Pfprintf( FILE *fp, const char *format, ... )
{
  /*+  Function:  fprintf with [ProcID] +*/
  static char buf[1024] ;
  va_list ap ;
  int ret ;


  if( COM_ID.nproc == 1 ) {
    sprintf( buf, "%s ", PRE_MESSAGE ) ;
    strncat( buf, format, 1023 ) ;

    va_start( ap, format ) ;

    ret = vfprintf( fp, buf, ap ) ;
    va_end( ap ) ;

    fflush( fp ) ;


  } else {
    sprintf( buf, "%s [%d] ", PRE_MESSAGE, COM_ID.myprocid ) ;
    strncat( buf, format, 1023 ) ;

    va_start( ap, format ) ;

    ret = vfprintf( fp, buf, ap ) ;
    va_end( ap ) ;

    fflush( fp ) ;
  }


  return( ret ) ;
}

/***********************************************************************/

int EPfprintf( FILE *fp, const char *format, ... )
{
  /*+  Function:  fprintf with [ProcID] +*/
  static char buf[1024] ;
  va_list ap ;
  int ret ;


  if( COM_ID.nproc == 1 ) {
    sprintf( buf, "error: " ) ;
    strncat( buf, format, 1023 ) ;

    va_start( ap, format ) ;

    ret = vfprintf( fp, buf, ap ) ;
    va_end( ap ) ;

    fflush( fp ) ;


  } else {
    sprintf( buf, "error[%d]:  ", COM_ID.myprocid ) ;
    strncat( buf, format, 1023 ) ;

    va_start( ap, format ) ;

    ret = vfprintf(fp, buf, ap) ;
    va_end( ap ) ;

    fflush( fp ) ;
  }


  return( ret ) ;
}

/***********************************************************************/

int Rfprintf( FILE *fp, const char *format, ... ) 
{
  /*+  Function:  fprintf only if I am grand parent +*/
  va_list ap ;


  if( COM_GID.rootid == COM_ID.myprocid ){
    va_start( ap, format ) ;
    vfprintf( fp, format, ap ) ;
    va_end( ap ) ;
    fflush( fp ) ;
  }


  return( 0 ) ;
}

/***********************************************************************/

FILE *Fopen( char *fname, char *mode )
{
  /*+ Functon: file open +*/
  FILE *fp ;


  if( (fp = fopen(fname,mode)) == NULL ) {
    emessage( 10, __ABORT_ADVFW_LIB__, fname ) ;
  }


  return fp ;
}

/***********************************************************************/

void Fclose( FILE *fp )
{
  /*+ Functon: file close +*/
  fclose( fp ) ;
}

/***********************************************************************/

AdvDatabox *AdvDboxOpen( char *fname )
{
  /*+ Functon: ADVENTURE data box open +*/
  AdvDatabox *dbox ;


  dbox = adv_dbox_new() ;
  if( !adv_dbox_add(dbox, fname) ){
    emessage( 10, __ABORT_ADVFW_LIB__, fname ) ;
  }


  return dbox ;
}

/***********************************************************************/

void AdvDboxClose( AdvDatabox *dbox )
{
  /*+ Functon: ADVENTURE data box close +*/
  adv_dbox_close( dbox ) ;
}

/***********************************************************************/

AdvDocFile *AdvFopen( char *fname, char *mode )
{
  /*+ Functon: ADVENTURE file open +*/
  AdvDocFile *fp ;


  if( (fp = adv_dio_file_open(fname,mode)) == NULL ) {
    emessage( 10, __ABORT_ADVFW_LIB__, fname ) ;
  }


  return fp ;
}

/***********************************************************************/

void AdvFclose( AdvDocFile *fp )
{
  /*+ Functon: ADVENTURE file close +*/
  adv_dio_file_close( fp ) ;
}
