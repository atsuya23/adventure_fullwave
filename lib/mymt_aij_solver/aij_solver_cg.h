/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/



int __MYMT_AIJ_SOLVER_CGM__( AIJ_MAT *aij_mat, SolvOpt solvopt,
                             FILE *monitor )
{
  /*+ Function: switch preconditioner for CG method
                or SOCG method for symmetric matrix for AIJ +*/
  int    retVal ;

  switch( solvopt.solver_pc ) {
    case SOLVER_PC_NONE :
      __MYMT_AIJ_PC_NONE_MAKE__( aij_mat, solvopt, monitor ) ;
      retVal = __MYMT_AIJ_SOLVER_CGM_FUNC__( aij_mat, solvopt,
                                             __MYMT_AIJ_PC_NONE_SET__,
                                             monitor ) ;     
      printf("COCG SOLVER_PC_NONE\n");
      break ;
    case SOLVER_PC_ICC :
      __MYMT_AIJ_PC_ICC_MAKE__( aij_mat, solvopt, monitor ) ;
      retVal = __MYMT_AIJ_SOLVER_CGM_FUNC__( aij_mat, solvopt,
                                             __MYMT_AIJ_PC_ICC_SET__,
                                             monitor ) ;
      //printf("COCG SOLVER_PC_ICC\n");
      break ;
    case SOLVER_PC_MRIC0 :
      __MYMT_AIJ_PC_MRIC0_MAKE__( aij_mat, solvopt, monitor ) ;
      retVal = __MYMT_AIJ_SOLVER_CGM_FUNC__( aij_mat, solvopt,
                                             __MYMT_AIJ_PC_MRIC0_SET__,
                                             monitor ) ;
      break ;
    default :
	retVal = SOLVER_STATUS_UNKNOWN_PC ;
  }

  //printf("retVal_CG=%d\n",retVal);
  return retVal ;

}

/********************************************************************/

int __MYMT_DMAT_SOLVER_LUM__( AIJ_MAT *aij_mat, SolvOpt solvopt,
			      FILE *monitor )
{
  /*+ Function: switch preconditioner for CG method
                or SOCG method for symmetric matrix for AIJ +*/
  int    retVal ;
      printf("Check1\n");
		     
      retVal = __MYMT_DMAT_SOLVER_LUM_FUNC__( aij_mat, solvopt, monitor) ;
      
      printf("LU SOLVER_PC_NONE\n");

  printf("retVal_LU=%d\n",retVal);

  return retVal ;

}

/********************************************************************/

int __MYMT_AIJ_SOLVER_CGM_NMPCM__( AIJ_MAT *aij_mat, SolvOpt solvopt,
                                   FILE *monitor )
{
  /*+ Function: switch preconditioner for CG method
                or SOCG method for symmetric matrix for AIJ
                but not making preconditioner matrix  +*/
  int    retVal ;

  switch( solvopt.solver_pc ) {
    case SOLVER_PC_NONE :
      retVal = __MYMT_AIJ_SOLVER_CGM_FUNC__( aij_mat, solvopt,
                                             __MYMT_AIJ_PC_NONE_SET__,
                                             monitor ) ;
      break ;
    case SOLVER_PC_ICC :
      retVal = __MYMT_AIJ_SOLVER_CGM_FUNC__( aij_mat, solvopt,
                                             __MYMT_AIJ_PC_ICC_SET__,
                                             monitor ) ;
      break ;
    default :
	retVal = SOLVER_STATUS_UNKNOWN_PC ;
  }

  return retVal ;
}

/********************************************************************/

int __MYMT_DMAT_SOLVER_LU_NMPCM__( AIJ_MAT *aij_mat, SolvOpt solvopt,
                             FILE *monitor )
{
  /*+ Function: switch preconditioner for CG method
                or SOCG method for symmetric matrix for AIJ +*/
  int    retVal ;			     

    printf("Check1.1\n");		
    retVal = __MYMT_DMAT_SOLVER_LUM_FUNC__( aij_mat, solvopt, monitor) ;
      /* __MYMT_AIJ_PC_NONE_SET__ */

  return retVal ;

}

/********************************************************************/

