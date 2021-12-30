/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "make_elem_em.h"

#define TAU 1.0


/********************************************************************/

void tet_MakeElement_EM_E( int el, int *nop, double *crd,
                                int *flag, ANTENNA *antenna, MTRL mtrl,
                                TMP_AIJ *tmp_aij )
{
  /*+ Function: make element
                  conventional piecewise linear tetrahedral
                  Full Wave Analysis
                  E method

   (1/ur rotE,rotE*)-(k0^2 er E,E*)+(j w u0 J,E*)=0

 +*/
  
  int    i, j, k ;

  int    dimension = 3 ;
  int    ap_elem = 4 ;
  int    nd_elem = 10 ;
  int    mp_elem = nd_elem - ap_elem ;

  int    *ne ;
  int    flg = flag[el] ;
  /*** nu = 1/u ***/
  double nu=7.957747e+05;
  double nu_omega;
  double sigma = 0 ;
  double omega = mtrl.antenna_omega ;

  double epsilon = 0;
  //int    rs = 1 ;
  int    nc = -1 ;
  double x[ap_elem], y[ap_elem], z[ap_elem] ;
  double D ;
  double l[mp_elem] ;
  double tetg[3] ;
  double b[ap_elem], c[ap_elem], d[ap_elem] ;
  int    si[mp_elem] ;
  double xi[mp_elem], yi[mp_elem], zi[mp_elem] ;
  double X[mp_elem], Y[mp_elem], Z[mp_elem] ;
  double xy[mp_elem], yz[mp_elem], zx[mp_elem] ;
  double Fx[mp_elem], Fy[mp_elem], Fz[mp_elem] ;
  double rr[mp_elem][mp_elem] ;

  double ss[mp_elem][mp_elem] ;
  double ss_s[mp_elem][mp_elem] ;
  double dd ;

  int    szRow = mp_elem ;
  int    indRow[szRow] ;

  Complex ae[szRow*szRow] ;
  Complex be[szRow] ;


  /*** Read volt.dat file.: ./MODEL/volt.dat ***/

  FILE *fvp;
  int dg;
  char buf[256];
 
  if((fvp =fopen("./MODEL/volt.dat","r")) == NULL){
    printf("Error:Can't open volt.dat\n");
    exit(1);
  }

/***** bc.dat is not read : 2019.09.06 *****/
/*** Read bc.dat file.: ./MODEL/bc.dat ***/
/*
  FILE *fbcp;
  int  abc;
  char buf1[256];

  if((fbcp =fopen("./MODEL/bc.dat","r")) == NULL){
    printf("Error:Can't open bc.dat\n");
    exit(1);
  }
  fgets(buf, sizeof(buf), fbcp);
  sscanf(buf, "%d",&abc);
*/

  /********* Get mtrl data *********/
  fgets(buf, sizeof(buf), fvp);
  sscanf(buf, "%d",&dg);

  for( i=0 ; i<mtrl.nflag ; i++ ) {
    if( flg == mtrl.flag_i[i] ) {
      epsilon = mtrl.flag_nu[i] ;
      break ;
    }
  }
  for( i=0 ; i<mtrl.nR ; i++ ) {
    if( flg == mtrl.R_i[i] ) {
      sigma = mtrl.R_sigma[i] ;
      break ;
    }
  }
  for( i=0 ; i<mtrl.nantenna ; i++ ) {
    if( flg == mtrl.antenna_def[i].flag ) {
      nc = i ;
      break ;
    }
  }

  ne = &(nop[el*nd_elem]) ;
  
  for( i=0,j=ap_elem ; i<szRow ; i++,j++ ) {
    indRow[i] = ne[j] ;
  }

  for( i=0,k=0 ; i<szRow ; i++ ) {
    for( j=0 ; j<szRow ; j++,k++ ) {
      ae[k] = complex_0 ;
    }
    be[i] = complex_0 ;
  }


  tet_pickup_coordinate_4vertex( ne, crd, x, y, z ) ;
  D = tet_Volume6( x, y, z ) ;
  tet_SideLength( x, y, z, l ) ;
  tet_Center( x, y, z, tetg ) ;
  tet_simple_b( x, y, z, b ) ;
  tet_simple_c( x, y, z, c ) ;
  tet_simple_d( x, y, z, d ) ;
  tetNedelec_Direction( x, y, z, si ) ;
  tetNedelec_mk( x, y, z, xi, yi, zi ) ;
  tetNedelec_XYZ(  l, si, xi, yi, zi, X, Y, Z ) ;
  tetNedelec_km_mk( x, y, z, xy, yz, zx ) ;
  tetNedelec_Fxyz( l, si, tetg, xi, yi, zi, xy, yz, zx, Fx, Fy, Fz ) ;

  /***** bc.dat is not read : 2019.09.06 *****/
  /********* ABC *********/
  /*
  if(0<abc){	
    
    int    tri_elem = 3 ;
    double S ;
    int    ndID3d[tri_elem];
    double nomal[tri_elem];
    double ss_abc[mp_elem][mp_elem] ;
    double xbc0[abc],xbc1[abc],ybc0[abc],ybc1[abc],zbc0[abc],zbc1[abc];
    double Y0 = sqrt( nu * epsilon );*/ /* Y0:(Wave Impedance)^-1 */
  /*double xc[mp_elem], yc[mp_elem], zc[mp_elem] ;
    double xic[mp_elem], yic[mp_elem], zic[mp_elem] ;
    double xyc[mp_elem], yzc[mp_elem], zxc[mp_elem] ;
    double xs[tri_elem];
    double ys[tri_elem];
    double zs[tri_elem];
    int    total_ID = 0;
    int    egdeID[4][3]={ { 0, 1, 3 },
			  { 0, 2, 4 },
			  { 1, 2, 5 },
			  { 3, 4 ,5 } };
	
    for(i=0 ; i<abc ; i++){

      fgets(buf1, sizeof(buf1), fbcp);
      sscanf(buf1, "%lf %lf %lf %lf %lf %lf"
	     ,&xbc0[i],&xbc1[i],&ybc0[i],&ybc1[i],&zbc0[i],&zbc1[i]);
    }  
    for( i=0 ; i<abc ; i++ ) {
      int ibc = 0;
      for( j=0 ; j<ap_elem ; j++ ) { 
	if((  xbc0[i]<=x[j]&&x[j]<=xbc1[i])
	   &&(ybc0[i]<=y[j]&&y[j]<=ybc1[i])
	   &&(zbc0[i]<=z[j]&&z[j]<=zbc1[i])){
	      
	  xs[ibc] = x[j] ;
	  ys[ibc] = y[j] ;
	  zs[ibc] = z[j] ;
	      
	  ibc++;
	      
	  total_ID += j ; 

	} 
      }
	  
      if(ibc==3){
   
	for(j = 0 ; j < tri_elem ; j++ )
	  ndID3d[j] = egdeID[total_ID - 3][j]; 

	S = tri_square( xs, ys, zs , nomal ); 
	    
	if( total_ID == 3 || total_ID == 5 ){
	  for( j = 0 ; j < tri_elem ; j++ )
	    nomal[j] *= -1;
	}
  */    
  /*tet_cross( x, y, x, nomal, xc, yc, zc );*/ /* E x n */
  /*
	tetNedelec_mk( xc, yc, zc, xic, yic, zic ) ;
	tetNedelec_km_mk( xc, yc, zc, xyc, yzc, zxc ) ;

	triNedelec_sideside( S, ndID3d, D, l, tetg, si, xc, yc, zc, 
			     xic, yic, zic, xyc, yzc, zxc, ss_abc ) ;
	
	for( j=0 ; j<mp_elem ; j++ ) {
	  int    ii = j * szRow ;
	      
	  for( k=0 ; k<mp_elem ; k++,ii++ ) {
	    ae[ii].im += ( 1/omega ) * Y0 * ss_abc[j][k] ;
	  }
	}
      }
    }
  }
*/

    /********* (1/u rotE,rotE) *********/

    /* 1/miu  ->  1/(miu*omega) */
    nu_omega = (nu / omega); 

    tetNedelec_rotrot( D, X, Y, Z, rr ) ;
    for( i=0 ; i<mp_elem ; i++ ) {
      int    ii = i * szRow ;
      for( j=0 ; j<mp_elem ; j++,ii++ ) {
	ae[ii].re += nu_omega * rr[i][j] ;
      }
    }
    
    /********* (e w^2 E,E) *********/
    
    dd= -(epsilon * omega) ;
    
    tetNedelec_sideside( D, l, tetg, si, x, y, z, xi, yi, zi,
                         xy, yz, zx, ss ) ;
    
    /* -j(im)  ->  -(re) */
    
    for( i=0 ; i<mp_elem ; i++ ) {
      int    ii = i * szRow ;
      
      for( j=0 ; j<mp_elem ; j++,ii++ ) {
        ae[ii].re += dd * ss[i][j] ;
      }
    }
    
    /********* Conductivity:Sigma *********/
    
    tetNedelec_sideside( D, l, tetg, si, x, y, z, xi, yi, zi,
                         xy, yz, zx, ss_s ) ;
    
    for( i=0 ; i<mp_elem ; i++ ) {
      int    ii = i * szRow ;
	
      for( j=0 ; j<mp_elem ; j++,ii++ ) {
        ae[ii].im += sigma * ss_s[i][j] ;
      }
    }
    
    if(dg==0){
      
      /******* dg==0:Input is current J. *******/
      
      fclose(fvp);
      
      /********* nc ! = -1 : Antenna area *********/ 
      
      if( nc != -1 ) {
	Complex Joe[ap_elem][dimension] ;
	Complex Ihe[ap_elem] ;
		
	double gs[mp_elem][ap_elem] ;
	
	Complex be_JoA[mp_elem] ;
	Complex be_gs[mp_elem] ;
	

	/*------------- 
	  A          E  
	  J     ->  jJ
	  Jr(A) ->  Ji(E)
	  Ji(A) -> -Jr(E)
	  -------------*/
	for( i=0 ; i<ap_elem ; i++ ) {
	  int    ii = antenna[nc].enf[ne[i]] * dimension ;	  
	  for( j=0 ; j<dimension ; j++,ii++ ) {
	    Joe[i][j].re = -antenna[nc].Jo[ii] ;/* antenna[nc].Jor[ii] : Re(A) */
	    Joe[i][j].im = -antenna[nc].Jor[ii] ;/* antenna[nc].Jo[ii]  : Im(A) */
	  }
     
	  
	    Ihe[i].re=0.0;
	    Ihe[i].im=0.0;
	  
	}

	tetNedelec_JoA_Complex( Joe, si, l, b, c, d, be_JoA ) ;
	tetNedelec_gradside( D, b, c, d, Fx, Fy, Fz, gs ) ;
	for( i=0 ; i<6 ; i++ ) {
	  be_gs[i] = complex_0 ;
	  for( j=0 ; j<4 ; j++ ) {
	    be_gs[i] = ComplexPlus( be_gs[i],
				    ComplexMultiScalar(Ihe[j], gs[i][j]) ) ;
	  }
	}
	for( i=0 ; i<mp_elem ; i++ ) {
	  be[i] = ComplexPlus( be[i], ComplexMinus(be_JoA[i], be_gs[i]) ) ;
	}

      }
    }
    else
      {/* dg!=0 */

	/******* dg!=0:Input is a Delta-Gap voltage. *******/
	
	double xp0[dg],xp1[dg],yp0[dg],yp1[dg],zp0[dg],zp1[dg];
	double dV[dg];
	int dVn=0;
	Complex DGe[ap_elem] ; 
	
	/* Zero clear:DGe[] */
	for(i=0 ; i< ap_elem ; i++)
	  DGe[i] = complex_0 ;
		
	/* Get DG area */
	
	for(i=0 ; i<dg ; i++){
	  fgets(buf, sizeof(buf), fvp);
	  sscanf(buf, "%lf %lf",&xp0[i],&xp1[i]);
	  
	  fgets(buf, sizeof(buf), fvp);
	  sscanf(buf, "%lf %lf",&yp0[i],&yp1[i]);
	  
	  fgets(buf, sizeof(buf), fvp);
	  sscanf(buf, "%lf %lf",&zp0[i],&zp1[i]);
	  
	  fgets(buf, sizeof(buf), fvp);
	  sscanf(buf, "%lf",&dV[i]);
	}
	            
	/* Checking added nodes of dV */
	for(i=0 ; i<dg ; i++){
	  for(j=0 ; j<ap_elem ; j++){
	    if((xp0[i] <= x[j] && x[j] <= xp1[i]) &&
	       (yp0[i] <= y[j] && y[j] <= yp1[i]) &&
	       (zp0[i] <= z[j] && z[j] <= zp1[i])){
	      DGe[j].re=0.0;
	      DGe[j].im=dV[i];
	      dVn=1;
	    }
	  }
	}
	
	/********* J = Sigma*E *********/
	
	/* dVn == 1:Added nodes of dV */
	
	if( dVn == 1 ) {
	  
	  double gs[mp_elem][ap_elem] ;
	  Complex be_gs[mp_elem] ;
	  
	  
	  /*------------- 
	    A -> E  
	    J -> singma * grad(dV)
	    -------------*/	  
	  
	  tetNedelec_gradside( D, b, c, d, Fx, Fy, Fz, gs ) ;
	  for( i=0 ; i<6 ; i++ ) {
	    be_gs[i] = complex_0 ;
	    
	    
	    for( j=0 ; j<4 ; j++ ) {
	      
	      be_gs[i] = ComplexPlus( be_gs[i],
				      ComplexMultiScalar(DGe[j], 
							 gs[i][j]) ) ;
	   
	    }
	  }
	  
	  for( i=0 ; i<mp_elem ; i++ ) {
	  be[i] = ComplexMultiScalar(be_gs[i],sigma*omega);
	  }
	}
	fclose(fvp);
      }
    
    
    mymt_CS_AIJ_Set( tmp_aij, szRow, indRow, szRow, indRow, ae, be ) ;
}
