/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "comm_mpi.h"


COM_WLD_ID COM_ID;
COM_GRP_ID COM_GID;


#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
static MPI_Datatype COM_Dtype_Complex ;
static MPI_Comm  COM_GroupID_me ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */


int COM_Init(int * pargc, char *** pargv)
{
  /*+  Function:  Initialize communication  +*/
  int nproc, i ;

#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  int ret, myprocid ;

  ret = MPI_Init(pargc,  pargv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &myprocid) ;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc) ;
  COM_Def_Struct_Complex() ;
  COM_ID.allid = (int *)malloc( sizeof(int)*nproc ) ;
  /*   MPI's rank is 0 to nproc */
  for (i = 0; i< nproc; i++) {
    COM_ID.allid[i] = i ;
  }
  COM_GID.rootid = COM_ID.allid[0] ;
  COM_ID.nproc = nproc ;
  COM_ID.myprocid = myprocid ;
  return ret ;
#else /* s-mode */
  nproc = 1 ;
  COM_ID.allid = (int *)malloc( sizeof(int)*nproc ) ;
  /*   MPI's rank is 0 to nproc */
  for (i = 0; i< nproc; i++) {
    COM_ID.allid[i] = i ;
  }
  COM_GID.rootid = COM_ID.allid[0] ;
  COM_ID.nproc = nproc ;
  COM_ID.myprocid = 0 ;
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Finalize( void )
{
  /*+  Function:  Finalize communication  +*/
  free( COM_ID.allid ) ;

#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  MPI_Finalize() ;
#endif /* __ADVFW_SINGLE_MODE__ */

  return 0 ;
}

/***********************************************************************/

