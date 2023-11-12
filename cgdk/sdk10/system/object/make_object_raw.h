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
// _make_object_raw<TREF>
//
//-----------------------------------------------------------------------------
template <class TOBJECT, class... TARGS>
object_ptr<TOBJECT> _make_object_raw(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA TARGS&&... _args)
{
	class OBJECT_CREATE : public TOBJECT
	{
	public:
				OBJECT_CREATE(TARGS&&... _nested_args) : TOBJECT(std::forward<TARGS>(_nested_args)...), Idisposable::Ndelete() {}
		virtual ~OBJECT_CREATE() noexcept { _execute_process_destroy<TOBJECT>(this);}

		virtual void process_dispose() noexcept override { delete this; }
	};

	// declare)
	OBJECT_CREATE* pobject = nullptr;

#pragma push_macro("new")
#undef new
		pobject = new OBJECT_CREATE(std::forward<TARGS>(_args)...);
#pragma pop_macro("new")

	// Debug)
	CGNEW_DEBUG_INFO_SET(pobject, _filename, _line);

	// return)
	return pobject;
}

template <class TOBJECT, class... TARGS>
object_ptr<TOBJECT> _make_object(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA TARGS&&... _args)
{
	// check) TOBJECT must inherite class'Ireferenceable'
	static_assert(std::is_base_of_v<Ireferenceable, TOBJECT>, "CGObj: TOBJECT must inherite 'Ireferenceable' class");

	// return) 
	return _make_object_create<TOBJECT, TARGS...>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA std::forward<TARGS>(_args)...);
}

template <class TOBJECT, class... TARGS>
std::tuple<TOBJECT*, TARGS...> _make_create_parameters(TARGS&&... _args)
{
	return std::tuple<TOBJECT*, TARGS...>(nullptr, std::forward<TARGS>(_args)...);
}

template<class TOBJECT, class T>
object_ptr<TOBJECT> _make_object_tuple_begin(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA std::tuple<T>&& /*_tuple*/)
{
	return _make_object<TOBJECT>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING);
}

template<class TOBJECT, class T, class... TARGS>
object_ptr<TOBJECT> _make_object_tuple(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA std::tuple<T>&& _tuple, TARGS&&... _args)
{
	return _make_object<TOBJECT>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA std::forward<TARGS>(_args)..., std::forward<T>(std::get<0>(_tuple)));
}

template<class TOBJECT, class T1, class T2, class... TREST, class... TARGS>
object_ptr<TOBJECT> _make_object_tuple(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA std::tuple<T1, T2, TREST...>&& _tuple, TARGS&&... _args)
{
	return _make_object_tuple<TOBJECT>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA std::move((std::tuple<T2, TREST...>&)_tuple), std::forward<TARGS>(_args)..., std::forward<T1>(std::get<0>(_tuple)));
}

template<class TOBJECT, class T>
object_ptr<TOBJECT> _make_object_tuple(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA std::tuple<T>&& _tuple)
{
	return _make_object<TOBJECT>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA std::forward<T>(std::get<0>(_tuple)));
}

template<class TOBJECT, class T1, class T2, class... TREST>
object_ptr<TOBJECT> _make_object_tuple(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA std::tuple<T1, T2, TREST...>&& _tuple)
{
	return _make_object_tuple<TOBJECT>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA std::move((std::tuple<T2, TREST...>&)_tuple), std::forward<T1>(std::get<0>(_tuple)));
}

template<class TOBJECT, class T1, class T2, class... TREST>
object_ptr<TOBJECT> _make_object_tuple_begin(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA std::tuple<T1, T2, TREST...>&& _tuple)
{
	return _make_object_tuple<TOBJECT>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA std::move((std::tuple<T2, TREST...>&)_tuple));
}


}