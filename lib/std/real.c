/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "real.h"


double RealInner( int n, double *a, double *b )
{
  /*+ Function: inner product of real vectors ( aHb ) +*/
  int    i ;

  double d = 0.0 ;


  for( i=0 ; i<n ; i++ ) {
    d += a[i] * b[i] ;
  }


  return d ;
}
