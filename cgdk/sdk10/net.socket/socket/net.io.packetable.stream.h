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
// net::io::packetable::Nstream
//
//-----------------------------------------------------------------------------
template <class TMSG_HEAD = uint32_t>
class net::io::packetable::Nstream :
	virtual public				net::io::Isocket_tcp,
	virtual public				net::io::Ipacketable,
	virtual public				Imessageable,
	virtual public				net::io::statistics::Nsocket
{
public:
			using TMESSAGE_HEAD = TMSG_HEAD;

protected:
	virtual	void				process_pre_message(sMESSAGE_NETWORK& /*_msg*/) {}
	virtual	std::size_t			process_packet( shared_buffer& _buffer, const TSharedPtr<FInternetAddr>& _address_peer) override;
};


template <class TMSG_HEAD>
std::size_t net::io::packetable::Nstream<TMSG_HEAD>::process_packet( shared_buffer& _buffer, const TSharedPtr<FInternetAddr>& _address_peer)
{
	// check)
	ERROR_THROW_IF(_buffer.data() == nullptr, throwable::failure(eRESULT::INVALID_MESSAGE_BUFFER_IS_NULL), /*LOG_ERROR(DEFAULT_LOGGER, "(Err ) CGNetBase: _buffer.data_ is nullptr [_buffer.size_: {}] ({})", _buffer.size(), __FUNCTION__)*/)

	// check)
	check(_buffer.size() != 0)

	// declare) 
	int count_message = 0;

	sMESSAGE_NETWORK msg;

	shared_buffer message = _buffer ^ sizeof(TMESSAGE_HEAD);

	// loop) 
	std::size_t remained_size = _buffer.size();

	try
	{
		while(remained_size >= sizeof(TMESSAGE_HEAD))
		{
			auto message_size = definition_message_header<TMESSAGE_HEAD>::_get_message_size(message);

			message.set_size(message_size);

			// check)
			ERROR_THROW_IF(message.size() < sizeof(TMESSAGE_HEAD), throwable::failure(eRESULT::INVALID_MESSAGE_TOO_SHORT_LENGTH), /*LOG_ERROR(DEFAULT_LOGGER, "(Err ) CGNetBase: Message size is too short[Message Size: {}] ({})", message.size(), __FUNCTION__)*/);

			// check)
			BREAK_IF(message.size() > remained_size);

			msg.buf_message = message;
			msg.address = _address_peer;

			process_pre_message(msg);
		
			process_message(msg);

			++count_message;

			message.add_data(message.size());
			remained_size -= message.size();

			// check)
			if (this->is_invalid_native_handle())
			{
				remained_size = 0;
				break;
			}
		}
	}
	catch(...)
	{
		// statistiscs)
		statistics_on_receive_message(count_message);

		// reraise) 
		throw;
	}
	
	// statistics)
	statistics_on_receive_message(count_message);

	_buffer = buffer_view(message.data(), remained_size);

	// return)
	return	static_cast<std::size_t>(message.size());
}


}