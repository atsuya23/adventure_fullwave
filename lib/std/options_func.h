/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __OPTIONS_FUNC_H__
#define __OPTIONS_FUNC_H__


/*** Option String ***/
/** help options **/
#define OPTION_STRING_V                 "-v"
#define OPTION_STRING_VERSION           "-version"
#define OPTION_STRING_H                 "-h"
#define OPTION_STRING_HELP              "-help"


/** directory / file name options **/
#define OPTION_STRING_DATA_DIR          "-data-dir"
#define OPTION_STRING_FWDATA_DIR       "-fwdata-dir"
#define OPTION_STRING_FWDATA_FILE      "-fwdata-file"
#define OPTION_STRING_ONEDATA_DIR       "-onedata-dir"
#define OPTION_STRING_ONEDATA_FILE      "-onedata-file"
#define OPTION_STRING_MODELDATA_DIR     "-model-dir"
#define OPTION_STRING_MODELDATA_FILE    "-model-file"
#define OPTION_STRING_JOMETIS_DIR       "-Jometis-dir"
#define OPTION_STRING_JOMETIS_FILE      "-Jometis-file"
#define OPTION_STRING_INIDATA_DIR       "-Inidata-dir"
#define OPTION_STRING_INIDATA_FILE      "-Inidata-file"
#define OPTION_STRING_INIMETIS_DIR      "-Inimetis-dir"
#define OPTION_STRING_INIMETIS_FILE     "-Inimetis-file"
#define OPTION_STRING_OUTDATA_DIR       "-result-dir"
#define OPTION_STRING_OUTDATA_FILE      "-result-file"
#define OPTION_STRING_AVSFILE           "-avsfile"
#define OPTION_STRING_AVSFILE_NO_NEED   "-avsfile-no"
#define OPTION_STRING_AVSFILE_MICRO     "-avsfile-micro"
#define OPTION_STRING_VTKFILE           "-vtkfile"
#define OPTION_STRING_AVSFILE_DIR       "-avsfile-dir"
#define OPTION_STRING_AVSFILE_FILE      "-avsfile-file"
#define OPTION_STRING_VTKFILE_DIR       "-vtkfile-dir"
#define OPTION_STRING_VTKFILE_FILE      "-vtkfile-file"
#define OPTION_STRING_CALCLOG_DIR       "-calclog-dir"
#define OPTION_STRING_CALCLOG_FILE      "-calclog-file"
#define OPTION_STRING_MTRLDAT_DIR       "-mtrldat-dir"
#define OPTION_STRING_MTRLDAT_FILE      "-mtrldat-file"


/** Options for Analysis **/
#define OPTION_STRING_FORMULATION       "-formulation"


/** Options for Solver **/
#define OPTION_STRING_SOLVER            "-linear-solver"
#define OPTION_STRING_SOLVER_PC         "-l-sol-pc"
#define OPTION_STRING_SOLVER_EPS        "-l-sol-eps"
#define OPTION_STRING_SOLVER_DIV        "-l-sol-div"
#define OPTION_STRING_SOLVER_IC_SHIFT   "-l-sol-ic-shift"
#define OPTION_STRING_SOLVER_LOG        "-l-sol-log"
#define OPTION_STRING_SOLVER_NO_LOG     "-l-sol-no-log"

#define OPTION_STRING_COJO_SOL          "-coJo-l-sol"
#define OPTION_STRING_COJO_SOL_PC       "-coJo-l-sol-pc"
#define OPTION_STRING_COJO_SOL_EPS      "-coJo-l-sol-eps"
#define OPTION_STRING_COJO_SOL_DIV      "-coJo-l-sol-div"
#define OPTION_STRING_COJO_SOL_IC_SHIFT "-coJo-l-sol-ic-shift"
#define OPTION_STRING_COJO_SOL_LOG      "-coJo-l-sol-log"
#define OPTION_STRING_COJO_SOL_NO_LOG   "-coJo-l-sol-no-log"


/** Options for HDDM solver **/
#define OPTION_STRING_HDDM_SOLVER       "-hddm-solver"
#define OPTION_STRING_HDDM_PC           "-hddm-pc"
#define OPTION_STRING_HDDM_EPS          "-hddm-eps"
#define OPTION_STRING_HDDM_DIV          "-hddm-div"
#define OPTION_STRING_HDDM_MAX_LOOP     "-hddm-max-loop"
#define OPTION_STRING_HDDM_LOG          "-hddm-log"
#define OPTION_STRING_HDDM_NO_LOG       "-hddm-no-log"

#define OPTION_STRING_COJO_HDDM_SOL      "-coJo-hddm-sol"
#define OPTION_STRING_COJO_HDDM_PC       "-coJo-hddm-pc"
#define OPTION_STRING_COJO_HDDM_EPS      "-coJo-hddm-eps"
#define OPTION_STRING_COJO_HDDM_DIV      "-coJo-hddm-div"
#define OPTION_STRING_COJO_HDDM_MAX_LOOP "-coJo-hddm-max-loop"
#define OPTION_STRING_COJO_HDDM_LOG      "-coJo-hddm-log"
#define OPTION_STRING_COJO_HDDM_NO_LOG   "-coJo-hddm-no-log"


/** Options for unit **/
#define OPTION_STRING_UNIT_CURRENT_DENSITY  "-unit-current-density"
#define OPTION_STRING_UNIT_ELECTROMAGNETIC_FIELD "-unit-electromagnetic-field"


/** Options for makefem **/
#define OPTION_STRING_MAKEFEM_TRNFILE            "-t"
#define OPTION_STRING_MAKEFEM_TMPFILE            "-temperature"
#define OPTION_STRING_MAKEFEM_FEMFILE            "-f"
#define OPTION_STRING_MAKEFEM_CRD_FWNIFICATION  "-crd-magnification"


/** Misc. options **/
#define OPTION_STRING_MEMLIMIT          "-memlimit"




#include "advfullwave_lib.h"


void HelpOptions( int argc, char **argv, char *mname, char *version,
                  char *usage )
{
  int    i ;


  i = 1 ;
  while( i < argc ) {
    /* Misc. and help options */
    if( !strcmp(argv[i], OPTION_STRING_V) ) {
      Fprintf( stdout, "%s %s\n", mname, version ) ;
      Exit( 1 ) ;
    } else if( !strcmp(argv[i], OPTION_STRING_VERSION) ) {
      Fprintf( stdout, "%s %s\n", mname, version ) ;
      Exit( 1 ) ;
    } else if( !strcmp(argv[i], OPTION_STRING_H) ) {
      PrintUsage( argv[0], mname, version, usage) ;
      Exit( 1 ) ;
    } else if( !strcmp(argv[i], OPTION_STRING_HELP) ) {
      PrintUsage( argv[0], mname, version, usage ) ;
      Exit( 1 ) ;
    }
    i++ ;
  }
}

