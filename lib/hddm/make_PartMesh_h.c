/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "make_PartMesh_h.h"


extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;

extern int nstart ;
extern int nend ;
extern int nrepeat ;

extern HDDM_SW hddm_sw ;
extern WORK_TIMES work_times ;
extern double s_time ;


/*** make PartMesh ***/
#define __MAKE_PARTMESH_CHECK_FLAG_PH__ make_PartMesh_check_flag_h


/** Mangetic Field **/



/*** change inner B.C. ***/
/** common **/
#define __MAKE_PARTMESH_CHANGE_INNERBC_CHANGE_PINBC_2ND_PH__ make_PartMesh_change_innerbc_change_Pinbc_2nd_h
#define __MAKE_PARTMESH_CHANGE_INNERBC_CHANGE_PINBC_5TH_PH__ make_PartMesh_change_innerbc_change_Pinbc_5th_h


#include "make_PartMesh_ph.h"


/*void make_PartMesh_check_flag_h( PartMesh *pm, OPTIONS options )
{*/
  /*+ Function: check flag for PartMesh +*/
/*}*/

/********************************************************************/
