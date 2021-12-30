/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __MTYPE_H__
#define __MTYPE_H__


#ifdef __ADVFW_MATRIX_SYSTEM_COMPLEX__ /* Complex */
  #define MTYPE                Complex
  #define TMP_VIJ              tmp_vij_c
  #define TMP_B                tmp_b_c
  #define TMP_X                tmp_x_c
  #define TMP_PC               tmp_pc_c
  #define AIJ_VIJ              vij_c
  #define AIJ_B                b_c
  #define AIJ_X                x_c
  #define AIJ_PC               pc_c
  #define M_ZERO               complex_0
  #define M_ONE                complex_1
  #define MPLUS( x, y )        ComplexPlus( (x), (y) )
  #define MMINUS( x, y )       ComplexMinus( (x), (y) )
  #define MMULTI( x, y )       ComplexMulti( (x), (y) )
  #define MMULTIS( c, r )      ComplexMultiScalar( (c), (r) )
  #define MDIVIDE( x, y )      ComplexDivide( (x), (y) )
  #define MRECIP( x )          ComplexReciprocal( (x) )
  #define MABS( x )            ComplexAbs( (x) )
  #define MABS2( x )           ComplexAbs2( (x) )
  #define MSQRT( x )           ComplexSqrt( (x) )
  #define MINNER( n, a, b )    ComplexInner( (n), (a), (b) )
  #define MBILINEAR( n, a, b ) ComplexBilinear( (n), (a), (b) )
  #define MSORT_IM             sort_ic
  #define ISZERO( x )          ( ((x).re == 0.0) && ((x).im == 0.0) )
  #define COM_Bcast_MTYPE      COM_Bcast_Cpl
  #define COM_Grp_Bcast_MTYPE  COM_Grp_Bcast_Cpl
  #define COM_WriteMTYPE       COM_WriteCpl
  #define COM_ReadMTYPE        COM_ReadCpl
#else /* __ADVFW_MATRIX_SYSTEM_COMPLEX__ */ /* Real */
  #define MTYPE                double
  #define TMP_VIJ              tmp_vij_d
  #define TMP_B                tmp_b_d
  #define TMP_X                tmp_x_d
  #define TMP_PC               tmp_pc_d
  #define AIJ_VIJ              vij_d
  #define AIJ_B                b_d
  #define AIJ_X                x_d
  #define AIJ_PC               pc_d
  #define M_ZERO               0.0
  #define M_ONE                1.0
  #define MPLUS( x, y )        ( (x) + (y) )
  #define MMINUS( x, y )       ( (x) - (y) )
  #define MMULTI( x, y )       ( (x) * (y) )
  #define MMULTIS( c, r )      ( (c) * (r) )
  #define MDIVIDE( x, y )      ( (x) / (y) )
  #define MRECIP( x )          ( 1.0 / (x) )
  #define MABS( x )            ( fabs(x) )
  #define MABS2( x )           ( (x) * (x) )
  #define MSQRT( x )           ( sqrt(fabs(x)) )
  #define MINNER( n, a, b )    RealInner( (n), (a), (b) )
  #define MBILINEAR( n, a, b ) RealInner( (n), (a), (b) )
  #define MSORT_IM             sort_id
  #define ISZERO( x )          ( (x) == 0.0 )
  #define COM_Bcast_MTYPE      COM_Bcast_Dbl
  #define COM_Grp_Bcast_MTYPE  COM_Grp_Bcast_Dbl
  #define COM_WriteMTYPE       COM_WriteDbl
  #define COM_ReadMTYPE        COM_ReadDbl
#endif /* __ADVFW_MATRIX_SYSTEM_COMPLEX__ */



#ifdef __ADVFW_MATRIX_SYSTEM_COMPLEX__ /* Complex */
  #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ /* Symmetric */
    #define MAIJMXV( nf, ai, aij, vij, v, x) mymt_CS_AIJ_MxV( (nf), (ai), (aij), (vij), (v), (x) )
    #define MGETAIJ( i, j, ai, aij, vij )    mymt_CS_AIJ_getAIJ( (i), (j), (ai), (aij), (vij) ) 
  #else /* __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ */ /* Asymmetric */
    #define MAIJMXV( nf, ai, aij, vij, v, x) mymt_CA_AIJ_MxV( (nf), (ai), (aij), (vij), (v), (x) )
    #define MGETAIJ( i, j, ai, aij, vij )    mymt_CA_AIJ_getAIJ( (i), (j), (ai), (aij), (vij) ) 
  #endif /* __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ */
#else /* __ADVFW_MATRIX_SYSTEM_COMPLEX__ */ /* Real */
  #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ /* Symmetric */
    #define MAIJMXV( nf, ai, aij, vij, v, x) mymt_RS_AIJ_MxV( (nf), (ai), (aij), (vij), (v), (x) )
    #define MGETAIJ( i, j, ai, aij, vij )    mymt_RS_AIJ_getAIJ( (i), (j), (ai), (aij), (vij) ) 
  #else /* __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ */ /* Asymmetric */
    #define MAIJMXV( nf, ai, aij, vij, v, x) mymt_RA_AIJ_MxV( (nf), (ai), (aij), (vij), (v), (x) )
    #define MGETAIJ( i, j, ai, aij, vij )    mymt_RA_AIJ_getAIJ( (i), (j), (ai), (aij), (vij) ) 
  #endif /* __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ */
