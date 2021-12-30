/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "libfem.h"


void tet_MakeElement_correct_Jo( int antenna, int el,
                                 int *nop, double *crd, int *enf,
                                 int flg, MTRL mtrl,
                                 double *Jo, TMP_AIJ *tmp_aij )
{
  /*+ Function: make element
                  conventional piecewise linear tetrahedral
                  correct Jo +*/
  int    i, j, k ;

  int    ap_elem = 4 ;
  int    nd_elem = 10 ;
  int    dimension = 3 ;

  int    *ne ;



  double jo[ap_elem][dimension] ;
  double x[ap_elem], y[ap_elem], z[ap_elem] ;
  double D ;
  double b[ap_elem], c[ap_elem], d[ap_elem] ;
  double gg[ap_elem][ap_elem] ;
  int    szRow = ap_elem ;
  int    indRow[szRow] ;

  double ae[ap_elem*ap_elem] ;
  double be[ap_elem] ;


  if( flg != mtrl.antenna_def[antenna].flag ) {
    return ;
  }
  ne = &(nop[el*nd_elem]) ;
  for( i=0 ; i<szRow ; i++ ) {
    indRow[i] = enf[ne[i]] ;
  }
  for( i=0 ; i<ap_elem ; i++ ) {
    int    ii = indRow[i] * dimension ;
    for( j=0 ; j<dimension ; j++,ii++ ) {
      jo[i][j] = Jo[ii] ;
    }
  }
  tet_pickup_coordinate_4vertex( ne, crd, x, y, z ) ;
  D = tet_Volume6( x, y, z ) ;
  tet_simple_b( x, y, z, b ) ;
  tet_simple_c( x, y, z, c ) ;
  tet_simple_d( x, y, z, d ) ;
  tet_simple_gradgrad( D, b, c, d, gg ) ;

  for( i=0,k=0 ; i<ap_elem ; i++ ) {
    for( j=0 ; j<ap_elem ; j++,k++ ) {

      ae[k] =  gg[i][j] ;
    }
  }
  tet_simple_be_apgrad( D, b, c, d, jo, be ) ;

  mymt_RS_AIJ_Set( tmp_aij, szRow, indRow, szRow, indRow, ae, be ) ;
}

/********************************************************************/

void tet_MakeElement_correct_Jo_b( int antenna, int el,
                                   int *nop, double *crd, int *enf,
                                   int flg, MTRL mtrl,
                                   double *Jo, double *B )
{
  /*+ Function: make element only b
                  conventional piecewise linear tetrahedral
                  correct Jo +*/
  int    i, j ;

  int    ap_elem = 4 ;
  int    nd_elem = 10 ;
  int    dimension = 3 ;

  int    *ne ;
  double jo[ap_elem][dimension] ;
  double x[ap_elem], y[ap_elem], z[ap_elem] ;
  double D ;
  double b[ap_elem], c[ap_elem], d[ap_elem] ;
  int    szRow = ap_elem ;
  int    indRow[szRow] ;

  double be[ap_elem] ;


  if( flg != mtrl.antenna_def[antenna].flag ) {
    return ;
  }
  ne = &(nop[el*nd_elem]) ;
  for( i=0 ; i<szRow ; i++ ) {
    indRow[i] = enf[ne[i]] ;
  }
  for( i=0 ; i<ap_elem ; i++ ) {
    int    ii = indRow[i] * dimension ;
    for( j=0 ; j<dimension ; j++,ii++ ) {
      jo[i][j] = Jo[ii] ;
    }
  }
  tet_pickup_coordinate_4vertex( ne, crd, x, y, z ) ;
  D = tet_Volume6( x, y, z ) ;
  tet_simple_b( x, y, z, b ) ;
  tet_simple_c( x, y, z, c ) ;
  tet_simple_d( x, y, z, d ) ;

  tet_simple_be_apgrad( D, b, c, d, jo, be ) ;

  mymt_RS_AIJ_SetFe( B, szRow, indRow, be ) ;
}
