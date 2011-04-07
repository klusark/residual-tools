/* Residual - A 3D game interpreter
 *
 * Residual is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://residual.svn.sourceforge.net/svnroot/residual/residual/trunk/common/sys.h $
 * $Id: sys.h 1648 2010-01-23 09:56:04Z aquadran $
 *
 */

#ifndef COMMON_SYS_H
#define COMMON_SYS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#ifndef _MSC_VER
#include <dirent.h>
#include <unistd.h>
#endif

#ifdef _MSC_VER
#pragma once
#pragma warning( disable : 4068 ) // turn off "unknown pragma" warning
#pragma warning( disable : 4100 ) // turn off "unreferenced formal parameter" warning
#pragma warning( disable : 4127 ) // turn off "conditional expression is constant" warning
#pragma warning( disable : 4189 ) // turn off "local variable is initialized but not referenced" warning
#pragma warning( disable : 4244 ) // turn off "conversion type" warning
#pragma warning( disable : 4250 ) // turn off "inherits via dominance" warning
#pragma warning( disable : 4505 ) // turn off "unreferenced local function has been removed" warning
#pragma warning( disable : 4512 ) // turn off "assignment operator could not be generated" warning
#pragma warning( disable : 4611 ) // turn off "interaction between '_setjmp' and C++ object destruction is non-portable" warning
#pragma warning( disable : 4800 ) // turn off "forcing value to bool 'true' or 'false' (performance warning)"
#pragma warning( disable : 4996 ) // turn off "This function or variable may be unsafe" warning

// vsnprintf is already defined in Visual Studio 2008
#if (_MSC_VER < 1500)
	#define vsnprintf _vsnprintf
#endif
		
#endif

#ifndef LOCAL_PI
#define LOCAL_PI 3.14159265358979323846
#endif

// Use config.h, generated by configure
#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

// In the following we configure various targets, in particular those
// which can't use our "configure" tool and hence don't use config.h.
//
// Some #defines that occur here frequently:
// SYSTEM_LITTLE_ENDIAN
//    - Define this on a little endian target
// SYSTEM_BIG_ENDIAN
//    - Define this on a big endian target
// SYSTEM_NEED_ALIGNMENT
//    - Define this if your system has problems reading e.g. an int32 from an odd address
// SYSTEM_DONT_DEFINE_TYPES
//    - Define this if you need to provide your own typedefs, e.g. because your
//      system headers conflict with our typenames, or because you have odd
//      type requirements.
// SMALL_SCREEN_DEVICE
//    - ...
// ...

// We define all types in config.h, so we don't want to typedef those types
// here again!
#ifdef HAVE_CONFIG_H
#define SYSTEM_DONT_DEFINE_TYPES
#endif


//
// By default we try to use pragma push/pop to ensure various structs we use
// are "packed". If your compiler doesn't support this pragma, you are in for
// a problem. If you are lucky, there is a compiler switch, or another pragma,
// doing the same thing -- in that case, try to modify common/pack-begin.h and
// common/pack-end.h accordingly. Or maybe your port simply *always* packs
// everything, in which case you could #undefine SCUMMVM_USE_PRAGMA_PACK.
//
// If neither is possible, tough luck. Try to contact the team, maybe we can
// come up with a solution, though I wouldn't hold my breath on it :-/.
//
#define SYSTEM_USE_PRAGMA_PACK


#if defined(__SYMBIAN32__)

	#define SYSTEM_LITTLE_ENDIAN
	#define SYSTEM_NEED_ALIGNMENT

	#define SMALL_SCREEN_DEVICE

	// Enable Symbians own datatypes
	// This is done for two reasons
	// a) uint is already defined by Symbians libc component
	// b) Symbian is using its "own" datatyping, and the Residual port
	//    should follow this to ensure the best compability possible.
	#define SYSTEM_DONT_DEFINE_TYPES
	typedef unsigned char byte;

	typedef unsigned char uint8;
	typedef signed char int8;

	typedef unsigned short int uint16;
	typedef signed short int int16;

	typedef unsigned long int uint32;
	typedef signed long int int32;

#elif defined(_WIN32_WCE)

	#define strcasecmp stricmp
	#define strncasecmp strnicmp
	#define snprintf _snprintf

	#define SYSTEM_LITTLE_ENDIAN

	#ifndef __GNUC__
		#define FORCEINLINE __forceinline
		#define NORETURN_PRE __declspec(noreturn)
	#endif
	#define PLUGIN_EXPORT __declspec(dllexport)

	#if _WIN32_WCE < 300
	#define SMALL_SCREEN_DEVICE
	#endif

#elif defined(_MSC_VER)

	#define strcasecmp stricmp
	#define strncasecmp strnicmp
	#define snprintf _snprintf

	#define SYSTEM_LITTLE_ENDIAN

	#define FORCEINLINE __forceinline
	#define NORETURN_PRE __declspec(noreturn)
	#define PLUGIN_EXPORT __declspec(dllexport)


#elif defined(__MINGW32__)

	#define strcasecmp stricmp
	#define strncasecmp strnicmp

	#define SYSTEM_LITTLE_ENDIAN

	#define PLUGIN_EXPORT __declspec(dllexport)

