/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __COMM_MPI_H__
#define __COMM_MPI_H__


#include "advfullwave_lib.h"


/*** debuging option ***/
#define __ADVFW_COMM_FP__ stdout
/*#define __ADVFW_COMM_DBG__*/


#ifndef __ADVFW_SINGLE_MODE__
#include <mpi.h>
#endif /* #ifndef __ADVFW_SINGLE_MODE__ */


#define GRP_PARENT      0
#define GRP_CHILD       1


typedef struct {
  int  group;      /*+ GroupID              +*/
  int  mygprocid;  /*+ ProcessID in a group +*/
  int  nparent;    /*+ num. of parents      +*/
  int  nchild;     /*+ num. of children     +*/
  int  rootid;     /*+ ProcID of root       +*/
  int *parid;      /*+ ProcIDs of parents   +*/
  int *chiid;      /*+ ProcIDs of children  +*/
} COM_GRP_ID;

typedef struct {
  int nproc;      /*+ num. of PE +*/
  int myprocid;   /*+ ProcID +*/
  int *allid ;
} COM_WLD_ID;


int COM_Init(int * pargc, char *** pargv) ;
int COM_Finalize( void ) ;
int COM_Def_Struct_Complex( void ) ;
void COM_SetGroup ( int nparent ) ;
int COM_Bcast_Chr( char *buf, int count, int root ) ;
int COM_Bcast_Int( int *buf, int count, int root ) ;
int COM_Bcast_Dbl( double *buf, int count, int root ) ;
int COM_Bcast_Cpl( Complex *buf, int count, int root ) ;
int COM_Grp_Bcast_Chr( char *buf, int count, int root ) ;
int COM_Grp_Bcast_Int( int *buf, int count, int root ) ;
int COM_Grp_Bcast_Dbl( double *buf, int count, int root ) ;
int COM_Grp_Bcast_Cpl( Complex *buf, int count, int root ) ;
int COM_Synchronize(void) ;
int COM_Grp_Synchronize(void) ;
void COM_Exit(int status);

int COM_ReadChr( char *buf, int size, int *node, int *type ) ;
int COM_ReadInt( int *buf, int size, int *node, int *type ) ;
int COM_ReadDbl( double *buf, int size, int *node, int *type ) ;
int COM_ReadCpl( Complex *buf, int size, int *node, int *type ) ;
int COM_WriteChr( char *buf, int size, int node, int type ) ;
int COM_WriteInt( int *buf, int size, int node, int type ) ;
int COM_WriteDbl( double *buf, int size, int node, int type ) ;
int COM_WriteCpl( Complex *buf, int size, int node, int type ) ;

int COM_MaxChr( char *send, char *recv, int count ) ;
int COM_MaxInt( int *send, int *recv, int count ) ;
int COM_MaxDbl( double *send, double *recv, int count ) ;
int COM_MaxCpl( Complex *send, Complex *recv, int count ) ;
int COM_Grp_MaxChr( char *send, char *recv, int count ) ;
int COM_Grp_MaxInt( int *send, int *recv, int count ) ;
int COM_Grp_MaxDbl( double *send, double *recv, int count ) ;
int COM_Grp_MaxCpl( Complex *send, Complex *recv, int count ) ;

#endif /* __COMM_MPI_H__ */
