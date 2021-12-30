#include <stdio.h>
#include <stdlib.h>
#include "advlas_skyline.h"


/**************************************************************************/

/**
 * Convert index of nonzero data structure to skyline data structure.
 */

void advlas_copy_nzindex_to_skyindex(int** taij, DMatrix *dmat)
{
  int i;

  for (i = 0; i < dmat->ndim; i++){
    dmat->idx1[i] = taij[i][0];
  }

  dmat->idx2[0] = 0;
  for (i = 1; i < dmat->ndim; i++){
    dmat->idx2[i] = dmat->idx2[i-1] + i - dmat->idx1[i];
  }
  dmat->nlen = dmat->idx2[dmat->ndim-1] + dmat->ndim;
}

void advlas_copy_nzindex_to_skyindex_org(int* NzIdx_ia, int* NzIdx_jcn,
				     DMatrix *dmat)
{
  int i;

  for (i = 0; i < dmat->ndim; i++){
    dmat->idx1[i] = NzIdx_jcn[NzIdx_ia[i]];
  }

  dmat->idx2[0] = 0;
  for (i = 1; i < dmat->ndim; i++){
    dmat->idx2[i] = dmat->idx2[i-1] + i - dmat->idx1[i];
  }
  dmat->nlen = dmat->idx2[dmat->ndim-1] + dmat->ndim;
}

/**************************************************************************/

/**
 * Convert matrix of nonzero data structure to skyline data structure.
 */

void advlas_copy_nzmat_to_skymat(int *ai, int **taij, Complex **tvij,
				 DMatrix *dmat)
{
  int i, j ;

  for (i = 0; i < dmat->ndim; i++){
    for (j = 0; j < ai[i]; j++){
      dmat->mat_dc[dmat->idx2[i]+taij[i][j]] =
	tvij[i][j].re + tvij[i][j].im * I ;
    }
  }
}

void advlas_copy_nzmat_to_skymat_org(int *NzIdx_ia, int *NzIdx_jcn, Complex *NzMat,
				 DMatrix *dmat)
{
  int i, j ;

  for (i = 0; i < dmat->ndim; i++){
    for (j = NzIdx_ia[i]; j < NzIdx_ia[i+1]; j++){
      dmat->mat_c[dmat->idx2[i]+NzIdx_jcn[j]] = NzMat[j];
    }
  }
}

/**************************************************************************/

/**
 * Do modified Cholesky factorization of skyline matrix.
 */ 
void advlas_skyline_ldl_decomposite(DMatrix dmat, double _Complex *work){
  int nDofs, iDof, jDof, kDof, iMax ;
  int *Index_ii, *Index_ij ;
  double _Complex *SMat, *SMatDiag, r ;

  nDofs = dmat.ndim ;
  Index_ii = dmat.idx1 ;
  Index_ij = dmat.idx2 ;
  SMat = dmat.mat_dc ;
  SMatDiag = work ;

  SMatDiag[0] = 1/SMat[0] ;
  for (iDof = 1; iDof < nDofs; iDof++){
    SMatDiag[iDof] = SMat[Index_ij[iDof]+iDof];
  }

  for (iDof = 1; iDof < nDofs; iDof++) {
    int ifst=Index_ij[iDof];
    for (jDof = Index_ii[iDof]; jDof < iDof; jDof++) {
      int jfst=Index_ij[jDof];

      if (Index_ii[iDof] > Index_ii[jDof]){
	iMax = Index_ii[iDof];
      }else{
	iMax = Index_ii[jDof];
      }

      r = 0.0 + 0.0 * I;

      for (kDof = iMax; kDof < jDof; kDof++){
	r += SMat[ifst+kDof] * SMat[jfst+kDof] ;
      } 
      SMat[ifst+jDof] -= r ;
    }

    r = 0.0 + 0.0 * I ;
 
    for (kDof = Index_ii[iDof]; kDof < iDof; kDof++) {
      r += SMat[ifst+kDof] * SMat[ifst+kDof] * SMatDiag[kDof] ;
      SMat[ifst+kDof] *= SMatDiag[kDof] ;
       }
    SMatDiag[iDof] = 1 / (SMatDiag[iDof] - r) ;
  }
  
  for (iDof = 0; iDof < nDofs; iDof++){
    SMat[Index_ij[iDof]+iDof] = SMatDiag[iDof] ;
  }
  
  dmat.mat_dc= SMat ;
}

