/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "fileio.h"


extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;


void read_matFile( char *fname, MTRL *mtrl, OPTIONS options )
{
  /*+ Funciton: read matFile +*/
  int    i ;
  FILE   *fp ;

  char   title[1024] ;

 
  fp = Fopen( fname, "r" ) ;


  Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;


  fscanf( fp, "%s", title ) ;
  if( strcmp(title, "#materialInfo") ) {
    char str[2048] ;
    sprintf( str, "%s in %s", title, fname ) ;
    emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
  }


  fscanf( fp, "%s", title ) ;
  while( strcmp(title, "#volumeInfo") ) {

    if( !strcmp(title, "materialN") ) {
      fscanf( fp, "%d", &(mtrl->nflag) ) ;

    } else if( !strcmp(title, "propertyN") ) {
      fscanf( fp, "%d", &i ) ;
      if( options.field == FIELD_ELECTROMAGNETIC ) {
        if( i != 0 ) {
          emessage( 113, __ABORT_ADVFW_LIB__, NULL ) ;
        }
      }

    } else {
      char str[2048] ;
      sprintf( str, "%s in %s", title, fname ) ;
      emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
    }

    fscanf( fp, "%s", title ) ;
  }
  fscanf( fp, "%s", title ) ;
  if( strcmp(title, "volumeN") ) {
    char str[2048] ;
    sprintf( str, "%s in %s", title, fname ) ;
    emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
  }
  fscanf( fp, "%d", &(mtrl->nvol) ) ;
  mtrl->vol = (int *)Malloc( sizeof(int), mtrl->nvol ) ;
  for( i=0 ; i<mtrl->nvol ; i++ ) {
    fscanf( fp, "%d", &(mtrl->vol[i]) ) ;
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;
}

/********************************************************************/

void read_cndFile( char *fname, MTRL *mtrl, OPTIONS options )
{
  /*+ Funciton: read cndFile +*/
  int    i, j ;
  double n ;
  FILE   *fp ;

  char   title[1024] ;
  int    dimension = 3 ;


  fp = Fopen( fname, "r" ) ;


  Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;


  if( options.field == FIELD_ELECTROMAGNETIC ) {
    fscanf( fp, "%s", title ) ;
    if( strcmp(title, "gravity") ) {
      char str[2048] ;
      sprintf( str, "%s in %s", title, fname ) ;
      emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
    }
    for( i=0 ; i<dimension ; i++ ) {
      fscanf( fp, "%lf", &n ) ;
    }

    fscanf( fp, "%s", title ) ;
    if( strcmp(title, "boundary") ) {
      char str[2048] ;
      sprintf( str, "%s in %s", title, fname ) ;
      emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
    }
    fscanf( fp, "%d", &(mtrl->nbc) ) ;
    mtrl->bci = (int *)Malloc( sizeof(int), mtrl->nbc ) ;
    for( i=0 ; i<mtrl->nbc ; i++ ) {
      fscanf( fp, "%s", title ) ;
      if( strcmp(title, "dispOnFaceGroup") ) {
        char str[2048] ;
        sprintf( str, "%s in %s", title, fname ) ;
        emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
      }
      fscanf( fp, "%d", &(mtrl->bci[i]) ) ;
      for( j=0 ; j<dimension ; j++ ) {
        fscanf( fp, "%lf", &n ) ;
      }
    }
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;
}

/********************************************************************/

void read_fgrFile( char *fname, FaceGroup *fgr, OPTIONS options )
{
  /*+ Funciton: read fgrFile +*/
  int    i, j , k ;
  FILE   *fp ;

  int    nd_face = 0 ;


  fp = Fopen( fname, "r" ) ;


  Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;


  fscanf( fp, "%d", &(fgr->n) ) ;
  if( fgr->n == 4 ) {
    nd_face = 3 ;
  } else if( fgr->n == 10 ) {
    nd_face = 6 ;
  } else if( fgr->n == 8 ) {
    nd_face = 4 ;
  } else if( fgr->n == 20 ) {
    nd_face = 8 ;
  } else {
    char str[1024] ;
    sprintf( str, "%d", fgr->n ) ;
    emessage( 126, __ABORT_ADVFW_LIB__, str ) ;
  }


  fscanf( fp, "%d", &(fgr->nfgr) ) ;
  fgr->nface = (int *)Malloc( sizeof(int), fgr->nfgr ) ;
  fgr->el = (void *)Malloc( sizeof(int*), fgr->nfgr ) ;
  fgr->face = (void *)Malloc( sizeof(int*), fgr->nfgr ) ;
  fgr->nop = (void *)Malloc( sizeof(int*), fgr->nfgr ) ;


  for( i=0 ; i<fgr->nfgr ; i++ ) {
    int    ii ;
    fscanf( fp, "%d", &(fgr->nface[i]) ) ;
    fgr->el[i] = (int *)Malloc( sizeof(int), fgr->nface[i] ) ;
    fgr->face[i] = (int *)Malloc( sizeof(int), fgr->nface[i] ) ;
    fgr->nop[i] = (int *)Malloc( sizeof(int), fgr->nface[i]*nd_face ) ;
    for( j=0,ii=0 ; j<fgr->nface[i] ; j++ ) {
      fscanf( fp, "%d", &(fgr->el[i][j]) ) ;
      fscanf( fp, "%d", &(fgr->face[i][j]) ) ;
      for( k=0 ; k<nd_face ; k++,ii++ ) {
        fscanf( fp, "%d", &(fgr->nop[i][ii]) ) ;
      }
    }
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;
}

/********************************************************************/

void read_mshFile( char *fname, MESH *mesh, FaceGroup fgr,
                   OPTIONS options )
{
  /*+ Funciton: read mshFile +*/
  int     i, j, k ;
  FILE   *fp ;

  int     el ;
  int     nd ;
  int     dimension = 3 ;
  int     nvol ;


  fp = Fopen( fname, "r" ) ;


  Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;


  mesh->nd_elem = fgr.n ;
  mesh->dimension = dimension ;
  if( fgr.n == 4 ) {
    sprintf( mesh->elem_type, "%s", ELEM_TYPE_3DLT ) ;
    mesh->ap_elem = mesh->nd_elem ;
  } else if( fgr.n == 10 ) {
    sprintf( mesh->elem_type, "%s", ELEM_TYPE_3DQT ) ;
    mesh->ap_elem = 4 ;
  } else if( fgr.n == 8 ) {
    sprintf( mesh->elem_type, "%s", ELEM_TYPE_3DLH ) ;
    mesh->ap_elem = mesh->nd_elem ;
  } else if( fgr.n == 20 ) {
    sprintf( mesh->elem_type, "%s", ELEM_TYPE_3DQH ) ;
    mesh->ap_elem = 8 ;
  } else {
    char str[1024] ;
    sprintf( str, "%d", fgr.n ) ;
    emessage( 126, __ABORT_ADVFW_LIB__, str ) ;
  }


  fscanf( fp, "%d", &(mesh->nel) ) ;
  mesh->nop = (int *)Malloc( sizeof(int), mesh->nel*mesh->nd_elem ) ;
  for( el=0,j=0 ; el<mesh->nel ; el++ ) {
    for( i=0 ; i<mesh->nd_elem ; i++,j++ ) {
      fscanf( fp, "%d", &(mesh->nop[j]) ) ;
    }
  }

  fscanf( fp, "%d", &(mesh->nnd) ) ;
  mesh->crd = (double *)Malloc( sizeof(double), mesh->nnd*dimension ) ;
  for( nd=0,j=0 ; nd<mesh->nnd ; nd++ ) {
    for( i=0 ; i<dimension ; i++,j++ ) {
      fscanf( fp, "%le", &(mesh->crd[j]) ) ;
    }
  }
  if( options.crd_magnification != 1.0 ) {
    double dd = options.crd_magnification ;
    for( nd=0,j=0 ; nd<mesh->nnd ; nd++ ) {
      for( i=0 ; i<dimension ; i++,j++ ) {
        mesh->crd[j] *= dd ;
      }
    }
  }

  fscanf( fp, "%d", &(nvol) ) ;
  mesh->flag = (int *)Calloc( sizeof(int), mesh->nel ) ;
  if( !feof(fp) ) {
    if( nvol != mesh->mtrl.nvol ) {
      char   str[1024] ;
      sprintf( str, "%d", nvol ) ;
      emessage( 128, __ABORT_ADVFW_LIB__, str ) ;
    }
    for( i=0 ; i<nvol ; i++ ) {
      int    vol = mesh->mtrl.vol[i] ;
      fscanf( fp, "%d", &k ) ;
      for( j=0 ; j<k ; j++ ) {
        fscanf( fp, "%d", &el ) ;
        mesh->flag[el] = vol ;
      }
    }
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;
}

/********************************************************************/

void read_fwdata_file( char *fname, MESH *mesh )
{
  /*+ Funciton: read fwdata file +*/
  int    i, j ;
  FILE   *fp ;

  int    el ;
  int    nd ;
  int    bc ;


  fp = Fopen( fname, "r" ) ;


  Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;


  mesh->ap_elem = 4 ;
  mesh->nd_elem = 10 ;
  strcpy( mesh->elem_type, ELEM_TYPE_3DQT ) ;
  mesh->dimension = 3 ;
  mesh->dbc_Axn0.need = NEED ;


  fscanf( fp, "%d", &(mesh->nel) ) ;
  mesh->nop = (int *)Malloc( sizeof(int), mesh->nel*mesh->nd_elem ) ;
  mesh->flag = (int *)Malloc( sizeof(int), mesh->nel ) ;
  for( el=0,j=0 ; el<mesh->nel ; el++ ) {
    for( i=0 ; i<mesh->nd_elem ; i++,j++ ) {
      fscanf( fp, "%d", &(mesh->nop[j]) ) ;
    }
    fscanf( fp, "%d", &(mesh->flag[el]) ) ;
  }

  fscanf( fp, "%d", &(mesh->nnd) ) ;
  mesh->crd = (double *)Malloc( sizeof(double),
                                mesh->nnd*mesh->dimension ) ;
  for( nd=0,j=0 ; nd<mesh->nnd ; nd++ ) {
    for( i=0 ; i<mesh->dimension ; i++,j++ ) {
      fscanf( fp, "%lf", &(mesh->crd[j]) ) ;
    }
  }

  fscanf( fp, "%d", &(mesh->dbc_Axn0.nbc) ) ;
  mesh->dbc_Axn0.ibc = (int *)Malloc( sizeof(int), mesh->dbc_Axn0.nbc ) ;
  for( bc=0 ; bc<mesh->dbc_Axn0.nbc ; bc++ ) {
    fscanf( fp, "%d", &(mesh->dbc_Axn0.ibc[bc]) ) ;
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;
}

/********************************************************************/

void write_fwdata_file( char *fname, MESH mesh )
{
  /*+ Funciton: write fwdata file +*/
  int    i, j ;
  FILE   *fp ;

  int    el ;
  int    nd ;
  int    bc ;


  fp = Fopen( fname, "w" ) ;


  Pfprintf( stdout, "  Start writing to \"%s\"\n", fname ) ;


  fprintf( fp, "%d\n", mesh.nel ) ;
  for( el=0,j=0 ; el<mesh.nel ; el++ ) {
    for( i=0 ; i<mesh.nd_elem ; i++,j++ ) {
      fprintf( fp, "%d ", mesh.nop[j] ) ;
    }
    fprintf( fp, " %d\n", mesh.flag[el] ) ;
  }

  fprintf( fp, "\n%d\n", mesh.nnd ) ;
  for( nd=0,j=0 ; nd<mesh.nnd ; nd++ ) {
    for( i=0 ; i<mesh.dimension ; i++,j++ ) {
      fprintf( fp, "%le", mesh.crd[j] ) ;
      if( i < (mesh.dimension-1) ) {
        fprintf( fp, " " ) ;
      } else {
        fprintf( fp, "\n" ) ;
      }
    }
  }

  if( mesh.dbc_Axn0.need == NEED ) {
    fprintf( fp, "\n%d\n", mesh.dbc_Axn0.nbc ) ;
    for( bc=0 ; bc<mesh.dbc_Axn0.nbc ; bc++ ) {
      fprintf( fp, "%d\n", mesh.dbc_Axn0.ibc[bc] ) ;
    }
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish writing to \"%s\"\n", fname ) ;
}

/********************************************************************/

void read_onedata_file( char *fname, MESH *mesh )
{
  /*+ Funciton: read onedata file +*/
  int    i ;
  double d ;

  AdvDatabox  *dbox ;
  AdvDocument *doc ;
  adv_off_t   off ;

  char   content_type[1024] ;
  char   label[1024] ;
  char   str[1024] ;
  int    size ;

  printf("read_onedata_file\n");

  dbox = AdvDboxOpen( fname ) ;


  Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;


  sprintf( label, "Options" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                       label, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
  }
  adv_dio_get_property_float64( doc, "N_VERSION", &d ) ;
  if( d < OUT_VERSION ) {
    sprintf( str, "%lf", d ) ;
    emessage( 100, __ABORT_ADVFW_LIB__, str ) ;
  }
  strcpy( content_type, adv_dio_get_property(doc, "DirichletBCs_Axn0") ) ;
  if( !strcmp(content_type, "NEED") ) {
    mesh->dbc_Axn0.need = NEED ;
  } else if( !strcmp(content_type, "NO_NEED") ) {
    mesh->dbc_Axn0.need = NO_NEED ;
  } else {
    sprintf( str, "%s in \"DirichletBCs_Axn0\"", content_type ) ;
    emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
  }
  adv_dio_close( doc ) ;


  sprintf( content_type, "Element" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "content_type",
                                       content_type, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, content_type ) ;
  }
  adv_dio_get_property_int32( doc, "num_items", &(mesh->nel) ) ;
  adv_dio_get_property_int32( doc, "num_nodes_per_element",
                              &(mesh->nd_elem) ) ;
  adv_dio_get_property_int32( doc, "dimension", &(mesh->dimension) ) ;
  strcpy( mesh->elem_type, adv_dio_get_property(doc, "element_type") ) ;
  if( (!strcmp(mesh->elem_type, ELEM_TYPE_3DLT))
      || (!strcmp(mesh->elem_type, ELEM_TYPE_3DLH)) ) {
    mesh->ap_elem = mesh->nd_elem ;
  } else if( !strcmp(mesh->elem_type, ELEM_TYPE_3DQT) ) {
    mesh->ap_elem = 4 ;
  } else if( !strcmp(mesh->elem_type, ELEM_TYPE_3DQH) ) {
    mesh->ap_elem = 8 ;
  }
  size = mesh->nel * mesh->nd_elem ;
  mesh->nop = (int *)Malloc( sizeof(int), size ) ;
  off = 0 ;
  off += adv_dio_read_int32v( doc, off, size, mesh->nop ) ;
  adv_dio_close( doc ) ;


  sprintf( content_type, "Node" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "content_type",
                                       content_type, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, content_type ) ;
  }
  adv_dio_get_property_int32( doc, "num_items", &(mesh->nnd) ) ;
  size = mesh->nnd*mesh->dimension ;
  mesh->crd = (double *)Malloc( sizeof(double), size ) ;
  off = 0 ;
  off += adv_dio_read_float64v( doc, off, size, mesh->crd ) ;
  adv_dio_close( doc ) ;


  if( mesh->dbc_Axn0.need == NEED ) {
    sprintf( label, "DirichletBCs_Axn0" ) ;
    if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                         label, NULL)) == NULL ) {
      emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
    }
    adv_dio_get_property_int32( doc, "num_items", &(size) ) ;
    mesh->dbc_Axn0.nbc = size ;
    mesh->dbc_Axn0.ibc = (int *)Malloc( sizeof(int), size ) ;
    off = 0 ;
    off += adv_dio_read_int32v( doc, off, size, mesh->dbc_Axn0.ibc ) ;
    adv_dio_close( doc ) ;
  }


  sprintf( label, "Flag" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                       label, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
  }
  adv_dio_get_property_int32( doc, "num_items", &i ) ;
  if( i != mesh->nel ) {
    sprintf( content_type, "%d in label \"%s\"", i, label ) ;
    emessage( 120, __ABORT_ADVFW_LIB__, content_type ) ;
  }
  size = mesh->nel ;
  mesh->flag = (int *)Malloc( sizeof(int), size ) ;
  off = 0 ;
  off += adv_dio_read_int32v( doc, off, size, mesh->flag ) ;
  adv_dio_close( doc ) ;


  AdvDboxClose( dbox ) ;


  Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;
}

