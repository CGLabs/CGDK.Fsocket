//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                                  Common                                   *
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
//-----------------------------------------------------------------------------
//
// mutex
//
//-----------------------------------------------------------------------------
namespace CGDK
{

struct MUTEX_LOCK
{
public:
			MUTEX_LOCK() noexcept : lock_mutex()													{ check(false);}
			MUTEX_LOCK(MUTEX_LOCK&& _mutex ) noexcept												{ check(false);}
			MUTEX_LOCK(const MUTEX_LOCK&) noexcept = delete;										  
			MUTEX_LOCK(std::string_view _name) noexcept												{ check(false);}
			MUTEX_LOCK(std::wstring_view _name) noexcept											{ check(false);}
			~MUTEX_LOCK() noexcept																	{}

public:
			MUTEX_LOCK&			operator=(MUTEX_LOCK&& _rhs) noexcept								{ check(false); return *this;}
			MUTEX_LOCK&			operator=(const MUTEX_LOCK& _rhs) noexcept = delete;

public:
			lock_object			lock_mutex;
};


//-----------------------------------------------------------------------------
// 
// CGDK::lock_guard
// 
// 1. CGDK::lock_guard란..
//    1) Critical section 객체를 Set하게 되면 자동으로 EnterCriticalsection을
//       수행한다.
//    2) 객체가 파괴되면 자동으로 LeaveCriticalsection을 수행한다.
//    3) 파괴되기 이전에 release()를 수행하게 되면 객체를 해제하면서 Leave-
//       Criticalsection을 수행한다.
//    4) 단 객체를 생성할 때 반드시 Critical section 객체를 설정해야 합니다.
//       (중간에 객체를 설정해 놓을 수 없습니다.)
//     
//
//-----------------------------------------------------------------------------
// 1) unique_lock
template <class Tlockable, class TENABLE = void>
class unique_lock_any : public std::unique_lock<lock_object>
{
public:
				unique_lock_any() noexcept : std::unique_lock<lock_object>() {}
				unique_lock_any(lock_object& _mtx) noexcept : std::unique_lock<lock_object>(_mtx)	{}
				unique_lock_any(lock_object& _mtx, std::defer_lock_t _defer_lock_t) noexcept : std::unique_lock<lock_object>(_mtx, _defer_lock_t) {}
				unique_lock_any(lock_object& _mtx, std::try_to_lock_t _try_to_lock_t) noexcept : std::unique_lock<lock_object>(_mtx, _try_to_lock_t) {}

	template<class _Rep, class _Period>
				unique_lock_any(lock_object& _mtx, const std::chrono::duration<_Rep, _Period>& _Rel_time) noexcept : std::unique_lock<lock_object>(_mtx, _Rel_time) {}
	template<class _Clock, class _Duration>
				unique_lock_any(lock_object& _mtx, const std::chrono::time_point<_Clock, _Duration>& _Abs_time) noexcept : std::unique_lock<lock_object>(_mtx, _Abs_time) {}

	constexpr	operator bool() { return false; }
};

template<class Tlockable>
class unique_lock_any<Tlockable, typename std::enable_if<std::is_base_of<std::mutex, Tlockable>::value, void>::type> : public std::unique_lock<std::mutex>
{
public:
				unique_lock_any() noexcept : std::unique_lock<std::mutex>() {}
				unique_lock_any(std::mutex& _mtx) noexcept : std::unique_lock<std::mutex>(_mtx)	{}
				unique_lock_any(std::mutex& _mtx, std::defer_lock_t _defer_lock_t) noexcept : std::unique_lock<std::mutex>(_mtx, _defer_lock_t) {}
				unique_lock_any(std::mutex& _mtx, std::try_to_lock_t _try_to_lock_t) noexcept : std::unique_lock<std::mutex>(_mtx, _try_to_lock_t) {}
	template<class _Rep, class _Period>
				unique_lock_any(std::mutex& _mtx, const std::chrono::duration<_Rep, _Period>& _Rel_time) noexcept : std::unique_lock<std::mutex>(_mtx, _Rel_time) {}
	template<class _Clock, class _Duration>
				unique_lock_any(std::mutex& _mtx, const std::chrono::time_point<_Clock, _Duration>& _Abs_time) noexcept : std::unique_lock<std::mutex>(_mtx, _Abs_time) {}

				constexpr	operator bool() { return false; }
};

template<class Tlockable>
class unique_lock_any<Tlockable, typename std::enable_if<std::is_base_of<std::recursive_mutex, Tlockable>::value, void>::type> : public std::unique_lock<std::recursive_mutex>
{
public:
				unique_lock_any() noexcept : std::unique_lock<std::recursive_mutex>() {}
				unique_lock_any(std::recursive_mutex& _mtx) noexcept : std::unique_lock<std::recursive_mutex>(_mtx)	{}
				unique_lock_any(std::recursive_mutex& _mtx, std::defer_lock_t _defer_lock_t) noexcept : std::unique_lock<std::recursive_mutex>(_mtx, _defer_lock_t) {}
				unique_lock_any(std::recursive_mutex& _mtx, std::try_to_lock_t _try_to_lock_t) noexcept : std::unique_lock<std::recursive_mutex>(_mtx, _try_to_lock_t) {}
	template<class _Rep, class _Period>
				unique_lock_any(std::recursive_mutex& _mtx, const std::chrono::duration<_Rep, _Period>& _Rel_time) noexcept : std::unique_lock<std::recursive_mutex>(_mtx, _Rel_time) {}
	template<class _Clock, class _Duration>
				unique_lock_any(std::recursive_mutex& _mtx, const std::chrono::time_point<_Clock, _Duration>& _Abs_time) noexcept : std::unique_lock<std::recursive_mutex>(_mtx, _Abs_time) {}

