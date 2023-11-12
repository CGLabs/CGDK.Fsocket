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
// Definitions
//                                                                            
//-----------------------------------------------------------------------------
#define	INLINE										inline

#ifndef PURE
	#define	PURE									= 0
#endif

#ifndef EMPTY
	#define	EMPTY									{}
#endif

#ifndef EMPTY_TRUE
	#define	EMPTY_TRUE								{	return true;}
#endif

#ifndef EMPTY_FALSE
	#define	EMPTY_FALSE								{	return false;}
#endif

#ifndef EMPTY_ZERO
	#define	EMPTY_ZERO								{	return 0;}
#endif

#ifndef SAFE_FREE
	#define	SAFE_FREE(p)							if(p){ free(p);p=nullptr;}
#endif

#ifndef SAFE_DELETE
	#define	SAFE_DELETE(p)							if(p){ delete p; p=nullptr;}
#endif

#ifndef SAFE_DELETE_ARRAY
	#define	SAFE_DELETE_ARRAY(p)					if(p){ delete [] p; p=nullptr;}
#endif

#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p)					{ if(p) { delete[] (p);   (p) = nullptr; } }
#endif

#ifndef SAFE_DESTROY
	#define	SAFE_DESTROY(data)						if(data!=nullptr){ data->destroy(); data=nullptr;}
#endif

#ifndef SAFE_RELEASE
	#define	SAFE_RELEASE(data)						if(data){ (data)->release();(data)=nullptr;}
#endif

#ifndef SAFE_CLOSEHANDLE
	#define	SAFE_CLOSEHANDLE(data)					if(data!=INVALID_CGHANDLE_VALUE){ CloseHandle(data);data=INVALID_CGHANDLE_VALUE;}
#endif


// ----------------------------------------------------------------------------
// check
// ----------------------------------------------------------------------------
// 1) control State
#define	CONTINUE_IF(equation)									if(equation) { continue;}
#define	DO_AND_CONTINUE_IF(equation, todo)						if(equation) { todo; continue;}
#define	BREAK_IF(equation)										if(equation) { break;}
#define	THROW_IF(equation, value)								if(equation) { throw value;}
#define	RETURN_IF(equation, value)								if(equation) { return value;}

// 2) control State with STATE
#define	ERROR_IF(equation, state)								if(equation) { state;}
#define	ERROR_CONTINUE_IF(equation, state)						if(equation) { state; continue;}
#define	ERROR_BREAK_IF(equation, state)							if(equation) { state; break;}
#define	ERROR_THROW(value, state)								{state; throw value;}
#define	ERROR_THROW_IF(equation, value, state)					if(equation) { state; throw value;}
#define	ERROR_RETURN_IF(equation, value, state)					if(equation) { state; return value;}

// 3) Conditional
#define	CGIF(condition, state)									if(condition) { state;}

// 4) Debug Only
#if (defined(_WIN32) && !defined(NDEBUG)) || (!defined(_WIN32) && defined(NDEBUG))
	#define	CGDEBUG_ONLY(a)										a
#else
	#define	CGDEBUG_ONLY(a)
#endif


// ----------------------------------------------------------------------------
// Simple Operations
// ----------------------------------------------------------------------------
// 1) Settings
#define	SET_ZERO(variable)										variable=0
#define SET_ZERO_VECTOR3(variable)								variable.x=variable.y=variable.z=0.0f;
#define SET_ZERO_VECTOR4(variable)								variable.x=variable.y=variable.z=variable.w=0.0f;
#define	SET_NULL(variable)										variable=nullptr
#define	SET_TRUE(variable)										variable=true
#define	SET_FALSE(variable)										variable=false

// 2) Bit operations
// Toggle(true는 false로 false는 true로 바꿉니다.)
#define	TOGGLE(value)											(value) = !(value)

