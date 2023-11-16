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
	virtual public				Isocket,
	virtual public				net::io::statistics::Nsocket,
	virtual public				net::io::Iconnectable,
	virtual public				net::io::Isendable,
	virtual public				net::io::Ipacketable,
	virtual public				Imessageable
{
public:
			Nsocket_tcp() {}

public:
	virtual	bool				closesocket(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept;
	virtual	bool				disconnect(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept override;
	virtual	bool				destroy() noexcept override { return closesocket(); }

			std::size_t			get_minimum_mesage_buffer_size() const noexcept { return m_minimum_mesage_buffer_size;}
			void				set_minimum_mesage_buffer_size(std::size_t _value) noexcept { m_minimum_mesage_buffer_size = _value; }
			std::size_t			get_maximum_mesage_buffer_size() const noexcept { return m_maximum_mesage_buffer_size; }
			void				set_maximum_mesage_buffer_size(std::size_t _value) noexcept { m_maximum_mesage_buffer_size = _value; }
			std::size_t			get_max_depth_of_send_buffer() const noexcept { return m_max_depth_of_send_buffer; }
			void				set_max_depth_of_send_buffer(std::size_t _value) noexcept { m_max_depth_of_send_buffer = _value; }

			TSharedPtr<FInternetAddr> remote_endpoint() const noexcept { return process_get_peer_address(); };
			TSharedPtr<FInternetAddr> local_endpoint() const noexcept { return process_get_socket_address(); };

protected:
	virtual void				on_connect(net::io::Iconnective* _pconnective) {}
	virtual void				on_disconnect(uint64_t _disconnect_reason) {}
	virtual void				on_fail_connect(net::io::Iconnective* _pconnective, uint64_t _disconnect_reason) {}
	virtual	void				on_closesocket(uint64_t _disconnect_reason) {}

protected:
	virtual	TSharedPtr<FInternetAddr> process_get_socket_address() const noexcept override;
	virtual	TSharedPtr<FInternetAddr> process_get_peer_address() const noexcept override;

	virtual	bool				process_complete_connect(net::io::Iconnective* _pconnective, uintptr_t _return) override;
	virtual	bool				process_complete_disconnect() override;
	virtual	bool				process_sendable(net::io::Isend_completor* _psend_completor, const buffer_view* _array_buffer, std::size_t _count_buffer, Ireferenceable* _powner, std::size_t _count_message, const FInternetAddr& _address) override;
			intptr_t			process_receiving();
			void				process_sending();
	static	ESocketErrors		process_send_buffer(FSocket* _psocket, buffer_view& _buf_send);
	virtual	bool				process_closesocket(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept override;

	virtual void				process_socket_io() override;

protected:
			std::size_t			m_minimum_mesage_buffer_size = 1024; // default 1Kbytes
			std::size_t			m_maximum_mesage_buffer_size = 64 * 1024 * 1024; // default 64Mbytes
			std::size_t			m_max_depth_of_send_buffer = 1024;
			struct QUEUE_SEND
			{
				object_ptr<net::io::Isend_completor> psend_completor;
				buffer_view					array_buffer;
				object_ptr<Ireferenceable>	powner;
				std::size_t					count_message = 0;
			};
			lockable<>			m_lock_sending;
			circular_list<QUEUE_SEND> m_queue_pending;
			circular_list<QUEUE_SEND> m_queue_processing;
			bool				m_flag_sending = false;

			shared_buffer		m_buffer_received;
};

inline bool net::io::Nsocket_tcp::closesocket(uint64_t _disconnect_reason) noexcept
{
	// 1) get native hnadle
	auto psocket = native_handle();

	// check) 
	RETURN_IF(psocket == nullptr, false);

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
	auto psocket = native_handle();

	// check)
	RETURN_IF(psocket == nullptr, false);

	// 1) set linger option to graceful close
	psocket->SetLinger(true, 0);

	// 2) shutdown
	psocket->Shutdown(ESocketShutdownMode::Write);

	// 2) call 'process_complete_disconnect()' first
	this->process_complete_disconnect();

	// 3) closesocket
	return this->process_closesocket(_disconnect_reason);
}


inline TSharedPtr<FInternetAddr> net::io::Nsocket_tcp::process_get_socket_address() const noexcept
{
	// 1) get native hnadle
	auto psocket = native_handle();

	// check)
	RETURN_IF(psocket == nullptr, TSharedPtr<FInternetAddr>(nullptr));

	// 2) alloc 'FInternetAddr' object
	auto addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	// 3) get peer address
	psocket->GetAddress(*addr);

	// return)
	return addr;
}

inline TSharedPtr<FInternetAddr> net::io::Nsocket_tcp::process_get_peer_address() const noexcept
{
	// 1) get native hnadle
	auto psocket = native_handle();

	// check)
	RETURN_IF(psocket == nullptr, TSharedPtr<FInternetAddr>(nullptr));

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
	auto psocket = native_handle();

	// check) return if psocket is nullptr
	RETURN_IF(psocket == nullptr, false);

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
					auto is_changed = set_socket_state_if(eSOCKET_STATE::SYN, eSOCKET_STATE::ESTABLISHED);

					// - 
					if (is_changed == true)
					{
						// - prepare receive
						m_buffer_received.clear();

						scoped_lock(m_lock_sending)
						{
							m_queue_pending.clear();
							m_queue_processing.clear();
							m_flag_sending = false;
						}

						// statistics) 
						statistics_on_connect();

						// - on_connect
						on_connect(_pconnective);
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
	auto psocket = native_handle(nullptr);

	// check)
	RETURN_IF(psocket == nullptr, false)

	// 3) call 'on_closesocket()'
	on_closesocket(m_disconnect_reason);

	// 4) closesocket
	psocket->Close();

	// 5) destroy socket
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(psocket);

	// 6) detach from socket executor
	executor::socket::get_instance()->detach(this);

	// statistics) 
	statistics_on_disconnect();

	// return)
	return true;
}

inline bool net::io::Nsocket_tcp::process_complete_disconnect()
{
	// 1) closesockcet
	auto is_changed = set_socket_state_if(eSOCKET_STATE::ESTABLISHED, eSOCKET_STATE::CLOSED);

	// check)
	check(is_changed == true);

	// check) 
	RETURN_IF(is_changed == false, false);

	// 2) on_disconnect
	on_disconnect(m_disconnect_reason);

	// return) 
	return true;
}

inline ESocketErrors net::io::Nsocket_tcp::process_send_buffer(FSocket* _psocket, buffer_view& _buf_send)
{
	auto buf_send = _buf_send;

	do
	{
		// declare)
		int32_t bytes_send = 0;

		// - send
		auto result = _psocket->Send(_buf_send.data<uint8>(), _buf_send.size<int32_t>(), bytes_send);

		// - send completed
		if (result)
		{
			// - process
			buf_send += offset(bytes_send);
		}
		else
		{
			// check)
			check(ISocketSubsystem::Get()->GetLastErrorCode() != ESocketErrors::SE_NO_ERROR);

			// - buffer update
			_buf_send = buf_send;

			// return)
			return ISocketSubsystem::Get()->GetLastErrorCode();
		}

		if (buf_send.size() != 0)
		{
			_buf_send = buf_send;
		}

	} while (buf_send.size() != 0);

	// return)
	return ESocketErrors::SE_NO_ERROR;
}

inline bool net::io::Nsocket_tcp::process_sendable(net::io::Isend_completor* _psend_completor, const buffer_view* _array_buffer, std::size_t _count_buffer, Ireferenceable* _powner, std::size_t _count_message, const FInternetAddr& /*_address*/)
{
	// check)
	check(_array_buffer != nullptr);
	check(_count_buffer == 1);
	check(_powner != nullptr);

	// check) return if socket state is not CLOSED
	RETURN_IF(get_socket_state() != eSOCKET_STATE::ESTABLISHED, false);

	// 1) get socket handle
	auto psocket = native_handle();

	// check) 
	check(psocket != nullptr);

	// check) return if psocket is nullptr
	RETURN_IF(psocket == nullptr, false);

	// 2) queuing if aleady processing
	scoped_lock(m_lock_sending)
	{
		if(m_flag_sending == true
		|| m_queue_pending.empty() == false
		|| m_queue_processing.empty() == false
		)
		{
			// check) check overflow 
			RETURN_IF(m_queue_pending.size() > m_max_depth_of_send_buffer, false);

			// - generate 
			QUEUE_SEND temp;
			temp.psend_completor = _psend_completor;
			temp.array_buffer = *_array_buffer;
			temp.powner = _powner;
			temp.count_message = _count_message;

			// - queuing buffer
			m_queue_pending.push_back(temp);

			// return) 
			return true;
		}

		// - set flag_sending
		m_flag_sending = true;
	}

	// declare)
	auto buf_send = *_array_buffer;

	// 3) send buffer
	auto result = process_send_buffer(psocket, buf_send);

	// 4) queuing if it occured EWOULDBLOCK error
	if (result == ESocketErrors::SE_EWOULDBLOCK)
	{
		// - queuing in front of buffer
		QUEUE_SEND temp;
		temp.psend_completor = _psend_completor;
		temp.array_buffer = *_array_buffer;
		temp.powner = _powner;
		temp.count_message = _count_message;

		// - queuing buffer
		scoped_lock(m_lock_sending)
		{
			m_queue_pending.push_front(temp);
		}

		// - set flag_sending
		m_flag_sending = false;

		// return)
		return true;
	}

	// 5) success or not
	bool is_successed = (result == ESocketErrors::SE_NO_ERROR);

	// 6) complete sending
	if (_psend_completor != nullptr)
	{
		_psend_completor->process_complete_send(is_successed, _array_buffer->size(), _count_message); // (uintptr_t _return, std::size_t _transferred_bytes, std::size_t _transferred_message)
	}

	// 7) set flag_sending
	m_flag_sending = false;

	// return) 
	return is_successed;
}

inline void net::io::Nsocket_tcp::process_sending()
{
	// 1) get find...
	auto psocket = native_handle();

	// check) return if psocket is nullptr
	if (psocket == nullptr)
	{
		// - ...
		m_flag_sending = false;

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

	// check)
	scoped_lock(m_lock_sending)
	{
		// check) 
		RETURN_IF(m_flag_sending, );

		// 2) swap if empty
		if (m_queue_processing.empty() == false)
		{
		}
		else if(m_queue_pending.empty() == false)
		{
			m_queue_processing.swap(m_queue_pending);
		}
		else
		{
			return;
		}

		// 2) sending now
		m_flag_sending = true;
	}


	// 3) sending process
	do
	{
		auto& iter = m_queue_processing.front();

		// - send buffer
		auto result = process_send_buffer(psocket, iter.array_buffer);

		// check) 
		BREAK_IF(result != ESocketErrors::SE_NO_ERROR)

		// - complete sending
		if (iter.psend_completor.exist())
		{
			iter.psend_completor->process_complete_send(0, 0, iter.count_message); // (uintptr_t _return, std::size_t _transferred_bytes, std::size_t _transferred_message)
		}

		// - pop_front
		m_queue_processing.pop_front();

	} while (m_queue_processing.empty() == false);

	// 4) 
	m_flag_sending = false;
}

inline intptr_t net::io::Nsocket_tcp::process_receiving()
{
	// check) return if socket state is not eSOCKET_STATE::ESTABLISHED
	check(get_socket_state() == eSOCKET_STATE::ESTABLISHED);

	// 1) get socket handle
	auto psocket = native_handle();

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
		auto buffer_size = std::max<uint32_t>(m_minimum_mesage_buffer_size, bytes_pended); // g_bytes_message_buffer_default_min_size

		// 4) get receiving buffer
		if (m_buffer_received.get_remained_size() >= m_buffer_received.size() + buffer_size)
		{
			buffer_receive = std::move(m_buffer_received);
		}
		else
		{
			// - alloc new buffer
			buffer_receive = alloc_shared_buffer(m_buffer_received.size() + buffer_size);

			// - copy stored data
			if (m_buffer_received.size() != 0)
			{
				buffer_receive.append(m_buffer_received.size(), m_buffer_received.data());
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
				process_complete_disconnect();

				// - closesocket
				process_closesocket();
			}

			// return)
			return 0;
		}

		// statistics)
		statistics_on_receive_bytes(bytes_pended);

		// 7) add size 
		buffer_receive.add_size(bytes_pended);

		// 8) process message
		process_packet(buffer_receive, TSharedPtr<FInternetAddr>(nullptr));

		// 9) store if remained
		if (buffer_receive.size() != 0)
		{
			m_buffer_received = std::move(buffer_receive);
		}
		else
		{
			m_buffer_received.clear();
		}
	}
	catch (...)
	{
		// - call 'process_complete_disconnect()'
		process_complete_disconnect();

		// - close socket
		process_closesocket(DISCONNECT_REASON_ACTIVE | DISCONNECT_REASON_ABORTIVE);
	}

	// return)
	return 0;
}

inline void net::io::Nsocket_tcp::process_socket_io()
{
	switch (get_socket_state())
	{
	case	eSOCKET_STATE::SYN:
			process_complete_connect(nullptr, 0);
			break;

	case	eSOCKET_STATE::ESTABLISHED:
			process_receiving();
			process_sending();
			break;

	default:
			break;
	}
}


}