#endif /* __ADVFW_MATRIX_SYSTEM_COMPLEX__ */



/*** MYMT solver ***/
#ifdef __ADVFW_MATRIX_SYSTEM_REAL__
  #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ /* real symmetric */
    /** aij_matrix_func.h **/
    #define __MYMT_AIJ_INIT__              mymt_RS_AIJ_Init
    #define __MYMT_AIJ_SET__               mymt_RS_AIJ_Set
    #define __MYMT_AIJ_SETBC__             mymt_RS_AIJ_SetBC
    #define __MYMT_AIJ_ASSEM__             mymt_RS_AIJ_Assem
    #define __MYMT_AIJ_FREE__              mymt_RS_AIJ_Free
    #define __MYMT_AIJ_SETAE__             mymt_RS_AIJ_SetAe
    #define __MYMT_AIJ_SETFE__             mymt_RS_AIJ_SetFe
    #define __MYMT_AIJ_DELETEZERO__        mymt_RS_AIJ_DeleteZero
    #define __MYMT_AIJ_COPY_TMP_AIJ__      mymt_RS_AIJ_Copy_TMP_AIJ
    #define __MYMT_AIJ_COPY_TMP_AIJ_A__    mymt_RS_AIJ_Copy_TMP_AIJ_A
    #define __MYMT_AIJ_COPY_TMP_AIJ_B__    mymt_RS_AIJ_Copy_TMP_AIJ_B
    #define __MYMT_AIJ_PRINT_TMP_AIJ__     mymt_RS_AIJ_Print_TMP_AIJ
    #define __MYMT_AIJ_PRINT_AIJ_MAT__     mymt_RS_AIJ_Print_AIJ_MAT
    #define __MYMT_AIJ_MAIJMXV__           mymt_RS_AIJ_MxV
    #define __MYMT_AIJ_MAIJMXV_TMP_AIJ__   mymt_RS_AIJ_MxV_TMP_AIJ
    #define __MYMT_AIJ_MGETAIJ__           mymt_RS_AIJ_getAIJ
    #define __MYMT_AIJ_FREE_TMP_AIJ__      mymt_RS_AIJ_Free_TMP_AIJ

    /** aij_pc_func.h **/
    #define __MYMT_AIJ_PC_NONE_MAKE__      mymt_RS_AIJ_PC_NONE_make
    #define __MYMT_AIJ_PC_NONE_SET__       mymt_RS_AIJ_PC_NONE_set
    #define __MYMT_AIJ_PC_ICC_MAKE__       mymt_RS_AIJ_PC_ICC_make
    #define __MYMT_AIJ_PC_ICC_SET__        mymt_RS_AIJ_PC_ICC_set
    #define __MYMT_AIJ_PC_MRIC0_MAKE__     mymt_RS_AIJ_PC_MRIC0_make
    #define __MYMT_AIJ_PC_MRIC0_SET__      mymt_RS_AIJ_PC_MRIC0_set

    /** aij_solver_func.h **/
    #define __MYMT_AIJ_SOLVER_JUDGE__      mymt_RS_AIJ_Solver_Judge
    /* CG method */
    #define __MYMT_AIJ_SOLVER_CG__         mymt_RS_AIJ_Solver_CG
    #define __MYMT_AIJ_SOLVER_CG_NMPCM__   mymt_RS_AIJ_Solver_CG_nmpcm
    #define __MYMT_AIJ_SOLVER_CG_FUNC__    mymt_RS_AIJ_Solver_CG_func
    /* LU solver */
    #define __MYMT_DMAT_SOLVER_LU__  mymt_RS_DMAT_Solver_LU
    #define __MYMT_DMAT_SOLVER_LU_NMPCM__ mymt_RS_DMAT_Solver_LU_nmpcm
    #define __MYMT_DMAT_SOLVER_LU_FUNC__ mymt_RS_DMAT_Solver_LU_func

  #else                                     /* real asymmetric */
    /** aij_matrix_func.h **/
    #define __MYMT_AIJ_INIT__              mymt_RA_AIJ_Init
    #define __MYMT_AIJ_SET__               mymt_RA_AIJ_Set
    #define __MYMT_AIJ_SETBC__             mymt_RA_AIJ_SetBC
    #define __MYMT_AIJ_ASSEM__             mymt_RA_AIJ_Assem
    #define __MYMT_AIJ_FREE__              mymt_RA_AIJ_Free
    #define __MYMT_AIJ_SETAE__             mymt_RA_AIJ_SetAe
    #define __MYMT_AIJ_SETFE__             mymt_RA_AIJ_SetFe
    #define __MYMT_AIJ_DELETEZERO__        mymt_RA_AIJ_DeleteZero
    #define __MYMT_AIJ_COPY_TMP_AIJ__      mymt_RA_AIJ_Copy_TMP_AIJ
    #define __MYMT_AIJ_COPY_TMP_AIJ_A__    mymt_RA_AIJ_Copy_TMP_AIJ_A
    #define __MYMT_AIJ_COPY_TMP_AIJ_B__    mymt_RA_AIJ_Copy_TMP_AIJ_B
    #define __MYMT_AIJ_PRINT_TMP_AIJ__     mymt_RA_AIJ_Print_TMP_AIJ
    #define __MYMT_AIJ_PRINT_AIJ_MAT__     mymt_RA_AIJ_Print_AIJ_MAT
    #define __MYMT_AIJ_MAIJMXV__           mymt_RA_AIJ_MxV
    #define __MYMT_AIJ_MAIJMXV_TMP_AIJ__   mymt_RA_AIJ_MxV_TMP_AIJ
    #define __MYMT_AIJ_MGETAIJ__           mymt_RA_AIJ_getAIJ
    #define __MYMT_AIJ_FREE_TMP_AIJ__      mymt_RA_AIJ_Free_TMP_AIJ

    /** aij_pc_func.h **/
    #define __MYMT_AIJ_PC_NONE_MAKE__      mymt_RA_AIJ_PC_NONE_make
    #define __MYMT_AIJ_PC_NONE_SET__       mymt_RA_AIJ_PC_NONE_set

    /** aij_solver_func.h **/
    #define __MYMT_AIJ_SOLVER_JUDGE__      mymt_RA_AIJ_Solver_Judge


  #endif
