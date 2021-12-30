/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "tetrahedral.h"


void tet_pickup_coordinate_4vertex( int *ne, double *crd,
                                    double *x, double *y, double *z )
{
  /*+ Function: pickup coordinates of 4 vertexes of tetrahedral +*/
  int    i ;


  for( i=0 ; i<4 ; i++ ) {
    int    ii = ne[i] * 3 ;
    x[i] = crd[ii] ;
    y[i] = crd[++ii] ;
    z[i] = crd[++ii] ;
  }
}

/********************************************************************/

void tet_SideLength( double *x, double *y, double *z, double *l )
{
  /*+ Function: calculate lengths of 6 sides of tetrahedral +*/
  int    i ;


  for( i=0 ; i<6 ; i++ ){
    int   ii = tet_n[i][0] ;
    int   jj = tet_n[i][1] ;
    l[i] = sqrt( pow( x[ii] - x[jj], 2)
                +pow( y[ii] - y[jj], 2)
                +pow( z[ii] - z[jj], 2) ) ;
  }
}

/********************************************************************/

double tet_Volume6( double *x, double *y, double *z )
{
  /*+ Function: calculate 6 times volume of tetrahedral +*/
  double D ;


  D = - x[0]*y[1]*z[2] + x[0]*y[1]*z[3] + x[0]*y[2]*z[1]
    - x[0]*y[2]*z[3] - x[0]*y[3]*z[1] + x[0]*y[3]*z[2]
    + x[1]*y[0]*z[2] - x[1]*y[0]*z[3] - x[1]*y[2]*z[0]
    + x[1]*y[2]*z[3] + x[1]*y[3]*z[0] - x[1]*y[3]*z[2]
    - x[2]*y[0]*z[1] + x[2]*y[0]*z[3] + x[2]*y[1]*z[0]
    - x[2]*y[1]*z[3] - x[2]*y[3]*z[0] + x[2]*y[3]*z[1]
    + x[3]*y[0]*z[1] - x[3]*y[0]*z[2] - x[3]*y[1]*z[0]
    + x[3]*y[1]*z[2] + x[3]*y[2]*z[0] - x[3]*y[2]*z[1] ;


  return D ;
}


/********************************************************************/

double tet_Volume( double *x, double *y, double *z )
{
  /*+ Function: calculate volume of tetrahedral +*/
  double V ;
  

  V = tet_Volume6( x, y, z ) / 6.0 ;


  return V  ;
}

/********************************************************************/

void tet_Center( double *x, double *y, double *z, double *g )
{
  /*+ Function: calculate center of gravity of tetrahedral +*/
  g[0] = ( x[0] + x[1] + x[2] + x[3] ) * 0.25 ;
  g[1] = ( y[0] + y[1] + y[2] + y[3] ) * 0.25 ;
  g[2] = ( z[0] + z[1] + z[2] + z[3] ) * 0.25 ;
}

/********************************************************************/

void tri_Center( double *x, double *y, double *z, double *g )
{
  /*+ Function: calculate center of gravity of triangle +*/
  g[0] = ( x[0] + x[1] + x[2] ) * (1/3.0) ;
  g[1] = ( y[0] + y[1] + y[2] ) * (1/3.0) ;
  g[2] = ( z[0] + z[1] + z[2] ) * (1/3.0) ;
}

/********************************************************************/

void tet_simple_a( double *x, double *y, double *z, double *a )
{
  /*+ Function: calculate determinant `a'
                      of conventional piecewise linear tetrahedral +*/
  /*+     | xj yj zj |
      a = | xk yk zk |
          | xm ym zm | +*/
  int i;


  for( i=0 ; i<4 ; i++ ) {
    int    ii = tet_m[i][1] ;
    int    jj = tet_m[i][2] ;
    int    kk = tet_m[i][3] ;
    a[i] = x[ii]*(y[jj]*z[kk] - y[kk]*z[jj])
          +x[jj]*(y[kk]*z[ii] - y[ii]*z[kk])
          +x[kk]*(y[ii]*z[jj] - y[jj]*z[ii]) ;
  }
}

