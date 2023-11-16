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
class executor::socket :
// Inherited classes)
	virtual public				Ireferenceable
{
// constructor/destructor)
public:
	//! @brief 기본 생성자
			socket() noexcept {}
	virtual	~socket() noexcept {}

// public)
public:
	// 1) start
			bool				start(size_t _count_thread = 0);
			void				stop() noexcept;

	//! @brief 'queue에 핸들을 붙인다. @return true 성공 @return false 실패
	virtual	bool				attach(net::io::Isocket* _psocket);
	virtual	bool				detach(net::io::Isocket* _psocket) noexcept;

	// 2) execute함수임...
	//! @brief 실행을 처리하는 함수 @param _tick_diff 최대 실행 대기 시간(이 시간 내에 처리가 되지 않으면 TIME_OUT으로 리턴한다.) @return	true 성공 @return false 실패
	virtual	bool				execute(FTimespan _wait = FTimespan::MaxValue(), intptr_t _option = 0);

	//! @brief 큐잉된 '실행 객체'를 모두 실행한다.
	virtual	void				execute_queued_all() noexcept;

// implementation)
protected:
			lockable<std::vector<object_ptr<net::io::Isocket>>> m_list_socket;
			std::vector<std::thread> m_list_thread;

public:
	static	object_ptr<socket>	get_instance() 
	{ 
		static own_ptr<socket> temp_instance = initialize_instance(0);

		return temp_instance;
	}

public:
	static	own_ptr<socket>		initialize_instance(size_t _count_thread = 0);
	static	void				run_executor();
	static	void				_fn_thread(socket* _this);
			void				process_thread();
			bool				m_flag_run = false;
			std::atomic<int>	m_count_thread = 0;
};

inline bool executor::socket::start(size_t _count_thread)
{
	// 1) set flag
	m_flag_run = true;
	m_count_thread = 0;

	// 2) start thread
	if (_count_thread != 0)
	{
		std::thread temp_thread(_fn_thread, this);

		m_list_thread.push_back(std::move(temp_thread));
	}

	// return) 
	return true;
}

inline void executor::socket::stop() noexcept
{
	// 1) stop the threads
	m_flag_run = false;

	// 2) wait for terminating all threas
	while (m_count_thread != 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// 3) clear threads
	m_list_thread.clear();
}

inline bool executor::socket::attach(net::io::Isocket* _psocket)
{
	//return m_queue.attach(_executable_handle, _option, _pexecutable);
	scoped_lock(m_list_socket)
	{
		// 1) find psocket
		auto iter_find = std::find(m_list_socket.begin(), m_list_socket.end(), _psocket);

		// check) aleady exist?
		RETURN_IF(iter_find != m_list_socket.end(), false);

		// 2) add new
		m_list_socket.push_back(_psocket);
	}

	// return) 
	return true;
}

inline bool executor::socket::detach(net::io::Isocket* _psocket) noexcept
{
	scoped_lock(m_list_socket)
	{
		// 1) find psocket
		auto iter_find = std::find(m_list_socket.begin(), m_list_socket.end(), _psocket);

		// check) no exist?
		RETURN_IF(iter_find == m_list_socket.end(), false);

		// 2) erase
		m_list_socket.erase(iter_find);

	}

	// return) 
	return true;
}

inline bool executor::socket::execute(FTimespan _wait, intptr_t _option)
{
	// declare) 
	lockable<std::vector<object_ptr<net::io::Isocket>>> list_socket;

	// 1) list socket을 복사한다. (주의! 복사해서 process_socket_io()를 돌리지 않으면 iterator 무효화 문제가 발생할 수 있다.
	scoped_lock(m_list_socket)
	{
		list_socket = m_list_socket;
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

inline own_ptr<executor::socket> executor::socket::initialize_instance(size_t _count_thread)
{
	// declare)
	auto pinstance = make_own<executor::socket>();

	// 1) start socket executor
	pinstance->start(_count_thread);

	// return)
	return pinstance;
}

inline void executor::socket::process_thread()
{
	++m_count_thread;

	while (m_flag_run)
	{
		execute(FTimespan(100));
	}

	--m_count_thread;
}

inline void executor::socket::_fn_thread(executor::socket* _this)
{
	_this->process_thread();
}

inline void executor::socket::run_executor()
{
	get_instance()->execute(FTimespan::Zero());
}


}


