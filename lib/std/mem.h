/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __MEM_H__
#define __MEM_H__


#include "advfullwave_lib.h"


/******************
 *     Malloc     *
 ******************/
/*** debuging option ***/
#define __ADVFW_MEM_FP__ stderr
/*#define __ADVFW_MEM_DBG__ 0*/


#define Malloc( x, y )        __MemoryAllocate__( (x)*(y), NULL, __FILE__, __LINE__ )
#define Calloc( x, y )        __ClearAllocate__( (x)*(y), NULL, __FILE__, __LINE__ )
#define Realloc( a, x, y, z ) __ReAllocate__( (a), (x)*(y), (x)*(z), NULL, __FILE__, __LINE__ )
#define Free( x, y )          __FreeMemory__( (void *)(&(y)), (x), NULL, __FILE__, __LINE__ )


/*#define Mal( x, y )       (__MemoryAllocate__( (x), (y), __FILE__, __LINE__ ))
#define Cal( x, y )       (__ClearAllocate__( (x), (y), __FILE__, __LINE__ ))
#define Ral( a, x, y, z ) (__ReAllocate__( (a), (x), (y), (z), __FILE__, __LINE__ ))

#define FrM( x, y, z )    (__FreeMemory__( (x), (y), (z), __FILE__, __LINE__ ))*/


/*** functions ***/
void mem_Init( size_t max_memory ) ;
void mem_Finalize( void ) ;
size_t getMem_rest( void ) ;
size_t getMem_use( void ) ;
size_t getMem_usemax( void ) ;
void mem_err( FILE *fp, size_t num, char *name, char *mname,
              char *mess ) ;
void *__MemoryAllocate__( size_t mem, char *name, char *fname,
                          int line ) ;
void *__ClearAllocate__( size_t mem, char *name, char *fname,
                         int line ) ;
int __AllocateFunction__( size_t mem, char *name, char *func,
                          char *fname, int line ) ;
void *__ReAllocate__( void *ptr, size_t oldmem, size_t newmem,
                      char *name, char *fname, int line ) ;
void __FreeMemory__( void **x, size_t mem, char *name, char *fname,
                     int line ) ;

#endif /* __MEM_H__ */
