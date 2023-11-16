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

namespace CGDK
{
//-----------------------------------------------------------------------------
//                                                                            
// Ireferenceable
//                                                                            
//-----------------------------------------------------------------------------
class Ireferenceable : virtual public Idisposable
{
// constructor/destructor) 
protected:
			Ireferenceable() noexcept : m_counter(0) {}
			Ireferenceable(const Ireferenceable&) noexcept : m_counter(0) {}
			Ireferenceable(Ireferenceable&&) noexcept : m_counter(0) {}
public:
	virtual	~Ireferenceable() noexcept {}

// Publics) 
#ifdef NDEBUG
	#define		DEBUG_ON_ADD_REFERENCECOUNT(count)	
	#define		DEBUG_ON_RELEASE_REFERENCECOUNT
#else
	#define		DEBUG_ON_ADD_REFERENCECOUNT(count)	_DEBUG_on_add_reference_count(count)
	#define		DEBUG_ON_RELEASE_REFERENCECOUNT		_DEBUG_on_release_reference_count()
#endif

public:
			void				reset_reference_count() noexcept { m_counter = 0;}
			void				set_reference_count(int _count) noexcept { m_counter = _count;}
			int					reference_count() const noexcept { return m_counter; }

			int					add_ref() noexcept { DEBUG_ON_ADD_REFERENCECOUNT(1); return ++m_counter;}
			int					add_ref(int _count) noexcept { DEBUG_ON_ADD_REFERENCECOUNT(_count); check(_count>=0); return m_counter += _count;}
			int					release() noexcept;

			Ireferenceable&		operator ++ () noexcept { add_ref(); return *this;}
			Ireferenceable&		operator -- () noexcept { release(); return *this;}
			void				operator += (int _rhs) noexcept { add_ref(_rhs);}

			Ireferenceable&		operator = (const Ireferenceable&) noexcept { return *this;}
			Ireferenceable&		operator = (Ireferenceable&&) noexcept { return *this; }

// framework) 
protected:
	virtual	void				on_final_release() noexcept {}

	// debug) 
#ifndef NDEBUG
	virtual	void				_DEBUG_on_add_reference_count(int /*_count*/){}
	virtual	void				_DEBUG_on_release_reference_count() {}
#endif
			void				_process_on_final_release() noexcept;

// implementation) 
private:
			std::atomic<int>	m_counter;
};


inline int Ireferenceable::release() noexcept
{
	// check) 
	DEBUG_ON_RELEASE_REFERENCECOUNT;

	// 1) decrease count
	const int result = --m_counter;

	// check)
	check(result >= 0);

	// 2) call '_process_on_final_release()' if count is zero
	if (result == 0)
	{
		_process_on_final_release();
	}

	// return) 
	return result;
}

inline void Ireferenceable::_process_on_final_release() noexcept
{
	// - call 'on_final_release()'
	on_final_release();

	// - reset debug info
	CGNEW_DEBUG_INFO_RESET(this);

	// - call 'process_dispose()'
	process_dispose();
}


}