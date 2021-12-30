/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/


int __MYMT_AIJ_PC_NONE_MAKE__( AIJ_MAT *aij_mat, SolvOpt solvopt,
                               FILE *monitor )
{
  /*+ Function: make preconditioner matrix (NONE) for AIJ +*/
  aij_mat->pc_size = 0 ;
  aij_mat->AIJ_PC = NULL ;


  return 0 ;
}

/********************************************************************/

int __MYMT_AIJ_PC_NONE_SET__( AIJ_MAT *aij_mat, MTYPE *Mr, MTYPE *r,
                              SolvOpt solvopt, FILE *monitor )
{
  /*+ Function: set preconditioner matrix (NONE) for AIJ +*/
  if( Mr != r ) {
    memcpy( Mr, r, sizeof(MTYPE)*aij_mat->nf ) ;
  }


  return 0 ;
}

/********************************************************************/

#ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ /* symmetric */
int __MYMT_AIJ_PC_ICC_MAKE__( AIJ_MAT *aij_mat, SolvOpt solvopt,
                              FILE *monitor )
{
  /*+ Function: make preconditioner matrix (ICC) for AIJ +*/
  int    i ;

  int    col ;
  int    row ;

  int    nf = aij_mat->nf ;
  int*   ai = aij_mat->ai ;
  int*   aij = aij_mat->aij ;
  MTYPE* vij = aij_mat->AIJ_VIJ ;

  double af = solvopt.solver_ic_shift ;
  double error_af = 1.05 ;
  int    pc_size = ai[nf] + nf ;
  MTYPE  **pM ;
  MTYPE  *M ;
  MTYPE  *L ;
  MTYPE  *d ;


  pM = &(aij_mat->AIJ_PC) ;
  *pM = (MTYPE *)Malloc( sizeof(MTYPE), pc_size ) ;
  M = *pM ;
  L = M ;
  d = M + ai[nf] ;


  if( (af > 3.0) || (af < 1.0) ) {
    if( solvopt.solver_log == NEED ) {
      Fprintf( monitor,
      "Accelararion factor (af) is NOT collect. So program set af %g\n",
      error_af ) ;
    }
    af = error_af ;
  }


  for( col=0 ; col<nf ; col++ ) {
    int    ii = ai[col] ;
    int    jj = ai[col+1] - 1 ;
    MTYPE  dd ;
    for( row=ii ; row<jj ; row++ ) {
      dd = M_ZERO ;
      for( i=ii ; i<row ; i++ ) {
        dd = MPLUS( dd,
                    MMULTI(d[aij[i]],
                           MMULTI(MGETAIJ(col, aij[i], ai, aij, L),
                                  MGETAIJ(aij[row], aij[i], ai, aij, L))) ) ;
      }
      L[row] = MMINUS( vij[row], dd ) ;
    }


    dd = M_ZERO ;
    for( i=ii ; i<jj ; i++ ) {
      MTYPE  ddd = MGETAIJ( col, aij[i], ai, aij, L ) ;
      dd = MPLUS( dd, MMULTI(d[aij[i]], MMULTI(ddd, ddd)) ) ;
    }
    L[jj] = MMINUS( MMULTIS(vij[jj], af), dd ) ;


    d[col] = MDIVIDE( M_ONE, L[jj] ) ;
  }


  aij_mat->pc_size = pc_size ;


  return 0 ;
}

/********************************************************************/

int __MYMT_AIJ_PC_ICC_SET__( AIJ_MAT *aij_mat, MTYPE *Mr, MTYPE *r,
                             SolvOpt solvopt, FILE *monitor )
{
  /*+ Function: set preconditioner matrix (ICC) for AIJ +*/
  int    col ;
  int    row ;

  int    nf = aij_mat->nf ;
  int*   ai = aij_mat->ai ;
  int*   aij = aij_mat->aij ;
  MTYPE* L = aij_mat->AIJ_PC ;
  MTYPE* d = aij_mat->AIJ_PC + ai[nf] ;
  MTYPE* tmp ;
  MTYPE* x = Mr ;
  MTYPE* b = r ;


  if( x == b ) {
    tmp = (MTYPE *)Malloc( sizeof(MTYPE), nf ) ;
  } else {
    tmp = x ;
  }


  /* Ly = r ( Forward elimitanion like calculation ) */
  for( col=0 ; col<nf ; col++ ) {
    int    ii = ai[col] ;
    int    jj = ai[col+1] - 1 ;
    MTYPE  dd = M_ZERO ;
    for( row=ii ; row<jj ; row++ ) {
      dd = MPLUS( dd, MMULTI(tmp[aij[row]], L[row]) ) ;
    }
    tmp[col] = MDIVIDE( MMINUS(b[col], dd), L[jj] ) ;
  }


  /* DLtv = y ( Back elimination like calculation ) */
  for( col=nf-1 ; col>=0 ; col-- ) {
    int    ii = ai[col] ;
    int    jj = ai[col+1] - 2 ;
    for( row=jj ; row>=ii ; row-- ) {
      tmp[aij[row]] = MMINUS( tmp[aij[row]],
                              MMULTI(tmp[col],
                                     MMULTI(L[row], d[aij[row]])) ) ;
    }
  }


  if( x == b ) {
    memcpy( x, tmp, sizeof(MTYPE)*nf ) ;
    Free( sizeof(MTYPE)*nf, tmp ) ;
  }


  return 0 ;
}
#endif /* __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ */

