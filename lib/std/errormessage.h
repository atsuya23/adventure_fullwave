/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef  __ERRORMESSAGE_H__
#define  __ERRORMESSAGE_H__

#include "advfullwave_lib.h"


#define emessage( x, y, z ) __ErrorMessage__( (x), (y), (z), __FILE__, __LINE__ )


int __ErrorMessage__( int ErrorNumber, int AbortNumber,
                      char *str, char *fname, int line ) ;

#endif /* __ERRORMESSAGE_H__ */
