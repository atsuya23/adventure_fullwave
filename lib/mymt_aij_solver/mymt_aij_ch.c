/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "mymt_aij_ch.h"


#define __MYMT_AIJ_COMPLEX__   /* complex */
#define __MYMT_AIJ_HERMITIAN__ /* hermitian */


#include "mtype.h"


/*** aij_matrix_func.h ***/
#define __MYMT_AIJ_INIT__              mymt_CH_AIJ_Init
#define __MYMT_AIJ_SET__               mymt_CH_AIJ_Set
#define __MYMT_AIJ_SETBC__             mymt_CH_AIJ_SetBC
#define __MYMT_AIJ_ASSEM__             mymt_CH_AIJ_Assem
#define __MYMT_AIJ_FREE__              mymt_CH_AIJ_Free
#define __MYMT_AIJ_SETAE__             mymt_CH_AIJ_SetAe
#define __MYMT_AIJ_SETFE__             mymt_CH_AIJ_SetFe
#define __MYMT_AIJ_DELETEZERO__        mymt_CH_AIJ_DeleteZero
#define __MYMT_AIJ_COPY_TMP_AIJ__      mymt_CH_AIJ_Copy_TMP_AIJ
#define __MYMT_AIJ_COPY_TMP_AIJ_A__    mymt_CH_AIJ_Copy_TMP_AIJ_A
#define __MYMT_AIJ_COPY_TMP_AIJ_B__    mymt_CH_AIJ_Copy_TMP_AIJ_B
#define __MYMT_AIJ_PRINT_TMP_AIJ__     mymt_CH_AIJ_Print_TMP_AIJ
#define __MYMT_AIJ_PRINT_AIJ_MAT__     mymt_CH_AIJ_Print_AIJ_MAT
#define __MYMT_AIJ_MAIJMXV__           mymt_CH_AIJ_MxV
#define __MYMT_AIJ_MGETAIJ__           mymt_CH_AIJ_getAIJ


#include "aij_matrix_func.h"



/*** aij_pc_func.h ***/
#define __MYMT_AIJ_PC_NONE_MAKE__      mymt_CH_AIJ_PC_NONE_make
#define __MYMT_AIJ_PC_NONE_SET__       mymt_CH_AIJ_PC_NONE_set
#define __MYMT_AIJ_PC_ICC_MAKE__       mymt_CH_AIJ_PC_ICC_make
#define __MYMT_AIJ_PC_ICC_SET__        mymt_CH_AIJ_PC_ICC_set
#define __MYMT_AIJ_PC_MRIC0_MAKE__     mymt_CH_AIJ_PC_MRIC0_make
#define __MYMT_AIJ_PC_MRIC0_SET__      mymt_CH_AIJ_PC_MRIC0_set


#include "aij_pc_func.h"



/*** aij_solver_func.h ***/
#define __MYMT_AIJ_SOLVER_JUDGE__      mymt_CH_AIJ_Solver_Judge
/** CG method **/
#define __MYMT_AIJ_SOLVER_CG__         mymt_CH_AIJ_Solver_CG
#define __MYMT_AIJ_SOLVER_CG_NMPCM__   mymt_CH_AIJ_Solver_CG_nmpcm
#define __MYMT_AIJ_SOLVER_CG_FUNC__    mymt_CH_AIJ_Solver_CG_func


#include "aij_solver_func.h"
