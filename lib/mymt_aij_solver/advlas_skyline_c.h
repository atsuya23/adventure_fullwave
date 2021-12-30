#include "../std/advfullwave_lib.h"
#include "../std/mtype.h"

//#ifndef _advlas_skyline_c_h_
//#define _advlas_skyline_c_h_

void advlas_copy_nzindex_to_skyindex( int *NzIdx_ia, int *NzIdx_jcn, 
				      DMatrix *dmat );

void advlas_copy_nzmat_to_skymat(int *NzIdx_ia, int *NzIdx_jcn, 
				 MTYPE *NzMat, DMatrix *dmat) ;

void advlas_skyline_ldl_decomposite(DMatrix *dmat, MTYPE* work) ;

void advlas_skyline_ldl_solve(DMatrix *dmat, MTYPE *solution) ;

//#endif /* _advlas_skyline_c_h_ */
