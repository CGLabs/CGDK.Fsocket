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
	template <class TREF> class own_ptr;

//-----------------------------------------------------------------------------
//                                                                            
// object_ptr<T>
//                                                                            
//-----------------------------------------------------------------------------
template <class TREF>
class object_ptr : public _CGTRAITS_PTR<TREF>
{
// Definitions) 
public:	
			using value_type = typename _CGTRAITS_PTR<TREF>::value_type;
			using _mytype = object_ptr<TREF>;

// constructor/destructor) 
public:
			object_ptr() noexcept : m_ref(nullptr) {}
			template <class T>
			object_ptr(T* _ref)  noexcept : m_ref(_ref)							{ if(_ref!=nullptr) static_cast<Ireferenceable*>(_ref)->add_ref();}
			object_ptr(const object_ptr<TREF>& _ref)  noexcept : m_ref(_ref.get()){ if(m_ref!=nullptr) static_cast<Ireferenceable*>(m_ref)->add_ref(); }
			template <class T>
			object_ptr(const object_ptr<T>& _ref) noexcept : m_ref(_ref.get())	{ if(m_ref!=nullptr) static_cast<Ireferenceable*>(m_ref)->add_ref();}
			object_ptr(object_ptr<TREF>&& _ref) noexcept : m_ref(_ref.get())	{ _ref._clear();}
			template <class T>													  
			object_ptr(object_ptr<T>&&  _ref) noexcept : m_ref(_ref.get())		{ _ref._clear(); }
			template <class T>													  
			object_ptr(own_ptr<T>&&) noexcept = delete;							  
			template <class T>													  
			object_ptr(const own_ptr<T>& _ref)  noexcept : m_ref(_ref.get())	{ if(m_ref != nullptr) static_cast<Ireferenceable*>(m_ref)->add_ref(); }
			~object_ptr() noexcept												{ if(m_ref != nullptr) { m_ref->release(); _clear(); }}

// Members)
protected:
			value_type*		m_ref;

// Publics) 
public:
	// 1) basic operating functions
			void				reset() noexcept								{ _reset();}
			template <class T>													  
			void				reset(T* _ref) noexcept							{ _reset(_ref);}
			template <class T>													  
			void				reset(const object_ptr<T>& _ref) noexcept		{ _reset(_ref.get());}
			template <class T>													  
			void				reset(object_ptr<T>&& _ref) noexcept			{ move<T>(_ref);}
			template <class T>													  
			void				swap(object_ptr<T>& _ref) noexcept				{ value_type* temp=m_ref; m_ref=_ref.m_ref; _ref.m_ref=temp;}
			void				detach() noexcept								{ _clear(); }

			template <class T>													  
			void				move(object_ptr<T>& _ref) noexcept				{ auto p=_ref.get(); auto old=m_ref; if(p==old) return; m_ref=p; _ref._clear(); if(old!=nullptr) static_cast<Ireferenceable*>(old)->release();}
			template <class T>													  
			void				move(object_ptr<T>&& _rhs) noexcept				{ auto p = m_ref; m_ref = _rhs.get(); _rhs._clear(); if (p != nullptr) static_cast<Ireferenceable*>(p)->release(); }
			value_type*			get() const noexcept							{ return m_ref;}
			value_type**		get_ptr() noexcept								{ return &m_ref;}
			const value_type**	get_ptr() const noexcept						{ return &m_ref;}
																				  
			bool				exist() const noexcept							{ return m_ref!=nullptr;}
			bool				empty() const noexcept							{ return m_ref==nullptr;}
																				  
	// 2) operator- Assignment-A												  
			_mytype&			operator=(value_type* _rhs) noexcept			{ _reset(_rhs); return (*this);}
			template <class T>													  
			_mytype&			operator=(T* _rhs) noexcept						{ _reset(_rhs); return (*this);}
			_mytype&			operator=(const _mytype& _rhs) noexcept			{ _reset(_rhs.get()); return (*this);}
			_mytype&			operator=(_mytype&& _rhs) noexcept				{ move(_rhs); return (*this);}
			_mytype&			operator=(const own_ptr<TREF>&) noexcept;
			_mytype&			operator=(own_ptr<TREF>&&) = delete;

	// 3) operator- Assignment-B
			template <class T>
			_mytype&			operator=(const object_ptr<T>& _rhs) noexcept	{ _reset(_rhs.get()); return (*this);}
			template <class T>													  
			_mytype&			operator=(object_ptr<T>&& _rhs) noexcept		{ move(_rhs); return (*this);}
			template <class T>
			_mytype&			operator=(const own_ptr<T>& _rhs) noexcept;
			template <class T>
			_mytype&			operator=(own_ptr<T>&& _rhs) noexcept;

	// 4) Operator- 
			value_type*			operator->() const noexcept	{ check(m_ref!=nullptr); return m_ref;}
			value_type&			operator*() const noexcept { check(m_ref!=nullptr); return *m_ref;}
			value_type&			operator[](size_t _index) const	{ check(m_ref!=nullptr); return m_ref[_index];}

