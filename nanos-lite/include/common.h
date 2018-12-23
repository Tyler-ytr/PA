#ifndef __COMMON_H__
#define __COMMON_H__

/* Uncomment these macros to enable corresponding functionality. */
#define HAS_CTE
#define HAS_VME//Nanos-lite在初始化的时候首先就会调用init_mm()函数(在nanos-lite/src/mm.c中定义)来初始化MM. 

#include <am.h>
#include <klib.h>
#include "debug.h"

typedef char bool;
#define true 1
#define false 0
extern _Context* schedule(_Context *prev);
#endif
