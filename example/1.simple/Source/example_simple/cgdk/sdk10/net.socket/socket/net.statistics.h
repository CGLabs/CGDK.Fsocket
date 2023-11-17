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
// net::io::statistics
//
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// I/O statistics
// ----------------------------------------------------------------------------
//#define	_NO_STATISTICS

#ifndef _NO_STATISTICS
#define	_USE_STATISTICS_TOTAL
#endif

#ifndef _NO_STATISTICS
#define	_USE_STATISTICS_CONNECTIVE
#define	_USE_STATISTICS_SOCKET
#define	_USE_STATISTICS_POOL
#endif 

#ifdef	_USE_STATISTICS_CONNECTIVE
	#define	CONNECTIVE_STATISTICS(value)			value
#else
	#define	CONNECTIVE_STATISTICS(value)
#endif	// _USE_STATISTICS_CONNECTIVE


#ifdef	_USE_STATISTICS_SOCKET
	#define	SOCKET_STATISTICS(value)				value
#else
	#define	SOCKET_STATISTICS(value)
#endif	// _USE_STATISTICS_SOCKET

#ifdef	_USE_STATISTICS_POOL
	#define	POOL_STATISTICS(value)					value
#else
	#define	POOL_STATISTICS(value)
#endif	// _USE_STATISTICS_POOL

// 2) socket Info
struct net::io::statistics::TRAFFIC
{
public:
	FDateTime					time_begin;
	FDateTime					time_end;

	std::atomic<uint64_t>		count_received_message;		// 받은 Message 수.
	std::atomic<uint64_t>		count_received_bytes;		// 받은 Bytes

	std::atomic<uint64_t>		count_sended_message;		// 보낸 Message 수.
	std::atomic<uint64_t>		count_sended_bytes;			// 보낸 Bytes

	std::atomic<uint64_t>		count_send_io;				// 송신 I/O횟수
	std::atomic<uint64_t>		count_receive_io;			// 수신 I/O횟수
	std::atomic<uint64_t>		count_general_io;			// 일반 송수신 I/O수

	std::atomic<uint64_t>		count_error_on_send;		// send시 Error
	std::atomic<uint64_t>		count_error_on_receive;		// receive시 Error

public:
	TRAFFIC() noexcept :
		time_begin(),
		time_end(),
		count_received_message(0),
		count_received_bytes(0),
		count_sended_message(0),
		count_sended_bytes(0),
		count_send_io(0),
		count_receive_io(0),
		count_general_io(0),
		count_error_on_send(0),
		count_error_on_receive(0)
	{
	}

	TRAFFIC(const TRAFFIC& _Copy) noexcept :
		time_begin(_Copy.time_begin),
		time_end(_Copy.time_end),
		count_received_message((uint64_t)_Copy.count_received_message),
		count_received_bytes((uint64_t)_Copy.count_received_bytes),
		count_sended_message((uint64_t)_Copy.count_sended_message),
		count_sended_bytes((uint64_t)_Copy.count_sended_bytes),
		count_send_io((uint64_t)_Copy.count_send_io),
		count_receive_io((uint64_t)_Copy.count_receive_io),
		count_general_io((uint64_t)_Copy.count_general_io),
		count_error_on_send((uint64_t)_Copy.count_error_on_send),
		count_error_on_receive((uint64_t)_Copy.count_error_on_receive)
	{
	}

	TRAFFIC& operator=(const TRAFFIC& _rhs) noexcept
	{
		time_begin = _rhs.time_begin;
		time_end = _rhs.time_end;

		count_received_message.store(_rhs.count_received_message);
		count_received_bytes.store(_rhs.count_received_bytes);

		count_sended_message.store(_rhs.count_sended_message);
		count_sended_bytes.store(_rhs.count_sended_bytes);

		count_send_io.store(_rhs.count_send_io);
		count_receive_io.store(_rhs.count_receive_io);
		count_general_io.store(_rhs.count_general_io);

		count_error_on_send.store(_rhs.count_error_on_send);
		count_error_on_receive.store(_rhs.count_error_on_receive);

		// return)
		return	*this;
	}

