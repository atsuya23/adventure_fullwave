/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __LIBHDDM_H__
#define __LIBHDDM_H__


#include "advfullwave_lib.h"
#include "mymt_aij_solver.h"
#include "libfem.h"

#include "make_PartMesh.h"
#include "hddm_solver_rs.h"
#include "hddm_solver_cs.h"
#include "set_hddm_func.h"


/* errormessage */
#define __ABORT_LIBHDDM__     4

/* change inner B.C. */
#define __ADVFW_HDDM_CHANGE_INNER_BC_NOT_CHANGE__ -1
#define __ADVFW_HDDM_CHANGE_INNER_BC_CHANGE__      1

#define __ADVFW_HDDM_BC_FLAG_ESSENTIAL_BC__    -2
#define __ADVFW_HDDM_BC_FLAG_NOT_CHANGE__      -1
#define __ADVFW_HDDM_BC_FLAG_NO_DOF__           0
#define __ADVFW_HDDM_BC_FLAG_INFREE__           1
#define __ADVFW_HDDM_BC_FLAG_OUTFREE__          2
#define __ADVFW_HDDM_BC_FLAG_ONLY_ONE__         3
#define __ADVFW_HDDM_BC_FLAG_ONLY_THIS_PART__   4

/* hddm solver */
#define __HDDM_SW_FIRST__        0
#define __HDDM_SW_ITERATION__    1
#define __HDDM_SW_LAST__         2
#define __HDDM_SW_END__          3
#define __HDDM_SW_RESTART__      4

#define __HDDM_COMM_SIG_NONE__          -1
#define __HDDM_COMM_SIG_SEND_DOM__      -2
#define __HDDM_COMM_SIG_RECV_DOM__      -3
#define __HDDM_COMM_SIG_SEND_RES__      -4
#define __HDDM_COMM_SIG_RECV_RES__      -5
#define __HDDM_COMM_SIG_BREAK__         -6
#define __HDDM_COMM_SIG_READY__         -7
#define __HDDM_COMM_SIG_FINISH__        -8
#define __HDDM_COMM_SIG_RECV_OK__       -9
#define __HDDM_COMM_SIG_SEND__          -10
#define __HDDM_COMM_SIG_HAVE_DOM__      -11
#define __HDDM_COMM_SIG_NOTHING_DOM__   -12
#define __HDDM_COMM_SIG_CHANGE_PARENT__ -13



#define __HDDM_WORK_SW_FIRST__            0
#define __HDDM_WORK_SW_MAKE_ELEM__        1
#define __HDDM_WORK_SW_MAKE_ELEM_B__      2

#endif /* __LIBHDDM_H__ */
