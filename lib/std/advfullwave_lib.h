/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef __ADVFW_LIB_H__
#define __ADVFW_LIB_H__


#ifndef __ADVFW_HIERARCHICAL_MODE__
#ifndef __ADVFW_PARALLEL_MODE__
#ifndef __ADVFW_SINGLE_MODE__
#define __ADVFW_SINGLE_MODE__
#endif
#endif
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <math.h>
#include <errno.h>
#include <complex.h>

#ifdef __ADVFW_OS_UNIX__
#include <sys/time.h>
#include <sys/stat.h>
#endif /* __ADVFW_OS_UNIX__ */

#include "Adv/AdvDocument.h"
#include "defs.h"
#include "types.h"
#include "struct.h"
#include "mem.h"
#include "timefunc.h"
#include "iofunc.h"
#include "comm_mpi.h"
#include "fdname.h"
#include "fileio.h"
#include "func_main.h"
#include "sort.h"
#include "real.h"
#include "complex_adv.h"
#include "errormessage.h"
#include "options.h"


#define Exit COM_Exit

/* iofunc */
#define PRE_MESSAGE          "#"
#define END_MESSAGE          ""

/* errormessage */
#define __ABORT_NONE__       0
#define __ABORT_ADVFW_LIB__ 1

#endif /* __AFW_LIB_H__ */
