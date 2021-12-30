/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/


#ifndef __TYPES_H__
#define __TYPES_H__


#define N_INI 2


typedef struct {
  double re ;
  double im ;
} Complex ;


/* MYMT solver */
typedef struct {
  int     nf ;
  int     *tmp_ai ;
  int     **tmp_aij ;

  double  **tmp_vij_d ;
  Complex **tmp_vij_c ;

  double  *tmp_b_d ;
  Complex *tmp_b_c ;

  double  *tmp_x_d ;
  Complex *tmp_x_c ;

  int     nMargin ;
  int     *nSize ;

  int     tmp_pc_size ;
  double  *tmp_pc_d ;
  Complex *tmp_pc_c ;
} TMP_AIJ ;

typedef struct {
  int     nf ;
  int     *ai ;
  int     *aij ;

  double  *vij_d ;
  Complex *vij_c ;

  double  *b_d ;
  Complex *b_c ;

  double  *x_d ;
  Complex *x_c ;

  int     pc_size ;
  double  *pc_d ;
  Complex *pc_c ;
} AIJ_MAT ;


/* Switches for data */
typedef struct {
  char *data_dir ;
  char *fwdata_dir ;
  char *fwdata_file ;
  char *onedata_dir ;
  char *onedata_file ;
  char *modeldata_dir ;
  char *modeldata_file ;
  char *Jometis_dir ;
  char *Jometis_file ;
  char *Inidata_dir ;
  char *Inidata_file ;
  char *Inimetis_dir ;
  char *Inimetis_file ;
  char *outdata_dir ;
  char *outdata_file ;
  int  avsfile ;
  int  avsfile_micro ;
  int  vtkfile ;
  char *avsfile_dir ;
  char *avsfile_file ;  
  char *vtkfile_dir ;
  char *vtkfile_file ;

  char *calclog_dir ;
  char *calclog_file ;
  char *mtrldat_dir ;
  char *mtrldat_file ;
} DataSW ;
  

/* Options for solver */
typedef struct {
  FILE   *fp ;

  int    solver ;

  int    solver_dom ; 

  int    solver_pc ;
  double solver_eps ;
  double solver_div ;
  int    solver_max_loop ;
  double solver_ic_shift ;
  int    solver_log ;

  int    coJo_sol ;
  int    coJo_sol_pc ;
  double coJo_sol_eps ;
  int    coJo_sol_max_loop ;
  double coJo_sol_div ;
  double coJo_sol_ic_shift ;
  int    coJo_sol_log ;
} SolvOpt ;


/* Options for HDDM solver */
typedef struct {
  FILE   *fp ;

  int    hddm_solver ;
  int    hddm_pc ;
  double hddm_eps ;
  double hddm_nl_eps ;
  double hddm_div ;
  double hddm_nl_div ;
  int    hddm_max_loop ;
  int    hddm_log ;

  int    coJo_hddm_sol ;
  int    coJo_hddm_pc ;
  double coJo_hddm_eps ;
  int    coJo_hddm_max_loop ;
  double coJo_hddm_div ;
  int    coJo_hddm_log ;
} HDDM_Opt ;


typedef struct {
  int type ;    /* matrix type */
  int ndim ;    /* dimension of this matrix */
  int nlen ;    /* size of 1-dimensional arranged matrix */
  int* idx1 ;   /* index data */
  int* idx2 ;   /* index data */
  
  double* mat ;
 /* 1-dimensional arranged matrix data */
  Complex* mat_c ; /* 1-dimensional arranged matrix data for Complex*/
  double _Complex *mat_dc ; /* 1-dimensional arranged matrix data for Complex*/

  double* rhs ;
  Complex* rhs_c ;
} DMatrix;

/* Switches for unit */
typedef struct {
  int    CurrentDensity ;
  int    MagneticField ;
} UNIT ;


