/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef  __SORT_H__
#define  __SORT_H__

#include "advfullwave_lib.h"


#define __ADVFW_SORT_BUBBLE_SORT__ 0

#define __ADVFW_SORT__             __ADVFW_SORT_BUBBLE_SORT__


/*** sort main ***/
void sort_i( int n, int *vi ) ;
void sort_id( int n, int *vi, double *vd, int row ) ;
void sort_ic( int n, int *vi, Complex *vc, int row ) ;



/*** bubble sort ***/
void sort_i_bubble_sort( int n, int *vi ) ;
void sort_id_bubble_sort( int n, int *vi, double *vd, int row ) ;
void sort_ic_bubble_sort( int n, int *vi, Complex *vc, int row ) ;

#endif /* __SORT_H__ */
