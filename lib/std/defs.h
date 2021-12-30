/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __DEFS_H__
#define __DEFS_H__


#include "advfullwave_lib.h"


/* Module information */
#define ADVFW_NAME          "ADVENTURE_FullWave"
#define ADVFW_VERSION       "1.0"
#define N_VERSION            1.0  /* version of current module */
#define OUT_VERSION          1.0  /* version of oldest
                                     support file format */


/* kind of Work */
#define WORK_PRE_MAKEMODEL          0
#define WORK_PRE_PATCHGENERATE      1
#define WORK_PRE_MESHGENERATE       2
#define WORK_PRE_BCTOOL             3
#define WORK_PRE_FILECONVERT        4
#define WORK_PRE_MAKEDATA           5
#define WORK_FEM_ANALYSIS           6
#define WORK_HDDM_METIS             7
#define WORK_HDDM_ANALYSIS          8
#define WORK_POST_MERGE             9
#define WORK_POST_FEM_CALC          10
#define WORK_POST_HDDM_CALC         11
#define WORK_POST_VISUALIZE         12


/* kind of Field */
#define FIELD_ELECTROMAGNETIC              5


/* kind of Analysis */
#define ANALYSIS_UNKNOWN             -2
#define ANALYSIS_ANOTHER             -1
#define ANALYSIS_FULL_WAVECURRENT 501
#define ANALYSIS_POST_FULL_WAVE   551



/* Formulation */
#define FORMULATION_UNKNOWN            0
#define FORMULATION_FW_EMETHOD        521

#define FORMULATION_FULL_WAVECURRENT FORMULATION_FW_EMETHOD
#define FORMULATION_FW_EMETHOD_STRING        "E"



/* File or Directory name */
#define ADVFILE_EXT                  ".adv"   /* Extention of ADV files */
#define DEFAULT_DATA_DIR             "data"
#define DEFAULT_FWDATA_DIR          "indata_one"
#define DEFAULT_FWDATA_FILE         "one_indata"
#define DEFAULT_ONEDATA_DIR          "model_one"
#define DEFAULT_ONEDATA_FILE         "input"
#define DEFAULT_MODELDATA_DIR        "model"
#define DEFAULT_MODELDATA_FILE       "advhddm_in"
#define DEFAULT_JOMETIS_DIR          DEFAULT_MODELDATA_DIR
#define DEFAULT_JOMETIS_FILE         "advhddm_Jo"
#define DEFAULT_INIDATA_DIR          DEFAULT_FWDATA_DIR
#define DEFAULT_INIDATA_FILE         "initial.dat"
#define DEFAULT_INIMETIS_DIR         DEFAULT_MODELDATA_DIR
#define DEFAULT_INIMETIS_FILE        "advhddm_Ini"
#define DEFAULT_OUTDATA_DIR          "result"
#define DEFAULT_OUTDATA_FILE         "advhddm_out"
#define DEFAULT_AVSFILE_DIR          "result"
#define DEFAULT_AVSFILE_FILE         "avs"
#define DEFAULT_VTKFILE_DIR          "result"
#define DEFAULT_VTKFILE_FILE         "res"
#define DEFAULT_CALCLOG_DIR          "calc_log"
#define DEFAULT_CALCLOG_FILE         "log_g"
#define DEFAULT_MTRLDAT_DIR          ""
#define DEFAULT_MTRLDAT_FILE         "mtrl.dat"

#define DEFAULT_INDATA_DIR           DEFAULT_MODELDATA_DIR
#define DEFAULT_INDATA_BH            "bh_curve"
#define DEFAULT_INDATA_IH            "ih.r"


/* MTRLDAT */
#define MTRL_PERMITTIVITY  "Permittivity"
#define MTRL_ANTENNA                  "Antenna"
#define MTRL_ANTENNA_READ_FROM_FILE   "rf"
#define MTRL_ANTENNA_RF_NUMBER        1
#define MTRL_ANTENNA_MAKE_FROM_DEFINE "md"
#define MTRL_ANTENNA_MD_NUMBER        2
#define MTRL_ANTENNA_FORM_PARALLELPIPED           "Parallelpiped"
#define MTRL_ANTENNA_FORM_DOUBLESECTORIALCYLINDER "DoubleSectorialCylinder"
#define MTRL_ANTENNA_ANALYSIS_FULL_WAVE "Full-Wave"
#define MTRL_ANTENNAOMEGA       "AntennaOmega"
#define MTRL_CONDUCTOR       "Conductor"


/* basic settings */
#define NEARLY_ZERO          1.0e-60

#define ASCII                0
#define BINARY               1

#define NO_NEED              -1
#define NEED                 0

#define NUMBER_REAL          1
#define NUMBER_COMPLEX       2

#define MEMLIMIT             256*1024*1024


