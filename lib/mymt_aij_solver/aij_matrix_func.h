/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/


void __MYMT_AIJ_INIT__( int nf, TMP_AIJ *tmp_aij, int nMargin )
{
  /*+ Function: initialize struct TMP_AIJ +*/
  int    col ;
  int    row ;


  tmp_aij->nf = nf ;
  tmp_aij->tmp_ai = (int *)Calloc( sizeof(int), nf+1 ) ;
  tmp_aij->tmp_aij = (void *)Malloc( sizeof(int*), nf ) ;
  tmp_aij->TMP_VIJ = (void *)Malloc( sizeof(MTYPE*), nf ) ;
  tmp_aij->TMP_B = (MTYPE *)Calloc( sizeof(MTYPE), nf ) ;


  if( nMargin <= 0 ) {
    nMargin = 0 ;
  }
  tmp_aij->nSize = (int *)Malloc( sizeof(int), nf ) ;
  for( col=0 ; col<nf ; col++ ) {
    tmp_aij->tmp_aij[col] = (int *)Malloc( sizeof(int), nMargin ) ;
    for( row=0 ; row<nMargin ; row++ ) {
      tmp_aij->tmp_aij[col][row] = -1 ;
    }
    tmp_aij->TMP_VIJ[col] = (MTYPE *)Malloc( sizeof(MTYPE), nMargin ) ;
    tmp_aij->nSize[col] = nMargin ;
  }
  if( nMargin == 0 ) {
    tmp_aij->nMargin = 1 ;
  } else {
    tmp_aij->nMargin = nMargin ;
  }


  tmp_aij->TMP_X = NULL ;
  tmp_aij->tmp_pc_size = 0 ;
  tmp_aij->TMP_PC = NULL ;
}

/********************************************************************/

void __MYMT_AIJ_SET__( TMP_AIJ *tmp_aij,
                       int szRow, int *indRow,
                       int szCol, int *indCol,
                       MTYPE *ae, MTYPE *fe )
{
  /*+ Function: set Ae & fe to AIJ +*/
  __MYMT_AIJ_SETAE__( tmp_aij, szRow, indRow, szCol, indCol, ae ) ;
  __MYMT_AIJ_SETFE__( tmp_aij->TMP_B, szCol, indCol, fe ) ;
}

/********************************************************************/

void __MYMT_AIJ_SETBC__( TMP_AIJ *tmp_aij,
                         int nbc, int *indbc,
                         MTYPE *val )
{
  /*+ Function: set B.C. to AIJ +*/
  int    bc ;
  int    col ;
  int    row ;
  int    nf = tmp_aij->nf ;
  int*   ai = tmp_aij->tmp_ai ;
  int**  taij = tmp_aij->tmp_aij ;
  MTYPE** tvij = tmp_aij->TMP_VIJ ;
  MTYPE* b = tmp_aij->TMP_B ;
  int*   nSize = tmp_aij->nSize ;



  MSORT_IM( nbc, indbc, val, 1 ) ;
  for( col=0 ; col<nf ; col++ ) {
    MSORT_IM( ai[col], taij[col], tvij[col], 1 ) ;
  }


  for( bc=0 ; bc<nbc ; bc++ ) {
    int    icol = indbc[bc] ;
    int    size = nSize[icol] ;
    #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ /* symmetric */
      int    nrow = ai[icol] ;
      MTYPE  vbc = val[bc] ;
      int*   aaij = taij[icol] ;
      MTYPE* vvij = tvij[icol] ;
      for( row=0 ; row<nrow ; row++ ) {
        col = aaij[row] ;
        b[col] = MMINUS( b[col], MMULTI( vvij[row], vbc ) ) ;
      }
    #endif /*__ADVFW_MATRIX_SYSTEM_SYMMETRIC__ */


    tvij[icol] = (MTYPE *)Realloc( tvij[icol], sizeof(MTYPE), size, 1 ) ;
    tvij[icol][0] = M_ONE ;
    taij[icol] = (int *)Realloc( taij[icol], sizeof(int), size, 1 ) ;
    taij[icol][0] = icol ;
    ai[icol] = 1 ;
    nSize[icol] = 1 ;
  }


  for( col=0 ; col<nf ; col++ ) {
    int    nrow = ai[col] ;
    int*   aaij = taij[col] ;
    MTYPE* vvij = tvij[col] ;
    for( row=0,bc=0 ; row<nrow ; row++ ) {
      int    irow = aaij[row] ;
      if( irow == col ) {
        continue ;
      }
      for( ; bc<nbc ; bc++ ) {
        if( irow < indbc[bc] ) {
          break ;
        } else if( indbc[bc] == irow ) {
          b[col] = MMINUS( b[col], MMULTI( vvij[row], val[bc] ) ) ;
          vvij[row] = M_ZERO ;
          bc++ ;
          break ;
        }
      }
    }
  }


  for( bc=0 ; bc<nbc ; bc++ ) {
    b[indbc[bc]] = val[bc] ;
  }


  __MYMT_AIJ_DELETEZERO__( tmp_aij ) ;
}

