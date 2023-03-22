#ifndef DVR_DEFINES_H
#define DVR_DEFINES_H

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include "dvr_app_api.h"
#include "predefine.h"
#include "coordinator.h"
#include "strdefine.h"
#include "struct.h"
#include "dvr_util.h"

#include "define.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern int g_rfs_type;
/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
#define eprintf(x...) printf("!err-ui: " x);
//#define dprintf(x...) printf("ui: " x);
#define dprintf(x...)

#ifndef FALSE
  #define FALSE		0
#endif

#ifndef TRUE
  #define TRUE		1
#endif

#endif 


