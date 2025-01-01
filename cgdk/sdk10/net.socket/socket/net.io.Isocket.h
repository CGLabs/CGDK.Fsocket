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
	// net::io::Isocket
	//
	//-----------------------------------------------------------------------------
	class net::io::Isocket : public TSharedFromThis<Isocket>
	{
	public:
		virtual	~Isocket() noexcept {}

	public:
		virtual	bool		closesocket(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept PURE;
		virtual	bool		process_closesocket(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept PURE;
		virtual void		process_socket_io() PURE;

	public:
		TSharedPtr<FSocket>	native_handle(TSharedPtr<FSocket>&& _psocket) noexcept { assert(_psocket.IsValid()); auto temp = std::move(this->m_psocket); this->m_psocket = std::move(_psocket); return temp; }
		TSharedPtr<FSocket>	native_handle() const noexcept { return this->m_psocket; }
		TSharedPtr<FSocket>	reset_native_handle() noexcept { return std::move(this->m_psocket); }
		bool				is_invalid_native_handle() const noexcept { return !this->is_valid_native_handle(); }
		bool				is_valid_native_handle() const noexcept { return this->m_psocket.IsValid(); }

		eSOCKET_STATE		get_socket_state() const noexcept { return this->m_socket_status; }
		eSOCKET_STATE		set_socket_state(eSOCKET_STATE _status) noexcept { return this->m_socket_status.exchange(_status); }
		bool				set_socket_state_if(eSOCKET_STATE _status_comparend, eSOCKET_STATE _status_new) noexcept { return this->m_socket_status.compare_exchange_weak(_status_comparend, _status_new); }
		bool				exchange_socket_state_if(eSOCKET_STATE& _status_comparend, eSOCKET_STATE _status_new) noexcept { return this->m_socket_status.compare_exchange_weak(_status_comparend, _status_new); }

		void				set_disconnect_reason(uint64_t _disconnect_reason) noexcept { this->m_disconnect_reason = _disconnect_reason; }
		uint64_t			get_disconnect_reason() const noexcept { return this->m_disconnect_reason; }

		static	TSharedPtr<FInternetAddr> get_local_address() noexcept
		{
			TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
			return addr;
		}

	protected:
		FCriticalSection	m_lock_socket;
		TSharedPtr<FSocket>	m_psocket;
		std::atomic<eSOCKET_STATE> m_socket_status{ eSOCKET_STATE::CLOSED };
		uint64_t			m_disconnect_reason{ 0 };
	};

	class net::io::Isocket_tcp : virtual public Isocket
	{
	public:
		virtual	bool				process_sendable(shared_buffer&& _buffer, std::size_t _count_message) PURE;
	};

	class net::io::Isocket_udp : virtual public Isocket
	{
	public:
		virtual	bool				process_sendable(shared_buffer&& _buffer, std::size_t _count_message, const FInternetAddr& _address) PURE;
	};

}