/********************************************************************/

void tet_simple_b( double *x, double *y, double *z, double *b )
{
  /*+ Function: calculate determinant `b'
                      of conventional piecewise linear tetrahedral +*/
  /*+       | 1 yj zj |
      b = - | 1 yk zk |
            | 1 ym zm | +*/
  int i;


  for( i=0 ; i<4 ; i++ ) {
    int    ii = tet_m[i][1] ;
    int    jj = tet_m[i][2] ;
    int    kk = tet_m[i][3] ;
    b[i] = -1.0*( y[ii]*(z[jj] - z[kk])
                 +y[jj]*(z[kk] - z[ii])
                 +y[kk]*(z[ii] - z[jj]) ) ;
  }
}

/********************************************************************/

void tet_simple_c( double *x, double *y, double *z, double *c )
{
  /*+ Function: calculate determinant `c'
                      of conventional piecewise linear tetrahedral +*/
  /*+       | xj 1 zj |
      c = - | xk 1 zk |
            | xm 1 zm | +*/
  int i;


  for( i=0 ; i<4 ; i++ ) {
    int    ii = tet_m[i][1] ;
    int    jj = tet_m[i][2] ;
    int    kk = tet_m[i][3] ;
    c[i] = -1.0*( z[ii]*(x[jj] - x[kk])
                 +z[jj]*(x[kk] - x[ii])
                 +z[kk]*(x[ii] - x[jj]) ) ;
  }
}

/********************************************************************/

void tet_simple_d( double *x, double *y, double *z, double *d )
{
  /*+ Function: calculate determinant `c'
                      of conventional piecewise linear tetrahedral +*/
  /*+       | xj yj 1 |
      d = - | xk yk 1 |
            | xm ym 1 | +*/
  int i;


  for( i=0 ; i<4 ; i++ ) {
    int    ii = tet_m[i][1] ;
    int    jj = tet_m[i][2] ;
    int    kk = tet_m[i][3] ;
    d[i] = -1.0*( x[ii]*(y[jj] - y[kk])
                 +x[jj]*(y[kk] - y[ii])
                 +x[kk]*(y[ii] - y[jj]) ) ;
  }
}

/********************************************************************/

void tet_simple_gradgrad( double D, double *b, double *c, double *d,
                          double gg[][4] )
{
  /*+ Function: calculate (grad u, grad v) of tetrahedral +*/
  int    i, j ;

  double dd = 1.0 / (6.0 * D) ;


  for( i=0 ; i<4 ; i++ ) {
    for( j=0 ; j<4 ; j++ ) {
      gg[i][j] = (b[i]*b[j] + c[i]*c[j] + d[i]*d[j]) * dd ;
    }
  }
}

/********************************************************************/

void tet_simple_be_apgrad( double D, double *b, double *c, double *d,
                           double u[][3], double *ag )
{
  /*+ Function: calculate (u(ap), grad v) of tetrahedral for be +*/
  int    i, j ;

  double dd = 1.0 / 24.0 ;


  for( i=0 ; i<4 ; i++ ) {
    ag[i] = 0.0 ;
    for( j=0 ; j<4 ; j++ ) {
      ag[i] += (b[i]*u[j][0] + c[i]*u[j][1] + d[i]*u[j][2]) * dd ;
    }
  }
}

/********************************************************************/

void tetNedelec_Direction( double *x, double *y, double *z, int *si )
{
  /*+ Function: determine direction of Nedelec by coordinates +*/
  int    i ;

  double pp ;


  for( i=0 ; i<6 ; i++ ){
    /* try x-define */
    int    ii = tet_n[i][1] ;
    int    jj = tet_n[i][0] ;
    pp = x[ii] - x[jj] ;
    if( pp > 0.0 ) {
      si[i] =  1 ;
    } else if( pp < 0.0 ) {
      si[i] = -1 ;
    } else {
      /* try y-define */
      pp = y[ii] - y[jj] ;
      if( pp > 0.0 ) {
        si[i] =  1 ;
      } else if( pp < 0.0 ) {
        si[i] = -1 ;
      } else {
        /* try z-define */
        pp = z[ii] - z[jj] ;
        if( pp > 0.0 ) {
          si[i] =  1 ;
        } else if( pp < 0.0 ) {
          si[i] = -1 ;
        } else {
	  /* error */
          emessage( 2050, __ABORT_LIBFEM__, NULL ) ;
        }
      }
    }
  }
}