/********************************************************************/

void __MYMT_AIJ_ASSEM__( AIJ_MAT *aij_mat, TMP_AIJ *tmp_aij )
{
  /*+ Function: assemble struct AIJ_MAT +*/
  int    col ;
  int    row ;
  int    nf = tmp_aij->nf ;
  int**  taij = tmp_aij->tmp_aij ;
  MTYPE** tvij = tmp_aij->TMP_VIJ ;
  int*   nSize = tmp_aij->nSize ;
  int*   ai = tmp_aij->tmp_ai ;
  int*   aij = NULL ;
  MTYPE* vij = NULL ;
  int    nel ;


  /*nel = 0 ;
  for( col=0 ; col<nf ; col++ ) {
    nel += ai[col] ;
  }
  aij = (int *)Malloc( sizeof(int), nel ) ;
  vij = (MTYPE *)Malloc( sizeof(MTYPE), nel ) ;*/
  aij_mat->nf = nf ;
  nel = 0 ;
  for( col=0 ; col<nf ; col++ ) {
    int    nrow = ai[col] ;
    int*   aaij = taij[col] ;  //Tate.
    int    size = nSize[col] ; 
    aij = (int *)Realloc( aij, sizeof(int), nel, nel+nrow ) ;
    for( row=0 ; row<nrow ; row++,nel++ ) {
      aij[nel] = aaij[row] ; //Yoko.
    }
    Free( sizeof(int)*size, taij[col] ) ;
  }
  nel = 0 ;
  for( col=0 ; col<nf ; col++ ) {
    int    nrow = ai[col] ;
    MTYPE* vvij = tvij[col] ;
    int    size = nSize[col] ;
    vij = (MTYPE *)Realloc( vij, sizeof(MTYPE), nel, nel+nrow ) ;
    for( row=0 ; row<nrow ; row++,nel++ ) {
      vij[nel] = vvij[row] ;
    }
    Free( sizeof(MTYPE)*size, tvij[col] ) ;
  }
  /*nel = 0 ;
  for( col=0 ; col<nf ; col++ ) {
    int    nrow = ai[col] ;
    int*   aaij = taij[col] ;
    MTYPE* vvij = tvij[col] ;
    int    size = nSize[col] ;*/
    /*aij = (int *)Realloc( aij, sizeof(int), nel, nel+nrow ) ;
    vij = (MTYPE *)Realloc( vij, sizeof(MTYPE), nel, nel+nrow ) ;*/
    /*for( row=0 ; row<nrow ; row++,nel++ ) {
      aij[nel] = aaij[row] ;
      vij[nel] = vvij[row] ;
    }
    Free( sizeof(int)*size, taij[col] ) ;
    Free( sizeof(MTYPE)*size, tvij[col] ) ;
  }*/
  Free( sizeof(int*)*nf, taij ) ;
  Free( sizeof(MTYPE*)*nf, tvij ) ;
  Free( sizeof(int)*nf, nSize ) ;

  //printf("check__MYMT_AIJ_ASSEM__\n");

  for( col=nf ; col>=0 ; col-- ) {
    nel -= ai[col] ;
    ai[col] = nel ;
  }
  if( ai[0] != 0 ) {
    emessage( 1000, __ABORT_MYMT_AIJ_SOLVER__, NULL ) ;
  }
  // Complex *tmp_b;
  //tmp_b=(Complex *)tmp_aij->TMP_B;
  /*
  for(col=0;col<nf;col++)
    printf("tmp_b[%d]=(%e,%e)\n",col,tmp_b[col].re,tmp_b[col].im);
  */

  aij_mat->ai = ai ;
  aij_mat->aij = aij ;
  aij_mat->AIJ_VIJ = vij ;
  aij_mat->AIJ_B = tmp_aij->TMP_B ;
  if( tmp_aij->TMP_X != NULL ) {
    aij_mat->AIJ_X = tmp_aij->TMP_X ;
  } else {
    aij_mat->AIJ_X = NULL ;
  }
  if( tmp_aij->TMP_PC != NULL ) {
    aij_mat->pc_size = tmp_aij->tmp_pc_size ;
    aij_mat->AIJ_PC = tmp_aij->TMP_PC ;
  } else {
    aij_mat->pc_size = 0 ;
    aij_mat->AIJ_PC = NULL ;
  }
}