// 3) Simple Operations
template <typename T> T __select_max(T a, T b)					{return (a>b) ? a:b;}
template <typename T> T __select_max(T a, T b, T c)				{return (a>b) ? ((a>c) ? a:c) : ((b>c) ? b:c);}
template <typename T> T __select_max(T a, T b, T c, T d)		{return __select_max(__select_max(a,b), __select_max(c,d));}
template <typename T> T __select_min(T a, T b)					{return (a<b) ? a:b;}
template <typename T> T __select_min(T a, T b, T c)				{return (a<b) ? ((a<c) ? a:c) : ((b<c) ? b:c);}
template <typename T> T __select_min(T a, T b, T c, T d)		{return __select_min(__select_min(a,b), __select_min(c,d));}
template <typename T> T __get_saturated(T _v, T _min, T _max)	{return (_v<_min) ? _min : ((_v>_max) ? _max : _v);}
#define	UPDATE_MAX(a, b)										if(a<(b)) {a=(b);}
#define	UPDATE_MIN(a, c)										if(a>(c)) {a=(c);}
#define	SATURATE_MAX(a, c)										if(a>(c)) a=(c);
#define	SATURATE_MIN(a, b)										if(a<(b)) a=(b);
#define	SATURATE(a, b, c)										if(a<(b)) a=(b); else if(a>(c)) a=(c);
#define	ROUND_MAX(a, c)											if(a>(c)) a-=(c);
#define	ROUND_MIN(a, b)											if(a<(b)) a+=(b);
#define	ROUND_MINIMAX(a, b, c)									if(a<(b)) a+=(b); else if(a>(c)) a-=(c);
#define	UINT_DIFFER(a, b)										(a>=b) ? (a-b) : (UINT_MAX-a+b);

// 4) SWAP
template <typename TA>
void	SWAP(TA& a, TA& b)										{TA t=std::move(a); a=std::move(b); b=std::move(t);}

// 5) Increase/Decrese
#define	INCREASE(value)											++value
#define	INCREASE_IF(condition, value)							if(condition) {++value;}
#define	DECREASE(value)											--value
#define	DECREASE_IF(condition, value)							if(condition) {--value;}
#define	DISCOUNT_TO_ZERO(value)									if(value > 0) {--value;}

// 7) 계산 관련
#define	SQURE(value)											(value*value)
#define	VECTOR3_DISTANCE_SQURE(value)							(value.x*value.x+value.y*value.y+value.z*value.z)

// 8) Unique name
#define _MERGE_(a,b)  a##b
#define _LABEL_(a,b) _MERGE_(a,b)
#define _UNIQUE_TEMPORARY_NAME(name) _LABEL_(name, __LINE__)

// 9)...
#define _AT(_array, _pos)										(*(_array + (_pos)))


// ----------------------------------------------------------------------------
// for Enum
// ----------------------------------------------------------------------------
#define CGDEFINE_ENUM_FLAG_OPERATORS_UINT32(ENUMTYPE) \
extern "C++" { \
	inline ENUMTYPE		operator |	(ENUMTYPE  _lhs, ENUMTYPE _rhs) noexcept	{ return static_cast<ENUMTYPE>		(static_cast<uint32_t>(_lhs) | static_cast<uint32_t>(_rhs)); } \
	inline ENUMTYPE&	operator |= (ENUMTYPE& _lhs, ENUMTYPE _rhs) noexcept	{ return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<uint32_t&>(_lhs) |= static_cast<uint32_t>(_rhs)); } \
	inline ENUMTYPE		operator &	(ENUMTYPE  _lhs, ENUMTYPE _rhs) noexcept	{ return static_cast<ENUMTYPE>		(static_cast<uint32_t>(_lhs) & static_cast<uint32_t>(_rhs)); } \
	inline ENUMTYPE&	operator &=	(ENUMTYPE& _lhs, ENUMTYPE _rhs) noexcept	{ return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<uint32_t&>(_lhs) &= static_cast<uint32_t>(_rhs)); } \
	inline ENUMTYPE		operator ~	(ENUMTYPE  _lhs) noexcept					{ return static_cast<ENUMTYPE>		(~static_cast<uint32_t>(_lhs)); } \
	inline ENUMTYPE		operator ^	(ENUMTYPE  _lhs, ENUMTYPE _rhs) noexcept	{ return static_cast<ENUMTYPE>		(static_cast<uint32_t>(_lhs) ^ static_cast<uint32_t>(_rhs)); } \
	inline ENUMTYPE&	operator ^= (ENUMTYPE& _lhs, ENUMTYPE _rhs) noexcept	{ return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<uint32_t&>(_lhs) ^= static_cast<uint32_t>(_rhs)); } \
	inline ENUMTYPE		operator |	(ENUMTYPE  _lhs, uint32_t _rhs) noexcept	{ return static_cast<ENUMTYPE>		(static_cast<uint32_t>(_lhs) | _rhs); } \
	inline ENUMTYPE&	operator |= (ENUMTYPE& _lhs, uint32_t _rhs) noexcept	{ return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<uint32_t&>(_lhs) |= _rhs); } \
	inline ENUMTYPE		operator &	(ENUMTYPE  _lhs, uint32_t _rhs) noexcept	{ return static_cast<ENUMTYPE>		(static_cast<uint32_t>(_lhs) & _rhs); } \
	inline ENUMTYPE&	operator &=	(ENUMTYPE& _lhs, uint32_t _rhs) noexcept	{ return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<uint32_t&>(_lhs) &= _rhs); } \
	inline ENUMTYPE		operator ^	(ENUMTYPE  _lhs, uint32_t _rhs) noexcept	{ return static_cast<ENUMTYPE>		(static_cast<uint32_t>(_lhs) ^ _rhs); } \
	inline ENUMTYPE&	operator ^=	(ENUMTYPE& _lhs, uint32_t _rhs) noexcept	{ return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<uint32_t&>(_lhs) ^= _rhs); } \
}

