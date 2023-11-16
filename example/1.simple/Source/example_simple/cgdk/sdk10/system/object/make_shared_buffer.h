//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                             Archive Classes                               *
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
// make_shared_buffer
//
//-----------------------------------------------------------------------------
template <class T, class E = void>
constexpr void _do_post_make_shared_buffer(CGDK::_shared_buffer<buffer>&)
{
}

//-----------------------------------------------------------------------------
//
// alloc_shared_buffer
//
//
//-----------------------------------------------------------------------------
inline _shared_buffer<buffer> _alloc_shared_buffer(std::size_t _size CGNEW_DEBUG_INFO_COMMA CGNEW_DEBUG_INFO_PARAMETERS_WITH_DEFAULT)
{
	// 1) alloc memory object
	_shared_buffer<buffer> buf_serialized;

	// 2) set memory source
	buf_serialized = _mem_alloc(_size CGNEW_DEBUG_INFO_COMMA CGNEW_DEBUG_INFO_PARAMETERS_PASSING);

	// 3) set bound info
	buf_serialized.bound.upper = reinterpret_cast<const char*>(buf_serialized.bound.lower) + _size;

	// return) 
	return	buf_serialized;
}

template <class T>
_shared_buffer<buffer> _make_shared_buffer(CGNEW_DEBUG_INFO_PARAMETERS CGNEW_DEBUG_INFO_COMMA const T& _data)
{
	// 1) allocate shared_buffer
	auto buf_serialized = _alloc_shared_buffer(get_size_of(_data) CGNEW_DEBUG_INFO_COMMA CGNEW_DEBUG_INFO_PARAMETERS_PASSING);
	
	// 2) append _data
	buf_serialized.template append<std::remove_reference_t<std::remove_cv_t<T>>>(_data);

	// 3) do post_process (optional)
	_do_post_make_shared_buffer<T>(buf_serialized);

	// return) 
	return	buf_serialized;
}

template <class ELEM_T>
template<class BUFFER_T>
constexpr _shared_buffer<BUFFER_T> _buffer_view<ELEM_T>::_extract_shared_buffer()
{
	// declare)
	_shared_buffer<BUFFER_T> buf_temp;

	// 1) get length of data
	std::size_t length = this->_extract<size_type>();

	// check)
	if (length == 0)
		return buf_temp;

	// 2) alloc shared buffer
	buf_temp = alloc_shared_buffer(length);

	// 3) append data
	buf_temp.append(length, data());

	// 4) extract bytes
	this->_extract_bytes(length);

	// return) 
	return	buf_temp;
}


}