/********************************************************************/

void __MYMT_AIJ_FREE__( AIJ_MAT *aij_mat )
{
  /*+ Function: free struct AIJ_MAT +*/
  int    nf = aij_mat->nf ;
  int    nel = aij_mat->ai[nf] ;


  Free( sizeof(int)*(nf+1), aij_mat->ai ) ;
  Free( sizeof(int)*nel, aij_mat->aij ) ;
  Free( sizeof(MTYPE)*nel, aij_mat->AIJ_VIJ ) ;
  Free( sizeof(MTYPE)*nf, aij_mat->AIJ_B ) ;
  /*if( aij_mat->AIJ_X != NULL ) {
    Free( sizeof(MTYPE)*nf, aij_mat->AIJ_X ) ;
  }*/
  if( aij_mat->pc_size != 0 ) {
    Free( sizeof(MTYPE)*aij_mat->pc_size, aij_mat->AIJ_PC ) ;
  }
}

/********************************************************************/

void __MYMT_AIJ_SETAE__( TMP_AIJ *tmp_aij,
                         int szRow, int *indRow,
                         int szCol, int *indCol,
                         MTYPE *ae )
{
  /*+ Function: set Ae to AIJ +*/
  int    i, j ;

  int    row ;
  int    col ;

  int*   ai = tmp_aij->tmp_ai ;
  int**  taij = tmp_aij->tmp_aij ;
  MTYPE** tvij = tmp_aij->TMP_VIJ ;
  int*   nSize = tmp_aij->nSize ;



  #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ /* symmetric */
    for( col=0,i=0 ; col<szCol ; col++ ) {
      for( row=0 ; row<szRow ; row++,i++ ) {
        if( indCol[col] < indRow[row] ) {
          ae[i] = M_ZERO ;
        }
      }
    }
  #endif /*__ADVFW_MATRIX_SYSTEM_SYMMETRIC__ */


  for( col=0,j=0 ; col<szCol ; col++ ) {
    int    icol = indCol[col] ;
    for( row=0 ; row<szRow ; row++,j++ ) {
      int    irow = indRow[row] ;
      MTYPE  dd = ae[j] ;
      if( !ISZERO(dd) ) {
        int    ii = 0 ;
        int    jj = ai[icol] ;
        for( i=0 ; i<jj ; i++ ) {
          if( taij[icol][i] == irow ) {
            tvij[icol][i] = MPLUS( tvij[icol][i], dd ) ;
            ii = 1 ;
            break ;
          }
        }
        if( ii == 0 ) {
          if( jj != nSize[icol] ) {
            taij[icol][jj] = irow ;
            tvij[icol][jj] = dd ;
          } else {
            int    kk = jj + tmp_aij->nMargin ;
            tvij[icol] = (MTYPE *)Realloc( tvij[icol], sizeof(MTYPE),
                                           jj, kk ) ;
            tvij[icol][jj] = dd ;
            taij[icol] = (int *)Realloc( taij[icol], sizeof(int),
                                         jj, kk ) ;
            taij[icol][jj] = irow ;
            nSize[icol] = kk ;
          }
          ai[icol]++ ;
        }
      }
    }
  }
}

/********************************************************************/

