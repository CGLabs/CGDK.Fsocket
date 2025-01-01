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

			FCriticalSection	m_cs_peer_adddress_reconnect;
			TSharedPtr<FInternetAddr> m_peer_adddress_reconnect;
};

inline bool net::io::Nsocket_tcp_client::start(TSharedPtr<FInternetAddr>&& _endpoint_remote)
{
	// 1) get socket handle
	auto psocket = this->native_handle();

	// 2) create new socket object
	if (!psocket.IsValid())
	{
		// - create new socke object
		auto psocket_raw = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

		// check) 
		check(psocket_raw != nullptr);

		// check) 
		ERROR_THROW_IF(psocket_raw == nullptr, std::exception(), )

		// - set socket option
		psocket_raw->SetNonBlocking(true);
		psocket_raw->SetReuseAddr(true);

		// - make sharable
		psocket = MakeShareable<FSocket>(psocket_raw, [](FSocket* _psocket) 
			{
				// check)
				RETURN_IF(_psocket == nullptr, );

				// log)
				UE_LOG(LogSockets, Display, TEXT("[CGDK] < 'Fsocket' destroyed"));

				// - destroy
				ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(_psocket);
			});

		// - set socket object
		this->native_handle(TSharedPtr<FSocket>(psocket));

		// - socket을 Closed로 한다.
		this->set_socket_state(eSOCKET_STATE::CLOSED);
	}

	// log)
	UE_LOG(LogSockets, Display, TEXT("[CGDK] < 'Fsocket' created"));

	// 3) set socket status as eSOCKET_STATE::SYN only if socket status is eSOCKET_STATE::CLOSED
	bool is_changed = this->set_socket_state_if(eSOCKET_STATE::CLOSED, eSOCKET_STATE::SYN);

	// check) return if socket state is not CLOSED
	RETURN_IF(is_changed == false, false);

	// 4) reset socket reason
	this->m_disconnect_reason = 0;

	// 5) reset statistics info
	this->reset_statistics();

	// declare)
	TSharedPtr<FInternetAddr> adddress_connect;

	// 6) get address
	{
		// lock)
		FScopeLock cs(&this->m_cs_peer_adddress_reconnect);

		// - stroe address
		if(_endpoint_remote.IsValid() && _endpoint_remote->IsValid())
			this->m_peer_adddress_reconnect = std::move(_endpoint_remote);

		// check) 
		ERROR_RETURN_IF(this->m_peer_adddress_reconnect.IsValid() == false || this->m_peer_adddress_reconnect->IsValid() == false, false, );

		// - copu address
		adddress_connect = this->m_peer_adddress_reconnect;
	}

	// trace) 
	UE_LOG(LogSockets, Display, TEXT("[CGDK] > socket request connect to %s (%u)"), *adddress_connect->ToString(true), this->native_handle().Get());

	// 7) try connect
	auto result = psocket->Connect(*adddress_connect);

	// check) if failed
	if (result == false)
	{
		// trace) 
		UE_LOG(LogSockets, Display, TEXT("[CGDK] > socket fail to connecting to %s (%u)"), *adddress_connect->ToString(true), this->native_handle().Get());

		// return)
		return false;
	}

	// statistics) 
	this->statistics_on_connect_try();

	// 8) call 'process_complete_connect'
	result = this->process_complete_connect(nullptr, 0);

	// 9) register to socket executor
	if (result == true || this->m_enable_reconnect == false)
	{
		executor::socket::get_instance()->attach(this->AsShared());
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
	auto psocket = this->reset_native_handle();

	// check)
	RETURN_IF(!psocket.IsValid(), false)

	// 2) call 'on_closesocket()'
	this->on_closesocket(this->m_disconnect_reason);

	// trace) 
	UE_LOG(LogSockets, Display, TEXT("[CGDK] < socket closed (reason:%d) (%u)"), this->m_disconnect_reason, this->native_handle().Get());

	// 3) detach from socket executor
	if (this->m_enable_reconnect == false)
	{
		executor::socket::get_instance()->detach(this);
	}

	// statistics) 
	this->statistics_on_disconnect();

	// return)
	return true;
}

inline void net::io::Nsocket_tcp_client::process_reconnectiong()
{
	// 1) reset socket handle
	auto psocket = this->reset_native_handle();

	// check)
	RETURN_IF(!psocket.IsValid(), );

	// 2) get diff time
	auto diff_time = FDateTime::UtcNow() - this->statistics_get_time_disconnect();

	// 3) try reconnect
	RETURN_IF(diff_time < this->m_tick_reconnection_interval, );

	// 4) get address
	TSharedPtr<FInternetAddr> peer_address = this->m_peer_adddress_reconnect;

	// check) 
	RETURN_IF(!peer_address.IsValid(), );

	// 5) try reconnect
	this->on_request_reconnect(nullptr);

	// 6) reconnect
	this->start(std::move(peer_address));
}

inline void net::io::Nsocket_tcp_client::process_socket_io()
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

	case	eSOCKET_STATE::CLOSED:
			if(this->m_enable_reconnect == true)
			{
				this->process_reconnectiong();
			}
			break;

	default:
		break;
	}
}

inline TSharedPtr<FInternetAddr> MakeInternetAddr(const net::sockaddr& _endpoint_remote)
{
		// check) 
	RETURN_IF(_endpoint_remote.is_unspecified(), {});
	
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
		return {};
	
		//TArray<uint8> array_bytes;
		//for (auto& iter : _endpoint_remote.sockaddr_v6.sin6_addr.u.Byte)
		//	array_bytes.Add(iter);
		//addr_temp->SetRawIp(array_bytes);
		//addr_temp->SetPort(_endpoint_remote.port());
	}
	
	// 3) connect
	return addr_temp;
}

}