/***********************************************************************/
/**
 * Make solution by modified Cholesky factorization.
 */
void advlas_skyline_ldl_solve(DMatrix dmat, double _Complex *solution)
{
  int nDofs, iDof, jDof;
  int *Index_ii, *Index_ij;
  double _Complex *SMat, *v, r ;

  nDofs = dmat.ndim;
  Index_ii = dmat.idx1;
  Index_ij = dmat.idx2;
  SMat = dmat.mat_dc;
  v = solution;

  for (iDof = 1; iDof < nDofs; iDof++) {
    int ii = Index_ij[iDof];
    r = 0.0 + 0.0 /* * I*/ ;
    for (jDof = Index_ii[iDof]; jDof < iDof; jDof++){
      r += SMat[ii+jDof] * v[jDof] ;
    }
    v[iDof] -= r ;
  }

  for (iDof = 0; iDof < nDofs; iDof++) {
    v[iDof] *= SMat[Index_ij[iDof]+iDof] ;
  }

  for (iDof = nDofs-1; iDof >= 1; iDof--) {
    int ii = Index_ij[iDof];
    double _Complex r = v[iDof] ;
    for (jDof = Index_ii[iDof]; jDof < iDof; jDof++) {
      v[jDof] -= SMat[ii+jDof] * r ;
    }
  }
  solution = v ;
}

/********************************************************************/

void dmat_init(DMatrix *dmat)
{

  dmat->ndim = 0 ;
  dmat->nlen = 0 ;
  dmat->idx1 = NULL ;
  dmat->idx2 = NULL ;
  dmat->mat = NULL ;
  dmat->rhs = NULL ;
  dmat->mat_c = NULL ;
  dmat->rhs_c = NULL ;
}


/********************************************************************/

void dmat_free(DMatrix *dmat)
{

    if(dmat->idx1!=NULL){
    Free( sizeof(int)*(dmat->ndim + 2), dmat->idx1 ) ;
    //printf("free:idx1\n") ;
  } else{
    dmat->idx1 = NULL ;
  }

  if(dmat->idx2!=NULL){
    Free( sizeof(int)*(dmat->ndim + 2), dmat->idx2 ) ;
    //printf("free:idx2\n") ;
  }else{
    dmat->idx2 = NULL ;
  }

  if(dmat->mat!=NULL){
    Free( sizeof(double)*dmat->nlen, dmat->mat ) ;
    //printf("free:mat\n") ;
  }else{

    dmat->mat = NULL ;
    //printf("free:dmat->mat = NULL\n") ;  
  }

  if(dmat->rhs!=NULL){
    Free( sizeof(double)*dmat->ndim, dmat->rhs ) ;
    //printf("free:rhs\n") ;
  }else{
    dmat->rhs = NULL ;
    //printf("free:dmat->rhs = NULL\n") ;  
  }

  if(dmat->mat_c!=NULL){
    //printf("free:mat_c0\n") ;
    Free( sizeof(Complex)*dmat->nlen + 1, dmat->mat_c ) ;
    //printf("free:mat_c\n") ;
  }else{
    dmat->mat_c = NULL ;
    //printf("free:dmat->mat_c = NULL\n") ;  
  }

  if(dmat->rhs_c!=NULL){
    //printf("free:rhs_c0\n") ;
    Free( sizeof(Complex)*dmat->ndim + 1, dmat->rhs_c ) ;
    //printf("free:rhs_c\n") ;
  }else{
    dmat->rhs_c = NULL ;
    //printf("free:dmat->rhs_c = NULL\n") ;  
  }

}

/************************************************************************/