void __MYMT_AIJ_SETFE__( MTYPE *TMP_B,
                         int szCol, int *indCol,
                         MTYPE *fe )
{
  /*+ Function: set fe to AIJ +*/
  int    col ;


  for( col=0 ; col<szCol ; col++ ) {
    TMP_B[indCol[col]] = MPLUS( TMP_B[indCol[col]], fe[col] ) ;
  }
}

/********************************************************************/

void __MYMT_AIJ_DELETEZERO__( TMP_AIJ *tmp_aij )
{
  /*+ Function: delete zero element +*/
  int    col ;
  int    row ;

  int    nf = tmp_aij->nf ;
  int*   ai = tmp_aij->tmp_ai ;
  int**  taij = tmp_aij->tmp_aij ;
  MTYPE** tvij = tmp_aij->TMP_VIJ ;
  int*   nSize = tmp_aij->nSize ;


  for( col=0 ; col<nf ; col++ ) {
    int    nrow = ai[col] ;
    int*   aaij = taij[col] ;
    MTYPE* vvij = tvij[col] ;
    int    ii = 0 ;
    for( row=0 ; row<nrow ; row++ ) {
      if( !ISZERO(vvij[row]) ) {
        ii++ ;
      }
    }
    if( ii != nrow ) {
      int    jj ;
      int    *aij ;
      MTYPE  *vij ;
      int    size = nSize[col] ;
      aij = (int *)Malloc( sizeof(int), ii ) ;
      vij = (MTYPE *)Malloc( sizeof(MTYPE), ii ) ;
      jj = 0 ;
      for( row=0 ; row<nrow ; row++ ) {
        if( !ISZERO(vvij[row]) ) {
          aij[jj] = aaij[row] ;
          vij[jj] = vvij[row] ;
          jj++ ;
        }
      }
      if( ii != jj ) {
        emessage( 1010, __ABORT_MYMT_AIJ_SOLVER__, NULL ) ;
      }
      Free( sizeof(int)*size, aaij ) ;
      Free( sizeof(MTYPE)*size, vvij ) ;
      ai[col] = ii ;
      taij[col] = aij ;
      tvij[col] = vij ;
      nSize[col] = ii ;
    }
  }
}

/********************************************************************/

void __MYMT_AIJ_COPY_TMP_AIJ__( TMP_AIJ *a, TMP_AIJ b )
{
  /*+ Function: copy struct TMP_AIJ b -> a +*/
  __MYMT_AIJ_COPY_TMP_AIJ_A__( a, b ) ;
  __MYMT_AIJ_COPY_TMP_AIJ_B__( a, b ) ;
}

/********************************************************************/

void __MYMT_AIJ_COPY_TMP_AIJ_A__( TMP_AIJ *a, TMP_AIJ b )
{
  /*+ Function: copy struct TMP_AIJ b -> a +*/
  int    i ;

  int    nf = b.nf ;


  a->nf = nf ;
  i = nf + 1 ;
  a->tmp_ai = (int *)Calloc( sizeof(int), i ) ;
  memcpy( a->tmp_ai, b.tmp_ai, sizeof(int)*i ) ;
  a->nMargin = b.nMargin ;
  a->tmp_aij = (void *)Malloc( sizeof(int*), nf ) ;
  a->TMP_VIJ = (void *)Malloc( sizeof(MTYPE*), nf ) ;
  for( i=0 ; i<nf ; i++ ) {
    int    nrow = b.nSize[i] ;
    a->tmp_aij[i] = (int *)Malloc( sizeof(int), nrow ) ;
    a->TMP_VIJ[i] = (MTYPE *)Malloc( sizeof(MTYPE), nrow ) ;
    memcpy( a->tmp_aij[i], b.tmp_aij[i], sizeof(int)*nrow ) ;
    memcpy( a->TMP_VIJ[i], b.TMP_VIJ[i], sizeof(MTYPE)*nrow ) ;
  }
  a->nSize = (int *)Malloc( sizeof(int), nf ) ;
  memcpy( a->nSize, b.nSize, sizeof(int)*nf ) ;
  a->tmp_pc_size = b.tmp_pc_size ;
  if( b.TMP_PC != NULL ) {
    a->TMP_PC = (MTYPE *)Malloc( sizeof(MTYPE), b.tmp_pc_size ) ;
    memcpy( a->TMP_PC, b.TMP_PC, sizeof(MTYPE)*b.tmp_pc_size ) ;
  } else {
    a->TMP_PC = NULL ;
  }
}