/********************************************************************/

void tetNedelec_mk( double *x, double *y, double *z,
                    double *xi, double *yi, double *zi )
{
  /*+ Function: calculate xm-xk, ym-yk, zm-zk of Nedelec +*/
  int    i ;


  for( i=0 ; i<6 ; i++ ) {
    int    ii = tet_n[i][2] ;
    int    jj = tet_n[i][3] ;
    xi[i] = x[jj] - x[ii] ;
    yi[i] = y[jj] - y[ii] ;
    zi[i] = z[jj] - z[ii] ;
  }
}

/********************************************************************/

void tetNedelec_XYZ( double *l, int *si,
                     double *xi, double *yi, double *zi,
                     double *X, double *Y, double *Z )
{
  /*+ Function: calculate X = l*si*xi,
                          Y = l*si*yi,
                          Z = l*si*zi of Nedelec +*/
  int    i ;


  for( i=0 ; i<6 ; i++ ) {
    double d = l[i] * (double)si[i] ;
    X[i] = d * xi[i] ;
    Y[i] = d * yi[i] ;
    Z[i] = d * zi[i] ;
  }
}

/********************************************************************/

void tetNedelec_km_mk( double *x, double *y, double *z,
                       double *xy, double *yz, double *zx )
{
  /*+ Function: calculate xkym-xmyk, ykzm-ymzk, zkxm-zmxk of Nedelec +*/
  int    i ;


  for( i=0 ; i<6 ; i++ ) {
    int    ii = tet_n[i][2] ;
    int    jj = tet_n[i][3] ;
    xy[i] = x[ii]*y[jj] - x[jj]*y[ii] ;
    yz[i] = y[ii]*z[jj] - y[jj]*z[ii] ;
    zx[i] = z[ii]*x[jj] - z[jj]*x[ii] ;
  }
}

/********************************************************************/

void tetNedelec_Fxyz( double *l, int *si, double *g,
                      double *xi, double *yi, double *zi,
                      double *xy, double *yz, double *zx,
                      double *Fx, double *Fy, double *Fz )
{
  /*+ Function: calculate Fx = l*si*(yz-zi*gy+yi*gz),
                          Fy = l*si*(zx-xi*gz+zi*gx),
                          Fz = l*si*(xy-yi*gx+xi*gy) of Nedelec +*/
  int    i ;


  for( i=0 ; i<6 ; i++ ) {
    double d = l[i] * (double)si[i] ;
    Fx[i] = d * (yz[i] - zi[i]*g[1] + yi[i]*g[2]) ;
    Fy[i] = d * (zx[i] - xi[i]*g[2] + zi[i]*g[0]) ;
    Fz[i] = d * (xy[i] - yi[i]*g[0] + xi[i]*g[1]) ;
  }
}

/********************************************************************/

void tetNedelec_rotrot( double D, double *X, double *Y, double *Z,
                        double rr[][6] )
{
  /*+ Function: calculate (rot u, rot v) of Nedelec +*/
  int    i, j ;

  double d = 2.0 / (3.0 * D) ;


  for( i=0 ; i<6 ; i++ ) {
    for( j=0 ; j<6 ; j++ ) {
      rr[i][j] = d * (X[i]*X[j] + Y[i]*Y[j] + Z[i]*Z[j]) ;
    }
  }
}

/********************************************************************/