int COM_Def_Struct_Complex( void )
{
  /*+  Function:  Define sturct Complex for communication  +*/
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  int    count = 2 ;
  int    blengths[2] ;
  Complex c ;

  MPI_Aint displacements[2] ;
  MPI_Datatype datatypes[2] ;


  blengths[0] = 1 ;
  displacements[0] = (MPI_Aint)(&c.re) - (MPI_Aint)(&c) ;
  datatypes[0] = MPI_DOUBLE ;


  blengths[1] = 1 ;
  displacements[1] = (MPI_Aint)(&c.im) - (MPI_Aint)(&c) ;
  datatypes[1] = MPI_DOUBLE ;


  MPI_Type_struct( count, blengths, displacements,
		   datatypes, &COM_Dtype_Complex ) ;


  return( MPI_Type_commit(&COM_Dtype_Complex) ) ;
#else /* s-mode */
  return 0 ;
#endif /* __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

void COM_SetGroup( int nparent )
{

  int procid, nproc ; (void)procid; (void)nproc;
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  int nchild ; (void)nchild;
#endif /* __ADVFW_SINGLE_MODE__ */
  
  procid = COM_ID.myprocid ;
  nproc = COM_ID.nproc ;
  
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  nchild = nproc - nparent ;
  #ifndef __ADVFW_PARALLEL_MODE__ /* h-mode */
  if ( nchild <= 0 ) {
    emessage( 20, __ABORT_ADVFW_LIB__, NULL ) ;
  }

  COM_GID.nparent = nparent ;
  COM_GID.nchild = nchild ;
  #else /* p-mode */
  if ( nchild != 0 ) {
    emessage( 21, __ABORT_ADVFW_LIB__, NULL ) ;
  }
  
  COM_GID.nparent = nparent ;
  COM_GID.nchild = 0 ;
  #endif /* #ifndef __ADVFW_PARALLEL_MODE__ */
#else /* s-mode */
  COM_GID.nparent = nparent ;
  COM_GID.nchild  = 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */

  COM_GID.parid  =  COM_ID.allid ;
  COM_GID.chiid  =  COM_ID.allid + nparent ;

#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  if ( COM_ID.myprocid < nparent ) {
    COM_GID.group = GRP_PARENT ;
    COM_GID.mygprocid = procid ;
    MPI_Comm_split(MPI_COMM_WORLD, 0, procid, &COM_GroupID_me) ;

  } else {
    COM_GID.group = GRP_CHILD ;
    COM_GID.mygprocid = procid - nparent ;
    MPI_Comm_split(MPI_COMM_WORLD, 1, procid, &COM_GroupID_me) ;
  }
#else /* s-mode */
  COM_GID.group = GRP_PARENT ;
  COM_GID.mygprocid = COM_ID.myprocid ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Bcast_Chr( char *buf, int count, int root )
{
  /*+  Function:  Broadcast char to all +*/
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "BcastChr : root %d, count %d\n",
            root, count ) ;
#endif
  return( MPI_Bcast(buf, count, MPI_CHAR, root, MPI_COMM_WORLD) ) ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Bcast_Int( int *buf, int count, int root )
{
  /*+  Function:  Broadcast int to all +*/
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "BcastInt : root %d, count %d\n",
            root, count ) ;
#endif
  return( MPI_Bcast(buf, count, MPI_INT, root, MPI_COMM_WORLD) ) ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Bcast_Dbl( double *buf, int count, int root )
{
  /*+  Function:  Broadcast double to all +*/
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "BcastDbl : root %d, count %d\n",
            root, count ) ;
#endif
  return( MPI_Bcast(buf, count, MPI_DOUBLE, root, MPI_COMM_WORLD) ) ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Bcast_Cpl( Complex *buf, int count, int root )
{
  /*+  Function:  Broadcast Complex to all +*/
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "BcastCpl : root %d, count %d\n",
            root, count ) ;
#endif
  return( MPI_Bcast(buf, count, COM_Dtype_Complex, root, MPI_COMM_WORLD) ) ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Grp_Bcast_Chr( char *buf, int count, int root )
{
  /*+  Function:  Broadcast char to all +*/
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "GrpBcastChr : root %d, count %d\n",
            root, count ) ;
#endif
  return( MPI_Bcast(buf, count, MPI_CHAR, root, COM_GroupID_me) ) ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Grp_Bcast_Int( int *buf, int count, int root )
{
  /*+  Function:  Broadcast int to all +*/
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "GrpBcastInt : root %d, count %d\n",
            root, count ) ;
#endif
  return( MPI_Bcast(buf, count, MPI_INT, root, COM_GroupID_me) ) ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Grp_Bcast_Dbl( double *buf, int count, int root )
{
  /*+  Function:  Broadcast double to all +*/
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "GrpBcastDbl : root %d, count %d\n",
            root, count ) ;
#endif
  return( MPI_Bcast(buf, count, MPI_DOUBLE, root, COM_GroupID_me) ) ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Grp_Bcast_Cpl( Complex *buf, int count, int root )
{
  /*+  Function:  Broadcast Complex to all +*/
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "GrpBcastCpl : root %d, count %d\n",
            root, count ) ;
#endif
  return( MPI_Bcast(buf, count, COM_Dtype_Complex, root, COM_GroupID_me) ) ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int  COM_Synchronize(void)
{
  /*+  Function:  Synchronize all processes +*/
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "Syncronize\n" ) ;
#endif
  return( MPI_Barrier(MPI_COMM_WORLD) ) ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int  COM_Grp_Synchronize(void)
{
  /*+  Function:  Synchronize processes in own group +*/
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "GrpSyncronize\n" ) ;
#endif
  return( MPI_Barrier(COM_GroupID_me) ) ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

void  COM_Exit(int status)
{
  /*+  Function:  Exit +*/
  COM_Finalize() ;
  exit( status ) ;
}

/***********************************************************************/

int COM_ReadChr( char *buf, int size, int *node, int *type )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  MPI_Status status ;
  int rv ;

#if ( monit >= 2 )
  fprintf( stderr, "(t%x) wait     type=%5d from t%x\n",
           COM_ID.myprocid, *type, *node ) ;
#endif
  if ( *node == -1 ) *node = MPI_ANY_SOURCE ;
  if ( *type == -1 ) *type = MPI_ANY_TAG ;

  rv = MPI_Recv( buf, size, MPI_CHAR, *node, *type,
		MPI_COMM_WORLD, &status ) ;

  if ( rv ) {
    fprintf( stderr, "(%x) COM_ReadChr - error#%d from %x %d\n",
             COM_ID.myprocid, rv, *node, *type ) ;
    pause() ;
  }

  *node = status.MPI_SOURCE ;
  *type = status.MPI_TAG ;

#if ( monit >= 2 )
  fprintf( stderr, "(t%x) read     type=%5d size=%6d from t%x\n",
           COM_ID.myprocid, *type, size, *node ) ;
#endif

#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "ReadChr : node %d, type %d size %d\n",
            *node, *type, size ) ;
#endif

  return rv ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_ReadInt( int *buf, int size, int *node, int *type )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  MPI_Status status ;
  int rv ;

#if ( monit >= 2 )
  fprintf( stderr, "(t%x) wait     type=%5d from t%x\n",
           COM_ID.myprocid, *type, *node ) ;
#endif
  if ( *node == -1 ) *node = MPI_ANY_SOURCE ;
  if ( *type == -1 ) *type = MPI_ANY_TAG ;

  rv = MPI_Recv( buf, size, MPI_INT, *node, *type,
		MPI_COMM_WORLD, &status ) ;

  if ( rv ) {
    fprintf( stderr, "(%x) COM_ReadInt - error#%d from %x %d\n",
             COM_ID.myprocid, rv, *node, *type ) ;
    pause() ;
  }

  *node = status.MPI_SOURCE ;
  *type = status.MPI_TAG ;