/********************************************************************/

#ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ /* symmetric */
int __MYMT_AIJ_PC_MRIC0_MAKE__( AIJ_MAT *aij_mat, SolvOpt solvopt,
                                FILE *monitor )
{
  /*+ Function: make preconditioner matrix (ICC) for AIJ +*/
  /*int    i ;

  int    col ;
  int    row ;

  int    nf = aij_mat->nf ;
  int*   ai = aij_mat->ai ;
  int*   aij = aij_mat->aij ;
  MTYPE* vij = aij_mat->AIJ_VIJ ;

  double wd = 1.0 ;
  double error_wd = 1.0 ;
  int    pc_size = ai[nf] ;
  MTYPE  **pM ;
  MTYPE  *M ;
  MTYPE  *U ;
  MTYPE  *d ;
  MTYPE  *v ;


  pM = &(aij_mat->AIJ_PC) ;
  *pM = (MTYPE *)Malloc( sizeof(MTYPE), pc_size ) ;
  M = *pM ;
  U = M ;
  d = (MTYPE *)Malloc( sizeof(MTYPE), nf ) ;
  v = (MTYPE *)Malloc( sizeof(MTYPE), nf ) ;


  if( (wd > 1.0) || (wd < -1.0) ) {
    if( solvopt.solver_log == NEED ) {
      Fprintf( monitor,
      "Accelararion factor (af) is NOT collect. So program set af %g\n",
      error_wd ) ;
    }
    wd = error_wd ;
  }


  for( col=0 ; col<nf ; ) {
    d[col] = vij[ai[++col]-1] ;
  }
  for( col=0 ; col<nf ; col++ ) {
    int    ii = ai[col] ;
    int    jj = ai[col+1] - 1 ;
    MTYPE  dd ;
    for( row=col+1 ; row<nf ; row++ ) {
      v[row] = MGETAIJ( col, row, ai, aij, vij ) ;
    }
    for( row=0 ; row<col ; row++ ) {
      for( i=col+1 ; i<nf ; i++ ) {
        v[i] = MMINUS( v[i], MMULTI(MGETAIJ(row, col, ai, aij, U),
                                    MGETAIJ(row, i, ai, aij, U)) ) ;
      }
    }
    for( row=col+1 ; row<nf ; row++ ) {
      double e = MABS( v[row] ) / MABS( MSQRT(MMULTI(d[col], d[row])) ) ;
      if( ISZERO(MGETAIJ(col, row, ai, aij, vij)) ) {
        v[row] = M_ZERO ;
      } else {
        d[col] = MMULTIS( d[col], (1.0 + wd*e) ) ;
        d[row] = MMULTIS( d[row], (1.0 + wd*e) ) ;
      }
    }
    U[jj] = MSQRT( d[col] ) ;
    for( row=col+1 ; row<nf ; row++ ) {
      if( !ISZERO(MGETAIJ(col, row, ai, aij, vij)) ) {
        for( i=ai[row] ; i<ai[row+1] ; i++ ) {
          if( col == aij[i] ) {
            U[i] = MDIVIDE( v[row], U[jj] ) ;
            d[row] = MMINUS( d[row], MMULTI(U[i], U[i]) ) ;
          }
        }
      }
    }
  }


  aij_mat->pc_size = pc_size ;


  Free( sizeof(MTYPE)*nf, d ) ;
  Free( sizeof(MTYPE)*nf, v ) ;*/


  return 0 ;
}

/********************************************************************/

int __MYMT_AIJ_PC_MRIC0_SET__( AIJ_MAT *aij_mat, MTYPE *Mr, MTYPE *r,
                               SolvOpt solvopt, FILE *monitor )
{
  /*+ Function: set preconditioner matrix (ICC) for AIJ +*/
  /*int    col ;
  int    row ;

  int    nf = aij_mat->nf ;
  int*   ai = aij_mat->ai ;
  int*   aij = aij_mat->aij ;
  MTYPE* U = aij_mat->AIJ_PC ;
  MTYPE* tmp ;
  MTYPE* x = Mr ;
  MTYPE* b = r ;


  if( x == b ) {
    tmp = (MTYPE *)Malloc( sizeof(MTYPE), nf ) ;
  } else {
    tmp = x ;
  }*/


  /* Uty = r ( Forward elimitanion like calculation ) */
  /*for( col=0 ; col<nf ; col++ ) {
    int    ii = ai[col] ;
    int    jj = ai[col+1] - 1 ;
    MTYPE  dd = M_ZERO ;
    for( row=ii ; row<jj ; row++ ) {
      dd = MPLUS( dd, MMULTI(tmp[aij[row]], U[row]) ) ;
    }
    tmp[col] = MDIVIDE( MMINUS(b[col], dd), U[jj] ) ;
  }*/


  /* Uv = y ( Back elimination like calculation ) */
  /*for( col=nf-1 ; col>=0 ; col-- ) {
    int    ii = ai[col] ;
    int    jj = ai[col+1] - 2 ;
    for( row=jj ; row>=ii ; row-- ) {
      tmp[aij[row]] = MMINUS( tmp[aij[row]], MMULTI(tmp[col], U[row]) ) ;
    }
  }


  if( x == b ) {
    memcpy( x, tmp, sizeof(MTYPE)*nf ) ;
    Free( sizeof(MTYPE)*nf, tmp ) ;
  }*/


  return 0 ;
}
#endif /* __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ */
