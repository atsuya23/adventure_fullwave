/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef  __IOFUNC_H__
#define  __IOFUNC_H__

#include "advfullwave_lib.h"


/*** fprintf ***/
int Fprintf( FILE *fp, const char *format, ... ) ;
int Efprintf( FILE *fp, const char *format, ... ) ;
int Pfprintf( FILE *fp, const char *format, ... ) ;
int EPfprintf( FILE *fp, const char *format, ... ) ;
int Rfprintf( FILE *fp, const char *format, ... )  ;



/*** file open/close ***/
FILE *Fopen( char *fname, char *mode ) ;
void Fclose( FILE *fp ) ;
AdvDatabox *AdvDboxOpen( char *fname ) ;
void AdvDboxClose( AdvDatabox *dbox ) ;
AdvDocFile *AdvFopen( char *fname, char *mode ) ;
void AdvFclose( AdvDocFile *fp ) ;

#endif /* __IOFUNC_H__ */
