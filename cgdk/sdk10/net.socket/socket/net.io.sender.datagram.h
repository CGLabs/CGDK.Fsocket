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
// net::io::sender::Ncontainer
//
//-----------------------------------------------------------------------------
class net::io::sender::Ndatagram : 
	virtual public				net::io::Isender_datagram,
	virtual public				net::io::Isocket_udp,
	virtual public				net::io::statistics::Nsocket
{
public:
	virtual	bool				process_send(shared_buffer&& _buffer, const FInternetAddr& _address, uint64_t _option) override;
};

inline bool net::io::sender::Ndatagram::process_send(shared_buffer&& _buffer, const FInternetAddr& _address, uint64_t _option)
{
	// check) _array_buffer->data_가 nullptr이 아닌가?
	check(_buffer.data() != nullptr)

	// check) _array_buffer->size_이 0이 아닌가?
	check(_buffer.size() != 0);

	// check) 버퍼가 Overflow되었는가?
	check(_buffer._is_buffer_overflow() == false);

	// 1) request send
	return this->process_sendable(std::move(_buffer), 1, _address);
}

}
