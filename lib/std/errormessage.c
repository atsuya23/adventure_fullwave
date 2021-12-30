/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "errormessage.h"


extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;


int __ErrorMessage__( int ErrorNumber, int AbortNumber,
                      char *str, char *fname, int line )
{
  /*+  Function:  print error messages +*/
  static char buf[1024] ;


  sprintf( buf, "Error(%d):", ErrorNumber ) ;


  switch( ErrorNumber ) {
    /*** libfw 0/999 -1/-999 ***/
    /** libfw/mem 0/9 & -1/-9 **/
    #ifdef __ADVFW_DEBUG__
      case -1 :
        fprintf( stderr,
                 "%s call mem_Init before calling this function\n", buf ) ;
        break ;
      case -2 :
        fprintf( stderr,
                 "%s unsuitable number for current used memory\n",
                 buf ) ;
        break ;
    #endif /* __ADVFW_DEBUG__ */
    case 0 :
      fprintf( stderr, "%s limit of memory is not correct\n", buf ) ;
      break ;


    /** libfw/iofunc 10/19 -10/-19 **/
    case 10 :
      fprintf( stderr, "%s cannot open file \"%s\"\n", buf, str ) ;
      break ;


    /** libfw/comm_mpi 20/29 -20/-29 **/
    case 20 :
      Fprintf( stderr, "%s too few prosesses\n", buf ) ;
      break ;
    case 21 :
      Fprintf( stderr, "%s in Parallel processor mode (p-mode),"
               " number of parts must be equal to number of processors\n",
               buf ) ;
      break ;


    /** libfw/fileio 100/199 -100/-199 **/
    case 100 :
      fprintf( stderr, "%s this file is written by old version %s\n",
               buf, str ) ;
      break ;
    case 110 :
      fprintf( stderr, "%s cannot find \"%s\"\n", buf, str ) ;
      break ;
    case 111 :
      fprintf( stderr, "%s unknown symbol %s\n", buf, str ) ;
      break ;
    case 112 :
      fprintf( stderr, "%s %s this data have already read\n", buf, str ) ;
      break ;
    case 113 :
      fprintf( stderr, "%s propertyN must be zero\n", buf ) ;
      break ;
    case 114 :
      fprintf( stderr, "%s value is improper %s\n", buf, str ) ;
      break ;


    /** about numbers **/
    case 120 :
      fprintf( stderr, "%s unsuitable number of elements %s\n", buf, str ) ;
      break ;
    case 121 :
      fprintf( stderr, "%s unsuitable number of nodes %s\n", buf, str ) ;
      break ;
    case 122 :
      fprintf( stderr, "%s unsuitable number of apexes %s\n", buf, str ) ;
      break ;
    case 123 :
      fprintf( stderr, "%s unsuitable number of middle points %s\n",
               buf, str ) ;
      break ;
    case 124 :
      fprintf( stderr, "%s unsuitable number of apexes per element %s\n",
               buf, str ) ;
      break ;
    case 125 :
      fprintf( stderr,
               "%s unsuitable number of middle points per element %s\n",
               buf, str ) ;
      break ;
    case 126 :
      fprintf( stderr, "%s unknown Element Type \"%s\"\n", buf, str ) ;
      break ;
    case 127 :
      fprintf( stderr, "%s unsuitable dimension %s\n", buf, str ) ;
      break ;
    case 128 :
      fprintf( stderr, "%s unsuitable number of volume %s\n", buf, str ) ;
      break ;

    case 140 :
      fprintf( stderr, "%s unsuitable number of phi %s\n", buf, str ) ;
      break ;

    case 150 :
      fprintf( stderr, "%s unknown Work %s\n", buf, str ) ;
      break ;
    case 151 :
      fprintf( stderr, "%s unknown Field %s\n", buf, str ) ;
      break ;
    case 152 :
      fprintf( stderr, "%s unknown Analysis %s\n", buf, str ) ;
      break ;
    case 153 :
      fprintf( stderr, "%s unknown Formulation %s\n", buf, str ) ;
      break ;
    case 154 :
      fprintf( stderr, "%s unknown HDDM switch %s\n", buf, str ) ;
      break ;
    case 155 :
      fprintf( stderr, "%s unknown Non-linear method %s\n", buf, str ) ;
      break ;


    /** libfw/func_main 200/299 -200/-299 **/
    case 200 :
      fprintf( stderr, "%s Cannot find number of parts in \"%s\"\n",
               buf, str ) ;
      break ;



    /*** fem 1000/1999 -1000/-1999 ***/
    /** mymt_aij_solver/aij_matrix_func 1000/1099 -1000/-1099 **/
    case 1000 :
      fprintf( stderr, "%s in mymt solver ASSEM\n", buf ) ;
      break ;
    case 1010 :
      fprintf( stderr, "%s in mymt solver DELETE ZERO\n", buf ) ;
      break ;


    /** mymt_aij_solver/aij_pc_func 1100/1199 -1100/-1199 **/


    /** mymt_aij_solver/aij_solver_func 1200/1299 -1200/-1299 **/
    case 1200 :
      fprintf( stderr, "%s linear solver divergent\n", buf ) ;
      break ;
    case 1201 :
      fprintf( stderr, "%s linear solver reached max iteration\n", buf ) ;
      break ;
    case 1202 :
      fprintf( stderr, "%s unknown preconditioner type in linear solver\n",
               buf ) ;
      break ;
    case 1203 :
      fprintf( stderr, "%s unknown error in linear solver\n", buf ) ;
      break ;
    case 1204 :
      fprintf( stderr, "%s GMRES restart\n", buf ) ;
      break ;



    /*** fem 2000/2999 -2000/-2999 ***/
    /** fem/make_MESH 2000/2049 -2000/-2049 **/
    case 2000 :
      fprintf( stderr, "%s mtrldat is untuitable to mesh (%s)\n",
               buf, str ) ;
      break ;


    /** fem for element 2050/2099 -2050/-2099 **/
    case 2050 :
      fprintf( stderr, "%s unsuitable element exists\n", buf ) ;
      break ;

    /** fem for Magnetic Analysis 2500/2599 -2500/-2599 **/
    case 2500 :
      fprintf( stderr,
               "%s t_loop is not suitable in Function \"correct_Jo\"\n",
               buf ) ;
      break ;



    /*** hddm 3000/3999 -3000/-3999 ***/
    /** hddm/make_PartMesh 3000/3049 -3000/-3049 **/
    case 3000 :
      fprintf( stderr, "%s pinbc.nf is not unsuitable (%s)\n",
               buf, str ) ;
      break ;
    case 3100 :
      fprintf( stderr, "%s unknown communication signal (%s) for H-mode\n",
               buf, str ) ;
      break ;



    default :
      fprintf( stdout, "unknwon Error Number %d\n", ErrorNumber ) ;
      Exit( AbortNumber ) ;
  }


  #ifdef __ADVFW_DEBUG__
    if( COM_ID.nproc != 1 ) {
      fprintf( stderr, "[%d] ", COM_ID.myprocid ) ;
    }
    fprintf( stderr, "%s line %d\n", fname, line ) ;
  #endif /* __ADVFW_DEBUG__ */


  if( AbortNumber != __ABORT_NONE__ ) Exit( AbortNumber ) ;


  return 0 ;
}
