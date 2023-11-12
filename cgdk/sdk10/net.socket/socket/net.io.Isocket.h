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
class net::io::Isocket : virtual public object::Idestroyable
{
public:
	virtual	~Isocket() noexcept {}

public:
	virtual	bool				process_closesocket(uint64_t _disconnect_reason = DISCONNECT_REASON_NONE) noexcept PURE;
	virtual void				process_socket_io() PURE;

public:
			FSocket*			native_handle(FSocket* _psocket) noexcept{ auto temp = m_psocket; m_psocket = _psocket; return temp;}
			FSocket*			native_handle() const noexcept { return m_psocket;}
			bool				is_invalid_native_handle() const noexcept { return m_psocket == nullptr; }
			bool				is_valid_native_handle() const noexcept { return !is_invalid_native_handle();}

			eSOCKET_STATE		get_socket_state() const noexcept {	return m_socket_status;}
			eSOCKET_STATE		set_socket_state( eSOCKET_STATE _status) noexcept {	return m_socket_status.exchange(_status);}
			bool				set_socket_state_if( eSOCKET_STATE _status_comparend, eSOCKET_STATE _status_new) noexcept { return m_socket_status.compare_exchange_weak(_status_comparend, _status_new);}
			bool				exchange_socket_state_if( eSOCKET_STATE& _status_comparend, eSOCKET_STATE _status_new) noexcept { return m_socket_status.compare_exchange_weak(_status_comparend, _status_new);}

			void				set_disconnect_reason( uint64_t _disconnect_reason) noexcept { m_disconnect_reason = _disconnect_reason;}
			uint64_t			get_disconnect_reason() const noexcept { return m_disconnect_reason;}

protected:
			lockable<>			m_lock_socket;
			FSocket*			m_psocket = nullptr;
			std::atomic<eSOCKET_STATE> m_socket_status{ eSOCKET_STATE::CLOSED };
			uint64_t			m_disconnect_reason{ 0 };
};


}