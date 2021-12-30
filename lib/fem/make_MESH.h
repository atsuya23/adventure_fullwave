/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __MAKE_MESH_H__
#define __MAKE_MESH_H__


#include "libfem.h"


/*** make MESH ***/
void make_MESH( MESH *mesh, OPTIONS options ) ;
void make_MESH_check_flag( int nel, int *flag, MTRL mtrl,
                           OPTIONS options ) ;
void make_MESH_check_flag_1st( int nel, int *flag,
                               int *nflag, int **flag_i ) ;
void make_MESH_check_flag_2nd( int nflag, int *flag_i, MTRL mtrl,
                               OPTIONS options ) ;


/** Magnetic Field **/
void make_MESH_FW( MESH *mesh, OPTIONS options ) ;
void make_MESH_FW_TetQ( MESH *mesh, OPTIONS options ) ;
void make_MESH_make_ap( int *nap, int **ap,
                        int nel, int nnd, int *nop,
                        int ap_elem, int nd_elem ) ;
void make_MESH_change_crd( int nnd, int dimension, double **crd,
                           int nap, int *ap ) ;

void make_MESH_make_apid2phiid_with_Disp( int *nphi, int **apid2phiid,
                                          int nap, int nmp ) ;



void make_MESH_make_apid2phiid( int *nphi, int **apid2phiid,
                                int nel, int nap, int nmp,
                                int *nop, int *flag, int *ap,
                                int ap_elem, int nd_elem,
                                MTRL mtrl ) ;
void make_MESH_make_apid2gnid( int **apid2gnid,
                               int nnd, int nap, int *ap ) ;
void make_MESH_make_mpid2gnid( int **mpid2gnid,
                               int nnd, int nap, int nmp, int nphi,
                               int *ap, int *apid2phiid, int *apid2gnid ) ;
void make_MESH_change_dbc_Axn0_1st( DirichletBC *dbc_tmp_Axn0,
                                    DirichletBC *dbc_Axn0,
                                    int *ap ) ;
void make_MESH_make_ANTENNA( ANTENNA **antenna,
                          int nel, int *nop, int *flag,
                          int ap_elem, int nd_elem,
                          int nap, int *ap, DirichletBC dbc_tmp,
                          int *apid2gnid, MTRL mtrl ) ;
void make_MESH_make_ANTENNA_make_enf( ANTENNA *antenna,
                                   int nel, int *nop, int *flag,
                                   int ap_elem, int nd_elem,
                                   int nap, int *ap,
                                   int *apid2gnid, int antenna_flag ) ;
void make_MESH_make_ANTENNA_change_bc( ANTENNA *antenna, DirichletBC dbc_tmp ) ;
void make_MESH_change_ap( int *ap, int nnd ) ;
void make_MESH_change_nop( int nel, int nd_elem, int *nop,
                           int *ap, char *elem_type, OPTIONS options ) ;
void make_MESH_change_dbc_Axn0_2nd( DirichletBC *dbc_Axn0_tmp,
                                    DirichletBC *dbc_Axn0,
                                    int *ap, int *apid2phiid, int *nbc_A,
                                    OPTIONS options ) ;



void correct_Jo( int t_loop, int nel, int nap,
                 int *nop, double *crd, int *flag,
                 int ap_elem, int nd_elem, char *elem_type,
                 int dimension, ANTENNA *antenna, MTRL mtrl,
                 OPTIONS options ) ;

#endif /* __MAKE_MESH_H__ */
