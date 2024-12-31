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
// net::io::sender::Nstream
//
//-----------------------------------------------------------------------------
class net::io::sender::Nstream : 
	virtual public				net::io::Isender,
	virtual public				net::io::Isocket_tcp
{
public:
	virtual	bool				process_send( shared_buffer&& _buffer, uint64_t _option) override;
};

inline bool net::io::sender::Nstream::process_send( shared_buffer&& _buffer, uint64_t /*_option*/)
{
	// check) _buffer->data_가 nullptr이 아닌가?
	ERROR_RETURN_IF(_buffer.data() == nullptr, false, )

	// check) _buffer->size_이 0이 아닌가?
	ERROR_RETURN_IF(_buffer.size() == 0, false, )

	// check) 버퍼가 Overflow되었는가?
	check(_buffer._is_buffer_overflow() == false);

	// 1) 전송한다.
	return this->process_sendable(std::move(_buffer), 1);

}

}