/********************************************************************/

void SetOptions( int argc, char **argv, OPTIONS *options,
                 char *mname, char *version,
                 int work, int field, int analysis,
                 int n_argument, char *usage )
{
  int    i ;
  int    num_min_opts ;

  DataSW*   datasw = &(options->datasw) ;
  SolvOpt*  solvopt = &(options->solvopt) ;
  HDDM_Opt* hddm_opt = &(options->hddm_opt) ;
  UNIT*     unit = &(options->unit) ;


  /* default options */
  DataSW_init( datasw ) ;
  SolvOpt_init( solvopt ) ;
  HDDM_Opt_init( hddm_opt, field, analysis ) ;
  UNIT_init( unit ) ;
  OPTIONS_init( options, work, field, analysis ) ;

  i = n_argument + 1 ; num_min_opts = 0 ;
  /*num_min_opts = 1 ;*/
#ifndef __ADVFW_OPTIONS_MAKEFEM__
  argc-- ;
#endif
  while ( i < argc ) {

    /*=== Options for analysis ===*/
    if( !strcmp(argv[i], OPTION_STRING_MEMLIMIT) ) {
      if( atoi(argv[++i]) < 0 ) {
        Efprintf( stderr, "Invalid memlimit %s\n"
                  "       memlimit should be 0 or more\n", argv[i] ) ;
        Exit( 1 ) ;
      }
      options->memlimit = atoi(argv[i]) *1024*1024 ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;

    /** directory / file name options **/
    /*} else if( !strcmp(argv[i], OPTION_STRING_DATA_DIR) ) {
      i++ ;
      datasw->data_dir = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;*/

#ifdef __ADVFW_OPTIONS_FWDATA__
    } else if( !strcmp(argv[i], OPTION_STRING_FWDATA_DIR) ) {
      i++ ;
      datasw->fwdata_dir = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_FWDATA_FILE) ) {
      i++ ;
      datasw->fwdata_file = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_FWDATA__ */

#ifdef __ADVFW_OPTIONS_ONEDATA__
    } else if( !strcmp(argv[i], OPTION_STRING_ONEDATA_DIR) ) {
      i++ ;
      datasw->onedata_dir = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_ONEDATA_FILE) ) {
      i++ ;
      datasw->onedata_file = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_ONEDATA__ */

#ifdef __ADVFW_OPTIONS_MODELDATA__
    } else if( !strcmp(argv[i], OPTION_STRING_MODELDATA_DIR) ) {
      i++ ;
      datasw->modeldata_dir = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_MODELDATA_FILE) ) {
      i++ ;
      datasw->modeldata_file = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_MODLEDATA__ */

#ifdef __ADVFW_OPTIONS_JOMETIS__
    } else if( !strcmp(argv[i], OPTION_STRING_JOMETIS_DIR) ) {
      i++ ;
      datasw->Jometis_dir = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_JOMETIS_FILE) ) {
      i++ ;
      datasw->Jometis_file = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_JOMETIS__ */

#ifdef __ADVFW_OPTIONS_INIDATA__
    } else if( !strcmp(argv[i], OPTION_STRING_INIDATA_DIR) ) {
      i++ ;
      datasw->Inidata_dir = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_INIDATA_FILE) ) {
      i++ ;
      datasw->Inidata_file = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_INIDATA__ */

#ifdef __ADVFW_OPTIONS_INIMETIS__
    } else if( !strcmp(argv[i], OPTION_STRING_INIMETIS_DIR) ) {
      i++ ;
      datasw->Inimetis_dir = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_INIMETIS_FILE) ) {
      i++ ;
      datasw->Inimetis_file = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_INIMETIS__ */

#ifdef __ADVFW_OPTIONS_OUTDATA__
    } else if( !strcmp(argv[i], OPTION_STRING_OUTDATA_DIR) ) {
      i++ ;
      datasw->outdata_dir = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_OUTDATA_FILE) ) {
      i++ ;
      datasw->outdata_file = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_OUTDATA__ */

#ifdef __ADVFW_OPTIONS_AVSFILE__
    } else if( !strcmp(argv[i], OPTION_STRING_AVSFILE) ) {
      datasw->avsfile = NEED ;
      Fprintf( stdout, "    %s\n", argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_AVSFILE_NO_NEED) ) {
      datasw->avsfile = NO_NEED ;
      Fprintf( stdout, "    %s\n", argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_AVSFILE_MICRO) ) {
      datasw->avsfile_micro = NEED ;
      Fprintf( stdout, "    %s\n", argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_VTKFILE) ) {
      datasw->vtkfile = NEED ;
      Fprintf( stdout, "    %s\n", argv[i] ) ;
      i++ ;
    }else if( !strcmp(argv[i], OPTION_STRING_AVSFILE_DIR) ) {
      i++ ;
      datasw->avsfile_dir = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_AVSFILE_FILE) ) {
      i++ ;
      datasw->avsfile_file = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    }else if( !strcmp(argv[i], OPTION_STRING_VTKFILE_DIR) ) {
      i++ ;
      datasw->vtkfile_dir = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_VTKFILE_FILE) ) {
      i++ ;
      datasw->vtkfile_file = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_AVSFILE__ */

#ifdef __ADVFW_OPTIONS_CALCLOG__
    } else if( !strcmp(argv[i], OPTION_STRING_CALCLOG_DIR) ) {
      i++ ;
      datasw->calclog_dir = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_CALCLOG_FILE) ) {
      i++ ;
      datasw->calclog_file = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_CALCLOG__ */

#ifdef __ADVFW_OPTIONS_MTRLDAT__
    } else if( !strcmp(argv[i], OPTION_STRING_MTRLDAT_DIR) ) {
      i++ ;
      datasw->mtrldat_dir = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_MTRLDAT_FILE) ) {
      i++ ;
      datasw->mtrldat_file = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_MTRLDAT__ */



    /** Options for Analysis **/
#ifdef __ADVFW_OPTIONS_FORMULATION__
    } else if( !strcmp(argv[i], OPTION_STRING_FORMULATION) ) {
      i++ ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      if( !strcmp(argv[i], FORMULATION_FW_EMETHOD_STRING) ) {
	options->formulation = FORMULATION_FW_EMETHOD ;
      } else {
	Fprintf( stdout, "      Unknown symbol\n" ) ;
	Fprintf( stdout, "      set Formulation to default\n" ) ;
      }
      i++ ;

#endif /* __ADVFW_OPTIONS_FORMULATION__ */

    /** Options for Analysis **/
    /* Options for Solver */
#ifdef __ADVFW_OPTIONS_SOLVER__

    } else if( !strcmp(argv[i], OPTION_STRING_SOLVER) ) {
      i++ ;
      solvopt->solver = atoi( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_SOLVER_PC) ) {
      i++ ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      if( !strcmp(argv[i], SOLVER_PC_NONE_STRING) ) {
        solvopt->solver_pc = SOLVER_PC_NONE ;
      } else if( !strcmp(argv[i], SOLVER_PC_ICC_STRING) ) {
        solvopt->solver_pc = SOLVER_PC_ICC ;
      } else {
        Fprintf( stdout, "      Unknown symbol\n" ) ;
        Fprintf( stdout, "      set Linear Solver PC to DIAG\n" ) ;
      }
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_SOLVER_EPS) ) {
      i++ ;
      solvopt->solver_eps = atof( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_SOLVER_DIV) ) {
      i++ ;
      solvopt->solver_div = atof( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_SOLVER_IC_SHIFT) ) {
      i++ ;
      solvopt->solver_ic_shift = atof( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i]);
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_SOLVER_LOG) ) {
      solvopt->solver_log = NEED ;
      Fprintf( stdout, "    %s\n", argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_SOLVER_NO_LOG) ) {
      solvopt->solver_log = NO_NEED ;
      Fprintf( stdout, "    %s\n", argv[i] ) ;
      i++ ;

    } else if( !strcmp(argv[i], OPTION_STRING_COJO_SOL) ) {
      i++ ;
      solvopt->coJo_sol = atoi( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_COJO_SOL_PC) ) {
      i++ ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      if( !strcmp(argv[i], SOLVER_PC_NONE_STRING) ) {
        solvopt->coJo_sol_pc = SOLVER_PC_NONE ;
      } else if( !strcmp(argv[i], SOLVER_PC_ICC_STRING) ) {
        solvopt->coJo_sol_pc = SOLVER_PC_ICC ;
      } else {
        Fprintf( stdout, "      Unknown symbol\n" ) ;
        Fprintf( stdout,
                 "      set correcting Jo Linear Solver PC to DIAG\n" ) ;
      }
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_COJO_SOL_EPS) ) {
      i++ ;
      solvopt->coJo_sol_eps = atof( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_COJO_SOL_DIV) ) {
      i++ ;
      solvopt->coJo_sol_div = atof( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_COJO_SOL_IC_SHIFT) ) {
      i++ ;
      solvopt->coJo_sol_ic_shift = atof( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i]);
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_COJO_SOL_LOG) ) {
      solvopt->coJo_sol_log = NEED ;
      Fprintf( stdout, "    %s\n", argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_COJO_SOL_NO_LOG) ) {
      solvopt->coJo_sol_log = NO_NEED ;
      Fprintf( stdout, "    %s\n", argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_SOLVER__ */


    /* Options for Solver */
#ifdef __ADVFW_OPTIONS_HDDM__
    } else if( !strcmp(argv[i], OPTION_STRING_HDDM_SOLVER) ) {
      i++ ;
      hddm_opt->hddm_solver = atoi( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_HDDM_PC) ) {
      i++ ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      if( !strcmp(argv[i], HDDM_PC_NONE_STRING) ) {
        hddm_opt->hddm_pc = HDDM_PC_NONE ;
      } else if( !strcmp(argv[i], HDDM_PC_DIAG_STRING) ) {
        hddm_opt->hddm_pc = HDDM_PC_DIAG ;
      } else {
        Fprintf( stdout, "      Unknown symbol\n" ) ;
        Fprintf( stdout, "      set HDDM PC to DIAG\n" ) ;
      }
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_HDDM_EPS) ) {
      i++ ;
      hddm_opt->hddm_eps = atof( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_HDDM_DIV) ) {
      i++ ;
      hddm_opt->hddm_div = atof( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_HDDM_MAX_LOOP) ) {
      i++ ;
      hddm_opt->hddm_max_loop = atoi( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_HDDM_LOG) ) {
      hddm_opt->hddm_log = NEED ;
      Fprintf( stdout, "    %s\n", argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_HDDM_NO_LOG) ) {
      hddm_opt->hddm_log = NO_NEED ;
      Fprintf( stdout, "    %s\n", argv[i] ) ;
      i++ ;

    } else if( !strcmp(argv[i], OPTION_STRING_COJO_HDDM_SOL) ) {
      i++ ;
      hddm_opt->coJo_hddm_sol = atoi( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_COJO_HDDM_PC) ) {
      i++ ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      if( !strcmp(argv[i], HDDM_PC_NONE_STRING) ) {
        hddm_opt->coJo_hddm_pc = HDDM_PC_NONE ;
      } else if( !strcmp(argv[i], HDDM_PC_DIAG_STRING) ) {
        hddm_opt->coJo_hddm_pc = HDDM_PC_DIAG ;
      } else {
        Fprintf( stdout, "      Unknown symbol\n" ) ;
        Fprintf( stdout, "      set correcting Jo HDDM PC to DIAG\n" ) ;
      }
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_COJO_HDDM_EPS) ) {
      i++ ;
      hddm_opt->coJo_hddm_eps = atof( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_COJO_HDDM_DIV) ) {
      i++ ;
      hddm_opt->coJo_hddm_div = atof( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_COJO_HDDM_MAX_LOOP) ) {
      i++ ;
      hddm_opt->coJo_hddm_max_loop = atoi( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_COJO_HDDM_LOG) ) {
      hddm_opt->coJo_hddm_log = NEED ;
      Fprintf( stdout, "    %s\n", argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_COJO_HDDM_NO_LOG) ) {
      hddm_opt->coJo_hddm_log = NO_NEED ;
      Fprintf( stdout, "    %s\n", argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_HDDM__ */


#ifdef __ADVFW_OPTIONS_UNIT_CURRENT_DENSITY__
    } else if( !strcmp(argv[i], OPTION_STRING_UNIT_CURRENT_DENSITY) ) {
      i++ ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      if( !strcmp(argv[i], UNIT_A_m2_STRING) ) {
        unit->CurrentDensity = UNIT_A_m2 ;
      } else if( !strcmp(argv[i], UNIT_A_mm2_STRING) ) {
        unit->CurrentDensity = UNIT_A_mm2 ;
      } else {
        Fprintf( stdout, "      Unknown symbol\n" ) ;
        Fprintf( stdout, "      set the unit to [A/m]\n" ) ;
      }
      i++ ;
#endif /* __ADVFW_OPTIONS_UNIT_CURRENT_DENSITY__ */

#ifdef __ADVFW_OPTIONS_UNIT_ELECTROMAGNETIC_FIELD__
    } else if( !strcmp(argv[i], OPTION_STRING_UNIT_ELECTROMAGNETIC_FIELD) ) {
      i++ ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      if( !strcmp(argv[i], UNIT_T_STRING) ) {
        unit->MagneticField = UNIT_T ;
      } else if( !strcmp(argv[i], UNIT_G_STRING) ) {
        unit->MagneticField = UNIT_G ;
      } else if( !strcmp(argv[i], UNIT_A_m_STRING) ) {
        unit->MagneticField = UNIT_A_m ;
      } else if( !strcmp(argv[i], UNIT_A_mm_STRING) ) {
        unit->MagneticField = UNIT_A_mm ;
      } else {
        Fprintf( stdout, "      Unknown symbol\n" ) ;
        Fprintf( stdout, "      set the unit to [T]\n" ) ;
      }
      i++ ;
#endif /* __ADVFW_OPTIONS_UNIT_ELECTROMAGNETIC_FIELD__ */

#ifdef __ADVFW_OPTIONS_MAKEFEM__
    } else if( !strcmp(argv[i], OPTION_STRING_MAKEFEM_TRNFILE) ) {
      i++ ;
      options->trnFile = NEED ;
      options->fn_trnFile = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_MAKEFEM_TMPFILE) ) {
      i++ ;
      options->tmpFile = NEED ;
      options->fn_tmpFile = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_MAKEFEM_FEMFILE) ) {
      i++ ;
      options->femFile = NEED ;
      options->fn_femFile = argv[i] ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
    } else if( !strcmp(argv[i], OPTION_STRING_MAKEFEM_CRD_FWNIFICATION) ) {
      i++ ;
      options->crd_magnification = atof( argv[i] ) ;
      Fprintf( stdout, "    %s %s\n", argv[i-1], argv[i] ) ;
      i++ ;
#endif /* __ADVFW_OPTIONS_MAKEFEM__ */



    /** Exceptions **/
    } else {
      if (  i == argc - num_min_opts ) break ;
      if ( i > argc - num_min_opts ) {
	Efprintf(stderr,"Options are not enough\n");
      } else {
	Efprintf(stderr,"No such option %s\n", argv[i]);
      }
      PrintUsage( argv[0], mname, version, usage ) ;
      Exit(-1);
    }
    fflush(stdout);
  }