/********************************************************************/

 void write_onedata_file( char *fname, MESH mesh ) 
 { 
   /*+ Funciton: write onedata file +*/
   int    i ; 

   AdvDocFile  *fp ; 
   AdvDocument *doc ; 
   adv_off_t   off ; 

   int    nid = 0 ; 
   char   id[100][1024] ; 
   char   title[1024] ; 
   char   label[1024] ; 


   fp = AdvFopen( fname, "c" ) ; 


   Pfprintf( stdout, "  Start writing to \"%s\"\n", fname ) ; 


   sprintf( title, "Element" ) ; 
   strcpy( id[nid], (char *)adv_dio_make_documentid("Elements") ) ; 
   doc = adv_dio_create( fp, id[nid] ) ; 
   nid++ ; 
   adv_dio_set_property( doc, "content_type", title ) ; 
   adv_dio_set_property_int32( doc, "num_items", mesh.nel ) ; 
   adv_dio_set_property_int32( doc, "num_nodes_per_element", mesh.nd_elem ) ; 
   adv_dio_set_property_int32( doc, "dimension", mesh.dimension ) ; 
   adv_dio_set_property( doc, "element_type", mesh.elem_type ) ; 
   strcpy( label, "i4" ) ; 
   for( i=1 ; i<mesh.nd_elem ; i++ ) strcat( label, "i4" ) ; 
   adv_dio_set_property( doc, "format", label ) ; 
   off = 0 ; 
   off += adv_dio_write_int32v( doc, off, 
                                mesh.nel*mesh.nd_elem, mesh.nop ) ; 
   adv_dio_close( doc ) ; 


   sprintf( title, "Node" ) ; 
   strcpy( id[nid], (char *)adv_dio_make_documentid("Nodes") ) ; 
   doc = adv_dio_create( fp, id[nid] ) ; 
   nid++ ; 
   adv_dio_set_property( doc, "content_type", title ) ; 
   adv_dio_set_property_int32( doc, "num_items", mesh.nnd ) ; 
   adv_dio_set_property_int32( doc, "dimension", mesh.dimension ) ; 
   strcpy( label, "f8" ) ; 
   for( i=1 ; i<mesh.dimension ; i++ ) strcat( label, "f8" ) ; 
   adv_dio_set_property( doc, "format", label ) ; 
   off = 0 ; 
   off += adv_dio_write_float64v( doc, off, 
                                  mesh.nnd*mesh.dimension, mesh.crd ) ; 
   adv_dio_close( doc ) ; 


   if( mesh.dbc_Axn0.need == NEED ) { 
     sprintf( title, "FEGenericAttribute" ) ; 
     sprintf( label, "DirichletBCs_Axn0" ) ; 
     strcpy( id[nid], (char *)adv_dio_make_documentid(label) ) ; 
     doc = adv_dio_create( fp, id[nid] ) ; 
     nid++ ; 
     adv_dio_set_property( doc, "content_type", title ) ; 
     adv_dio_set_property_int32( doc, "num_items", mesh.dbc_Axn0.nbc ) ; 
     adv_dio_set_property( doc, "fega_type", "NodeVariable" ) ; 
     adv_dio_set_property( doc, "label", label ) ; 
     adv_dio_set_property( doc, "format", "" ) ; 
     adv_dio_set_property_int32( doc, "index_byte", 4 ) ; 
     off = 0 ; 
     off += adv_dio_write_int32v( doc, off, mesh.dbc_Axn0.nbc, 
                                  mesh.dbc_Axn0.ibc ) ; 
     adv_dio_close( doc ) ; 
   } 


   sprintf( title, "FEGenericAttribute" ) ; 
   sprintf( label, "Flag" ) ; 
   strcpy( id[nid], (char *)adv_dio_make_documentid(label) ) ; 
   doc = adv_dio_create( fp, id[nid] ) ; 
   nid++ ; 
   adv_dio_set_property( doc, "content_type", title ) ; 
   adv_dio_set_property_int32( doc, "num_items", mesh.nel ) ; 
   adv_dio_set_property( doc, "fega_type", "AllElementVariable" ) ; 
   adv_dio_set_property( doc, "label", label ) ; 
   adv_dio_set_property( doc, "format", "i4" ) ; 
   adv_dio_set_property_int32( doc, "index_byte", 4 ) ; 
   off = 0 ; 
   off += adv_dio_write_int32v( doc, off, mesh.nel, mesh.flag ) ; 
   adv_dio_close( doc ) ; 


   sprintf( title, "FEGenericAttribute" ) ; 
   sprintf( label, "Options" ) ; 
   strcpy( id[nid], (char *)adv_dio_make_documentid(label) ) ; 
   doc = adv_dio_create( fp, id[nid] ) ; 
   nid++ ; 
   adv_dio_set_property( doc, "content_type", title ) ; 
   adv_dio_set_property_int32( doc, "num_items", 0 ) ; 
   adv_dio_set_property( doc, "fega_type", "AllElementConstant" ) ; 
   adv_dio_set_property( doc, "label", label ) ; 
   adv_dio_set_property( doc, "format", "" ) ; 
   adv_dio_set_property_int32( doc, "index_byte", 4 ) ; 
   adv_dio_set_property( doc, "ADVFW_NAME", ADVFW_NAME ) ; 
   adv_dio_set_property_float64( doc, "N_VERSION", N_VERSION ) ; 
   if( mesh.dbc_Axn0.need == NEED ) { 
     adv_dio_set_property( doc, "DirichletBCs_Axn0", "NEED" ) ; 
   } else { 
     adv_dio_set_property( doc, "DirichletBCs_Axn0", "NO_NEED" ) ; 
   } 
   adv_dio_close( doc ) ; 


   AdvFclose( fp ) ; 


   Pfprintf( stdout, "  Finish writing to \"%s\"\n", fname ) ; 
 } 

/********************************************************************/



/********************************************************************/

void Bcast_Grp_onedata_file( MESH *mesh, OPTIONS options )
{
  /*+ Funciton: Bcast onedata file +*/
  int    size ;


  if( COM_GID.rootid != COM_ID.myprocid ) {
    init_MESH( mesh, options ) ;
  }
  COM_Grp_Bcast_Int( &(mesh->nel), 1, COM_GID.rootid ) ;
  COM_Grp_Bcast_Int( &(mesh->nnd), 1, COM_GID.rootid ) ;
  COM_Grp_Bcast_Int( &(mesh->ap_elem), 1, COM_GID.rootid ) ;
  COM_Grp_Bcast_Int( &(mesh->nd_elem), 1, COM_GID.rootid ) ;
  COM_Grp_Bcast_Chr( mesh->elem_type, 128, COM_GID.rootid ) ;
  COM_Grp_Bcast_Int( &(mesh->dimension), 1, COM_GID.rootid ) ;

  size = mesh->nel * mesh->nd_elem ;
  if( COM_GID.rootid != COM_ID.myprocid ) {
    mesh->nop = (int *)Malloc( sizeof(int), size ) ;
  }
  COM_Grp_Bcast_Int( mesh->nop, size, COM_GID.rootid ) ;

  size = mesh->nel ;
  if( COM_GID.rootid != COM_ID.myprocid ) {
    mesh->flag = (int *)Malloc( sizeof(int), size ) ;
  }
  COM_Grp_Bcast_Int( mesh->flag, size, COM_GID.rootid ) ;

  size = mesh->nnd * mesh->dimension ;
  if( COM_GID.rootid != COM_ID.myprocid ) {
    mesh->crd = (double *)Malloc( sizeof(double), size ) ;
  }
  COM_Grp_Bcast_Dbl( mesh->crd, size, COM_GID.rootid ) ;

  Bcast_Grp_DirichletBC( &(mesh->dbc_Axn0) ) ;
}

/********************************************************************/

void Bcast_onedata_file( MESH *mesh, OPTIONS options )
{
  /*+ Funciton: Bcast onedata file +*/
  int    size ;


  if( COM_GID.rootid != COM_ID.myprocid ) {
    init_MESH( mesh, options ) ;
  }
  COM_Bcast_Int( &(mesh->nel), 1, COM_GID.rootid ) ;
  COM_Bcast_Int( &(mesh->nnd), 1, COM_GID.rootid ) ;
  COM_Bcast_Int( &(mesh->ap_elem), 1, COM_GID.rootid ) ;
  COM_Bcast_Int( &(mesh->nd_elem), 1, COM_GID.rootid ) ;
  COM_Bcast_Chr( mesh->elem_type, 128, COM_GID.rootid ) ;
  COM_Bcast_Int( &(mesh->dimension), 1, COM_GID.rootid ) ;

  size = mesh->nel * mesh->nd_elem ;
  if( COM_GID.rootid != COM_ID.myprocid ) {
    mesh->nop = (int *)Malloc( sizeof(int), size ) ;
  }
  COM_Bcast_Int( mesh->nop, size, COM_GID.rootid ) ;

  size = mesh->nel ;
  if( COM_GID.rootid != COM_ID.myprocid ) {
    mesh->flag = (int *)Malloc( sizeof(int), size ) ;
  }
  COM_Bcast_Int( mesh->flag, size, COM_GID.rootid ) ;

  size = mesh->nnd * mesh->dimension ;
  if( COM_GID.rootid != COM_ID.myprocid ) {
    mesh->crd = (double *)Malloc( sizeof(double), size ) ;
  }
  COM_Bcast_Dbl( mesh->crd, size, COM_GID.rootid ) ;

  Bcast_DirichletBC( &(mesh->dbc_Axn0) ) ;
}

/********************************************************************/

int get_num_part_from_modeldata( char *fname )
{
  /*+ Funciton: get number of parts from modeldata +*/
  AdvDatabox  *dbox ;
  AdvDocument *doc ;

  int    npart ;
  char   content_type[1024] ;


  dbox = AdvDboxOpen( fname ) ;


  Pfprintf( stdout, "Getting number of parts from \"%s\"\n",
            fname ) ;
  sprintf( content_type, "HDDM_InterfaceDOF" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "content_type",
                                       content_type, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, content_type ) ;
  }
  adv_dio_get_property_int32( doc, "num_parts", &npart ) ;
  adv_dio_close( doc ) ;
  Pfprintf( stdout, "  Number of parts = %d\n", npart ) ;


  AdvDboxClose( dbox ) ;


  return npart ;
}

/********************************************************************/

void read_modeldata_file( char *fname, PartMesh *pm, OPTIONS options )
{
  /*+ Funciton: read modeldata file +*/
  int    i, j ;
  double d ;

  AdvDatabox  *dbox ;
  AdvDocument *doc ;
  adv_off_t   off ;

  char   content_type[1024] ;
  char   label[1024] ;
  char   str[1024] ;
  int    size ;
  
  dbox = AdvDboxOpen( fname ) ;

  Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;


  sprintf( label, "Options" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                       label, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
  }
  adv_dio_get_property_float64( doc, "N_VERSION", &d ) ;
  if( d < OUT_VERSION ) {
    sprintf( label, "%lf", d ) ;
    emessage( 100, __ABORT_ADVFW_LIB__, label ) ;
  }
  strcpy( content_type, adv_dio_get_property(doc, "DirichletBCs_Axn0") ) ;
  if( !strcmp(content_type, "NEED") ) {
    pm->dbc_Axn0.need = NEED ;
  } else if( !strcmp(content_type, "NO_NEED") ) {
    pm->dbc_Axn0.need = NO_NEED ;
  } else {
    sprintf( str, "%s in \"DirichletBCs_Axn0\"", content_type ) ;
    emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
  }
  adv_dio_close( doc ) ;


  sprintf( content_type, "Node" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "content_type",
                                       content_type, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, content_type ) ;
  }
  adv_dio_get_property_int32( doc, "num_items", &(pm->nnd) ) ;
  adv_dio_get_property_int32( doc, "num_items_orig", &(pm->nnd_all) ) ;
  adv_dio_get_property_int32( doc, "dimension", &(pm->dimension) ) ;
  size = pm->nnd * pm->dimension ;
  pm->crd = (double *)Malloc( sizeof(double), size ) ;
  off = 0 ;
  off += adv_dio_read_float64v( doc, off, size, pm->crd ) ;
  adv_dio_close( doc ) ;

  /*********************  NodeIndex_PartToGlobal  **************************/

  sprintf( label, "NodeIndex_PartToGlobal" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                       label, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
  }
  size = pm->nnd ;
  pm->pnid2gnid = (int *)Malloc( sizeof(int), size ) ;
  off = 0 ;
  off += adv_dio_read_int32v( doc, off, size, pm->pnid2gnid ) ;
  adv_dio_close( doc ) ;

  /************************  HDDM_InterfaceDOF  *******************************/

  sprintf( content_type, "HDDM_InterfaceDOF" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "content_type",
                                       content_type, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
  }
  adv_dio_get_property_int32( doc, "num_parts", &(pm->npart) ) ;
  adv_dio_get_property_int32( doc, "part_number", &(pm->partid) ) ;
  adv_dio_get_property_int32( doc, "num_subdomains", &(pm->ndomain) ) ;
  adv_dio_get_property_int32( doc, "num_outdom", &(pm->n_outdom) ) ;
  adv_dio_get_property_int32( doc, "total_num_infree", &(pm->infree) ) ;
  adv_dio_get_property_int32( doc, "total_num_outfree", &(pm->outfree) ) ;
  adv_dio_get_property_int32( doc, "total_num_midfree", &(pm->midfree) ) ;
  pm->tfree = pm->infree + pm->outfree ;
  pm->op = (OPinfo *)Malloc( sizeof(OPinfo), pm->npart ) ;
  off = 0 ;
  for( i=0 ; i<pm->npart ; i++ ) {
    init_OPinfo( &(pm->op[i]) ) ;
    off += adv_dio_read_int32( doc, off, &size ) ;
    pm->op[i].n_mofree = size ;
    pm->op[i].ibid = (int *)Malloc( sizeof(int), size ) ;
    off += adv_dio_read_int32v( doc, off, size, pm->op[i].ibid ) ;
  }
  adv_dio_close( doc ) ;


  /**************************  HDDM_Element  ******************************/

  sprintf( content_type, "HDDM_Element" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "content_type",
                                       content_type, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, content_type ) ;
  }
  strcpy( pm->elem_type, adv_dio_get_property(doc, "element_type") ) ;
  adv_dio_get_property_int32( doc, "num_nodes_per_element",
                              &(pm->nd_elem) ) ;
  if( (!strcmp(pm->elem_type, ELEM_TYPE_3DLT))
      || (!strcmp(pm->elem_type, ELEM_TYPE_3DLH)) ) {
    pm->ap_elem = pm->nd_elem ;
  } else if( !strcmp(pm->elem_type, ELEM_TYPE_3DQT) ) {
    pm->ap_elem = 4 ;
  } else if( !strcmp(pm->elem_type, ELEM_TYPE_3DQH) ) {
    pm->ap_elem = 8 ;
  }
  adv_dio_get_property_int32( doc, "sum_items", &(pm->nel) ) ;
  adv_dio_get_property_int32( doc, "num_items_orig", &(pm->nel_all) ) ;
  pm->dm = (DomMesh *)Malloc( sizeof(DomMesh), pm->ndomain ) ;
  off = 0 ;
  for( i=0 ; i<pm->ndomain ; i++ ) {
    init_DomMesh( &(pm->dm[i]), options ) ;
    sprintf( pm->dm[i].elem_type, "%s", pm->elem_type ) ;
    pm->dm[i].partid = pm->partid ;
    pm->dm[i].domid = i ;
    pm->dm[i].npart = pm->npart ;
    pm->dm[i].ndomain = pm->ndomain ;
    pm->dm[i].ap_elem = pm->ap_elem ;
    pm->dm[i].nd_elem = pm->nd_elem ;
    pm->dm[i].dimension = pm->dimension ;
    off += adv_dio_read_int32( doc, off, &size ) ;
    if( size == 0 ) {
      sprintf( str, ": zero in part %d, subdomain %d", pm->partid, i ) ;
      emessage( 120, __ABORT_ADVFW_LIB__, str ) ;
    }
    pm->dm[i].nel = size ;
    size *= pm->nd_elem ;
    pm->dm[i].nop = (int *)Malloc( sizeof(int), size ) ;
    off += adv_dio_read_int32v( doc, off, size, pm->dm[i].nop ) ;
  }
  adv_dio_close( doc ) ;

  /********************  NodeIndex_SubdomainToPar  **************************/

  sprintf( label, "NodeIndex_SubdomainToPart" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                       label, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
  }
  off = 0 ;
  for( i=0 ; i<pm->ndomain ; i++ ) {
    off += adv_dio_read_int32( doc, off, &size ) ;
    pm->dm[i].nnd = size ;
    pm->dm[i].snid2pnid = (int *)Malloc( sizeof(int), size ) ;
    off += adv_dio_read_int32v( doc, off, size, pm->dm[i].snid2pnid ) ;
  }
  adv_dio_close( doc ) ;

  /**************************  InterfaceDOF  *********************************/

  sprintf( label, "InterfaceDOF" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                       label, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
  }
  adv_dio_get_property_int32( doc, "ifd_dimension", &(pm->ifd_dimension) ) ;
    off = 0 ;
  for( i=0 ; i<pm->ndomain ; i++ ) {
    off += adv_dio_read_int32( doc, off, &size ) ;
    pm->dm[i].ninbc = size ;
    pm->dm[i].inbc = (Inbc *)Malloc( sizeof(Inbc), size ) ;
    for( j=0 ; j<size ; j++ ) {
      off += adv_dio_read_int32( doc, off, &(pm->dm[i].inbc[j].snid) ) ;
      off += adv_dio_read_int32( doc, off, &(pm->dm[i].inbc[j].coordinate) ) ;
      off += adv_dio_read_int32( doc, off, &(pm->dm[i].inbc[j].charge_part) ) ;
      off += adv_dio_read_int32( doc, off, &(pm->dm[i].inbc[j].ln_chpar) ) ;
      off += adv_dio_read_int32( doc, off, &(pm->dm[i].inbc[j].ln_part) ) ;
    }
  }
  adv_dio_close( doc ) ;

  /********************** DirichletBCs_Axn0  ******************************/
  if( pm->dbc_Axn0.need == NEED ) {
    sprintf( label, "DirichletBCs_Axn0" ) ;
    if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                         label, NULL)) == NULL ) {
      emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
    }
    adv_dio_get_property_int32( doc, "num_items_orig", &(pm->dbc_Axn0.nbc) ) ; 
    off = 0 ;
    for( i=0 ; i<pm->ndomain ; i++ ) {
      pm->dm[i].dbc_Axn0.need = NEED ;
      off += adv_dio_read_int32( doc, off, &size ) ;
      pm->dm[i].dbc_Axn0.nbc = size ;
      pm->dm[i].dbc_Axn0.ibc = (int *)Malloc( sizeof(int), size ) ;
      off += adv_dio_read_int32v( doc, off, size, pm->dm[i].dbc_Axn0.ibc ) ;
    }
      adv_dio_close( doc ) ;
  }

  /********************** Flag ******************************/
  sprintf( label, "Flag" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                       label, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
  }
  off = 0 ;
  for( i=0 ; i<pm->ndomain ; i++ ) {
    off += adv_dio_read_int32( doc, off, &size ) ;
    pm->dm[i].flag = (int *)Malloc( sizeof(int), size ) ;
    off += adv_dio_read_int32v( doc, off, size, pm->dm[i].flag ) ;
  }
  adv_dio_close( doc ) ;

  /************* ElementIndex_SubdomainToGlobal  *************/
  sprintf( label, "ElementIndex_SubdomainToGlobal" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                       label, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
  }
  off = 0 ;
  for( i=0 ; i<pm->ndomain ; i++ ) {
    off += adv_dio_read_int32( doc, off, &size ) ;
    pm->dm[i].seid2geid = (int *)Malloc( sizeof(int), size ) ;
    off += adv_dio_read_int32v( doc, off, size, pm->dm[i].seid2geid ) ;
  }
  adv_dio_close( doc ) ;


  AdvDboxClose( dbox ) ;


  Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;
}