int __MYMT_AIJ_SOLVER_CGM_FUNC__( AIJ_MAT *aij_mat, SolvOpt solvopt,
                                  int (*setPC)( AIJ_MAT *aij_mat,
                                                MTYPE *Mr, MTYPE *r,
                                                SolvOpt solvopt,
                                                FILE *monitor ),
                                  FILE *monitor )
{
  /*+ Function: CG method for Hermitian matrix for AIJ
             or COCG method for complex symmetric matrix for AIJ +*/

  /** Algorithm **/
  /*
    x(0) is an initial guess,
    r(0) = b - Ax(0),
    if ||r(0)|| / ||b|| <= eps -> end ;
    v(0) = M^-1 r(0),
    q(0) = v(0) / (v(0),r(0)),
    for n = 0, 1, ...
      aq(n) = Aq(n),
      alpha(n) = 1 / (q(n),aq(n)),
      x(n+1) = x(n) + alpha(n) q(n),
      r(n+1) = r(n) - alpha(n) aq(n),
      if ||r(n+1)|| / ||b|| <= eps -> end ;
      v(n+1) = M^-1 r(n+1),
      q(n+1) = q(n) + v(n+1) / (v(n+1),r(n+1)),
    end ;
  */



  int    i ;

  int    n ;

  int    nf = aij_mat->nf ;
  int*   ai = aij_mat->ai ;
  int*   aij = aij_mat->aij ;
  MTYPE* vij = aij_mat->AIJ_VIJ ;
  MTYPE* b = aij_mat->AIJ_B ;
  MTYPE* x = aij_mat->AIJ_X ;

  MTYPE  *r ;
  MTYPE  *v ;
  MTYPE  *q ;
  MTYPE  *aq ;
  MTYPE  alpha ;
  double b_norm ;
  double r_norm ;
  MTYPE  rv ;
  double residual ;

  int    max_ite = nf * 10 ;
  double eps = solvopt.solver_eps ;
  double div = solvopt.solver_div ;
  int    retVal ;


#ifdef __ADVFW_OS_UNIX__
  tval start_time ;
  gettimeofday( &start_time, NULL ) ;
#else
  time_t start_time ;
  time( &start_time ) ;
#endif


  //  int col;
  //  Complex *aaa;
  //    aaa=(Complex *)b;
  //  for(col=0;col<nf;col++){
  //    printf("b_cg[%d] =(%e,%e)\n",col,aaa[col].re,aaa[col].im);
  //  }


  retVal = SOLVER_STATUS_UNKNOWN_ERROR ;


  r = (MTYPE *)Malloc( sizeof(MTYPE), nf ) ;
  v = (MTYPE *)Malloc( sizeof(MTYPE), nf ) ;
  aq = (MTYPE *)Malloc( sizeof(MTYPE), nf ) ;
  q = (MTYPE *)Malloc( sizeof(MTYPE), nf ) ;


  if( solvopt.solver_log == NEED ) {
    #ifdef __MYMT_AIJ_HERMITIAN__
      Fprintf( monitor, "Conjugate Gradient method\n" ) ;
    #else
      Fprintf( monitor,
               "Conjugate Orthogonal Conjugate Gradient method\n" ) ;
    #endif /* __MYMT_AIJ_HERMITIAN__ */
    Fprintf( monitor, "  date: %s", cTime());
    Fprintf( monitor, "  --status--\n" ) ;
    Fprintf( monitor, "    Matrix type\n" ) ;
    #ifdef __ADVFW_MATRIX_SYSTEM_REAL__
      Fprintf( monitor, "      REAL\n" ) ;
    #endif /* __ADVFW_MATRIX_SYSTEM_REAL__ */
    #ifdef __ADVFW_MATRIX_SYSTEM_COMPLEX__
      Fprintf( monitor, "      COMPLEX\n" ) ;
    #endif /* __ADVFW_MATRIX_SYSTEM_COMPLEX__ */
    #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__
      Fprintf( monitor, "      SYMMETRIC\n" ) ;
    #endif /* __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ */
    #ifdef __ADVFW_MATRIX_SYSTEM_ASYMMETRIC__
      Fprintf( monitor, "      ASYMMETRIC\n" ) ;
    #endif /* __ADVFW_MATRIX_SYSTEM_ASYMMETRIC__ */
    Fprintf( monitor, "    %-20s%d\n", "Degrees of Freedom", nf ) ;
    Fprintf( monitor, "    %-20s%le\n", "Epsiron", solvopt.solver_eps ) ;
    Fprintf( monitor, "    %-20s%le\n", "Divergence", solvopt.solver_div ) ;
    Fprintf( monitor, "    %-20s%d\n", "Max iteration", max_ite ) ;
    if( solvopt.solver_pc == SOLVER_PC_NONE ) {
      Fprintf( monitor, "    Using no preconditioner\n" ) ;
    } else if( solvopt.solver_pc == SOLVER_PC_DIAG ) {
      Fprintf( monitor, "    Using diagonal scaling as preconditioner\n" ) ;
    } else if( solvopt.solver_pc == SOLVER_PC_JACOBI ) {
      Fprintf( monitor, "    Using jacobi as preconditioner\n" ) ;
    } else if( solvopt.solver_pc == SOLVER_PC_ICC ) {
      Fprintf( monitor,
      "    Using incomplete Cholesky facorisation as preconditioner\n" ) ;
      Fprintf( monitor, "        acceleration factor %lf\n",
               solvopt.solver_ic_shift ) ;
    } else if( solvopt.solver_pc == SOLVER_PC_ILDU ) {
      Fprintf( monitor, "    Using ILDU as preconditioner\n" ) ;
    } else if( solvopt.solver_pc == SOLVER_PC_SOR ) {
      Fprintf( monitor, "    Using SOR as preconditioner\n" ) ;
    }
    Fprintf( monitor, "Start solver\n" ) ;
  }
  /*
  for( i=0 ; i<nf ; i++ ) {
    printf("ai[%d]=%d\n",i,ai[i]) ;
  }
  */
  /* r(0) = b - Ax(0) */
  MAIJMXV( nf, ai, aij, vij, x, aq ) ;
  for( i=0 ; i<nf ; i++ ) {
    r[i] = MMINUS( b[i], aq[i] ) ;
  }
  /* if ||b(0)|| = 0 -> end */
  b_norm = sqrt( MABS( MINNER(nf, b, b) ) ) ;
  if( b_norm < NEARLY_ZERO ) {
    if( solvopt.solver_log == NEED ) {
      Fprintf( monitor, "\n" ) ;
      Fprintf( monitor, "vector b is zero!\n" ) ;
    }
    retVal = 0 ;
    goto END ;
  }
  /* if ||r(0)|| / ||b|| <= eps -> end */
  b_norm = 1.0 / b_norm ;
  r_norm = sqrt( MABS( MINNER(nf, r, r) ) ) ;
  residual = r_norm * b_norm ;
  if( residual <= eps ) {
    if( solvopt.solver_log == NEED ) {
      Fprintf( monitor, "\n" ) ;
      Fprintf( monitor,
               "Initial guess answer is equal to real answer!\n" ) ;
      fprintf( monitor, "%d  %le\n", 0, residual ) ;
      fflush( monitor ) ;
    }
    retVal = 0 ;
    goto END ;
  } else {
    if( solvopt.solver_log == NEED ) {
      fprintf( monitor, "%d  %le\n", 0, residual ) ;
      fflush( monitor ) ;
    }
  }
  /* v(0) = M^-1 r(0) */
  setPC( aij_mat, v, r, solvopt, monitor ) ;
  /* q(0) = v(0) / (v(0),r(0)) */
  rv = MRECIP( __LOCAL_INNER__(nf, v, r) ) ;
  for( i=0 ; i<nf ; i++ ) {
    q[i] = MMULTI( v[i], rv ) ;
  }


  for( n=1 ; n<max_ite ; n++ ) {
    /* aq(n) = Aq(n) */
    MAIJMXV( nf, ai, aij, vij, q, aq ) ;
    /* alpha(n) = 1 / (q(n),aq(n)) */
    alpha = __LOCAL_INNER__( nf, q, aq ) ;
    if( ISZERO(alpha) ) {
      fprintf( stderr, "alpha is zero!\n" ) ;
      Exit( __ABORT_MYMT_AIJ_SOLVER__ ) ;
    }
    alpha = MDIVIDE( M_ONE, alpha ) ;
    /* x(n+1) = x(n) + alpha(n) q(n) */
    for( i=0 ; i<nf ; i++ ) {
      x[i] = MPLUS( x[i], MMULTI(alpha, q[i]) ) ;
    }
    /* r(n+1) = r(n) - alpha(n) aq(n) */
    alpha = MMINUS( M_ZERO, alpha ) ;
    for( i=0 ; i<nf ; i++ ) {
      r[i] = MPLUS( r[i], MMULTI(alpha, aq[i]) ) ;
    }
    /* if ||r(n+1)|| / ||b|| < eps -> end */
    r_norm = sqrt( MABS( MINNER(nf, r, r) ) ) ;
    residual = r_norm * b_norm ;
    if( solvopt.solver_log == NEED ) {
      fprintf( monitor, "%d  %le\n", n, residual ) ;
      fflush( monitor ) ;
    }
    if( residual <= eps ) {
      retVal = n ;
      goto END ;
    } else if( residual >= div ) {
      Fprintf( monitor, "Divergent!\n" ) ;
      retVal = SOLVER_STATUS_DIVERGED ;
      goto END ;
    }
    /* v(n+1) = M^-1 r(n+1) */
    setPC( aij_mat, v, r, solvopt, monitor ) ;
    /* q(n+1) = q(n) + v(n+1) / (v(n+1),r(n+1)) */
    rv = MRECIP( __LOCAL_INNER__(nf, v, r) ) ;
    for( i=0 ; i<nf ; i++ ) {
      q[i] = MPLUS( q[i], MMULTI(v[i], rv) ) ;
    }
  }

  if( n == max_ite ) {
    retVal = SOLVER_STATUS_REACHED_MAX_ITE ;
  }

 END:
  Free( sizeof(MTYPE)*nf, r ) ;
  Free( sizeof(MTYPE)*nf, v ) ;
  Free( sizeof(MTYPE)*nf, aq ) ;
  Free( sizeof(MTYPE)*nf, q ) ;


  if( solvopt.solver_log == NEED ) {
    #ifdef __MYMT_AIJ_HERMITIAN__
      Fprintf( monitor, "End CG method\n" ) ;
    #else
      Fprintf( monitor, "End COCG method\n" ) ;
    #endif /* __MYMT_AIJ_HERMITIAN__ */
    Fprintf( monitor, "All time   %g[s]\n", passTime(start_time) ) ;
    fflush( monitor ) ;
  }


  return retVal ;
}