#ifndef __ADVFW_OPTIONS_MAKEFEM__
  if( i != argc ) {
    PrintUsage( argv[0], mname, version, usage );
    Exit(-1);
  }
  datasw->data_dir = argv[argc] ;
#endif


  if( argc == 1 ) {
    Fprintf(stdout, "    Nothing\n" );
  }


  if (  i != argc - num_min_opts ) {
    PrintUsage( argv[0], mname, version, usage );
    Exit(-1);
  }
}

/************************************************************************/

void DataSW_init( DataSW  *datasw )
{
  /*datasw->data_dir = DEFAULT_DATA_DIR ;*/

#ifdef __ADVFW_OPTIONS_FWDATA__
  datasw->fwdata_dir = DEFAULT_FWDATA_DIR ;
  datasw->fwdata_file = DEFAULT_FWDATA_FILE ;
#endif /* __ADVFW_OPTIONS_FWDATA__ */

#ifdef __ADVFW_OPTIONS_ONEDATA__
  datasw->onedata_dir = DEFAULT_ONEDATA_DIR ;
  datasw->onedata_file = DEFAULT_ONEDATA_FILE ;
#endif /* __ADVFW_OPTIONS_ONEDATA__ */

#ifdef __ADVFW_OPTIONS_MODELDATA__
  datasw->modeldata_dir = DEFAULT_MODELDATA_DIR ;
  datasw->modeldata_file = DEFAULT_MODELDATA_FILE ;
#endif /* __ADVFW_OPTIONS_MODELDATA__ */

#ifdef __ADVFW_OPTIONS_JOMETIS__
  datasw->Jometis_dir = DEFAULT_JOMETIS_DIR ;
  datasw->Jometis_file = DEFAULT_JOMETIS_FILE ;
#endif /* __ADVFW_OPTIONS_JOMETIS__ */

#ifdef __ADVFW_OPTIONS_INIDATA__
  datasw->Inidata_dir = DEFAULT_INIDATA_DIR ;
  datasw->Inidata_file = DEFAULT_INIDATA_FILE ;
#endif /* __ADVFW_OPTIONS_INIDATA__ */

#ifdef __ADVFW_OPTIONS_INIMETIS__
  datasw->Inimetis_dir = DEFAULT_INIMETIS_DIR ;
  datasw->Inimetis_file = DEFAULT_INIMETIS_FILE ;
#endif /* __ADVFW_OPTIONS_INIMETIS__ */

#ifdef __ADVFW_OPTIONS_OUTDATA__
  datasw->outdata_dir = DEFAULT_OUTDATA_DIR ;
  datasw->outdata_file = DEFAULT_OUTDATA_FILE ;
#endif /* __ADVFW_OPTIONS_OUTDATA__ */

#ifdef __ADVFW_OPTIONS_AVSFILE__
  datasw->avsfile = NO_NEED ;
  datasw->avsfile_micro = NO_NEED ;
  datasw->vtkfile = NO_NEED ;
  datasw->avsfile_dir = DEFAULT_AVSFILE_DIR ;
  datasw->avsfile_file = DEFAULT_AVSFILE_FILE ;
  datasw->vtkfile_dir = DEFAULT_VTKFILE_DIR ;
  datasw->vtkfile_file = DEFAULT_VTKFILE_FILE ;
#endif /* __ADVFW_OPTIONS_AVSFILE__ */

#ifdef __ADVFW_OPTIONS_CALCLOG__
  datasw->calclog_dir = DEFAULT_CALCLOG_DIR ;
  datasw->calclog_file = DEFAULT_CALCLOG_FILE ;
#endif /* __ADVFW_OPTIONS_CALCLOG__ */

#ifdef __ADVFW_OPTIONS_MTRLDAT__
  datasw->mtrldat_dir = DEFAULT_MTRLDAT_DIR ;
  datasw->mtrldat_file = DEFAULT_MTRLDAT_FILE ;
#endif /* __ADVFW_OPTIONS_MTRLDAT__ */
}

