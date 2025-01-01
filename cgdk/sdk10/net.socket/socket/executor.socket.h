//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                              object classes                               *
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

#include <thread>

namespace CGDK
{
//-----------------------------------------------------------------------------
//
// executor::socket
//
// ----------------------------------------------------------------------------
class executor::socket
{
public:
			socket() noexcept {}
	virtual	~socket() noexcept {}

// public)
public:
	// 1) start
			bool				start(size_t _count_thread = 0);
			void				stop() noexcept;

	//! @brief 'queue에 핸들을 붙인다. @return true 성공 @return false 실패
	virtual	bool				attach(TSharedPtr<net::io::Isocket>&& _psocket);
	virtual	bool				detach(net::io::Isocket* _psocket) noexcept;

	// 2) execute함수임...
	//! @brief 실행을 처리하는 함수 @param _tick_diff 최대 실행 대기 시간(이 시간 내에 처리가 되지 않으면 TIME_OUT으로 리턴한다.) @return	true 성공 @return false 실패
	virtual	bool				execute(FTimespan _wait = FTimespan::MaxValue(), intptr_t _option = 0);

	//! @brief 큐잉된 '실행 객체'를 모두 실행한다.
	virtual	void				execute_queued_all() noexcept;

public:
	static	TSharedPtr<socket>& get_instance()
	{
		static TSharedPtr<socket> temp_instance;

		return temp_instance;
	}

	enum class eSTATUS
	{
		NONE,
		RUN,
		DESTORY
	};

protected:
			FCriticalSection	m_cs_list_socket;
			std::vector<TSharedPtr<net::io::Isocket>> m_list_socket;
			std::vector<std::thread> m_list_thread;
			eSTATUS				m_status{ eSTATUS::NONE };

public:
	static	void				initialize_instance(size_t _count_thread = 0);
	static	bool				run_executor();
	static	void				destory_executor() noexcept;

	static	void				_fn_thread(socket* _this);
			void				process_thread();
	static	TSharedPtr<socket>	m_g_instance;
};

inline bool executor::socket::start(size_t _count_thread)
{
	// lock)
	FScopeLock cs(&this->m_cs_list_socket);

	// check)
	if (this->m_status != eSTATUS::NONE)
		return false;

	// 1) set flat
	this->m_status = eSTATUS::RUN;

	// declare)
	std::vector<std::thread> list_thread;
	bool result = true;

	try
	{
		// 2) start thread
		for (int i = 0; i < _count_thread; ++i)
			list_thread.emplace_back(_fn_thread, this);

		// 3) add to thread list
		this->m_list_thread = std::move(list_thread);
	}
	catch (...)
	{
		// rollback)
		this->m_status = eSTATUS::NONE;

		// - set result false
		result = false;
	}

	// return) 
	return true;
}

inline void executor::socket::stop() noexcept
{
	// declare)
	std::vector<std::thread> list_thread;
	std::vector<TSharedPtr<net::io::Isocket>> list_socket;

	// 1) reset thread and get remained socket for close
	{
		// lock)
		FScopeLock cs(&this->m_cs_list_socket);

		// check) 
		if (this->m_status != eSTATUS::RUN)
			return;

		// - stop the threads
		if(this->m_list_socket.empty())
			this->m_status = eSTATUS::NONE;
		else
			this->m_status = eSTATUS::DESTORY;

		// - reset thread & copy sockets
		list_thread = std::move(this->m_list_thread);

		// - copy sockets
		list_socket = this->m_list_socket;
	}

	// 2) close sockets
	for (auto& iter : list_socket)
		iter->closesocket();

	// 3) wait until socket 
	while (this->m_status != eSTATUS::NONE)
	{
		this->run_executor();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// 4) wait thread termination
	for (auto& iter : list_thread)
		iter.join();
}

inline bool executor::socket::attach(TSharedPtr<net::io::Isocket>&& _psocket)
{
	// lock)
	FScopeLock cs(&this->m_cs_list_socket);

	// check) 
	if (this->m_status != eSTATUS::RUN)
		return false;

	// 1) find psocket
	auto iter_find = std::find(this->m_list_socket.begin(), this->m_list_socket.end(), _psocket);
	
	// check) aleady exist?
	RETURN_IF(iter_find != this->m_list_socket.end(), false);

	// 2) add new
	this->m_list_socket.push_back(std::move(_psocket));

	// return) 
	return true;
}

inline bool executor::socket::detach(net::io::Isocket* _psocket) noexcept
{
	// lock)
	FScopeLock cs(&this->m_cs_list_socket);

	// 1) find socket
	auto iter_find = std::find_if(this->m_list_socket.begin(), this->m_list_socket.end(), [=](const TSharedPtr<net::io::Isocket>& _iter) noexcept { return _iter.Get() == _psocket; });

	// check)
	if(iter_find == this->m_list_socket.end())
		return false;

	// 2) erase socket
	this->m_list_socket.erase(iter_find);

	// 3) check all released on eSTATUS::DESTORY
	if (this->m_status == eSTATUS::DESTORY && this->m_list_socket.empty())
		this->m_status = eSTATUS::NONE;

	// return) 
	return true;
}

inline bool executor::socket::execute(FTimespan _wait, intptr_t _option)
{
	// declare) 
	std::vector<TSharedPtr<net::io::Isocket>> list_socket;

	// 1) list socket을 복사한다. (주의! 복사해서 process_socket_io()를 돌리지 않으면 iterator 무효화 문제가 발생할 수 있다.
	{
		// lock) 
		FScopeLock cs(&this->m_cs_list_socket);

		// check)
		if (this->m_status == eSTATUS::NONE)
			return false;
		
		// - copy soocket list
		list_socket = this->m_list_socket;
	}

	// 2) 모든 socket의 socket_io를 호출한다.
	for (auto& iter : list_socket)
	{
		iter->process_socket_io();
	}

	// return) 
	return true;
}

inline void executor::socket::execute_queued_all() noexcept
{
}

inline void executor::socket::initialize_instance(size_t _count_thread)
{
	// declare)
	auto pinstance = MakeShared<executor::socket>();

	// 1) start socket executor
	pinstance->start(_count_thread);

	// 2) set instance
	get_instance() = pinstance;
}

inline void executor::socket::destory_executor() noexcept
{
	// static)
	static FCriticalSection cs_destory_instance;

	// declare)
	TSharedPtr<socket> pinstance;

	{
		// lock) destroy
		FScopeLock Lock(&cs_destory_instance);

		// 1) get executor_instance
		pinstance = get_instance();

		// check) 
		assert(pinstance);
	}

	// 2) close instance
	pinstance->stop();
}

inline void executor::socket::process_thread()
{
	while (this->m_status != eSTATUS::NONE)
	{
		execute(FTimespan(100));
	}
}

inline void executor::socket::_fn_thread(executor::socket* _this)
{
	_this->process_thread();
}

inline bool executor::socket::run_executor()
{
	return get_instance()->execute(FTimespan::Zero());
}


}


