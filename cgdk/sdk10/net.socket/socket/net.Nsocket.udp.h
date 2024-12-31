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
// net::io::Nsocket_udptcp
//
//-----------------------------------------------------------------------------
class net::io::Nsocket_udp :
	virtual public				Isocket,
	virtual public				net::io::statistics::Nsocket,
	virtual public				net::io::Ipacketable
{
public:
			Nsocket_udp() {}

public:
			bool				bind();
			bool				bind(FInternetAddr& _endpoint_bind);
	virtual	bool				closesocket(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept;

public:
			TSharedPtr<FInternetAddr> get_socket_address() const noexcept;
			TSharedPtr<FInternetAddr> local_endpoint() const noexcept { return get_socket_address(); };

			bool				set_multicast_ttl(uint8_t _TTL);
			bool				set_multicast_loopback(bool _enable=false);
			bool				add_multicast_member_ship(const FInternetAddr& _group_address, const FInternetAddr& _interface_address);
			bool				drop_multicast_member_ship(const FInternetAddr& _group_address);

protected:
	virtual	void				on_bind() {}
	virtual	void				on_receive(const buffer_view& _buffer, const FInternetAddr& _address) {}
	virtual	void				on_closesocket(uint64_t _disconnect_reason) {}

protected:
	virtual	bool				process_sendable(shared_buffer&& _buffer, std::size_t _count_message, const FInternetAddr& _address);
	virtual void				process_on_receive (const buffer_view& _buffer, const FInternetAddr& _address) { on_receive(_buffer, _address); }
			void				process_receiving();
	virtual	bool				process_closesocket(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept override;
	virtual void				process_socket_io() override;

private:
			bool				m_enable_report_connect_reset;
};

inline TSharedPtr<FInternetAddr> net::io::Nsocket_udp::get_socket_address() const noexcept
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

inline bool net::io::Nsocket_udp::set_multicast_ttl(uint8_t _ttl)
{
	// check)
	RETURN_IF(_ttl < 1 || _ttl > 255, false);

	// 1) get native hnadle
	auto psocket = native_handle();

	// check)
	RETURN_IF(psocket == nullptr, false);

	// 1) Set socket Option
	return psocket->SetMulticastTtl(_ttl);
}

inline bool net::io::Nsocket_udp::set_multicast_loopback(bool _flag_enable)
{
	// 1) get native hnadle
	auto psocket = native_handle();

	// check)
	RETURN_IF(psocket == nullptr, false);

	// 1) set socket option
	return psocket->SetMulticastLoopback(_flag_enable);
}

inline bool net::io::Nsocket_udp::add_multicast_member_ship(const FInternetAddr& _group_address, const FInternetAddr& _interface_address)
{
	// 1) get native hnadle
	auto psocket = native_handle();

	// check)
	RETURN_IF(psocket == nullptr, false);

	// 2) 
	return psocket->JoinMulticastGroup(_group_address, _interface_address);
}

inline bool net::io::Nsocket_udp::drop_multicast_member_ship(const FInternetAddr& _group_address)
{
	// 1) get native hnadle
	auto psocket = native_handle();

	// check)
	RETURN_IF(psocket == nullptr, false);

	// 2) 
	return psocket->LeaveMulticastGroup(_group_address);
}

inline bool net::io::Nsocket_udp::bind()
{
	// 1) create address object
	auto paddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	// 2) set as bin_any
	paddr->SetAnyAddress();

	// 3) bind
	return bind(*paddr);
}

inline bool net::io::Nsocket_udp::bind(FInternetAddr& _endpoint_bind)
{
	//  check) aleady binded?
	RETURN_IF(get_socket_state() >= eSOCKET_STATE::BINDED, false);

	// 2) get socket handle
	auto psocket = this->native_handle();

	// 1) create new socket object
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

		// - make shared
		psocket = MakeShareable<FSocket>(psocket_raw, [](FSocket* _psocket)
				{
					// check)
					RETURN_IF(_psocket == nullptr, );

					// - destroy
					ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(_psocket);
				});

		// - set socket object
		this->native_handle(TSharedPtr<FSocket>(psocket));

		// - socket을 Closed로 한다.
		this->set_socket_state(eSOCKET_STATE::CLOSED);
	}

	// 2) set socket status as eSOCKET_STATE::SYN only if socket status is eSOCKET_STATE::CLOSED
	bool is_changed = set_socket_state_if(eSOCKET_STATE::CLOSED, eSOCKET_STATE::BINDED);

	// check) return if socket state is not CLOSED
	RETURN_IF(is_changed == false, false);

	// 3) reset statistics info
	this->reset_statistics();

	// trace) 
	//LOG_INFO_LOW(DEFAULT_LOGGER, "(info) CGDK.net.socket: binding tcp socket ({}) ({})", to_string<char>(socket_address()).c_str(), __FUNCTION__);

	// 4) try connect
	auto result = psocket->Bind(_endpoint_bind);

	// check) if failed
	if (result == false)
	{
		//// log) 
		//LOG_ERROR(DEFAULT_LOGGER, "(Err ) CONNECTOR['{}'] Fail to connecting socket [ErrorCode: {}] ({})", (name().empty() == false) ? name().data() : "", api::get_last_error(), __FUNCTION__);

		// return)
		return	false;
	}

	// 5) on_bind함수를 호출한다.
	on_bind();

	// 6) register to socket executor
	executor::socket::get_instance()->attach(this->AsShared());

	// return) 
	return	true;
}

inline bool net::io::Nsocket_udp::closesocket(uint64_t _disconnect_reason) noexcept
{
	return process_closesocket(_disconnect_reason);
}

inline bool net::io::Nsocket_udp::process_sendable(shared_buffer&& _buffer, std::size_t _count_message, const FInternetAddr& _address)
{
	// check)
	check(_buffer.exist());

	shared_buffer buf_send = std::move(_buffer);

	// check) return if socket state is not CLOSED
	RETURN_IF(this->get_socket_state() < eSOCKET_STATE::BINDED, false);

	// 1) get socket handle
	auto psocket = this->native_handle();

	// check) 
	check(psocket.IsValid());

	// check) return if psocket is nullptr
	RETURN_IF(!psocket.IsValid(), false);

	// declare)
	int32 bytes_send{ 0 };

	// 2) send
	auto result = psocket->SendTo(buf_send.data<uint8>(), buf_send.size<int32>(), bytes_send, _address);

	// return) 
	return result;
}

inline void net::io::Nsocket_udp::process_receiving()
{
	// check) 
	check(get_socket_state() >= eSOCKET_STATE::BINDED);

	// 1) get socket handle
	auto handle_socket = native_handle();

	while (true)
	{
		// declare) 
		int32_t bytes_pended = 0;

		// 2) get pended data bytes
		handle_socket->HasPendingData(reinterpret_cast<uint32_t&>(bytes_pended));

		// check) ...
		BREAK_IF(bytes_pended <= 0);

		// 3) alloc shared buffer
		auto buffer_receive = alloc_shared_buffer(bytes_pended);
		auto address_peer = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		auto bytes_transfered = bytes_pended;

		// 4) receive datagram data
		handle_socket->RecvFrom(buffer_receive.data<uint8_t>(), bytes_pended, bytes_transfered, *address_peer);

		// check) ...
		BREAK_IF(bytes_transfered <= 0);

		// 5) set length
		buffer_receive.set_size(bytes_transfered);

		// statistics)
		statistics_on_receive_bytes(bytes_transfered);

		// 6) call process_on_receive (일반적으로 여기서 하는 역할은 거의 없다.)
		process_on_receive(buffer_receive, *address_peer);

		// 7) process message
		//    - 여기서 Return해 오는 값은 다음 Message을 받기위해 대음에 필요로 하는 메모리 용량이다.
		//    - 주어지는 &buffer_receive에 진행된 Memory과 남은 Size등이 되돌아 온다.
		//      따라서 buffer_receive은 받은 데이터 그대로 사용하면 된다.
		//    - Exception이 발생가능하다.
		process_packet(buffer_receive, address_peer);
	}
}

inline bool net::io::Nsocket_udp::process_closesocket(uint64_t _disconnect_reason) noexcept
{
	// 1) exchange
	auto psocket = m_psocket;
	m_psocket = nullptr;

	// check)
	RETURN_IF(m_psocket == nullptr, false);

	// 1) detach from socket executor
	executor::socket::get_instance()->detach(this);

	// 2) set state
	this->set_socket_state(eSOCKET_STATE::CLOSED);

	// 3) call 'on_closesocket()'
	on_closesocket(_disconnect_reason);

	// 4) closesocket
	psocket->Close();

	// 5) ...
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(psocket.Get());

	// return)
	return true;
}

inline void net::io::Nsocket_udp::process_socket_io()
{
	switch (get_socket_state())
	{
	case	eSOCKET_STATE::BINDED:
			process_receiving();
			break;

	default:
		break;
	}
}


}