	void reset() noexcept
	{
		time_begin = FDateTime::MinValue();
		time_end = FDateTime::MinValue();
		count_received_message = 0;
		count_received_bytes = 0;
		count_sended_message = 0;
		count_sended_bytes = 0;
		count_send_io = 0;
		count_receive_io = 0;
		count_general_io = 0;
		count_error_on_send = 0;
		count_error_on_receive = 0;
	}
};


// 7일 동안의 정보 분량으로 1분 단위로 저장함.
//#define	TOTAL_INFO			1440*7
#define	TOTAL_INFO				32
																							  
struct net::io::statistics::TRAFFIC_INFO : virtual public Ireferenceable					  
{																							  
public:																						  
			void				reset() noexcept
			{
				__zero_memory(reinterpret_cast<void*>(m_period), sizeof(m_period));
				__zero_memory(reinterpret_cast<void*>(&m_total), sizeof(m_total));
			}

			uint64_t			get_sended_message_total() const noexcept					{ return m_total.count_sended_message + _AT(m_period, get_statistics_time()).count_sended_message;}
			uint64_t			get_sended_message_now() const noexcept						{ return _AT(m_period, get_statistics_time()).count_sended_message;}
			uint64_t			get_sended_message( int _time) const noexcept				{ return _AT(m_period, _time).count_sended_message; }
																							  
			uint64_t			get_sended_bytes_total() const noexcept						{ return m_total.count_sended_bytes + _AT(m_period, get_statistics_time()).count_sended_bytes;}
			uint64_t			get_sended_bytes_now() const noexcept						{ return _AT(m_period, get_statistics_time()).count_sended_bytes;}
			uint64_t			get_sended_bytes( int _time) const noexcept					{ return _AT(m_period, _time).count_sended_bytes; }
																							  
			uint64_t			get_received_message_total() const noexcept					{ return m_total.count_received_message + _AT(m_period, get_statistics_time()).count_received_message;}
			uint64_t			get_received_message_now() const noexcept					{ return _AT(m_period, get_statistics_time()).count_received_message;}
			uint64_t			get_received_message( int _time) const noexcept				{ return _AT(m_period, _time).count_received_message; }
																							  
			uint64_t			get_received_bytes_total() const noexcept					{ return m_total.count_received_bytes + _AT(m_period, get_statistics_time()).count_received_bytes;}
			uint64_t			get_received_bytes_now() const noexcept						{ return _AT(m_period, get_statistics_time()).count_received_bytes;}
			uint64_t			get_received_bytes( int _time) const noexcept				{ return _AT(m_period, _time).count_received_bytes; }
																							  
			uint64_t			get_error_on_send_total() const noexcept					{ return m_total.count_error_on_send + _AT(m_period, get_statistics_time()).count_error_on_send;}
			uint64_t			get_error_on_send_now() const noexcept						{ return _AT(m_period, get_statistics_time()).count_error_on_send;}
			uint64_t			get_error_on_send( int _time) const noexcept				{ return _AT(m_period, _time).count_error_on_send; }
																							  
			uint64_t			get_error_on_reveive_total() const noexcept					{ return m_total.count_error_on_receive + _AT(m_period, get_statistics_time()).count_error_on_receive;}
			uint64_t			get_error_on_reveive_now() const noexcept					{ return _AT(m_period, get_statistics_time()).count_error_on_receive;}
			uint64_t			get_error_on_reveive( int _time) const noexcept				{ return _AT(m_period, _time).count_error_on_receive; }
																							  
			uint64_t			get_async_send_total() const noexcept						{ return m_total.count_send_io + _AT(m_period, get_statistics_time()).count_send_io;}
			uint64_t			get_async_send_now() const noexcept							{ return _AT(m_period, get_statistics_time()).count_send_io;}
			uint64_t			get_async_send( int _time) const noexcept					{ return _AT(m_period, _time).count_send_io; }
																							  
			uint64_t			get_async_reveive_total() const noexcept					{ return m_total.count_receive_io + _AT(m_period, get_statistics_time()).count_receive_io;}
			uint64_t			get_async_reveive_now() const noexcept						{ return _AT(m_period, get_statistics_time()).count_receive_io;}
			uint64_t			get_async_reveive( int _time) const noexcept				{ return _AT(m_period, _time).count_receive_io; }
																							  