/********************************************************************/

void __MYMT_AIJ_COPY_TMP_AIJ_B__( TMP_AIJ *a, TMP_AIJ b )
{
  /*+ Function: copy struct TMP_AIJ.TMP_B b -> a +*/
  int    nf = b.nf ;


  a->TMP_B = (MTYPE *)Malloc( sizeof(MTYPE), nf ) ;
  memcpy( a->TMP_B, b.TMP_B, sizeof(MTYPE)*nf ) ;
  if( b.TMP_X != NULL ) {
    a->TMP_X = (MTYPE *)Malloc( sizeof(MTYPE), nf ) ;
    memcpy( a->TMP_X, b.TMP_X, sizeof(MTYPE)*nf ) ;
  } else {
    a->TMP_X = NULL ;
  }
}

/********************************************************************/

void __MYMT_AIJ_PRINT_TMP_AIJ__( FILE *fp, TMP_AIJ tmp_aij )
{
  /*+ Function: print struct TMP_AIJ +*/
  int    i ;

  int    nf = tmp_aij.nf ;


  fprintf( fp, "nf %d\ntmp_ai\n", nf ) ;
  for( i=0 ; i<nf+1 ;i ++ ) {
    fprintf( fp, " %d", tmp_aij.tmp_ai[i] ) ;
  }
  fprintf( fp, "\ntmp_aij\n" ) ;
  for( i=0 ; i<nf ; i++ ) {
    int row ;
    int nrow = tmp_aij.tmp_ai[i] ;
    for( row=0 ; row<nrow ; row++ ) {
      fprintf( fp, " %d", tmp_aij.tmp_aij[i][row] ) ;
    }
    fprintf( fp, "\n" ) ;
  }
  fprintf( fp, "tmp_vij\n" ) ;
  for( i=0 ; i<nf ; i++ ) {
    int row ;
    int nrow = tmp_aij.tmp_ai[i] ;
    for( row=0 ; row<nrow ; row++ ) {
      #ifdef __ADVFW_MATRIX_SYSTEM_REAL__ /* real number */
        fprintf( fp, " %lg", tmp_aij.TMP_VIJ[i][row] ) ;
      #else /* __ADVFW_MATRIX_SYSTEM_REAL__ */ /* complex number */
        fprintf( fp, " %lg+%lgi", tmp_aij.TMP_VIJ[i][row].re,
                                  tmp_aij.TMP_VIJ[i][row].im ) ;
      #endif /* __ADVFW_MATRIX_SYSTEM_REAL__ */
    }
    fprintf( fp, "\n" ) ;
  }
  fprintf( fp, "tmp_b\n" ) ;
  for( i=0 ; i<nf ; i++ ) {
    #ifdef __ADVFW_MATRIX_SYSTEM_REAL__ /* real number */
      fprintf( fp, " %lg", tmp_aij.TMP_B[i] ) ;
    #else /* __ADVFW_MATRIX_SYSTEM_REAL__ */ /* complex number */
      fprintf( fp, " %lg+%lgi", tmp_aij.TMP_B[i].re,
                                tmp_aij.TMP_B[i].im ) ;
    #endif /* __ADVFW_MATRIX_SYSTEM_REAL__ */
  }
  fprintf( fp, "\n" ) ;

  if( tmp_aij.TMP_X != NULL ) {
    fprintf( fp, "tmp_x\n" ) ;
    for( i=0 ; i<nf ; i++ ) {
      #ifdef __ADVFW_MATRIX_SYSTEM_REAL__ /* real number */
        fprintf( fp, " %lg", tmp_aij.TMP_X[i] ) ;
      #else /* __ADVFW_MATRIX_SYSTEM_REAL__ */ /* complex number */
        fprintf( fp, " %lg+%lgi", tmp_aij.TMP_X[i].re,
                                  tmp_aij.TMP_X[i].im ) ;
      #endif /* __ADVFW_MATRIX_SYSTEM_REAL__ */
    }
    fprintf( fp, "\n" ) ;
  }

  if( tmp_aij.TMP_PC != NULL ) {
    fprintf( fp, "tmp_pc  size %d\n", tmp_aij.tmp_pc_size ) ;
    for( i=0 ; i<tmp_aij.tmp_pc_size ; i++) {
      #ifdef __ADVFW_MATRIX_SYSTEM_REAL__ /* real number */
        fprintf( fp, " %lg\n", tmp_aij.TMP_PC[i] ) ;
      #else /* __ADVFW_MATRIX_SYSTEM_REAL__ */ /* complex number */
        fprintf( fp, " %lg+%lgi\n", tmp_aij.TMP_PC[i].re,
                                    tmp_aij.TMP_PC[i].im ) ;
      #endif /* __ADVFW_MATRIX_SYSTEM_REAL__ */
    }
  }
}

