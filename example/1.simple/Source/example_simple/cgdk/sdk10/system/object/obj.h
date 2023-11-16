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

namespace CGDK
{
//-----------------------------------------------------------------------------
//                                                                            
// OBJ<T>
//                                                                            
//-----------------------------------------------------------------------------
template<class TOBJECT, class FLAG = void>
class OBJ :
	public	Nreferenceable_no_act,
	public	TOBJECT
{
public:
	OBJ() noexcept : Nreferenceable_no_act(), TOBJECT() {}
	OBJ(const TOBJECT& _copy) noexcept : Nreferenceable_no_act(), TOBJECT(_copy) {}
	OBJ(TOBJECT&& _move) noexcept : Nreferenceable_no_act(), TOBJECT(std::move(_move)) {}
	template <class T, class... TARGS>
	OBJ(T&& _first, TARGS&&... _args) : Nreferenceable_no_act(), TOBJECT(std::forward<T>(_first), std::forward<TARGS>(_args)...) {}
	OBJ(const OBJ<TOBJECT,FLAG>& _copy) : Nreferenceable_no_act(), TOBJECT(static_cast<TOBJECT&>(_copy)) {}
	OBJ(OBJ<TOBJECT, FLAG>&& _move) : Nreferenceable_no_act(), TOBJECT(std::move(static_cast<TOBJECT&>(_move))) {}
	virtual ~OBJ() noexcept { _process_on_final_release();}

public:
	template<class NTYPE>
	OBJ<TOBJECT>&		operator=(const NTYPE& _rhs) { TOBJECT::operator=(_rhs); return *this; }
	template<class NTYPE>
	OBJ<TOBJECT>&		operator=(NTYPE&& _rhs) { TOBJECT::operator=(std::forward<NTYPE>(_rhs)); return *this; }
	OBJ<TOBJECT>&		operator=(const OBJ<TOBJECT, FLAG>& _rhs) { TOBJECT::operator=(static_cast<TOBJECT&>(_rhs)); return *this; }
	OBJ<TOBJECT>&		operator=(OBJ<TOBJECT, FLAG>&& _rhs) { TOBJECT::operator=(std::move(static_cast<TOBJECT&>(_rhs))); return *this; }

	template<class NTYPE>
	bool				operator==(const NTYPE& _rhs) const noexcept { return TOBJECT::operator==(_rhs); }
	template<class NTYPE>
	bool				operator!=(const NTYPE& _rhs) const noexcept { return TOBJECT::operator!=(_rhs); }
	template<class NTYPE>
	bool				operator>=(const NTYPE& _rhs) const noexcept { return TOBJECT::operator>=(_rhs); }
	template<class NTYPE>
	bool				operator> (const NTYPE& _rhs) const noexcept { return TOBJECT::operator> (_rhs); }
	template<class NTYPE>
	bool				operator<=(const NTYPE& _rhs) const noexcept { return TOBJECT::operator<=(_rhs); }
	template<class NTYPE>
	bool				operator< (const NTYPE& _rhs) const noexcept { return TOBJECT::operator< (_rhs); }
};

template<class TOBJECT>
class OBJ<TOBJECT, std::enable_if_t<std::is_base_of_v<CGDK::object::Idestroyable, TOBJECT>>> :
	public	Nreferenceable_no_act,
	public	TOBJECT
{
public:
	OBJ() : Nreferenceable_no_act(), TOBJECT() {}
	OBJ(const TOBJECT& _copy) : Nreferenceable_no_act(), TOBJECT(_copy) {}
	template <class T, class... TARGS>
	OBJ(T&& _first, TARGS&&... _args) : Nreferenceable_no_act(), TOBJECT(std::forward<T>(_first), std::forward<TARGS>(_args)...) {}
	virtual ~OBJ() { TOBJECT::destroy(); _process_on_final_release(); }

public:
	template<class NTYPE>
	OBJ<TOBJECT>&		operator=(const NTYPE& _rhs) { TOBJECT::operator=(_rhs); return *this; }
	template<class NTYPE>
	OBJ<TOBJECT>&		operator=(NTYPE&& _rhs) { TOBJECT::operator=(std::forward<NTYPE>(_rhs)); return *this; }

	template<class NTYPE>
	bool				operator==(const NTYPE& _rhs) const noexcept { return TOBJECT::operator==(_rhs); }
	template<class NTYPE>
	bool				operator!=(const NTYPE& _rhs) const noexcept { return TOBJECT::operator!=(_rhs); }
	template<class NTYPE>
	bool				operator>=(const NTYPE& _rhs) const noexcept { return TOBJECT::operator>=(_rhs); }
	template<class NTYPE>
	bool				operator> (const NTYPE& _rhs) const noexcept { return TOBJECT::operator> (_rhs); }
	template<class NTYPE>
	bool				operator<=(const NTYPE& _rhs) const noexcept { return TOBJECT::operator<=(_rhs); }
	template<class NTYPE>
	bool				operator< (const NTYPE& _rhs) const noexcept { return TOBJECT::operator< (_rhs); }
};


}