/* Options for module */
typedef struct {
  DataSW   datasw ;
  SolvOpt  solvopt ;
  HDDM_Opt hddm_opt ;
  UNIT     unit ;

  int    work ;
  int    field ;
  int    analysis ;
  int    formulation ;
  /*int    displacement_current ;*/
  size_t memlimit ;


  /* makefem */
  int      trnFile ;
  char     *fn_trnFile ;
  int      tmpFile ;
  char     *fn_tmpFile ;
  int      femFile ;
  char     *fn_femFile ;
  double   crd_magnification ;

  /*int    periodicityBC ;*/

  /* NS-Eddy */
  int    start_step ;
  int    time_step ;
  int    out_interval ;
  double delta_t ;

  /* NL-Static */
  int    nl_method ;
  int    nl_max_loop ;
  double nl_eps ;
  double nl_div ;
  int    nl_curve ;
  int    nl_curve_ip ;
  double nl_picard_weight ;
} OPTIONS ;



typedef struct {
  int    n ;
  int    nfgr ;
  int    *nface ;
  int    **el ;
  int    **face ;
  int    **nop ;
} FaceGroup ;



typedef struct {
  int    n ;
  double ap[5][3] ;
  char   analysis[128] ;
  double Jor[3] ;
  double Joi[3] ;

  double abc[3][3] ;
  double d[2][3] ;
  double tolerance ;
} MTRL_ANTENNA_DEF_PP ;

typedef struct {
  int    n ;
  int    xyz ; /* 0: parallel to x  1: parallel to y  2: parallel to z */
  double ap[4] ;
  double theta[2] ;
  double r[2] ;
  char   analysis[128] ;
  double Jor ;
  double Joi ;
} MTRL_ANTENNA_DEF_DSC ;

typedef struct {
  int    flag ;
  int    mode ;
  char   fname[FILENAME_MAX] ;

  int    ndef ;
  int    npp ; /* Number of definition of parallelpiped */
  int    ndsc ; /* Number of definition of DoubleSectorialCylinder */
  MTRL_ANTENNA_DEF_PP  *pp ;
  MTRL_ANTENNA_DEF_DSC *dsc ;
} MTRL_ANTENNA_DEFINITION ;


typedef struct {
  int    nvol ;
  int    *vol ;

  int    nbc ;
  int    *bci ;


  int    nflag ;
  int    *flag_i ;
  double *flag_nu ;

  int    nantenna ;
  MTRL_ANTENNA_DEFINITION *antenna_def ;
  int    nantenna_omega ;
  double antenna_omega ;


  /* Conductor domain (Eddy Current) */
  int    nR ;
  int    *R_i ;
  double *R_sigma ;
} MTRL ;


/* dirichlet B.C. */
typedef struct {
  int    need ;
  int    nbc ;
  int    *ibc ;
  double *vbc ;
  Complex *vbc_c ;
} DirichletBC ;


typedef struct {
  int     snid ;
  int     nfid ;
  int     coordinate ;
  int     charge_part ;
  int     ln_chpar ;  /* local interface B.C. number in charge part */
  int     ln_part ; /* local interface B.C. number in part */
} Inbc ;


typedef struct {
  int     n_mofree ;
  int     *ibid ;
} OPinfo ;


/* Antenna */
typedef struct {
  int    antenna ;
  int    nf ;
  int    *enf ;
  int    *apid2gnid ;
  double *Jo ;
  double *Jor ;
  double *Ihr ;
  double *Ihi ;
  DirichletBC dbc ;
  TMP_AIJ tmp_aij ;
  AIJ_MAT aij_mat ;
  int     ninbc ;
  Inbc    *inbc ;
} ANTENNA ;


/* post */
typedef struct {

  double  *B_d ;
  Complex *B_c ;

  double  *E_d ;
  Complex *E_c ;

  double  *Je_d ;
  Complex *Je_c ;


  double *el_B_d ;
  Complex *el_B_c ;

  double  *el_E_d ;
  Complex *el_E_c ;

  double *el_Je_d ;
  Complex *el_Je_c ;

} ResultValue ;