/********************************************************************/

void read_Jodata_file( char *fname, int nap, int dimension,
                       ANTENNA *antenna, MTRL mtrl )
{
  /*+ Funciton: read Jodata file +*/
  int    nJo ;
  int    *iJo ;
  double *Jo ;


  read_Jodata_file_read( fname, &nJo, &iJo, &Jo, dimension ) ;


  read_Jodata_file_decompose( nap, dimension, antenna, mtrl, nJo, iJo, Jo ) ;


  Free( sizeof(int)*nJo, iJo ) ;
  Free( sizeof(double)*nJo*dimension, Jo ) ;
}

/********************************************************************/

void read_Jodata_file_read( char *fname,
                            int *nJo, int **iJo, double **Jo,
                            int dimension )
{
  /*+ Funciton: read Jodata file +*/
  int    i, j, k ;
  FILE   *fp ;


  fp = Fopen( fname, "r" ) ;


  /*Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;*/


  fscanf( fp, "%d", nJo ) ;
  *iJo = (int *)Malloc( sizeof(int), (*nJo) ) ;
  *Jo = (double *)Malloc( sizeof(double), (*nJo)*dimension ) ;
  for( i=0,k=0 ; i<(*nJo) ; i++ ) {
    fscanf( fp, "%d", &((*iJo)[i]) ) ;
    for( j=0 ; j<dimension ; j++,k++ ) {
      fscanf( fp, "%le", &((*Jo)[k]) ) ;
    }
  }


  Fclose( fp ) ;


  sort_id( (*nJo), (*iJo), (*Jo), dimension ) ;


  /*Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;*/
}

/********************************************************************/

void read_Jodata_file_decompose( int nap, int dimension,
                                 ANTENNA *antenna, MTRL mtrl,
                                 int nJo, int *iJo, double *Jo )
{
  /*+ Funciton: decompose Jo to each antenna +*/
  int    i, j ;

  int    *apid2Joid ;
  int    *apid2gnid ;


  apid2gnid = antenna->apid2gnid ;


  apid2Joid = (int *)Malloc( sizeof(int), nap ) ;
  for( i=0 ; i<nap ; i++ ) {
    apid2Joid[i] = -1 ;
  }
  for( i=0,j=0 ; i<nJo ; i++ ) {
    for( ; j<nap ; j++ ) {
      if( apid2gnid[j] > iJo[i] ) {
        break ;
      }
      if( apid2gnid[j] == iJo[i] ) {
        apid2Joid[j] = i ;
        break ;
      }
    }
  }
  antenna->Jo = (double *)Malloc( sizeof(double), antenna->nf*dimension ) ;
  for( i=0 ; i<nap ; i++ ) {
    int    ii = antenna->enf[i] ;
    if( ii > -1 ) {
      int    jj = apid2Joid[i] ;
      if( jj > -1 ) {
        int    iii = ii * dimension ;
        int    kk = jj * dimension ;
        for( j=0 ; j<dimension ; j++,iii++,kk++ ) {
          antenna->Jo[iii] = Jo[kk] ;
        }
      } else {
        int    iii = ii * dimension ;
        for( j=0 ; j<dimension ; j++,iii++ ) {
          antenna->Jo[iii] = 0.0 ;
        }
      }
    }
  }


  Free( sizeof(int)*nap, apid2Joid ) ;
}

/********************************************************************/

void write_Jodata_file( char *fname, int nap, int dimension,
                        ANTENNA antenna, MTRL mtrl )
{
  /*+ Funciton: write Jodata file +*/
  int    i, j, k ;
  FILE   *fp ;

  int    nJo ;
  int    *iJo ;
  double *Jo ;
  int    *apid2gnid ;


  apid2gnid = antenna.apid2gnid ;


  fp = Fopen( fname, "w" ) ;


  Pfprintf( stdout, "  Start writing to \"%s\"\n", fname ) ;


  nJo = antenna.nf ;
  iJo = (int *)Malloc( sizeof(int), nJo ) ;
  Jo = (double *)Malloc( sizeof(double), nJo*dimension ) ;
  for( i=0,k=0 ; i<nap ; i++ ) {
    int    ii = antenna.enf[i] ;
    if( ii > -1 ) {
      int    jj = ii * dimension ;
      int    kk = k * dimension ;
      iJo[k] = apid2gnid[i] ;
      for( j=0 ; j<dimension ; j++,jj++,kk++ ) {
        Jo[kk] = antenna.Jo[jj] ;
      }
      k++ ;
    }
  }
  sort_id( nJo, iJo, Jo, dimension ) ;


  fprintf( fp, "%d\n", nJo ) ;
  for( i=0,k=0 ; i<nJo ; i++ ) {
    fprintf( fp, "%d", iJo[i] ) ;
    for( j=0 ; j<dimension ; j++,k++ ) {
      fprintf( fp, " % le", Jo[k] ) ;
    }
    fprintf( fp, "\n" ) ;
  }


  Free( sizeof(int)*nJo, iJo ) ;
  Free( sizeof(double)*nJo*dimension, Jo ) ;


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish writing to \"%s\"\n", fname ) ;
}

/********************************************************************/

void read_Jodefine_file( char *fname, MTRL_ANTENNA_DEFINITION *antenna_def )
{
  /*+ Funciton: read Jodefine file +*/
  FILE   *fp ;

  char   title[1024] ;
  char   analysis[128] ;


  fp = Fopen( fname, "r" ) ;


  /*Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;*/


  fscanf( fp, "%s", analysis ) ;

  fscanf( fp, "%s", title ) ;
  while( !feof(fp) ) {

    if( !strcmp(title, MTRL_ANTENNA_FORM_PARALLELPIPED) ) {
      int    npp = antenna_def->npp ;
      antenna_def->pp = (MTRL_ANTENNA_DEF_PP *)Realloc( antenna_def->pp,
                                                  sizeof(MTRL_ANTENNA_DEF_PP),
                                                  npp, npp+1 ) ;
      antenna_def->pp[npp].n = antenna_def->ndef ;
      read_Jodefine_file_pp( fp, &(antenna_def->pp[npp]), analysis ) ;
      antenna_def->npp++ ;
      antenna_def->ndef++ ;

    } else if( !strcmp(title, MTRL_ANTENNA_FORM_DOUBLESECTORIALCYLINDER) ) {
      int    ndsc = antenna_def->ndsc ;
      antenna_def->dsc = (MTRL_ANTENNA_DEF_DSC *)Realloc( antenna_def->dsc,
                                                    sizeof(MTRL_ANTENNA_DEF_DSC),
                                                    ndsc, ndsc+1 ) ;
      antenna_def->dsc[ndsc].n = antenna_def->ndef ;
      read_Jodefine_file_dsc( fp, &(antenna_def->dsc[ndsc]), analysis ) ;
      antenna_def->ndsc++ ;
      antenna_def->ndef++ ;

    } else {
      char str[2048] ;
      sprintf( str, "%s in %s", title, fname ) ;
      emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
    }

    fscanf( fp, "%s", title ) ;
  }


  Fclose( fp ) ;


  /*Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;*/
}

/********************************************************************/

void read_Jodefine_file_pp( FILE *fp, MTRL_ANTENNA_DEF_PP *pp,
                            char *analysis )
{
  /*+ Funciton: read MTRL_ANTENNA_DEF_PP from Jodefine file +*/
  int    i, j, k ;

  double vec[3][3] ;


  /* read data */
  for( i=0 ; i<4 ; i++ ) {
    for( j=0 ; j<3 ; j++ ) {
      fscanf( fp, "%le", &(pp->ap[i][j]) ) ;
    }
  }
  sprintf( pp->analysis, "%s", analysis ) ;
  if( !strcmp(analysis, MTRL_ANTENNA_ANALYSIS_FULL_WAVE) ) {
    for( i=0 ; i<3 ; i++ ) {
      fscanf( fp, "%le", &(pp->Jor[i]) ) ;
    }
    for( i=0 ; i<3 ; i++ ) {
      fscanf( fp, "%le", &(pp->Joi[i]) ) ;
    }
  } else {
    char str[2048] ;
    sprintf( str, "%s for %dth in Jo definition file", analysis, pp->n ) ;
    emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
  }


  /* check data */


  /* make a, b, c, d */
  for( i=0,k=1 ; i<3 ;i++,k++ ) {
    for( j=0 ; j<3 ; j++ ) {
      vec[i][j] = pp->ap[k][j] - pp->ap[0][j] ;
    }
  }
  for( i=0 ; i<3 ; i++ ) {
    pp->ap[4][i] = pp->ap[0][i] ;
  }
  for( i=0 ; i<3 ; i++ ) {
    for( j=0 ; j<3 ; j++ ) {
      pp->ap[4][j] += vec[i][j] ;
    }
  }
  make_face_equation_abc( vec[0], vec[1], pp->abc[0] ) ;
  make_face_equation_abc( vec[0], vec[2], pp->abc[1] ) ;
  make_face_equation_abc( vec[1], vec[2], pp->abc[2] ) ;
  for( i=0 ; i<3 ; i++ ) {
    make_face_equation_d( pp->abc[i], pp->ap[0], &(pp->d[0][i]) ) ;
    make_face_equation_d( pp->abc[i], pp->ap[4], &(pp->d[1][i]) ) ;
  }
  {
    double tol = 1.0e+100 ;
    for( i=0 ; i<3 ; i++ ) {
      double dd = 0.0 ;
      for( j=0 ; j<3 ; j++ ) {
        dd += vec[i][j] * vec[i][j] ;
      }
      dd = sqrt( dd ) ;
      if( dd < tol ) tol = dd ;
    }
    pp->tolerance = tol * 1.0e-05 ;
  }
}

/********************************************************************/

void make_face_equation( double *p, double *q, double *ap,
                         double *abc, double *d )
{
  /*+ Funciton: make face equation (a, b, c, d) +*/
  /*+           p x q = (a, b, c) +*/
  /*+           d = -ax-by-cz +*/
  make_face_equation_abc( p, q, abc ) ;
  make_face_equation_d( abc, ap, d ) ;
}

/********************************************************************/

void make_face_equation_abc( double *p, double *q, double *abc )
{
  /*+ Funciton: make face equation (a, b, c) +*/
  /*+           p x q = (a, b, c) +*/
  make_OuterProduct( p, q, abc ) ;
}

/********************************************************************/

void make_face_equation_d( double *abc, double *ap, double *d )
{
  /*+ Funciton: make face equation (d) +*/
  /*+           d = -ax-by-cz +*/
  int    i ;
  double dd = 0.0 ;


  for( i=0 ; i<3 ; i++ ) {
    dd += abc[i] * ap[i] ;
  }
  (*d) = -dd ;
}

/********************************************************************/

void make_OuterProduct( double *p, double *q, double *OuterProduct )
{
  /*+ Funciton: make outer product p x q = (a, b, c) +*/
  int    i ;

  int    n[][2] = { { 1, 2 } ,
                    { 2, 0 } ,
                    { 0, 1 } } ;


  for( i=0 ; i<3 ; i++ ) {
    OuterProduct[i] = p[n[i][0]]*q[n[i][1]] - p[n[i][1]]*q[n[i][0]] ;
  }
}

/********************************************************************/

