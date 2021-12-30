/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "timefunc.h"


#ifdef __ADVFW_OS_UNIX__ /* Unix or Linux */
static tval start_time ;
#else
static time_t start_time ;
#endif


void setTimer( void )
{
  /*+ Function: Initialize timer +*/
#ifdef __ADVFW_OS_UNIX__ /* Unix or Linux */
  gettimeofday( &start_time, NULL ) ;
#else
  time( &start_time ) ;
#endif
}

/************************************************************************/

double nowTime( void )
{
  /*+ Function: return passed time after program starts +*/
#ifdef __ADVFW_OS_UNIX__ /* Unix or Linux */
  tval   now_time ;
  double use_time ;


  gettimeofday( &now_time, NULL ) ;
  use_time = ( ((double)now_time.tv_sec + (double)now_time.tv_usec*1.0e-6)
             - ((double)start_time.tv_sec + (double)start_time.tv_usec*1.0e-6) ) ;


  return( use_time ) ;
#else
  time_t now_time ;
  double use_time ;


  time( &now_time ) ;
  use_time = (double)(now_time - start_time) ;


  return( use_time ) ;
#endif
}

/************************************************************************/

#ifdef __ADVFW_OS_UNIX__ /* Unix or Linux */
double passTime( tval old_time )
{
  /*+ Function: return passed time old to now +*/
  tval   now_time ;
  double use_time ;


  gettimeofday( &now_time, NULL ) ;
  use_time = ( ((double)now_time.tv_sec + (double)now_time.tv_usec*1.0e-6)
             - ((double)old_time.tv_sec + (double)old_time.tv_usec*1.0e-6) ) ;


  return( use_time ) ;
}
#else
double passTime( time_t old_time )
{
  /*+ Function: return passed time old to now +*/
  time_t now_time ;
  double use_time ;


  time( &now_time ) ;
  use_time = (double)(now_time - old_time) ;

  return( use_time ) ;
}
#endif

/************************************************************************/

char *cTime( void )
{
  /*+ Function: return date +*/
  time_t now_time ;


  time( &now_time ) ;
  return (char *)(ctime( &now_time )) ;
}

/************************************************************************/

double plusTime( double *n_time )
{
  /*+ Function: return passed time old to now +*/
  double s_time ;


  s_time = (*n_time) ;
  (*n_time) = nowTime() ;
  return ((*n_time) - s_time) ;
}