#else
  #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ /* complex symmetric */
    /** aij_matrix_func.h **/
    #define __MYMT_AIJ_INIT__              mymt_CS_AIJ_Init
    #define __MYMT_AIJ_SET__               mymt_CS_AIJ_Set
    #define __MYMT_AIJ_SETBC__             mymt_CS_AIJ_SetBC
    #define __MYMT_AIJ_ASSEM__             mymt_CS_AIJ_Assem
    #define __MYMT_AIJ_FREE__              mymt_CS_AIJ_Free
    #define __MYMT_AIJ_SETAE__             mymt_CS_AIJ_SetAe
    #define __MYMT_AIJ_SETFE__             mymt_CS_AIJ_SetFe
    #define __MYMT_AIJ_DELETEZERO__        mymt_CS_AIJ_DeleteZero
    #define __MYMT_AIJ_COPY_TMP_AIJ__      mymt_CS_AIJ_Copy_TMP_AIJ
    #define __MYMT_AIJ_COPY_TMP_AIJ_A__    mymt_CS_AIJ_Copy_TMP_AIJ_A
    #define __MYMT_AIJ_COPY_TMP_AIJ_B__    mymt_CS_AIJ_Copy_TMP_AIJ_B
    #define __MYMT_AIJ_PRINT_TMP_AIJ__     mymt_CS_AIJ_Print_TMP_AIJ
    #define __MYMT_AIJ_PRINT_AIJ_MAT__     mymt_CS_AIJ_Print_AIJ_MAT
    #define __MYMT_AIJ_MAIJMXV__           mymt_CS_AIJ_MxV
    #define __MYMT_AIJ_MAIJMXV_TMP_AIJ__   mymt_CS_AIJ_MxV_TMP_AIJ
    #define __MYMT_AIJ_MGETAIJ__           mymt_CS_AIJ_getAIJ
    #define __MYMT_AIJ_FREE_TMP_AIJ__      mymt_CS_AIJ_Free_TMP_AIJ

    /** aij_pc_func.h **/
    #define __MYMT_AIJ_PC_NONE_MAKE__      mymt_CS_AIJ_PC_NONE_make
    #define __MYMT_AIJ_PC_NONE_SET__       mymt_CS_AIJ_PC_NONE_set
    #define __MYMT_AIJ_PC_ICC_MAKE__       mymt_CS_AIJ_PC_ICC_make
    #define __MYMT_AIJ_PC_ICC_SET__        mymt_CS_AIJ_PC_ICC_set
    #define __MYMT_AIJ_PC_MRIC0_MAKE__     mymt_CS_AIJ_PC_MRIC0_make
    #define __MYMT_AIJ_PC_MRIC0_SET__      mymt_CS_AIJ_PC_MRIC0_set

    /** aij_solver_func.h **/
    #define __MYMT_AIJ_SOLVER_JUDGE__      mymt_CS_AIJ_Solver_Judge
    /* COCG method */
    #define __MYMT_AIJ_SOLVER_COCG__       mymt_CS_AIJ_Solver_COCG
    #define __MYMT_AIJ_SOLVER_COCG_NMPCM__ mymt_CS_AIJ_Solver_COCG_nmpcm
    #define __MYMT_AIJ_SOLVER_COCG_FUNC__  mymt_CS_AIJ_Solver_COCG_func
    /* LU solver */
    #define __MYMT_DMAT_SOLVER_LU__  mymt_CS_DMAT_Solver_LU
    #define __MYMT_DMAT_SOLVER_LU_NMPCM__ mymt_CS_DMAT_Solver_LU_nmpcm
    #define __MYMT_DMAT_SOLVER_LU_FUNC__ mymt_CS_DMAT_Solver_LU_func
  #else                                     /* complex asymmetric */
    /** aij_matrix_func.h **/
    #define __MYMT_AIJ_INIT__              mymt_CA_AIJ_Init
    #define __MYMT_AIJ_SET__               mymt_CA_AIJ_Set
    #define __MYMT_AIJ_SETBC__             mymt_CA_AIJ_SetBC
    #define __MYMT_AIJ_ASSEM__             mymt_CA_AIJ_Assem
    #define __MYMT_AIJ_FREE__              mymt_CA_AIJ_Free
    #define __MYMT_AIJ_SETAE__             mymt_CA_AIJ_SetAe
    #define __MYMT_AIJ_SETFE__             mymt_CA_AIJ_SetFe
    #define __MYMT_AIJ_DELETEZERO__        mymt_CA_AIJ_DeleteZero
    #define __MYMT_AIJ_COPY_TMP_AIJ__      mymt_CA_AIJ_Copy_TMP_AIJ
    #define __MYMT_AIJ_COPY_TMP_AIJ_A__    mymt_CA_AIJ_Copy_TMP_AIJ_A
    #define __MYMT_AIJ_COPY_TMP_AIJ_B__    mymt_CA_AIJ_Copy_TMP_AIJ_B
    #define __MYMT_AIJ_PRINT_TMP_AIJ__     mymt_CA_AIJ_Print_TMP_AIJ
    #define __MYMT_AIJ_PRINT_AIJ_MAT__     mymt_CA_AIJ_Print_AIJ_MAT
    #define __MYMT_AIJ_MAIJMXV__           mymt_CA_AIJ_MxV
    #define __MYMT_AIJ_MAIJMXV_TMP_AIJ__   mymt_CA_AIJ_MxV_TMP_AIJ
    #define __MYMT_AIJ_MGETAIJ__           mymt_CA_AIJ_getAIJ
    #define __MYMT_AIJ_FREE_TMP_AIJ__      mymt_CA_AIJ_Free_TMP_AIJ

    /** aij_pc_func.h **/
    #define __MYMT_AIJ_PC_NONE_MAKE__      mymt_CA_AIJ_PC_NONE_make
    #define __MYMT_AIJ_PC_NONE_SET__       mymt_CA_AIJ_PC_NONE_set

    /** aij_solver_func.h **/
    #define __MYMT_AIJ_SOLVER_JUDGE__      mymt_CA_AIJ_Solver_Judge
  #endif
