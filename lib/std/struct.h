/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef  __STRUCT_H__
#define  __STRUCT_H__

#include "advfullwave_lib.h"


/** init or free structure **/
/* FaceGroup */
void init_FaceGroup( FaceGroup *fgr ) ;
void free_FaceGroup( FaceGroup *fgr ) ;
/* MTRL_ANTENNA_DEF_PP */
void init_MTRL_ANTENNA_DEF_PP( MTRL_ANTENNA_DEF_PP *pp ) ;
void free_MTRL_ANTENNA_DEF_PP( MTRL_ANTENNA_DEF_PP *pp ) ;
void Copy_MTRL_ANTENNA_DEF_PP( MTRL_ANTENNA_DEF_PP *a, MTRL_ANTENNA_DEF_PP b ) ;
void Bcast_Grp_MTRL_ANTENNA_DEF_PP( MTRL_ANTENNA_DEF_PP *pp ) ;
void Bcast_MTRL_ANTENNA_DEF_PP( MTRL_ANTENNA_DEF_PP *pp ) ;
/* MTRL_ANTENNA_DEF_DSC */
void init_MTRL_ANTENNA_DEF_DSC( MTRL_ANTENNA_DEF_DSC *dsc ) ;
void free_MTRL_ANTENNA_DEF_DSC( MTRL_ANTENNA_DEF_DSC *dsc ) ;
void Copy_MTRL_ANTENNA_DEF_DSC( MTRL_ANTENNA_DEF_DSC *a, MTRL_ANTENNA_DEF_DSC b ) ;
void Bcast_Grp_MTRL_ANTENNA_DEF_DSC( MTRL_ANTENNA_DEF_DSC *dsc ) ;
void Bcast_MTRL_ANTENNA_DEF_DSC( MTRL_ANTENNA_DEF_DSC *dsc ) ;
/* MTRL_ANTENNA_DEFINITION */
void init_MTRL_ANTENNA_DEFINITION( MTRL_ANTENNA_DEFINITION *antenna_def ) ;
void free_MTRL_ANTENNA_DEFINITION( MTRL_ANTENNA_DEFINITION *antenna_def ) ;
void Copy_MTRL_ANTENNA_DEFINITION( MTRL_ANTENNA_DEFINITION *a,
                                MTRL_ANTENNA_DEFINITION b ) ;
void Bcast_Grp_MTRL_ANTENNA_DEFINITION( MTRL_ANTENNA_DEFINITION *antenna_def ) ;
void Bcast_MTRL_ANTENNA_DEFINITION( MTRL_ANTENNA_DEFINITION *antenna_def ) ;

/* MTRL */
void init_MTRL( MTRL *mtrl ) ;
void free_MTRL( MTRL *mtrl ) ;
void Copy_MTRL( MTRL *a, MTRL b ) ;
void Bcast_Grp_MTRL( MTRL *mtrl ) ;
void Bcast_MTRL( MTRL *mtrl ) ;
/* DirichletBC */
void init_DirichletBC( DirichletBC *dbc ) ;
void free_DirichletBC( DirichletBC *dbc ) ;
void Bcast_Grp_DirichletBC( DirichletBC *dbc ) ;
void Bcast_DirichletBC( DirichletBC *dbc ) ;
void send_DirichletBC( DirichletBC *dbc, OPTIONS options,
                       int compid, int type ) ;
void recv_DirichletBC( DirichletBC *dbc, OPTIONS options,
                       int compid, int type ) ;
/* ANTENNA */
void init_ANTENNA( ANTENNA *antenna ) ;
void free_ANTENNA( ANTENNA *antenna, int nap, int dimension ) ;
void send_ANTENNA( ANTENNA *antenna, OPTIONS options, int nap, int dimension,
                int compid, int type ) ;
void recv_ANTENNA( ANTENNA *antenna, OPTIONS options, int nap, int dimension,
                int compid, int type ) ;
/* DomANTENNA */
void init_DomANTENNA( DomANTENNA *dantenna ) ;
void free_DomANTENNA( DomANTENNA *dantenna, int dimension ) ;
/* PartANTENNA */
void init_PartANTENNA( PartANTENNA *pantenna ) ;
void free_PartANTENNA( PartANTENNA *pantenna, int dimension ) ;
/* ResultValue */
void init_ResultValue( ResultValue *rv, OPTIONS options ) ;
void free_ResultValue( ResultValue *rv, int nel, int nnd, int dimension,
                       OPTIONS options ) ;
void send_ResultValue( ResultValue *rv, OPTIONS options,
                       int nel, int nap, int dimension,
                       int compid, int type ) ;
void recv_ResultValue( ResultValue *rv, OPTIONS options,
                       int nel, int nap, int dimension,
                       int compid, int type ) ;
/* MESH */
void init_MESH( MESH *mesh, OPTIONS options ) ;
void free_MESH( MESH *mesh, OPTIONS options ) ;
/* Inbc */
void init_Inbc( Inbc *inbc ) ;
void free_Inbc( Inbc *inbc ) ;
void Copy_Inbc( Inbc *a, Inbc b ) ;
void send_Inbc( Inbc *inbc, OPTIONS options, int compid, int type ) ;
void recv_Inbc( Inbc *inbc, OPTIONS options, int compid, int type ) ;
/* DomMesh */
void init_DomMesh( DomMesh *dm, OPTIONS options ) ;
void free_DomMesh( DomMesh *dm, MTRL mtrl, OPTIONS options ) ;
void send_DomMesh( DomMesh *dm, MTRL mtrl, OPTIONS options,
                   int compid, int type ) ;
void recv_DomMesh( DomMesh *dm, MTRL mtrl, OPTIONS options,
                   int compid, int type ) ;
/* OPinfo */
void init_OPinfo( OPinfo *op ) ;
void free_OPinfo( OPinfo *op ) ;
void Copy_OPinfo( OPinfo *a, OPinfo b ) ;
/* Pinbc */
void init_Pinbc( Pinbc *pinbc ) ;
void free_Pinbc( Pinbc *pinbc ) ;
/* HDDM_VEC */
void init_HDDM_VEC( HDDM_VEC *hddm_vec ) ;
void free_HDDM_VEC( HDDM_VEC *hddm_vec ) ;
/* PartMesh */
void init_PartMesh( PartMesh *pm, OPTIONS options ) ;
void free_PartMesh( PartMesh *pm, OPTIONS options ) ;
/* HDDM_DOM_VEC */
void init_HDDM_DOM_VEC( HDDM_DOM_VEC *hddm_dom_vec ) ;
void free_HDDM_DOM_VEC( HDDM_DOM_VEC *hddm_dom_vec ) ;

#endif /* __STRUCT_H__ */