			uint64_t			get_async_general_total() const noexcept					{ return m_total.count_general_io + _AT(m_period, get_statistics_time()).count_general_io; }
			uint64_t			get_async_general_now() const noexcept						{ return _AT(m_period, get_statistics_time()).count_general_io; }
			uint64_t			get_async_general( int _time) const noexcept				{ return _AT(m_period, _time).count_general_io; }
																							  
			void				statistics_on_receive_message() noexcept					{ ++_AT(m_period, get_statistics_time()).count_received_message;}
			void				statistics_on_receive_message( uint64_t _count) noexcept	{ _AT(m_period, get_statistics_time()).count_received_message += _count; }
			void				statistics_on_receive_bytes( uint64_t _bytes) noexcept		{ _AT(m_period, get_statistics_time()).count_received_bytes += _bytes; }
			void				statistics_on_send_message() noexcept						{ ++_AT(m_period, get_statistics_time()).count_sended_message;}
			void				statistics_on_send_message( uint64_t _count) noexcept		{ _AT(m_period, get_statistics_time()).count_sended_message += _count; }
			void				statistics_on_send_bytes( uint64_t _bytes) noexcept			{ _AT(m_period, get_statistics_time()).count_sended_bytes += _bytes; }
			void				statistics_on_error_send() noexcept							{ ++_AT(m_period, get_statistics_time()).count_error_on_send;}
			void				statistics_on_error_receive() noexcept						{ ++_AT(m_period, get_statistics_time()).count_error_on_receive;}
			void				statistics_on_async_receive() noexcept						{ ++_AT(m_period, get_statistics_time()).count_receive_io;}
			void				statistics_on_async_send() noexcept							{ ++_AT(m_period, get_statistics_time()).count_send_io;}
			void				statistics_on_async_general() noexcept						{ ++_AT(m_period, get_statistics_time()).count_general_io;}
																							  
			auto&				get_traffic_info() const noexcept							{ return *(const TRAFFIC*)(m_period + get_statistics_time());}
			auto&				get_traffic_info( int _index) const noexcept				{ check(_index<TOTAL_INFO); return *(const TRAFFIC*)(m_period + _index); }

	static	int					get_statistics_time() noexcept								{ static int g_statistics_time = 0; return g_statistics_time;}

private:																					  
			TRAFFIC				m_period[TOTAL_INFO];										  
			TRAFFIC				m_total;													  
																							  
public:																						  
			TRAFFIC_INFO() noexcept	{}														  
			TRAFFIC_INFO(const TRAFFIC_INFO& _copy) noexcept								{ for (int i = 0; i < TOTAL_INFO; ++i) { m_period[i] = _copy.m_period[i]; } m_total = _copy.m_total; }
			TRAFFIC_INFO(TRAFFIC_INFO&& _move) noexcept										{ for (int i = 0; i < TOTAL_INFO; ++i) { m_period[i] = _move.m_period[i];  _move.m_period[i].reset();} m_total = _move.m_total; _move.m_total.reset(); }
																							  
			TRAFFIC_INFO&		operator=(const TRAFFIC_INFO& _copy) noexcept				{ for (int i = 0; i < TOTAL_INFO; ++i) { m_period[i] = _copy.m_period[i]; } m_total = _copy.m_total; return *this;}
			TRAFFIC_INFO&		operator=(TRAFFIC_INFO&& _move) noexcept					{ for (int i = 0; i < TOTAL_INFO; ++i) { m_period[i] = _move.m_period[i];  _move.m_period[i].reset();} m_total = _move.m_total; _move.m_total.reset(); return *this;}
};

class net::io::statistics::Nsocket
{
// constructor/destructor) 
public:
			Nsocket() :
				m_pstatistics_traffic(make_object<TRAFFIC_INFO>()),
				m_count_total_connect_try(0),
				m_count_total_connect(0),
				m_count_connect_try(0),
				m_time_disconnect(FDateTime::MinValue())
			{
				reset_statistics();
			}