/************************************************************************/

void SolvOpt_init( SolvOpt *solvopt )
{
#ifdef __ADVFW_OPTIONS_SOLVER__
  /*solvopt->solver = DEFAULT_SOLVER ;*/
  /*solvopt->solver_pc = DEFAULT_SOLVER_PC ;*/
  /*solvopt->solver_eps = DEFAULT_SOLVER_EPS ;*/
  solvopt->solver_div = DEFAULT_SOLVER_DIV ;
  solvopt->solver_ic_shift = DEFAULT_SOLVER_IC_SHIFT ;
  /*solvopt->solver_log = NEED ;*/

  /*solvopt->coJo_sol = DEFAULT_COJO_SOL ;*/
  /*solvopt->coJo_sol_pc = DEFAULT_COJO_SOL_PC ;*/
  solvopt->coJo_sol_eps = DEFAULT_COJO_SOL_EPS ;
  solvopt->coJo_sol_div = DEFAULT_COJO_SOL_DIV ;
  solvopt->coJo_sol_ic_shift = DEFAULT_COJO_SOL_IC_SHIFT ;
  /*solvopt->coJo_sol_log = NEED ;*/
#endif /* __ADVFW_OPTIONS_SOLVER__ */
}

/************************************************************************/

void HDDM_Opt_init( HDDM_Opt *hddm_opt, int field, int analysis )
{
#ifdef __ADVFW_OPTIONS_HDDM__
  /*hddm_opt->hddm_solver = DEFAULT_HDDM_SOLVER ;*/
  /*hddm_opt->hddm_pc = DEFAULT_HDDM_PC ;*/
  hddm_opt->hddm_eps = DEFAULT_HDDM_EPS ;
  if( field == FIELD_ELECTROMAGNETIC ) {
    if( analysis == ANALYSIS_FULL_WAVECURRENT ) {
      hddm_opt->hddm_eps = DEFAULT_HDDM_EPS ;
    } else {
      hddm_opt->hddm_eps = DEFAULT_HDDM_EPS ;
    }
  }
  hddm_opt->hddm_div = DEFAULT_HDDM_DIV ;
  hddm_opt->hddm_nl_div = DEFAULT_HDDM_NL_DIV ;
  hddm_opt->hddm_max_loop = DEFAULT_HDDM_MAX_LOOP ;
  /*hddm_opt->hddm_log = NEED ;*/

  /*hddm_opt->coJo_hddm_sol = DEFAULT_COJO_HDDM_SOL ;*/
  /*hddm_opt->coJo_hddm_pc = DEFAULT_COJO_HDDM_PC ;*/
  hddm_opt->coJo_hddm_eps = DEFAULT_COJO_HDDM_EPS ;
  hddm_opt->coJo_hddm_div = DEFAULT_COJO_HDDM_DIV ;
  hddm_opt->coJo_hddm_max_loop = DEFAULT_COJO_HDDM_MAX_LOOP ;
  /*hddm_opt->coJo_hddm_log = NEED ;*/
#endif /* __ADVFW_OPTIONS_HDDM__ */
}

