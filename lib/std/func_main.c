/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "func_main.h"


extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;


void __MainInit__( int argc, char **argv, OPTIONS *options,
                   char *mname, char *version,
                   int work, int field,  int analysis,
                   int n_argument, char *usage,
                   char *now_time, char *today )
{
  /*+ Function: main initialize +*/
  int    i ;
  char   fname[FILENAME_MAX] ;
  int    npart = 1 ;


  COM_Init( &argc, &argv ) ;
  setTimer() ;
  HelpOptions( argc, argv, mname, version, usage ) ;
  /*** Print initial messages by root ***/

printf("Print initial messages by root\n");

  Fprintf( stdout,
           "---------------------------------------------------------\n" );
  Fprintf( stdout, "Starting %s (Version: %s)\n", argv[0], version );
  Fprintf( stdout, "   (This program was compiled at %s on %s)\n",
           now_time, today ) ;
  Fprintf( stdout, "Date: %s", cTime() );
  Fprintf( stdout, "Selected Options:\n");
  SetOptions( argc, argv, options, mname, version, work, field,
              analysis, n_argument, usage ) ;
  Fprintf( stdout,
          "---------------------------------------------------------\n" );
  mem_Init( options->memlimit ) ;


  /*** Get num_part from indata file ***/
printf("Get num start\n");

  if( (work == WORK_HDDM_ANALYSIS)
      || (work == WORK_HDDM_METIS)
      || (work == WORK_POST_MERGE)
      || (work == WORK_POST_HDDM_CALC) ) {
    if( COM_GID.rootid == COM_ID.myprocid ){
      make_modeldata_filename( fname, options->datasw, 0 );
      npart = get_num_part_from_modeldata( fname );
      if( npart <=0 ) {
        emessage( 200, __ABORT_ADVFW_LIB__, fname ) ;
      }
    }
  }
printf("Get num end\n");


  COM_Bcast_Int( &npart, 1, COM_GID.rootid ) ;
  COM_SetGroup( npart ) ;


  Fprintf( stdout, "\n" ) ;
  for( i=0 ; i<COM_ID.nproc ; i++ ) {
    if( i == COM_ID.myprocid ) {
      char   hostname[FILENAME_MAX] ;

      gethostname( hostname, FILENAME_MAX ) ;
      Pfprintf( stdout, "Host: %s, Rank %d, Group: %d, RankinGroup: %d",
                hostname, COM_ID.myprocid, COM_GID.group,
                COM_GID.mygprocid, getpid() ) ;
#ifdef __ADVFW_OS_UNIX__
      fprintf( stdout, ", System PID: %d", getpid() ) ;
#endif /* __ADVFW_OS_UNIX__ */
      fprintf( stdout, "\n" ) ;
      fflush( stdout ) ;
    }
    COM_Synchronize();
  }
}

/********************************************************************/

void main_Finalize( void )
{
  /*+ Function: main finalize +*/
  int    i ;


  /* Print Memory Usage */
  Fprintf( stdout, "\n" ) ;
  for( i=0 ; i<COM_ID.nproc ; i++ ) {
    if( i == COM_ID.myprocid ) {
      Pfprintf( stdout, "Used Max Memory [Byte]: %lu\n",
                getMem_usemax() ) ;
      mem_Finalize() ;
    }
    COM_Synchronize() ;
  }


  Fprintf( stdout, "\n" ) ;
  /* Print All Time */
  Fprintf( stdout, "All time %g[s]\n", nowTime() );
  Fprintf( stdout, "Finish date: %s", cTime());


  COM_Finalize() ;
}

/********************************************************************/

