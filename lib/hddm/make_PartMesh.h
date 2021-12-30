/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 Copyright (C) 2003, 2005, 2007, 2014, 2015, 2016, 2017, 2018, 2019
               ADVENTURE Project,
 All Rights Reserved
*********************************************************************/

#ifndef __MAKE_PARTMESH_H__
#define __MAKE_PARTMESH_H__


#include "libhddm.h"

#ifdef __ADVFW_SINGLE_MODE__ /* s-mode */
  #include "make_PartMesh_s.h"
#else
  #ifdef __ADVFW_PARALLEL_MODE__ /* p-mode */
    #include "make_PartMesh_p.h"
  #else /* h-mode */
    #include "make_PartMesh_h.h"
  #endif /* __ADVFW_PARALLEL_MODE__ */
#endif /* __ADVFW_SINGLE_MODE__ */


void copy(DirichletBC *a, DirichletBC b) ;


/*** Initialize ***/
void hddm_time_Init( void ) ;
void hddm_time_Finalize( void ) ;
void hddm_Init( PartMesh **pm, OPTIONS options ) ;



/*** make PartMesh ***/
void make_PartMesh( PartMesh *pm, OPTIONS options ) ;
void make_PartMesh_check_flag( PartMesh *pm, OPTIONS options ) ;


/** MPC **/
void make_PartMesh_add_innerbc_MPC( PartMesh *pm, OPTIONS options ) ;


/** change inner B.C. **/
void make_PartMesh_change_innerbc_func( int *infree, int *outfree,
                                        OPinfo **op,
                                        int **ninbc, Inbc ***inbc,
                                        PartMesh *pm, OPTIONS options,
                                        void (*make_nf)( int  **nf,
                                                         DomMesh dm,
                                                         MTRL mtrl,
                                                         int kk ),
                                        int kk ) ;
void make_PartMesh_change_innerbc_make_temp_struct( PartMesh *pm,
                                                    int **infree,
                                                    int **outfree,
                                                    OPinfo ***op,
                                                    int ***ninbc,
                                                    Inbc ****inbc ) ;
void make_PartMesh_change_innerbc_make_temp_struct_sub( PartMesh pm,
                                                        int *infree,
                                                        int *outfree,
                                                        OPinfo **op,
                                                        int **ninbc,
                                                        Inbc ***inbc ) ;
void make_PartMesh_change_innerbc_free_temp_struct( PartMesh pm,
                                                    int *infree,
                                                    int *outfree,
                                                    OPinfo **op,
                                                    int **ninbc,
                                                    Inbc ***inbc ) ;
void make_PartMesh_change_innerbc_add_nf( Pinbc *pinbc, int tfree,
                                          int *nf, int nnd,
                                          int ninbc, Inbc *inbc ) ;
void make_PartMesh_change_innerbc_change_Pinbc_1st( PartMesh *pm,
                                                    int **ninbc, Inbc ***inbc,
                                                    Pinbc **pinbc ) ;
void make_PartMesh_change_innerbc_change_Pinbc_2nd( PartMesh *pm,
                                                    int *tfree,
                                                    OPinfo **op,
                                                    Pinbc **pinbc ) ;
void make_PartMesh_change_innerbc_change_Pinbc_2nd_sub( PartMesh pm,
                                                        int tfree,
                                                        OPinfo *op,
                                                        Pinbc *pinbc ) ;
void make_PartMesh_change_innerbc_change_Pinbc_3rd( int *tfree,
                                                    Pinbc **pinbc ) ;
void make_PartMesh_change_innerbc_change_Pinbc_4th( PartMesh *pm,
                                                    int *tfree,
                                                    int *infree,
                                                    int *outfree,
                                                    Pinbc **pinbc ) ;
void make_PartMesh_change_innerbc_change_Pinbc_5th( PartMesh *pm,
                                                    int *tfree,
                                                    OPinfo **op,
                                                    Pinbc **pinbc ) ;
void make_PartMesh_change_innerbc_change_Inbc( Pinbc *pinbc, int tfree,
                                               int *nf,
                                               int *ninbc, Inbc **inbc ) ;
void make_PartMesh_change_innerbc_change_OPinfo( Pinbc *pinbc, int tfree,
                                                 int infree, int outfree,
                                                 int partid, OPinfo **op ) ;