/************************************************************************/

void UNIT_init( UNIT *unit )
{
#ifdef __ADVFW_OPTIONS_UNIT_CURRENT_DENSITY__
  unit->CurrentDensity = DEFAULT_UNIT_CURRENT_DENSITY ;
#endif /* __ADVFW_OPTIONS_UNIT_CURRENT_DENSITY__ */
#ifdef __ADVFW_OPTIONS_UNIT_ELECTROMAGNETIC_FIELD__
  unit->MagneticField = DEFAULT_UNIT_ELECTROMAGNETIC_FIELD ;
#endif /* __ADVFW_OPTIONS_UNIT_ELECTROMAGNETIC_FIELD__ */
}

/************************************************************************/

void OPTIONS_init( OPTIONS *options, int work, int field, int analysis )
{
  options->work = work ;
  options->field = field ;
  options->analysis = analysis ;
#ifdef __ADVFW_OPTIONS_FORMULATION__
  if( field == FIELD_ELECTROMAGNETIC ) {
    if( analysis == ANALYSIS_FULL_WAVECURRENT ) {
      options->formulation = FORMULATION_FULL_WAVECURRENT ;
    } else {
      options->formulation = FORMULATION_UNKNOWN ;
    }
  }
#endif /* __ADVFW_OPTIONS_FORMULATION__ */

#ifdef __ADVFW_OPTIONS_MAKEFEM__
  options->trnFile = NO_NEED ;
  options->tmpFile = NO_NEED ;
  options->femFile = NO_NEED ;
  options->crd_magnification = 1.0 ;
#endif /* __ADVFW_OPTIONS_MAKEFEM__ */

  options->memlimit = MEMLIMIT ;

}

/************************************************************************/

