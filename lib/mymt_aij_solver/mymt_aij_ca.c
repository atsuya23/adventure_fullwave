/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "mymt_aij_ca.h"


#define __ADVFW_MATRIX_SYSTEM_COMPLEX__    /* complex */
#define __ADVFW_MATRIX_SYSTEM_ASYMMETRIC__ /* symmetric */


#include "mtype.h"
#include "aij_matrix_func.h"
#include "aij_pc_func.h"
#include "aij_solver_func.h"