	// 5) operator- Conversion
			//template <class T, class TFLAG=std::enable_if_t<std::is_base_of_v<std::decay_t<T>, TREF>, void>>
								//operator	T*() const noexcept	{ return m_ref;}
								operator value_type*() const noexcept { return m_ref; }

// implementation) 
public:
			void				_clear() noexcept { m_ref=nullptr;}
protected:
			void				_reset() noexcept { value_type* ref_old=m_ref; if(ref_old==nullptr) return; _clear(); static_cast<Ireferenceable*>(ref_old)->release();}
			void				_reset(value_type* _ref) noexcept { value_type* ref_old=m_ref; if(_ref==ref_old) return; m_ref=_ref; if(_ref!=nullptr) static_cast<Ireferenceable*>(_ref)->add_ref(); if(ref_old!=nullptr) static_cast<Ireferenceable*>(ref_old)->release();}

protected:
	#if defined(_MSC_VER)
			friend class object_ptr<TREF>;
			friend class own_ptr<TREF>;
#endif
};

template <class TONE, class TOTHER>	bool operator==(const object_ptr<TONE>& _lhs, const object_ptr<TOTHER>& _rhs) noexcept	{ return (_lhs.get() == _rhs.get());}
template <class TONE, class TOTHER>	bool operator!=(const object_ptr<TONE>& _lhs, const object_ptr<TOTHER>& _rhs) noexcept	{ return (_lhs.get() != _rhs.get());}
template <class TONE, class TOTHER>	bool operator> (const object_ptr<TONE>& _lhs, const object_ptr<TOTHER>& _rhs) noexcept	{ return (_lhs.get() > _rhs.get());}
template <class TONE, class TOTHER>	bool operator>=(const object_ptr<TONE>& _lhs, const object_ptr<TOTHER>& _rhs) noexcept	{ return (_lhs.get() >= _rhs.get());}
template <class TONE, class TOTHER>	bool operator< (const object_ptr<TONE>& _lhs, const object_ptr<TOTHER>& _rhs) noexcept	{ return (_lhs.get() < _rhs.get());}
template <class TONE, class TOTHER>	bool operator<=(const object_ptr<TONE>& _lhs, const object_ptr<TOTHER>& _rhs) noexcept	{ return (_lhs.get() <= _rhs.get());}
																															  
template <class TONE, class TOTHER>	bool operator==(const own_ptr<TONE>& _lhs, const object_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() == _rhs.get());}
template <class TONE, class TOTHER>	bool operator!=(const own_ptr<TONE>& _lhs, const object_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() != _rhs.get());}
template <class TONE, class TOTHER>	bool operator> (const own_ptr<TONE>& _lhs, const object_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() > _rhs.get());}
template <class TONE, class TOTHER>	bool operator>=(const own_ptr<TONE>& _lhs, const object_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() >= _rhs.get());}
template <class TONE, class TOTHER>	bool operator< (const own_ptr<TONE>& _lhs, const object_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() < _rhs.get());}
template <class TONE, class TOTHER>	bool operator<=(const own_ptr<TONE>& _lhs, const object_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() <= _rhs.get());}
																															  
template <class TONE, class TOTHER>	bool operator==(const object_ptr<TONE>& _lhs, const own_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() == _rhs.get());}
template <class TONE, class TOTHER>	bool operator!=(const object_ptr<TONE>& _lhs, const own_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() != _rhs.get());}
template <class TONE, class TOTHER>	bool operator> (const object_ptr<TONE>& _lhs, const own_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() > _rhs.get());}
template <class TONE, class TOTHER>	bool operator>=(const object_ptr<TONE>& _lhs, const own_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() >= _rhs.get());}
template <class TONE, class TOTHER>	bool operator< (const object_ptr<TONE>& _lhs, const own_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() < _rhs.get());}
template <class TONE, class TOTHER>	bool operator<=(const object_ptr<TONE>& _lhs, const own_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() <= _rhs.get());}
																															  
template <class TONE, class TOTHER>	bool operator==(const own_ptr<TONE>& _lhs, const own_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() == _rhs.get());}
template <class TONE, class TOTHER>	bool operator!=(const own_ptr<TONE>& _lhs, const own_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() != _rhs.get());}
template <class TONE, class TOTHER>	bool operator> (const own_ptr<TONE>& _lhs, const own_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() > _rhs.get());}
template <class TONE, class TOTHER>	bool operator>=(const own_ptr<TONE>& _lhs, const own_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() >= _rhs.get());}
template <class TONE, class TOTHER>	bool operator< (const own_ptr<TONE>& _lhs, const own_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() < _rhs.get());}
template <class TONE, class TOTHER>	bool operator<=(const own_ptr<TONE>& _lhs, const own_ptr<TOTHER>& _rhs) noexcept		{ return (_lhs.get() <= _rhs.get());}
																															  
