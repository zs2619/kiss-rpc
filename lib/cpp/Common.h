//==============================================
/**
	@date:		2012:3:31  
	@file: 		Common.h
	@author		zhangshuai
*/
//==============================================

#ifndef		__RPCTYPEDEF_H__
#define		__RPCTYPEDEF_H__

typedef unsigned char 	uint8;
typedef signed   char 	int8;

typedef unsigned short 	uint16;
typedef signed	 short 	int16;


typedef unsigned int 	uint32;
typedef signed	 int 	int32;

typedef signed	 long long 	int64;
typedef unsigned long  long uint64;

#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <chrono>
#include "EnumMap.h"
#define RpcAssert(E) assert(E)
#endif