void tetNedelec_sideside( double D, double *l, double *tetg, int *si,
                          double *x, double *y, double *z,
                          double *xi, double *yi, double *zi,
                          double *xy, double *yz, double *zx,
                          double ss[][6] )
{
  /*+ Function: calculate (u(side), v(side)) of Nedelec +*/
  int    i, j ;

  double d = 1.0 / (6.0 * D) ;
  double gxx[6] ;
  double dx = 0.0, dy = 0.0, dz = 0.0 ;
  double lsi[6] ;


  for( i=0 ; i<4 ; i++ ) {
    dx += x[i] ;
    dy += y[i] ;
    dz += z[i] ;
  }
  gxx[0] = dx * dx ;
  gxx[1] = dy * dy ;
  gxx[2] = dz * dz ;
  gxx[3] = dx * dy ;
  gxx[4] = dy * dz ;
  gxx[5] = dz * dx ;
  for( i=0 ; i<4 ; i++ ) {
    gxx[0] += x[i] * x[i] ;
    gxx[1] += y[i] * y[i] ;
    gxx[2] += z[i] * z[i] ;
    gxx[3] += x[i] * y[i] ;
    gxx[4] += y[i] * z[i] ;
    gxx[5] += z[i] * x[i] ;
  }
  for( i=0 ; i<6 ; i++ ) {
    gxx[i] *= 0.05 ;
  }


  for( i=0 ; i<6 ; i++ ) {
    lsi[i] = l[i] * (double)si[i] ;
  }


  for( i=0 ; i<6 ; i++ ) {
    for( j=0 ; j<6 ; j++ ) {
      ss[i][j] = lsi[i] * lsi[j] * d
                *(-zi[i]*(-zi[j]*gxx[1]  + yi[j]*gxx[4]  + yz[j]*tetg[1])
                  +yi[i]*(-zi[j]*gxx[4]  + yi[j]*gxx[2]  + yz[j]*tetg[2])
                  +yz[i]*(-zi[j]*tetg[1] + yi[j]*tetg[2] + yz[j])

                  -xi[i]*(-xi[j]*gxx[2]  + zi[j]*gxx[5]  + zx[j]*tetg[2])
                  +zi[i]*(-xi[j]*gxx[5]  + zi[j]*gxx[0]  + zx[j]*tetg[0])
                  +zx[i]*(-xi[j]*tetg[2] + zi[j]*tetg[0] + zx[j])

                  -yi[i]*(-yi[j]*gxx[0]  + xi[j]*gxx[3]  + xy[j]*tetg[0])
                  +xi[i]*(-yi[j]*gxx[3]  + xi[j]*gxx[1]  + xy[j]*tetg[1])
                  +xy[i]*(-yi[j]*tetg[0] + xi[j]*tetg[1] + xy[j])) ;
    }
  }
}

/********************************************************************/

/* S: 2 times Square of triangle. */