template <class TONE, class TPTR>	bool operator==(const object_ptr<TONE>& _lhs, TPTR* _rhs) noexcept						{ return (_lhs.get() == _rhs);}
template <class TONE, class TPTR>	bool operator!=(const object_ptr<TONE>& _lhs, TPTR* _rhs) noexcept						{ return (_lhs.get() != _rhs);}
template <class TONE, class TPTR>	bool operator> (const object_ptr<TONE>& _lhs, TPTR* _rhs) noexcept						{ return (_lhs.get() > _rhs);}
template <class TONE, class TPTR>	bool operator>=(const object_ptr<TONE>& _lhs, TPTR* _rhs) noexcept						{ return (_lhs.get() >= _rhs);}
template <class TONE, class TPTR>	bool operator< (const object_ptr<TONE>& _lhs, TPTR* _rhs) noexcept						{ return (_lhs.get() < _rhs);}
template <class TONE, class TPTR>	bool operator<=(const object_ptr<TONE>& _lhs, TPTR* _rhs) noexcept						{ return (_lhs.get() <= _rhs);}
																															  
template <class TONE, class TPTR>	bool operator==(const own_ptr<TONE>& _lhs, TPTR* _rhs) noexcept							{ return (_lhs.get() == _rhs);}
template <class TONE, class TPTR>	bool operator!=(const own_ptr<TONE>& _lhs, TPTR* _rhs) noexcept							{ return (_lhs.get() != _rhs);}
template <class TONE, class TPTR>	bool operator> (const own_ptr<TONE>& _lhs, TPTR* _rhs) noexcept							{ return (_lhs.get() > _rhs);}
template <class TONE, class TPTR>	bool operator>=(const own_ptr<TONE>& _lhs, TPTR* _rhs) noexcept							{ return (_lhs.get() >= _rhs);}
template <class TONE, class TPTR>	bool operator< (const own_ptr<TONE>& _lhs, TPTR* _rhs) noexcept							{ return (_lhs.get() < _rhs);}
template <class TONE, class TPTR>	bool operator<=(const own_ptr<TONE>& _lhs, TPTR* _rhs) noexcept							{ return (_lhs.get() <= _rhs);}

template <class TONE, class TOTHER>	void swap_object(object_ptr<TONE>& _lhs, object_ptr<TOTHER>& _rhs) noexcept	
{
	_lhs.swap(_rhs);
}

template <class TTARGET, class TSOURCE>
object_ptr<TTARGET> static_pointer_cast(const object_ptr<TSOURCE>& _source) noexcept
{
	return object_ptr<TTARGET>(static_cast<TTARGET*>(_source.get()));
}

template <class TTARGET, class TSOURCE>
object_ptr<TTARGET> static_pointer_cast(const own_ptr<TSOURCE>& _source) noexcept
{
	return object_ptr<TTARGET>(static_cast<TTARGET*>(_source.get()));
}

template <class TTARGET, class TSOURCE>
object_ptr<TTARGET> const_pointer_cast(const object_ptr<TSOURCE>& _source) noexcept
{
	return object_ptr<TTARGET>(const_cast<TTARGET*>(_source.get()));
}

template <class TTARGET, class TSOURCE>
object_ptr<TTARGET> const_pointer_cast(const own_ptr<TSOURCE>& _source) noexcept
{
	return object_ptr<TTARGET>(const_cast<TTARGET*>(_source.get()));
}

template <class TTARGET, class TSOURCE>
object_ptr<TTARGET> dynamic_pointer_cast(const object_ptr<TSOURCE>& _source) noexcept
{
	return object_ptr<TTARGET>(dynamic_cast<TTARGET*>(_source.get()));
}

template <class TTARGET, class TSOURCE>
object_ptr<TTARGET> dynamic_pointer_cast(const own_ptr<TSOURCE>& _source) noexcept
{
	return object_ptr<TTARGET>(dynamic_cast<TTARGET*>(_source.get()));
}

template <class TTYPE>
struct hash_object_ptr
{
	size_t operator()(const object_ptr<TTYPE>& _rhs) const noexcept
	{
		return	static_cast<size_t>(_rhs.get());
	}
};

template <class TTYPE>
struct hash_own_ptr
{
	size_t operator()(const own_ptr<TTYPE>& _rhs) const noexcept
	{
		return	static_cast<size_t>(_rhs.get());
	}
};

}

namespace std
{
	//template<class TKEY>
	//struct hash<CGDK::object_ptr<TKEY>>
	//{
	//	using argument_type = TKEY*;
	//	using result_type = size_t;

	//	size_t operator()(const CGDK::object_ptr<TKEY>& _keyval) const
	//	{
	//		return (std::_Hash_seq(reinterpret_cast<const unsigned char *>(_keyval.get()), sizeof(TKEY*)));
	//	}
	//};

	//template<class TKEY>
	//struct hash<CGDK::own_ptr<TKEY>>
	//{
	//	using argument_type = TKEY*;
	//	using result_type = size_t;

	//	size_t operator()(const CGDK::own_ptr<TKEY>& _keyval) const
	//	{
	//		return (std::_Hash_seq(reinterpret_cast<const unsigned char *>(_keyval.get()), sizeof(TKEY*)));
	//	}
	//};
}