/* MESH of one domain */
typedef struct {
  int    nel ;
  int    nnd ;
  int    nap ;
  int    nmp ; /* number of middle points */
  int    *nop ;
  int    *flag ;
  double *crd;
  int    ap_elem ;
  int    nd_elem ;
  char   elem_type[128] ;
  int    dimension ;

  MTRL   mtrl ;

  AIJ_MAT aij_mat ;

  double *x_d ;
  Complex *x_c ;
  double *b_d_mem ;
  Complex *b_c_mem ;

  /*** Magnetic Analysis ***/
  DirichletBC  dbc_Axn0 ;
  DirichletBC  dbc_Axn0_backup ;
  ANTENNA   *antenna ;


  /** Eddy Current Analysis **/
  int    nphi ;
  int    *apid2phiid ;
  int    *apid2gnid ;
  int    *mpid2gnid ;

  /* Non-Steady Eddy Current Analysis */
  int    nini ;
  double **ini ;
  double *MagField ;
  int    nbc_A ;


  /* post */
  ResultValue rvalue ;
} MESH;




/*typedef struct {
  int    nbc ;
  int    **rbc ;
} PeriodicityBC ;


typedef struct {
  int    nf ;
  int    *enf ;
  int    *nd ;
  double *Jo ;
  double *ih ;

  DirichletBC dbc ;
} NEC ;


typedef struct {
  int    nbh;
  double *h;
  double *b;
} BH;*/


typedef struct {
  int    ninbc ;
  Inbc   *inbc ;
  /*int    nap ;*/
} DomANTENNA ;


typedef struct {
  int    partid ;
  int    domid ;
  int    npart ;
  int    ndomain ;

  int    nel ;
  int    nnd ;
  int    nap ;
  int    nmp ; /* number of middle points */
  int    nphi ;
  int    *nop ;
  int    *flag ;
  double *crd;
  int    ap_elem ;
  int    nd_elem ;
  char   elem_type[128] ;
  int    dimension ;
  int    ifd_dimension ;
  int    ninbc ;
  Inbc   *inbc ;


  int    *snid2pnid ;
  int    *seid2geid ;

  int    *apid2phiid ;
  int    *apid2snid ;
  int    *mpid2snid ;


  TMP_AIJ tmp_aij ;
  DMatrix  dmat ;
  /*int     pc_size ;
  double *pc_d ;
  Complex *pc_c ;*/
  double *x_d ;
  Complex *x_c ;
  double *b_d_mem ;
  Complex *b_c_mem ;

  /*** Magnetic Analysis ***/
  DirichletBC  dbc_Axn0 ;
  DirichletBC  dbc_Axn0_backup ;
  int    nbc_A ;
  ANTENNA   *antenna ;


  /* post */
  ResultValue rvalue ;


} DomMesh ;



typedef struct {
  int     nf ;
  int     yng_npart ;
  int     *yng_part ;
  int     *yng_nf_part ;
  int     old_npart ;
  int     *old_part ;
  int     *old_nf_part ;
  int     charge_part ;
  int     ln_chpar ;
  int     ln_part ;
  int     bc_flag ;
} Pinbc ;


typedef struct {
  int    npart ;
  int    ndomain ;
  int    infree ;
  int    outfree ;
  int    tfree ;
  int    total_free ;
  OPinfo *op ;
  DomANTENNA *dantenna ;
} PartANTENNA ;


typedef struct {
  int    partid ;
  int    npart ;
  int    ndomain ;

  int    nel ;
  int    nel_all ;
  int    nnd ;
  int    nnd_all ;
  int    nap ;
  int    nmp ; /* number of middle points */
  double *crd;
  int    ap_elem ;
  int    nd_elem ;
  char   elem_type[128] ;
  int    dimension ;


  int    *pnid2gnid ;


  int    infree ;
  int    outfree ;
  int    midfree ;
  int    tfree ;
  int    total_free ;
  OPinfo *op ;
  int    ifd_dimension ;
  Pinbc  *pinbc ;

  MTRL   mtrl ;
  PartANTENNA *pantenna ;
  PartANTENNA presult ;

  /*** Magnetic Analysis ***/
  DirichletBC  dbc_Axn0 ;
  DirichletBC  dbc_Axn0_backup ;

  double  *hddm_vec_u_d_mem ;
  Complex *hddm_vec_u_c_mem ;


  DomMesh *dm ;

/*** BDD4AdvFullWave ***/
  int    n_outdom  ;  /* Number of out domains  */
  int    sum_ninbd ;  /* Sumation of the number of 
                         inner boundary nodes             */
} PartMesh ;


