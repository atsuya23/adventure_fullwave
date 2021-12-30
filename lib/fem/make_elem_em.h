/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __MAKE_ELEM_EM_H__
#define __MAKE_ELEM_EM_H__


#include "libfem.h"


void tet_MakeElement_EM_Ephi( int el, int *nop, double *crd,
				 int *flag, int *apid2phiid,
				 ANTENNA *antenna, MTRL mtrl,
				 TMP_AIJ *tmp_aij ) ;
void tet_MakeElement_EM_Ep( int el, int *nop, double *crd,
			       int *flag, int *apid2phiid,
			       ANTENNA *antenna, MTRL mtrl,
			       TMP_AIJ *tmp_aij ) ;
void tet_MakeElement_EM_E( int el, int *nop, double *crd,
			      int *flag, ANTENNA *antenna, MTRL mtrl,
			      TMP_AIJ *tmp_aij ) ;

#endif /* __MAKE_ELEM_FULL_WAVE_H__ */