				constexpr	operator bool() { return false; }
};

//template<class Tlockable>
//class unique_lock_any<Tlockable, typename std::enable_if<std::is_base_of<MUTEX_LOCK, Tlockable>::value, void>::type> : public std::unique_lock<std::timed_mutex>
//{
//public:
//				unique_lock_any() noexcept : pMutex(nullptr) {}
//				unique_lock_any(MUTEX_LOCK& _mtx) noexcept : pMutex(&_mtx)			{ lock(); }
//				unique_lock_any(MUTEX_LOCK& _mtx, std::defer_lock_t /*_defer_lock_t*/) noexcept : pMutex(&_mtx)  {}
//				unique_lock_any(MUTEX_LOCK& _mtx, std::try_to_lock_t /*_try_to_lock_t*/) noexcept : pMutex(&_mtx) { lock(); }
//	template<class _Rep, class _Period>
//				unique_lock_any(MUTEX_LOCK& _mtx, const std::chrono::duration<_Rep, _Period>& _Rel_time) noexcept : pMutex(&_mtx) { try_lock_for(_Rel_time); }
//	template<class _Clock, class _Duration>
//				unique_lock_any(MUTEX_LOCK& _mtx, const std::chrono::time_point<_Clock, _Duration>& _Abs_time) noexcept : pMutex(&_mtx) { try_lock_until(_Abs_time); }
//				~unique_lock_any() noexcept { if (pMutex != nullptr) { pMutex->unlock(); pMutex=nullptr; } }
//public:
//	bool		try_lock() noexcept														{ if(pMutex==nullptr) return false; return pMutex->try_lock();}
//	template<class _Rep, class _Period>
//	bool		try_lock_for(const std::chrono::duration<_Rep, _Period>& _Rel_time) noexcept { if (pMutex == nullptr) return false; return pMutex->try_lock_for(_Rel_time); }
//	template<class _Clock, class _Duration>
//	bool		try_lock_until(const std::chrono::time_point<_Clock, _Duration>& _Abs_time) noexcept { if (pMutex == nullptr) return false; return pMutex->try_lock_until(_Abs_time); }
//	bool		try_lock_until(const xtime *_Abs_time) noexcept							{ if (pMutex == nullptr) return false; return pMutex->try_lock_until(_Abs_time); }
//	void		unlock() noexcept														{ if (pMutex == nullptr) return false; return pMutex->unlock(); }
//	void		swap(unique_lock_any& _other) noexcept									{ std::swap(pMutex, _other.pMutex);}
//	void		release() noexcept														{ pMutex=nullptr;}
//	bool		owns_lock() const noexcept												{ return pMutex != nullptr;}
//
//	constexpr	operator bool() { return false; }
//
//private:
//				MUTEX_LOCK*				pMutex;
//};


}


// ----------------------------------------------------------------------------
// 4. Critical Section Lock
//    - 이것은 지정된 Block을 이용해서 지정된 영역을 벗어날 경우 자동으로
//      CriticalSection을 벗어나게 하게 하기 위한 것이다.
// 
// ----------------------------------------------------------------------------
// 1) lock_guard
#define	scoped_lock(cs)						if(unique_lock_any<typename std::decay<decltype(cs)>::type> _UNIQUE_TEMPORARY_NAME(_tempCSLock)=cs) /*throw std::exception()*/; else
#define	synchronized(cs)					scoped_lock(cs)

// 2) unique lock
#define	scoped_unique_lock(cs)				if(unique_lock_any<typename std::decay<decltype(cs)>::type> _UNIQUE_TEMPORARY_NAME(_temp_cs_lock) = const_cast<typename std::decay<decltype(cs)>::type&>(cs)) /*throw std::exception()*/; else
#define	scoped_unique_lock_named(cs)		if(unique_lock_any<typename std::decay<decltype(cs)>::type> name(_temp_cs_lock) = const_cast<typename std::decay<decltype(cs)>::type&>(cs)) /*throw std::exception()*/; else

// 3) shared lock
#define	scoped_shared_lock(cs)				if(shared_lock_any<typename std::decay<decltype(cs)>::type> _UNIQUE_TEMPORARY_NAME(_temp_cs_lock) = const_cast<typename std::decay<decltype(cs)>::type&>(cs)) /*throw std::exception()*/; else
#define	scoped_shared_lock_named(cs)		if(shared_lock_any<typename std::decay<decltype(cs)>::type> name(_temp_cs_lock) = const_cast<typename std::decay<decltype(cs)>::type&>(cs)) /*throw std::exception()*/; else

// 4) for capability
#define	scoped_lock_named(cs, name)			scoped_unique_lock_named(cs, name)
#define	scoped_lock_leave(name)				name.unlock();