void read_Jodefine_file_dsc( FILE *fp, MTRL_ANTENNA_DEF_DSC *dsc,
                             char *analysis )
{
  /*+ Funciton: read MTRL_ANTENNA_DEF_DSC from Jodefine file +*/
  int    i ;

  double height ;


  /*** read data ***/
  /** reference point **/
  {
    double ap[3] ;
    char   xyz[128] ;
    int    x = 0, y = 0, z = 0 ;
    for( i=0 ; i<3 ; i++ ) {
      fscanf( fp, "%le", &(ap[i]) ) ;
    }
    fscanf( fp, "%s", xyz ) ;
    fscanf( fp, "%le", &height ) ;
    if( height == 0.0 ) {
      char str[128] ;
      sprintf( str, ": height of DSC in Jo definition file" ) ;
      emessage( 114, __ABORT_NONE__, str ) ;
      sprintf( str, ": height shuld not be 0" ) ;
      emessage( 114, __ABORT_ADVFW_LIB__, str ) ;
    }
    if( !strcmp(xyz, "x") ) {
      dsc->xyz = 0 ; x = 1 ; y = 2 ; z = 0 ;
    } else if( !strcmp(xyz, "y") ) {
      dsc->xyz = 1 ; x = 2 ; y = 0 ; z = 1 ;
    } else if( !strcmp(xyz, "z") ) {
      dsc->xyz = 2 ; x = 0 ; y = 1 ; z = 2 ;
    } else {
      char str[2048] ;
      sprintf( str, "%s for %dth in Jo definition file", xyz, dsc->n ) ;
      emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
    }
    dsc->ap[0] = ap[x] ;
    dsc->ap[1] = ap[y] ;
    if( height > 0.0 ) {
      dsc->ap[2] = ap[z] ;
      dsc->ap[3] = dsc->ap[2] + height ;
    } else {
      dsc->ap[3] = ap[z] ;
      dsc->ap[2] = dsc->ap[3] + height ;
    }
  }

  /** starting point & rotation angle **/
  {
    char   rad_deg[128] ;
    double theta ;
    double angle ;
    double dd = 2.0 * PI ;
    double ee = -dd ;
    fscanf( fp, "%s", rad_deg ) ;
    fscanf( fp, "%le", &theta ) ;
    fscanf( fp, "%le", &angle ) ;
    if( strcmp(rad_deg, "rad") ) {
      if( !strcmp(rad_deg, "deg") ) {
        theta = DEG_RAD( theta ) ;
        angle = DEG_RAD( angle ) ;
      } else {
        char str[2048] ;
        sprintf( str, "%s for %dth in Jo definition file", rad_deg, dsc->n ) ;
        emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
      }
    }
    if( angle == 0.0 ) {
      char str[128] ;
      sprintf( str, ": rotation angle of DSC in Jo definition file" ) ;
      emessage( 114, __ABORT_NONE__, str ) ;
      sprintf( str, ": rotation angle shuld not be 0" ) ;
      emessage( 114, __ABORT_ADVFW_LIB__, str ) ;
    } else if( (angle > dd) || (angle < ee) ) {
      dsc->theta[0] = dsc->theta[1] = 0.0 ;
    } else if( angle > 0.0 ) {
      dsc->theta[0] = theta ;
      dsc->theta[1] = theta + angle ;
    } else {
      dsc->theta[1] = theta ;
      dsc->theta[0] = theta + angle ;
    }
    while( PI < dsc->theta[0] ) {
      dsc->theta[0] += ee ;
      dsc->theta[1] += ee ;
    }
    while( dsc->theta[0] < (-PI) ) {
      dsc->theta[0] += dd ;
      dsc->theta[1] += dd ;
    }
  }

  /** radius **/
  for( i=0 ; i<2 ; i++ ) {
    fscanf( fp, "%le", &(dsc->r[i]) ) ;
  }
  if( (0 <= dsc->r[0] ) && (dsc->r[0] < dsc->r[1]) ) {
    dsc->r[0] *= dsc->r[0] ;
    dsc->r[1] *= dsc->r[1] ;
  } else {
    char str[128] ;
    sprintf( str, ": radius of DSC in Jo definition file" ) ;
    emessage( 114, __ABORT_ADVFW_LIB__, str ) ;
  }

  /* Jo */
  sprintf( dsc->analysis, "%s", analysis ) ;
  if( !strcmp(analysis, MTRL_ANTENNA_ANALYSIS_FULL_WAVE) ) {
    fscanf( fp, "%le", &(dsc->Jor) ) ;
     fscanf( fp, "%le", &(dsc->Joi) ) ;
  } else {
    char str[2048] ;
    sprintf( str, "%s for %dth in Jo definition file", analysis, dsc->n ) ;
    emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
  }
  if( height < 0.0 ) {
    dsc->Jor = -dsc->Jor ;
    dsc->Joi = -dsc->Joi ;
  }
}

/********************************************************************/

void make_Jo_from_definition( MTRL_ANTENNA_DEFINITION antenna_def, ANTENNA *antenna,
                              int nap, int dimension, double *crd,
                              int Jo_sw )
{
  /*+ Funciton: make Jo from definition +*/
  int    i, j ;

  int    ndef = antenna_def.ndef ;


  if( Jo_sw == -2 ) {
    antenna->Jor = antenna->Jo ;
    for( i=0 ; i<antenna_def.npp ; i++ ) {
      for( j=0 ; j<3 ; j++ ) {
        double dd ;
        dd = antenna_def.pp[i].Jor[j] ;
        antenna_def.pp[i].Jor[j] = antenna_def.pp[i].Joi[j] ;
        antenna_def.pp[i].Joi[j] = dd ;
      }
    }
    for( i=0 ; i<antenna_def.ndsc ; i++ ) {
      double dd ;
      dd = antenna_def.dsc[i].Jor ;
      antenna_def.dsc[i].Jor = antenna_def.dsc[i].Joi ;
      antenna_def.dsc[i].Joi = dd ;
    }
  }

  antenna->Jo = (double *)Calloc( sizeof(double), antenna->nf*dimension ) ;


  if( antenna_def.npp == 0 ) {
    for( i=ndef-1 ; i>=0 ; i-- ) {
      make_Jo_from_definition_dsc( antenna_def.dsc[i], antenna,
                                   nap, dimension, crd ) ;
    }

  } else if( antenna_def.ndsc == 0 ) {
    for( i=ndef-1 ; i>=0 ; i-- ) {
      make_Jo_from_definition_pp( antenna_def.pp[i], antenna,
                                  nap, dimension, crd ) ;
    }

  } else {
    int    npp = antenna_def.npp - 1 ;
    int    ndsc = antenna_def.ndsc - 1 ;
    for( i=ndef-1 ; i>=0 ; i-- ) {
      if( antenna_def.pp[npp].n == i ) {
        make_Jo_from_definition_pp( antenna_def.pp[npp], antenna,
                                    nap, dimension, crd ) ;
        npp-- ;
      } else if( antenna_def.dsc[ndsc].n == i ) {
        make_Jo_from_definition_dsc( antenna_def.dsc[ndsc], antenna,
                                     nap, dimension, crd ) ;
        ndsc-- ;
      }
    }
  }

  if( Jo_sw == -2 ) {
    for( i=0 ; i<antenna_def.npp ; i++ ) {
      for( j=0 ; j<3 ; j++ ) {
        double dd ;
        dd = antenna_def.pp[i].Jor[j] ;
        antenna_def.pp[i].Jor[j] = antenna_def.pp[i].Joi[j] ;
        antenna_def.pp[i].Joi[j] = dd ;
      }
    }
    for( i=0 ; i<antenna_def.ndsc ; i++ ) {
      double dd ;
      dd = antenna_def.dsc[i].Jor ;
      antenna_def.dsc[i].Jor = antenna_def.dsc[i].Joi ;
      antenna_def.dsc[i].Joi = dd ;
    }
  }
}

/********************************************************************/

void make_Jo_from_definition_pp( MTRL_ANTENNA_DEF_PP pp, ANTENNA *antenna,
                                 int nap, int dimension, double *crd )
{
  /*+ Funciton: make Jo from definition PP +*/
  int    i, j ;

  int*   enf = antenna->enf ;
  double* Jo = antenna->Jo ;


  for( i=0 ; i<nap ; i++ ) {
    int    ii = enf[i] ;
    int    jj ;
    int    kk = 0 ;
    double* ap ;
    if( ii < 0 ) {
      continue ;
    }
    jj = i * dimension ;
    ap = &(crd[jj]) ;
    for( j=0 ; j<3 ;j++ ) {
      kk = make_Jo_from_definition_pp_confirm( ap, pp.ap[0], pp.tolerance,
                                               pp.abc[j], pp.d[1][j] );
      if( kk == 0 ) break ;
      kk = make_Jo_from_definition_pp_confirm( ap, pp.ap[4], pp.tolerance,
                                               pp.abc[j], pp.d[0][j] );
      if( kk == 0 ) break ;
    }

    if( kk == 1 ) {
      jj = ii * dimension ;
      for( j=0 ; j<3 ; j++,jj++ ) {
        Jo[jj] = pp.Jor[j] ;
      }
    }
  }
}

/********************************************************************/

int make_Jo_from_definition_pp_confirm( double *ap, double *xyz,
                                        double tolerance,
                                        double *abc, double d )
{
  /*+ Funciton: confirm the ap is in domain or not +*/
  int    i ;
  double d0 = d , d1 = d ;


  for( i=0 ; i<3 ; i++ ) {
    d0 += abc[i] * ap[i] ;
    d1 += abc[i] * xyz[i] ;
  }
  if( fabs(d0) < tolerance ) d0 = 0.0 ;
  if( fabs(d1) < tolerance ) d1 = 0.0 ;
  d = d0 * d1 ;
  if( d >= 0.0 ) {
    return 1 ;
  } else {
    return 0 ;
  }
}

/********************************************************************/

void make_Jo_from_definition_dsc( MTRL_ANTENNA_DEF_DSC dsc, ANTENNA *antenna,
                                  int nap, int dimension, double *crd )
{
  /*+ Funciton: make Jo from definition DSC +*/
  int    i, j ;

  int*   enf = antenna->enf ;
  double* Jo = antenna->Jo ;
  int    x = 0, y = 0, z = 0 ;


  if( dsc.xyz == 0 ) {
    x = 1 ; y = 2 ; z = 0 ;
  } else if( dsc.xyz == 1 ) {
    x = 2 ; y = 0 ; z = 1 ;
  } else if( dsc.xyz == 2 ) {
    x = 0 ; y = 1 ; z = 2 ;
  }


  for( i=0 ; i<nap ; i++ ) {
    int    ii = enf[i] ;
    int    jj ;
    double ap[3] ;
    double theta = 0.0 ;

    if( ii < 0 ) {
      continue ;
    }

    jj = i * dimension ;
    ap[0] = crd[jj+x] - dsc.ap[0] ;
    ap[1] = crd[jj+y] - dsc.ap[1] ;
    ap[2] = crd[jj+z] ;

    /* r0 <= l <= r1 */
    {
      double l = 0.0 ;
      for( j=0 ; j<2 ; j++ ) {
        l += ap[j] * ap[j] ;
      }
      if( (l < dsc.r[0]) || (dsc.r[1] < l) ) {
        continue ;
      }
    }

    /* x0 <= x2 <= x1 */
    /* y0 <= y2 <= y1 */
    /* z0 <= z2 <= z1 */
    if( (ap[2] < dsc.ap[2]) || (dsc.ap[3] < ap[2]) ) {
      continue ;
    }

    /* theta0 <= theta2 <= theta1 */
    {
      double dd = 2.0 * PI ;
      theta = atan2( ap[1], ap[0] ) ;
      if( !((dsc.theta[0] == 0.0) && (dsc.theta[1] == 0.0)) ) {
        if( dsc.theta[1] < theta ) {
          continue ;
        } else if( theta < dsc.theta[0] ) {
          theta += dd ;
          if( dsc.theta[1] < theta ) {
            continue ;
          }
        }
      }
    }

    /* make Jo */
    jj = ii * dimension ;
    Jo[jj+x] = -dsc.Jor * sin(theta) ;
    Jo[jj+y] = dsc.Jor * cos(theta) ;
    Jo[jj+z] = 0.0 ;
  }
}

/********************************************************************/

void write_Jodefine_file( char *fname, MTRL_ANTENNA_DEFINITION antenna_def )
{
  /*+ Funciton: write Jodefine file +*/
  int    i ;
  FILE   *fp ;

  int    ndef = antenna_def.ndef ;


  fp = Fopen( fname, "w" ) ;


  Pfprintf( stdout, "  Start writing to \"%s\"\n", fname ) ;


  if( antenna_def.npp == 0 ) {
    fprintf( fp, "%s\n", antenna_def.dsc[0].analysis ) ;
    for( i=0 ; i<ndef ; i++ ) {
      write_Jodefine_file_dsc( fp, antenna_def.dsc[i] ) ;
    }

  } else if( antenna_def.ndsc == 0 ) {
    fprintf( fp, "%s\n", antenna_def.pp[0].analysis ) ;
    for( i=0 ; i<ndef ; i++ ) {
      write_Jodefine_file_pp( fp, antenna_def.pp[i] ) ;
    }

  } else {
    int    npp = 0 ;
    int    ndsc = 0 ;
    fprintf( fp, "%s\n", antenna_def.pp[0].analysis ) ;
    for( i=0 ; i<ndef ; i++ ) {
      if( antenna_def.pp[npp].n == i ) {
        write_Jodefine_file_pp( fp, antenna_def.pp[npp] ) ;
        npp++ ;
      } else if( antenna_def.dsc[ndsc].n == i ) {
        write_Jodefine_file_dsc( fp, antenna_def.dsc[ndsc] ) ;
        ndsc++ ;
      }
    }
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish writing to \"%s\"\n", fname ) ;
}

/********************************************************************/

void write_Jodefine_file_pp( FILE *fp, MTRL_ANTENNA_DEF_PP pp )
{
  /*+ Funciton: write MTRL_ANTENNA_DEF_PP to Jodefine file +*/
  int    i, j ;


  fprintf( fp, "\n%s\n", MTRL_ANTENNA_FORM_PARALLELPIPED ) ;
  for( i=0 ; i<4 ; i++ ) {
    for( j=0 ; j<3 ; j++ ) {
      fprintf( fp, " % lg", pp.ap[i][j] ) ;
    }
    fprintf( fp, "\n" ) ;
  }
  if( !strcmp(pp.analysis, MTRL_ANTENNA_ANALYSIS_FULL_WAVE) ) {
    for( i=0 ; i<3 ; i++ ) {
      fprintf( fp, " % lg", pp.Jor[i] ) ;
    }
    fprintf( fp, "\n" ) ;
    for( i=0 ; i<3 ; i++ ) {
      fprintf( fp, " % lg", pp.Joi[i] ) ;
    }
    fprintf( fp, "\n" ) ;
  } else {
    char str[2048] ;
    sprintf( str, "%s for %dth in Jo definition file", pp.analysis, pp.n ) ;
    emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
  }
}

/********************************************************************/