void triNedelec_sideside( double S, int ndID2d[3], double D, double *l, 
			  double *tetg, int *si,
                          double *x, double *y, double *z,
                          double *xi, double *yi, double *zi,
                          double *xy, double *yz, double *zx,
                          double ss[][6] )
{
  /*+ Function: calculate (u(side), v(side)) of Nedelec +*/
  int    i, j ;
  /*
  double d = 1.0 / (6.0 * D) ;
  */  
  double dds = (1.0 / D) * (1.0 / D) * (S / 2.0) ;

  double gxx[6] ;
  double dx = 0.0, dy = 0.0, dz = 0.0 ;
  double lsi[6] ;
  
  for( i=0 ; i<4 ; i++ ) {
    dx += x[i] ;
    dy += y[i] ;
    dz += z[i] ;
  }
  gxx[0] = dx * dx ;
  gxx[1] = dy * dy ;
  gxx[2] = dz * dz ;
  gxx[3] = dx * dy ;
  gxx[4] = dy * dz ;
  gxx[5] = dz * dx ;
  for( i=0 ; i<4 ; i++ ) {
    gxx[0] += x[i] * x[i] ;
    gxx[1] += y[i] * y[i] ;
    gxx[2] += z[i] * z[i] ;
    gxx[3] += x[i] * y[i] ;
    gxx[4] += y[i] * z[i] ;
    gxx[5] += z[i] * x[i] ;
  }
  for( i=0 ; i<6 ; i++ ) {
    gxx[i] *= 0.05 ;
  }


  for( i=0 ; i<6 ; i++ ) {
    lsi[i] = l[i] * (double)si[i] ;
  }


  for( i=0 ; i<6 ; i++ ) {
    //if( i==ndID2d[0] || i==ndID2d[1] || i==ndID2d[2] ){
      for( j=0 ; j<6 ; j++ ) {
	if( ( i==ndID2d[0] || i==ndID2d[1] || i==ndID2d[2] )&&
	    ( j==ndID2d[0] || j==ndID2d[1] || j==ndID2d[2] ) 
	    ){/*If the edge belong the ABC,ss[][] is calculated. */
	  ss[i][j] = lsi[i] * lsi[j] * dds /* d -> dds */
	    *(-zi[i]*(-zi[j]*gxx[1]  + yi[j]*gxx[4]  + yz[j]*tetg[1])
	      +yi[i]*(-zi[j]*gxx[4]  + yi[j]*gxx[2]  + yz[j]*tetg[2])
	      +yz[i]*(-zi[j]*tetg[1] + yi[j]*tetg[2] + yz[j])
	      -xi[i]*(-xi[j]*gxx[2]  + zi[j]*gxx[5]  + zx[j]*tetg[2])
	      +zi[i]*(-xi[j]*gxx[5]  + zi[j]*gxx[0]  + zx[j]*tetg[0])
	      +zx[i]*(-xi[j]*tetg[2] + zi[j]*tetg[0] + zx[j])
	      -yi[i]*(-yi[j]*gxx[0]  + xi[j]*gxx[3]  + xy[j]*tetg[0])
	      +xi[i]*(-yi[j]*gxx[3]  + xi[j]*gxx[1]  + xy[j]*tetg[1])
	      +xy[i]*(-yi[j]*tetg[0] + xi[j]*tetg[1] + xy[j])) ;
	}
      else
	{
	  ss[i][j] = 0.0 ;
	}
      }
      //}
  }
}

/********************************************************************/

double tri_square( double *x, double *y, double *z, double *nomal )
{
  /*+ Function: calculate square and nomal vector of triangle +*/
  /*+         |  |   i     j     k   | |
      a =(1/2)|  | x1-x0 y1-y0 z1-z0 | |
              |  | x2-x0 y2-y0 z2-z0 | |+*/
     
  double S;
  double xg, yg, zg ;
  double abs_nomal;
  
  xg =    (y[1]-y[0]) * (z[2]-z[0]) - (z[1]-z[0]) * (y[2]-y[0]);
  yg = -( (x[1]-x[0]) * (z[2]-z[0]) - (z[1]-z[0]) * (x[2]-x[0]) );
  zg =    (x[1]-x[0]) * (y[2]-y[0]) - (y[1]-y[0]) * (x[2]-x[0]);
  
  abs_nomal = sqrt( xg*xg + yg*yg + zg*zg );


  printf("xg=%lf yg=%lf zg=%lf abs_nomal=%lf\n",xg, yg, zg, abs_nomal);

  nomal[0] = xg / abs_nomal ;
  nomal[1] = yg / abs_nomal ;
  nomal[2] = zg / abs_nomal ;

  S = 0.5 * abs_nomal; 

  return S;

}

/********************************************************************/

void tet_cross( double *x, double *y, double *z, double *nomal,
		double *xc, double *yc, double *zc)
{
  /*+ Function:x xcalculate square and nomal vector of triangle +*/
  /*+     |   i     j     k   | 
      a = |   nx    ny    nz  |
          |   xi    yi    zi  | +*/
     
  int i;
  //double S;
  //double xg, yg, zg; 
  
  for( i=0 ; i<4 ; i++ ){
    xc[i] =    nomal[1] * z[i] - nomal[2] * y[i];
    yc[i] = -( nomal[0] * z[i] - nomal[2] * x[i] );
    zc[i] =    nomal[0] * y[i] - nomal[1] * x[i];
  }
	
}

