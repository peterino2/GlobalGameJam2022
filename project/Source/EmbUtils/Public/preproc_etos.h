/** *
 * Copyright (C) 2021-2021
 *
 * Peter Li - All Rights Reserved
 * Unauthorized copying of this file, 
 * or any of the code content within the Embersong source directory
 * via any medium is strictly prohibited
 * Proprietary and confidential
 * 
 * Peter Li <Peterlimail47@gmail.com>
 * */

#pragma once

#include "preproc_repeat.h"


#if __cplusplus
#define _CPP_ETOS(E) inline const char * etos(E val){return etos_##E(val);}
#else
#define _CPP_ETOS(E)
#endif

#define __ETOS_SIZECHECK_INNER(X, ...)
#define __ETOS_SIZECHECK(E, ...)\
    inline const int enumcount_##E(){ E __fakearray[] = {PP_REPEAT(_PP_COMMAS, __VA_ARGS__)}; \
        return sizeof(__fakearray)/sizeof(__fakearray[0]);} \

#define __ETOS_INNER(X, ...) case X: return #X;

#define ETOS_DECLARE(E, ...) typedef enum { \
    PP_REPEAT(_PP_COMMAS, __VA_ARGS__)\
    }E;\
        \
    inline const char* etos_##E(E val){\
        switch(val){\
            PP_REPEAT(__ETOS_INNER, __VA_ARGS__)\
            default:\
                return "Unknown value for enum: " #E;\
        }\
    }\
    _CPP_ETOS(E)\
    __ETOS_SIZECHECK(E, __VA_ARGS__)

