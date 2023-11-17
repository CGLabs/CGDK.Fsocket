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
// net::io::packetable::Ndatagram
//
//-----------------------------------------------------------------------------
class net::io::packetable::Ndatagram : 
// Inherited classes)
	virtual public				net::io::Ipacketable,
	virtual public				Imessageable,
	virtual public				net::io::Isend_request,
	virtual public				net::io::statistics::Nsocket
{
// implementation)
public:
	virtual	std::size_t			process_packet( shared_buffer& _buffer, const TSharedPtr<FInternetAddr>& _address) override;
};

inline std::size_t net::io::packetable::Ndatagram::process_packet(shared_buffer& _buffer, const TSharedPtr<FInternetAddr>& _address)
{
	// 1) Message를 만든다.
	sMESSAGE_NETWORK msg(_buffer, _address);

	// 2) Message을 IMessageable로 전달하여 처리한다.
	process_message(msg);

	// statistics) receive Message수를 세린다.
	statistics_on_receive_message();

	// return) 다음 Message의 크기를 return한다!(Datagram에서는 별 의미없다.)
	return	sizeof(uint32_t);
}

}