/********************************************************************/

void __MYMT_AIJ_PRINT_AIJ_MAT__( FILE *fp, AIJ_MAT aij_mat )
{
  /*+ Function: print struct AIJ_MAT +*/
  int    i ;

  int    nf = aij_mat.nf ;


  fprintf( fp, "nf %d\nai\n", nf ) ;
  for( i=0 ; i<nf+1 ; i++ ) {
    fprintf( fp, " %d", aij_mat.ai[i] ) ;
  }
  fprintf( fp, "\naij\n" ) ;
  for( i=0 ; i<nf ; i++ ) {
    int row ;
    for( row=aij_mat.ai[i] ; row<aij_mat.ai[i+1] ; row++ ) {
      fprintf( fp, " %d", aij_mat.aij[row] ) ;
    }
    fprintf( fp, "\n" ) ;
  }
  fprintf( fp, "vij\n" ) ;
  for( i=0 ; i<nf ; i++ ) {
    int row ;
    for( row=aij_mat.ai[i] ; row<aij_mat.ai[i+1] ; row++ ) {
      #ifdef __ADVFW_MATRIX_SYSTEM_REAL__ /* real number */
        fprintf( fp, " %lg", aij_mat.AIJ_VIJ[row] ) ;
      #else /* __ADVFW_MATRIX_SYSTEM_REAL__ */ /* complex number */
        fprintf( fp, " %lg+%lgi", aij_mat.AIJ_VIJ[row].re,
                                  aij_mat.AIJ_VIJ[row].im ) ;
      #endif /* __ADVFW_MATRIX_SYSTEM_REAL__ */
    }
    fprintf( fp, "\n" ) ;
  }
  fprintf( fp, "f\n" ) ;
  for( i=0 ; i<nf ; i++ ) {
    #ifdef __ADVFW_MATRIX_SYSTEM_REAL__ /* real number */
      fprintf( fp, " %lg", aij_mat.AIJ_B[i] ) ;
    #else /* __ADVFW_MATRIX_SYSTEM_REAL__ */ /* complex number */
      fprintf( fp, " %lg+%lgi", aij_mat.AIJ_B[i].re,
                                aij_mat.AIJ_B[i].im ) ;
    #endif /* __ADVFW_MATRIX_SYSTEM_REAL__ */
  }
  fprintf( fp, "\n" ) ;

  if( aij_mat.AIJ_PC != NULL ) {
    fprintf( fp, "aij_pc  size %d\n", aij_mat.pc_size ) ;
    for( i=0 ; i<aij_mat.pc_size ; i++) {
      #ifdef __ADVFW_MATRIX_SYSTEM_REAL__ /* real number */
        fprintf( fp, " %lg\n", aij_mat.AIJ_PC[i] ) ;
      #else /* __ADVFW_MATRIX_SYSTEM_REAL__ */ /* complex number */
        fprintf( fp, " %lg+%lgi\n", aij_mat.AIJ_PC[i].re,
                                    aij_mat.AIJ_PC[i].im ) ;
      #endif /* __ADVFW_MATRIX_SYSTEM_REAL__ */
    }
  }
}

/********************************************************************/

