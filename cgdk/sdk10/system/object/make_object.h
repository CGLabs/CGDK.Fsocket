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
// make_object
//                                                                            
//-----------------------------------------------------------------------------
template <class TOBJECT>
std::enable_if_t<!std::is_base_of_v<CGDK::object::Idestroyable, TOBJECT>, void>
_execute_process_destroy(TOBJECT* /*_object*/)
{
}

template <class TOBJECT>
std::enable_if_t<std::is_base_of_v<CGDK::object::Idestroyable, TOBJECT>, void>
_execute_process_destroy(TOBJECT* _object)
{
	_object->destroy();
}

template <class TOBJECT, class... TARGS>
TOBJECT* _make_object_create(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA TARGS&&... _args)
{
	class OBJECT_CREATE : public TOBJECT
	{
	public:
				OBJECT_CREATE(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA TARGS&&... _nested_args) : TOBJECT(std::forward<TARGS>(_nested_args)...) {}
		virtual ~OBJECT_CREATE()	{ _execute_process_destroy<TOBJECT>(this); }

		virtual void process_dispose() noexcept override { delete this; }
	};

#pragma push_macro("new")
#undef new
	auto pobject = new OBJECT_CREATE(std::forward<TARGS>(_args)...);
#pragma pop_macro("new")

	// Debug)
	CGNEW_DEBUG_INFO_SET(pobject, _filename, _line); 

	// return)
	return pobject;
}

template <class TOBJECT>
void DEALLOC(TOBJECT* p)
{
	delete	p;
}
