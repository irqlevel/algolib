#pragma once

#ifndef NULL
#define NULL 0
#endif

typedef __INT8_TYPE__	s8;
typedef __INT16_TYPE__ s16;
typedef __INT32_TYPE__ s32;
typedef __INT64_TYPE__ s64;

typedef __UINT8_TYPE__	u8;
typedef __UINT16_TYPE__ u16;
typedef __UINT32_TYPE__ u32;
typedef __UINT64_TYPE__ u64;

typedef __SIZE_TYPE__ size_t;

#define ct_assert(e) extern char (*ct_assert(void)) [sizeof(char[1 - 2*!(e)])]
