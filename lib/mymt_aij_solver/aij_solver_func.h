/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/


int __MYMT_AIJ_SOLVER_JUDGE__( int solver_status )
{
  /*+ Function: Judgement of solver status +*/
  if( solver_status > 0 ) {
    return 0 ;
  } else if( solver_status == SOLVER_STATUS_CONVERGED ) {
    return 0 ;
  } else if( solver_status == SOLVER_STATUS_DIVERGED ) {
    emessage( 1200, __ABORT_MYMT_AIJ_SOLVER__, NULL ) ;
  } else if( solver_status == SOLVER_STATUS_REACHED_MAX_ITE ) {
    emessage( 1201, __ABORT_MYMT_AIJ_SOLVER__, NULL ) ;
  } else if( solver_status == SOLVER_STATUS_UNKNOWN_PC ) {
    emessage( 1202, __ABORT_MYMT_AIJ_SOLVER__, NULL ) ;
  } else if( solver_status == SOLVER_STATUS_UNKNOWN_ERROR ) {
    emessage( 1203, __ABORT_MYMT_AIJ_SOLVER__, NULL ) ;
  } else if( solver_status == SOLVER_STATUS_GMRES_RESTART ) {
    emessage( 1204, __ABORT_MYMT_AIJ_SOLVER__, NULL ) ;
  }

  return 0 ;
}

/********************************************************************/

#ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__

/*** CG method ***/
#ifdef __ADVFW_MATRIX_SYSTEM_REAL__
#define __MYMT_AIJ_HERMITIAN__
#define __MYMT_AIJ_SOLVER_CGM__       __MYMT_AIJ_SOLVER_CG__
#define __MYMT_AIJ_SOLVER_CGM_NMPCM__ __MYMT_AIJ_SOLVER_CG_NMPCM__
#define __MYMT_AIJ_SOLVER_CGM_FUNC__  __MYMT_AIJ_SOLVER_CG_FUNC__
#define __MYMT_DMAT_SOLVER_LUM__       __MYMT_DMAT_SOLVER_LU__
#define __MYMT_DMAT_SOLVER_LUM_NMPCM__ __MYMT_DMAT_SOLVER_LU_NMPCM__
#define __MYMT_DMAT_SOLVER_LUM_FUNC__ __MYMT_DMAT_SOLVER_LU_FUNC__
#define __LOCAL_INNER__( n, a, b ) MINNER( (n), (a), (b) )
#include "aij_solver_cg.h"
#undef __MYMT_AIJ_HERMITIAN__
#undef __MYMT_AIJ_SOLVER_CGM_FUNC__
#undef __MYMT_AIJ_SOLVER_CGM_NMPCM__
#undef __MYMT_AIJ_SOLVER_CGM__
#undef __LOCAL_INNER__
#undef __MYMT_DMAT_SOLVER_LUM_FUNC__
#undef __MYMT_DMAT_SOLVER_LUM_NMPCM__
#undef __MYMT_DMAT_SOLVER_LUM__

#endif /* __ADVFW_MATRIX_SYSTEM_REAL__ */

/*** COCG method / LU solver ***/
#ifdef __ADVFW_MATRIX_SYSTEM_COMPLEX__
#define __MYMT_AIJ_SOLVER_CGM__       __MYMT_AIJ_SOLVER_COCG__
#define __MYMT_AIJ_SOLVER_CGM_NMPCM__ __MYMT_AIJ_SOLVER_COCG_NMPCM__
#define __MYMT_AIJ_SOLVER_CGM_FUNC__  __MYMT_AIJ_SOLVER_COCG_FUNC__
#define __MYMT_DMAT_SOLVER_LUM__       __MYMT_DMAT_SOLVER_LU__
#define __MYMT_DMAT_SOLVER_LUM_NMPCM__ __MYMT_DMAT_SOLVER_LU_NMPCM__
#define __MYMT_DMAT_SOLVER_LUM_FUNC__ __MYMT_DMAT_SOLVER_LU_FUNC__
#define __LOCAL_INNER__( n, a, b ) MBILINEAR( (n), (a), (b) )
#include "aij_solver_cg.h"
#undef __LOCAL_INNER__
#undef __MYMT_AIJ_SOLVER_CGM_FUNC__
#undef __MYMT_AIJ_SOLVER_CGM_NMPCM__
#undef __MYMT_AIJ_SOLVER_CGM__

#undef __MYMT_DMAT_SOLVER_LUM_FUNC__
#undef __MYMT_DMAT_SOLVER_LUM_NMPCM__
#undef __MYMT_DMAT_SOLVER_LUM__
#endif /* __ADVFW_MATRIX_SYSTEM_COMPLEX__ */

#endif /* __ADVFW_MATRIX_SYSTEM_SYMMETRC__ */

/********************************************************************/