void skyline_copy(DMatrix  *dmat,  DMatrix  dmat_orig) 
{

  int  i ;
  int ndim = dmat_orig.ndim ;
  int  nlen = dmat_orig.nlen ;

  dmat->ndim = ndim ;
  dmat->nlen = nlen ;
  dmat->idx1 = (int *)Calloc( sizeof(int),ndim + 2 ) ;
  dmat->idx2 = (int *)Calloc( sizeof(int),ndim + 2 ) ;
  dmat->mat_c = (Complex *)Calloc( sizeof(Complex),nlen + 1 ) ;
  dmat->rhs_c = (Complex *)Calloc( sizeof(Complex),ndim + 1 ) ;

  for(i = 0; i < ndim + 1; i++){
    dmat->idx1[i] = dmat_orig.idx1[i] ;
    dmat->idx2[i] = dmat_orig.idx2[i] ;
  }

  for(i = 0; i < ndim; i++)
    dmat->rhs_c[i] = dmat_orig.rhs_c[i] ;
  
  //printf("ndim = %d, nlen = %d\n", ndim, nlen);

  for(i = 0; i < nlen; i++)
    {
      //printf("mat_c[i]=%lf\n",dmat_orig.mat_c[i].re);
	dmat->mat_c[i] = dmat_orig.mat_c[i] ;
    }

}

/************************************************************************/

void skyline_copy_1(DMatrix  dmat_temp,  DMatrix  *dmat )
{

  int  i ;
  int ndim = dmat_temp.ndim ;
  int  nlen = dmat_temp.nlen ;

  dmat->ndim = ndim ;
  dmat->nlen = nlen ;

  dmat->idx1 = (int *)Calloc( sizeof(int),ndim + 2 ) ;
  dmat->idx2 = (int *)Calloc( sizeof(int),ndim + 2 ) ;
  dmat->mat_c = (Complex *)Calloc( sizeof(Complex),nlen + 1 ) ;
  dmat->rhs_c = (Complex *)Calloc( sizeof(Complex),ndim + 1 ) ;

  for(i=0;i<ndim+1;i++){
    dmat->idx1[i] = dmat_temp.idx1[i] ;
    dmat->idx2[i] = dmat_temp.idx2[i] ;
  }
  for(i=0;i<ndim;i++){
    dmat->rhs_c[i] = dmat_temp.rhs_c[i] ;
  }
  for(i=0;i<nlen;i++){
    dmat->mat_c[i] = dmat_temp.mat_c[i] ;
  }

}

/*************************************************************************/

void change_aij_2_skyline_index(TMP_AIJ tmp_aij, DMatrix *dmat )
{

  int  i, j ;
  int  nf = tmp_aij.nf ;
  int*   ai = tmp_aij.tmp_ai ;
  int**  taij = tmp_aij.tmp_aij ;
  Complex** tvij = tmp_aij.tmp_vij_c ;
  Complex* b = tmp_aij.tmp_b_c ;


  dmat->ndim = nf ;

  dmat->idx1 = (int *)Calloc( sizeof(int),dmat->ndim + 2 ) ;
  dmat->idx2 = (int *)Calloc( sizeof(int),dmat->ndim + 2 ) ;
  for( i=0 ; i<dmat->ndim ; i++ ) {
    dmat->idx1[i] = taij[i][0] ;
  }

  dmat->idx2[0] = 0 ;
  for (i = 1; i < dmat->ndim; i++){
    dmat->idx2[i] = dmat->idx2[i-1] + i - dmat->idx1[i] ;
  }
  dmat->nlen = dmat->idx2[dmat->ndim-1] + dmat->ndim;

  dmat->mat_c = (Complex *)Calloc( sizeof(Complex),dmat->nlen + 1 ) ;
  for (i = 0; i < dmat->ndim; i++){
    for (j = 0; j<ai[i]; j++){
      dmat->mat_c[dmat->idx2[i]+taij[i][j]] = tvij[i][j] ;
    }
  }

  dmat->rhs_c =  (Complex *)Calloc( sizeof(Complex), dmat->ndim + 1 ) ;
  for(i=0;i<dmat->ndim;i++){
    dmat->rhs_c[i] = b[i] ;
  }
}