void write_Jodefine_file_dsc( FILE *fp, MTRL_ANTENNA_DEF_DSC dsc )
{
  /*+ Funciton: write MTRL_ANTENNA_DEF_DSC to Jodefine file +*/
  int    i ;


  fprintf( fp, "\n%s\n", MTRL_ANTENNA_FORM_DOUBLESECTORIALCYLINDER ) ;
  if( dsc.xyz == 0 ) {
    fprintf( fp, " % lg % lg % lg\n", dsc.ap[2], dsc.ap[0], dsc.ap[1] ) ;
    fprintf( fp, " x % lg\n", dsc.ap[3]-dsc.ap[2] ) ;
  } else if( dsc.xyz == 1 ) {
    fprintf( fp, " % lg % lg % lg\n", dsc.ap[1], dsc.ap[2], dsc.ap[0] ) ;
    fprintf( fp, " y % lg\n", dsc.ap[3]-dsc.ap[2] ) ;
  } else if( dsc.xyz == 2 ) {
    fprintf( fp, " % lg % lg % lg\n", dsc.ap[0], dsc.ap[1], dsc.ap[2] ) ;
    fprintf( fp, " z % lg\n", dsc.ap[3]-dsc.ap[2] ) ;
  } else {
    char str[2048] ;
    sprintf( str, "%d for %dth in Jo definition file", dsc.xyz, dsc.n ) ;
    emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
  }
  fprintf( fp, " rad % lg ", dsc.theta[0] ) ;
  if( dsc.theta[1] == 0 ) {
    fprintf( fp, "% lg\n", 3*PI ) ;
  } else {
    fprintf( fp, "% lg\n", dsc.theta[1]-dsc.theta[0] ) ;
  }
  for( i=0 ; i<2 ; i++ ) {
    fprintf( fp, " % lg", sqrt(dsc.r[i]) ) ;
  }
  fprintf( fp, "\n" ) ;
  if( !strcmp(dsc.analysis, MTRL_ANTENNA_ANALYSIS_FULL_WAVE) ) {
    fprintf( fp, " % lg % lg\n", dsc.Jor, dsc.Joi ) ;
  } else {
    char str[2048] ;
    sprintf( str, "%s for %dth in Jo definition file", dsc.analysis, dsc.n ) ;
    emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
  }
}

/********************************************************************/

void read_Inidata_file( char *fname, MESH *mesh )
{
  /*+ Funciton: read Inidata file +*/
  int    i, j ;
  FILE   *fp ;

  int    el ;
  int    nel ;
  int    dimension = mesh->dimension ;


  fp = Fopen( fname, "r" ) ;


  Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;


  fscanf( fp, "%d", &nel ) ;
  if( nel != mesh->nel ) {
    char str[1024] ;
    sprintf( str, "%d", nel ) ;
    emessage( 120, __ABORT_ADVFW_LIB__, str ) ;
  }
  mesh->MagField = (double *)Malloc( sizeof(double), nel*dimension ) ;
  for( el=0,j=0 ; el<nel ; el++ ) {
    for( i=0 ; i<dimension ; i++,j++ ) {
      fscanf( fp, "%le", &(mesh->MagField[j]) ) ;
    }
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;
}

/********************************************************************/

void write_Inidata_file( char *fname, MESH mesh )
{
  /*+ Funciton: write Inidata file +*/
  int    i, j ;
  FILE   *fp ;

  int    el ;
  int    nel = mesh.nel ;
  int    dimension = mesh.dimension ;


  fp = Fopen( fname, "w" ) ;


  Pfprintf( stdout, "  Start writing to \"%s\"\n", fname ) ;


  fprintf( fp, "%d\n", nel ) ;
  for( el=0,j=0 ; el<nel ; el++ ) {
    for( i=0 ; i<dimension ; i++,j++ ) {
      fprintf( fp, " % le", mesh.MagField[j] ) ;
    }
    fprintf( fp, "\n" ) ;
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish writing to \"%s\"\n", fname ) ;
}

/********************************************************************/

void read_outdata_file_FEM( char *fname, int t_loop,
                            MESH *mesh, OPTIONS *options )
{
  /*+ Funciton: read outdata file for FEM +*/
  char   str[128] ;


  switch( options->field ) {
    /* magnetic analysis */
    case FIELD_ELECTROMAGNETIC :
      read_outdata_file_FEM_FW( fname, t_loop, mesh, options ) ;
      break ;

    default :
      sprintf( str, "%d in \"%s\"", options->field, fname ) ;
      emessage( 151, __ABORT_ADVFW_LIB__, str ) ;
  }
}

/********************************************************************/

void read_outdata_file_FEM_FW( char *fname, int t_loop,
                                MESH *mesh, OPTIONS *options )
{
  /*+ Funciton: read outdata file for FEM +*/
  int    i, j, k ;

  AdvDatabox  *dbox ;
  AdvDocument *doc ;
  adv_off_t   off ;

  char   label[1024] ;
  char   str[1024] ;
  int    size ;


  dbox = AdvDboxOpen( fname ) ;


    if( t_loop < 0 ) {
      Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;
    }


  if( t_loop <= 0 ) {
    read_outdata_file_options_FW( dbox, &t_loop, options ) ;
  }


  if( options->work == WORK_POST_HDDM_CALC ) {
    AdvDboxClose( dbox ) ;
    return ;
  }


  sprintf( label, "MagneticVectorPotential" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                       label, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
  }
  adv_dio_get_property_int32( doc, "num_items", &size ) ;
  off = 0 ;
  if( (options->analysis == ANALYSIS_FULL_WAVECURRENT)
      || (options->analysis == ANALYSIS_POST_FULL_WAVE) ) {
    if( mesh->x_c == NULL ) {
      mesh->x_c = (Complex *)Malloc( sizeof(Complex), size ) ;
      mesh->nmp = size ;
    } else {
      if( mesh->nmp != size ) {
        sprintf( str, "%d", size ) ;
        emessage( 123, __ABORT_ADVFW_LIB__, str ) ;
      }
    }
    for( i=0 ; i<size ; i++ ) {
      off += adv_dio_read_int32( doc, off, &j ) ;
      off += adv_dio_read_float64( doc, off, &(mesh->x_c[i].re) ) ;
      off += adv_dio_read_float64( doc, off, &(mesh->x_c[i].im) ) ;
    }
  }
  adv_dio_close( doc ) ;

  if( options->analysis == ANALYSIS_POST_FULL_WAVE ) {
    int    ii ;
    int    start = 1 ;
    int    end = 3 ;
    char   unit[128] ;
 
    for( ii=start ; ii<end ; ii++ ) {
      switch( ii ) {
        case 0 :
          sprintf( label, "MagneticField" ) ;
          break ;
        case 1 :
          sprintf( label, "MagneticFieldReal" ) ;
          break ;
        case 2 :
          sprintf( label, "MagneticFieldImaginary" ) ;
          break ;
      }
      if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                           label, NULL)) == NULL ) {
        emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
      }
      strcpy( unit, adv_dio_get_property(doc, "unit") ) ;
      if( !strcmp(unit, "T") ) {
        options->unit.MagneticField = UNIT_T ;
      } else if( !strcmp(unit, "G") ) {
        options->unit.MagneticField = UNIT_G ;
      } else if( !strcmp(unit, "A/m") ) {
        options->unit.MagneticField = UNIT_A_m ;
      } else if( !strcmp(unit, "A/mm") ) {
        options->unit.MagneticField = UNIT_A_mm ;
      } else {
        char   str[1024] ;
        sprintf( str, "%s for the unit of Magnetic Field", unit ) ;
        emessage( 114, __ABORT_ADVFW_LIB__, str ) ;
      }
      adv_dio_get_property_int32( doc, "num_items", &size ) ;
      off = 0 ;
      if( (mesh->nap == 0) && (size != 0) ) {
        mesh->nap = size ;
      } else {
        if( mesh->nap != size ) {
          sprintf( str, "%d", size ) ;
          emessage( 122, __ABORT_ADVFW_LIB__, str ) ;
        }
      }
      switch( ii ) {
        case 0 :
          if( mesh->rvalue.B_d == NULL ) {
            mesh->rvalue.B_d = (double *)Malloc( sizeof(double),
                                                 size*mesh->dimension ) ;
          }
          for( i=0,j=0 ; i<size ; i++ ) {
            off += adv_dio_read_int32( doc, off, &k ) ;
            off += adv_dio_read_float64( doc, off, &(mesh->rvalue.B_d[j++]) ) ;
            off += adv_dio_read_float64( doc, off, &(mesh->rvalue.B_d[j++]) ) ;
            off += adv_dio_read_float64( doc, off, &(mesh->rvalue.B_d[j++]) ) ;
          }
          break ;
      case 1 :
          if( mesh->rvalue.B_c == NULL ) {
            mesh->rvalue.B_c = (Complex *)Malloc( sizeof(Complex),
                                                  size*mesh->dimension ) ;
          }
          for( i=0,j=0 ; i<size ; i++ ) {
            off += adv_dio_read_int32( doc, off, &k ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.B_c[j++].re) ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.B_c[j++].re) ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.B_c[j++].re) ) ;
          }
          
      
          if( mesh->rvalue.E_c == NULL ) {
            mesh->rvalue.E_c = (Complex *)Malloc( sizeof(Complex),
                                                  size*mesh->dimension ) ;
          }
          for( i=0,j=0 ; i<size ; i++ ) {
            off += adv_dio_read_int32( doc, off, &k ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.E_c[j++].im) ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.E_c[j++].im) ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.E_c[j++].im) ) ;
          }
          break ;
      }
      adv_dio_close( doc ) ;

      switch( ii ) {
        case 0 :
          sprintf( label, "ElectricField" ) ;
          break ;
        case 1 :
          sprintf( label, "ElectricFieldReal" ) ;
          break ;
        case 2 :
          sprintf( label, "ElectricFieldImaginary" ) ;
          break ;
      }
      if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                           label, NULL)) == NULL ) {
        emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
      }
      strcpy( unit, adv_dio_get_property(doc, "unit") ) ;
      if( !strcmp(unit, "A/m^2") ) {
        options->unit.MagneticField = UNIT_A_m2 ;
      } else if( !strcmp(unit, "A/mm^2") ) {
        options->unit.MagneticField = UNIT_A_mm2 ;
      } else {
        char   str[1024] ;
        sprintf( str, "%s for the unit of Eddy Current Density", unit ) ;
        emessage( 114, __ABORT_ADVFW_LIB__, str ) ;
      }
      adv_dio_get_property_int32( doc, "num_items", &size ) ;
      off = 0 ;
      if( mesh->nap != size ) {
        sprintf( str, "%d", size ) ;
        emessage( 122, __ABORT_ADVFW_LIB__, str ) ;
      }
      switch( ii ) {
        case 0 :
          if( mesh->rvalue.Je_d == NULL ) {
            mesh->rvalue.Je_d = (double *)Malloc( sizeof(double),
                                                 size*mesh->dimension ) ;
          }
          for( i=0,j=0 ; i<size ; i++ ) {
            off += adv_dio_read_int32( doc, off, &k ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.Je_d[j++]) ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.Je_d[j++]) ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.Je_d[j++]) ) ;
          }
          break ;
        case 1 :
          if( mesh->rvalue.Je_c == NULL ) {
            mesh->rvalue.Je_c = (Complex *)Malloc( sizeof(Complex),
                                                  size*mesh->dimension ) ;
          }
          for( i=0,j=0 ; i<size ; i++ ) {
            off += adv_dio_read_int32( doc, off, &k ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.Je_c[j++].re) ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.Je_c[j++].re) ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.Je_c[j++].re) ) ;
          }
          break ;
        case 2 :
	  if( mesh->rvalue.Je_c == NULL ) {
            mesh->rvalue.Je_c = (Complex *)Malloc( sizeof(Complex),
                                                  size*mesh->dimension ) ;
          }
          for( i=0,j=0 ; i<size ; i++ ) {
            off += adv_dio_read_int32( doc, off, &k ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.Je_c[j++].im) ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.Je_c[j++].im) ) ;
            off += adv_dio_read_float64( doc, off,
                                         &(mesh->rvalue.Je_c[j++].im) ) ;
          }
          break ;
      }
      adv_dio_close( doc ) ;
    }
  }

  AdvDboxClose( dbox ) ;


  if( t_loop < 0 ) {
    Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;
  }
}

/********************************************************************/

void read_outdata_file_HDDM_temp( char *fname, int t_loop,
                                  PartMesh *pm, OPTIONS *options )
{
  /*+ Funciton: read outdata file for HDDM +*/
  char   str[128] ;

  switch( options->field ) {
    /* magnetic analysis */
    case FIELD_ELECTROMAGNETIC :
      read_outdata_file_HDDM_FW_temp( fname, t_loop, pm, options ) ;
      break ;
    default :
      sprintf( str, "%d in \"%s\"", options->field, fname ) ;
      emessage( 151, __ABORT_ADVFW_LIB__, str ) ;
  }
}

/********************************************************************/

