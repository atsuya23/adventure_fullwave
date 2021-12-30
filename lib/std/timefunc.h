/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __TIMEFUNC_H__
#define __TIMEFUNC_H__

#include "advfullwave_lib.h"


#ifdef __ADVFW_OS_UNIX__
typedef struct timeval tval ;
#endif /* __ADVFW_OS_UNIX__ */


void setTimer( void ) ;
double nowTime( void ) ;
#ifdef __ADVFW_OS_UNIX__
double passTime( tval old_time ) ;
#else
double passTime( time_t old_time ) ;
#endif /* __ADVFW_OS_UNIX__ */
char *cTime( void ) ;
double plusTime( double *n_time ) ;

#endif /* __TIMEFUNC_H__ */
