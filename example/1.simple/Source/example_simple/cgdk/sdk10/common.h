//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                                  Common                                   *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*  This Program is programmed by Cho SangHyun. sangduck@cgcii.co.kr         *
//*  Best for Game Developement and Optimized for Game Developement.          *
//*                                                                           *
//*                (c) 2003. Cho Sanghyun. All right reserved.                *
//*                          http://www.CGCII.co.kr                           *
//*                                                                           *
//*****************************************************************************

#pragma once
//-----------------------------------------------------------------------------
//
//  definitions
//
//-----------------------------------------------------------------------------
// 1) NDEBUG...
#if !defined(NDEBUG) && !defined(_DEBUG)
	#define	_DEBUG
#endif

// 2) STL..
#ifdef NDEBUG
	#ifndef _SECURE_SCL
		#define	_SECURE_SCL	0
	#endif

	#ifndef _HAS_ITERATOR_DEBUGGING
		#define _HAS_ITERATOR_DEBUGGING 0
	#endif
#endif

// 3) Settings
#define	_CGD_TIME

// 4) 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다(WIN32용)
#define WIN32_LEAN_AND_MEAN

// 5) CGDK
#define _CGDK	10


//-----------------------------------------------------------------------------
// STL & Containers
//-----------------------------------------------------------------------------
// 1) C++ standard header
#include <climits>
#include <map>
#include <condition_variable>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <string>

// 2) containers
#include "containers/circular_list"


//-----------------------------------------------------------------------------
// Basic)
//-----------------------------------------------------------------------------
#include "definitions.unreal.h"


//-----------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------
// 1) tick clock
#define		_USE_TICK_CLOCK_STEADY

// 2) time clock
#define		_USE_TIME_CLOCK_LINUX_STANDARD

// 3) event clock
#define		_USE_EVENT_CLOCK_SYSTEM

// 53) String
#define		_USE_CHAR_STRING	// CP_949

// 6) Lock
#define		_USE_NATIVE_LOCK


//-----------------------------------------------------------------------------
// common
//-----------------------------------------------------------------------------
#include "common/definitions.h"
#include "common/definitions.assert.h"
#include "common/lockable.h"
#include "common/scoped_lock.h"
#include "common/definition.timer.h"
#include "common/definitions.message.h"
#include "common/message_map.h"
