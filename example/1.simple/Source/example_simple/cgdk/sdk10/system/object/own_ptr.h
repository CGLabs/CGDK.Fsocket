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

namespace CGDK
{
//-----------------------------------------------------------------------------
//                                                                            
// own_ptr<T>
//                                                                            
//-----------------------------------------------------------------------------
template <class TREF>
class own_ptr : public object_ptr<TREF>
{
// definitions) 
public:	
			struct CONTROL
			{
				object::Idestroyable*	pdestroyable;
				Imessage_transmitter*	pmessage_transmitter ;

				void reset() noexcept { pdestroyable=nullptr; pmessage_transmitter=nullptr;}
			};

			using value_type = typename _CGTRAITS_PTR<TREF>::value_type;
			using _mytype = own_ptr<TREF>;

			class  OWN_SET
			{
			public:
			explicit OWN_SET(value_type* _ptr, CONTROL _control) : ptr(_ptr), control(_control) {}
			public:
				value_type*	ptr;
				CONTROL control;
			};

// constructor/destructor) 
public:	
			own_ptr() noexcept {}
			template <class T>
			own_ptr(own_ptr<T>&& _ref) noexcept { move<T>(std::move(_ref));}
			~own_ptr() noexcept;

private:
			// ����) own_ptr ��ü ������ ���縦 ������� �ʴ´�. ���� move���� ����Ѵ�.
	explicit own_ptr(object_ptr<TREF>&& _ref) noexcept = delete;
			template <class T>
	explicit own_ptr(const own_ptr<T>& _ref) noexcept = delete;
			template <class T>
	explicit own_ptr(const object_ptr<T>& _ref) noexcept = delete;
			template <class T>
	explicit own_ptr(T* _ref) noexcept = delete;

public:
	explicit own_ptr(OWN_SET _set) noexcept : object_ptr<TREF>(_set.ptr), m_control(_set.control) {}


// Publics) 
public:
	// 1) basic operating functions
			void				destroy() noexcept;
			void				reset() noexcept { _reset_own(); }
			//template <class T>
			//void				force_assign(T* _ref) noexcept { _reset_own(_ref); }
			template <class T>
			void				move(own_ptr<T>&& _rhs) noexcept;
			void				detach() noexcept;

	// 2) operator- Assignment-Enabled
			template <class T>
			_mytype&			operator=(own_ptr<T>&& _rhs) noexcept { move<T>(std::move(_rhs)); return (*this);}

	// 3) operator- Conversion											  
								operator value_type*()	noexcept { return object_ptr<TREF>::m_ref;}
								operator const value_type*() const noexcept { return object_ptr<TREF>::m_ref;}

private:
	// 5) operator- Assignment-Disabled
			template <class T>
			_mytype&			operator=(object_ptr<T>&& _rhs) noexcept = delete;
			template <class T>
			_mytype&			operator=(const object_ptr<T>& _rhs) noexcept = delete;
			template <class T>
			_mytype&			operator=(const own_ptr<T>& _rhs) = delete;
			template <class T>
			_mytype&			operator=(T* _rhs) noexcept = delete;

protected:
	static	void				_destroy_own(value_type* _ref, CONTROL _control) noexcept;

