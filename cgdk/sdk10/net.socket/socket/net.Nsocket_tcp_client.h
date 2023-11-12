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
// net::io::Nsocket_tcp_client
//
//-----------------------------------------------------------------------------
class net::io::Nsocket_tcp_client : public net::io::Nsocket_tcp
{
public:
			bool				start(const net::sockaddr& _endpoint_remote);
			bool				start(const FInternetAddr& _endpoint_remote);
			bool				start(TSharedPtr<FInternetAddr>&& _endpoint_remote);
			eRESULT				wait(FTimespan _duration_wait= FTimespan(0,0,30));

			void				enable_reconnect( bool _flag_enable = true) noexcept { m_enable_reconnect = _flag_enable; }
			void				disable_reconnect() noexcept { m_enable_reconnect=false;}
			bool				reconnection_enable() const noexcept { return m_enable_reconnect; }
																							  
			void				set_reconnect_interval(const FTimespan& _tick) noexcept { m_tick_reconnection_interval = _tick; }
			FTimespan			get_reconnect_interval() const noexcept { return m_tick_reconnection_interval;}

protected:
	virtual	void				on_request_reconnect(net::io::Iconnective* _pconnective) {}

protected:
	virtual	bool				process_closesocket(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept override;
			void				process_reconnectiong();

	virtual void				process_socket_io() override;

protected:
			bool				m_enable_reconnect = false;
			FTimespan			m_tick_reconnection_interval = FTimespan(0,0,10);

			lockable<TSharedPtr<FInternetAddr>> m_peer_adddress_reconnect;
};

inline bool net::io::Nsocket_tcp_client::start(const net::sockaddr& _endpoint_remote)
{
	// check) 
	RETURN_IF(_endpoint_remote.is_unspecified(), false);

	// 1) alloc FInternetAddr
	auto addr_temp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	if (_endpoint_remote.is_v4())
	{
		TArray<uint8> array_bytes;
		for (auto& iter : _endpoint_remote.sockaddr_v4.sin_addr.S_un.S_bytes)
			array_bytes.Add(iter);
		addr_temp->SetRawIp(array_bytes);
		addr_temp->SetPort(_endpoint_remote.port());
	}
	else
	{
		return false;

		//TArray<uint8> array_bytes;
		//for (auto& iter : _endpoint_remote.sockaddr_v6.sin6_addr.u.Byte)
		//	array_bytes.Add(iter);
		//addr_temp->SetRawIp(array_bytes);
		//addr_temp->SetPort(_endpoint_remote.port());
	}

	// 3) connect
	return start(std::move(addr_temp));
}

inline bool net::io::Nsocket_tcp_client::start(const FInternetAddr& _endpoint_remote)
{
	// 1) alloc FInternetAddr
	auto addr_temp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	// 2) copy address
	*addr_temp = _endpoint_remote;

	// 3) connect
	return start(std::move(addr_temp));
}

inline bool net::io::Nsocket_tcp_client::start(TSharedPtr<FInternetAddr>&& _endpoint_remote)
{
	// 1) get socket handle
	auto psocket = native_handle();

	// 2) create new socket object
	if (psocket == nullptr)
	{
		// - create new socke object
		psocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

		// check) 
		check(psocket != nullptr);

		// check) 
		ERROR_THROW_IF(psocket == nullptr, std::exception(), )

		// - set socket option
		psocket->SetNonBlocking(true);
		psocket->SetReuseAddr(true);

		// - set socket object
		native_handle(psocket);

		// - socket을 Closed로 한다.
		set_socket_state(eSOCKET_STATE::CLOSED);
	}

	// 3) set socket status as eSOCKET_STATE::SYN only if socket status is eSOCKET_STATE::CLOSED
	bool is_changed = set_socket_state_if(eSOCKET_STATE::CLOSED, eSOCKET_STATE::SYN);

	// check) return if socket state is not CLOSED
	RETURN_IF(is_changed == false, false);

	// 4) reset socket reason
	m_disconnect_reason = 0;

	// 5) reset statistics info
	reset_statistics();

	// declare)
	TSharedPtr<FInternetAddr> adddress_connect;

	// 6) get address
	scoped_lock(m_peer_adddress_reconnect)
	{
		// - stroe address
		if (_endpoint_remote.IsValid() && _endpoint_remote->IsValid())
		{
			m_peer_adddress_reconnect = _endpoint_remote->Clone();
		}

		// check) 
		ERROR_RETURN_IF(m_peer_adddress_reconnect.IsValid() == false || m_peer_adddress_reconnect->IsValid() == false, false, );

		// - copu address
		adddress_connect = m_peer_adddress_reconnect;
	}

	// trace) 
	//LOG_INFO_LOW(DEFAULT_LOGGER, "(info) CGDK.net.socket: binding tcp socket ({}) ({})", to_string<char>(socket_address()).c_str(), __FUNCTION__);

	// 7) try connect
	auto result = psocket->Connect(*adddress_connect);

	// check) if failed
	if (result == false)
	{
		// log) 
		//LOG_ERROR(DEFAULT_LOGGER, "(Err ) CONNECTOR['{}'] Fail to connecting socket [ErrorCode: {}] ({})", (name().empty() == false) ? name().data() : "", api::get_last_error(), __FUNCTION__);

		// return)
		return false;
	}

	// statistics) 
	statistics_on_connect_try();

	// 8) call 'process_complete_connect'
	result = process_complete_connect(nullptr, 0);

	// 9) register to socket executor
	if (result == true || m_enable_reconnect == false)
	{
		executor::socket::get_instance()->attach(this);
	}

	// return) 
	return result;
}

inline eRESULT net::io::Nsocket_tcp_client::wait(FTimespan _duration_wait)
{
	return eRESULT::SUCCESS;
}

inline bool net::io::Nsocket_tcp_client::process_closesocket(uint64_t _disconnect_reason) noexcept
{
	// 1) exchange
	auto psocket = native_handle(nullptr);

	// check)
	RETURN_IF(psocket == nullptr, false)

	// 2) call 'on_closesocket()'
	on_closesocket(m_disconnect_reason);

	// 3) destroy socket
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(psocket);

	// 4) detach from socket executor
	if (m_enable_reconnect == false)
	{
		executor::socket::get_instance()->detach(this);
	}

	// statistics) 
	statistics_on_disconnect();

	// return)
	return true;
}

inline void net::io::Nsocket_tcp_client::process_reconnectiong()
{
	// 1) reset socket handle
	auto psocket = native_handle(nullptr);

	// check)
	RETURN_IF(psocket != nullptr, );

	// 2) get diff time
	auto diff_time = FDateTime::UtcNow() - statistics_get_time_disconnect();

	// 3) try reconnect
	RETURN_IF(diff_time < m_tick_reconnection_interval, );

	// 4) try reconnect
	on_request_reconnect(nullptr);

	// 5) reconnect
	start(TSharedPtr<FInternetAddr>(nullptr));
}

inline void net::io::Nsocket_tcp_client::process_socket_io()
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

	case	eSOCKET_STATE::CLOSED:
			if(m_enable_reconnect == true)
			{
				process_reconnectiong();
			}

	default:
		break;
	}
}



}