/********************************************************************/

void tetNedelec_pointpoint( double D, double *a, double *b, double *c, double   
			    *d, double *x, double *y, double *z, double pp[][4])
{
  /*+ Function: calculate (p(point), p*(point)) of Nedelec +*/
  int    i, j ;
  double dd=D/120.0 ;
  double gxx[6] ;
  double dx = 0.0, dy = 0.0, dz = 0.0 ;

  for( i=0 ; i<4 ; i++ ) {
    dx += x[i] ;
    dy += y[i] ;
    dz += z[i] ;
  }
  gxx[0] = dx * dx ;
  gxx[1] = dy * dy ;
  gxx[2] = dz * dz ;
  gxx[3] = dx * dy ;
  gxx[4] = dy * dz ;
  gxx[5] = dz * dx ;
  for( i=0 ; i<4 ; i++ ) {
    gxx[0] += x[i] * x[i] ;
    gxx[1] += y[i] * y[i] ;
    gxx[2] += z[i] * z[i] ;
    gxx[3] += x[i] * y[i] ;
    gxx[4] += y[i] * z[i] ;
    gxx[5] += z[i] * x[i] ;
  }


  for( i=0 ; i<4 ; i++ ) {
    for( j=0 ; j<4 ; j++ ) {
      pp[i][j] = dd * ( 20.0 * a[i] * a[j]
			+ 5.0 * ( a[i] * b[j] + a[j] * b[i] ) * dx
			+ 5.0 * ( a[i] * c[j] + a[j] * c[i] ) * dy
			+ 5.0 * ( a[i] * d[j] + a[j] * d[i] ) * dy
			+ ( b[i] * c[j] + b[j] * c[i] ) * gxx[3]
			+ ( c[i] * d[j] + c[j] * d[i] ) * gxx[4]
			+ ( b[i] * d[j] + b[j] * d[i] ) * gxx[5]
			+ b[i] * b[j] * gxx[0]
			+ c[i] * c[j] * gxx[1]
			+ d[i] * d[j] * gxx[2] );
    }
  }
}

/********************************************************************/

void tetNedelec_be_sideside( double *u,
                             double D, double *l, double *tetg, int *si,
                             double *x, double *y, double *z,
                             double *xi, double *yi, double *zi,
                             double *xy, double *yz, double *zx,
                             double *be_ss )
{
  /*+ Function: calculate (u(side), v(side)) of Nedelec +*/
  int    i, j ;

  double ss[6][6] ;

  tetNedelec_sideside( D, l, tetg, si, x, y, z, xi, yi, zi,
                       xy, yz, zx, ss ) ;
  for( i=0 ; i<6 ; i++ ) {
    be_ss[i] = 0.0 ;
    for( j=0 ; j<6 ; j++ ) {
      be_ss[i] += u[j] * ss[i][j] ;
    }
  }
}

/********************************************************************/

void tetNedelec_be_elrot( double *u, double *X, double *Y, double *Z,
                          double *er )
{
  /*+ Function: calculate (u(element), rot v) of Nedelec for be +*/
  int    i ;

  double d = 1.0 / 3.0 ;


  for( i=0 ; i<6 ; i++ ) {
    er[i] = d * (u[0]*X[i] + u[1]*Y[i] + u[2]*Z[i] ) ;
  }
}

/********************************************************************/

void tetNedelec_gradside( double D,
                          double *b, double *c, double *d,
                          double *Fx, double *Fy, double *Fz,
                          double gs[][4] )
{
  /*+ Function: calculate (grad u, v(side)) of Nedelec +*/
  int    i, j ;

  double dd = 1.0 / (6.0 * D) ;


  for( i=0 ; i<6 ; i++ ) {
    for( j=0 ; j<4 ; j++ ) {
      gs[i][j] = dd * (Fx[i]*b[j] + Fy[i]*c[j] + Fz[i]*d[j]) ;
    }
  }
}

