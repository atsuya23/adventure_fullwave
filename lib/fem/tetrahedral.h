/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __TETRAHEDRAL_H__
#define __TETRAHEDRAL_H__


#include "libfem.h"


static const int tet_m[][4] = { { 0, 1, 2, 3 } ,
                                { 1, 3, 2, 0 } ,
                                { 2, 3, 0, 1 } ,
                                { 3, 1, 0, 2 }  } ;

static const int tet_n[][4] = { { 0 , 1 , 2 , 3 } ,
                                { 0 , 2 , 3 , 1 } ,
                                { 0 , 3 , 1 , 2 } ,
                                { 1 , 2 , 0 , 3 } ,
                                { 1 , 3 , 2 , 0 } ,
                                { 2 , 3 , 0 , 1 } } ;


/*** conventional piecewise linear tetrahedral element */
void tet_pickup_coordinate_4vertex( int *ne, double *crd,
                                    double *x, double *y, double *z ) ;
void tet_SideLength( double *x, double *y, double *z, double *l ) ;
double tet_Volume6( double *x, double *y, double *z ) ;
double tet_Volume( double *x, double *y, double *z ) ;
void tet_Center( double *x, double *y, double *z, double *g ) ;
void tet_simple_a( double *x, double *y, double *z, double *a ) ;
void tet_simple_b( double *x, double *y, double *z, double *b ) ;
void tet_simple_c( double *x, double *y, double *z, double *c ) ;
void tet_simple_d( double *x, double *y, double *z, double *d ) ;
void tet_simple_gradgrad( double D, double *b, double *c, double *d,
                          double gg[][4] ) ;
void tet_simple_be_apgrad( double D, double *b, double *c, double *d,
                           double u[][3], double *ag ) ;


/*** Nedelec elements of simplex type ***/
void tetNedelec_Direction( double *x, double *y, double *z, int *si ) ;
void tetNedelec_mk( double *x, double *y, double *z,
                    double *xi, double *yi, double *zi ) ;
void tetNedelec_XYZ( double *l, int *si,
                     double *xi, double *yi, double *zi,
                     double *X, double *Y, double *Z ) ;
void tetNedelec_km_mk( double *x, double *y, double *z,
                       double *xy, double *yz, double *zx ) ;
void tetNedelec_Fxyz( double *l, int *si, double *g,
                      double *xi, double *yi, double *zi,
                      double *xy, double *yz, double *zx,
                      double *Fx, double *Fy, double *Fz ) ;
void tetNedelec_rotrot( double D, double *X, double *Y, double *Z,
                        double rr[][6] ) ;
void tetNedelec_sideside( double D, double *l, double *tetg, int *si,
                          double *x, double *y, double *z,
                          double *xi, double *yi, double *zi,
                          double *xy, double *yz, double *zx,
                          double ss[][6] ) ;
void tetNedelec_be_sideside( double *u,
                             double D, double *l, double *tetg, int *si,
                             double *x, double *y, double *z,
                             double *xi, double *yi, double *zi,
                             double *xy, double *yz, double *zx,
                             double *be_ss ) ;
void tetNedelec_be_elrot( double *u, double *X, double *Y, double *Z,
                          double *er ) ;


void tetNedelec_gradside( double D,
                          double *b, double *c, double *d,
                          double *Fx, double *Fy, double *Fz,
                          double gs[][4] ) ;
void tetNedelec_be_gradside( double *u, double D,
                             double *b, double *c, double *d,
                             double *Fx, double *Fy, double *Fz,
                             double *be_gs ) ;
void tetNedelec_sidegrad( double D,
                          double *b, double *c, double *d,
                          double *Fx, double *Fy, double *Fz,
                          double sg[][6] ) ;
void tetNedelec_be_sidegrad( double *u, double D,
                             double *b, double *c, double *d,
                             double *Fx, double *Fy, double *Fz,
                             double *be_sg ) ;


void tetNedelec_JoA( double Joe[][3], int *si, double *l,
                     double *b, double *c, double *d,
                     double *be_JoA ) ;
void tetNedelec_JoA_Complex( Complex Joe[][3], int *si, double *l,
                             double *b, double *c, double *d,
                             Complex *be_JoA ) ;


void tri_Center( double *x, double *y, double *z, double *g );

void triNedelec_sideside( double S, int ndID2d[3], double D, double *l, 
			  double *tetg, int *si,
                          double *x, double *y, double *z,
                          double *xi, double *yi, double *zi,
                          double *xy, double *yz, double *zx,
                          double ss[][6] );
double tri_square( double *x, double *y, double *z, double *nomal );

void tet_cross( double *x, double *y, double *z, double *nomal,
		double *xc, double *yc, double *zc);

void tetNedelec_pointpoint( double D, double *a, double *b, double *c, double   
			    *d, double *x, double *y, double *z, double pp[][4]);

#endif /* __TETRAHEDRAL_H__ */