void __MainPrintCondition__( OPTIONS options,
                             int nel, int nap, int nmp,
                             char *elem_type, int nf,
                             char *mname )
{
  /*+ Function: print conditions +*/
  if( COM_GID.rootid == COM_ID.myprocid ){
    Fprintf( stdout, "\n" ) ;
    Fprintf( stdout, "\n" ) ;
    Fprintf( stdout,
             "------------------ Main Calc Condition ------------------\n" );
    Fprintf( stdout, "%s\n", mname ) ;
    Fprintf( stdout, "  %-13s : ", "Work" ) ;
    switch( options.work ) {
      case WORK_PRE_MAKEMODEL :
        fprintf( stdout, "\n" ) ;
        break ;
      case WORK_PRE_PATCHGENERATE :
        fprintf( stdout, "\n" ) ;
        break ;
      case WORK_PRE_MESHGENERATE :
        fprintf( stdout, "\n" ) ;
        break ;
      case WORK_PRE_BCTOOL :
        fprintf( stdout, "\n" ) ;
        break ;
      case WORK_PRE_FILECONVERT :
        fprintf( stdout, "File Converter\n" ) ;
        break ;
      case WORK_PRE_MAKEDATA :
        fprintf( stdout, "Making data\n" ) ;
        break ;
      case WORK_FEM_ANALYSIS :
        fprintf( stdout, "Analysis with FEM\n" ) ;
        break ;
      case WORK_HDDM_ANALYSIS :
        fprintf( stdout, "Analysis with HDDM\n" ) ;
        break ;
      case WORK_POST_MERGE :
        fprintf( stdout, "\n" ) ;
        break ;
      case WORK_POST_FEM_CALC :
        fprintf( stdout, "Post for FEM\n" ) ;
        break ;
      case WORK_POST_HDDM_CALC :
        fprintf( stdout, "Post for HDDM\n" ) ;
        break ;
      case WORK_POST_VISUALIZE :
        fprintf( stdout, "\n" ) ;
        break ;

      default :
        fprintf( stdout, "Unknown\n" ) ;
    }
    Fprintf( stdout, "  %-13s : ", "Field" ) ;
    switch( options.field ) {
     case FIELD_ELECTROMAGNETIC :
        fprintf( stdout, "ElectroMagnetic Fild Analysis\n" ) ;
        break ;

      default :
        fprintf( stdout, "Unknown\n" ) ;
    }
    Fprintf( stdout, "  %-13s : ", "Analysis" ) ;
    switch( options.analysis ) {
      case ANALYSIS_FULL_WAVECURRENT :
        fprintf( stdout, "Full-Wave Analysis\n" ) ;
        break ;

      default :
        fprintf( stdout, "Unknown\n" ) ;
    }
    Fprintf( stdout, "  %-13s : ", "Formulation" ) ;
    switch( options.formulation ) {
      case FORMULATION_FW_EMETHOD :
        fprintf( stdout, "E method\n" ) ;
        break ;
      
      default :
        fprintf( stdout, "Unknown\n" ) ;
    }


    if( (options.work == WORK_FEM_ANALYSIS)
        || (options.work == WORK_HDDM_ANALYSIS) ) {
      if( options.work == WORK_FEM_ANALYSIS ) {
        Fprintf( stdout, "  Linear solver's condtions\n" ) ;
      } else {
        Fprintf( stdout, "  Linear solver's condtions in subdomain\n" ) ;
      }
      Fprintf( stdout, "    %-20s : ", "Solver" ) ;
      switch( options.solvopt.solver ) {
        case SOLVER_DIRECT :
          fprintf( stdout, "Direct Solver\n" ) ;
          break ;
        case SOLVER_CGMETHOD :
          fprintf( stdout, "Conjugate Gradient method\n" ) ;
          break ;
        case SOLVER_COCG :
          fprintf( stdout, "\n" ) ;
          Fprintf( stdout,
                   "      Conjugate Orthogonal Conjugate Gradient method\n" ) ;
          break ;
        case SOLVER_LU :
          fprintf( stdout, "Direct method:LU\n" ) ;
          break ;
        default :
          fprintf( stdout, "Unknown\n" ) ;
      }
      Fprintf( stdout, "    %-20s : ", "Preconditioner" ) ;
      switch( options.solvopt.solver_pc ) {
        case SOLVER_PC_NONE :
          fprintf( stdout, "None\n" ) ;
          break ;
        case SOLVER_PC_DIAG :
          fprintf( stdout, "Diagonal scaling\n" ) ;
          break ;
        case SOLVER_PC_JACOBI :
          fprintf( stdout, "Jacobi method\n" ) ;
          break ;
        case SOLVER_PC_ICC :
          fprintf( stdout, "Incomplete Cholesky factorization\n" ) ;
          Fprintf( stdout, "      %-20s : %le\n", "Shited value of IC",
                   options.solvopt.solver_ic_shift ) ;
          break ;
        case SOLVER_PC_ILDU :
          fprintf( stdout, "iLDU\n" ) ;
          break ;
        case SOLVER_PC_SOR :
          fprintf( stdout, "SOR\n" ) ;
          break ;

        default :
          fprintf( stdout, "Unknown\n" ) ;
      }
      Fprintf( stdout, "    %-20s : %le\n", "EPS",
               options.solvopt.solver_eps ) ;
      Fprintf( stdout, "    %-20s : %le\n", "Divergence Criterion",
               options.solvopt.solver_div ) ;
    }


    if( options.work == WORK_HDDM_ANALYSIS ) {
      Fprintf( stdout, "  HDDM solver's condtions\n" ) ;
      Fprintf( stdout, "    %-20s : ", "Solver" ) ;
      switch( options.hddm_opt.hddm_solver ) {
        case HDDM_DIRECT :
          fprintf( stdout, "Direct Solver\n" ) ;
          break ;
        case HDDM_CGMETHOD :
          fprintf( stdout, "Conjugate Gradient method\n" ) ;
          break ;
        case HDDM_COCG :
          fprintf( stdout, "\n" ) ;
          Fprintf( stdout,
                   "      Conjugate Orthogonal Conjugate Gradient method\n" ) ;
          break ;

        default :
          fprintf( stdout, "Unknown\n" ) ;
      }
      Fprintf( stdout, "    %-20s : ", "Preconditioner" ) ;
      switch( options.hddm_opt.hddm_pc ) {
        case HDDM_PC_NONE :
          fprintf( stdout, "None\n" ) ;
          break ;
        case HDDM_PC_DIAG :
          fprintf( stdout, "Diagonal scaling\n" ) ;
          break ;

        default :
          fprintf( stdout, "Unknown\n" ) ;
      }
      Fprintf( stdout, "    %-20s : %le\n", "EPS",
               options.hddm_opt.hddm_eps ) ;
      Fprintf( stdout, "    %-20s : %le\n", "Divergence Criterion",
               options.hddm_opt.hddm_div ) ;
    }


    if( (options.field == FIELD_ELECTROMAGNETIC)
        && ((options.work == WORK_FEM_ANALYSIS)
            || (options.work == WORK_HDDM_ANALYSIS)) ) {
      if( options.work == WORK_FEM_ANALYSIS ) {
        Fprintf( stdout, "  Linear solver's condtions for Correction Jo\n" ) ;
      } else {
        Fprintf( stdout,
                 "  Linear solver's conditions for Correction Jo in subdomain\n" ) ;
      }
      Fprintf( stdout, "    %-20s : ", "Solver" ) ;
      switch( options.solvopt.coJo_sol ) {
        case SOLVER_DIRECT :
          fprintf( stdout, "Direct Solver\n" ) ;
          break ;
        case SOLVER_CGMETHOD :
          fprintf( stdout, "Conjugate Gradient method\n" ) ;
          break ;
        case SOLVER_COCG :
          fprintf( stdout, "\n" ) ;
          Fprintf( stdout,
                   "      Conjugate Orthogonal Conjugate Gradient method\n" ) ;
          break ;

        default :
          fprintf( stdout, "Unknown\n" ) ;
      }
      Fprintf( stdout, "    %-20s : ", "Preconditioner" ) ;
      switch( options.solvopt.coJo_sol_pc ) {
        case SOLVER_PC_NONE :
          fprintf( stdout, "None\n" ) ;
          break ;
        case SOLVER_PC_DIAG :
          fprintf( stdout, "Diagonal scaling\n" ) ;
          break ;
        case SOLVER_PC_JACOBI :
          fprintf( stdout, "Jacobi method\n" ) ;
          break ;
        case SOLVER_PC_ICC :
          fprintf( stdout, "Incomplete Cholesky factorization\n" ) ;
          Fprintf( stdout, "      %-20s : %le\n", "Shited value of IC",
                   options.solvopt.coJo_sol_ic_shift ) ;
          break ;
        case SOLVER_LU :
          fprintf( stdout, "Direct method:LU\n" ) ;
          break ;
        case SOLVER_PC_ILDU :
          fprintf( stdout, "iLDU\n" ) ;
          break ;
        case SOLVER_PC_SOR :
          fprintf( stdout, "SOR\n" ) ;
          break ;

        default :
          fprintf( stdout, "Unknown\n" ) ;
      }
      Fprintf( stdout, "    %-20s : %le\n", "EPS",
               options.solvopt.coJo_sol_eps ) ;
      Fprintf( stdout, "    %-20s : %le\n", "Divergence Criterion",
               options.solvopt.coJo_sol_div ) ;
    }


    if( (options.field == FIELD_ELECTROMAGNETIC)
        && (options.work == WORK_HDDM_ANALYSIS) ) {
      Fprintf( stdout, "  HDDM solver's condtions for Correction Jo\n" ) ;
      Fprintf( stdout, "    %-20s : ", "Solver" ) ;
      switch( options.hddm_opt.coJo_hddm_sol ) {
        case HDDM_DIRECT :
          fprintf( stdout, "Direct Solver\n" ) ;
          break ;
        case HDDM_CGMETHOD :
          fprintf( stdout, "Conjugate Gradient method\n" ) ;
          break ;
        case HDDM_COCG :
          fprintf( stdout, "\n" ) ;
          Fprintf( stdout,
                   "      Conjugate Orthogonal Conjugate Gradient method\n" ) ;
          break ;

        default :
          fprintf( stdout, "Unknown\n" ) ;
      }
      Fprintf( stdout, "    %-20s : ", "Preconditioner" ) ;
      switch( options.hddm_opt.coJo_hddm_pc ) {
        case HDDM_PC_NONE :
          fprintf( stdout, "None\n" ) ;
          break ;
        case HDDM_PC_DIAG :
          fprintf( stdout, "Diagonal scaling\n" ) ;
          break ;

        default :
          fprintf( stdout, "Unknown\n" ) ;
      }
      Fprintf( stdout, "    %-20s : %le\n", "EPS",
               options.hddm_opt.coJo_hddm_eps ) ;
      Fprintf( stdout, "    %-20s : %le\n", "Divergence Criterion",
               options.hddm_opt.coJo_hddm_div ) ;
    }


    Fprintf( stdout, "  Mesh information\n" ) ;
    Fprintf( stdout, "    %-20s : ", "Type of Element" ) ;
    if( !strcmp(elem_type, ELEM_TYPE_3DLT) ) {
      fprintf( stdout, "3D Linear Tetrahedron\n" ) ;
    } else if( !strcmp(elem_type, ELEM_TYPE_3DQT) ) {
      fprintf( stdout, "3D Quadratic Tetrahedron\n" ) ;
    } else if( !strcmp(elem_type, ELEM_TYPE_3DLH) ) {
      fprintf( stdout, "3D Linear Hexahedron\n" ) ;
    } else if( !strcmp(elem_type, ELEM_TYPE_3DQH) ) {
      fprintf( stdout, "3D Quadratic Hexahedron\n" ) ;
    }
    Fprintf( stdout, "    %-20s : %d\n", "Numbers of Elements", nel ) ;
    if( (options.work == WORK_FEM_ANALYSIS)
        || (options.work == WORK_POST_FEM_CALC) ) {
      if( (!strcmp(elem_type, ELEM_TYPE_3DQT))
          || (!strcmp(elem_type, ELEM_TYPE_3DQH)) ) {
        Fprintf( stdout, "    %-20s : %d\n", "Numbers of 1ry Nodes", nap ) ;
        Fprintf( stdout, "    %-20s : %d\n", "Numbers of 2ry Nodes", nmp ) ;
      } else {
        Fprintf( stdout, "    %-20s : %d\n", "Numbers of Nodes", nap ) ;
        Fprintf( stdout, "    %-20s : %d\n", "Numbers of Sides", nmp ) ;
      }
      Fprintf( stdout, "    %-20s : %d\n", "Degrees of Freedom", nf ) ;
      Fprintf( stdout, "    %-20s : %d\n", "Degrees of Freedom", nf ) ;
    } else if( (options.work == WORK_HDDM_ANALYSIS)
               || (options.work == WORK_POST_HDDM_CALC) ) {
      Fprintf( stdout,
               "    %-20s : %d\n", "Degrees of Freedom on the interface",
               nf ) ;
    }


    Fprintf( stdout,
             "---------------------------------------------------------\n" );
  }
}
