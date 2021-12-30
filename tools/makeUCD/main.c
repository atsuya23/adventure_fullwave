/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "advfullwave_lib.h"
#include "libfem.h"


#define LOCAL_MODULE_NAME "Post Processor for Fullwave Analysis"
#define LOCAL_VERSION     ADVFW_VERSION
#define LOCAL_WORK        WORK_POST_FEM_CALC
#define LOCAL_FIELD       FIELD_ELECTROMAGNETIC
#define LOCAL_ANALYSIS    ANALYSIS_UNKNOWN
#define LOCAL_N_ARGUMENT  0
#define LOCAL_USAGE       ""


void post_HDDM_AVS( MESH *mesh, OPTIONS *options ) ;
void post_HDDM_AVS_Full_Wave( Complex *el_B_c, Complex *el_E_c, Complex *el_Je_c,MESH *mesh, OPTIONS options ) ;

extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;


int main( int argc, char **argv )
{
  char   fname[FILENAME_MAX] ;

  MESH    mesh ;
  OPTIONS options ;

  /*** Initialize ***/
  main_Init( argc, argv, &options ) ;
  
  /*** Make or Check output files are writable ***/
  options.datasw.avsfile = NEED ;
  if( COM_GID.rootid == COM_ID.myprocid ){
    if( options.datasw.avsfile == NEED ) {
      if( make_avsfile_directory(options.datasw) > 0 ) {
        Exit( -1 ) ;
      }
    }
    if( options.datasw.vtkfile == NEED ) {
      if( make_vtkfile_directory(options.datasw) > 0 ) {
        Exit( -1 ) ;
      }
    }
  }

  /*** Main Function ***/
  Fprintf( stdout, "\n" ) ;
  /* read onedata */

  init_MESH( &mesh, options ) ;

  make_onedata_filename( fname, options.datasw, 0 ) ;

  read_onedata_file( fname, &mesh ) ;

  /* read mtrldat */
  make_mtrldat_filename( fname, options.datasw, 0 ) ;
  read_mtrldat_file( fname, &(mesh.mtrl), options ) ;
  /* read outdata */
  make_outdata_filename( fname, options.datasw, -1, 0 ) ;
  read_outdata_file_FEM_FW( fname, -1, &mesh, &options ) ;
  /* make MESH */
  make_MESH( &mesh, options ) ;


  /*main_PrintCondition( options, mesh.nel, mesh.nap, mesh.nmp,
                       mesh.elem_type, mesh.nmp+mesh.nphi ) ;*/

  if( options.work == WORK_POST_HDDM_CALC ) {
    post_HDDM_AVS( &mesh, &options ) ;
  }

  Fprintf( stdout, "\n" ) ;
  Fprintf( stdout, "Start writing UCD file for AVS   time %g[s]\n",
           nowTime() ) ;
  switch( options.analysis ) {
    case ANALYSIS_POST_FULL_WAVE :
      post_FullWave_AVS( &mesh, options ) ;
      break ;
  }
  Fprintf( stdout, "Finish writing UCD file for AVS   time %g[s]\n",
           nowTime() ) ;



  free_MESH( &mesh, options ) ;



  /*** Finalize ***/
  main_Finalize() ;


  return 0 ;
}

/********************************************************************/

