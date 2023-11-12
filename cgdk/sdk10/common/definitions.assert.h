//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*                       Ver 10.0 / Release 2019.12.11                       *
//*                                                                           *
//*                              memory Classes                               *
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
// Chosanghyun's Game Basic Definitions
//-----------------------------------------------------------------------------
// 1) Assert
#include <cstdarg>
#include <cassert>


// ----------------------------------------------------------------------------
//
// Assert
//
// ----------------------------------------------------------------------------
#if defined(UE_BUILD_DEBUG) || defined(UE_BUILD_DEVELOPMENT)
	#define	_USE_CGASSERT
#endif

#ifndef NDEBUG
	#define	_USE_CGASSERT
#endif

// 1) ASSERT
#ifdef _USE_CGASSERT
	#define	CGASSERT(equation, value)				if(!(equation))	{ assert(equation); return value;}
	#define	CGASSERT_TRACE(equation, value, state)	if(!(equation))	{ state; assert(equation); return value;}
	#define	CGASSERT_ERROR(equation)				assert(equation);
	#define	CGASSERT_ERROR_TRACE(equation, state)	if(!(equation))	{ state; assert(equation);}
	#define	CGASSERT_ERROR_CRITICAL					assert(false);
#else
	#define	CGASSERT(equation, value)
	#define	CGASSERT_TRACE(equation, value, string)
	#define	CGASSERT_ERROR(equation)
	#define	CGASSERT_ERROR_TRACE(equation, string)
	#define	CGASSERT_ERROR_CRITICAL
#endif
