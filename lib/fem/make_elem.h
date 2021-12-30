/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __MAKE_ELEM_H__
#define __MAKE_ELEM_H__


#include "libfem.h"


void tet_MakeElement_correct_Jo( int antenna, int el,
                                 int *nop, double *crd, int *enf,
                                 int flg, MTRL mtrl,
                                 double *Jo, TMP_AIJ *tmp_aij ) ;
void tet_MakeElement_correct_Jo_b( int antenna, int el,
                                   int *nop, double *crd, int *enf,
                                   int flg, MTRL mtrl,
                                   double *Jo, double *B ) ;

#endif /* __MAKE_ELEM_H__ */
