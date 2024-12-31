//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                          network socket classes                           *
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
// definition_message_header
//
//-----------------------------------------------------------------------------
struct message_headerable
{
	static void			_set_message_size(buffer& _buffer) noexcept { _buffer.front<uint32_t>() = _buffer.size<uint32_t>(); }
	static std::size_t	_get_message_size(const buffer_view& _buffer) noexcept { return *_buffer.data<uint32_t>(); }
	static bool			_validate_message(const buffer_view& _buffer) noexcept
	{
		// 1) message 크기를 확인한다.
		const char* data_ = _buffer.data();
		std::size_t size_ = _buffer.size();

		while(size_ != 0)
		{
			// check) message의 크기가 0Byte면 안됀다.
			CGDK_ASSERT(size_ >= sizeof(uint32_t));
			if (size_ < sizeof(uint32_t)) return false;

			auto message_size = *reinterpret_cast<const uint32_t*>(data_);

			// check) message의 크기가 0Byte면 안됀다.
			CGDK_ASSERT(message_size != 0);
			if(message_size == 0) return false;

			// check) message의 크기가 실제 버퍼의 크기보다 작으면 안됀다.
			CGDK_ASSERT(message_size <= size_);
			if(message_size > size_) return false;

			// - 다음 message
			data_ += message_size;
			size_ -= message_size;
		}

		// return) 성공~
		return true;
	}
};

template <class T = uint32_t, std::size_t N = 0>
struct message_headable_primitive : public message_headerable
{
	static void			_set_message_size(buffer& _buffer) noexcept { _buffer.front<uint32_t>(N) = _buffer.size<uint32_t>(); }
	static std::size_t	_get_message_size(const buffer_view& _buffer) noexcept { return *_buffer.data<uint32_t>(N); }
	static bool			_validate_message(const buffer_view& _buffer) noexcept
	{
		// 1) message 크기를 확인한다.
		const char* data_ = _buffer.data();
		std::size_t size_ = _buffer.size();

		while(size_ != 0)
		{
			// check) message의 크기가 0Byte면 안됀다.
			CGDK_ASSERT(size_ >= sizeof(T) + N);
			if (size_ < sizeof(T) + N) return false;

			auto message_size = *reinterpret_cast<const T*>(data_ + N);

			// check) message의 크기가 0Byte면 안됀다.
			CGDK_ASSERT(message_size !=0);
			if(message_size ==0) return false;

			// check) message의 크기가 실제 버퍼의 크기보다 작으면 안됀다.
			CGDK_ASSERT(message_size <= size_);
			if(message_size > size_) return false;

			// - 다음 message
			data_ += message_size;
			size_ -= message_size;
		}

		// return) 성공~
		return true;
	}
};

template<class T = uint32_t, std::size_t N = 0, class E = void>
struct definition_message_header : public message_headable_primitive<T, N>
{
};

template<class T, std::size_t N>
struct definition_message_header<T, N, std::enable_if_t<std::is_base_of_v<message_headerable, T>>>
{
	static void			_set_message_size(buffer& _buffer) noexcept { T::_set_message_size(_buffer); }
	static std::size_t	_get_message_size(const buffer_view& _buffer) noexcept { return T::_get_message_size(_buffer); }
	static bool			_validate_message(const buffer_view& _buffer) noexcept { return T::_validate_message(_buffer); }
};

template <class T, std::enable_if_t<std::is_base_of_v<message_headerable, T>>>
constexpr void _do_post_make_shared_buffer(CGDK::_shared_buffer<buffer>& _source)
{
	T::_set_message_size(_source);
}


}