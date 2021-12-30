/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "advfullwave_lib.h"
#include "libfem.h"
#include "libhddm.h"

#include "fullwave.h"

#define LOCAL_MODULE_NAME "Full-Wave analysis with HDDM"
#define LOCAL_VERSION     ADVFW_VERSION
#define LOCAL_WORK        WORK_HDDM_ANALYSIS
#define LOCAL_FIELD       FIELD_ELECTROMAGNETIC
#define LOCAL_ANALYSIS    ANALYSIS_FULL_WAVECURRENT
#define LOCAL_N_ARGUMENT  0
#define LOCAL_USAGE       ""


extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;

extern int nstart ;
extern int nend ;
extern int nrepeat ;

extern HDDM_SW hddm_sw ;
extern WORK_TIMES work_times ;
extern double s_time ;


int main( int argc, char **argv )
{
  int    i, j ;

  PartMesh* pm = NULL ;
  OPTIONS options ;


  /*** Initialize ***/

  hddm_time_Init() ;

  options.solvopt.solver_dom   = SOLVER_LU ;  
  //options.solvopt.solver_pc    = SOLVER_PC_NONE ;
  //options.solvopt.solver       = SOLVER_COCG ;
  options.solvopt.solver_pc    = SOLVER_PC_ICC ;
  options.solvopt.solver_eps   = 1.0e-09 ;
  options.solvopt.solver_log   = NO_NEED ;
  options.solvopt.coJo_sol     = SOLVER_CGMETHOD ;
  options.solvopt.coJo_sol_pc  = SOLVER_PC_ICC ;
  options.solvopt.coJo_sol_log = NO_NEED ;
  options.hddm_opt.hddm_solver   = HDDM_COCG ;
  options.hddm_opt.hddm_pc       = HDDM_PC_DIAG ;
  options.hddm_opt.hddm_log      = NEED ;
  options.hddm_opt.coJo_hddm_sol = HDDM_CGMETHOD ;
  options.hddm_opt.coJo_hddm_pc  = HDDM_PC_DIAG ;
  options.hddm_opt.coJo_hddm_log = NEED ;

  main_Init( argc, argv, &options ) ;

  /*** Make or Check output files are writable ***/
  if( COM_GID.rootid == COM_ID.myprocid ){
    if( make_outdata_directory(options.datasw) > 0 ) {
      Exit( -1 ) ;
    }
    if( make_calclog_directory(options.datasw) > 0 ) {
      Exit( -1 ) ;
    }
  }

  work_times.work += plusTime( &s_time ) ;
  COM_Synchronize() ;
  work_times.idle += plusTime( &s_time ) ;

  /*** Main Function ***/
  hddm_Init( &pm, options ) ;

  main_PrintCondition( options, pm[0].nel_all, 0, 0,
                       pm[0].elem_type, pm[0].total_free ) ;


  /** main function **/

  work_times.work += plusTime( &s_time ) ;
  COM_Synchronize() ;
  work_times.idle += plusTime( &s_time ) ;
  hddm_sw.t_loop = -2 ;
  
  for( i=0 ; i<pm[0].mtrl.nantenna ; i++ ) {
    
    hddm_sw.antenna = i ;
    hddm_solver_RS( pm, options, set_hddm_func_correct_Jor ) ;
    hddm_solver_RS( pm, options, set_hddm_func_correct_Joi ) ;
    
  }
 

  work_times.work += plusTime( &s_time ) ;
  COM_Synchronize() ;
  work_times.idle += plusTime( &s_time ) ;
  hddm_sw.antenna = -1 ;
  hddm_solver_CS( pm, options, set_hddm_func_fullwave ) ;

  if( pm != NULL ) {
    for( i=nstart,j=0 ; i<nend ; i++,j++ ) {
      free_PartMesh( &(pm[j]), options ) ;
    }
    Free( sizeof(PartMesh)*nrepeat, pm ) ;
  }
  work_times.work += plusTime( &s_time ) ;
  COM_Synchronize() ;
  work_times.idle += plusTime( &s_time ) ;


  /*** Finalize ***/
  hddm_time_Finalize() ;
  main_Finalize() ;


  return 0 ;
}
