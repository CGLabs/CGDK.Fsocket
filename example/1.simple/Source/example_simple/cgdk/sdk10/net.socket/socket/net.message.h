//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                           Network Base Classes                            *
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
//----------------------------------------------------------------------------
//
//  sMESSAGE_NETWORK
//
//----------------------------------------------------------------------------
struct sMESSAGE_NETWORK : public sMESSAGE_BUFFER
{
public:
	sMESSAGE_NETWORK() noexcept : sMESSAGE_BUFFER(eMESSAGE::SYSTEM::NETWORK), address() {}
	template<class TSOURCE>
	sMESSAGE_NETWORK(uint32_t _message, TSOURCE _psource=nullptr) noexcept : sMESSAGE_BUFFER(_message, _psource), address() {}
	template<class TSOURCE>
	sMESSAGE_NETWORK(uint32_t _message, TSOURCE _psource, const TSharedPtr<FInternetAddr>& _address) noexcept : sMESSAGE_BUFFER(_message, _psource), address(_address) {}
	template<class TSOURCE>
	sMESSAGE_NETWORK(uint32_t _message, TSOURCE _psource, const shared_buffer& _buffer) noexcept : sMESSAGE_BUFFER(_message, _psource, _buffer), address() {}
	template<class TSOURCE>
	sMESSAGE_NETWORK(uint32_t _message, TSOURCE _psource, const shared_buffer& _buffer, const TSharedPtr<FInternetAddr>& _address) noexcept : sMESSAGE_BUFFER(_message, _psource, _buffer), address(_address)	{}
	sMESSAGE_NETWORK(uint32_t _message, const shared_buffer& _buffer) noexcept : sMESSAGE_BUFFER(_message, _buffer), address() {}
	sMESSAGE_NETWORK(uint32_t _message, const shared_buffer& _buffer, const TSharedPtr<FInternetAddr>& _address) noexcept : sMESSAGE_BUFFER(_message, _buffer), address(_address)	{}
	sMESSAGE_NETWORK(const sMESSAGE_BUFFER& _msg) noexcept : sMESSAGE_BUFFER(_msg), address()	{}
	sMESSAGE_NETWORK(const sMESSAGE_BUFFER& _msg, const TSharedPtr<FInternetAddr>& _address) noexcept : sMESSAGE_BUFFER(_msg), address(_address) {}
	sMESSAGE_NETWORK(const shared_buffer& _buffer) noexcept : sMESSAGE_BUFFER(eMESSAGE::SYSTEM::NETWORK, _buffer), address() {}
	sMESSAGE_NETWORK(const shared_buffer& _buffer, const TSharedPtr<FInternetAddr>& _address) noexcept : sMESSAGE_BUFFER(eMESSAGE::SYSTEM::NETWORK, _buffer), address(_address)	{}

public:
	TSharedPtr<FInternetAddr> address;
};

inline bool is_network_message(uint32_t _message_type) noexcept
{
	return (_message_type & eMESSAGE::SYSTEM::NETWORK_MASK) == eMESSAGE::SYSTEM::NETWORK;
}

#define	CGMESSAGE_TYPE_SUB_BUFFER_NETWORK				CGMESSAGE_TYPE_SUB(eMESSAGE::SYSTEM::NETWORK, static_cast<sMESSAGE_BUFFER&>(_msg).buf_message.front<uint32_t>(4) & eMESSAGE::SYSTEM::MASK)
#define	CGMESSAGE_TYPE_SUB_BUFFER_NETWORK_TYPE(type)	CGMESSAGE_TYPE_SUB(eMESSAGE::SYSTEM::NETWORK, static_cast<type>(static_cast<uint32_t>(((sMESSAGE_BUFFER&)_msg).buf_message.front<type>(4)) & eMESSAGE::SYSTEM::MASK))
#define TRANSLATE_REQUEST_MESSAGE(msg, source)			if((msg.message==eMESSAGE::SYSTEM::eMESSAGE) && ((sMESSAGE_NETWORK&)msg).buf_message.front<uint32_t>(4)==eMESSAGE::NETWORK::SOCKET::REMOTE_REQUEST) { msg=sMESSAGE_RESPONSE(source, msg);}


}