#if ( monit >= 2 )
  fprintf( stderr, "(t%x) read     type=%5d size=%6d from t%x\n",
           COM_ID.myprocid, *type, size, *node ) ;
#endif

#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "ReadInt : node %d, type %d size %d\n",
            *node, *type, size ) ;
#endif

  return rv ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_ReadDbl( double *buf, int size, int *node, int *type )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  MPI_Status status ;
  int rv ;

#if ( monit >= 2 )
  fprintf( stderr, "(t%x) wait_dbl type=%5d from t%x\n",
           COM_ID.myprocid, *type, *node ) ;
#endif
  if ( *node == -1 ) *node = MPI_ANY_SOURCE ;
  if ( *type == -1 ) *type = MPI_ANY_TAG ;

  rv = MPI_Recv( buf, size, MPI_DOUBLE, *node, *type,
                 MPI_COMM_WORLD, &status ) ;

  if ( rv ) {
    fprintf( stderr, "(%x) COM_ReadDbl - error#%d from %x %d\n",
             COM_ID.myprocid, rv, *node, *type ) ;
    pause() ;
  }

  *node = status.MPI_SOURCE ;
  *type = status.MPI_TAG ;

#if ( monit >= 2 )
  fprintf( stderr, "(t%x) COM_ReadDbl type=%5d size=%6d from t%x\n",
           COM_ID.myprocid, *type, size, *node ) ;
#endif

#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "ReadDbl : node %d, type %d size %d\n",
            *node, *type, size ) ;
#endif

  return rv ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_ReadCpl( Complex *buf, int size, int *node, int *type )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  MPI_Status status ;
  int rv ;

#if ( monit >= 2 )
  fprintf( stderr, "(t%x) wait_cpl type=%5d from t%x\n",
           COM_ID.myprocid, *type, *node ) ;
#endif
  if ( *node == -1 ) *node = MPI_ANY_SOURCE ;
  if ( *type == -1 ) *type = MPI_ANY_TAG ;

  rv = MPI_Recv( buf, size, COM_Dtype_Complex, *node, *type,
                 MPI_COMM_WORLD, &status ) ;

  if ( rv ) {
    fprintf( stderr, "(%x) COM_ReadCpl - error#%d from %x %d\n",
             COM_ID.myprocid, rv, *node, *type ) ;
    pause() ;
  }

  *node = status.MPI_SOURCE ;
  *type = status.MPI_TAG ;

#if ( monit >= 2 )
  fprintf( stderr, "(t%x) COM_ReadCpl type=%5d size=%6d from t%x\n",
           COM_ID.myprocid, *type, size, *node ) ;
#endif

#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "ReadCpl : node %d, type %d size %d\n",
            *node, *type, size ) ;
#endif

  return rv ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_WriteChr( char *buf, int size, int node, int type )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  int rv ;

  rv = MPI_Send( buf, size, MPI_CHAR, node, type, MPI_COMM_WORLD ) ;

  if ( rv ) {
    fprintf( stderr, "(%x) COM_WriteChr - error#%d to %x %d\n",
             COM_ID.myprocid, rv, node, type ) ;
    pause() ;
  }
#if ( monit >= 2 )
  fprintf( stderr, "(t%x) write     type=%5d size=%6d to  t%x\n",
           COM_ID.myprocid, type, size, node ) ;
#endif

#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "WriteChr : node %d, type %d, size %d\n",
            node, type, size ) ;
#endif

  return rv ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_WriteInt( int *buf, int size, int node, int type )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  int rv ;

  rv = MPI_Send( buf, size, MPI_INT, node, type, MPI_COMM_WORLD ) ;

  if ( rv ) {
    fprintf( stderr, "(%x) COM_WriteInt - error#%d to %x %d\n",
             COM_ID.myprocid, rv, node, type ) ;
    pause() ;
  }
#if ( monit >= 2 )
  fprintf( stderr, "(t%x) write     type=%5d size=%6d to  t%x\n",
           COM_ID.myprocid, type, size, node ) ;
#endif

#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "WriteInt : node %d, type %d, size %d\n",
            node, type, size ) ;
#endif

  return rv ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_WriteDbl( double *buf, int size, int node, int type )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  int rv ;
  rv = MPI_Send( buf, size, MPI_DOUBLE, node, type, MPI_COMM_WORLD ) ;

  if ( rv < 0 ) {
    fprintf( stderr, "(%x) COM_WriteDbl - error#%d to %x %d\n",
             COM_ID.myprocid, rv, node, type ) ;
    pause() ;
  }
