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

#define __CGDK_IMEMORY

namespace CGDK
{
//-----------------------------------------------------------------------------
//
// memory Base!
//
//-----------------------------------------------------------------------------
class Imemory : 
// Inherited classes)
	public						buffer_view,
	virtual public				Ireferenceable
{
// constructor/destructor)
public:
			Imemory() noexcept {}
			Imemory(char* _buf, uint32_t _len) noexcept : buffer_view(_buf, _len) {}
			Imemory(const _buffer_view<char>& _buffer) noexcept : buffer_view(_buffer) {}
	virtual	~Imemory() noexcept {}

	auto	get_lower_bound() const noexcept { return data_;};
	auto	get_upper_bound() const noexcept { return data_ + size_;};
	auto	get_bound() const noexcept { return buffer_bound{ get_lower_bound(), get_upper_bound() }; };
};

//template <class ELEM_T>
//constexpr _buffer_view<ELEM_T>& _buffer_view<ELEM_T>::operator = (Imemory* _rhs) noexcept
//{
//	if (_rhs != nullptr)
//	{
//		base_t::data_ = _rhs->data();
//		base_t::size_ = 0;
//	}
//	else
//	{
//		clear();
//	}
//
//	// return) 
//	return *this;
//}
//template <class ELEM_T>
//constexpr _buffer_view<ELEM_T>& _buffer_view<ELEM_T>::operator = (const object_ptr<Imemory>& _rhs) noexcept
//{
//	if (_rhs.exist())
//	{
//		base_t::data_ = _rhs->data<typename base_t::element_t>();
//		base_t::size_ = 0;
//	}
//	else
//	{
//		clear();
//	}
//
//	// return) 
//	return *this;
//}

object_ptr<Imemory> _mem_raw_alloc(size_t _size CGNEW_DEBUG_INFO_COMMA CGNEW_DEBUG_INFO_PARAMETERS_WITH_DEFAULT);
object_ptr<Imemory> _mem_alloc(size_t _size CGNEW_DEBUG_INFO_COMMA CGNEW_DEBUG_INFO_PARAMETERS);
_shared_buffer<buffer> _alloc_shared_buffer(std::size_t _size CGNEW_DEBUG_INFO_COMMA CGNEW_DEBUG_INFO_PARAMETERS);


inline object_ptr<Imemory> _mem_alloc(size_t _size CGNEW_DEBUG_INFO_COMMA CGNEW_DEBUG_INFO_PARAMETERS_WITH_DEFAULT)
{
	// definitions) 
	class __CREATE : public Imemory
	{
	public:
		__CREATE(size_t _size)
		{
			// 1) alloc buffer
			auto pmemory = malloc(_size);

			// check)
			ERROR_THROW_IF(pmemory == nullptr, std::bad_alloc(), );

			set_data(reinterpret_cast<char*>(pmemory));
			set_size(_size);
		}

		virtual	~__CREATE() noexcept { free(data_); }
	};

	// return) create and Return 
	return _make_object<__CREATE>(CGNEW_DEBUG_INFO_PARAMETERS_PASSING CGNEW_DEBUG_INFO_COMMA _size);
}



}