#define CGDEFINE_ENUM_FLAG_OPERATORS_UINT64(ENUMTYPE) \
extern "C++" { \
	inline ENUMTYPE		operator |	(ENUMTYPE  _lhs, ENUMTYPE _rhs) noexcept	{ return static_cast<ENUMTYPE>		(static_cast<uint64_t>(_lhs) | static_cast<uint64_t>(_rhs)); } \
	inline ENUMTYPE&	operator |= (ENUMTYPE& _lhs, ENUMTYPE _rhs) noexcept	{ return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<uint64_t&>(_lhs) |= static_cast<uint64_t>(_rhs)); } \
	inline ENUMTYPE		operator &	(ENUMTYPE  _lhs, ENUMTYPE _rhs) noexcept	{ return static_cast<uint64_t>		(static_cast<uint64_t>(_lhs) & static_cast<uint64_t>(_rhs)); } \
	inline ENUMTYPE&	operator &=	(ENUMTYPE& _lhs, ENUMTYPE _rhs) noexcept	{ return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<uint64_t&>(_lhs) &= static_cast<uint64_t>(_rhs)); } \
	inline ENUMTYPE		operator ~	(ENUMTYPE  _lhs) noexcept			 		{ return static_cast<ENUMTYPE>		(~static_cast<uint64_t>(_lhs)); } \
	inline ENUMTYPE		operator ^	(ENUMTYPE  _lhs, ENUMTYPE _rhs) noexcept	{ return static_cast<ENUMTYPE>		(static_cast<uint64_t>(_lhs) ^ static_cast<uint64_t>(_rhs)); } \
	inline ENUMTYPE&	operator ^= (ENUMTYPE& _lhs, ENUMTYPE _rhs) noexcept	{ return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<uint64_t&>(_lhs) ^= static_cast<uint64_t>(_rhs)); } \
	inline ENUMTYPE		operator |	(ENUMTYPE  _lhs, uint64_t _rhs) noexcept	{ return static_cast<ENUMTYPE>		(static_cast<uint64_t>(_lhs) | _rhs); } \
	inline ENUMTYPE&	operator |= (ENUMTYPE& _lhs, uint64_t _rhs) noexcept	{ return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<uint64_t&>(_lhs) |= _rhs); } \
	inline ENUMTYPE		operator &	(ENUMTYPE  _lhs, uint64_t _rhs) noexcept	{ return static_cast<ENUMTYPE>		(static_cast<uint64_t>(_lhs) & _rhs); } \
	inline ENUMTYPE&	operator &=	(ENUMTYPE& _lhs, uint64_t _rhs) noexcept	{ return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<uint64_t&>(_lhs) &= _rhs); } \
	inline ENUMTYPE		operator ^	(ENUMTYPE  _lhs, uint64_t _rhs) noexcept	{ return static_cast<ENUMTYPE>		(static_cast<uint64_t>(_lhs) ^ _rhs); } \
	inline ENUMTYPE&	operator ^=	(ENUMTYPE& _lhs, uint64_t _rhs) noexcept	{ return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<uint64_t&>(_lhs) ^= _rhs); } \
}
