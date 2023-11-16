//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                              object classes                               *
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
// _CGTRAITS_PTR
//                                                                            
//-----------------------------------------------------------------------------
template <class TREF>
class _CGTRAITS_PTR
{
public:
	using value_type = typename std::remove_reference_t<std::remove_const_t<TREF>>;
};

#define	CGNEW_DEBUG_INFO_INITIALIZE
#define	CGNEW_DEBUG_INFO_PARAMETERS						
#define	CGNEW_DEBUG_INFO_PARAMETERS_WITH_DEFAULT
#define	CGNEW_DEBUG_INFO_PARAMETERS_PASSING				
#define	CGNEW_DEBUG_INFO_PARAMETERS_INITIAL
#define	CGNEW_DEBUG_INFO_PARAMETERS_INITIAL_2
#define	CGNEW_DEBUG_INFO_PARAMETERS_MALLOC
#define	CGNEW_DEBUG_INFO_COMMA							
#define	CGNEW_DEBUG_INFO_COLON
#define	CGNEW_DEBUG_INFO_SET(pobject, filename, line)	
#define	CGNEW_DEBUG_INFO_RESET(pobject)					
#define	CGNEW_DEBUG_INFO_DECLARE						public: void set_debug_info(const char* , int )	{}	\
														void reset_debug_info()							{}


//-----------------------------------------------------------------------------
/**
@class		make_object
*/
//-----------------------------------------------------------------------------
#define	MALLOC											malloc
#define	make_object										CGDK::_make_object
#define	make_own										CGDK::_make_own
#define	alloc_object									CGDK::_alloc_object
#define	mem_raw_alloc(size)								CGDK::_mem_raw_alloc(size)
#define	mem_alloc(size)									CGDK::_mem_alloc(size)
#define	mem_alloc_with_debug_info(size, f, l)			CGDK::_mem_alloc(size)
#define	alloc_shared_buffer(size)						CGDK::_alloc_shared_buffer(size)
#define	MEM_POOL_ALLOC_WITH_DEBUG_INFO(size, f, l)		CGDK::_alloc_shared_buffer(size)
#define	make_shared_buffer(value, ...)					CGDK::_make_shared_buffer(value, ##__VA_ARGS__)