			void				_reset_own() noexcept;
			void				_reset_own(value_type* _ref, CONTROL _control) noexcept;

public:
			CONTROL				m_control;
};

template <class TREF>
own_ptr<TREF>::~own_ptr() noexcept
{
	// 1) ���� m_ptr�� ��´�.
	value_type*	p = object_ptr<TREF>::get();
	CONTROL control = this->m_control;

	// 2) m_ptr�� nullptr�� �ƴϸ� destroyó���Ѵ�.
	if(p!=nullptr)
	{
		_destroy_own(p, control);
	}
}

template <class TREF>
void own_ptr<TREF>::destroy() noexcept
{
	object_ptr<TREF> ptr = *this;

	// 2) destroy�Ѵ�.
	if(ptr.exist())
	{
		_destroy_own(ptr.get(), this->m_control);
	}

	// 3) Pointer reset~
	object_ptr<TREF>::_reset();
}

template <class TREF>
template <class T>
void own_ptr<TREF>::move(own_ptr<T>&& _rhs) noexcept
{
	// 1)  copy m_ref temporary
	auto p = object_ptr<TREF>::m_ref;
	CONTROL control = this->m_control;

	// 2) destroy old object
	if(p != _rhs.get())
	{
		// - destry p
		if (p != nullptr)
			_destroy_own(p, control);

		// - copy ptr
		object_ptr<TREF>::m_ref = _rhs.get();
		this->m_control.pdestroyable = _rhs.m_control.pdestroyable;
		this->m_control.pmessage_transmitter = _rhs.m_control.pmessage_transmitter;
	}

	// 3) clear _rhs
	_rhs._clear();
	_rhs.m_control.reset();

	// 4) release if same ptr
	if (p != nullptr)
	{
		p->release();
	}
}

template <class TREF>
void own_ptr<TREF>::detach() noexcept
{
	object_ptr<TREF>::detach();
}

template <class TREF>
void own_ptr<TREF>::_destroy_own(value_type* _ref, CONTROL _control) noexcept
{
	//// 1) stop�� ���� �����Ѵ�.
	//if (_control.pstartable != nullptr)
	//{
	//	_control.pstartable->stop();
	//}

	// 2) destroy�� �����Ѵ�.
	if (_control.pdestroyable != nullptr)
	{
		_control.pdestroyable->destroy();
	}

	// 3) Dispatchable�� �����Ѵ�.
	if (_control.pmessage_transmitter != nullptr)
	{
		_control.pmessage_transmitter->reset_message_transmitter();
	}
}

template <class TREF>
void own_ptr<TREF>::_reset_own() noexcept
{
	// 1) ref_old�� ������ ���´�.
	auto ref_old = object_ptr<TREF>::m_ref;
	auto control_old = this->m_control;
	
	// check) ref_old�� nullptr�̸� ���⼭ ����.
	if (ref_old == nullptr)
		return;

	// 2) clear)
	object_ptr<TREF>::_clear();
	this->m_control.reset();

	// 3) destroy�� ȣ���Ѵ�.
	_destroy_own(ref_old, control_old);

	// 4) Reference Count�� ���δ�.
	static_cast<Ireferenceable*>(ref_old)->release();
}

template <class TREF>
void own_ptr<TREF>::_reset_own(value_type* _ref, CONTROL _control) noexcept
{
	// 1) ref_old�� ������ ���´�.
	auto ref_old = object_ptr<TREF>::m_ref;
	auto control_old = this->m_control;

	// check) ref_old�� nullptr�̸� ���⼭ ����.
	if (_ref == ref_old)
		return;

	// 2) destroy�� ȣ���Ѵ�.
	_destroy_own(ref_old, control_old);

	// 3) m_ref�� �����Ѵ�.
	object_ptr<TREF>::m_ref = _ref;
	this->m_control = _control;

	// 4) _ref�� nullptr�� �ƴϸ� add_ref �Ѵ�.
	if (_ref != nullptr)
		static_cast<Ireferenceable*>(_ref)->add_ref();
	
	// 5) ref_old�� nullptr�� �ƴϸ� Release �Ѵ�.
	if (ref_old != nullptr)
		static_cast<Ireferenceable*>(ref_old)->release();
}

template <class TREF>
template <class T>
typename object_ptr<TREF>::_mytype& object_ptr<TREF>::operator=(own_ptr<T>&& _rhs) noexcept
{
	// 1) ���� Move�Ѵ�.
	*this = _rhs.destroy();
	
	// return) 
	return (*this);
}

template <class TREF>
typename object_ptr<TREF>::_mytype& object_ptr<TREF>::operator=(const own_ptr<TREF>& _rhs) noexcept
{
	// 1) setting�Ѵ�.
	_reset(_rhs.get());
	
	// return) 
	return (*this);
}

template <class TREF>
template <class T>
typename object_ptr<TREF>::_mytype& object_ptr<TREF>::operator=(const own_ptr<T>& _rhs) noexcept
{
	// 1) setting�Ѵ�.
	_reset(_rhs.get());
	
	// return) 
	return (*this);
}


}
