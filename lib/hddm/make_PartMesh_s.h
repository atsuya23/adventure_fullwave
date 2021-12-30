/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __MAKE_PARTMESH_S_H__
#define __MAKE_PARTMESH_S_H__


#include "libhddm.h"


/*** make PartMesh ***/
void make_PartMesh_check_flag_s( PartMesh *pm, OPTIONS options ) ;


/** Mangetic Field **/



/*** change inner B.C. ***/
/** common **/
void make_PartMesh_change_innerbc_change_Pinbc_2nd_s( PartMesh *pm,
                                                      int *tfree,
                                                      OPinfo **op,
                                                      Pinbc **pinbc ) ;
void make_PartMesh_change_innerbc_change_Pinbc_5th_s( OPinfo **op,
                                                      Pinbc **pinbc ) ;

#endif /* __MAKE_PARTMESH_S_H__ */