void PrintUsage( char *commandname, char *mname, char *version,
                 char *usage )
{
  /*+ Function: Print main help  +*/
  
  int    itmp;
  char   str[128] ;


  Fprintf( stdout, "%s Ver.%s\n", ADVFW_NAME, version ) ;
  Fprintf( stdout, "  >>> %s <<<\n", mname ) ;
#ifndef __ADVFW_OPTIONS_MAKEFEM__
#ifdef __ADVFW_PARALLEL_MODE__
  Fprintf( stdout,
       "  Usage : mpirun  [options for mpirun]  %s %s [options]  data_dir\n",
          commandname, usage ) ;
#else
#ifdef __ADVFW_HIERARCHICAL_MODE__
  Fprintf( stdout,
       "  Usage : mpirun  [options for mpirun]  %s %s [options]  data_dir\n",
          commandname, usage ) ;
#else
  Fprintf( stdout, "  Usage : %s %s [options]  data_dir\n", commandname,
           usage ) ;
#endif
#endif
#else
  Fprintf( stdout, "  Usage : %s %s [options] \n", commandname, usage ) ;
#endif
  Fprintf( stdout, "\n") ;

  Fprintf(stdout,"***** Main input/output file names *****\n" );

  /*sprintf( str, " %s <dir>", OPTION_STRING_DATA_DIR ) ;
  Fprintf( stdout, "%-26s: Top directory of input/output data files [%s]\n",
           str, DEFAULT_DATA_DIR ) ;*/

#ifdef __ADVFW_OPTIONS_FWDATA__
  sprintf( str, " %s <dir>", OPTION_STRING_FWDATA_DIR ) ;
  Fprintf( stdout, "%-26s: Directory of Mag format file [%s]\n",
           str, DEFAULT_FWDATA_DIR ) ;
  sprintf( str, " %s <file>", OPTION_STRING_FWDATA_FILE ) ;
  Fprintf( stdout, "%-26s: File name of Mag format file [%s]\n",
           str, DEFAULT_FWDATA_FILE ) ;
#endif /* __ADVFW_OPTIONS_FWDATA__ */

#ifdef __ADVFW_OPTIONS_ONEDATA__
  sprintf( str, " %s <dir>", OPTION_STRING_ONEDATA_DIR ) ;
  Fprintf( stdout, "%-26s: Directory of one domain model file [%s]\n",
           str, DEFAULT_ONEDATA_DIR ) ;
  sprintf( str, " %s <file>", OPTION_STRING_ONEDATA_FILE ) ;
  Fprintf( stdout, "%-26s: File name of one domain model file [%s]\n",
           str, DEFAULT_ONEDATA_FILE ) ;
#endif /* __ADVFW_OPTIONS_ONEDATA__ */

#ifdef __ADVFW_OPTIONS_MODELDATA__
  sprintf( str, " %s <dir>", OPTION_STRING_MODELDATA_DIR ) ;
  Fprintf( stdout, "%-26s: Directory of HDDM file [%s]\n",
           str, DEFAULT_MODELDATA_DIR ) ;
  sprintf( str, " %s <file>", OPTION_STRING_MODELDATA_FILE ) ;
  Fprintf( stdout, "%-26s: File name of HDDM file [%s]\n",
           str, DEFAULT_MODELDATA_FILE ) ;
#endif /* __ADVFW_OPTIONS_MODELDATA__ */

#ifdef __ADVFW_OPTIONS_JOMETIS__
  sprintf( str, " %s <dir>", OPTION_STRING_JOMETIS_DIR ) ;
  Fprintf( stdout, "%-26s: Directory of Jo metis file [%s]\n",
           str, DEFAULT_JOMETIS_DIR ) ;
  sprintf( str, " %s <file>", OPTION_STRING_JOMETIS_FILE ) ;
  Fprintf( stdout, "%-26s: File name of Jo metis file [%s]\n",
           str, DEFAULT_JOMETIS_FILE ) ;
#endif /* __ADVFW_OPTIONS_JOMETIS__ */

#ifdef __ADVFW_OPTIONS_INIDATA__
  sprintf( str, " %s <dir>", OPTION_STRING_INIDATA_DIR ) ;
  Fprintf( stdout, "%-26s: Directory of Ini file [%s]\n",
           str, DEFAULT_INIDATA_DIR ) ;
  sprintf( str, " %s <file>", OPTION_STRING_INIDATA_FILE ) ;
  Fprintf( stdout, "%-26s: File name of Ini file [%s]\n",
           str, DEFAULT_INIDATA_FILE ) ;
#endif /* __ADVFW_OPTIONS_INIDATA__ */

#ifdef __ADVFW_OPTIONS_INIMETIS__
  sprintf( str, " %s <dir>", OPTION_STRING_INIMETIS_DIR ) ;
  Fprintf( stdout, "%-26s: Directory of Ini metis file [%s]\n",
           str, DEFAULT_INIMETIS_DIR ) ;
  sprintf( str, " %s <file>", OPTION_STRING_INIMETIS_FILE ) ;
  Fprintf( stdout, "%-26s: File name of Ini metis file [%s]\n",
           str, DEFAULT_INIMETIS_FILE ) ;
#endif /* __ADVFW_OPTIONS_INIMETIS__ */

#ifdef __ADVFW_OPTIONS_OUTDATA__
  sprintf( str, " %s <dir>", OPTION_STRING_OUTDATA_DIR ) ;
  Fprintf( stdout, "%-26s: Directory of answer file [%s]\n",
           str, DEFAULT_OUTDATA_DIR ) ;
  sprintf( str, " %s <file>", OPTION_STRING_OUTDATA_FILE ) ;
  Fprintf( stdout, "%-26s: File name of answer file [%s]\n",
           str, DEFAULT_OUTDATA_FILE ) ;
#endif /* __ADVFW_OPTIONS_OUTDATA__ */

#ifdef __ADVFW_OPTIONS_AVSFILE__
  sprintf( str, " %s", OPTION_STRING_AVSFILE ) ;
  Fprintf( stdout, "%-26s: Output file written by AVS format\n", str ) ;
  sprintf( str, " %s", OPTION_STRING_AVSFILE_NO_NEED ) ;
  Fprintf( stdout, "%-26s: Not output file written by AVS format\n", str ) ;
  sprintf( str, " %s", OPTION_STRING_AVSFILE_MICRO ) ;
  Fprintf( stdout, "%-26s: Output file written by MicroAVS format\n", str ) ;
  sprintf( str, " %s", OPTION_STRING_VTKFILE ) ;
  Fprintf( stdout, "%-26s: Output file written by VTK format\n", str ) ;
  sprintf( str, " %s <dir>", OPTION_STRING_AVSFILE_DIR ) ;
  Fprintf( stdout,
           "%-26s: Directory of result file written by AVS format [%s]\n",
           str, DEFAULT_AVSFILE_DIR ) ;
  sprintf( str, " %s <file>", OPTION_STRING_AVSFILE_FILE ) ;
  Fprintf( stdout,
           "%-26s: File name of result file written by AVS format [%s]\n",
           str, DEFAULT_AVSFILE_FILE ) ;
  sprintf( str, " %s <dir>", OPTION_STRING_VTKFILE_DIR ) ;
  Fprintf( stdout,
           "%-26s: Directory of result file written by VTK format [%s]\n",
           str, DEFAULT_VTKFILE_DIR ) ;
  sprintf( str, " %s <file>", OPTION_STRING_VTKFILE_FILE ) ;
  Fprintf( stdout,
           "%-26s: File name of result file written by VTK format [%s]\n",
           str, DEFAULT_VTKFILE_FILE ) ;
#endif /* __ADVFW_OPTIONS_AVSFILE__ */

#ifdef __ADVFW_OPTIONS_CALCLOG__
  sprintf( str, " %s <dir>", OPTION_STRING_CALCLOG_DIR ) ;
  Fprintf( stdout, "%-26s: Directory of log file [%s]\n",
           str, DEFAULT_CALCLOG_DIR ) ;
  sprintf( str, " %s <file>", OPTION_STRING_CALCLOG_FILE ) ;
  Fprintf( stdout, "%-26s: File name of log file [%s]\n",
           str, DEFAULT_CALCLOG_FILE ) ;
#endif /* __ADVFW_OPTIONS_CALCLOG__ */

#ifdef __ADVFW_OPTIONS_MTRLDAT__
  sprintf( str, " %s <dir>", OPTION_STRING_MTRLDAT_DIR ) ;
  Fprintf( stdout, "%-26s: Directory of material file [%s]\n",
           str, DEFAULT_MTRLDAT_DIR ) ;
  sprintf( str, " %s <file>", OPTION_STRING_MTRLDAT_FILE ) ;
  Fprintf( stdout, "%-26s: File name of material file [%s]\n",
           str, DEFAULT_MTRLDAT_FILE ) ;
#endif /* __ADVFW_OPTIONS_MTRLDAT__ */


  Fprintf( stdout, "\n" ) ;
  Fprintf( stdout, "***** Options for Analysis *****\n" ) ;
#ifdef __ADVFW_OPTIONS_FORMULATION__
  sprintf( str, " %s <s>", OPTION_STRING_FORMULATION ) ;
  Fprintf( stdout, "%-26s: Selection of formulation\n", str ) ;
  Fprintf( stdout, "%-29s%-5s: E Method\n", " ",
           FORMULATION_FW_EMETHOD_STRING ) ;

#endif /* __ADVFW_OPTIONS_FORMULATION__ */


#ifdef __ADVFW_OPTIONS_SOLVER__
  Fprintf( stdout, "\n" ) ;
  Fprintf( stdout, "***** Options for Solver *****\n" ) ;
  /*sprintf( str, " %s <s>", OPTION_STRING_SOLVER ) ;
  Fprintf( stdout, "%-26s: Selection of Solver\n", str ) ;
  Fprintf( stdout, "%-29s%d: CG method\n", " ", SOLVER_CGMETHOD ) ;*/

  sprintf( str, " %s <s>", OPTION_STRING_SOLVER_PC ) ;
  Fprintf( stdout, "%-26s: Selection of preconditioner\n", str ) ;
  Fprintf( stdout, "%-29s%-5s: not use PC\n", " ", SOLVER_PC_NONE_STRING ) ;
  Fprintf( stdout, "%-29s%-5s: imcomplete Cholesky factorization\n", " ",
           SOLVER_PC_ICC_STRING ) ;

  sprintf( str, " %s <x>", OPTION_STRING_SOLVER_IC_SHIFT ) ;
  Fprintf( stdout, "%-26s: Set shift value of ICC to x [%g]\n",
           str, DEFAULT_SOLVER_IC_SHIFT ) ;

  sprintf( str, " %s <x>", OPTION_STRING_SOLVER_EPS ) ;
  Fprintf( stdout, "%-26s: Set Solver tolerance to x [%e]\n",
           str, DEFAULT_SOLVER_EPS ) ;

  sprintf( str, " %s <x>", OPTION_STRING_SOLVER_DIV ) ;
  Fprintf( stdout, "%-26s: Set Solver divergent to x [%e]\n",
           str, DEFAULT_SOLVER_DIV ) ;

  sprintf( str, " %s", OPTION_STRING_SOLVER_LOG ) ;
  Fprintf( stdout, "%-26s: Monitor Solver residual norms\n", str ) ;
  sprintf( str, " %s", OPTION_STRING_SOLVER_NO_LOG ) ;
  Fprintf( stdout, "%-26s: Not monitor Solver residual norms\n", str ) ;


  /*sprintf( str, " %s <s>", OPTION_STRING_COJO_SOL ) ;
  Fprintf( stdout, "%-26s: Selection of Solver for correciton Jo\n", str ) ;
  Fprintf( stdout, "%-29s%d: CG method\n", " ", SOLVER_CGMETHOD ) ;*/

  sprintf( str, " %s <n>", OPTION_STRING_COJO_SOL_PC ) ;
  Fprintf( stdout, "%-26s: Selection of preconditioner for correciton Jo\n",
           str ) ;
  Fprintf( stdout, "%-29s%-5s: not use PC\n", " ", SOLVER_PC_NONE_STRING ) ;
  Fprintf( stdout, "%-29s%-5s: imcomplete Cholesky factorization\n", " ",
           SOLVER_PC_ICC_STRING ) ;

  sprintf( str, " %s <x>", OPTION_STRING_COJO_SOL_IC_SHIFT ) ;
  Fprintf( stdout,
           "%-26s: Set shift value of ICC for correciton Jo to x [%g]\n",
           str, DEFAULT_COJO_SOL_IC_SHIFT ) ;

  sprintf( str, " %s <x>", OPTION_STRING_COJO_SOL_EPS ) ;
  Fprintf( stdout, "%-26s: Set Solver tolerance for correciton Jo to x [%e]\n",
           str, DEFAULT_COJO_SOL_EPS ) ;

  sprintf( str, " %s <x>", OPTION_STRING_COJO_SOL_DIV ) ;
  Fprintf( stdout, "%-26s: Set Solver divergent for correciton Jo to x [%e]\n",
           str, DEFAULT_COJO_SOL_DIV ) ;

  sprintf( str, " %s", OPTION_STRING_COJO_SOL_LOG ) ;
  Fprintf( stdout, "%-26s: Monitor Solver residual norms of correciton Jo\n",
           str ) ;
  sprintf( str, " %s", OPTION_STRING_COJO_SOL_NO_LOG ) ;
  Fprintf( stdout,
           "%-26s: Not monitor Solver residual norms of correciton Jo\n",
           str ) ;
#endif /* __ADVFW_OPTIONS_SOLVER__ */


#ifdef __ADVFW_OPTIONS_HDDM__
  Fprintf( stdout, "\n" ) ;
  Fprintf( stdout, "***** Options for HDDM *****\n" ) ;
  /*sprintf( str, " %s <s>", OPTION_STRING_HDDM_SOLVER ) ;
  Fprintf( stdout, "%-26s: Selection of HDDM Solver\n", str ) ;
  Fprintf( stdout, "%-29s%d: CG method\n", " ", HDDM_CGMETHOD ) ;*/

  sprintf( str, " %s <s>", OPTION_STRING_HDDM_PC ) ;
  Fprintf( stdout,
           "%-26s: Selection of preconditioner for HDDM Solver\n", str ) ;
  Fprintf( stdout, "%-29s%-5s: not use PC\n", " ", HDDM_PC_NONE_STRING ) ;
  Fprintf( stdout, "%-29s%-5s: Diagonal Scaling\n", " ",
           HDDM_PC_DIAG_STRING ) ;

  sprintf( str, " %s <x>", OPTION_STRING_HDDM_EPS ) ;
  Fprintf( stdout, "%-26s: Set HDDM Solver tolerance to x [%e]\n",
           str, DEFAULT_HDDM_EPS ) ;

  sprintf( str, " %s <x>", OPTION_STRING_HDDM_DIV ) ;
  Fprintf( stdout, "%-26s: Set HDDM Solver divergent to x [%e]\n",
           str, DEFAULT_HDDM_DIV ) ;

  sprintf( str, " %s <n>", OPTION_STRING_HDDM_MAX_LOOP ) ;
  Fprintf( stdout, "%-26s: Set the limit of number of iteration of HDDM Solver to n [%d]\n",
           str, DEFAULT_HDDM_MAX_LOOP ) ;

  sprintf( str, " %s", OPTION_STRING_HDDM_LOG ) ;
  Fprintf( stdout, "%-26s: Monitor HDDM Solver residual norms\n", str ) ;
  sprintf( str, " %s", OPTION_STRING_HDDM_NO_LOG ) ;
  Fprintf( stdout, "%-26s: Not monitor HDDM Solver residual norms\n", str ) ;


  /*sprintf( str, " %s <s>", OPTION_STRING_COJO_HDDM_SOL ) ;
  Fprintf( stdout,
           "%-26s: Selection of HDDM Solver for correciton Jo\n", str ) ;
  Fprintf( stdout, "%-29s%d: CG method\n", " ", HDDM_CGMETHOD ) ;*/

  sprintf( str, " %s <s>", OPTION_STRING_COJO_HDDM_PC ) ;
  Fprintf( stdout, "%-26s: Selection of preconditioner for HDDM Solver for correciton Jo\n",
           str ) ;
  Fprintf( stdout, "%-29s%-5s: NONE\n", " ", HDDM_PC_NONE_STRING ) ;
  Fprintf( stdout, "%-29s%-5s: Diagonal Scaling\n", " ",
           HDDM_PC_DIAG_STRING ) ;

  sprintf( str, " %s <x>", OPTION_STRING_COJO_HDDM_EPS ) ;
  Fprintf( stdout,
           "%-26s: Set HDDM Solver tolerance for correciton Jo to x [%e]\n",
           str, DEFAULT_COJO_HDDM_EPS ) ;

  sprintf( str, " %s <x>", OPTION_STRING_COJO_HDDM_DIV ) ;
  Fprintf( stdout,
           "%-26s: Set HDDM Solver divergent for correciton Jo to x [%e]\n",
           str, DEFAULT_COJO_HDDM_DIV ) ;

  sprintf( str, " %s <n>", OPTION_STRING_COJO_HDDM_MAX_LOOP ) ;
  Fprintf( stdout,  "%-26s: Set the limit of number of iteration of HDDM Solver for correction Jo to n [%d]\n",
           str, DEFAULT_COJO_HDDM_MAX_LOOP ) ;

  sprintf( str, " %s", OPTION_STRING_COJO_HDDM_LOG ) ;
  Fprintf( stdout,
           "%-26s: Monitor HDDM Solver residual norms of correciton Jo\n",
           str ) ;
  sprintf( str, " %s", OPTION_STRING_COJO_HDDM_NO_LOG ) ;
  Fprintf( stdout,
           "%-26s: Not monitor HDDM Solver residual norms of correciton Jo\n",
           str ) ;
#endif /* __ADVFW_OPTIONS_HDDM__ */


  Fprintf( stdout, "\n" ) ;
  Fprintf( stdout, "***** Misc. options  *****\n" ) ;
  itmp = MEMLIMIT/(1024*1024) ;
  sprintf( str, " %s <n>", OPTION_STRING_MEMLIMIT ) ;
  if (itmp == 0 ){
    Fprintf( stdout,
             "%-26s: Set upper limit for memory to use in Mbytes [None]\n",
             str ) ;
  } else {
    Fprintf( stdout,
             "%-26s: Set upper limit for memory to use in Mbytes [%d]\n",
             str, itmp ) ;
  }

#ifdef __ADVFW_OPTIONS_UNIT_CURRENT_DENSITY__
  sprintf( str, " %s <s>", OPTION_STRING_UNIT_CURRENT_DENSITY ) ;
  Fprintf( stdout,
           "%-26s: Selection of unit of Eddy Current Density\n", str ) ;
  Fprintf( stdout, "%-29s%-5s: [A/m^2]\n", " ", UNIT_A_m2_STRING ) ;
  Fprintf( stdout, "%-29s%-5s: [A/mm^2]\n", " ", UNIT_A_mm2_STRING ) ;
#endif /* __ADVFW_OPTIONS_UNIT_CURRENT_DENSITY__ */

#ifdef __ADVFW_OPTIONS_UNIT_ELECTROMAGNETIC_FIELD__
  sprintf( str, " %s <s>", OPTION_STRING_UNIT_ELECTROMAGNETIC_FIELD ) ;
  Fprintf( stdout,
           "%-26s: Selection of unit of Magnetic Flux Density\n", str ) ;
  Fprintf( stdout, "%-29s%-5s: [T]\n", " ", UNIT_T_STRING ) ;
  Fprintf( stdout, "%-29s%-5s: [G]\n", " ", UNIT_G_STRING ) ;
  Fprintf( stdout, "%-29s%-5s: [A/m]\n", " ", UNIT_A_m_STRING ) ;
  Fprintf( stdout, "%-29s%-5s: [A/mm]\n", " ", UNIT_A_mm_STRING ) ;
#endif /* __ADVFW_OPTIONS_UNIT_ELECTROMAGNETIC_FIELD__ */

#ifdef __ADVFW_OPTIONS_MAKEFEM__
  /*sprintf( str, " %s <file>", OPTION_STRING_MAKEFEM_TRNFILE ) ;
  Fprintf( stdout, "%-26s: Read grobal index file\n", str ) ;
  sprintf( str, " %s <file>", OPTION_STRING_MAKEFEM_FEMFILE ) ;
  Fprintf( stdout, "%-26s: Read temperature file\n", str ) ;
  sprintf( str, " %s <file>", OPTION_STRING_MAKEFEM_TMPFILE ) ;
  Fprintf( stdout, "%-26s: Write fem file by ascii\n", str ) ;
  sprintf( str, " %s <x>", OPTION_STRING_MAKEFEM_CRD_FWNIFICATION ) ;
  Fprintf( stdout, "%-26s: Change scale [1.0]\n", str ) ;*/
#endif /* __ADVFW_OPTIONS_MAKEFEM__ */

  sprintf( str, " %s|%s", OPTION_STRING_V, OPTION_STRING_VERSION ) ;
  Fprintf( stdout, "%-26s: Print version\n", str ) ;
  sprintf( str, " %s|%s", OPTION_STRING_H, OPTION_STRING_HELP ) ;
  Fprintf( stdout, "%-26s: Print this help\n", str ) ;
}

#endif /* __OPTIONS_FUNC_H__ */
