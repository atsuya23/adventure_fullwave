/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "sort.h"


void sort_i( int n, int *vi )
{
  /*+ Function: sort int +*/
#if __ADVFW_SORT__ == __ADVFW_SORT_BUBBLE_SORT__
  sort_i_bubble_sort( n, vi ) ;
#else
  sort_i_bubble_sort( n, vi ) ;
#endif
}

/***********************************************************************/

void sort_id( int n, int *vi, double *vd, int row )
{
  /*+ Function: sort int & double +*/
#if __ADVFW_SORT__ == __ADVFW_SORT_BUBBLE_SORT__
  sort_id_bubble_sort( n, vi, vd, row ) ;
#else
  sort_id_bubble_sort( n, vi, vd, row ) ;
#endif
}

/***********************************************************************/

void sort_ic( int n, int *vi, Complex *vc, int row )
{
  /*+ Function: sort int & double +*/
#if __ADVFW_SORT__ == __ADVFW_SORT_BUBBLE_SORT__
  sort_ic_bubble_sort( n, vi, vc, row ) ;
#else
  sort_ic_bubble_sort( n, vi, vc, row ) ;
#endif
}

/***********************************************************************/

void sort_i_bubble_sort( int n, int *vi )
{
  /*+ Function: sort int by bubble sort +*/
  int    i, j ;


  for( i=1 ; i<n ; i++ ) {
    for( j=n-1 ; j>=i ; j-- ) {
      if( vi[j-1] > vi[j] ) {
        int    ii ;
        ii = vi[j-1] ; vi[j-1] = vi[j] ; vi[j] = ii ;
      }
    }
  }
}

/***********************************************************************/

void sort_id_bubble_sort( int n, int *vi, double *vd, int row )
{
  /*+ Function: sort int & double by bubble sort +*/
  int    i, j, k ;


  for( i=1 ; i<n ; i++ ) {
    for( j=n-1 ; j>=i ; j-- ) {
      if( vi[j-1] > vi[j] ) {
        int    ii ;
        int    jj = (j - 1) * row ;
        int    kk = j * row ;
        ii = vi[j-1] ; vi[j-1] = vi[j] ; vi[j] = ii ;
        for( k=0 ; k<row ; k++ ) {
          double dd ;
          dd = vd[jj+k] ; vd[jj+k] = vd[kk+k] ; vd[kk+k] = dd ;
        }
      }
    }
  }
}

/***********************************************************************/

void sort_ic_bubble_sort( int n, int *vi, Complex *vc, int row )
{
  /*+ Function: sort int & double by bubble sort +*/
  int    i, j, k ;


  for( i=1 ; i<n ; i++ ) {
    for( j=n-1 ; j>=i ; j-- ) {
      if( vi[j-1] > vi[j] ) {
        int    ii ;
        int    jj = (j - 1) * row ;
        int    kk = j * row ;
        ii = vi[j-1] ; vi[j-1] = vi[j] ; vi[j] = ii ;
        for( k=0 ; k<row ; k++ ) {
          Complex cc ;
          cc = vc[jj+k] ; vc[jj+k] = vc[kk+k] ; vc[kk+k] = cc ;
        }
      }
    }
  }
}
