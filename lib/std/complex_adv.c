/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "complex_adv.h"

Complex ComplexNew( double r, double i )
{

  Complex ret ;


  ret.re = r ;
  ret.im = i ;


  return ret ;
}

/********************************************************************/

Complex ComplexConjugate( Complex a )
{
  /*+ Function: conjugate of complex number +*/
  Complex ret ;


  ret.re = a.re ;
  ret.im = -a.im ;


  return ret ;
}

/********************************************************************/

Complex ComplexReciprocal( Complex a )
{
  /*+ Function: reciprocal of complex number +*/
  Complex ret ;
  double  d ;


  d = 1.0 / (a.re*a.re + a.im*a.im) ;
  ret.re = a.re * d ;
  ret.im = -(a.im * d) ;


  return ret ;
}

/********************************************************************/

double ComplexAbs( Complex a )
{
  /*+ Function: absolute value of complex number +*/
  return  sqrt( a.re*a.re+a.im*a.im ) ;
}

/********************************************************************/

double ComplexAbs2( Complex a )
{
  /*+ Function: raise absolute value of complex number
                                     to the second power+*/
  return a.re*a.re+a.im*a.im ;
}

/********************************************************************/

Complex ComplexPlus( Complex a, Complex b )
{
  /*+ Function: complex calculation ( a + b ) +*/
  Complex ret ;


  ret.re = a.re + b.re ;
  ret.im = a.im + b.im ;


  return ret ;
}

/********************************************************************/

Complex ComplexMinus( Complex a, Complex b )
{
  /*+ Function: complex calculation ( a - b ) +*/
  Complex ret ;


  ret.re = a.re - b.re ;
  ret.im = a.im - b.im ;


  return ret ;
}

/********************************************************************/

Complex ComplexMulti( Complex a, Complex b )
{
  /*+ Function: complex calculation ( a * b ) +*/
  Complex ret ;


  ret.re = a.re*b.re - a.im*b.im ;
  ret.im = a.im*b.re + a.re*b.im ;


  return ret ;
}

/********************************************************************/

Complex ComplexMultiScalar( Complex a, double b )
{
  /*+ Function: (complex value) * ( real value ) +*/
  Complex ret ;


  ret.re = a.re*b ;
  ret.im = a.im*b ;


  return ret ;
}

/********************************************************************/

Complex ComplexDivide( Complex a, Complex b )
{
  /*+ Function: complex calculation ( a / b ) +*/
  return ComplexMulti(a, ComplexReciprocal(b)) ;
}

/********************************************************************/

Complex ComplexSqrt( Complex a )
{
  /*+ Function: square root of complex value +*/
  double re, im ;


  re = sqrt((a.re + sqrt(a.re*a.re + a.im*a.im)) / 2.0) ;
  if( re >= 1.0e-40 ) {
    im = a.im / (2.0 * re) ;
  } else {
    im = sqrt( fabs(a.re) ) ;
  }


  return ComplexNew(re, im) ;
}

/********************************************************************/

Complex ComplexInner( int n, Complex *a, Complex *b )
{
  /*+ Function: inner product of complex vectors ( aHb ) +*/
  int    i ;

  double re, im ;
  double d0, d1, d2, d3 ;


  d0 = d1 = d2 = d3 = 0.0 ;
  for( i=0 ; i<n ; i++ ) {
    d0 += a[i].re*b[i].re ;
    d1 += a[i].im*b[i].im ;
    d2 += a[i].re*b[i].im ;
    d3 += a[i].im*b[i].re ;
  }
  re = d0 + d1 ;
  im = d2 - d3 ;


  return ComplexNew(re, im) ;
}

/********************************************************************/

Complex ComplexBilinear( int n, Complex *a, Complex *b )
{
  /*+ Function: bilinear form of complex vectors ( aTb ) +*/
  int    i ;

  double re, im ;
  double d0, d1, d2, d3 ;


  d0 = d1 = d2 = d3 = 0.0 ;
  for( i=0 ; i<n ; i++ ) {
    d0 += a[i].re*b[i].re ;
    d1 += a[i].im*b[i].im ;
    d2 += a[i].re*b[i].im ;
    d3 += a[i].im*b[i].re ;
  }
  re = d0 - d1 ;
  im = d2 + d3 ;


  return ComplexNew(re, im) ;
}

