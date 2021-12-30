/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __COMPLEX_H__
#define __COMPLEX_H__


#include "advfullwave_lib.h"

static const Complex complex_1 = {1.0, 0.0} ;
static const Complex complex_i = {0.0, 1.0} ;
static const Complex complex_0 = {0.0, 0.0} ;


Complex ComplexNew( double r, double i ) ;
Complex ComplexConjugate( Complex a ) ;
Complex ComplexReciprocal( Complex a ) ;
double ComplexAbs( Complex a ) ;
double ComplexAbs2( Complex a ) ;
Complex ComplexPlus( Complex a, Complex b ) ;
Complex ComplexMinus( Complex a, Complex b ) ;
Complex ComplexMulti( Complex a, Complex b ) ;
Complex ComplexMultiScalar( Complex a, double b ) ;
Complex ComplexDivide( Complex a, Complex b ) ;
Complex ComplexSqrt( Complex a ) ;
Complex ComplexInner( int n, Complex *a, Complex *b ) ;
Complex ComplexBilinear( int n, Complex *a, Complex *b ) ;

#endif /* __COMPLEX_H__ */
