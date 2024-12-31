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

	//! @brief 'queue�� �ڵ��� ���δ�. @return true ���� @return false ����
	virtual	bool				attach(TSharedPtr<net::io::Isocket>&& _psocket);
	virtual	bool				detach(net::io::Isocket* _psocket) noexcept;

	// 2) execute�Լ���...
	//! @brief ������ ó���ϴ� �Լ� @param _tick_diff �ִ� ���� ��� �ð�(�� �ð� ���� ó���� ���� ������ TIME_OUT���� �����Ѵ�.) @return	true ���� @return false ����
	virtual	bool				execute(FTimespan _wait = FTimespan::MaxValue(), intptr_t _option = 0);

	//! @brief ť�׵� '���� ��ü'�� ��� �����Ѵ�.
	virtual	void				execute_queued_all() noexcept;

// implementation)
protected:
			FCriticalSection	m_cs_list_socket;
			std::vector<TSharedPtr<net::io::Isocket>> m_list_socket;
			std::vector<std::thread> m_list_thread;

public:
	static	TSharedPtr<socket>&	get_instance()
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

public:
	static	void				initialize_instance(size_t _count_thread = 0);
	static	bool				run_executor();
	static	void				destory_executor() noexcept;

	static	eSTATUS&			_status()
	{
		static eSTATUS status{ eSTATUS::NONE };
		return status;
	}
	static	void				_fn_thread(socket* _this);
			void				process_thread();
	static	TSharedPtr<socket>	m_g_instance;
};

inline bool executor::socket::start(size_t _count_thread)
{
	// lock)
	FScopeLock cs(&this->m_cs_list_socket);

	// check)
	if (this->_status() != eSTATUS::NONE)
		return false;

	// 1) set flat
	this->_status() = eSTATUS::RUN;

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
		this->_status() = eSTATUS::NONE;

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

	// 1) reset thread
	{
		// lock)
		FScopeLock cs(&this->m_cs_list_socket);

		// check) 
		if (this->_status() != eSTATUS::RUN)
			return;

		// - stop the threads
		if(this->m_list_socket.empty())
			this->_status() = eSTATUS::NONE;
		else
			this->_status() = eSTATUS::DESTORY;

		// - reset thread & copy sockets
		list_thread = std::move(this->m_list_thread);
		list_socket = this->m_list_socket;
	}

	// 2) close sockets
	for (auto& iter : list_socket)
		iter->process_closesocket();

	// 3) wait until socket 
	while (this->_status() != eSTATUS::NONE)
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
	if (this->_status() != eSTATUS::RUN)
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

	// 1) find psocket
	auto iter_find = std::find_if(this->m_list_socket.begin(), this->m_list_socket.end(), [=](const TSharedPtr<net::io::Isocket>& _iter) noexcept { return _iter.Get() == _psocket; });

	// check) no exist?
	RETURN_IF(iter_find == this->m_list_socket.end(), false);

	// 2) erase
	this->m_list_socket.erase(iter_find);

	// 3) reset status
	if (this->_status() == eSTATUS::DESTORY && this->m_list_socket.empty())
		this->_status() = eSTATUS::NONE;

	// return) 
	return true;
}

inline bool executor::socket::execute(FTimespan _wait, intptr_t _option)
{
	// declare) 
	std::vector<TSharedPtr<net::io::Isocket>> list_socket;

	// 1) list socket�� �����Ѵ�. (����! �����ؼ� process_socket_io()�� ������ ������ iterator ��ȿȭ ������ �߻��� �� �ִ�.
	{
		// lock) 
		FScopeLock cs(&this->m_cs_list_socket);

		// check)
		if (_status() == eSTATUS::NONE)
			return false;
		
		// - copy soocket list
		list_socket = this->m_list_socket;
	}

	// 2) ��� socket�� socket_io�� ȣ���Ѵ�.
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

		// check)
		if (_status() != eSTATUS::RUN)
			return;

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
	while (this->_status() != eSTATUS::NONE)
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


