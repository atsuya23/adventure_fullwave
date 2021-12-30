/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "advfullwave_lib.h"


#define LOCAL_MODULE_NAME "make FEM file"
#define LOCAL_VERSION     ADVFW_VERSION
#define LOCAL_WORK        WORK_PRE_FILECONVERT
#define LOCAL_FIELD       FIELD_ELECTROMAGNETIC
#define LOCAL_ANALYSIS    ANALYSIS_ANOTHER
#define LOCAL_N_ARGUMENT  5
#define LOCAL_USAGE       "mshFile fgrFile cndFile matFile advFile"


extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;


void make_DirichletBC_Axn0( MESH *mesh, FaceGroup fgr ) ;


int main( int argc, char **argv )
{
  char   fn_msh[FILENAME_MAX] ;
  char   fn_fgr[FILENAME_MAX] ;
  char   fn_cnd[FILENAME_MAX] ;
  char   fn_mat[FILENAME_MAX] ;
  char   fn_adv[FILENAME_MAX] ;

  MESH    mesh ;
  FaceGroup fgr ;
  OPTIONS options ;


  /*** Initialize ***/
  main_Init( argc, argv, &options ) ;


  /*** Get File name ***/
  strcpy( fn_msh, argv[1] ) ;
  strcpy( fn_fgr, argv[2] ) ;
  strcpy( fn_cnd, argv[3] ) ;
  strcpy( fn_mat, argv[4] ) ;
  strcpy( fn_adv, argv[5] ) ;


  /*** Main Function ***/
  Fprintf( stdout, "\n" ) ;
  Fprintf( stdout, "Start making ADV file\n" ) ;
  init_MESH( &mesh, options ) ;
  init_FaceGroup( &fgr ) ;
  read_matFile( fn_mat, &(mesh.mtrl), options ) ;
  read_cndFile( fn_cnd, &(mesh.mtrl), options ) ;
  read_fgrFile( fn_fgr, &fgr, options ) ;
  read_mshFile( fn_msh, &mesh, fgr, options ) ;
  make_DirichletBC_Axn0( &mesh, fgr ) ;
  write_onedata_file( fn_adv, mesh ) ;
  free_FaceGroup( &fgr ) ;
  free_MESH( &mesh, options ) ;


  /*** Finalize ***/
  main_Finalize() ;


  return 0 ;
}

/********************************************************************/

void make_DirichletBC_Axn0( MESH *mesh, FaceGroup fgr )
{
  /*+ Funciton: make Dirichlet B.C. A x n = 0 +*/
  int    i, j, k ;

  int    nd_face = 0 ;
  int    nnd = mesh->nnd ;
  int    nbc = 0 ;
  int    *bci ;


  if( fgr.n == 4 ) {
    nd_face = 3 ;
  } else if( fgr.n == 10 ) {
    nd_face = 6 ;
  } else if( fgr.n == 8 ) {
    nd_face = 4 ;
  } else if( fgr.n == 20 ) {
    nd_face = 8 ;
  } else {
    char str[1024] ;
    sprintf( str, "%d", fgr.n ) ;
    emessage( 126, __ABORT_ADVFW_LIB__, str ) ;
  }


  bci = (int *)Malloc( sizeof(int), nnd ) ;
  for( i=0 ; i<nnd ; i++ ) {
    bci[i] = -1 ;
  }
  for( i=0 ; i<fgr.nfgr ; i++ ) {
    for( j=0 ; j<mesh->mtrl.nbc ; j++ ) {
      if( mesh->mtrl.bci[j] == i ) {
        int    nface = fgr.nface[i] ;
        int    ii ;
        for( k=0,ii=0 ; k<nface ; k++ ) {
          int    jj ;
          for( jj=0 ; jj<nd_face ; ii++,jj++ ) {
            bci[fgr.nop[i][ii]] = 0 ;
          }
        }
      }
    }
  }
  for( i=0 ; i<nnd ; i++ ) {
    if( bci[i] > -1 ) {
      nbc++ ;
    }
  }
  if( nbc > 0 ) {
    mesh->dbc_Axn0.need = NEED ;
    mesh->dbc_Axn0.nbc = nbc ;
    mesh->dbc_Axn0.ibc = (int *)Malloc( sizeof(int), nbc ) ;
    for( i=0,j=0 ; i<nnd ; i++ ) {
      if( bci[i] > -1 ) {
        mesh->dbc_Axn0.ibc[j] = i ;
        j++ ;
      }
    }
  }


  Free( sizeof(int)*nnd, bci ) ;
}
