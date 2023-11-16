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
// _make_own<TREF>
//
//-----------------------------------------------------------------------------
template <class TOBJECT> 
std::enable_if_t<!std::is_base_of_v< object::Idestroyable, TOBJECT>, object::Idestroyable*>
_is_destroyable(TOBJECT* ) { return nullptr;}
template <class TOBJECT>
std::enable_if_t<std::is_base_of_v< object::Idestroyable, TOBJECT>, object::Idestroyable*>
_is_destroyable(TOBJECT* _p) { return _p; }

template <class TOBJECT>
std::enable_if_t<!std::is_base_of_v< Imessage_transmitter, TOBJECT>, Imessage_transmitter*>
_is_message_transmitter(TOBJECT* ) { return nullptr; }
template <class TOBJECT>
std::enable_if_t<std::is_base_of_v< Imessage_transmitter, TOBJECT>, Imessage_transmitter*>
_is_message_transmitter(TOBJECT* _p) { return _p; }

template <class TOBJECT, class... TARGS>
own_ptr<TOBJECT> _make_own_raw(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA TARGS&&... _args)
{
	class OBJECT_OWN : public TOBJECT, public Idisposable::Ndelete
	{
	public:
				OBJECT_OWN(TARGS&&... _nested_args) : TOBJECT(std::forward<TARGS>(_nested_args)...), Idisposable::Ndelete() {}
		virtual ~OBJECT_OWN() {}
	};

	// declare)
	OBJECT_OWN*	pobject = nullptr;

#pragma push_macro("new")
#undef new
		pobject = new OBJECT_OWN(std::forward<TARGS>(_args)...);
#pragma pop_macro("new")

	// Debug)
	CGNEW_DEBUG_INFO_SET(pobject, _filename, _line);

	// return)
	//return	pobject;
	return own_ptr<TOBJECT>(typename own_ptr<TOBJECT>::OWN_SET(pobject, { _is_destroyable<TOBJECT>(pobject), _is_message_transmitter<TOBJECT>(pobject) }));
}

template <class TOBJECT, class... TARGS>
own_ptr<TOBJECT> _make_own(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA TARGS&&... _args)
{
	// check) TOBJECT must inherite class'Ireferenceable'
	static_assert(std::is_base_of_v<Ireferenceable, TOBJECT>, "CGObj: TOBJECT must inherite 'Ireferenceable' class");

	// return) 
	return _make_own_raw<TOBJECT, TARGS...>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA std::forward<TARGS>(_args)...);
}

template <class TOBJECT, class... TARGS>
std::tuple<TOBJECT*, TARGS...> _make_own_parameters(TARGS&&... _args)
{
	return std::tuple<TOBJECT*, TARGS...>(nullptr, std::forward<TARGS>(_args)...);
}

template<class TOBJECT, class T>
own_ptr<TOBJECT> _make_own_tuple_begin(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA std::tuple<T>&& /*_tuple*/)
{
	return _make_own<TOBJECT>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING);
}

template<class TOBJECT, class T, class... TARGS>
own_ptr<TOBJECT> _make_own_tuple(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA std::tuple<T>&& _tuple, TARGS&&... _args)
{
	return _make_own<TOBJECT>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA std::forward<TARGS>(_args)..., std::forward<T>(std::get<0>(_tuple)));
}

template<class TOBJECT, class T1, class T2, class... TREST, class... TARGS>
own_ptr<TOBJECT> _make_own_tuple(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA std::tuple<T1, T2, TREST...>&& _tuple, TARGS&&... _args)
{
	return _make_own_tuple<TOBJECT>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA std::move((std::tuple<T2, TREST...>&)_tuple), std::forward<TARGS>(_args)..., std::forward<T1>(std::get<0>(_tuple)));
}

template<class TOBJECT, class T>
own_ptr<TOBJECT> _make_own_tuple(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA std::tuple<T>&& _tuple)
{
	return _make_own<TOBJECT>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA std::forward<T>(std::get<0>(_tuple)));
}

template<class TOBJECT, class T1, class T2, class... TREST>
own_ptr<TOBJECT> _make_own_tuple(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA std::tuple<T1, T2, TREST...>&& _tuple)
{
	return _make_own_tuple<TOBJECT>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA std::move((std::tuple<T2, TREST...>&)_tuple), std::forward<T1>(std::get<0>(_tuple)));
}

template<class TOBJECT, class T1, class T2, class... TREST>
own_ptr<TOBJECT> _make_own_tuple_begin(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA std::tuple<T1, T2, TREST...>&& _tuple)
{
	return _make_own_tuple<TOBJECT>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA std::move((std::tuple<T2, TREST...>&)_tuple));
}


}