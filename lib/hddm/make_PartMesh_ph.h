/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/


void __MAKE_PARTMESH_CHECK_FLAG_PH__( PartMesh pm, OPTIONS options )
{
  /*+ Function: check flag for PartMesh (p-mode & h-mode) +*/
  int    i, j ;

  int    part ;
  int    dom ;

  int    npart = COM_GID.nparent ;
  int    ndomain = pm.ndomain ;
  MTRL   mtrl = pm.mtrl ;

  int    nflag = 0 ;
  int*   flag_i ;


  /* check nflag & *flag_i */
  for( dom=0 ; dom<ndomain ; dom++ ) {
    int    nel = pm.dm[dom].nel ;
    int*   flag = pm.dm[dom].flag ;
    make_MESH_check_flag_1st( nel, flag, &nflag, &flag_i ) ;
  }
  if( COM_GID.rootid == COM_ID.myprocid ) {
    for( part=1 ; part<npart ; part++ ) {
      int    nflag_tmp ;
      int*   flag_i_tmp ;
      int    type = part ;
      int    compid = COM_GID.parid[part] ;

      COM_ReadInt( &nflag_tmp, 1, &compid, &type ) ;
      flag_i_tmp = (int *)Malloc( sizeof(int), nflag_tmp ) ;
      COM_ReadInt( flag_i_tmp, nflag_tmp, &compid, &type ) ;
      for( i=0 ; i<nflag_tmp ; i++ ) {
        int    ii = 0 ;
        for( j=0 ; j<nflag ; j++ ) {
          if( flag_i[j] == flag_i_tmp[i] ) {
            ii++ ;
            break ;
          }
        }
        if( ii == 0 ) {
          flag_i = (int *)Realloc( flag_i, sizeof(int), nflag, nflag+1 ) ;
          flag_i[nflag] = flag_i_tmp[i] ;
          nflag++ ;
        }
      }
      Free( sizeof(int)*nflag_tmp, flag_i_tmp ) ;
      COM_Grp_Synchronize() ;
    }
    sort_i_bubble_sort( nflag, flag_i ) ;
    COM_Grp_Bcast_Int( &nflag, 1, COM_GID.rootid ) ;
    COM_Grp_Bcast_Int( flag_i, nflag, COM_GID.rootid ) ;
  } else {
    for( part=1 ; part<npart ; part++ ) {
      if( COM_ID.myprocid == part ) {
        int    type = part ;
        int    compid = COM_GID.rootid ;

        COM_WriteInt( &nflag, 1, compid, type ) ;
        COM_WriteInt( flag_i, nflag, compid, type ) ;
        Free( sizeof(int)*nflag, flag_i ) ;
      }
      COM_Grp_Synchronize() ;
    }
    COM_Grp_Bcast_Int( &nflag, 1, COM_GID.rootid ) ;
    flag_i = (int *)Malloc( sizeof(int), nflag ) ;
    COM_Grp_Bcast_Int( flag_i, nflag, COM_GID.rootid ) ;
  }
  make_MESH_check_flag_2nd( nflag, flag_i, mtrl, options ) ;
  Free( sizeof(int)*nflag, flag_i ) ;
}

/********************************************************************/