void post_HDDM_AVS( MESH *mesh, OPTIONS *options )
{

  int    i ;
  char   fname[FILENAME_MAX] ;

  int    part ;
  int    npart ;

  int    dimension = mesh->dimension ;

  //double* el_B_d ;
  Complex* el_B_c ;
  Complex* el_E_c ;

  /*double* el_Je_d ;*/
  Complex* el_Je_c ;

    el_B_c = (Complex *)Calloc( sizeof(Complex), mesh->nel*dimension ) ;
    el_E_c = (Complex *)Calloc( sizeof(Complex), mesh->nel*dimension ) ;
    el_Je_c = (Complex *)Calloc( sizeof(Complex), mesh->nel*dimension ) ;

  
    //printf("post_HDDM_AVS1\n");

  make_modeldata_filename( fname, options->datasw, 0 );
  npart = get_num_part_from_modeldata( fname );

  for( part=0 ; part<npart ; part++ ) {
    PartMesh pm ;
    int    dom ;
    int    ndomain = 0 ;

    init_PartMesh( &pm, *options ) ;
    make_modeldata_filename( fname, options->datasw, part ) ;
    read_modeldata_file( fname, &pm, *options ) ;
    make_outdata_filename( fname, options->datasw, part, -1 ) ;
    //printf("post_HDDM_AVS1.1\n");
    
    read_outdata_file_HDDM_temp( fname, -1, &pm, options ) ;
    //printf("post_HDDM_AVS1.2\n");
    
    ndomain = pm.ndomain ;
    //printf("post_HDDM_AVS1.4\n");
    
    switch( options->analysis ) {
    case ANALYSIS_POST_FULL_WAVE :
      for( dom=0 ; dom<ndomain ; dom++ ) {
	int    el ;
	DomMesh* dm = &(pm.dm[dom]) ;
	int    nel = dm->nel ;
	int*   seid2geid = dm->seid2geid ;	  
	for( el=0 ; el<nel ; el++ ) {
	  int    ii = el * dimension ;
	  int    jj = seid2geid[el] * dimension ;
	  for( i=0 ; i<dimension ; i++,ii++,jj++ ) {
	    el_B_c[jj] = dm->rvalue.el_B_c[ii] ;
	    el_E_c[jj] = dm->rvalue.el_E_c[ii] ;
	    el_Je_c[jj] = dm->rvalue.el_Je_c[ii] ;
	  }
	}
      }
      break ;
    }
    free_PartMesh( &pm, *options ) ;
  }

    post_HDDM_AVS_Full_Wave( el_B_c, el_E_c, el_Je_c, mesh, *options ) ;
    Free( sizeof(Complex)*mesh->nel*dimension, el_B_c ) ;
    Free( sizeof(Complex)*mesh->nel*dimension, el_E_c ) ;
    Free( sizeof(Complex)*mesh->nel*dimension, el_Je_c ) ;
  
}

/********************************************************************/


/********************************************************************/