#elif defined(UNIX)

	#ifndef CONFIG_H
		/* need this for the SDL_BYTEORDER define */
		#include <SDL_byteorder.h>

		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		#define SCUMM_LITTLE_ENDIAN
		#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
		#define SCUMM_BIG_ENDIAN
		#else
		#error Neither SDL_BIG_ENDIAN nor SDL_LIL_ENDIAN is set.
		#endif
	#endif

	// You need to set this manually if necessary
//	#define SYSTEM_NEED_ALIGNMENT

	#if defined(__DECCXX) // Assume alpha architecture
	#define INVERSE_MKID
	#define SYSTEM_NEED_ALIGNMENT
	#endif

#elif defined(__PALMOS_TRAPS__)	|| defined (__PALMOS_ARMLET__)

#ifdef __PALMOS_ARMLET__
	#include <extras_string.h>
#endif
	#define SYSTEM_LITTLE_ENDIAN

	#define strcasecmp stricmp
	#define strncasecmp strnicmp

	#define SYSTEM_NEED_ALIGNMENT
	#define STRINGBUFLEN 256

	extern const char *RESIDUAL_SAVEPATH;

	#if !defined(COMPILE_ZODIAC) && !defined(COMPILE_OS5)
	#	define NEWGUI_256
	#else
	#	undef UNUSED
	#endif

#elif defined(__DC__)

	#define SYSTEM_LITTLE_ENDIAN
	#define SYSTEM_NEED_ALIGNMENT

#elif defined(__GP32__)

	#define strcasecmp stricmp
	#define strncasecmp strnicmp

	#define SYSTEM_LITTLE_ENDIAN
	#define SYSTEM_NEED_ALIGNMENT

	// Override typenames. uint is already defined by system header files.
	#define SYSTEM_DONT_DEFINE_TYPES
	typedef unsigned char byte;

	typedef unsigned char uint8;
	typedef signed char int8;

	typedef unsigned short int uint16;
	typedef signed short int int16;

	typedef unsigned long int uint32;
	typedef signed long int int32;

#elif defined(__PLAYSTATION2__)

	#define SYSTEM_LITTLE_ENDIAN
	#define SYSTEM_NEED_ALIGNMENT
#elif defined(__N64__)

	#define SYSTEM_BIG_ENDIAN
	#define SYSTEM_NEED_ALIGNMENT

	#define STRINGBUFLEN 256

	#define SYSTEM_DONT_DEFINE_TYPES
	typedef unsigned char byte;

	typedef unsigned char uint8;
	typedef signed char int8;

	typedef unsigned short int uint16;
	typedef signed short int int16;

	typedef unsigned int uint32;
	typedef signed int int32;

	typedef unsigned long long uint64;
	typedef signed long long int64;

#elif defined(__PSP__)

	#define	SYSTEM_LITTLE_ENDIAN
	#define	SYSTEM_NEED_ALIGNMENT

#elif defined(__amigaos4__)

	#define	SYSTEM_BIG_ENDIAN
	#define	SYSTEM_NEED_ALIGNMENT

#elif defined (__DS__)

	#define strcasecmp stricmp
	#define strncasecmp strnicmp

	#define SYSTEM_NEED_ALIGNMENT
	#define SYSTEM_LITTLE_ENDIAN

	#include "nds/jtypes.h"

	#define SYSTEM_DONT_DEFINE_TYPES

	#define STRINGBUFLEN 256
	#define printf(fmt, ...)					consolePrintf(fmt, ##__VA_ARGS__)

#elif defined(__WII__)

	#define	SYSTEM_BIG_ENDIAN
	#define	SYSTEM_NEED_ALIGNMENT

#else
	#error No system type defined

#endif


//
// GCC specific stuff
//
#if defined(__GNUC__)
	#define NORETURN __attribute__((__noreturn__))
	#define PACKED_STRUCT __attribute__((packed))
	#define GCC_PRINTF(x,y) __attribute__((format(printf, x, y)))
#else
	#define PACKED_STRUCT
	#define GCC_PRINTF(x,y)
#endif

//
// Fallbacks / default values for various special macros
//
#ifndef FORCEINLINE
#define FORCEINLINE inline
#endif

#ifndef PLUGIN_EXPORT
#define PLUGIN_EXPORT
#endif

#ifndef NORETURN_PRE
#define	NORETURN_PRE
#endif

#ifndef NORETURN_POST
#define	NORETURN_POST
#endif

#ifndef STRINGBUFLEN
#define STRINGBUFLEN 1024
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN 256
#endif


//
// Typedef our system types unless SYSTEM_DONT_DEFINE_TYPES is set.
//
#ifndef SYSTEM_DONT_DEFINE_TYPES
	typedef unsigned char byte;
	typedef unsigned char uint8;
	typedef signed char int8;
	typedef unsigned short uint16;
	typedef signed short int16;
	typedef unsigned int uint32;
	typedef signed int int32;
	typedef unsigned int uint;
#endif


#endif // COMMON_SYS_H