#endif



/*** HDDM solver ***/
#ifdef __ADVFW_MATRIX_SYSTEM_COMPLEX__ /* Complex */
  #define HDDM_VEC_P           p_c
  #define HDDM_VEC_Q           q_c
  #define HDDM_VEC_R           r_c
  #define HDDM_VEC_Z           z_c
  #define HDDM_VEC_U           u_c
  #define HDDM_VEC_U_MEM       hddm_vec_u_c_mem
  #define HDDM_VEC_RZ          rz_c
  #define HDDM_VEC_PC          pc_c
  #define HDDM_DOM_VEC_VEC     vec_c
  #define HDDM_DOM_VEC_PC      pc_c
  #define GATHER_VEC           gather_vec_c
#else /* __ADVFW_MATRIX_SYSTEM_COMPLEX__ */ /* Real */
  #define HDDM_VEC_P           p_d
  #define HDDM_VEC_Q           q_d
  #define HDDM_VEC_R           r_d
  #define HDDM_VEC_Z           z_d
  #define HDDM_VEC_U           u_d
  #define HDDM_VEC_U_MEM       hddm_vec_u_d_mem
  #define HDDM_VEC_RZ          rz_d
  #define HDDM_VEC_PC          pc_d
  #define HDDM_DOM_VEC_VEC     vec_d
  #define HDDM_DOM_VEC_PC      pc_d
  #define GATHER_VEC           gather_vec_d
#endif /* __ADVFW_MATRIX_SYSTEM_COMPLEX__ */