/********************************************************************/

void tetNedelec_be_gradside( double *u, double D,
                             double *b, double *c, double *d,
                             double *Fx, double *Fy, double *Fz,
                             double *be_gs )
{
  /*+ Function: calculate (grad u, v(side)) of Nedelec +*/
  int    i, j ;

  double gs[6][4] ;


  tetNedelec_gradside( D, b, c, d, Fx, Fy, Fz, gs ) ;
  for( i=0 ; i<6 ; i++ ) {
    be_gs[i] = 0.0 ;
    for( j=0 ; j<4 ; j++ ) {
      be_gs[i] += u[j] * gs[i][j] ;
    }
  }
}

/********************************************************************/

void tetNedelec_sidegrad( double D,
                          double *b, double *c, double *d,
                          double *Fx, double *Fy, double *Fz,
                          double sg[][6] )
{
  /*+ Function: calculate (u(side), grad v) of Nedelec +*/
  int    i, j ;

  double dd = 1.0 / (6.0 * D) ;


  for( i=0 ; i<4 ; i++ ) {
    for( j=0 ; j<6 ; j++ ) {
      sg[i][j] = dd * (b[i]*Fx[j] + c[i]*Fy[j] + d[i]*Fz[j]) ;
    }
  }
}

/********************************************************************/

void tetNedelec_be_sidegrad( double *u, double D,
                             double *b, double *c, double *d,
                             double *Fx, double *Fy, double *Fz,
                             double *be_sg )
{
  /*+ Function: calculate (u(side), grad v) of Nedelec +*/
  int    i, j ;

  double sg[4][6] ;


  tetNedelec_sidegrad( D, b, c, d, Fx, Fy, Fz, sg ) ;
  for( i=0 ; i<4 ; i++ ) {
    be_sg[i] = 0.0 ;
    for( j=0 ; j<6 ; j++ ) {
      be_sg[i] += u[j] * sg[i][j] ;
    }
  }
}

/********************************************************************/

void tetNedelec_JoA( double Joe[][3], int *si, double *l,
                     double *b, double *c, double *d,
                     double *be_JoA )
{
  /*+ Function: calculate (Jo(apex*3), A*) of Nedelec +*/
  int    i, j, k ;

  double PN[6][12] ;
  double d0[][4] = { {  2.0,  1.0,  1.0,  1.0},
                     {  2.0,  1.0,  1.0,  1.0},
                     {  2.0,  1.0,  1.0,  1.0},
                     {  1.0,  2.0,  1.0,  1.0},
                     {  1.0,  2.0,  1.0,  1.0},
                     {  1.0,  1.0,  2.0,  1.0} } ;
  double d1[][4] = { { -1.0, -2.0, -1.0, -1.0},
                     { -1.0, -1.0, -2.0, -1.0},
                     { -1.0, -1.0, -1.0, -2.0},
                     { -1.0, -1.0, -2.0, -1.0},
                     { -1.0, -1.0, -1.0, -2.0},
                     { -1.0, -1.0, -1.0, -2.0} } ;


  for( i=0 ; i<6 ; i++ ) {
    double dd = (double)si[i] * l[i] / 120.0 ;
    for( j=0 ; j<12 ; j++ ) {
      PN[i][j] = dd ;
    }
  }

  for( i=0 ; i<6 ; i++ ) {
    for( j=0 ; j<4 ; j++ ) {
      PN[i][j] *= d0[i][j]*b[tet_n[i][1]] + d1[i][j]*b[tet_n[i][0]] ;
    }
  }
  for( i=0 ; i<6 ; i++ ) {
    for( j=0,k=4 ; j<4 ; j++,k++ ) {
      PN[i][k] *= d0[i][j]*c[tet_n[i][1]] + d1[i][j]*c[tet_n[i][0]] ;
    }
  }
  for( i=0 ; i<6 ; i++ ) {
    for( j=0,k=8 ; j<4 ; j++,k++ ) {
      PN[i][k] *= d0[i][j]*d[tet_n[i][1]] + d1[i][j]*d[tet_n[i][0]] ;
    }
  }

  for( i=0 ; i<6 ; i++ ) {
    be_JoA[i] = 0.0 ;
    for( j=0 ; j<4 ; j++ ) {
      be_JoA[i] += PN[i][j] * Joe[j][0] ;
    }
  }
  for( i=0 ; i<6 ; i++ ) {
    for( j=0,k=4 ; j<4 ; j++,k++ ) {
      be_JoA[i] += PN[i][k] * Joe[j][1] ;
    }
  }
  for( i=0 ; i<6 ; i++ ) {
    for( j=0,k=8 ; j<4 ; j++,k++ ) {
      be_JoA[i] += PN[i][k] * Joe[j][2] ;
    }
  }
}