			~Nsocket() noexcept
			{
			}

// publics) 
public:
	// 1) Get 함수.
			auto				statistics_get_count_connect_try() const noexcept			{ return m_count_connect_try;}
			auto				statistics_get_time_connect() const noexcept				{ return m_time_connect;}
			auto				statistics_get_time_disconnect() const noexcept				{ return m_time_disconnect;}
																							  
			auto				statistics_get_received_message() const noexcept			{ return m_pstatistics_traffic->get_received_message_total();}
			auto				statistics_get_received_bytes() const noexcept				{ return m_pstatistics_traffic->get_received_bytes_total();}
			auto				statistics_get_time_last_send() const noexcept				{ return m_time_last_send;}
			auto				statistics_get_sended_message() const noexcept				{ return m_pstatistics_traffic->get_sended_message_total();}
			auto				statistics_get_sended_bytes() const noexcept				{ return m_pstatistics_traffic->get_sended_bytes_total();}
			auto				statistics_get_time_last_receive() const noexcept			{ return m_time_last_receive;}
																							  
			int					statistics_get_now_async_io() const noexcept				{ return m_count_async_con_dis+m_count_async_sending+m_count_async_receiving;}
			auto				statistics_get_async_send() const noexcept					{ return m_pstatistics_traffic->get_async_send_total();}
			int					statistics_get_async_sending() const noexcept				{ return m_count_async_sending;}
			auto				statistics_get_async_receive() const noexcept				{ return m_pstatistics_traffic->get_async_reveive_total();}
			int					statistics_get_async_receiving() const noexcept				{ return m_count_async_receiving;}
			auto				statistics_get_overlapped_general() const noexcept			{ return m_pstatistics_traffic->get_async_general_total();}

			int					is_error_disconnected() const noexcept						{ return m_is_error_disconnect;}
																							  
	// 2) socket의 생성후 전체 정보															   
			uint64_t			statistics_get_total_count_connect_try() const noexcept		{ return m_count_total_connect_try;}
			uint64_t			statistics_get_total_count_connect_success() const noexcept	{ return m_count_total_connect;}
			uint64_t			statistics_get_total_count_connect_fail() const noexcept	{ return m_count_total_connect_try-m_count_total_connect;}
																							  
	// 3) socket 전체 정보																	   
	static	const TRAFFIC_INFO&	get_total() noexcept										{ return _total();}

// implementation) 
private:
	// 1) 전송량 통계
			object_ptr<TRAFFIC_INFO> m_pstatistics_traffic;

	// 2) Total관련(이것은 socket이 disconnect되어도 reset되지 않는다.)
			std::atomic<uint64_t>	m_count_total_connect_try;	// 총 접속 시도 횟수
			std::atomic<uint64_t>	m_count_total_connect;		// 총 접속 성공 횟수

	// 3) 전송시간 관련
			uint64_t			m_count_connect_try;			// 접속 시도 횟수
			FDateTime			m_time_connect;					// 접속한 시간.
			FDateTime			m_time_last_send;				// 최후에 보낸 시간.(Tick)
			FDateTime			m_time_last_receive;			// 최후에 받은 시간.(Tick)
			FDateTime			m_time_wait_time;				// 기다리는 시간
			FDateTime			m_time_disconnect;				// 접속종료한 시간.(Tick)
			std::atomic<int>	m_is_error_disconnect;			// Error로 인한 강제접속종료 여부

			std::atomic<int>	m_count_async_con_dis;			// 현재 진행중인 connect/disconnect Overlapped I/O의 수.
			std::atomic<int>	m_count_async_sending;			// 현재 진행중인 send Overlapped I/O의 수.
			std::atomic<int>	m_count_async_receiving;		// 현재 진행중인 receive Overlapped I/O의 수.

	static	TRAFFIC_INFO&		_total() noexcept											{ static OBJ<TRAFFIC_INFO> m_statistics_traffic_total; return m_statistics_traffic_total; }

public:
			void				reset_statistics() noexcept
			{
				// 1) Traffic Info를 reset한다.
				m_pstatistics_traffic->reset();

				// 2) I/O관련...
				m_count_async_con_dis = 0;
				m_count_async_sending = 0;
				m_count_async_receiving = 0;
				m_is_error_disconnect = false;

				// 3) 전송시간 관련
				m_time_last_receive = FDateTime::MinValue();
				m_time_last_send = FDateTime::MinValue();
				m_time_wait_time = FDateTime::MinValue();
			}

