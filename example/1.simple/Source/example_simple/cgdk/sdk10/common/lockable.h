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

namespace CGDK
{
//-----------------------------------------------------------------------------
//
// definition
//
//-----------------------------------------------------------------------------
typedef std::recursive_mutex	lock_object;


//-----------------------------------------------------------------------------
//
// lockable
//
//-----------------------------------------------------------------------------
template <class TOBJECT=void, class TLOCK = std::recursive_mutex>
class lockable : public TOBJECT, public TLOCK
{
public:
	template<class... TARGS>
	lockable(TARGS&&... _arguments) : TOBJECT(std::forward<TARGS>(_arguments)...) {}

public:
			template <class TDATA>
			TOBJECT&			operator=( const TDATA& _data ) noexcept		{ *(TOBJECT*)this=_data; return *this; }
			template <class TDATA>
			TOBJECT&			operator=(TDATA&& _data ) noexcept				{ *(TOBJECT*)this=std::forward<TDATA>(_data); return *this; }

			template <class TDATA>
			bool				operator==(const TDATA& _rhs) const noexcept	{ return *(TOBJECT*)this == _rhs; }
			template <class TDATA>
			bool				operator!=(const TDATA& _rhs) const noexcept	{ return *(TOBJECT*)this != _rhs; }
			template <class TDATA>
			bool				operator> (const TDATA& _rhs) const noexcept	{ return *(TOBJECT*)this >  _rhs; }
			template <class TDATA>
			bool				operator>=(const TDATA& _rhs) const noexcept	{ return *(TOBJECT*)this >= _rhs; }
			template <class TDATA>
			bool				operator< (const TDATA& _rhs) const noexcept	{ return *(TOBJECT*)this <  _rhs; }
			template <class TDATA>
			bool				operator<=(const TDATA& _rhs) const noexcept	{ return *(TOBJECT*)this <= _rhs; }

			operator lockable<void, TLOCK>&() noexcept							{ return *reinterpret_cast<lockable<void, TLOCK>*>(static_cast<TLOCK*>(this)); }
};

template<class TLOCK>
class lockable<void, TLOCK> : public TLOCK
{
};


}