/* Full-Wave settings */
#define ND_ELEM              10
#define ELEM_TYPE            "3DQuadraticTetrahedron"
#define ELEM_TYPE_3DLT       "3DLinearTetrahedron"
#define ELEM_TYPE_3DQT       "3DQuadraticTetrahedron"
#define ELEM_TYPE_3DLH       "3DLinearHexahedron"
#define ELEM_TYPE_3DQH       "3DQuadraticHexahedron"

#define DIMENSION            3

/* solver settings */
#define SOLVER_DIRECT           0
#define SOLVER_CGMETHOD         1
#define SOLVER_COCG             2
#define SOLVER_QMR              3
#define DEFAULT_SOLVER          -1
#define DEFAULT_COJO_SOL        -1

#define SOLVER_PC_NONE          0
#define SOLVER_PC_DIAG          1
#define SOLVER_PC_JACOBI        2
#define SOLVER_PC_ICC           3
#define SOLVER_PC_MRIC0         4
#define SOLVER_PC_ILDU          5
#define SOLVER_PC_SOR           6
#define SOLVER_LU               7
#define DEFAULT_SOLVER_PC       -1
#define DEFAULT_COJO_SOL_PC     -1
#define SOLVER_PC_NONE_STRING   "none"
#define SOLVER_PC_DIAG_STRING   "diag"
#define SOLVER_PC_JACOBI_STRING "jacobi"
#define SOLVER_PC_ICC_STRING    "ICC"
#define SOLVER_PC_MRIC0_STRING  "MRIC0"
#define SOLVER_PC_ILDU_STRING   "iLU"
#define SOLVER_PC_SOR_STRING    "SOR"
#define SOLVER_LU_STRING       "LU"

#define DEFAULT_SOLVER_EPS      1.0e-7
#define DEFAULT_SOLVER_DIV      1.0e+10
#define DEFAULT_SOLVER_IC_SHIFT 1.2

#define DEFAULT_COJO_SOL_EPS      1.0e-12
#define DEFAULT_COJO_SOL_DIV      1.0e+10
#define DEFAULT_COJO_SOL_IC_SHIFT DEFAULT_SOLVER_IC_SHIFT

#define SOLVER_STATUS_CONVERGED         -1
#define SOLVER_STATUS_DIVERGED          -2
#define SOLVER_STATUS_REACHED_MAX_ITE   -3
#define SOLVER_STATUS_UNKNOWN_PC        -4
#define SOLVER_STATUS_UNKNOWN_ERROR     -5
#define SOLVER_STATUS_GMRES_RESTART     -6

/* HDDM Solver settings */
#define HDDM_DIRECT             0
#define HDDM_CGMETHOD           1
#define HDDM_COCG               2
#define HDDM_QMR                3
#define DEFAULT_HDDM_SOLVER     -1
#define DEFAULT_COJO_HDDM_SOL   -1

#define HDDM_PC_NONE            0
#define HDDM_PC_DIAG            1
#define DEFAULT_HDDM_PC         -1
#define DEFAULT_COJO_HDDM_PC     -1
#define HDDM_PC_NONE_STRING     "none"
#define HDDM_PC_DIAG_STRING     "diag"

#define DEFAULT_HDDM_EPS      1.0e-3
#define DEFAULT_HDDM_EPS_NL   1.0e-5
#define DEFAULT_HDDM_NL_EPS   1.0e-3
#define DEFAULT_HDDM_DIV      1.0e+10
#define DEFAULT_HDDM_NL_DIV   1.0e+10
#define DEFAULT_HDDM_MAX_LOOP 4000

#define DEFAULT_COJO_HDDM_EPS      1.0e-10
#define DEFAULT_COJO_HDDM_DIV      1.0e+10
#define DEFAULT_COJO_HDDM_MAX_LOOP 4000


/* unit */
#define UNIT_A_m             0
#define UNIT_A_m2            1
#define UNIT_A_mm            10
#define UNIT_A_mm2           11
#define UNIT_G               200
#define UNIT_T               300

#define DEFAULT_UNIT_CURRENT_DENSITY  UNIT_A_m2
#define DEFAULT_UNIT_ELECTROMAGNETIC_FIELD UNIT_T

#define UNIT_A_m_STRING      "A/m"
#define UNIT_A_m2_STRING     "A/m^2"
#define UNIT_A_mm_STRING     "A/mm"
#define UNIT_A_mm2_STRING    "A/mm^2"
#define UNIT_G_STRING        "G"
#define UNIT_T_STRING        "T"



#define PI                   3.14159265358979323846
#define RAD_DEG( x )         ( (x) / PI * 180.0 )
#define DEG_RAD( x )         ( (x) * PI / 180.0 )

#endif /* __DEFS_H__ */