void post_HDDM_AVS_Full_Wave( Complex *el_B_c, Complex *el_E_c,Complex *el_Je_c,
                            MESH *mesh, OPTIONS options )
{

  int    i, j, k ;

  int    el ;
  int    nd ;
  int    nel = mesh->nel ;
  int    nnd = mesh->nnd ;
  int    nap = mesh->nap ;
  int    dimension = mesh->dimension ;
  int*   nop = mesh->nop ;
  double* crd = mesh->crd ;
  int*   flag = mesh->flag ;
  /*int*   apid2phiid = mesh->apid2phiid ;*/
  MTRL   mtrl = mesh->mtrl ;


  Complex* B = (Complex *)Calloc( sizeof(Complex), nap*dimension ) ;
  Complex* E = (Complex *)Calloc( sizeof(Complex), nap*dimension ) ;
  Complex* Je = (Complex *)Calloc( sizeof(Complex), nap*dimension ) ;
  double* V = (double *)Calloc( sizeof(double), nap ) ;
  double* VR = (double *)Calloc( sizeof(double), nap ) ;
  int*   votex = (int *)Calloc( sizeof(int), nap ) ;

  //printf("post_HDDM_AVS_Full_Wave in main.c\n");

  for( el=0 ; el<nel ; el++ ) {
    double dd ;

    int    ap_elem = 4 ;
    int    nd_elem = 10 ;
    /*int    mp_elem = nd_elem - ap_elem ;*/

    int    *ne ;
    int    flg = flag[el] ;
    double x[ap_elem], y[ap_elem], z[ap_elem] ;
    double D ;

    double nu = 0 ;
    //int    rs = 1 ;

    Complex rotA[dimension] ;
    Complex Jee[dimension] ;
    Complex Ep[dimension];

    for( i=0 ; i<mtrl.nflag ; i++ ) {
      if( flg == mtrl.flag_i[i] ) {
        nu = 7.957747e+05 ;
        break ;
      }
    }
    /*for( i=0 ; i<mtrl.nR ; i++ ) {
      if( flg == mtrl.R_i[i] ) {
        rs = 2 ;
        break ;
      }
      }*/
    //printf("initial end\n");

    ne = &(nop[el*nd_elem]) ;
    tet_pickup_coordinate_4vertex( ne, crd, x, y, z ) ;
    D = tet_Volume6( x, y, z ) ;
    for( i=0 ; i<dimension ; i++ ) {
      rotA[i] = el_B_c[el*dimension+i] ;
      Ep[i] = el_E_c[el*dimension+i] ;
    }


    switch( options.unit.MagneticField ) {
      case UNIT_A_m : /* [A/m] */
        dd = D * nu ;
        break ;
      case UNIT_A_mm : /* [A/mm] */
        dd = D * nu * 1.0e-3 ;
        break ;
      case UNIT_T : /* [T] */
        dd = D ;
        break ;
      case UNIT_G : /* [G] */
        dd = D * 1.0e+4 ;
        break ;

      default : /* [T] */
        dd = D ;
    }

    /***************** Change by A->B E->H ***********************/
    //printf("Change by A->B E->H\n");

    for( i=0 ; i<ap_elem ; i++ ) {
      k = ne[i] ;
      V[k] += D ;
      k *= dimension ;
      for( j=0 ; j<dimension ; j++,k++ ) {
        B[k] = ComplexPlus( B[k], ComplexMultiScalar(rotA[j], dd) ) ;
	E[k] = ComplexPlus( E[k], ComplexMultiScalar(Ep[j], dd) ) ;
      }
    }

    //if( rs != 2 ) continue ;

    for( i=0 ; i<dimension ; i++ ) {
      Jee[i] = el_Je_c[el*dimension+i] ;
      //Ep[i] = el_E_c[el*dimension+i] ;
    }

    switch( options.unit.CurrentDensity ) {
      case UNIT_A_m2 : /* [A/m^2] */
        dd = D ;
        break ;
      case UNIT_A_mm2 : /* [A/mm^2] */
        dd = D * 1.0e+6 ;
        break ;

      default : /* [A/m^2] */
        dd = D ;
    }
    for( i=0 ; i<ap_elem ; i++ ) {
      k = ne[i] ;
      votex[k] = 1 ;
      VR[k] += D ;
      k *= dimension ;
      for( j=0 ; j<dimension ; j++,k++ ) {
        Je[k] = ComplexPlus( Je[k], ComplexMultiScalar(Jee[j], dd) ) ;
	//E[k] = ComplexPlus( E[k], ComplexMultiScalar(Ep[j], dd) ) ;
	
      }
    }
  }


  for( nd=0,j=0 ; nd<nnd ; nd++ ) {
    double dd = 1.0 / V[nd] ;
    if( votex[nd] > 0 ) {
      double ee = 1.0 / VR[nd] ;
      for( i=0 ; i<dimension ; i++,j++ ) {
        B[j] = ComplexMultiScalar( B[j], dd ) ;
	E[j] = ComplexMultiScalar( E[j], dd ) ;
        Je[j] = ComplexMultiScalar( Je[j], ee ) ;
      }
    } else {
      for( i=0 ; i<dimension ; i++,j++ ) {
        B[j] = ComplexMultiScalar( B[j], dd ) ;
      }
    }
  }


  mesh->rvalue.B_c = B ;
  mesh->rvalue.E_c = E ;  
  mesh->rvalue.Je_c = Je ;


  Free( sizeof(double)*nap, V ) ;
  Free( sizeof(double)*nap, VR ) ;
  Free( sizeof(int)*nap, votex ) ;
}