			auto				statistics_get_traffic_info() const noexcept				{ return m_pstatistics_traffic;}
																							  
			void				statistics_on_connect_try() noexcept						{ SOCKET_STATISTICS(++m_count_connect_try); SOCKET_STATISTICS(++m_count_total_connect_try); m_time_disconnect= FDateTime::MinValue();}
			void				statistics_on_connect() noexcept							{ m_time_connect=m_time_last_receive=m_time_last_send=FDateTime::UtcNow(); SOCKET_STATISTICS(++m_count_total_connect);}
			void				statistics_on_disconnect() noexcept							{ m_time_disconnect=FDateTime::UtcNow(); m_count_connect_try=0;}
			void				statistics_set_error_disconnect() noexcept					{ m_is_error_disconnect=static_cast<int>(true);}
			void				statistics_check_error_disconnect() noexcept				{ int count_error_disconnect=m_is_error_disconnect.exchange(false); }
																							  
			void				statistics_on_receive_message() noexcept					{ SOCKET_STATISTICS(m_pstatistics_traffic->statistics_on_receive_message());		_total().statistics_on_receive_message();}
			void				statistics_on_receive_message( uint64_t _count) noexcept	{ SOCKET_STATISTICS(m_pstatistics_traffic->statistics_on_receive_message(_count));	_total().statistics_on_receive_message(_count); }
			void				statistics_on_receive_bytes( uint64_t _bytes) noexcept		{ SOCKET_STATISTICS(m_pstatistics_traffic->statistics_on_receive_bytes(_bytes));	_total().statistics_on_receive_bytes(_bytes); m_time_last_receive=FDateTime::UtcNow(); }
			void				statistics_on_receive_async() noexcept						{ SOCKET_STATISTICS(m_pstatistics_traffic->statistics_on_async_receive());			_total().statistics_on_async_receive(); }
			void				statistics_on_general_async() noexcept						{ SOCKET_STATISTICS(m_pstatistics_traffic->statistics_on_async_general());			_total().statistics_on_async_general(); }
			void				statistics_on_send_message() noexcept						{ SOCKET_STATISTICS(m_pstatistics_traffic->statistics_on_send_message());			_total().statistics_on_send_message(); }
			void				statistics_on_send_message( uint64_t _count) noexcept		{ SOCKET_STATISTICS(m_pstatistics_traffic->statistics_on_send_message(_count));		_total().statistics_on_send_message(_count);}
			void				statistics_on_send_bytes( uint64_t _bytes) noexcept			{ SOCKET_STATISTICS(m_pstatistics_traffic->statistics_on_send_bytes(_bytes));		_total().statistics_on_send_bytes(_bytes);  m_time_last_send=FDateTime::UtcNow();}
			void				statistics_on_send_async() noexcept							{ SOCKET_STATISTICS(m_pstatistics_traffic->statistics_on_async_send());				_total().statistics_on_async_send();}
			void				statistics_on_error_send() noexcept							{ _total().statistics_on_error_send();}
			void				statistics_on_error_receive() noexcept						{ _total().statistics_on_error_receive();}
			void				statistics_update_tick_last_send(const FDateTime& _Tick) noexcept{ m_time_last_send = _Tick; }
			void				statistics_update_tick_last_receive(const FDateTime& _Tick) noexcept{ m_time_last_receive = _Tick; }

			int					statistics_on_increase_async_con_dis() noexcept				{ return ++m_count_async_con_dis;}
			int					statistics_on_decrease_async_con_dis() noexcept				{ return --m_count_async_con_dis;}
			int					statistics_on_increase_async_sending() noexcept				{ return ++m_count_async_sending;}
			int					statistics_on_decrease_async_sending() noexcept				{ return --m_count_async_sending;}
			int					statistics_on_increase_async_receiveing() noexcept			{ return ++m_count_async_receiving;}
			int					statistics_on_decrease_async_receiveing() noexcept			{ return --m_count_async_receiving;}
};


}