/** change PartMesh **/
void make_PartMesh_make_ap( int *nap, int **ap,
                            int nel, int nnd, int *nop,
                            int ap_elem, int nd_elem ) ;
double* make_PartMesh_change_crd( int nnd, int dimension,
                                  double *part_crd, int *snid2pnid,
                                  int nap, int *ap ) ;
void make_PartMesh_make_apid2phiid( int *nphi, int **apid2phiid,
                                    int nel, int nap, int nmp,
                                     int *nop, int *flag, int *ap,
                                    int ap_elem, int nd_elem,
                                    MTRL mtrl ) ;
void make_PartMesh_make_apid2phiid_with_Disp( int *nphi, int **apid2phiid,
                                              int nap, int nmp ) ;
void make_PartMesh_make_apid2snid( int **apid2snid,
                                   int nnd, int nap, int *ap ) ;
void make_PartMesh_make_mpid2snid( int **mpid2snid,
                                   int nnd, int nap, int nmp, int nphi,
                                   int *ap, int *apid2phiid, int *apid2snid ) ;
void make_PartMesh_change_dbc_Axn0_1st( DirichletBC *dbc_Axn0_tmp,
                                        DirichletBC *dbc_Axn0,
                                        int *ap ) ;

void make_PartMesh_make_ANTENNA_make_enf( ANTENNA *antenna,
                                       int nel, int *nop, int *flag,
                                       int ap_elem, int nd_elem,
                                       int nap, int *ap,
                                       int *apid2snid, int antenna_flag ) ;
void make_PartMesh_make_ANTENNA_change_bc( ANTENNA *antenna, DirichletBC dbc_tmp ) ;
void make_PartMesh_make_ANTENNA_add_inbc( ANTENNA *antenna,
                                       int nnd, int nap,
                                       int ninbc, Inbc *inbc ) ;
void make_PartMesh_change_ap( int *ap, int nnd ) ;
void make_PartMesh_change_nop( int nel, int nd_elem, int *nop,
                               int *ap, char *elem_type, OPTIONS options ) ;
void make_PartMesh_change_dbc_Axn0_2nd( DirichletBC *dbc_Axn0_tmp,
                                        DirichletBC *dbc_Axn0,
                                        int *ap, int *apid2phiid,
                                        int *nbc_A, OPTIONS options ) ;
void make_PartMesh_change_dbc_Axn0_3rd( DirichletBC *dbc,
                                        int nnd, int nap, int nmp, int nphi,
                                        int ninbc, Inbc *inbc,
                                        int *apid2phiid, int *apid2snid,
                                        int *mpid2snid ) ;
void make_PartMesh_sort_inbc( int ninbc, Inbc *inbc ) ;


/** Mangetic Field **/
void make_PartMesh_FW( PartMesh *pm, OPTIONS options ) ;

/* change inner B.C. */
void make_PartMesh_FW_change_innerbc( PartMesh *pm,
                                       OPTIONS options ) ;
void make_PartMesh_FW_change_innerbc_ANTENNA( PartMesh *pm,
                                            OPTIONS options ) ;
void make_PartMesh_FW_change_innerbc_ResultValue( PartMesh *pm,
                                                   OPTIONS options ) ;
void make_PartMesh_FW_change_innerbc_whole( PartMesh *pm,
                                             OPTIONS options ) ;
void make_PartMesh_FW_change_innerbc_ANTENNA_make_nf( int **nf, DomMesh dm,
                                                    MTRL mtrl, int kk ) ;
void make_PartMesh_FW_change_innerbc_ResultValue_make_nf( int **nf,
                                                           DomMesh dm,
                                                           MTRL mtrl,
                                                           int kk ) ;

void make_PartMesh_FW_change_innerbc_make_nf_A_Method( int **nf, DomMesh dm,
                                                        MTRL mtrl, int kk ) ;
#define make_PartMesh_FW_change_innerbc_make_nf_A_Method_Disp make_PartMesh_FW_change_innerbc_make_nf_A_Method
#define make_PartMesh_FW_change_innerbc_make_nf_A_Method_Perturbation make_PartMesh_FW_change_innerbc_make_nf_A_Method

/* change PartMesh **/
void make_PartMesh_FW_TetQ( PartMesh *pm, OPTIONS options ) ;

#endif /* __MAKE_PARTMESH_H__ */