void read_outdata_file_HDDM_FW_temp( char *fname, int t_loop,
                                      PartMesh *pm, OPTIONS *options )
{
 
  int    i, j ;

  AdvDatabox  *dbox ;
  AdvDocument *doc ;
  adv_off_t   off ;

  char   label[1024] ;
  char   str[1024] ;
  int    size ;

  int    dom ;
  int    ndomain = pm->ndomain ;


  dbox = AdvDboxOpen( fname ) ;


  if( t_loop < 0 ) {
    Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;
  }


  if( t_loop == -2 ) {
    read_outdata_file_options_FW( dbox, &t_loop, options ) ;
    AdvDboxClose( dbox ) ;
    return ;
  }


  sprintf( label, "MagneticVectorPotential" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                       label, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
  }
  off = 0 ;
  if( (options->analysis == ANALYSIS_FULL_WAVECURRENT)
      || (options->analysis == ANALYSIS_POST_FULL_WAVE) ) {
    for( dom=0 ; dom<ndomain ; dom++ ) {
      DomMesh* dm = &(pm->dm[dom]) ;
      off += adv_dio_read_int32( doc, off, &size ) ;
      if( dm->x_c == NULL ) {
        dm->x_c = (Complex *)Malloc( sizeof(Complex), size ) ;
        dm->nmp = size ;
      } else {
        if( dm->nmp != size ) {
          sprintf( str, "%d", size ) ;
          emessage( 123, __ABORT_ADVFW_LIB__, str ) ;
        }
      }
      for( i=0 ; i<size ; i++ ) {
        off += adv_dio_read_int32( doc, off, &j ) ;
        off += adv_dio_read_float64( doc, off, &(dm->x_c[i].re) ) ;
        off += adv_dio_read_float64( doc, off, &(dm->x_c[i].im) ) ;
      }
    }
  } 
  adv_dio_close( doc ) ;

  if( options->analysis == ANALYSIS_POST_FULL_WAVE ) {
    int    ii ;
    int    start = 1 ;
    int    end = 3 ;
    char   unit[128] ;
       
    for( ii=start ; ii<end ; ii++ ) {
      switch( ii ) {
        case 0 :
          sprintf( label, "MagneticField" ) ;
          break ;
        case 1 :
          sprintf( label, "MagneticFieldReal" ) ;
          break ;
        case 2 :
          sprintf( label, "MagneticFieldImaginary" ) ;
          break ;
      }
      if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                           label, NULL)) == NULL ) {
        emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
      }
      strcpy( unit, adv_dio_get_property(doc, "unit") ) ;
      if( !strcmp(unit, "T") ) {
        options->unit.MagneticField = UNIT_T ;
      } else if( !strcmp(unit, "G") ) {
        options->unit.MagneticField = UNIT_G ;
      } else if( !strcmp(unit, "A/m") ) {
        options->unit.MagneticField = UNIT_A_m ;
      } else if( !strcmp(unit, "A/mm") ) {
        options->unit.MagneticField = UNIT_A_mm ;
      } else {
        char   str[1024] ;
        sprintf( str, "%s for the unit of Magnetic Field", unit ) ;
        emessage( 114, __ABORT_ADVFW_LIB__, str ) ;
      }
      off = 0 ;
      //printf("switch flag=%d\n",ii);

      switch( ii ) {
        case 0 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm->dm[dom]) ;
            off += adv_dio_read_int32( doc, off, &size ) ;
            if( (dm->nel == 0) && (size != 0) ) {
              dm->nel = size ;
            } else {
              if( dm->nel != size ) {
                sprintf( str, "%d", size ) ;
                emessage( 122, __ABORT_ADVFW_LIB__, str ) ;
              }
            }
            if( dm->rvalue.el_B_d == NULL ) {
              dm->rvalue.el_B_d = (double *)Malloc( sizeof(double),
                                                    size*dm->dimension ) ;
            }
            off += adv_dio_read_float64v( doc, off, size*dm->dimension,
                                          dm->rvalue.el_B_d ) ;
          }
          break ;
        case 1 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm->dm[dom]) ;
            off += adv_dio_read_int32( doc, off, &size ) ;
            if( (dm->nel == 0) && (size != 0) ) {
              dm->nel = size ;
            } else {
              if( dm->nel != size ) {
                sprintf( str, "%d", size ) ;
                emessage( 122, __ABORT_ADVFW_LIB__, str ) ;
              }
            }

            if( dm->rvalue.el_B_c == NULL ) {
              dm->rvalue.el_B_c = (Complex *)Malloc( sizeof(Complex),
                                                     size*dm->dimension ) ;
            }
            for( i=0,j=0 ; i<size ; i++ ) {
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_B_c[j++].re) ) ;
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_B_c[j++].re) ) ;
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_B_c[j++].re) ) ;
            }
	  }
	  break ;
      case 2 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm->dm[dom]) ;
            off += adv_dio_read_int32( doc, off, &size ) ;
            if( (dm->nel == 0) && (size != 0) ) {
              dm->nel = size ;
            } else {
              if( dm->nel != size ) {
                sprintf( str, "%d", size ) ;
                emessage( 122, __ABORT_ADVFW_LIB__, str ) ;
              }
            }

            if( dm->rvalue.el_B_c == NULL ) {
              dm->rvalue.el_B_c = (Complex *)Malloc( sizeof(Complex),
                                                     size*dm->dimension ) ;
            }
            for( i=0,j=0 ; i<size ; i++ ) {
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_B_c[j++].im) ) ;
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_B_c[j++].im) ) ;
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_B_c[j++].im) ) ;
            }
	  }
	  break ;
      }
      adv_dio_close( doc ) ;

      switch( ii ) {
        case 0 :
          sprintf( label, "ElectricField" ) ;
          break ;
        case 1 :
          sprintf( label, "ElectricFieldReal" ) ;
          break ;
        case 2 :
          sprintf( label, "ElectricFieldImaginary" ) ;
          break ;
      }
      if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                           label, NULL)) == NULL ) {
        emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
      }

      off = 0 ;
      //printf("switch flag=%d\n",ii);
      switch( ii ) {
        case 0 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm->dm[dom]) ;
            off += adv_dio_read_int32( doc, off, &size ) ;
            if( (dm->nel == 0) && (size != 0) ) {
              dm->nel = size ;
            } else {
              if( dm->nel != size ) {
                sprintf( str, "%d", size ) ;
                emessage( 122, __ABORT_ADVFW_LIB__, str ) ;
              }
            }
            if( dm->rvalue.el_E_d == NULL ) {
              dm->rvalue.el_E_d = (double *)Malloc( sizeof(double),
                                                    size*dm->dimension ) ;
            }
            off += adv_dio_read_float64v( doc, off, size*dm->dimension,
                                          dm->rvalue.el_E_d ) ;
          }
          break ;
        case 1 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm->dm[dom]) ;
            off += adv_dio_read_int32( doc, off, &size ) ;
            if( (dm->nel == 0) && (size != 0) ) {
              dm->nel = size ;
            } else {
              if( dm->nel != size ) {
                sprintf( str, "%d", size ) ;
                emessage( 122, __ABORT_ADVFW_LIB__, str ) ;
              }
            }

            if( dm->rvalue.el_E_c == NULL ) {
              dm->rvalue.el_E_c = (Complex *)Malloc( sizeof(Complex),
                                                     size*dm->dimension ) ;
            }
            for( i=0,j=0 ; i<size ; i++ ) {
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_E_c[j++].re) ) ;
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_E_c[j++].re) ) ;
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_E_c[j++].re) ) ;
            }
	  }
	  break ;
        case 2 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm->dm[dom]) ;
            off += adv_dio_read_int32( doc, off, &size ) ;
            if( (dm->nel == 0) && (size != 0) ) {
              dm->nel = size ;
            } else {
              if( dm->nel != size ) {
                sprintf( str, "%d", size ) ;
                emessage( 122, __ABORT_ADVFW_LIB__, str ) ;
              }
            }

            if( dm->rvalue.el_E_c == NULL ) {
              dm->rvalue.el_E_c = (Complex *)Malloc( sizeof(Complex),
                                                     size*dm->dimension ) ;
            }
            for( i=0,j=0 ; i<size ; i++ ) {
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_E_c[j++].im) ) ;
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_E_c[j++].im) ) ;
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_E_c[j++].im) ) ;
            }
	  }
          break ;
      }
      adv_dio_close( doc ) ;

      switch (ii) {
        case 0 :
          sprintf( label, "CurrentDensity" ) ;
          break ;
        case 1 :
          sprintf( label, "CurrentDensityReal" ) ;
          break ;
        case 2 :
          sprintf( label, "CurrentDensityImaginary" ) ;
          break ;
      }
      if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                           label, NULL)) == NULL ) {
        emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
      }
      strcpy( unit, adv_dio_get_property(doc, "unit") ) ;
      if( !strcmp(unit, "A/m^2") ) {
        options->unit.MagneticField = UNIT_A_m2 ;
      } else if( !strcmp(unit, "A/mm^2") ) {
        options->unit.MagneticField = UNIT_A_mm2 ;
      } else {
        char   str[1024] ;
        sprintf( str, "%s for the unit of Eddy Current Density", unit ) ;
        emessage( 114, __ABORT_ADVFW_LIB__, str ) ;
      }
      off = 0 ;
      //printf("switch flag=%d\n",ii);
      switch( ii ) {
        case 0 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm->dm[dom]) ;
            off += adv_dio_read_int32( doc, off, &size ) ;
            if( dm->nel != size ) {
              sprintf( str, "%d", size ) ;
              emessage( 122, __ABORT_ADVFW_LIB__, str ) ;
            }
            if( dm->rvalue.el_Je_d == NULL ) {
              dm->rvalue.el_Je_d = (double *)Malloc( sizeof(double),
                                                     size*dm->dimension ) ;
            }
            off += adv_dio_read_float64v( doc, off, size*dm->dimension,
                                          dm->rvalue.el_Je_d ) ;
          }
          break ;
        case 1 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm->dm[dom]) ;
            off += adv_dio_read_int32( doc, off, &size ) ;
            if( dm->nel != size ) {
              sprintf( str, "%d", size ) ;
              emessage( 122, __ABORT_ADVFW_LIB__, str ) ;
            }
            if( dm->rvalue.el_Je_c == NULL ) {
              dm->rvalue.el_Je_c = (Complex *)Malloc( sizeof(Complex),
                                                      size*dm->dimension ) ;
            }
            for( i=0,j=0 ; i<size ; i++ ) {
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_Je_c[j++].re) ) ;
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_Je_c[j++].re) ) ;
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_Je_c[j++].re) ) ;
            }
          }
          break ;
        case 2 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm->dm[dom]) ;
            off += adv_dio_read_int32( doc, off, &size ) ;
            if( dm->nel != size ) {
              sprintf( str, "%d", size ) ;
              emessage( 122, __ABORT_ADVFW_LIB__, str ) ;
            }
            if( dm->rvalue.el_Je_c == NULL ) {
              dm->rvalue.el_Je_c = (Complex *)Malloc( sizeof(Complex),
                                                      size*dm->dimension ) ;
            }
            for( i=0,j=0 ; i<size ; i++ ) {
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_Je_c[j++].im) ) ;
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_Je_c[j++].im) ) ;
              off += adv_dio_read_float64( doc, off,
                                           &(dm->rvalue.el_Je_c[j++].im) ) ;
            }
          }
          break ;
      }
      adv_dio_close( doc ) ;
    }
  }

  AdvDboxClose( dbox ) ;


  if( t_loop < 0 ) {
    Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;
  }
}

/********************************************************************/

void read_outdata_file_options_FW( AdvDatabox *dbox, int *t_loop,
                                    OPTIONS *options )
{
  /*+ Funciton: read outdata file +*/
  AdvDocument *doc ;
  //  adv_off_t   off ;

  char   label[1024] ;
  char   str[1024] ;


  sprintf( label, "Options" ) ;
  if( (doc = adv_dbox_find_by_property(dbox, NULL, "label",
                                       label, NULL)) == NULL ) {
    emessage( 110, __ABORT_ADVFW_LIB__, label ) ;
  }
  strcpy( label, adv_dio_get_property(doc, "Analysis") ) ;
  if( !strcmp(label, "Mag_Full_Wavecurrent") ) {
    options->analysis = ANALYSIS_POST_FULL_WAVE ;
  } else {
    sprintf( str, "%s", label ) ;
    emessage( 152, __ABORT_ADVFW_LIB__, str ) ;
  }
  strcpy( label, adv_dio_get_property(doc, "Formulation") ) ;

  if( !strcmp(label, "EMETHOD") ) {
    options->formulation = FORMULATION_FW_EMETHOD ;
  } else {
    sprintf( str, "%s", label ) ;
    emessage( 153, __ABORT_ADVFW_LIB__, str ) ;
  }
  strcpy( label, adv_dio_get_property(doc, "Work") ) ;
  if( !strcmp(label, "HDDM_ANALYSIS") ) {
    options->work = WORK_POST_HDDM_CALC ;
  } else if( !strcmp(label, "FEM_ANALYSIS") ) {
    options->work = WORK_POST_FEM_CALC ;
  } else {
    sprintf( str, "%s", label ) ;
    emessage( 150, __ABORT_ADVFW_LIB__, str ) ;
  }

  adv_dio_close( doc ) ;
}

/********************************************************************/

void write_outdata_file_FEM( char *fname, int t_loop,
                             MESH mesh, OPTIONS options )
{
  /*+ Funciton: write outdata file for FEM +*/
  char   str[128] ;


  switch( options.field ) {
    case FIELD_ELECTROMAGNETIC :
      write_outdata_file_FEM_FW( fname, t_loop, mesh, options ) ;
      break ;

    default :
      sprintf( str, "%d in \"%s\"", options.field, fname ) ;
      emessage( 151, __ABORT_ADVFW_LIB__, str ) ;
  }
}

/********************************************************************/

void write_outdata_file_FEM_FW( char *fname, int t_loop, MESH mesh,
                                 OPTIONS options )
{
  /*+ Funciton: write outdata file for Magnetic FEM Analysis +*/
  int    i, j ;

  AdvDocFile  *fp ;
  AdvDocument *doc ;
  adv_off_t   off ;

  int    nid = 0 ;
  char   id[100][1024] ;
  char   title[1024] ;
  char   label[1024] ;
  char   str[1024] ;


  fp = AdvFopen( fname, "c" ) ;


  if( t_loop < 0 ) {
    Pfprintf( stdout, "  Start writing to \"%s\"\n", fname ) ;
  }


  if( t_loop <= 0 ) {
    write_outdata_file_options_FW( fp, &nid, id[nid], options ) ;
  }


  if( (options.work == WORK_HDDM_ANALYSIS)
      || (options.work == WORK_POST_HDDM_CALC) ) {
    AdvFclose( fp ) ;
    return ;
  }


  sprintf( title, "FEGenericAttribute" ) ;
  sprintf( label, "MagneticVectorPotential" ) ;
  strcpy( id[nid], (char *)adv_dio_make_documentid(label) ) ;
  doc = adv_dio_create( fp, id[nid] ) ;
  nid++ ;
  adv_dio_set_property( doc, "content_type", title ) ;
  adv_dio_set_property_int32( doc, "num_items", mesh.nmp ) ;
  adv_dio_set_property( doc, "fega_type", "NodeVariable" ) ;
  adv_dio_set_property( doc, "label", label ) ;
  switch( options.analysis ) {
    /* magnetic analysis */
    case ANALYSIS_FULL_WAVECURRENT :
      adv_dio_set_property( doc, "format", "f8f8" ) ;
      break ;

    default :
      sprintf( str, "%d in \"%s\"", options.analysis, fname ) ;
      emessage( 152, __ABORT_ADVFW_LIB__, str ) ;
  }
  adv_dio_set_property_int32( doc, "index_byte", 4 ) ;
  off = 0 ;
  switch( options.analysis ) {
    case ANALYSIS_FULL_WAVECURRENT :
      for( i=0 ; i<mesh.nmp ; i++ ) {
        off += adv_dio_write_int32( doc, off, mesh.mpid2gnid[i] ) ;
        off += adv_dio_write_float64( doc, off, mesh.x_c[i].re ) ;
        off += adv_dio_write_float64( doc, off, mesh.x_c[i].im ) ;
      }
      break ;

    default :
      sprintf( str, "%d in \"%s\"", options.analysis, fname ) ;
      emessage( 152, __ABORT_ADVFW_LIB__, str ) ;
  }
  adv_dio_close( doc ) ;

  if( options.analysis == ANALYSIS_FULL_WAVECURRENT ) {
    int    ii ;
    int    start = 1 ;
    int    end = 3 ;

    for( ii=start ; ii<end ; ii++ ) {
      sprintf( title, "FEGenericAttribute" ) ;
      switch( ii ) {
        case 0 :
          sprintf( label, "MagneticField" ) ;
          break ;
        case 1 :
          sprintf( label, "MagneticFieldReal" ) ;
          break ;
        case 2 :
          sprintf( label, "MagneticFieldImaginary" ) ;
          break ;
      }
      strcpy( id[nid], (char *)adv_dio_make_documentid(label) ) ;
      doc = adv_dio_create( fp, id[nid] ) ;
      nid++ ;
      adv_dio_set_property( doc, "content_type", title ) ;
      adv_dio_set_property_int32( doc, "num_items", mesh.nap ) ;
      adv_dio_set_property( doc, "fega_type", "NodeVariable" ) ;
      adv_dio_set_property( doc, "label", label ) ;
      adv_dio_set_property( doc, "format", "f8f8f8" ) ;
      switch( options.unit.MagneticField ) {
        case UNIT_T :
          adv_dio_set_property( doc, "unit", UNIT_T_STRING ) ;
          break ;
        case UNIT_G :
          adv_dio_set_property( doc, "unit", UNIT_G_STRING ) ;
          break ;
        case UNIT_A_m :
          adv_dio_set_property( doc, "unit", UNIT_A_m_STRING ) ;
          break ;
        case UNIT_A_mm :
          adv_dio_set_property( doc, "unit", UNIT_A_mm_STRING ) ;
          break ;
      }
      adv_dio_set_property_int32( doc, "index_byte", 4 ) ;
      off = 0 ;
      switch( ii ) {
        case 0 :
          for( i=0,j=0 ; i<mesh.nap ; i++ ) {
            off += adv_dio_write_int32( doc, off, mesh.apid2gnid[i] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.B_d[j++] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.B_d[j++] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.B_d[j++] ) ;
          }
          break ;
      case 1 :
          for( i=0,j=0 ; i<mesh.nap ; i++ ) {
            off += adv_dio_write_int32( doc, off, mesh.apid2gnid[i] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.B_c[j++].re ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.B_c[j++].re ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.B_c[j++].re ) ;
          }
	  break ;
        case 2 :
          for( i=0,j=0 ; i<mesh.nap ; i++ ) {
            off += adv_dio_write_int32( doc, off, mesh.apid2gnid[i] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.B_c[j++].im ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.B_c[j++].im ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.B_c[j++].im ) ;
          }
	  break ;        
      }
      adv_dio_close( doc ) ;

      sprintf( title, "FEGenericAttribute" ) ;
      switch( ii ) {
        case 0 :
          sprintf( label, "ElectricField" ) ;
          break ;
        case 1 :
          sprintf( label, "ElectricFieldReal" ) ;
          break ;
        case 2 :
          sprintf( label, "ElectricFieldImaginary" ) ;
          break ;
      }
      strcpy( id[nid], (char *)adv_dio_make_documentid(label) ) ;
      doc = adv_dio_create( fp, id[nid] ) ;
      nid++ ;
      adv_dio_set_property( doc, "content_type", title ) ;
      adv_dio_set_property_int32( doc, "num_items", mesh.nap ) ;
      adv_dio_set_property( doc, "fega_type", "NodeVariable" ) ;
      adv_dio_set_property( doc, "label", label ) ;
      adv_dio_set_property( doc, "format", "f8f8f8" ) ;
      switch( options.unit.MagneticField ) {
        case UNIT_T :
          adv_dio_set_property( doc, "unit", UNIT_T_STRING ) ;
          break ;
        case UNIT_G :
          adv_dio_set_property( doc, "unit", UNIT_G_STRING ) ;
          break ;
        case UNIT_A_m :
          adv_dio_set_property( doc, "unit", UNIT_A_m_STRING ) ;
          break ;
        case UNIT_A_mm :
          adv_dio_set_property( doc, "unit", UNIT_A_mm_STRING ) ;
          break ;
      }
      adv_dio_set_property_int32( doc, "index_byte", 4 ) ;
      off = 0 ;
      switch( ii ) {
        case 0 :
          for( i=0,j=0 ; i<mesh.nap ; i++ ) {
            off += adv_dio_write_int32( doc, off, mesh.apid2gnid[i] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.E_d[j++] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.E_d[j++] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.E_d[j++] ) ;
          }
          break ;
      case 1 :
          for( i=0,j=0 ; i<mesh.nap ; i++ ) {
            off += adv_dio_write_int32( doc, off, mesh.apid2gnid[i] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.E_c[j++].re ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.E_c[j++].re ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.E_c[j++].re ) ;
          }
          break ;
        case 2 :
          for( i=0,j=0 ; i<mesh.nap ; i++ ) {
            off += adv_dio_write_int32( doc, off, mesh.apid2gnid[i] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.E_c[j++].im ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.E_c[j++].im ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.E_c[j++].im ) ;
          }
	  break ;
      }
      adv_dio_close( doc ) ;

      sprintf( title, "FEGenericAttribute" ) ;
      switch( ii ) {
        case 0 :
          sprintf( label, "CurrentDensity" ) ;
          break ;
        case 1 :
          sprintf( label, "CurrentDensityReal" ) ;
          break ;
        case 2 :
          sprintf( label, "CurrentDensityImaginary" ) ;
          break ;
      }
      strcpy( id[nid], (char *)adv_dio_make_documentid(label) ) ;
      doc = adv_dio_create( fp, id[nid] ) ;
      nid++ ;
      adv_dio_set_property( doc, "content_type", title ) ;
      adv_dio_set_property_int32( doc, "num_items", mesh.nap ) ;
      adv_dio_set_property( doc, "fega_type", "NodeVariable" ) ;
      adv_dio_set_property( doc, "label", label ) ;
      adv_dio_set_property( doc, "format", "f8f8f8" ) ;
      switch( options.unit.CurrentDensity ) {
        case UNIT_A_m2 :
          adv_dio_set_property( doc, "unit", UNIT_A_m2_STRING ) ;
          break ;
        case UNIT_A_mm2 :
          adv_dio_set_property( doc, "unit", UNIT_A_mm2_STRING ) ;
          break ;
      }
      adv_dio_set_property_int32( doc, "index_byte", 4 ) ;
      off = 0 ;
      switch( ii ) {
        case 0 :
          for( i=0,j=0 ; i<mesh.nap ; i++ ) {
            off += adv_dio_write_int32( doc, off, mesh.apid2gnid[i] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.Je_d[j++] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.Je_d[j++] ) ;
            off += adv_dio_write_float64( doc, off, mesh.rvalue.Je_d[j++] ) ;
          }
          break ;
        case 1 :
          for( i=0,j=0 ; i<mesh.nap ; i++ ) {
            off += adv_dio_write_int32( doc, off, mesh.apid2gnid[i] ) ;
            off += adv_dio_write_float64( doc, off,
                                          mesh.rvalue.Je_c[j++].re ) ;
            off += adv_dio_write_float64( doc, off,
                                          mesh.rvalue.Je_c[j++].re ) ;
            off += adv_dio_write_float64( doc, off,
                                          mesh.rvalue.Je_c[j++].re ) ;
          }
          break ;
        case 2 :
          for( i=0,j=0 ; i<mesh.nap ; i++ ) {
            off += adv_dio_write_int32( doc, off, mesh.apid2gnid[i] ) ;
            off += adv_dio_write_float64( doc, off,
                                          mesh.rvalue.Je_c[j++].im ) ;
            off += adv_dio_write_float64( doc, off,
                                          mesh.rvalue.Je_c[j++].im ) ;
            off += adv_dio_write_float64( doc, off,
                                          mesh.rvalue.Je_c[j++].im ) ;
          }
          break ;
      }
      adv_dio_close( doc ) ;
    }


  }

  AdvFclose( fp ) ;


  if( t_loop < 0 ) {
    Pfprintf( stdout, "  Finish writing to \"%s\"\n", fname ) ;
  }
}