#if ( monit >= 2 )
  fprintf( stderr, "(t%x) COM_WriteDbl type=%5d size=%6d to  t%x\n",
           COM_ID.myprocid, type, size, node ) ;
#endif

#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "WriteDbl : node %d, type %d, size %d\n",
            node, type, size ) ;
#endif

  return rv ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_WriteCpl( Complex *buf, int size, int node, int type )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
  int rv ;
  rv = MPI_Send( buf, size, COM_Dtype_Complex, node, type, MPI_COMM_WORLD ) ;

  if ( rv < 0 ) {
    fprintf( stderr, "(%x) COM_WriteCpl - error#%d to %x %d\n",
             COM_ID.myprocid, rv, node, type ) ;
    pause() ;
  }
#if ( monit >= 2 )
  fprintf( stderr, "(t%x) COM_WriteCpl type=%5d size=%6d to  t%x\n",
           COM_ID.myprocid, type, size, node ) ;
#endif

#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "WriteCpl : node %d, type %d, size %d\n",
            node, type, size ) ;
#endif

  return rv ;
#else /* s-mode */
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_MaxChr( char *send, char *recv, int count )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "MaxChr : count %d\n", count ) ;
#endif
  return( MPI_Allreduce(send, recv, count, MPI_CHAR, MPI_MAX,
                        MPI_COMM_WORLD) ) ;
#else /* s-mode */
  int    i ;
  for( i=0 ; i<count ; i++ ) recv[i] = send[i] ;
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_MaxInt( int *send, int *recv, int count )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "MaxInt : count %d\n", count ) ;
#endif
  return( MPI_Allreduce(send, recv, count, MPI_INT, MPI_MAX,
                        MPI_COMM_WORLD) ) ;
#else /* s-mode */
  int    i ;
  for( i=0 ; i<count ; i++ ) recv[i] = send[i] ;
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_MaxDbl( double *send, double *recv, int count )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "MaxDbl : count %d\n", count ) ;
#endif
  return( MPI_Allreduce(send, recv, count, MPI_DOUBLE, MPI_MAX,
                        MPI_COMM_WORLD) ) ;
#else /* s-mode */
  int    i ;
  for( i=0 ; i<count ; i++ ) recv[i] = send[i] ;
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_MaxCpl( Complex *send, Complex *recv, int count )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "MaxCpl : count %d\n", count ) ;
#endif
  return( MPI_Allreduce(send, recv, count, COM_Dtype_Complex, MPI_MAX,
                        MPI_COMM_WORLD) ) ;
#else /* s-mode */
  int    i ;
  for( i=0 ; i<count ; i++ ) recv[i] = send[i] ;
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Grp_MaxChr( char *send, char *recv, int count )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "GrpMaxChr : count %d\n", count ) ;
#endif
  return( MPI_Allreduce(send, recv, count, MPI_CHAR, MPI_MAX,
                        COM_GroupID_me) ) ;
#else /* s-mode */
  int    i ;
  for( i=0 ; i<count ; i++ ) recv[i] = send[i] ;
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Grp_MaxInt( int *send, int *recv, int count )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "GrpMaxInt : count %d\n", count ) ;
#endif
  return( MPI_Allreduce(send, recv, count, MPI_INT, MPI_MAX,
                        COM_GroupID_me) ) ;
#else /* s-mode */
  int    i ;
  for( i=0 ; i<count ; i++ ) recv[i] = send[i] ;
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Grp_MaxDbl( double *send, double *recv, int count )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "GrpMaxDbl : count %d\n", count ) ;
#endif
  return( MPI_Allreduce(send, recv, count, MPI_DOUBLE, MPI_MAX,
                        COM_GroupID_me) ) ;
#else /* s-mode */
  int    i ;
  for( i=0 ; i<count ; i++ ) recv[i] = send[i] ;
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}

/***********************************************************************/

int COM_Grp_MaxCpl( Complex *send, Complex *recv, int count )
{
#ifndef __ADVFW_SINGLE_MODE__ /* h-mode, p-mode */
#ifdef __ADVFW_COMM_DBG__
  Pfprintf( __ADVFW_COMM_FP__, "GrpMaxCpl : count %d\n", count ) ;
#endif
  return( MPI_Allreduce(send, recv, count, COM_Dtype_Complex, MPI_MAX,
                        COM_GroupID_me) ) ;
#else /* s-mode */
  int    i ;
  for( i=0 ; i<count ; i++ ) recv[i] = send[i] ;
  return 0 ;
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */
}
