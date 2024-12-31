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
// net::io::sender::Nheader
// 
//-----------------------------------------------------------------------------
template <class HEADER_T = uint32_t, class E = void>
class net::io::sender::Nheader : virtual public net::io::Isender
{
public:
	template <class T>
	typename std::enable_if_t<std::is_base_of<HEADER_T, T>::value, bool>
								send( const T& _data, uint64_t _option = 0)
	{
		return net::io::Isender::send(make_shared_buffer(_data), _option);
	}
	template <class T>
	typename std::enable_if_t<!std::is_base_of<HEADER_T, T>::value, bool>
		send(const T& _data, uint64_t _option = 0)
	{
		return net::io::Isender::send(_data, _option);
	}
};

template <class T>
class net::io::sender::Nheader<T, std::enable_if_t<!std::is_class_v<T>>> : virtual public net::io::Isender
{
};


//-----------------------------------------------------------------------------
//
// net::io::sender::Nstream_header
// 
//-----------------------------------------------------------------------------
template <class HEADER_T = uint32_t>
class net::io::sender::Nstream_header : 
	public						net::io::sender::Nheader<HEADER_T>,
	virtual public				net::io::Isocket_tcp
{
public:
	virtual	bool				process_send( shared_buffer&& _buffer, uint64_t _option) override
	{
		// check) _buffer->data_가 nullptr이 아닌가?
		ERROR_RETURN_IF(_buffer.data() == nullptr, false, )

		// check) _buffer->size_이 0이 아닌가?
		ERROR_RETURN_IF(_buffer.size() == 0, false, )

		// check) 버퍼가 Overflow되었는가?
		check(_buffer._is_buffer_overflow() == false);

	#if defined(_VALIDATE_SEND_MESSAGE)
		// check) 
		definition_message_header<HEADER_T>::_validate_message(_buffer);
	#endif

		// 1) 전송한다.
		return this->process_sendable(std::move(_buffer), 1);
	}
};

}