void __MYMT_AIJ_MAIJMXV__( int m, int *ai, int *aij, MTYPE *vij,
                           MTYPE *vec, MTYPE *x )
{
  /*+ Function: calculate x = A{vec} +*/
  int    i, j ;


  for( i=0 ; i<m ; i++ ) {
    x[i] = M_ZERO ;
  }


  for( i=0 ; i<m ; i++ ) {
    int    ii = ai[i+1] ;
    for( j=ai[i] ; j<ii ; j++ ) {
    #ifdef __ADVFW_MATRIX_SYSTEM_ASYMMETRIC__
      x[i] = MPLUS( x[i], MMULTI(vij[j], vec[aij[j]]) ) ;
    #endif /*__ADVFW_MATRIX_SYSTEM_ASYMMETRIC__ */
    #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__
      int    jj = aij[j] ;
      MTYPE  dd = vij[j] ;
      x[i] = MPLUS( x[i], MMULTI(dd, vec[jj]) ) ;
      x[jj] = MPLUS( x[jj], MMULTI(dd, vec[i]) ) ;
    }
    j-- ;
    if( i == aij[j] ) {
      x[i] = MMINUS( x[i], MMULTI(vij[j], vec[aij[j]]) ) ;
    #endif /*__ADVFW_MATRIX_SYSTEM_SYMMETRIC__ */
    }
  }
}

/********************************************************************/

void __MYMT_AIJ_MAIJMXV_TMP_AIJ__( TMP_AIJ tmp_aij, MTYPE *vec, MTYPE *x )
{
  /*+ Function: calculate x = A{vec} +*/
  int    i ;

  int    col ;
  int    nf = tmp_aij.nf ;
  int*   tai = tmp_aij.tmp_ai ;
  int**  taij = tmp_aij.tmp_aij ;
  MTYPE** tvij = tmp_aij.TMP_VIJ ;


  for( i=0 ; i<nf ; i++ ) {
    x[i] = M_ZERO ;
  }


  for( col=0 ; col<nf ; col++ ) {
    int    row ;
    int    nrow = tai[col] ;
    int*   aaij = taij[col] ;
    MTYPE* vvij = tvij[col] ;
    for( row=0 ; row<nrow ; row++ ) {
      int    irow = aaij[row] ;
      MTYPE  d0 = MMULTI( vvij[row], vec[irow] ) ;
      x[col] = MPLUS( x[col], d0 ) ;
      #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__
        if( irow != col ) {
          MTYPE  d1 = MMULTI( vvij[row], vec[col] ) ;
          x[irow] = MPLUS( x[irow], d1 ) ;
        }
      #endif
    }
  }
}

/********************************************************************/

MTYPE __MYMT_AIJ_MGETAIJ__( int pos_i, int pos_j,
                            int *ai, int *aij, MTYPE *vij )
{
  /*+ Function: get A[i][j] +*/
  int i ;


  #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ /* symmetric */
    if( pos_i < pos_j ) {
      i = pos_i ; pos_i = pos_j ; pos_j = i ;
    }
  #endif /* __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ */
  for( i=ai[pos_i] ; i<ai[pos_i+1] ; i++ ) {
    if( pos_j == aij[i] ) {
      return vij[i] ;
    }
  }


  return M_ZERO ;
}

/********************************************************************/

void __MYMT_AIJ_FREE_TMP_AIJ__( TMP_AIJ *tmp_aij )
{
  /*+ Function: free struct TMP_AIJ +*/
  int    col ;
  int    nf = tmp_aij->nf ;
  int*   nSize = tmp_aij->nSize ;


  Free( sizeof(int)*(nf+1), tmp_aij->tmp_ai ) ;
  for( col=0 ; col<nf ; col++ ) {
    int    size = nSize[col] ;
    Free( sizeof(int)*size, tmp_aij->tmp_aij[col] ) ;
    Free( sizeof(MTYPE)*size, tmp_aij->TMP_VIJ[col] ) ;
  }
  Free( sizeof(int*)*nf, tmp_aij->tmp_aij ) ;
  Free( sizeof(MTYPE*)*nf, tmp_aij->TMP_VIJ ) ;
  Free( sizeof(MTYPE)*nf, tmp_aij->TMP_B ) ;
  Free( sizeof(int)*nf, tmp_aij->nSize ) ;
  if( tmp_aij->TMP_X != NULL ) {
    Free( sizeof(MTYPE)*nf, tmp_aij->TMP_X ) ;
  }
  if( tmp_aij->TMP_PC != NULL ) {
    Free( sizeof(MTYPE)*tmp_aij->tmp_pc_size, tmp_aij->TMP_PC ) ;
  }
}
