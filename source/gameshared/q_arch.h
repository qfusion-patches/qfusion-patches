/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef GAME_QARCH_H
#define GAME_QARCH_H

#ifdef __cplusplus
extern "C" {
#endif

// global preprocessor defines
#include "config.h"

// q_shared.h -- included first by ALL program modules
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#ifndef _MSC_VER
#include <strings.h>
#endif
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
};
#endif

#ifdef __cplusplus
#include <type_traits>
#endif

#ifdef __cplusplus
extern "C" {
#endif

//==============================================

#if defined ( __linux__ )

#define LIB_DIRECTORY "libs"
#define LIB_PREFIX "lib"
#define LIB_SUFFIX ".so"

#define BUILDSTRING "Linux"
#define OSNAME "Linux"

#if defined ( __x86_64__ )
#define ARCH "x86_64"
#define CPUSTRING "x86_64"
#else
#define CPUSTRING "Unknown"
#define ARCH "Unknown"
#endif

#define VAR( x ) # x

#include <alloca.h>

// wsw : aiwa : 64bit integers and integer-pointer types
typedef int ioctl_param_t;

typedef int socket_handle_t;

#define SOCKET_ERROR (-1)
#define INVALID_SOCKET (-1)

#endif

//==============================================

#define qcdecl

#define Q_stricmp( s1, s2 ) strcasecmp( ( s1 ), ( s2 ) )
#define Q_strnicmp( s1, s2, n ) strncasecmp( ( s1 ), ( s2 ), ( n ) )

#if defined ( __GNUC__ )
#define ATTRIBUTE_ALIGNED( x ) __attribute__( ( aligned( x ) ) )
#define ATTRIBUTE_NOINLINE     __attribute__((noinline))
#define ATTRIBUTE_NAKED
#else
#define ATTRIBUTE_ALIGNED( x )
#define ATTRIBUTE_NOINLINE
#define ATTRIBUTE_NAKED
#endif

#if defined ( __GNUC__ )
#define QF_DLL_IMPORT __attribute__ ((visibility("default")))
#define QF_DLL_EXPORT __attribute__ ((visibility("default")))
#define QF_DLL_LOCAL  __attribute__ ((visibility("hidden")))
#else
#define QF_DLL_IMPORT
#define QF_DLL_EXPORT
#define QF_DLL_LOCAL
#endif

//==============================================

#ifdef __cplusplus
};
#endif

#endif // GAME_QARCH_H
