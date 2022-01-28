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

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)
#define CLAMP(x, upper, lower) (MIN(upper, MAX(x, lower)))

#define SHOW_FVECTOR(fvector, msg, ...) do{auto _fv = fvector; UE_LOG(LogTemp, Display, TEXT(#fvector " = %fx %fy %fz; %s\n"), \
    _fv.X, _fv.Y, _fv.Z, TEXT(msg), ##__VA_ARGS__);}while(0)

#define SHOW_ERROR(msg, ...) do{ UE_LOG(LogTemp, Error, TEXT(" ERROR: %s\n"), TEXT(msg) ##__VA_ARGS__ ); } while(0)

// Use only in that rare case where you dont have any debugger output
#define USE_DESPERATE_TRACE 1

// probably just leave this one on
#define USE_WARN_ASSERT 1

#ifdef USE_DESPERATE_TRACE

#define DESPERATE_TRACE() do{\
    UE_LOG(LogTemp, Warning, TEXT("XX TRACE: %s :%d %s "), *FString(__FILE__), __LINE__, *FString(__func__));\
}while(0)

#else
#define DESPERATE_TRACE() do{}while(0)
#endif

#if USE_WARN_ASSERT
#define WARN_ASSERT(condition, msg, ...) do{ \
    if(!(condition))\
    {\
        UE_LOG(LogTemp, Warning, TEXT("[Assert!][%s :%d %s() ]: "msg), *FString(__FILE__), __LINE__, *FString(__func__), ##__VA_ARGS__)\
    }\
}while(0)

#else 

#define WARN_ASSERT(...)

#endif

#define WMSG(msg, ...) UE_LOG(LogTemp, Warning, TEXT("[todo: remove this msg][%s :%d %s() ]: "msg), *FString(__FILE__), __LINE__, *FString(__func__), ##__VA_ARGS__)

#ifdef IS_TEST_CPP
#define TESTDISPLAY(msg, ...) UE_LOG(LogTemp, Display, TEXT("[TEST]: "msg), ##__VA_ARGS__)
#else
#define TESTDISPLAY(msg, ...) UE_LOG(LogTemp, Warning, TEXT("[TEST(but not in a test.cpp)][%s :%d %s() ]: "msg), *FString(__FILE__), __LINE__, *FString(__func__), ##__VA_ARGS__)
#endif

#define GET_ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