/********************************************************************/

int __MYMT_DMAT_SOLVER_LUM_FUNC__( AIJ_MAT *aij_mat, SolvOpt solvopt,
					      FILE *monitor )
{
  /*+ Function: LU Direct method for Hermitian matrix : DMatrix
             or LU Direct method for complex symmetric matrix : DMatrix +*/

  int    nf = aij_mat->nf ;
  //int*   ai = aij_mat->ai ;
  //int*   aij = aij_mat->aij ;

  MTYPE* vij = aij_mat->AIJ_VIJ ;
  MTYPE* b = aij_mat->AIJ_B ;

  int    retVal ;

  /******** ********/
  int i, j ;
  int ndim, nlen ;
  int *idx1, *idx2 ;

  MTYPE *mat ;
  
  int*   NzIdx_ia = aij_mat->ai ;
  int*   NzIdx_jcn = aij_mat->aij ;

  int iDof, jDof, kDof, iMax ;
  MTYPE *work, *SMatDiag, r ;
  /******** ********/

  //Complex* v_tmp ;

#ifdef __ADVFW_OS_UNIX__
  tval start_time ;
  gettimeofday( &start_time, NULL ) ;
#else
  time_t start_time ;
  time( &start_time ) ;
#endif


  retVal = SOLVER_STATUS_UNKNOWN_ERROR ;
  

  if( solvopt.solver_log == NEED ) {
    #ifdef __MYMT_AIJ_HERMITIAN__
      Fprintf( monitor, 
	       "LU Direct method for Hermitian matrix\n" ) ;
    #else
      Fprintf( monitor,
               "LU Direct method for Complex matrix\n" ) ;
    #endif /* __MYMT_AIJ_HERMITIAN__ */
    Fprintf( monitor, "  date: %s", cTime());
    Fprintf( monitor, "  --status--\n" ) ;
    Fprintf( monitor, "    Matrix type\n" ) ;
    #ifdef __ADVFW_MATRIX_SYSTEM_REAL__
      Fprintf( monitor, "      REAL\n" ) ;
    #endif /* __ADVFW_MATRIX_SYSTEM_REAL__ */
    #ifdef __ADVFW_MATRIX_SYSTEM_COMPLEX__
      Fprintf( monitor, "      COMPLEX\n" ) ;
    #endif /* __ADVFW_MATRIX_SYSTEM_COMPLEX__ */
    #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__
      Fprintf( monitor, "      SYMMETRIC\n" ) ;
    #endif /* __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ */
    #ifdef __ADVFW_MATRIX_SYSTEM_ASYMMETRIC__
      Fprintf( monitor, "      ASYMMETRIC\n" ) ;
    #endif /* __ADVFW_MATRIX_SYSTEM_ASYMMETRIC__ */
    Fprintf( monitor, "    %-20s%d\n", "Degrees of Freedom", nf ) ;
 
    Fprintf( monitor, "Start solver\n" ) ;
  }
  
  /* Preparation */

  ndim = nf ;

  printf("ndim=%d\n",nf);

  /* Convert matrix data */
  
  idx1 = (int *)Malloc( sizeof(int), ndim + 1 ) ;
  idx2 = (int *)Malloc( sizeof(int), ndim + 1 ) ;

  printf("check2\n");

  //advlas_copy_nzindex_to_skyindex( ai, aij, &dmat ); /* 1st */

  /* copy_nzmat_to_skyindex  */
  
  for (i = 0; i < ndim ; i++)
    idx1[i] = NzIdx_jcn[NzIdx_ia[i]] ;

  idx2[0] = 0;

  for (i = 1; i < ndim ; i++)
    idx2[i] = idx2[i-1] + i - idx1[i] ;
  
  nlen = idx2[ ndim - 1 ] + ndim ;

  /* */
  /* copy_nzmat_to_skymat */ 

  mat = (MTYPE *)Malloc( sizeof(MTYPE), ndim*ndim ) ;

  for (i = 0; i < ndim; i++)
    for (j = NzIdx_ia[i] ; j < NzIdx_ia[i+1]; j++)
      mat[idx2[i] + NzIdx_jcn[j]] = vij[j] ;
  
  printf("check3cg\n") ;
  
  /* */
  /* Do LDLt factorization:advlas_skyline_ldl_decomposite */
  
  work = (MTYPE *)Malloc( sizeof(MTYPE), ndim*ndim ) ;

  SMatDiag = work ;

  SMatDiag[0] = MRECIP(mat[0]) ;
  for (iDof = 1; iDof < ndim; iDof++)
    SMatDiag[iDof] = mat[idx2[iDof] + iDof] ;

  for (iDof = 1; iDof < ndim; iDof++) {
    int ifst = idx2[iDof] ;
    for (jDof = idx1[iDof]; jDof < iDof; jDof++) {
      int jfst = idx2[jDof] ;
      if (idx1[iDof] > idx1[jDof])
	iMax = idx1[iDof];
      else
	iMax = idx1[jDof];
      r = M_ZERO ;
      for (kDof = iMax; kDof < jDof; kDof++)
	r = MPLUS( r , MMULTI(mat[ifst+kDof],mat[jfst+kDof]) ) ;
      mat[ifst+jDof] = MMINUS(mat[ifst+jDof],r);
    }
    r = M_ZERO ;    
    for (kDof = idx1[iDof]; kDof < iDof; kDof++) {
     r = MPLUS(r , MMULTI(mat[ifst+kDof],
			   MMULTI(mat[ifst+kDof],SMatDiag[kDof])
					)
			);	
      mat[ifst+kDof] = MMULTI(mat[ifst + kDof], SMatDiag[kDof]);
    }
    SMatDiag[iDof] = MRECIP(MMINUS(SMatDiag[iDof],r));
  }

  for (iDof = 0; iDof < ndim; iDof++)
    mat[idx1[iDof] + iDof] = SMatDiag[iDof];

  /* */
  /* Make solution */

  for (iDof = 1; iDof < ndim; iDof++) {
    int ii = idx2[iDof];
    MTYPE rr = M_ZERO ;
    for (jDof = idx1[iDof]; jDof < iDof; jDof++)
      rr = MPLUS( rr , MMULTI(mat[ii+jDof],b[jDof]) );
    b[iDof] = MMINUS(b[iDof],rr);
  }

  for (iDof = 0; iDof < ndim; iDof++) {
    b[iDof] = MMULTI(b[iDof],mat[idx2[iDof]+iDof]);
  }

  for (iDof = ndim - 1; iDof >= 1; iDof--) {
    int ii = idx2[iDof];
    MTYPE rr = b[iDof];
    for (jDof = idx1[iDof]; jDof < iDof; jDof++) {
      b[jDof] = MMINUS(b[jDof],MMULTI(mat[ii+jDof],rr ));
    }
  }
  
  aij_mat->AIJ_X = b ;

  printf("ndim=%d, nlen=%d\n",ndim, nlen) ;


  printf("check7\n") ;
  Free( sizeof(int)*(ndim+1), idx1) ;
  Free( sizeof(int)*(ndim+1), idx2) ;
  Free( sizeof(MTYPE)*(ndim*ndim), mat) ;
  Free( sizeof(MTYPE)*(ndim*ndim), work) ;

  if( solvopt.solver_log == NEED ) {
    #ifdef __MYMT_AIJ_HERMITIAN__
      Fprintf( monitor, "End LU method\n" ) ;
    #else
      Fprintf( monitor, "End LU method\n" ) ;
    #endif /* __MYMT_AIJ_HERMITIAN__ */
    Fprintf( monitor, "All time   %g[s]\n", passTime(start_time) ) ;
    fflush( monitor ) ;
  }

    printf("check8\n") ;
    return retVal = 0 ;
}