#ifdef __ADVFW_MATRIX_SYSTEM_REAL__
  #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ /* real symmetric */
    #define __HDDM_SOLVER__                hddm_solver_RS
    #define __HDDM_SOLVER_PARENT__         hddm_solver_RS_Parent
    #define __HDDM_SOLVER_CHILD__          hddm_solver_RS_Child
    #define __HDDM_SOLVER_MAKE_HDDM_VEC__  hddm_solver_RS_make_HDDM_VEC
    #define __HDDM_SOLVER_PARENT_LOOP_1ST__     hddm_solver_RS_Parent_loop_1st
    #define __HDDM_SOLVER_PARENT_LOOP_RESTART__ hddm_solver_RS_Parent_loop_restart
    #define __HDDM_SOLVER_PARENT_LOOP__         hddm_solver_RS_Parent_loop
    #define __HDDM_SOLVER_PARENT_LOOP_LAST__    hddm_solver_RS_Parent_loop_last
    #define __HDDM_SOLVER_CHILD_LOOP_1ST__     hddm_solver_RS_Child_loop_1st
    #define __HDDM_SOLVER_CHILD_LOOP_RESTART__ hddm_solver_RS_Child_loop_restart
    #define __HDDM_SOLVER_CHILD_LOOP__         hddm_solver_RS_Child_loop
    #define __HDDM_SOLVER_CHILD_LOOP_LAST__    hddm_solver_RS_Child_loop_last
    #define __HDDM_SOLVER_CHANGE_PARENT_H__ hddm_solver_RS_change_parent_h
    #define __HDDM_SOLVER_COM_RES_H_RECV_RES_1ST__  hddm_solver_RS_COM_res_h_recv_res_1st
    #define __HDDM_SOLVER_COM_RES_H_SEND_RES_1ST__  hddm_solver_RS_COM_res_h_send_res_1st
    #define __HDDM_SOLVER_COM_RES_H_RECV_RES__      hddm_solver_RS_COM_res_h_recv_res
    #define __HDDM_SOLVER_COM_RES_H_SEND_RES__      hddm_solver_RS_COM_res_h_send_res
    #define __HDDM_SOLVER_COM_RES_H_RECV_RES_FUNC__ hddm_solver_RS_COM_res_h_recv_res_func
    #define __HDDM_SOLVER_COM_RES_H_SEND_RES_FUNC__ hddm_sovler_RS_COM_res_h_send_res_func
    #define __HDDM_SOLVER_RENEWAL_BC_H_SEND_DOM__ hddm_solver_RS_renewal_bc_h_send_dom
    #define __HDDM_SOLVER_RENEWAL_BC_H_RECV_DOM__ hddm_solver_RS_renewal_bc_h_recv_dom
    #define __HDDM_SOLVER_SOLVE_DOM_1ST__  hddm_solver_RS_solve_dom_1st
    #define __HDDM_SOLVER_SOLVE_DOM__      hddm_solver_RS_solve_dom
    #define __HDDM_SOLVER_SOLVE_DOM_LAST__ hddm_solver_RS_solve_dom_last
    #define __HDDM_SOLVER_COM_RES_SP_1ST__ hddm_solver_RS_COM_res_sp_1st
    #define __HDDM_SOLVER_COM_RES_SP__     hddm_solver_RS_COM_res_sp
    #define __HDDM_SOLVER_COM_RES_SP_FUNC__ hddm_solver_RS_COM_res_sp_func
    #define __HDDM_SOLVER_RENEWAL_BC_SP__  hddm_solver_RS_renewal_bc_sp
    #define __HDDM_SOLVER_ITERATION_S_1ST__ hddm_solver_RS_iteration_s_1st
    #define __HDDM_SOLVER_ITERATION_S__    hddm_solver_RS_iteration_s
    #define __HDDM_SOLVER_ITERATION_S_GATHER__ hddm_solver_RS_iteration_s_gather
    #define __HDDM_SOLVER_ITERATION_S_FREE_OOP__ hddm_solver_RS_iteration_s_free_oop
    #define __HDDM_SOLVER_ITERATION_PH_1ST__ hddm_solver_RS_iteration_ph_1st
    #define __HDDM_SOLVER_ITERATION_PH__   hddm_solver_RS_iteration_ph
    #define __HDDM_SOLVER_ITERATION_PH_GATHER__ hddm_solver_RS_iteration_ph_gather
    #define __HDDM_SOLVER_ITERATION_PH_RENEWAL__ hddm_solver_RS_iteration_ph_renewal
    #define __HDDM_PC_NONE_MAKE__          hddm_pc_RS_NONE_make
    #define __HDDM_PC_NONE_SET__           hddm_pc_RS_NONE_set
    #define __HDDM_PC_NONE_GATHER_S__      hddm_pc_RS_NONE_gather_s
    #define __HDDM_PC_NONE_GATHER_PH__     hddm_pc_RS_NONE_gather_ph
    #define __HDDM_PC_DIAG_MAKE__          hddm_pc_RS_DIAG_make
    #define __HDDM_PC_DIAG_SET__           hddm_pc_RS_DIAG_set
    #define __HDDM_PC_DIAG_GATHER_S__      hddm_pc_RS_DIAG_gather_s
    #define __HDDM_PC_DIAG_GATHER_PH__     hddm_pc_RS_DIAG_gather_ph


  #else                                     /* real asymmetric */
    #define __HDDM_SOLVER__                hddm_solver_RA
    #define __HDDM_SOLVER_PARENT__         hddm_solver_RA_Parent
    #define __HDDM_SOLVER_CHILD__          hddm_solver_RA_Child
    #define __HDDM_SOLVER_MAKE_HDDM_VEC__  hddm_solver_RA_make_HDDM_VEC
    #define __HDDM_SOLVER_PARENT_LOOP_1ST__     hddm_solver_RA_Parent_loop_1st
    #define __HDDM_SOLVER_PARENT_LOOP_RESTART__ hddm_solver_RA_Parent_loop_restart
    #define __HDDM_SOLVER_PARENT_LOOP__         hddm_solver_RA_Parent_loop
    #define __HDDM_SOLVER_PARENT_LOOP_LAST__    hddm_solver_RA_Parent_loop_last
    #define __HDDM_SOLVER_SOLVE_DOM_1ST__  hddm_solver_RA_solve_dom_1st
    #define __HDDM_SOLVER_SOLVE_DOM__      hddm_solver_RA_solve_dom
    #define __HDDM_SOLVER_SOLVE_DOM_LAST__ hddm_solver_RA_solve_dom_last
    #define __HDDM_SOLVER_CHILD_LOOP_1ST__     hddm_solver_RA_Child_loop_1st
    #define __HDDM_SOLVER_CHILD_LOOP_RESTART__ hddm_solver_RA_Child_loop_restart
    #define __HDDM_SOLVER_CHILD_LOOP__         hddm_solver_RA_Child_loop
    #define __HDDM_SOLVER_CHILD_LOOP_LAST__    hddm_solver_RA_Child_loop_last
    #define __HDDM_SOLVER_CHANGE_PARENT_H__ hddm_solver_RA_change_parent_h
    #define __HDDM_SOLVER_COM_RES_H_RECV_RES_1ST__  hddm_solver_RA_COM_res_h_recv_res_1st
    #define __HDDM_SOLVER_COM_RES_H_SEND_RES_1ST__  hddm_solver_RA_COM_res_h_send_res_1st
    #define __HDDM_SOLVER_COM_RES_H_RECV_RES__      hddm_solver_RA_COM_res_h_recv_res
    #define __HDDM_SOLVER_COM_RES_H_SEND_RES__      hddm_solver_RA_COM_res_h_send_res
    #define __HDDM_SOLVER_COM_RES_H_RECV_RES_FUNC__ hddm_solver_RA_COM_res_h_recv_res_func
    #define __HDDM_SOLVER_COM_RES_H_SEND_RES_FUNC__ hddm_sovler_RA_COM_res_h_send_res_func
    #define __HDDM_SOLVER_RENEWAL_BC_H_SEND_DOM__ hddm_solver_RA_renewal_bc_h_send_dom
    #define __HDDM_SOLVER_RENEWAL_BC_H_RECV_DOM__ hddm_solver_RA_renewal_bc_h_recv_dom
    #define __HDDM_SOLVER_COM_RES_SP_1ST__ hddm_solver_RA_COM_res_sp_1st
    #define __HDDM_SOLVER_COM_RES_SP__     hddm_solver_RA_COM_res_sp
    #define __HDDM_SOLVER_RENEWAL_BC_SP__  hddm_solver_RA_renewal_bc_sp
    #define __HDDM_SOLVER_COM_RES_SP_FUNC__ hddm_solver_RA_COM_res_sp_func
    #define __HDDM_SOLVER_ITERATION_S_1ST__ hddm_solver_RA_iteration_s_1st
    #define __HDDM_SOLVER_ITERATION_S__    hddm_solver_RA_iteration_s
    #define __HDDM_SOLVER_ITERATION_S_GATHER__ hddm_solver_RA_iteration_s_gather
    #define __HDDM_SOLVER_ITERATION_S_FREE_OOP__ hddm_solver_RA_iteration_s_free_oop
    #define __HDDM_SOLVER_ITERATION_PH_1ST__ hddm_solver_RA_iteration_ph_1st
    #define __HDDM_SOLVER_ITERATION_PH__   hddm_solver_RA_iteration_ph
    #define __HDDM_SOLVER_ITERATION_PH_GATHER__ hddm_solver_RA_iteration_ph_gather
    #define __HDDM_SOLVER_ITERATION_PH_RENEWAL__ hddm_solver_RA_iteration_ph_renewal
    #define __HDDM_PC_NONE_MAKE__          hddm_pc_RA_NONE_make
    #define __HDDM_PC_NONE_SET__           hddm_pc_RA_NONE_set
    #define __HDDM_PC_NONE_GATHER_S__      hddm_pc_RA_NONE_gather_s
    #define __HDDM_PC_NONE_GATHER_PH__     hddm_pc_RA_NONE_gather_ph
    #define __HDDM_PC_DIAG_MAKE__          hddm_pc_RA_DIAG_make
    #define __HDDM_PC_DIAG_SET__           hddm_pc_RA_DIAG_set
    #define __HDDM_PC_DIAG_GATHER_S__      hddm_pc_RA_DIAG_gather_s
    #define __HDDM_PC_DIAG_GATHER_PH__     hddm_pc_RA_DIAG_gather_ph


  #endif