/********************************************************************/

void write_outdata_file_HDDM( char *fname, int t_loop,
                              PartMesh pm, OPTIONS options )
{
  /*+ Funciton: write outdata file for HDDM +*/
  char   str[128] ;


  switch( options.field ) {
    case FIELD_ELECTROMAGNETIC :
      write_outdata_file_HDDM_FW( fname, t_loop, pm, options ) ;
      break ;

    default :
      sprintf( str, "%d in \"%s\"", options.field, fname ) ;
      emessage( 151, __ABORT_ADVFW_LIB__, str ) ;
  }
}

/********************************************************************/

void write_outdata_file_HDDM_FW( char *fname, int t_loop, PartMesh pm,
                                  OPTIONS options )
{
  /*+ Funciton: write outdata file for Magnetic HDDM Analysis +*/

  int    i, j ;

  AdvDocFile  *fp ;
  AdvDocument *doc ;
  adv_off_t   off ;

  int    nid = 0 ;
  char   id[100][1024] ;
  char   title[1024] ;
  char   label[1024] ;
  char   str[1024] ;

  int    dom ;
  int    ndomain = pm.ndomain ;


  fp = AdvFopen( fname, "c" ) ;


  if( t_loop == -2 ) {
    write_outdata_file_options_FW( fp, &nid, id[nid], options ) ;
    AdvFclose( fp ) ;
    return ;
  }


  sprintf( title, "HDDM_FEGenericAttribute" ) ;
  sprintf( label, "MagneticVectorPotential" ) ;
  strcpy( id[nid], (char *)adv_dio_make_documentid(label) ) ;
  doc = adv_dio_create( fp, id[nid] ) ;
  nid++ ;
  adv_dio_set_property( doc, "content_type", title ) ;
  adv_dio_set_property_int32( doc, "num_subdomains", ndomain ) ;
  adv_dio_set_property( doc, "fega_type", "NodeVariable" ) ;
  switch( options.analysis ) {
   case ANALYSIS_FULL_WAVECURRENT :
     adv_dio_set_property( doc, "format", "f8f8" ) ;
   break ;
    
   default :
      sprintf( str, "%d in \"%s\"", options.analysis, fname ) ;
      emessage( 152, __ABORT_ADVFW_LIB__, str ) ;
  }
  adv_dio_set_property( doc, "label", label ) ;
  adv_dio_set_property_int32( doc, "index_byte", 4 ) ;
  off = 0 ;
  switch( options.analysis ) {
    case ANALYSIS_FULL_WAVECURRENT :
      for( dom=0 ; dom<ndomain ; dom++ ) {
        DomMesh* dm = &(pm.dm[dom]) ;
        int*   mpid2snid = dm->mpid2snid ;
        Complex* x_c = dm->x_c ;
        off += adv_dio_write_int32( doc, off, dm->nmp ) ;
        for( i=0 ; i<dm->nmp ; i++ ) {
          off += adv_dio_write_int32( doc, off, mpid2snid[i] ) ;
          off += adv_dio_write_float64( doc, off, x_c[i].re ) ;
          off += adv_dio_write_float64( doc, off, x_c[i].im ) ;
        }
      }
      break ;

    default :
      sprintf( str, "%d in \"%s\"", options.analysis, fname ) ;
      emessage( 152, __ABORT_ADVFW_LIB__, str ) ;
  }
  adv_dio_close( doc ) ;

  if( options.analysis == ANALYSIS_FULL_WAVECURRENT ) {
    int    ii ;
    int    start = 1 ;
    int    end = 3 ;
    
    for( ii=start ; ii<end ; ii++ ) {
      sprintf( title, "HDDM_FEGenericAttribute" ) ;
      switch( ii ) {
        case 0 :
          sprintf( label, "MagneticField" ) ;
          break ;
        case 1 :
          sprintf( label, "MagneticFieldReal" ) ;
          break ;
        case 2 :
          sprintf( label, "MagneticFieldImaginary" ) ;
          break ;
      }
      strcpy( id[nid], (char *)adv_dio_make_documentid(label) ) ;
      doc = adv_dio_create( fp, id[nid] ) ;
      nid++ ;
      adv_dio_set_property( doc, "content_type", title ) ;
      adv_dio_set_property_int32( doc, "num_subdomains", ndomain ) ;
      adv_dio_set_property_int32( doc, "num_items_orig", pm.nel_all ) ;
      adv_dio_set_property( doc, "fega_type", "AllElementVariable" ) ;
      adv_dio_set_property( doc, "format", "f8f8f8" ) ;
      adv_dio_set_property( doc, "label", label ) ;
      switch( options.unit.MagneticField ) {
        case UNIT_T :
          adv_dio_set_property( doc, "unit", UNIT_T_STRING ) ;
          break ;
        case UNIT_G :
          adv_dio_set_property( doc, "unit", UNIT_G_STRING ) ;
          break ;
        case UNIT_A_m :
          adv_dio_set_property( doc, "unit", UNIT_A_m_STRING ) ;
          break ;
        case UNIT_A_mm :
          adv_dio_set_property( doc, "unit", UNIT_A_mm_STRING ) ;
          break ;
      }
      adv_dio_set_property_int32( doc, "index_byte", 4 ) ;
      off = 0 ;
      switch( ii ) {
        case 0 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm.dm[dom]) ;
            off += adv_dio_write_int32( doc, off, dm->nel ) ;
            off += adv_dio_write_float64v( doc, off, dm->nel*3,
                                           dm->rvalue.el_B_d ) ;
          }
          break ;
        case 1 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm.dm[dom]) ;
            Complex* B = dm->rvalue.el_B_c ;
            off += adv_dio_write_int32( doc, off, dm->nel ) ;
            for( i=0,j=0 ; i<dm->nel ; i++ ) {
              off += adv_dio_write_float64( doc, off, B[j++].re ) ;
              off += adv_dio_write_float64( doc, off, B[j++].re ) ;
              off += adv_dio_write_float64( doc, off, B[j++].re ) ;
            }
	  }
	  break ;
        case 2 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm.dm[dom]) ;
            Complex* B = dm->rvalue.el_B_c ;
            off += adv_dio_write_int32( doc, off, dm->nel ) ;
            for( i=0,j=0 ; i<dm->nel ; i++ ) {
              off += adv_dio_write_float64( doc, off, B[j++].im ) ;
              off += adv_dio_write_float64( doc, off, B[j++].im ) ;
              off += adv_dio_write_float64( doc, off, B[j++].im ) ;
            }
          }
	  break ;
      }
      adv_dio_close( doc ) ;

      sprintf( title, "HDDM_FEGenericAttribute" ) ;
      switch( ii ) {
        case 0 :
          sprintf( label, "ElectricField" ) ;
          break ;
        case 1 :
          sprintf( label, "ElectricFieldReal" ) ;
          break ;
        case 2 :
          sprintf( label, "ElectricFieldImaginary" ) ;
          break ;
      }
      strcpy( id[nid], (char *)adv_dio_make_documentid(label) ) ;
      doc = adv_dio_create( fp, id[nid] ) ;
      nid++ ;
      adv_dio_set_property( doc, "content_type", title ) ;
      adv_dio_set_property_int32( doc, "num_subdomains", ndomain ) ;
      adv_dio_set_property_int32( doc, "num_items_orig", pm.nel_all ) ;
      adv_dio_set_property( doc, "fega_type", "AllElementVariable" ) ;
      adv_dio_set_property( doc, "format", "f8f8f8" ) ;
      adv_dio_set_property( doc, "label", label ) ;
      switch( options.unit.MagneticField ) {
        case UNIT_T :
          adv_dio_set_property( doc, "unit", UNIT_T_STRING ) ;
          break ;
        case UNIT_G :
          adv_dio_set_property( doc, "unit", UNIT_G_STRING ) ;
          break ;
        case UNIT_A_m :
          adv_dio_set_property( doc, "unit", UNIT_A_m_STRING ) ;
          break ;
        case UNIT_A_mm :
          adv_dio_set_property( doc, "unit", UNIT_A_mm_STRING ) ;
          break ;
      }
      adv_dio_set_property_int32( doc, "index_byte", 4 ) ;
      off = 0 ;
      switch( ii ) {
        case 0 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm.dm[dom]) ;
            off += adv_dio_write_int32( doc, off, dm->nel ) ;
            off += adv_dio_write_float64v( doc, off, dm->nel*3,
                                           dm->rvalue.el_E_d ) ;
          }
          break ;
        case 1 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm.dm[dom]) ;
            Complex* E = dm->rvalue.el_E_c ;
            off += adv_dio_write_int32( doc, off, dm->nel ) ;
            for( i=0,j=0 ; i<dm->nel ; i++ ) {
              off += adv_dio_write_float64( doc, off, E[j++].re ) ;
              off += adv_dio_write_float64( doc, off, E[j++].re ) ;
              off += adv_dio_write_float64( doc, off, E[j++].re ) ;
            }
          }
          break ;
        case 2 :
         for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm.dm[dom]) ;
            Complex* E = dm->rvalue.el_E_c ;
            off += adv_dio_write_int32( doc, off, dm->nel ) ;
            for( i=0,j=0 ; i<dm->nel ; i++ ) {
              off += adv_dio_write_float64( doc, off, E[j++].im ) ;
              off += adv_dio_write_float64( doc, off, E[j++].im ) ;
              off += adv_dio_write_float64( doc, off, E[j++].im ) ;
            }
          }
	  break ;
      }
      adv_dio_close( doc ) ;

      sprintf( title, "HDDM_FEGenericAttribute" ) ;
      switch( ii ) {
        case 0 :
          sprintf( label, "CurrentDensity" ) ;
          break ;
        case 1 :
          sprintf( label, "CurrentDensityReal" ) ;
          break ;
        case 2 :
          sprintf( label, "CurrentDensityImaginary" ) ;
          break ;
      }
      strcpy( id[nid], (char *)adv_dio_make_documentid(label) ) ;
      doc = adv_dio_create( fp, id[nid] ) ;
      nid++ ;
      adv_dio_set_property( doc, "content_type", title ) ;
      adv_dio_set_property_int32( doc, "num_subdomains", ndomain ) ;
      adv_dio_set_property_int32( doc, "num_items_orig", pm.nel_all ) ;
      adv_dio_set_property( doc, "fega_type", "AllElementVariable" ) ;
      adv_dio_set_property( doc, "format", "f8f8f8" ) ;
      adv_dio_set_property( doc, "label", label ) ;
      switch( options.unit.CurrentDensity ) {
        case UNIT_A_m2 :
          adv_dio_set_property( doc, "unit", UNIT_A_m2_STRING ) ;
          break ;
        case UNIT_A_mm2 :
          adv_dio_set_property( doc, "unit", UNIT_A_mm2_STRING ) ;
          break ;
      }
      adv_dio_set_property_int32( doc, "index_byte", 4 ) ;
      off = 0 ;
      switch( ii ) {
        case 0 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm.dm[dom]) ;
            off += adv_dio_write_int32( doc, off, dm->nel ) ;
            off += adv_dio_write_float64v( doc, off, dm->nel*3,
                                           dm->rvalue.el_Je_d ) ;
          }
          break ;
        case 1 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm.dm[dom]) ;
            Complex* Je = dm->rvalue.el_Je_c ;
            off += adv_dio_write_int32( doc, off, dm->nel ) ;
            for( i=0,j=0 ; i<dm->nel ; i++ ) {
              off += adv_dio_write_float64( doc, off, Je[j++].re ) ;
              off += adv_dio_write_float64( doc, off, Je[j++].re ) ;
              off += adv_dio_write_float64( doc, off, Je[j++].re ) ;
            }
          }
          break ;
        case 2 :
          for( dom=0 ; dom<ndomain ; dom++ ) {
            DomMesh* dm = &(pm.dm[dom]) ;
            Complex* Je = dm->rvalue.el_Je_c ;
            off += adv_dio_write_int32( doc, off, dm->nel ) ;
            for( i=0,j=0 ; i<dm->nel ; i++ ) {
              off += adv_dio_write_float64( doc, off, Je[j++].im ) ;
              off += adv_dio_write_float64( doc, off, Je[j++].im ) ;
              off += adv_dio_write_float64( doc, off, Je[j++].im ) ;
            }
          }
          break ;
      }
      adv_dio_close( doc ) ;
    }
  }

  AdvFclose( fp ) ;
}