/********************************************************************/

void tetNedelec_JoA_Complex( Complex Joe[][3], int *si, double *l,
                             double *b, double *c, double *d,
                             Complex *be_JoA )
{
  /*+ Function: calculate (Jo(apex*3), A*) of Nedelec +*/
  int    i, j, k ;

  double PN[6][12] ;
  double d0[][4] = { {  2.0,  1.0,  1.0,  1.0},
                     {  2.0,  1.0,  1.0,  1.0},
                     {  2.0,  1.0,  1.0,  1.0},
                     {  1.0,  2.0,  1.0,  1.0},
                     {  1.0,  2.0,  1.0,  1.0},
                     {  1.0,  1.0,  2.0,  1.0} } ;
  double d1[][4] = { { -1.0, -2.0, -1.0, -1.0},
                     { -1.0, -1.0, -2.0, -1.0},
                     { -1.0, -1.0, -1.0, -2.0},
                     { -1.0, -1.0, -2.0, -1.0},
                     { -1.0, -1.0, -1.0, -2.0},
                     { -1.0, -1.0, -1.0, -2.0} } ;


  for( i=0 ; i<6 ; i++ ) {
    double dd = (double)si[i] * l[i] / 120.0 ;
    for( j=0 ; j<12 ; j++ ) {
      PN[i][j] = dd ;
    }
  }

  for( i=0 ; i<6 ; i++ ) {
    for( j=0 ; j<4 ; j++ ) {
      PN[i][j] *= d0[i][j]*b[tet_n[i][1]] + d1[i][j]*b[tet_n[i][0]] ;
    }
  }
  for( i=0 ; i<6 ; i++ ) {
    for( j=0,k=4 ; j<4 ; j++,k++ ) {
      PN[i][k] *= d0[i][j]*c[tet_n[i][1]] + d1[i][j]*c[tet_n[i][0]] ;
    }
  }
  for( i=0 ; i<6 ; i++ ) {
    for( j=0,k=8 ; j<4 ; j++,k++ ) {
      PN[i][k] *= d0[i][j]*d[tet_n[i][1]] + d1[i][j]*d[tet_n[i][0]] ;
    }
  }

  for( i=0 ; i<6 ; i++ ) {
    be_JoA[i] = complex_0 ;
    for( j=0 ; j<4 ; j++ ) {
      be_JoA[i] = ComplexPlus( be_JoA[i],
                               ComplexMultiScalar( Joe[j][0], PN[i][j]) ) ;
    }
  }
  for( i=0 ; i<6 ; i++ ) {
    for( j=0,k=4 ; j<4 ; j++,k++ ) {
      be_JoA[i] = ComplexPlus( be_JoA[i],
                               ComplexMultiScalar( Joe[j][1], PN[i][k]) ) ;
    }
  }
  for( i=0 ; i<6 ; i++ ) {
    for( j=0,k=8 ; j<4 ; j++,k++ ) {
      be_JoA[i] = ComplexPlus( be_JoA[i],
                               ComplexMultiScalar( Joe[j][2], PN[i][k]) ) ;
    }
  }
}