#else
  #ifdef __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ /* complex symmetric */
    #define __HDDM_SOLVER__                hddm_solver_CS
    #define __HDDM_SOLVER_PARENT__         hddm_solver_CS_Parent
    #define __HDDM_SOLVER_CHILD__          hddm_solver_CS_Child
    #define __HDDM_SOLVER_MAKE_HDDM_VEC__  hddm_solver_CS_make_HDDM_VEC
    #define __HDDM_SOLVER_PARENT_LOOP_1ST__     hddm_solver_CS_Parent_loop_1st
    #define __HDDM_SOLVER_PARENT_LOOP_RESTART__ hddm_solver_CS_Parent_loop_restart
    #define __HDDM_SOLVER_PARENT_LOOP__         hddm_solver_CS_Parent_loop
    #define __HDDM_SOLVER_PARENT_LOOP_LAST__    hddm_solver_CS_Parent_loop_last
    #define __HDDM_SOLVER_CHILD_LOOP_1ST__     hddm_solver_CS_Child_loop_1st
    #define __HDDM_SOLVER_CHILD_LOOP_RESTART__ hddm_solver_CS_Child_loop_restart
    #define __HDDM_SOLVER_CHILD_LOOP__         hddm_solver_CS_Child_loop
    #define __HDDM_SOLVER_CHILD_LOOP_LAST__    hddm_solver_CS_Child_loop_last
    #define __HDDM_SOLVER_CHANGE_PARENT_H__ hddm_solver_CS_change_parent_h
    #define __HDDM_SOLVER_COM_RES_H_RECV_RES_1ST__  hddm_solver_CS_COM_res_h_recv_res_1st
    #define __HDDM_SOLVER_COM_RES_H_SEND_RES_1ST__  hddm_solver_CS_COM_res_h_send_res_1st
    #define __HDDM_SOLVER_COM_RES_H_RECV_RES__      hddm_solver_CS_COM_res_h_recv_res
    #define __HDDM_SOLVER_COM_RES_H_SEND_RES__      hddm_solver_CS_COM_res_h_send_res
    #define __HDDM_SOLVER_COM_RES_H_RECV_RES_FUNC__ hddm_solver_CS_COM_res_h_recv_res_func
    #define __HDDM_SOLVER_COM_RES_H_SEND_RES_FUNC__ hddm_sovler_CS_COM_res_h_send_res_func
    #define __HDDM_SOLVER_RENEWAL_BC_H_SEND_DOM__ hddm_solver_CS_renewal_bc_h_send_dom
    #define __HDDM_SOLVER_RENEWAL_BC_H_RECV_DOM__ hddm_solver_CS_renewal_bc_h_recv_dom

    #define __HDDM_SOLVER_SOLVE_DOM_1ST__  hddm_solver_CS_solve_dom_1st
    #define __HDDM_SOLVER_SOLVE_DOM__      hddm_solver_CS_solve_dom
    #define __HDDM_SOLVER_SOLVE_DOM_LAST__ hddm_solver_CS_solve_dom_last

    #define __HDDM_SOLVER_SOLVE_DOM_1ST_D__  hddm_solver_CS_solve_dom_1st_d
    #define __HDDM_SOLVER_SOLVE_DOM_D__      hddm_solver_CS_solve_dom_d
    #define __HDDM_SOLVER_SOLVE_DOM_LAST_D__ hddm_solver_CS_solve_dom_last_d

    #define __HDDM_SOLVER_COM_RES_SP_1ST__ hddm_solver_CS_COM_res_sp_1st
    #define __HDDM_SOLVER_COM_RES_SP__     hddm_solver_CS_COM_res_sp
    #define __HDDM_SOLVER_COM_RES_SP_FUNC__ hddm_solver_CS_COM_res_sp_func
    #define __HDDM_SOLVER_RENEWAL_BC_SP__  hddm_solver_CS_renewal_bc_sp
    #define __HDDM_SOLVER_ITERATION_S_1ST__ hddm_solver_CS_iteration_s_1st
    #define __HDDM_SOLVER_ITERATION_S__    hddm_solver_CS_iteration_s
    #define __HDDM_SOLVER_ITERATION_S_GATHER__ hddm_solver_CS_iteration_s_gather
    #define __HDDM_SOLVER_ITERATION_S_FREE_OOP__ hddm_solver_CS_iteration_s_free_oop
    #define __HDDM_SOLVER_ITERATION_PH_1ST__ hddm_solver_CS_iteration_ph_1st
    #define __HDDM_SOLVER_ITERATION_PH__   hddm_solver_CS_iteration_ph
    #define __HDDM_SOLVER_ITERATION_PH_GATHER__ hddm_solver_CS_iteration_ph_gather
    #define __HDDM_SOLVER_ITERATION_PH_RENEWAL__ hddm_solver_CS_iteration_ph_renewal
    #define __HDDM_PC_NONE_MAKE__          hddm_pc_CS_NONE_make
    #define __HDDM_PC_NONE_SET__           hddm_pc_CS_NONE_set
    #define __HDDM_PC_NONE_GATHER_S__      hddm_pc_CS_NONE_gather_s
    #define __HDDM_PC_NONE_GATHER_PH__     hddm_pc_CS_NONE_gather_ph
    #define __HDDM_PC_DIAG_MAKE__          hddm_pc_CS_DIAG_make
    #define __HDDM_PC_DIAG_SET__           hddm_pc_CS_DIAG_set
    #define __HDDM_PC_DIAG_GATHER_S__      hddm_pc_CS_DIAG_gather_s
    #define __HDDM_PC_DIAG_GATHER_PH__     hddm_pc_CS_DIAG_gather_ph


  #else                                     /* complex asymmetric */
    #define __HDDM_SOLVER__                hddm_solver_CA
    #define __HDDM_SOLVER_PARENT__         hddm_solver_CA_Parent
    #define __HDDM_SOLVER_CHILD__          hddm_solver_CA_Child
    #define __HDDM_SOLVER_MAKE_HDDM_VEC__  hddm_solver_CA_make_HDDM_VEC
    #define __HDDM_SOLVER_PARENT_LOOP_1ST__     hddm_solver_CA_Parent_loop_1st
    #define __HDDM_SOLVER_PARENT_LOOP_RESTART__ hddm_solver_CA_Parent_loop_restart
    #define __HDDM_SOLVER_PARENT_LOOP__         hddm_solver_CA_Parent_loop
    #define __HDDM_SOLVER_PARENT_LOOP_LAST__    hddm_solver_CA_Parent_loop_last
    #define __HDDM_SOLVER_CHILD_LOOP_1ST__     hddm_solver_CA_Child_loop_1st
    #define __HDDM_SOLVER_CHILD_LOOP_RESTART__ hddm_solver_CA_Child_loop_restart
    #define __HDDM_SOLVER_CHILD_LOOP__         hddm_solver_CA_Child_loop
    #define __HDDM_SOLVER_CHILD_LOOP_LAST__    hddm_solver_CA_Child_loop_last
    #define __HDDM_SOLVER_CHANGE_PARENT_H__ hddm_solver_CA_change_parent_h
    #define __HDDM_SOLVER_COM_RES_H_RECV_RES_1ST__  hddm_solver_CA_COM_res_h_recv_res_1st
    #define __HDDM_SOLVER_COM_RES_H_SEND_RES_1ST__  hddm_solver_CA_COM_res_h_send_res_1st
    #define __HDDM_SOLVER_COM_RES_H_RECV_RES__      hddm_solver_CA_COM_res_h_recv_res
    #define __HDDM_SOLVER_COM_RES_H_SEND_RES__      hddm_solver_CA_COM_res_h_send_res
    #define __HDDM_SOLVER_COM_RES_H_RECV_RES_FUNC__ hddm_solver_CA_COM_res_h_recv_res_func
    #define __HDDM_SOLVER_COM_RES_H_SEND_RES_FUNC__ hddm_sovler_CA_COM_res_h_send_res_func
    #define __HDDM_SOLVER_RENEWAL_BC_H_SEND_DOM__ hddm_solver_CA_renewal_bc_h_send_dom
    #define __HDDM_SOLVER_RENEWAL_BC_H_RECV_DOM__ hddm_solver_CA_renewal_bc_h_recv_dom
    #define __HDDM_SOLVER_SOLVE_DOM_1ST__  hddm_solver_CA_solve_dom_1st
    #define __HDDM_SOLVER_SOLVE_DOM__      hddm_solver_CA_solve_dom
    #define __HDDM_SOLVER_SOLVE_DOM_LAST__ hddm_solver_CA_solve_dom_last
    #define __HDDM_SOLVER_COM_RES_SP_1ST__ hddm_solver_CA_COM_res_sp_1st
    #define __HDDM_SOLVER_COM_RES_SP__     hddm_solver_CA_COM_res_sp
    #define __HDDM_SOLVER_COM_RES_SP_FUNC__ hddm_solver_CA_COM_res_sp_func
    #define __HDDM_SOLVER_RENEWAL_BC_SP__  hddm_solver_CA_renewal_bc_sp
    #define __HDDM_SOLVER_ITERATION_S_1ST__ hddm_solver_CA_iteration_s_1st
    #define __HDDM_SOLVER_ITERATION_S__    hddm_solver_CA_iteration_s
    #define __HDDM_SOLVER_ITERATION_S_GATHER__ hddm_solver_CA_iteration_s_gather
    #define __HDDM_SOLVER_ITERATION_S_FREE_OOP__ hddm_solver_CA_iteration_s_free_oop
    #define __HDDM_SOLVER_ITERATION_PH_1ST__ hddm_solver_CA_iteration_ph_1st
    #define __HDDM_SOLVER_ITERATION_PH__   hddm_solver_CA_iteration_ph
    #define __HDDM_SOLVER_ITERATION_PH_GATHER__ hddm_solver_CA_iteration_ph_gather
    #define __HDDM_SOLVER_ITERATION_PH_RENEWAL__ hddm_solver_CA_iteration_ph_renewal
    #define __HDDM_PC_NONE_MAKE__          hddm_pc_CA_NONE_make
    #define __HDDM_PC_NONE_SET__           hddm_pc_CA_NONE_set
    #define __HDDM_PC_NONE_GATHER_S__      hddm_pc_CA_NONE_gather_s
    #define __HDDM_PC_NONE_GATHER_PH__     hddm_pc_CA_NONE_gather_ph
    #define __HDDM_PC_DIAG_MAKE__          hddm_pc_CA_DIAG_make
    #define __HDDM_PC_DIAG_SET__           hddm_pc_CA_DIAG_set
    #define __HDDM_PC_DIAG_GATHER_S__      hddm_pc_CA_DIAG_gather_s
    #define __HDDM_PC_DIAG_GATHER_PH__     hddm_pc_CA_DIAG_gather_ph
  #endif
#endif

#endif /* __MTYPE_H__ */