/********************************************************************/

void write_outdata_file_options_FW( AdvDocFile *fp, int *nid, char *id,
                                     OPTIONS options )
{
  /*+ Funciton: write outdata file for Magnetic Analysis +*/
  AdvDocument *doc ;
  //adv_off_t   off ;

  char   title[1024] ;
  char   label[1024] ;
  char   str[1024] ;


  sprintf( title, "FEGenericAttribute" ) ;
  sprintf( label, "Options" ) ;
  strcpy( id, (char *)adv_dio_make_documentid(label) ) ;
  doc = adv_dio_create( fp, id ) ;
  (*nid)++ ;
  adv_dio_set_property( doc, "content_type", title ) ;
  switch( options.analysis ) {
    /* magnetic analysis */
    default :
      adv_dio_set_property_int32( doc, "num_items", 0 ) ;
      adv_dio_set_property( doc, "format", "" ) ;
  }
  adv_dio_set_property( doc, "fega_type", "AllElementConstant" ) ;
  adv_dio_set_property( doc, "label", label ) ;
  adv_dio_set_property_int32( doc, "index_byte", 4 ) ;
  switch( options.analysis ) {
    /* magnetic analysis */
    case ANALYSIS_FULL_WAVECURRENT :
      adv_dio_set_property( doc, "Analysis", "Mag_Full_Wavecurrent" ) ;
      break ;

    default :
      sprintf( str, "%d", options.analysis ) ;
      emessage( 152, __ABORT_ADVFW_LIB__, str ) ;
  }
  switch( options.formulation ) {
    case FORMULATION_FW_EMETHOD :
     adv_dio_set_property( doc, "Formulation", "EMETHOD" ) ;
    break ;
    
  default :
    sprintf( str, "%d in", options.formulation ) ;
    emessage( 153, __ABORT_ADVFW_LIB__, str ) ;
  }
  if( (options.work == WORK_HDDM_ANALYSIS)
      || (options.work == WORK_POST_HDDM_CALC) ) {
    adv_dio_set_property( doc, "Work", "HDDM_ANALYSIS" ) ;
  } else {
    adv_dio_set_property( doc, "Work", "FEM_ANALYSIS" ) ;
  }
 
  adv_dio_close( doc ) ;
}

/********************************************************************/

void write_avsfile_file( char *fname,
                         int nel, int nnd, int ap_elem, int nd_elem,
                         char *elem_type, int dimension,
                         int *nop, double *crd, int *flag, double *x,
                         OPTIONS options )
{
  /*+ Funciton: write avsfile file +*/
  int    i, j ;
  FILE   *fp ;

  int    nd ;
  int    el ;


  fp = Fopen( fname, "w" ) ;


  Pfprintf( stdout, "  Start writing to \"%s\"\n", fname ) ;

	printf("write_avsfile_file\n");


  if( options.datasw.avsfile_micro == NO_NEED ) {
    /* AVS / Express format */
    fprintf( fp, "%d %d %d 0 0\n", nnd, nel, dimension ) ;
  } else {
    /* Micro AVS format */
    fprintf( fp, "1\n" ) ;
    fprintf( fp, "data\n" ) ;
    fprintf( fp, "step1\n" ) ;
    fprintf( fp, "%d %d\n", nnd, nel ) ;
  }


  for( nd=0,j=0 ; nd<nnd ; ) {
    fprintf( fp, "%d", ++nd ) ;
    for( i=0 ; i<dimension ; i++,j++ ) {
      fprintf( fp, " % le", crd[j] ) ;
    }
    fprintf( fp, "\n" ) ;
  }
  if( (!strcmp(elem_type, ELEM_TYPE_3DLT))
      || (!strcmp(elem_type, ELEM_TYPE_3DQT)) ) {
    for( el=0 ; el<nel ; ) {
      int    ii = el * nd_elem ;
      int    flg = flag[el] ;
      fprintf( fp, "%d %d tet", ++el, flg ) ;
      for( i=0 ; i<ap_elem ; i++,ii++ ) {
        fprintf( fp, " %d", nop[ii]+1 ) ;
      }
      fprintf( fp, "\n" ) ;
    }
  } else if( (!strcmp(elem_type, ELEM_TYPE_3DLH))
             || (!strcmp(elem_type, ELEM_TYPE_3DQH)) ) {
    for( el=0 ; el<nel ; ) {
      int    ii = el * nd_elem ;
      int    flg = flag[el] ;
      fprintf( fp, "%d %d hex", ++el, flg ) ;
      for( i=0 ; i<ap_elem ; i++,ii++ ) {
        fprintf( fp, " %d", nop[ii] ) ;
      }
      fprintf( fp, "\n" ) ;
    }
  }


  if( options.datasw.avsfile_micro == NO_NEED ) {
    /* AVS / Express format */
    fprintf( fp, "1 %d\n", dimension ) ;
    fprintf( fp, "V\n" ) ;
  } else {
    /* Micro AVS format */
    fprintf( fp, "%d 0\n", dimension ) ;
    fprintf( fp, "%d", dimension ) ;
    for( i=0 ; i<dimension ; i++ ) {
      fprintf( fp, " 1" ) ;
    }
    fprintf( fp, "\n" ) ;
    fprintf( fp, "x,\n" ) ;
    if( dimension > 1 ) {
      fprintf( fp, "y,\n" ) ;
      if( dimension > 2 ) {
        fprintf( fp, "z,\n" ) ;
      }
    }
  }


  for( nd=0,j=0 ; nd<nnd ;  ) {
    fprintf( fp, "%d", ++nd ) ;
    for( i=0 ; i<dimension ; i++,j++ ) {
      fprintf( fp, " % le", x[j] ) ;
    }
    fprintf( fp, "\n" ) ;
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish writing to \"%s\"\n", fname ) ;
}

/********************************************************************/

void write_vtkfile_file( char *fname,
                         int nel, int nnd, int ap_elem, int nd_elem,
                         char *elem_type, int dimension,
                         int *nop, double *crd, int *flag, double *x,
                         OPTIONS options )
{
  /*+ Funciton: write vtkfile file +*/
  int    i, j ;
  FILE   *fp ;

  int    nd ;
  int    el ;


  fp = Fopen( fname, "w" ) ;


  Pfprintf( stdout, "  Start writing to \"%s\"\n", fname ) ;


  printf("write_vtkfile_file\n");


  /*write_vtkfile*/
  fprintf( fp, "# vtk DataFile Version 2.0\n" );
  fprintf( fp, "vector\n" );
  fprintf( fp, "ASCII\n" );
  fprintf( fp, "DATASET UNSTRUCTURED_GRID\n" );
  fprintf( fp, "POINTS %d float\n", nnd );

  for( nd=0,j=0 ; nd<nnd ; nd++ ) {
    for( i=0 ; i<dimension ; i++,j++ ) {
      fprintf( fp, "%le ", crd[j] ) ;
    }
    fprintf( fp, "\n" ) ;
  }


  fprintf( fp, "CELLS %d %d\n", nel, nel*5 );
  
  
  if( (!strcmp(elem_type, ELEM_TYPE_3DLT))
      || (!strcmp(elem_type, ELEM_TYPE_3DQT)) ) {
    for( el=0 ; el<nel ; el++) {
      int    ii = el * nd_elem ;
      fprintf( fp, "4" ) ;
      for( i=0 ; i<ap_elem ; i++,ii++ ) {
	fprintf( fp, " %d", nop[ii] ) ;
      }
      fprintf( fp, "\n" ) ;
    }
  } else if( (!strcmp(elem_type, ELEM_TYPE_3DLH))
	     || (!strcmp(elem_type, ELEM_TYPE_3DQH)) ) {
    for( el=0 ; el<nel ; el++) {
      int    ii = el * nd_elem ;
      fprintf( fp, "4" ) ;
      for( i=0 ; i<ap_elem ; i++,i++ ) {
	fprintf( fp, " %d", nop[ii] ) ;
      }
      fprintf( fp, "\n" ) ;
    }
  }

  fprintf( fp, "CELL_TYPES %d\n", nel );
  for( i = 0 ; i < nel ; i++ ){
    fprintf( fp, "10\n" );
  }
  fprintf( fp, "POINT_DATA %d\n", nnd );
  fprintf( fp, "VECTORS point_vectors float \n" );

  for( nd=0,j=0 ; nd<nnd ; nd++ ) {
    for( i=0 ; i<dimension ; i++,j++ ) {
      fprintf( fp, "%le ", x[j] ) ;
    }
    fprintf( fp, "\n" ) ;
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish writing to \"%s\"\n", fname ) ;
}

/********************************************************************/

void read_mtrldat_file( char *fname, MTRL *mtrl, OPTIONS options )
{
  /*+ Funciton: read mtrldat file +*/
  int    i ;
  FILE   *fp ;

  char   title[1024] ;


  fp = Fopen( fname, "r" ) ;


  Pfprintf( stdout, "  Start reading from \"%s\"\n", fname ) ;


  fscanf( fp, "%s", title ) ;
  while( !feof(fp) ) {

    if( !strcmp(title, MTRL_PERMITTIVITY) ) {
      if( mtrl->nflag == 0 ) {
        fscanf( fp, "%d", &(mtrl->nflag) ) ;
        mtrl->flag_i = (int *)Malloc( sizeof(int), mtrl->nflag ) ;
        mtrl->flag_nu = (double *)Malloc( sizeof(double), mtrl->nflag ) ;
        for( i=0 ; i<mtrl->nflag ; i++ ) {
          fscanf( fp, "%d", &(mtrl->flag_i[i]) ) ;
          fscanf( fp, "%le",  &(mtrl->flag_nu[i]) ) ;
        }
        sort_id_bubble_sort( mtrl->nflag, mtrl->flag_i, mtrl->flag_nu, 1 ) ;
      } else {
        emessage( 112, __ABORT_ADVFW_LIB__, title ) ;
      }

    } else if( !strcmp(title, MTRL_ANTENNA) ) {
      if( mtrl->nantenna == 0 ) {
        fscanf( fp, "%d", &(mtrl->nantenna) ) ;
        mtrl->antenna_def =
                (MTRL_ANTENNA_DEFINITION *)Malloc( sizeof(MTRL_ANTENNA_DEFINITION),
                                                mtrl->nantenna ) ;
        for( i=0 ; i<mtrl->nantenna ; i++ ) {
          char   str[128] ;
          init_MTRL_ANTENNA_DEFINITION( &(mtrl->antenna_def[i]) ) ;
          fscanf( fp, "%d", &(mtrl->antenna_def[i].flag) ) ;
          fscanf( fp, "%s", str ) ;
          fscanf( fp, "%s", mtrl->antenna_def[i].fname ) ;
          if( !strcmp(str, MTRL_ANTENNA_READ_FROM_FILE) ) {
            mtrl->antenna_def[i].mode = MTRL_ANTENNA_RF_NUMBER ;
          } else if( !strcmp(str, MTRL_ANTENNA_MAKE_FROM_DEFINE) ) {
            char fname[FILENAME_MAX] ;
            mtrl->antenna_def[i].mode = MTRL_ANTENNA_MD_NUMBER ;
            make_Jodefine_filename( fname, options.datasw,
                                    mtrl->antenna_def[i].fname, 0 ) ;
            read_Jodefine_file( fname, &(mtrl->antenna_def[i]) ) ;
          } else {
            char tmp_str[256] ;
            sprintf( tmp_str, "%s for antenna", str ) ;
            emessage( 111, __ABORT_ADVFW_LIB__, tmp_str ) ;
          }
        }
      } else {
        emessage( 112, __ABORT_ADVFW_LIB__, title ) ;
      }

    } else if( !strcmp(title, MTRL_ANTENNAOMEGA) ) {
      if( mtrl->nantenna_omega == 0 ) {
        mtrl->nantenna_omega++ ;
        fscanf( fp, "%le", &(mtrl->antenna_omega) ) ;
      }

    } else if( !strcmp(title, MTRL_CONDUCTOR) ) {
      if( mtrl->nR == 0 ) {
        fscanf( fp, "%d", &(mtrl->nR) ) ;
        mtrl->R_i = (int *)Malloc( sizeof(int), mtrl->nR ) ;
        mtrl->R_sigma = (double *)Malloc( sizeof(double), mtrl->nR ) ;
        for( i=0 ; i<mtrl->nR ; i++ ) {
          fscanf( fp, "%d", &(mtrl->R_i[i]) ) ;
          fscanf( fp, "%le", &(mtrl->R_sigma[i]) ) ;
        }
      } else {
        emessage( 112, __ABORT_ADVFW_LIB__, title ) ;
      }

    } else {
      char str[2048] ;
      sprintf( str, "%s in %s", title, fname ) ;
      emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
    }

    fscanf( fp, "%s", title ) ;
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish reading from \"%s\"\n", fname ) ;
}

/********************************************************************/

void write_mtrldat_file( char *fname, MTRL mtrl )
{
  /*+ Funciton: write mtrldat file +*/
  int    i ;
  FILE   *fp ;


  fp = Fopen( fname, "w" ) ;


  Pfprintf( stdout, "  Start writing to \"%s\"\n", fname ) ;


  fprintf( fp, "%d\n", mtrl.nflag ) ;
  for( i=0 ; i<mtrl.nflag ; i++ ) {
    fprintf( fp, "%d %le\n", mtrl.flag_i[i], mtrl.flag_nu[i] ) ;
  }

  if( mtrl.nantenna != 0 ) {
    fprintf( fp, "\n%s %d\n", MTRL_ANTENNA, mtrl.nantenna ) ;
    for( i=0 ; i<mtrl.nantenna ; i++ ) {
      fprintf( fp, "%d ", mtrl.antenna_def[i].flag ) ;
      if( mtrl.antenna_def[i].mode == MTRL_ANTENNA_RF_NUMBER ) {
        fprintf( fp, "%s %s\n",
                 MTRL_ANTENNA_READ_FROM_FILE, mtrl.antenna_def[i].fname ) ;
      } else if( mtrl.antenna_def[i].mode == MTRL_ANTENNA_MD_NUMBER ) {
        fprintf( fp, "%s %s\n",
                 MTRL_ANTENNA_MAKE_FROM_DEFINE, mtrl.antenna_def[i].fname ) ;
      } else {
        char   str[128] ;
        sprintf( str, "%d for antenna", mtrl.antenna_def[i].mode ) ;
        emessage( 111, __ABORT_ADVFW_LIB__, str ) ;
      }
    }
  }

  if( mtrl.nantenna_omega != 0 ) {
    fprintf( fp, "\n%s\n", MTRL_ANTENNAOMEGA ) ;
    fprintf( fp, "%le\n", mtrl.antenna_omega ) ;
  }
  
  if( mtrl.nR != 0 ) {
    fprintf( fp, "\n%s %d\n", MTRL_CONDUCTOR, mtrl.nR ) ;
    for( i=0 ; i<mtrl.nR ; i++ ) {
      fprintf( fp, "%d %le\n", mtrl.R_i[i], mtrl.R_sigma[i] ) ;
    }
  }


  Fclose( fp ) ;


  Pfprintf( stdout, "  Finish writing to \"%s\"\n", fname ) ;
}
