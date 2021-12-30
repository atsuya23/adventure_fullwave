/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __FUNC_MAIN_H__
#define __FUNC_MAIN_H__


#include "advfullwave_lib.h"


#define main_Init( x, y, z ) __MainInit__( (x), (y), (z), LOCAL_MODULE_NAME, LOCAL_VERSION, LOCAL_WORK, LOCAL_FIELD, LOCAL_ANALYSIS, LOCAL_N_ARGUMENT, LOCAL_USAGE, __DATE__, __TIME__ )

#define main_PrintCondition( a, b, c, d, e, f ) __MainPrintCondition__( (a), (b), (c), (d), (e), (f), LOCAL_MODULE_NAME )


/*** Initialize ***/
void __MainInit__( int argc, char **argv, OPTIONS *options,
                   char *mname, char *version,
                   int work, int field,  int analysis,
                   int n_argument, char *usage,
                   char *now_time, char *today ) ;



/*** Finalize ***/
void main_Finalize( void ) ;



/*** print conditions ***/
void __MainPrintCondition__( OPTIONS options,
                             int nel, int nap, int nmp,
                             char *elem_type, int nf,
                             char *mname ) ;

#endif /* __FUNC_MAIN_H__ */
