/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "hddm_solver_rs.h"

extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;

extern int nstart ;
extern int nend ;
extern int nrepeat ;

extern HDDM_SW hddm_sw ;
extern WORK_TIMES work_times ;
extern double s_time ;


#define __ADVFW_MATRIX_SYSTEM_REAL__      /* real */
#define __ADVFW_MATRIX_SYSTEM_SYMMETRIC__ /* symmetric */


#include "mtype.h"
#include "hddm_solver.h"
