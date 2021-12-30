/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "post.h"


/********************************************************************/

void post_FullWave_AVS( MESH *mesh, OPTIONS options )
{
  /*+ Function: post processor for Full-Wave Analysis +*/

  int    i ;
  char   fname[FILENAME_MAX] ;

  int    nel = mesh->nel ;
  int    nnd = mesh->nnd ;
  int    nap = mesh->nap ;
  int    ap_elem = mesh->ap_elem ;
  int    nd_elem = mesh->nd_elem ;
  char*  elem_type = mesh->elem_type ;
  int    dimension = mesh->dimension ;
  int*   nop = mesh->nop ;
  double* crd = mesh->crd ;
  int*   flag = mesh->flag ;

  Complex* B = mesh->rvalue.B_c ;
  Complex* E = mesh->rvalue.E_c ;
  Complex* Je = mesh->rvalue.Je_c;


  printf("post_FullWave_AVS\n");

  if( options.datasw.avsfile == NEED && options.datasw.vtkfile == NO_NEED ) {
    int    ii = nap * dimension ;
    double* x = (double *)Malloc( sizeof(double), ii ) ;
    for( i=0 ; i<ii ; i++ ) {
      x[i] = B[i].re ;
    }
    make_avsfile_filename( fname, options.datasw, -1, -1, "Hr" ) ;
    write_avsfile_file( fname, nel, nnd, ap_elem, nd_elem, elem_type,
                        dimension, nop, crd, flag, x, options ) ;

    for( i=0 ; i<ii ; i++ ) {
      x[i] = B[i].im ;
    }
    make_avsfile_filename( fname, options.datasw, -1, -1, "Hi" ) ;
    write_avsfile_file( fname, nel, nnd, ap_elem, nd_elem, elem_type,
                        dimension, nop, crd, flag, x, options ) ;


    for( i=0 ; i<ii ; i++ ) {
      x[i] = E[i].re ;
    }
    make_avsfile_filename( fname, options.datasw, -1, -1, "Er" ) ;
    write_avsfile_file( fname, nel, nnd, ap_elem, nd_elem, elem_type,
                        dimension, nop, crd, flag, x, options ) ;

    for( i=0 ; i<ii ; i++ ) {
      x[i] = E[i].im ;
    }
    make_avsfile_filename( fname, options.datasw, -1, -1, "Ei" ) ;
    write_avsfile_file( fname, nel, nnd, ap_elem, nd_elem, elem_type,
                        dimension, nop, crd, flag, x, options ) ;


    for( i=0 ; i<ii ; i++ ) {
      x[i] = Je[i].re ;
    }
    make_avsfile_filename( fname, options.datasw, -1, -1, "Jer" ) ;
    write_avsfile_file( fname, nel, nnd, ap_elem, nd_elem, elem_type,
                        dimension, nop, crd, flag, x, options ) ;

    for( i=0 ; i<ii ; i++ ) {
      x[i] = Je[i].im ;
    }
    make_avsfile_filename( fname, options.datasw, -1, -1, "Jei" ) ;
    write_avsfile_file( fname, nel, nnd, ap_elem, nd_elem, elem_type,
                        dimension, nop, crd, flag, x, options ) ;



    Free( sizeof(double)*ii, x ) ;
  }

  if( options.datasw.vtkfile == NEED ) {
    int    ii = nap * dimension ;
    double* x = (double *)Malloc( sizeof(double), ii ) ;
    for( i=0 ; i<ii ; i++ ) {
      x[i] = B[i].re ;
    }
    make_vtkfile_filename( fname, options.datasw, -1, -1, "Hr" ) ;
    write_vtkfile_file( fname, nel, nnd, ap_elem, nd_elem, elem_type,
                        dimension, nop, crd, flag, x, options ) ;

    for( i=0 ; i<ii ; i++ ) {
      x[i] = B[i].im ;
    }
    make_vtkfile_filename( fname, options.datasw, -1, -1, "Hi" ) ;
    write_vtkfile_file( fname, nel, nnd, ap_elem, nd_elem, elem_type,
                        dimension, nop, crd, flag, x, options ) ;


    for( i=0 ; i<ii ; i++ ) {
      x[i] = E[i].re ;
    }
    make_vtkfile_filename( fname, options.datasw, -1, -1, "Er" ) ;
    write_vtkfile_file( fname, nel, nnd, ap_elem, nd_elem, elem_type,
                        dimension, nop, crd, flag, x, options ) ;

    for( i=0 ; i<ii ; i++ ) {
      x[i] = E[i].im ;
    }
    make_vtkfile_filename( fname, options.datasw, -1, -1, "Ei" ) ;
    write_vtkfile_file( fname, nel, nnd, ap_elem, nd_elem, elem_type,
                        dimension, nop, crd, flag, x, options ) ;


    for( i=0 ; i<ii ; i++ ) {
      x[i] = Je[i].re ;
    }
    make_vtkfile_filename( fname, options.datasw, -1, -1, "Jer" ) ;
    write_vtkfile_file( fname, nel, nnd, ap_elem, nd_elem, elem_type,
                        dimension, nop, crd, flag, x, options ) ;

    for( i=0 ; i<ii ; i++ ) {
      x[i] = Je[i].im ;
    }
    make_vtkfile_filename( fname, options.datasw, -1, -1, "Jei" ) ;
    write_vtkfile_file( fname, nel, nnd, ap_elem, nd_elem, elem_type,
                        dimension, nop, crd, flag, x, options ) ;



    Free( sizeof(double)*ii, x ) ;
  }

 
  i = nap * dimension ;
  Free( sizeof(Complex)*i, mesh->rvalue.B_c ) ;
  Free( sizeof(Complex)*i, mesh->rvalue.E_c ) ;
  Free( sizeof(Complex)*i, mesh->rvalue.Je_c ) ;
}

/********************************************************************/

