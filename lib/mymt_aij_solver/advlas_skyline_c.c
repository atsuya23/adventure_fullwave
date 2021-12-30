/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "advlas_skyline_c.h"


/**************************************************************************/
/**
 * Convert index of nonzero data structure to skyline data structure.
 */
void advlas_copy_nzindex_to_skyindex(int *NzIdx_ia, int *NzIdx_jcn, 
				     DMatrix *dmat)
{
  int i;
  
  for (i = 0; i < dmat->ndim; i++)
    dmat->idx1[i] = NzIdx_jcn[NzIdx_ia[i]];

  dmat->idx2[0] = 0;
  for (i = 1; i < dmat->ndim; i++)
    dmat->idx2[i] = dmat->idx2[i-1] + i - dmat->idx1[i];

  dmat->nlen = dmat->idx2[dmat->ndim-1] + dmat->ndim;
}

/**************************************************************************/
/**
 * Convert matrix of nonzero data structure to skyline data structure.
 */

void advlas_copy_nzmat_to_skymat(int *NzIdx_ia, int *NzIdx_jcn, 
				 MTYPE *NzMat, DMatrix *dmat)
{
  int i, j;
  Complex *NzMat_c;
  
  NzMat_c = (Complex *)NzMat;

 for (i = 0; i < dmat->ndim; i++)
   for (j = NzIdx_ia[i] ; j < NzIdx_ia[i+1]; j++)
      dmat->mat_c[dmat->idx2[i]+NzIdx_jcn[j]] = NzMat_c[j];
 
}

/**************************************************************************/
/**
 * Do modified Cholesky factorization of skyline matrix.
 */
void advlas_skyline_ldl_decomposite(DMatrix *dmat, MTYPE* work)
{
  int nDofs, iDof, jDof, kDof, iMax;
  int *Index_ii, *Index_ij;
  MTYPE *SMat, *SMatDiag, r;

  nDofs = dmat->ndim;
  Index_ii = dmat->idx1;
  Index_ij = dmat->idx2;
  SMat = (MTYPE*)dmat->mat_c;
  SMatDiag = work;

  SMatDiag[0] = MRECIP(SMat[0]) ;
  for (iDof = 1; iDof < nDofs; iDof++)
    SMatDiag[iDof] = SMat[Index_ij[iDof]+iDof];

  for (iDof = 1; iDof < nDofs; iDof++) {
    int ifst = Index_ij[iDof];
    for (jDof = Index_ii[iDof]; jDof < iDof; jDof++) {
      int jfst = Index_ij[jDof];
      if (Index_ii[iDof] > Index_ii[jDof])
	iMax = Index_ii[iDof];
      else
	iMax = Index_ii[jDof];
      r = M_ZERO ;
      for (kDof = iMax; kDof < jDof; kDof++)
	r = MPLUS( r , MMULTI(SMat[ifst+kDof],SMat[jfst+kDof]) ) ;
      SMat[ifst+jDof] = MMINUS( SMat[ifst+jDof],r );
    }
    r = M_ZERO ;    
    for (kDof = Index_ii[iDof]; kDof < iDof; kDof++) {
     r = MPLUS( r , MMULTI(SMat[ifst+kDof],
			   MMULTI(SMat[ifst+kDof],SMatDiag[kDof])
					)
			);	
      SMat[ifst+kDof] = MMULTI(SMat[ifst+kDof],SMatDiag[kDof]);
    }
    SMatDiag[iDof] = MRECIP(MMINUS(SMatDiag[iDof],r));
  }

  for (iDof = 0; iDof < nDofs; iDof++)
    SMat[Index_ij[iDof]+iDof] = SMatDiag[iDof];

}

/**************************************************************************/
/**
 * Make solution by modified Cholesky factorization.
 */
void advlas_skyline_ldl_solve(DMatrix *dmat, MTYPE* solution)
{
  int nDofs, iDof, jDof;
  int *Index_ii, *Index_ij;
  MTYPE *SMat, *v;

  nDofs = dmat->ndim;
  Index_ii = dmat->idx1;
  Index_ij = dmat->idx2;
  SMat = (MTYPE*)dmat->mat_c;
  v = solution;

  for (iDof = 1; iDof < nDofs; iDof++) {
    int ii = Index_ij[iDof];
    MTYPE r = M_ZERO ;
    for (jDof = Index_ii[iDof]; jDof < iDof; jDof++)
      r = MPLUS( r , MMULTI(SMat[ii+jDof],v[jDof]) );
    v[iDof] = MMINUS(v[iDof],r);
  }

  for (iDof = 0; iDof < nDofs; iDof++) {
    v[iDof] = MMULTI(v[iDof],SMat[Index_ij[iDof]+iDof]);
  }

  for (iDof = nDofs-1; iDof >= 1; iDof--) {
    int ii = Index_ij[iDof];
    MTYPE r = v[iDof];
    for (jDof = Index_ii[iDof]; jDof < iDof; jDof++) {
      v[jDof] = MMINUS(v[jDof],MMULTI(SMat[ii+jDof],r ));
    }
  }
}

/**************************************************************************/
