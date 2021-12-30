/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "mem.h"


/***** Malloc  ******/
static int    memcheck   = 0 ;
static size_t memmax     = 0 ;
static size_t memuse     = 0 ;
static size_t memuse_max = 0 ;



void mem_Init( size_t max_memory )
{
  /*+ Function: set limit of memory be able to use +*/
  /* if set 0, not set limit */
  if( max_memory > 0 ) {
    memmax   = max_memory ;
    memcheck = 1 ;
    Fprintf( stdout, "Set the limit of memory [ %lu Byte ]\n#\n",
             memmax );
  } else if( max_memory == 0 ) {
    memcheck = -1 ;
    Fprintf( stdout, "Not set the limit of memory\n#\n" );
  } else {
    emessage( 0, __ABORT_ADVFW_LIB__, NULL ) ;
  }
}

/********************************************************************/

void mem_Finalize( void )
{
  if( getMem_use() != 0 ) {
#ifdef __ADVFW_DEBUG__
    Pfprintf( stdout, "now memory [byte]: %lu\n", getMem_use() ) ;
#endif /* __ADVFW_DEBUG__ */
  }
}

/********************************************************************/

size_t getMem_rest( void )
{
  /*+ Function: return amount of memory be able to use now +*/
  return (memmax - memuse) ;
}

/********************************************************************/

size_t getMem_use( void )
{
  /*+ Function: return amount of memory use +*/
  return memuse ;
}

/********************************************************************/

size_t getMem_usemax( void )
{
  /*+ Function: return amount of maximum memory use +*/
  return memuse_max ;
}

/********************************************************************/

void mem_err( FILE *fp, size_t num, char *name, char *mname, char *mess )
{
  /*+ Function: output error message for memory +*/
  fprintf( fp, "error: in %s\n", mname ) ;
  fprintf( fp,
           "      ( limit = %lu, now use = %lu, request = %lu, name = \"%s\" )\n",
           (unsigned long)memmax, (unsigned long)(memuse - num),
           (unsigned long)num, name ) ;
  fprintf( fp, "%s\n", mess ) ;
  Exit( __ABORT_ADVFW_LIB__ ) ;
}

/********************************************************************/

void *__MemoryAllocate__( size_t mem, char *name, char *fname,
                          int line )
{
  /*+ Function: malloc +*/
  int i ;


  i = __AllocateFunction__( mem, name, "malloc", fname, line ) ;
  if( i == 1 ) {
    return malloc( mem ) ;
  } else {
    return NULL ;
  }
}

/********************************************************************/

void *__ClearAllocate__( size_t mem, char *name, char *fname,
                         int line )
{
  /*+ Function: calloc +*/
  int i ;


  i = __AllocateFunction__( mem, name, "calloc", fname, line ) ;
  if( i == 1 ) {
    return calloc( mem, 1 ) ;
  } else {
    return NULL ;
  }
}

/********************************************************************/

int __AllocateFunction__( size_t mem, char *name, char *func,
                          char *fname, int line )
{
  /*+ Function: allocation memory +*/
  if( memcheck == 0 ) {
    emessage( -1, __ABORT_ADVFW_LIB__, NULL ) ;
  }


  if( mem == 0 ) {
    return 0 ;
  } else if( mem < 0 ) {
    char ffn[1024] ;
    sprintf( ffn, "%s line %d  ", fname, line ) ;
    mem_err( __ADVFW_MEM_FP__, mem, name, ffn, "negative argument" ) ;
  }

  memuse += mem ;
  if( (memcheck == 1) && (memmax < memuse) ){
    char ffn[1024] ;
    sprintf( ffn, "%s line %d  ", fname, line ) ;
    mem_err( __ADVFW_MEM_FP__, mem, name, ffn, "memory allocation fault" ) ;
  }else{
#ifdef __ADVFW_MEM_DBG__
    Pfprintf( __ADVFW_MEM_FP__,
              "%s : %s  %lu bytes  in \"%s\" line %d\n",
              func, name, (unsigned long)mem, fname, line ) ;
#endif

    if( memuse_max < memuse ) {
      memuse_max = memuse ;
    }

    return 1 ;
  }


  return 0 ;
}

/********************************************************************/

void *__ReAllocate__( void *ptr, size_t oldmem, size_t newmem,
                      char *name, char *fname, int line )
{
  /*+ Function: realloc +*/
  int i ;


  if( memcheck == 0 ) {
    emessage( -1, __ABORT_ADVFW_LIB__, NULL ) ;
  }


  if( (oldmem == 0) || (ptr == NULL) ) {
    i = __AllocateFunction__( newmem, name, "Realloc", fname, line ) ;
    if( i == 1 ) {
      return malloc( newmem ) ;
    } else {
      return NULL ;
    }
  } else if( oldmem < 0 ) {
    char ffn[1024] ;
    sprintf( ffn, "%s line %d  ", fname, line ) ;
    mem_err( __ADVFW_MEM_FP__, oldmem, name, ffn, "negative argument" ) ;
  } else if( memuse < oldmem ) {
    emessage( -2, __ABORT_ADVFW_LIB__, NULL ) ;
  }


  if( newmem == 0 ) {
    __FreeMemory__( ptr, oldmem, name, fname, line ) ;
    return NULL ;
  } else if( newmem < 0 ) {
    char ffn[1024] ;
    sprintf( ffn, "%s line %d  ", fname, line ) ;
    mem_err( __ADVFW_MEM_FP__, newmem, name, ffn,
             "negative argument" ) ;
  }

  memuse += (newmem - oldmem) ;
  if( (memcheck == 1) && (memmax < memuse) ){
    char ffn[1024] ;
    sprintf( ffn, "%s line %d  ", fname, line ) ;
    mem_err( __ADVFW_MEM_FP__, newmem, name, ffn,
             "memory allocation fault" ) ;
  }else{
#ifdef __ADVFW_MEM_DBG__
    Pfprintf( __ADVFW_MEM_FP__,
              "realloc : %s  %d bytes  in \"%s\" line %d\n",
               name, (newmem-oldmem), fname, line ) ;
#endif

    if( memuse_max < memuse ) {
      memuse_max = memuse ;
    }

    return realloc( ptr, newmem ) ;
  }


  return NULL ;
}

/********************************************************************/

void __FreeMemory__( void **x, size_t mem, char *name, char *fname,
                     int line )
{
  /*+ Function: free +*/
  free( *x ) ;
  (*x) = NULL ;
#ifdef __ADVFW_DEBUG__
  if( memuse < mem ) {
    Pfprintf( stdout,
              "  Warning !, memory already seems to be too much freed\n" ) ;
    Pfprintf( stdout, "    in \"%s\"  line %d\n", fname, line ) ;
    /*mem = 0 ;*/
  }
#endif /* __ADVFW_DEBUG__ */
  memuse -= mem ;
#ifdef __ADVFW_MEM_DBG__
  Pfprintf( __ADVFW_MEM_FP__,
            "free : %s  %lu bytes  in \"%s\" line %d\n",
            name, (unsigned long)mem, fname, line ) ;
#endif
}