typedef struct {
  int     infree ;
  int     outfree ;
  int     tfree ;
  int     total_free ;

  double  *p_d ;
  Complex *p_c ;

  double  *q_d ;
  Complex *q_c ;

  double  *r_d ;
  Complex *r_c ;

  double  *z_d ;
  Complex *z_c ;

  double  *u_d ;
  Complex *u_c ;

  double   r0 ;
  double   rz_d ;
  Complex  rz_c ;

  int     pc_size ;
  double  *pc_d ;
  Complex *pc_c ;

  /*int     infree ;
  int     outfree ;

  double  *w ;
  double  *a_w ;
  double  *g_diag ;
  double  *myu ;
  double  *g ;

  double  *a_w_in ;
  double  *w_in ;

  double  residual_1st ;*/
} HDDM_VEC ;


typedef struct {
  int      nf ;

  double*  vec_d ;
  Complex* vec_c ;

  /*double*  r_d ;
  Complex* r_c ;

  double*  q_d ;
  Complex* q_c ;*/

  int      pc_size ;
  double*  pc_d ;
  Complex* pc_c ;
} HDDM_DOM_VEC ;


typedef struct {
  /* set options */
  void (*set_HDDM_Opt)( HDDM_Opt *hddm_opt, OPTIONS options ) ;
  void (*set_SolvOpt)( SolvOpt *solvopt, OPTIONS options ) ;

  /* initialize */
  void (*make_HDDM_VEC)( HDDM_VEC *hddm_vec, PartMesh pm, HDDM_Opt hddm_opt ) ;
  void (*init_bc)( DomMesh *dm ) ;

  /* renewal interface B.C. */
  void (*renewal_bc_sp)( DomMesh *dm, HDDM_VEC hddm_vec ) ;
  void (*renewal_bc_h_send_dom)( DomMesh dm, HDDM_VEC hddm_vec,
                                 int compid, int type ) ;
  void (*renewal_bc_h_recv_dom)( DomMesh *dm, int compid, int type ) ;

  /* TMP_AIJ */
  void (*make_TMP_AIJ)( DomMesh *dm, MTRL mtrl ) ;
  void (*free_TMP_AIJ)( DomMesh *dm ) ;

  /* solver for subdomain */
  int  (*dom_solver)( AIJ_MAT *aij_mat, SolvOpt solvopt, FILE *monitor ) ;
  int  (*dom_solver_nmpcm)( AIJ_MAT *aij_mat, SolvOpt solvopt,
                            FILE *monitor ) ;

  /* preconditioner for HDDM */
  int  (*makePC)( TMP_AIJ tmp_aij, HDDM_DOM_VEC *hddm_dom_vec ) ;
  int  (*setPC)( HDDM_VEC *hddm_vec ) ;
  void (*gatherPC_s)( OPinfo **oop, HDDM_VEC *hddm_vec ) ;
  void (*gatherPC_ph)( OPinfo *op, HDDM_VEC *hddm_vec ) ;

  /* solve subdomain */
  void (*solve_dom_1st)( DomMesh *dm, HDDM_Opt hddm_opt, SolvOpt solvopt,
                         int (*dom_solver)( AIJ_MAT *aij_mat,
                                            SolvOpt solvopt,
                                            FILE *monitor ),
                         HDDM_DOM_VEC *hddm_dom_vec,
                         int (*makePC)( TMP_AIJ tmp_aij,
                                        HDDM_DOM_VEC *hddm_dom_vec ),
                         FILE *fp_log ) ;
  void (*solve_dom)( DomMesh *dm, SolvOpt solvopt,
                     int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                              SolvOpt solvopt,
                                              FILE *monitor ),
                     HDDM_DOM_VEC *hddm_dom_vec, FILE *fp_log ) ;
  void (*solve_dom_last)( DomMesh *dm, SolvOpt solvopt,
                          int (*dom_solver_nmpcm)( AIJ_MAT *aij_mat,
                                                   SolvOpt solvopt,
                                                   FILE *monitor ),
                          HDDM_DOM_VEC *hddm_dom_vec, FILE *fp_log ) ;

  /* communicate each iteration results */
  void (*COM_res_sp_1st)( HDDM_VEC *hddm_vec, HDDM_DOM_VEC hddm_dom_vec,
                          DomMesh dm, HDDM_Opt hddm_opt ) ;
  void (*COM_res_sp)( HDDM_VEC *hddm_vec, HDDM_DOM_VEC hddm_dom_vec,
                      DomMesh dm, HDDM_Opt hddm_opt ) ;
  void (*COM_res_h_recv_1st)( HDDM_VEC *hddm_vec, DomMesh dm,
                              HDDM_Opt hddm_opt, int compid, int type ) ;
  void (*COM_res_h_recv)( HDDM_VEC *hddm_vec, DomMesh dm,
                          HDDM_Opt hddm_opt, int compid, int type ) ;

  /* iterations */
  void (*iteration_s_1st)( PartMesh *pm, HDDM_VEC *hddm_vec,
                           HDDM_Opt hddm_opt,
                           int (*setPC)( HDDM_VEC *hddm_vec ),
                           void (*gatherPC_s)( OPinfo **oop,
                                               HDDM_VEC *hddm_vec ) ) ;
  void (*iteration_s)( PartMesh *pm, HDDM_VEC *hddm_vec,
                       HDDM_Opt hddm_opt,
                       int (*setPC)( HDDM_VEC *hddm_vec ) ) ;
  void (*iteration_ph_1st)( PartMesh pm, HDDM_VEC *hddm_vec,
                            HDDM_Opt hddm_opt,
                           int (*setPC)( HDDM_VEC *hddm_vec ),
                           void (*gatherPC_ph)( OPinfo *op,
                                                HDDM_VEC *hddm_vec ) ) ;
  void (*iteration_ph)( PartMesh pm, HDDM_VEC *hddm_vec,
                        HDDM_Opt hddm_opt,
                        int (*setPC)( HDDM_VEC *hddm_vec ) ) ;

  /* get result */
  void (*get_result_sp)( PartMesh *pm ) ;
  void (*post_sp)( PartMesh *pm ) ;
  void (*post_h_recv_res)( DomMesh *dm, int compid, int type ) ;
  void (*post_h_send_res)( DomMesh *dm, MTRL mtrl, int compid, int type ) ;
  void (*out_result)( int t_loop, PartMesh pm, OPTIONS options ) ;

  /* general functions */
  void (*general_func_1st)( PartMesh *pm, HDDM_VEC * hddm_vec,
                            OPTIONS options,
                            HDDM_Opt hddm_opt, SolvOpt solvopt ) ;
  void (*general_func_2nd)( PartMesh *pm, HDDM_VEC * hddm_vec,
                            OPTIONS options,
                            HDDM_Opt hddm_opt, SolvOpt solvopt ) ;
  void (*general_func_3rd)( PartMesh *pm, HDDM_VEC * hddm_vec,
                            OPTIONS options,
                            HDDM_Opt hddm_opt, SolvOpt solvopt ) ;
  void (*general_func_4th)( PartMesh *pm, HDDM_VEC * hddm_vec,
                            OPTIONS options,
                            HDDM_Opt hddm_opt, SolvOpt solvopt ) ;
  void (*general_func_5th)( PartMesh *pm, HDDM_VEC * hddm_vec,
                            OPTIONS options,
                            HDDM_Opt hddm_opt, SolvOpt solvopt ) ;
} HDDM_FUNC ;

typedef struct {
  int    status ;
  int    loop ;
  int    ndomain ;

  int    antenna ;
  int    Jo ;
  int    nl_loop ;
  int    t_loop ; /* time loop */
} HDDM_SW ;

typedef struct {
  double work ;
  double comm ;
  double idle ;
} WORK_TIMES ;

#endif /* __TYPES_H__ */
