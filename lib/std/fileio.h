/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef  __FILE_IO_H__
#define  __FILE_IO_H__

#include "advfullwave_lib.h"


/** read or write file **/
void read_matFile( char *fname, MTRL *mtrl, OPTIONS options ) ;
void read_cndFile( char *fname, MTRL *mtrl, OPTIONS options ) ;
void read_fgrFile( char *fname, FaceGroup *fgr, OPTIONS options ) ;
void read_mshFile( char *fname, MESH *mesh, FaceGroup fgr,
                   OPTIONS options ) ;
/* fwdata */
void read_fwdata_file( char *fname, MESH *mesh ) ;
void write_fwdata_file( char *fname, MESH mesh ) ;
/* onedata */
void read_onedata_file( char *fname, MESH *mesh ) ;
void write_onedata_file( char *fname, MESH mesh ) ;

void Bcast_Grp_onedata_file( MESH *mesh, OPTIONS options ) ;
void Bcast_onedata_file( MESH *mesh, OPTIONS options ) ;

/* modeldata */
int get_num_part_from_modeldata( char *fname ) ;
void read_modeldata_file( char *fname, PartMesh *pm, OPTIONS options ) ;

/* Jodata */
void read_Jodata_file( char *fname, int nap, int dimension,
                       ANTENNA *antenna, MTRL mtrl ) ;
void read_Jodata_file_read( char *fname,
                            int *nJo, int **iJo, double **Jo,
                            int dimension ) ;
void read_Jodata_file_decompose( int nap, int dimension,
                                 ANTENNA *antenna, MTRL mtrl,
                                 int nJo, int *iJo, double *Jo ) ;
void write_Jodata_file( char *fname, int nap, int dimension,
                        ANTENNA antenna, MTRL mtrl ) ;

/* Jodefine */
void read_Jodefine_file( char *fname, MTRL_ANTENNA_DEFINITION *antenna_def ) ;
void read_Jodefine_file_pp( FILE *fp, MTRL_ANTENNA_DEF_PP *pp,
                            char *analysis ) ;
void make_face_equation( double *p, double *q, double *ap,
                         double *abc, double *d ) ;
void make_face_equation_abc( double *p, double *q, double *abc ) ;
void make_face_equation_d( double *abc, double *ap, double *d ) ;
void make_OuterProduct( double *p, double *q, double *OuterProduct ) ;
void read_Jodefine_file_dsc( FILE *fp, MTRL_ANTENNA_DEF_DSC *dsc,
                             char *analysis ) ;
void make_Jo_from_definition( MTRL_ANTENNA_DEFINITION antenna_def, ANTENNA *antenna,
                              int nap, int dimension, double *crd,
                              int Jo_sw ) ;
void make_Jo_from_definition_pp( MTRL_ANTENNA_DEF_PP pp, ANTENNA *antenna,
                                 int nap, int dimension, double *crd ) ;
int make_Jo_from_definition_pp_confirm( double *ap, double *xyz,
                                        double tolerance,
                                        double *abc, double d ) ;
void make_Jo_from_definition_dsc( MTRL_ANTENNA_DEF_DSC dsc, ANTENNA *antenna,
                                  int nap, int dimension, double *crd ) ;
void write_Jodefine_file( char *fname, MTRL_ANTENNA_DEFINITION antenna_def ) ;
void write_Jodefine_file_pp( FILE *fp, MTRL_ANTENNA_DEF_PP pp ) ;
void write_Jodefine_file_dsc( FILE *fp, MTRL_ANTENNA_DEF_DSC dsc ) ;

/* Jometis */

/* inidata */
void read_Inidata_file( char *fname, MESH *mesh ) ;
void write_Inidata_file( char *fname, MESH mesh ) ;

/* inimetis */

/* outdata */
void read_outdata_file_FEM( char *fname, int t_loop,
                            MESH *mesh, OPTIONS *options ) ;
void read_outdata_file_FEM_FW( char *fname, int t_loop,
                                MESH *mesh, OPTIONS *options ) ;
void read_outdata_file_HDDM_temp( char *fname, int t_loop,
                                  PartMesh *pm, OPTIONS *options ) ;
void read_outdata_file_HDDM_FW_temp( char *fname, int t_loop,
                                      PartMesh *pm, OPTIONS *options ) ;
void read_outdata_file_options_FW( AdvDatabox *dbox, int *t_loop,
                                    OPTIONS *options ) ;
void write_outdata_file_FEM( char *fname, int t_loop,
                             MESH mesh, OPTIONS options ) ;
void write_outdata_file_FEM_FW( char *fname, int t_loop, MESH mesh,
                                 OPTIONS options ) ;
void write_outdata_file_HDDM( char *fname, int t_loop,
                              PartMesh pm, OPTIONS options ) ;
void write_outdata_file_HDDM_FW( char *fname, int t_loop, PartMesh pm,
                                  OPTIONS options ) ;
void write_outdata_file_options_FW( AdvDocFile *fp, int *nid, char *id,
                                     OPTIONS options ) ;

/* avsfile */
void write_avsfile_file( char *fname,
                         int nel, int nnd, int ap_elem, int nd_elem,
                         char *elem_type, int dimension,
                         int *nop, double *crd, int *flag, double *x,
                         OPTIONS options ) ;

/* vtkfile */
void write_vtkfile_file( char *fname,
                         int nel, int nnd, int ap_elem, int nd_elem,
                         char *elem_type, int dimension,
                         int *nop, double *crd, int *flag, double *x,
                         OPTIONS options ) ;

/* calclog */

/* mtrldat */
void read_mtrldat_file( char *fname, MTRL *mtrl, OPTIONS options ) ;
void write_mtrldat_file( char *fname, MTRL mtrl ) ;

#endif /* __FILE_IO_H__ */