void __MAKE_PARTMESH_CHANGE_INNERBC_CHANGE_PINBC_2ND_PH__( PartMesh pm,
                                                           int tfree,
                                                           OPinfo *op,
                                                           Pinbc *pinbc )
{
  /*+ Function: 2nd step of changing Pinbc (p-mode & h-mode) +*/
  int    i ;

  int    part ;
  int    partid = pm.partid ;
  int    npart = COM_GID.nparent ;

  int    start_id = 1 - partid ;
  int    end_id = 2 * npart - partid -2 ;


  /* trade data within part */
  make_PartMesh_change_innerbc_change_Pinbc_2nd_sub( pm, tfree, op,
                                                     pinbc ) ;


  for( part=start_id ; part<end_id ; part++ ) {
    COM_Grp_Synchronize() ;

    if( (-1 < part) && (part < partid) ) {
      int    size = op[part].n_mofree ;
      if( size != 0 ) {
        int    type = 1000 + partid ;
        int    compid = COM_GID.parid[part] ;
        int*   ibid = op[part].ibid ;
        int*   nf_send = (int *)Malloc( sizeof(int), size ) ;
        int*   nf_recv = (int *)Malloc( sizeof(int), size ) ;
        for( i=0 ; i<size ; i++ ) {
          nf_send[i] = pinbc[ibid[i]].nf ;
        }
        COM_WriteInt( nf_send, size, compid, type ) ;
        COM_ReadInt( nf_recv, size, &compid, &type ) ;
        for( i=0 ; i<size ; i++ ) {
          int    ii = ibid[i] ;
          pinbc[ii].yng_part[pinbc[ii].yng_npart] = part ;
          pinbc[ii].yng_nf_part[pinbc[ii].yng_npart] = nf_recv[i] ;
          pinbc[ii].yng_npart++ ;
        }
        Free( sizeof(int)*size, nf_send ) ;
        Free( sizeof(int)*size, nf_recv ) ;
      }
    } else if( (partid < part) && (part < npart) ) {
      int    size = op[part].n_mofree ;
      if( size != 0 ) {
        int    type = 1000 + part ;
        int    compid = COM_GID.parid[part] ;
        int*   ibid = op[part].ibid ;
        int*   nf_send = (int *)Malloc( sizeof(int), size ) ;
        int*   nf_recv = (int *)Malloc( sizeof(int), size ) ;
        for( i=0 ; i<size ; i++ ) {
          int    ii = ibid[i] ;
          nf_send[i] = pinbc[ii].nf ;
        }
        COM_ReadInt( nf_recv, size, &compid, &type ) ;
        COM_WriteInt( nf_send, size, compid, type ) ;
        for( i=0 ; i<size ; i++ ) {
          int    ii = ibid[i] ;
          pinbc[ii].old_part[pinbc[ii].old_npart] = part ;
          pinbc[ii].old_nf_part[pinbc[ii].old_npart] = nf_recv[i] ;
          pinbc[ii].old_npart++ ;
        }
        Free( sizeof(int)*size, nf_send ) ;
        Free( sizeof(int)*size, nf_recv ) ;
      }
    }
  }
}

/********************************************************************/

void __MAKE_PARTMESH_CHANGE_INNERBC_CHANGE_PINBC_5TH_PH__( PartMesh pm,
                                                           OPinfo *op,
                                                           Pinbc *pinbc )
{
  /*+ Function: 5th step of changing Pinbc (p-mode & h-mode) +*/
  /*+           5th step of change local interface B.C. number
                                                     in charge part +*/
  int    i ;

  int    part ;
  int    partid = pm.partid ;
  int    npart = COM_GID.nparent ;

  int    start_id = 1 - partid ;
  int    end_id = 2 * npart - partid -2 ;


  for( part=start_id ; part<end_id ; part++ ) {
    COM_Grp_Synchronize() ;

    if( (-1 < part) && (part < partid) ) {
      int    size = op[part].n_mofree ;
      if( size != 0 ) {
        int    type = 2000 + partid ;
        int    compid = COM_GID.parid[part] ;
        int*   ibid = op[part].ibid ;
        int*   ln_chpar = (int *)Malloc( sizeof(int), size ) ;
        COM_ReadInt( ln_chpar, size, &compid, &type ) ;
        for( i=0 ; i<size ; i++ ) {
          int    ii = ibid[i] ;
          if( pinbc[ii].charge_part == part ) {
            pinbc[ii].ln_chpar = ln_chpar[i] ;
          }
        }
        Free( sizeof(int)*size, ln_chpar ) ;
      }
    } else if( (partid < part) && (part < npart) ) {
      int    size = op[part].n_mofree ;
      if( size != 0 ) {
        int    type = 2000 + part ;
        int    compid = COM_GID.parid[part] ;
        int*   ibid = op[part].ibid ;
        int*   ln_chpar = (int *)Malloc( sizeof(int), size ) ;
        for( i=0 ; i<size ; i++ ) {
          int    ii = ibid[i] ;
          ln_chpar[i] = pinbc[ii].ln_part ;
        }
        COM_WriteInt( ln_chpar, size, compid, type ) ;
        Free( sizeof(int)*size, ln_chpar ) ;
      }
    }
  }
}
