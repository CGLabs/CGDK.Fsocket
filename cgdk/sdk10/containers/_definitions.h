//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*                       Ver 6.1 / Release 2012.05.28                        *
//*                                                                           *
//*                           Data Template Classes                           *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*  This Program is programmed by Cho SangHyun. sangducks@gmail.com          *
//*  Best for Game Developement and Optimized for Game Developement.          *
//*                                                                           *
//*                   (C) 2008 CGLabs All right reserved.                     *
//*                           http://www.CGDK.co.kr                           *
//*                                                                           *
//*****************************************************************************

#pragma once
//-----------------------------------------------------------------------------
//
// CGDK Definitions
//
// 1. circular란!
//    - 환영으로 동작하는 Array이다.
//    - list와 같은 상수시간 양방향 push/pop을 지원한다.
//    - array와 같은 Random Indexing을 제공한다.
//    - std::deque의 대체용이라고 보면 된다.
//    - 일반적인 모든 처리속도는 vector와 같은 수준이다.
//    - 단점은 expend시 엄청난 부하가 든다는 단점이 있다.
//    - size는 항상 2의 배수로만 설정할 수 있다.
//
// 2. circular의 핵심! Function의 기능 설명.
//    1) 
//    2) 
//
// 3. circular의 주요 사용 용도
//    1) vector를 대체 - 양방향 push/pop이 필요하면서 Random access가 필요할 때.
//    2) deque를 대체  - 양방향 push/pop이 필요하면서 매우 빈번한 iteration이 필요할 때.
//
//
// 4. Option) CGDK::vector의 주요 옵션.
// 
//    1) No auto expand on push_back operation
//
//		 #define _CGD_CIRCULAR_NO_AUTOEXPAND
//
//       vector의 _CGD_VECTOR_NO_AUTOEXPAND_PUSHBACK와 같은 역할을 하는 옵션이다.
//
//
//-----------------------------------------------------------------------------
#include <stdexcept>
#include <limits>
#include <cstddef>
#include <cassert>
//#define	_CGD_NO_EXCEPTION_GUARANTEE

// ----------------------------------------------------------------------------
// Exception Guarantee Levels) 
// ----------------------------------------------------------------------------
#ifdef _CGD_NO_EXCEPTION_GUARANTEE
#define	ON_EXCEPTION(condition)		if(FALSE)

#define	CGD_TRY			
#define	CGD_CATCH(x)	
#define	CGD_CATCH_ALL	if(FALSE)
#define	CGD_RETHROW

#define	CGD_THROW(x)	
#define	CGD_THROW_IF(c, r)	CGDASSERT_ERROR(!(c));

#else
#define	ON_EXCEPTION(condition)	if(condition)

#define	CGD_TRY					try
#define	CGD_CATCH(x)			catch (x)
#define	CGD_CATCH_ALL			catch (...)
#define	CGD_RETHROW				throw

#define	CGD_THROW(x)			throw(x)
#define	CGD_THROW_IF(c, r)		if(c){	throw r;}

#endif


#define	CGD_NOEXCEPT			noexcept


#define	LINK_NODE(a, b)			a->_next=b; b->_pre=a;
#define	LINK_NODE_SINGLE(a, b)	a->_next=b;

#if defined(_MSC_VER)
	#define	TLS					__declspec( thread )
#else
	#define TLS					__thread
#endif


// ----------------------------------------------------------------------------
// Definitions
// ----------------------------------------------------------------------------
#define	CONTAINER_MIN	8
#define	CONTAINER_MAX	65536

// 1) CGDK::list Sort Option. 
#define	_CGD_LIST_FAST_SORT



//*****************************************************************************
//*                                                                           *
//*                               Definitions                                 *
//*                                                                           *
//*  Definition for Global use of Application                                 *
//*                                                                           *
//*****************************************************************************
#ifndef SAFE_FREE
	#define	SAFE_FREE(p)									if(p){ free(p);p=nullptr;}
#endif

#ifndef SAFE_DELETE
	#define	SAFE_DELETE(p)									if(p){ delete p; p=nullptr;}
#endif

#ifndef SAFE_DELETE_ARRAY
	#define	SAFE_DELETE_ARRAY(p)							if(p){ delete [] p; p=nullptr;}
#endif

#ifndef SAFE_RELEASE
	#define	SAFE_RELEASE(data)								if(data){ (data)->Release();(data)=nullptr;}
#endif


// ----------------------------------------------------------------------------
// Trace & Assert
// ----------------------------------------------------------------------------
#ifndef NDEBUG
	#define	_USE_CGDNET_ASSERT
#endif

#ifdef _USE_CGDNET_ASSERT
	#define	CGDASSERT(equation, value)						assert(equation);	if(!(equation))	{ return value;}
	#define	CGDASSERT_TRACE(equation, value, state)			assert(equation);	if(!(equation))	{ state; return value;}
	#define	CGDASSERT_ERROR(equation)						assert(equation);
	#define	CGDASSERT_ERROR_TRACE(equation, state)			assert(equation);	if(!(equation))	{ state;}

	#define	CGDASSERT_ERROR_CRITICAL						assert(FALSE);
#else
	#define	CGDASSERT(equation, value)					
	#define	CGDASSERT_TRACE(equation, value, string)
	#define	CGDASSERT_ERROR(equation)					
	#define	CGDASSERT_ERROR_TRACE(equation, string)

	#define	CGDASSERT_ERROR_CRITICAL
#endif


// ----------------------------------------------------------------------------
// 제어문
// ----------------------------------------------------------------------------
// 1) 제어문 Serise
#define	CGD_CONTINUE_IF(equation)							if(equation)			{ continue;}
#define	CGD_BREAK_IF(equation)								if(equation)			{ break;}
#define	CGD_RETURN_IF(equation, value)						if(equation)			{ return value;}


// ----------------------------------------------------------------------------
// 연산
// ----------------------------------------------------------------------------
// 1) 음...
#define	CGD_MAX(a, b)										(((a) > (b)) ? (a) : (b))
#define	CGD_MIN(a, b)										(((a) < (b)) ? (a) : (b))

// 2) SATURATE
#define	CGD_SATURATE_MAX(value, max)						if(value > (max)) value=(max);
#define	CGD_SATURATE_MIN(value, min)						if(value < (min)) value=(min);
#define	CGD_SATURATE(value, min, max)						if(value < (min)) value=(min); else if(value > (max)) value=(max);

// 3) Swap
template<typename TA>
void	CGD_SWAP(TA& a, TA& b)								{	TA t=a; a=b; b=t;}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
#define	_ACQUIRES_LOCK(value)				_Acquires_lock_(value)
#define	_RELEASES_LOCK(value)				_Releases_lock_(value)
#define	_ACQUIRES_EXCLUSIVE_LOCK(value)		_Acquires_exclusive_lock_(value)
#define	_RELEASES_EXCLUSIVE_LOCK(value)		_Releases_exclusive_lock_(value)
#define	_ACQUIRES_SHARED_LOCK(value)		_Acquires_shared_lock_(value)
#define	_RELEASES_SHARED_LOCK(value)		_Releases_shared_lock_(value)

#pragma warning(disable: 26110)
#pragma warning(disable: 26135)
#pragma warning(disable: 26165)
