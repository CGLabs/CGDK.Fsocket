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
// net::io::Nsocket_tcp
//
//-----------------------------------------------------------------------------
class net::io::Nsocket_tcp : 
	virtual public				Isocket_tcp,
	virtual public				net::io::statistics::Nsocket,
	virtual public				net::io::Ipacketable,
	virtual public				Imessageable
{
public:
			Nsocket_tcp() {}
	virtual	~Nsocket_tcp() noexcept;

	virtual	bool				closesocket(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept;
	virtual	bool				disconnect(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept;

			std::size_t			get_minimum_mesage_buffer_size() const noexcept { return m_minimum_mesage_buffer_size;}
			void				set_minimum_mesage_buffer_size(std::size_t _value) noexcept { m_minimum_mesage_buffer_size = _value; }
			std::size_t			get_maximum_mesage_buffer_size() const noexcept { return m_maximum_mesage_buffer_size; }
			void				set_maximum_mesage_buffer_size(std::size_t _value) noexcept { m_maximum_mesage_buffer_size = _value; }
			std::size_t			get_max_depth_of_send_buffer() const noexcept { return m_max_depth_of_send_buffer; }
			void				set_max_depth_of_send_buffer(std::size_t _value) noexcept { m_max_depth_of_send_buffer = _value; }

			TSharedPtr<FInternetAddr> remote_endpoint() const noexcept;
			TSharedPtr<FInternetAddr> local_endpoint() const noexcept;

protected:
	virtual void				on_connect(net::io::Iconnective* _pconnective) {}
	virtual void				on_disconnect(uint64_t _disconnect_reason) {}
	virtual void				on_fail_connect(net::io::Iconnective* _pconnective, uint64_t _disconnect_reason) {}
	virtual	void				on_closesocket(uint64_t _disconnect_reason) {}

protected:
			struct SEND_DATA
			{
				shared_buffer	buf;
				std::size_t		messages = 0;
			};

	virtual	bool				process_complete_connect(net::io::Iconnective* _pconnective, uintptr_t _return);
	virtual	bool				process_complete_disconnect();
	virtual	bool				process_sendable(shared_buffer&& _buffer, std::size_t _messages);
			intptr_t			process_receiving();
			void				process_sending();
			bool				process_send_buffer(FSocket* _psocket, SEND_DATA&& _buf_send);
	virtual	bool				process_closesocket(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept override;

	virtual void				process_socket_io() override;

protected:
			std::size_t			m_minimum_mesage_buffer_size = 1024; // default 1Kbytes
			std::size_t			m_maximum_mesage_buffer_size = 64 * 1024 * 1024; // default 64Mbytes
			std::size_t			m_max_depth_of_send_buffer = 1024;
			FCriticalSection	m_cs_sending;
			std::vector<SEND_DATA> m_queue_pending;
			SEND_DATA			m_buf_send_processing;
			bool				m_flag_sending = false;

			shared_buffer		m_buffer_received;
};

inline net::io::Nsocket_tcp::~Nsocket_tcp() noexcept
{
	net::io::Nsocket_tcp::process_closesocket();
}

inline bool net::io::Nsocket_tcp::closesocket(uint64_t _disconnect_reason) noexcept
{
	// 1) get native hnadle
	auto psocket = this->native_handle();

	// check) 
	RETURN_IF(!psocket.IsValid(), false);

	// 1) set linger option to abortive close
	psocket->SetLinger(false, 0);

	// 2) call 'process_complete_disconnect()' first
	this->process_complete_disconnect();

	// 3) closesocket
	return this->process_closesocket(_disconnect_reason);
}

inline bool net::io::Nsocket_tcp::disconnect(uint64_t _disconnect_reason) noexcept
{
	// 1) get native hnadle
	auto psocket = this->native_handle();

	// check)
	RETURN_IF(!psocket.IsValid(), false);

	// 1) set linger option to graceful close
	psocket->SetLinger(true, 0);

	// 2) shutdown
	psocket->Shutdown(ESocketShutdownMode::Write);

	// 2) call 'process_complete_disconnect()' first
	this->process_complete_disconnect();

	// 3) closesocket
	return this->process_closesocket(_disconnect_reason);
}

inline TSharedPtr<FInternetAddr> net::io::Nsocket_tcp::local_endpoint() const noexcept
{
	// 1) get native hnadle
	auto psocket = this->native_handle();

	// check)
	RETURN_IF(!psocket.IsValid(), TSharedPtr<FInternetAddr>(nullptr));

	// 2) alloc 'FInternetAddr' object
	auto addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	// 3) get peer address
	psocket->GetAddress(*addr);

	// return)
	return addr;
}

inline TSharedPtr<FInternetAddr> net::io::Nsocket_tcp::remote_endpoint() const noexcept
{
	// 1) get native hnadle
	auto psocket = this->native_handle();

	// check)
	RETURN_IF(!psocket.IsValid(), TSharedPtr<FInternetAddr>(nullptr));

	// 2) alloc 'FInternetAddr' object
	auto addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	// 3) get peer address
	psocket->GetPeerAddress(*addr);

	// return)
	return addr;
}

inline bool net::io::Nsocket_tcp::process_complete_connect(net::io::Iconnective* _pconnective, uintptr_t _return)
{
	// check) return if socket state is not eSOCKET_STATE::ESTABLISHED
	check(get_socket_state() == eSOCKET_STATE::SYN);

	// 1) get socket handle
	auto psocket = this->native_handle();

	// check) return if psocket is nullptr
	RETURN_IF(!psocket.IsValid(), false);

	// declare)
	bool result = true;

	try
	{
		// 2) get connection status
		auto connection_status = psocket->GetConnectionState();

		switch (connection_status)
		{
		case	ESocketConnectionState::SCS_NotConnected:
				break;

		case	ESocketConnectionState::SCS_Connected:
				{
					// - closesockcet
					auto is_changed = this->set_socket_state_if(eSOCKET_STATE::SYN, eSOCKET_STATE::ESTABLISHED);

					// - 
					if (is_changed == true)
					{
						// - prepare receive
						this->m_buffer_received.clear();

						{
							FScopeLock cs(&this->m_cs_sending);

							this->m_queue_pending.clear();
							this->m_buf_send_processing = SEND_DATA();

							this->m_flag_sending = false;
						}

						// statistics) 
						this->statistics_on_connect();

						// - on_connect
						this->on_connect(_pconnective);
					}
				}
				break;

		case	ESocketConnectionState::SCS_ConnectionError:
				{
					// - closesockcet
					auto is_changed = set_socket_state_if(eSOCKET_STATE::SYN, eSOCKET_STATE::CLOSED);

					// - 
					if (is_changed == true)
					{
						on_fail_connect(_pconnective, 0);
					}

					// - closesocket
					this->process_closesocket();

					// - failed
					result = false;
				}
				break;
		}
	}
	catch (...)
	{
		// - closesocket
		this->process_closesocket();

		// - 
		result = false;
	}

	// return) 
	return result;
}

inline bool net::io::Nsocket_tcp::process_closesocket(uint64_t _disconnect_reason) noexcept
{
	// 1) exchange
	auto psocket = this->reset_native_handle();

	// check)
	RETURN_IF(!psocket.IsValid(), false)

	// 2) call 'on_closesocket()'
	this->on_closesocket(this->m_disconnect_reason);

	// 3) closesocket
	psocket->Close();

	// 4) detach from socket executor
	executor::socket::get_instance()->detach(this);

	// statistics) 
	this->statistics_on_disconnect();

	// return)
	return true;
}

inline bool net::io::Nsocket_tcp::process_complete_disconnect()
{
	// 1) closesockcet
	auto is_changed = set_socket_state_if(eSOCKET_STATE::ESTABLISHED, eSOCKET_STATE::CLOSED);

	// check) 
	RETURN_IF(is_changed == false, false);

	// 2) on_disconnect
	this->on_disconnect(m_disconnect_reason);

	// return) 
	return true;
}

inline bool net::io::Nsocket_tcp::process_send_buffer(FSocket* _psocket, SEND_DATA&& _send_data)
{
	// 1) send buffer
	while (_send_data.buf.size() != 0)
	{
		// declare)
		int32_t bytes_send = 0;

		// - send
		auto result= _psocket->Send(_send_data.buf.data<uint8>(), _send_data.buf.size<int32_t>(), bytes_send);

		// - send completed
		if (!result)
		{
			// - get error code
			auto err_code = ISocketSubsystem::Get()->GetLastErrorCode();

			// check) return if send_error
			if (err_code != ESocketErrors::SE_EWOULDBLOCK
			 && err_code != ESocketErrors::SE_TRY_AGAIN
			 /*&& err_code == ESocketErrors::SE_EINPROGRESS*/)
				return false;

			// - queuing buffer
			{
				FScopeLock cs(&this->m_cs_sending);
				this->m_buf_send_processing = std::move(_send_data);
			}

			// return) 
			return true;
		}

		// - process
		_send_data.buf += offset(bytes_send);
	}

	// 3) set flag_sending
	this->m_flag_sending = false;

	// return)
	return true;
}

inline bool net::io::Nsocket_tcp::process_sendable(shared_buffer&& _buffer, std::size_t _messages)
{
	// check)
	check(_buffer.exist());

	// check) return if socket state is not CLOSED
	RETURN_IF(this->get_socket_state() != eSOCKET_STATE::ESTABLISHED, false);

	// 1) get socket handle
	auto psocket = this->native_handle();

	// check) return if psocket is nullptr
	RETURN_IF(!psocket.IsValid(), false);

	// - generate 
	SEND_DATA temp;
	temp.buf = std::move(_buffer);
	temp.messages = _messages;

	// 2) queuing if aleady processing
	{
		FScopeLock cs(&this->m_cs_sending);

		if(this->m_flag_sending == true
		|| this->m_queue_pending.empty() == false
		|| this->m_buf_send_processing.buf.exist())
		{
			// check) check overflow 
			RETURN_IF(this->m_queue_pending.size() > this->m_max_depth_of_send_buffer, false);

			// - queuing buffer
			this->m_queue_pending.push_back(std::move(temp));

			// return) 
			return true;
		}

		// - set flag_sending
		this->m_flag_sending = true;
	}

	// 3) send buffer
	return this->process_send_buffer(psocket.Get(), std::move(temp));
}

inline void net::io::Nsocket_tcp::process_sending()
{
	// 1) get find...
	auto psocket = this->native_handle();

	// check) return if psocket is nullptr
	if (!psocket.IsValid())
	{
		// - ...
		this->m_flag_sending = false;

		// return) 
		return;
	}

	// 2) wait
	{
		// - peek status
		auto result = psocket->Wait(ESocketWaitConditions::WaitForWrite, FTimespan(0));

		// check)
		RETURN_IF(result == false, );
	}

	// decalre) 
	SEND_DATA send_data;

	{
		// lock) 
		FScopeLock cs(&this->m_cs_sending);

		// check) 
		RETURN_IF(this->m_flag_sending, );

		// 3) swap if empty
		if (this->m_buf_send_processing.buf.exist())
		{
			send_data = std::move(this->m_buf_send_processing);
		}
		else if (this->m_queue_pending.empty())
		{
			return;
		}
		else 
		{
			// - get queued pending bufs
			auto queue_pending = std::move(this->m_queue_pending);

			// unlock) 
			// 
			// 
			// - get 
			size_t size = 0;
			for (auto& iter : queue_pending)
			{
				size += iter.buf.size();
				send_data.messages += iter.messages;
			}

			// check)
			RETURN_IF(size == 0, );

			// - alloc buffer
			send_data.buf = alloc_shared_buffer(size);

			// - copy buffer
			for (auto& iter : queue_pending)
				send_data.buf.append(iter.buf.size(), iter.buf.data<char>());
		}
	}

	// 3) sending process
	this->process_send_buffer(psocket.Get(), std::move(send_data));
}

inline intptr_t net::io::Nsocket_tcp::process_receiving()
{
	// check) return if socket state is not eSOCKET_STATE::ESTABLISHED
	check(this->get_socket_state() == eSOCKET_STATE::ESTABLISHED);

	// 1) get socket handle
	auto psocket = this->native_handle();

	// check) return if psocket is nullptr
	RETURN_IF(psocket == nullptr, false);

	try
	{
		// declare) 
		int32_t bytes_pended = 0;

		// 2) get pended data bytes
		psocket->HasPendingData(reinterpret_cast<uint32_t&>(bytes_pended));

		// declare) 
		shared_buffer buffer_receive;
		int32_t bytes_transfered = 0;

		// 3) if bytes_receive_peneded size is less than min_size, set as min_size
		auto buffer_size = std::max<uint32_t>(this->m_minimum_mesage_buffer_size, bytes_pended); // g_bytes_message_buffer_default_min_size

		// 4) get receiving buffer
		if (this->m_buffer_received.get_remained_size() >= this->m_buffer_received.size() + buffer_size)
		{
			buffer_receive = std::move(this->m_buffer_received);
		}
		else
		{
			// - alloc new buffer
			buffer_receive = alloc_shared_buffer(this->m_buffer_received.size() + buffer_size);

			// - copy stored data
			if (this->m_buffer_received.size() != 0)
			{
				buffer_receive.append(this->m_buffer_received.size(), this->m_buffer_received.data());
			}
		}

		// 5) receive pended  data (offset for stored data)
		auto result = psocket->Recv(buffer_receive.get_back_ptr<uint8_t>(), buffer_size, bytes_pended, ESocketReceiveFlags::None);

		// check) 
		if (result == false || bytes_pended <= 0)
		{
			// - get error code
			auto error_code = ISocketSubsystem::Get()->GetLastErrorCode();

			// check) 
			if (error_code != ESocketErrors::SE_NO_ERROR 
			 && error_code != ESocketErrors::SE_EWOULDBLOCK)
			{
				// - call 'process_complete_disconnect()'
				this->process_complete_disconnect();

				// - closesocket
				this->process_closesocket();
			}

			// return)
			return 0;
		}

		// statistics)
		this->statistics_on_receive_bytes(bytes_pended);

		// 7) add size 
		buffer_receive.add_size(bytes_pended);

		// 8) process message
		this->process_packet(buffer_receive, TSharedPtr<FInternetAddr>(nullptr));

		// 9) store if remained
		if (buffer_receive.size() != 0)
		{
			this->m_buffer_received = std::move(buffer_receive);
		}
		else
		{
			this->m_buffer_received.clear();
		}
	}
	catch (...)
	{
		// - call 'process_complete_disconnect()'
		this->process_complete_disconnect();

		// - close socket
		this->process_closesocket(DISCONNECT_REASON_ACTIVE | DISCONNECT_REASON_ABORTIVE);
	}

	// return)
	return 0;
}

inline void net::io::Nsocket_tcp::process_socket_io()
{
	switch (this->get_socket_state())
	{
	case	eSOCKET_STATE::SYN:
			this->process_complete_connect(nullptr, 0);
			break;

	case	eSOCKET_STATE::ESTABLISHED:
			this->process_receiving();
			this->process_sending();
			break;

	default:
			break;
	}
}


}