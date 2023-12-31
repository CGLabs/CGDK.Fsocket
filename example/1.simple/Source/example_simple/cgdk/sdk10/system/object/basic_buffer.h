//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                            CGBuffer Templates                             *
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
// CGDK::basic_buffer
//
//-----------------------------------------------------------------------------
class basic_buffer : public _buffer_view<char>
{
// traits) 
public:
			using base_t	  = _buffer_view<char>;
			using self_t	  = basic_buffer;
			using traits	  = typename base_t::traits;
			using element_t	  = typename base_t::element_t;
			using size_type	  = typename base_t::size_type;

			template<class T> using peek_tr = PEEK_tr<basic_buffer, T>;
			template<class T> using extr_tr = EXTR_tr<basic_buffer, T>;
			template<class T> using appd_tr = APPD_tr<basic_buffer, T>;
			template<class T> using prpd_tr = PRPD_tr<basic_buffer, T>;

// constructor/destructor) 
public:
	constexpr basic_buffer() noexcept {}
	constexpr basic_buffer(const basic_buffer& _buffer) noexcept : base_t(_buffer), bound(_buffer.get_bound()) {}
	constexpr basic_buffer(basic_buffer&& _buffer) noexcept : base_t(_buffer), bound(_buffer.get_bound()) {}
	template <class T>
	constexpr basic_buffer(_buffer_view<T> _buffer) noexcept : base_t(_buffer), bound{ _buffer.get_front_ptr(), _buffer.get_back_ptr() } {}
	template <class T>
	constexpr basic_buffer(_buffer_view<T> _buffer, const buffer_bound& _bound) noexcept : base_t(_buffer), bound(_bound) {}
	template <class T>
	constexpr basic_buffer(_buffer_view<T> _buffer, buffer_bound&& _bound) noexcept : base_t(_buffer), bound(_bound) {}
	template <class T>
	constexpr basic_buffer(std::basic_string_view<T> _string) noexcept : base_t{ _string }, bound{ _string.data(), _string.data() + _string.size() } {}

protected:
			basic_buffer(Imemory* _pbuffer) noexcept : base_t((_pbuffer != nullptr) ? *_pbuffer : base_t()), bound((_pbuffer != nullptr) ? _pbuffer->get_bound() : buffer_bound()) {}
			basic_buffer(object_ptr<Imemory>&& _pbuffer) noexcept : base_t((_pbuffer.exist()) ? *_pbuffer : base_t()), bound((_pbuffer.exist()) ? _pbuffer->get_bound() : buffer_bound()) {}

public:
	// 1) capacity/clear/copy/clone
	constexpr void				resize(std::size_t _new_size)													{ if (data() + _new_size > get_upper_bound()) throw std::length_error("error! data_ is out of upper bound 'resize(size_t)'"); size_ = _new_size;}
			std::size_t			capacity() const noexcept														{ return (bound.upper != nullptr) ? (reinterpret_cast<const char*>(bound.upper) - data_) : 0;}
	constexpr void				clear() noexcept																{ base_t::clear(); bound.reset();}
			void				copy(const base_t& _source)														{ if (data_ + _source.size() > get_upper_bound()) throw std::length_error("error! data_ overflow 'copy()'"); memcpy(data(), _source.data(), _source.size()); size_ = _source.size();}
	constexpr void				swap(basic_buffer& _rhs) noexcept												{ base_t::swap(_rhs); auto p = _rhs.get_bound(); _rhs._set_bound(bound); bound = p; }
	constexpr void				swap(base_t& _rhs)																{ if (_rhs.data() < get_lower_bound() || (_rhs.data() + _rhs.size()) > get_upper_bound()) { assert(false); throw std::out_of_range("_rhs data_ is must in bound of this shared_buffer"); } base_t::swap(_rhs); }

			//shared_buffer		clone(CGNEW_DEBUG_INFO_PARAMETERS_WITH_DEFAULT) const;
			basic_buffer		split_head(const base_t& _source)
			{
				_CGD_BUFFER_BOUND_CHECK(size_ >= _source.size());
				char* p = data();
				memcpy(data(), _source.data(), _source.size());
				data_+= _source.size();
				size_-=_source.size();
				return basic_buffer { buffer_view{p, _source.size()}, bound };
			}
			basic_buffer		split_tail(const base_t& _source)
			{
				_CGD_BUFFER_BOUND_CHECK(size_ >= _source.size());
				memcpy(data() + size_ - _source.size(), _source.data(), _source.size());
				size_ -= _source.size();
				return basic_buffer{ buffer_view{data() + size(), _source.size()}, bound };
			}
																												  
	// 2) prepend																								  
			template <std::size_t ISIZE>
	constexpr auto				prepend()																		{ return _prepend_skip(ISIZE);}
			template <class T>																					  
	constexpr T&				prepend()																		{ return *reinterpret_cast<T*>(_prepend_skip(sizeof(T)));}
			template <class T>																					  
	constexpr prpd_tr<T>		prepend(const T& _data)															{ return PRPD_t<basic_buffer, T>::_do(*this, _data);;}
			template <class T>																					  
	constexpr prpd_tr<T>		prepend(T&& _data)																{ return PRPD_t<basic_buffer, T>::_do(*this, _data);;}
	//		template <class T, std::size_t N>
	//constexpr typename serializer_prepend<T[N]>::type prepend(const T (&_data)[N])								{ return _prepend(_data);}
			template <class T> 																					  
	constexpr base_t			prepend(const T* _data, std::size_t _count)										{ return _prepend_array(_data, _count);}
			template <class T> 																					  
	constexpr auto				prepend(const std::initializer_list<T>& _list)									{ _prepend_initializer_list<T>(_list); return data();}
			template<class I> 																					  
	constexpr std::enable_if_t<is_iterator<I>::value, base_t>
								prepend(I _first, I _last)														{ return _prepend_iterator(_first, _last);}
			auto				prepend(std::size_t _size, const void* _buffer)									{ return _prepend_bytes(_size, _buffer);}
			template <class T>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								prepend_string(std::basic_string_view<T> _text)									{ return _prepend_string(_text);}
	//		template <class T, std::size_t N>
	//constexpr std::enable_if_t<is_string_type<T>::value, base_t>
	//							prepend_string(const T(&_text)[N])												{ return _prepend_string_pointer(_text);}
			template <class T, class F, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								prepend_string(const std::basic_string_view<T> _format, F&& _first, TREST&&... _rest)	{ return _prepend_string_format(_format.data(), std::forward<F>(_first), std::forward<TREST>(_rest)...);}
			template <class T, std::size_t N, class F, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								prepend_string(const T(&_format)[N], F&& _first, TREST&&... _rest)				{ return _prepend_string_format(_format, std::forward<F>(_first), std::forward<TREST>(_rest)...);}
			template <class T>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								prepend_text(std::string_view _text)											{ return _prepend_text(_text);}
			template <class T, std::size_t N>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								prepend_text(const T(&_text)[N])												{ return _prepend_text(_text);}
			template <class T, class F, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								prepend_text(std::basic_string_view<T> _format, F&& _first, TREST&&... _rest)	{ return _prepend_text_format(_format, std::forward<F>(_first), std::forward<TREST>(_rest)...);}
			template <class T, std::size_t N, class F, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								prepend_text(const T(&_format)[N], F&& _first, TREST&&... _rest)				{ return _prepend_text_format(_format, std::forward<F>(_first), std::forward<TREST>(_rest)...);}

	// 3) append
			template <std::size_t ISIZE>
	constexpr auto&				append()																		{ return _append_skip(ISIZE);}
			template <class T>																					  
	constexpr auto&				append()																		{ return _append_emplace<T>();}
			template <class T>																					  
	constexpr appd_tr<T>		append(const T& _data)															{ return APPD_t<basic_buffer,T>::_do(*this, _data); }
			template <class T>																					  
	constexpr appd_tr<T>		append(T&& _data)																{ return APPD_t<basic_buffer,T>::_do(*this, std::forward<T>(_data)); }
	//		template <class T, std::size_t N>
	//constexpr appd_tr<T>			append(const std::remove_const_t<T>(&_data)[N])									{ return APPD_t<basic_buffer,T>::_do(*this, _data); }
			template <class T>																					  
	constexpr auto				append(const T* _data, std::size_t _count)										{ return _append_array(_data, _count);}
			template <class T> 																					  
	constexpr base_t			append(const std::initializer_list<T>& _List)									{ return _append_initializer_list<T>(_List);}
			template<class I> 																					  
	constexpr std::enable_if_t<is_iterator<I>::value, base_t>
								append(I _first, I _last)														{ return _append_iterator(_first, _last);}
	constexpr auto				append(std::size_t _size, const void* _buffer)									{ return _append_bytes(_size, _buffer); }

			template <class T, class F, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								append(std::basic_string_view<T> _format, F&& _first, TREST&&... _rest)			{ return _append_string_format(_format, std::forward<F>(_first), std::forward<TREST>(_rest)...);}
			template <class T, std::size_t N, class F, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								append(const T(&_format)[N], F&& _first, TREST&&... _rest)						{ return _append_string_format(_format, std::forward<F>(_first), std::forward<TREST>(_rest)...);}
			template <class T>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								append_text(std::basic_string_view<T> _text)									{ return _append_text(_text);}
			template <class T, std::size_t N>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								append_text(const T(&_text)[N])													{ return _append_text(_text);}
			template <class T, class F, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								append_text(std::basic_string_view<T> _format, F&& _first, TREST&&... _rest)	{ return _append_text_format(_format, std::forward<F>(_first), std::forward<TREST>(_rest)...);}
			template <class T, std::size_t N, class F, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								append_text(const T(&_format)[N] , F&& _first, TREST&&... _rest)				{ return _append_text_format(_format, std::forward<F>(_first), std::forward<TREST>(_rest)...);}
			template <class T = char>
	constexpr auto				append_text(int8_t _value)														{ return _append_text<T>(static_cast<int64_t>(_value));}
			template <class T = char>
	constexpr auto				append_text(int16_t _value)														{ return _append_text<T>(static_cast<int64_t>(_value));}
			template <class T = char>
	constexpr auto				append_text(int32_t _value)														{ return _append_text<T>(static_cast<int64_t>(_value));}
			template <class T = char>
	constexpr auto				append_text(int64_t _value)														{ return _append_text<T>(static_cast<int64_t>(_value));}
			template <class T = char>
	constexpr auto				append_text(uint8_t _value)														{ return _append_text<T>(static_cast<uint64_t>(_value));}
			template <class T = char>
	constexpr auto				append_text(uint16_t _value)													{ return _append_text<T>(static_cast<uint64_t>(_value));}
			template <class T = char>
	constexpr auto				append_text(uint32_t _value)													{ return _append_text<T>(static_cast<uint64_t>(_value));}
			template <class T = char>
	constexpr auto				append_text(uint64_t _value)													{ return _append_text<T>(static_cast<uint64_t>(_value));}
							
	// 4) extract/subtract
			template <class T>											  
	constexpr extr_tr<T>		extract()																		{ return serializer_extract<basic_buffer, std::remove_const_t<T>>::_do(*this);}
			template <std::size_t ISIZE>
	constexpr auto				extract()																		{ return basic_buffer(base_t::extract(ISIZE), bound);}
	constexpr auto				extract(size_t _length)															{ return basic_buffer(base_t::extract(_length), bound);}
	constexpr auto				extract(skip _skip)																{ return basic_buffer(base_t::extract(_skip.amount), bound);}

	// 5) reference																									  
	constexpr const buffer_bound& get_bound() const noexcept													{ return bound;}
	constexpr std::size_t		get_remained_size() const noexcept												{ return static_cast<const char*>(bound.upper) - data_ - size_;}
																												  
	// 6) operator overloading																					  
			// [operator] +/-		
	constexpr basic_buffer		operator ~ () const																{ auto data_value = data_ + size_; return basic_buffer { base_t { data_value, static_cast<size_type>(static_cast<const element_t*>(bound.upper) - data_value) }, bound }; }
	constexpr basic_buffer		operator+(offset _rhs) const													{ return basic_buffer{ base_t::operator+(_rhs), bound };}
	constexpr basic_buffer		operator-(offset _rhs) const
			{
				// check) upper bound
				_CGD_BUFFER_BOUND_CHECK(this->data_ - _rhs.amount >= get_lower_bound());

				// return) 
				return basic_buffer(base_t{ this->data_ - _rhs.amount, this->size_ + _rhs.amount }, bound);
			}
	constexpr basic_buffer		operator+(CGDK::size _rhs) const
			{
				// check) upper bound
				_CGD_BUFFER_BOUND_CHECK(this->data_ + this->size_ + _rhs.amount <= get_upper_bound());

				// return) 
				return basic_buffer(base_t{ this->data_, this->size_ + _rhs.amount }, bound);
			}
	constexpr basic_buffer		operator-(CGDK::size _rhs) const												{ return basic_buffer{ base_t::operator-(_rhs), bound }; }
			// [operator] +=/-=
	constexpr basic_buffer&		operator+=(offset _rhs)															{ base_t::operator+=(_rhs); return *this; }
	constexpr basic_buffer&		operator-=(offset _rhs)
			{
				// check) upper bound
				_CGD_BUFFER_BOUND_CHECK(this->data_ - _rhs.amount >= get_lower_bound());

				this->data_ -= _rhs.amount;
				this->size_ += _rhs.amount;
				
				return *this;
			}
	constexpr basic_buffer		operator+=(CGDK::size _rhs)
			{
				// check) upper bound
				_CGD_BUFFER_BOUND_CHECK(this->data_ + this->size_ + _rhs.amount <= get_upper_bound());

				// - add
				this->size_ += _rhs.amount;

				// return) 
				return *this;
			}
	constexpr basic_buffer		operator-=(CGDK::size _rhs)														{ base_t::operator-=(_rhs); return *this; }
	constexpr basic_buffer&		operator+=(const base_t& _rhs)													{ _append_bytes(_rhs.size(), _rhs.data()); return *this;}
	constexpr basic_buffer&		operator+=(base_t&& _rhs)														{ _append_bytes(_rhs.size(), _rhs.data()); return *this;}
			template <class T>
	constexpr std::enable_if_t<is_linear_container_with_buffer<T>::value, basic_buffer&>
								operator+=(const T& _rhs)														{ for (auto& iter : _rhs) { if (iter.empty()) continue; _append_bytes(iter.size_, iter.data()); } return *this; }
			template <class T>
	constexpr std::enable_if_t<is_linear_container_with_buffer<T>::value, basic_buffer&>
								operator+=(T&& _rhs)															{ for (auto& iter : _rhs) { if (iter.empty()) continue; _append_bytes(iter.size_, iter.data()); } return *this; }
			template <class T, std::size_t N>
	constexpr std::enable_if_t<std::is_base_of_v<base_t, T>, basic_buffer&>
								operator+=(const std::array<T, N>& _rhs)										{ for(auto& iter: _rhs) { if (iter.empty()) continue; _append_bytes(iter.size_, iter.data());} return *this; }
			template <class T, std::size_t N>
	constexpr std::enable_if_t<std::is_base_of_v<base_t, T>, basic_buffer&>
								operator+=(std::array<T, N>&& _rhs)												{ for(auto& iter: _rhs) { if (iter.empty()) continue; _append_bytes(iter.size_, iter.data());} return *this; }
			template <class T, std::size_t N>
	constexpr std::enable_if_t<std::is_base_of_v<base_t, T>, basic_buffer&>
								operator+=(const base_t(&_rhs)[N] )												{ const T* iter = _rhs; const T* iter_end = _rhs + N; for(;iter != iter_end; ++iter) { if (iter->empty()) continue; _append_bytes(iter->size_, iter->data_); } return *this;	}
	constexpr basic_buffer&		operator+=(const basic_buffer& _rhs)											{ _append_bytes(_rhs.size_, _rhs.data()); return *this;}
	constexpr basic_buffer&		operator+=(basic_buffer&& _rhs)													{ _append_bytes(_rhs.size_, _rhs.data()); return *this;}
			// [operator] =
	constexpr basic_buffer&		operator= (const base_t& _rhs) noexcept											{ _check_bound(_rhs); base_t::operator=(_rhs); return *this; }
	constexpr basic_buffer&		operator= (base_t&& _rhs) noexcept												{ _check_bound(_rhs); base_t::operator=(_rhs); return *this; }
	constexpr basic_buffer&		operator= (const basic_buffer& _rhs) noexcept									{ base_t::operator=(_rhs); bound = _rhs.bound; return *this; }
	constexpr basic_buffer&		operator= (basic_buffer&& _rhs) noexcept										{ base_t::operator=(_rhs); bound = _rhs.bound; return *this; }
			basic_buffer&		operator= (Imemory* _rhs) noexcept												{ if(_rhs != nullptr) { base_t::operator=(_rhs); bound = _rhs->get_bound(); } else { clear();} return *this; }
			basic_buffer&		operator= (const object_ptr<Imemory>& _rhs) noexcept							{ if(_rhs.exist()) { base_t::operator=(_rhs); bound = _rhs->get_bound(); } else { clear();} return *this; }
			basic_buffer&		operator= (object_ptr<Imemory>&& _rhs) noexcept									{ if(_rhs.exist()) { base_t::operator=(_rhs); bound = _rhs->get_bound(); } else { clear();} return *this; }
			// [operator] ^=
			template<class T>
	constexpr basic_buffer&		operator^=(const _buffer_view<T>& _rhs)											{ base_t::operator=(_rhs); return *this;}
			template<class T>
	constexpr basic_buffer&		operator^=(_buffer_view<T>&& _rhs)												{ base_t::operator=(_rhs); return *this;}
			template<class T>
	constexpr basic_buffer&		operator^=(const buffer_base<T>& _rhs)											{ base_t::operator=(_rhs); return *this;}
			template<class T>
	constexpr basic_buffer&		operator^=(buffer_base<T>&& _rhs)												{ base_t::operator=(_rhs); return *this;}
	constexpr basic_buffer&		operator^=(std::size_t _rhs)													{ base_t::set_size(_rhs); return *this;}
			// [operator >> - extract
			template <class T>
			basic_buffer&		operator>>(T& _rhs)																{ _rhs = _extract<T>(); return *this;}
			// [operator] << - append																			  
			template <class T>																					  
			basic_buffer&		operator<<(const T& _rhs)														{ APPD_t<basic_buffer, T>::_do(*this, _rhs); return *this;}

public:
	// prepend) 
	template <class T>
	constexpr prpd_tr<T>		_prepend(const T& _data) { return PRPD_t<basic_buffer,T>::_do(*this, _data);}
	template <class T>
	constexpr prpd_tr<T>		_prepend(T&& _data) { return PRPD_t<basic_buffer,T>::_do(*this, _data); }
			//template <class T, std::size_t N>
			//typename serializer_prepend< std::remove_const_t<T>>::type _prepend(const T(&_data)[N]) { return serializer_prepend<typename std::remove_const_t<T>[N]>::_do(*this, _data);}
	constexpr base_t			_prepend_skip(std::size_t _size)
			{
				// check) param '_size' must be > 0
				CGDK_ASSERT(_size >= 0, throw std::invalid_argument("_size is invalid [0]"));

				// check) lower bound
				_CGD_BUFFER_BOUND_CHECK((data_ - _size) >= get_lower_bound());

				// 1) [원본_버퍼_포인터]를 _size만큼 줄인다.
				data_ -= _size;

				// 2) [원본_버퍼_길이]를 _size만큼 늘린다. 
				size_ += _size;

				// return) 
				return base_t(data_, _size);
			}

	template <class T>
	constexpr T&				_prepend_general(const T& _data)
	{
		// check) Pointer형이면 안됀다.
		static_assert(!std::is_pointer<T>::value, "Must not be Pointer Type!!");

		// check) lower bound
		_CGD_BUFFER_BOUND_CHECK((data_ - sizeof(T)) >= get_lower_bound());

		// 1) [원본_버퍼]에 값을 복사해 넣는다.
		*reinterpret_cast<T*>(data() - sizeof(T)) = _data;
	
		// 2) [원본_버퍼_포인터]의 크기를 sizeof(T)만큼 줄인다.
		data_ -= sizeof(T);
	
		// 3) [원본_버퍼_길이]를 sizeof(T)만큼 늘린다. 
		size_ += sizeof(T);
	
		// return) 
		return *reinterpret_cast<T*>(data());
	}
	template <class T> 
	constexpr std::enable_if_t<is_memcopy_able<T>::value, base_t>
								_prepend_array(const T* _data, std::size_t _count)
	{
		// check) _count가 0이하면 안된다.
		CGDK_ASSERT(_count >= 0, throw std::invalid_argument("_count is invalid [0]"));

		// 1)
		auto buf_old = data_;

		// 2) [데이터_갯수]만큼 데이터들을 통채로 복사한다.
		_prepend_bytes(sizeof(T) * _count, _data);

		// 3) [데이터_갯수]를 써넣는다.
		_prepend<COUNT_T>(static_cast<COUNT_T>(_count));

		// return) 
		return base_t(data_, buf_old - data_);
	}
	template <class T>
	constexpr std::enable_if_t<!is_memcopy_able<T>::value, base_t>
								_prepend_array(const T* _data, std::size_t _count)
	{
		// check) _count가 0이하면 안된다.
		CGDK_ASSERT(_count >= 0, throw std::invalid_argument("_count is invalid [0]"));

		// 1)
		auto buf_old = data_;

		// 2) [데이터_갯수]만큼 데이터들을 추가한다.
		for (int i = static_cast<int>(_count - 1); i >= 0; --i)
		{
			_prepend<T>(_data[i]);
		}

		// 3) [데이터_갯수]를 써넣는다.
		_prepend<COUNT_T>(static_cast<COUNT_T>(_count));

		// return) 
		return base_t(data_, buf_old - data_);
	}
	template <class T, std::size_t N>
	constexpr base_t			_prepend_string(const T(&_string)[N])
	{
		// 1) calculate string size ( string + NULL )
		constexpr auto bytes_copy = N * sizeof(T);
		constexpr auto added_length = sizeof(COUNT_T) + bytes_copy;

		// check) lower bound
		_CGD_BUFFER_BOUND_CHECK((data_ - added_length) >= get_lower_bound());

		// 2) [원본_버퍼_포인터]를 [문자열 길이] 만큼 뺀 [목표_포인터]를 구한다..
		auto p = data_ - bytes_copy;

		// 3) [원본_버퍼_포인터]에 문자열을 [문자열 길이] 만큼 복사한다.
		memcpy(p, _string, bytes_copy);

		// 4) 이동
		p -= sizeof(COUNT_T);

		// 4) [문자열 길이]를 써넣는다.
		*reinterpret_cast<COUNT_T*>(p) = bytes_copy;

		// 5) [원본_버퍼_포인터]와 [원본_버퍼_길이]를 옮긴다.
		data_  = p;
		size_ += added_length;
				
		// return) 
		return base_t(p, added_length);
	}
	template <class T, class TY, std::size_t N>
	constexpr base_t			_prepend_string_encoding(const TY(&_string)[N])
	{
		// 1) calculate string size ( string + NULL )
		constexpr auto bytes_copy = N * sizeof(T);
		constexpr auto added_length = sizeof(COUNT_T) + bytes_copy;

		// check) lower bound
		_CGD_BUFFER_BOUND_CHECK((data_ - added_length) >= get_lower_bound());

		// 2) [원본_버퍼_포인터]를 [문자열 길이] 만큼 뺀 [목표_포인터]를 구한다..
		auto p = data_ - bytes_copy;

		// 3) 문자열을 변환한다.
		if (bytes_copy != 0 && p != _string)
		{
			encode_string(reinterpret_cast<T*>(p), _string);
		}

		// 4) 이동
		p -= sizeof(COUNT_T);

		// 4) [문자열 길이]를 써넣는다.
		*reinterpret_cast<COUNT_T*>(p) = bytes_copy;

		// 5) [원본_버퍼_포인터]와 [원본_버퍼_길이]를 옮긴다.
		data_  = p;
		size_ += added_length;
				
		// return) 
		return base_t(p, added_length);
	}
	template <class T, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_prepend_string_format(const T* _format, TREST&&... _rest)
	{
	#ifdef _WIN32
		// check) _format이 nullptr이면 안된다.
		CGDK_ASSERT(_format!=nullptr, throw std::invalid_argument("_format is nullptr [0]"));

		// 1) store data_
		auto buf_old = data_;

		// declare)
		constexpr std::size_t TEMP_STRING_BUFFER_SIZE = 2048;

		// 2) get lower bound
		auto bound_lower = get_lower_bound();

		// 3) get max length (saturate max _XX_MAX_STRING_SIZE)
		std::size_t	max_length = _buffer_string_size_saturate((reinterpret_cast<const T*>(data_ - sizeof(COUNT_T)) - reinterpret_cast<const T*>(bound_lower)));

		// 4) [임시_버퍼]를 할당한다
		auto pbuf_temp = mem_alloc(max_length* sizeof(T));
		auto str_start = reinterpret_cast<T*>(pbuf_temp->data_);

		// 5) [원본_버퍼_시작_크기]에 [형식_문자열]를 써넣는다.
		auto length_string = _Xsprintf(str_start, max_length, _format, std::forward<TREST>(_rest)...) + 1;
		auto size_string = length_string*sizeof(T);

		// 6) [목표_포인터]를 구한다.
		auto p = data_-size_string;

		// check) lower bound
		_CGD_BUFFER_BOUND_CHECK((p - sizeof(COUNT_T)) >= get_lower_bound());
	
		// 7) [문자열]을 복사한다.
		memcpy(p, str_start, size_string);

		// 8) [목표_포인터]를 sizeof(COUNT_T)만큼을 뺀다.
		p -= sizeof(COUNT_T);

		// 9) [문자열_길이]를 써넣는다.
		*reinterpret_cast<COUNT_T*>(p)	 = static_cast<COUNT_T>(length_string);

		// 10)[원본_버퍼_포인터]를 업데이트한다.
		data_ = p;
		size_ += size_string + sizeof(COUNT_T);

		// return) 
		return base_t(data_, buf_old - data_);
	#else
		// Error) Template
		check(false);

		return base_t();
	#endif
	}
	template <class T>
	constexpr base_t			_prepend_text(std::string_view _string)
	{
		// check) _string이 nullptr이면 안된다.
		CGDK_ASSERT(_string!=nullptr, throw std::invalid_argument("_string is nullptr [0]"));

		// 1) [문자열_길이]를 구한다.
		auto length_string = std::char_traits<T>::length(_string);
		auto size_string = length_string * sizeof(T);

		// check) lower bound
		_CGD_BUFFER_BOUND_CHECK((data_ - size_string) >= get_lower_bound());

		// 2) [원본_버퍼_포인터]를 [문자열 길이] 만큼 뺀 [목표_포인터]를 구한다..
		T* p = data_-size_string;
	
		// 3) [원본_버퍼_포인터]에 문자열을 [문자열 길이] 만큼 복사한다.
		if(p!=_string)
		{
			memcpy(p, _string, size_string);
		}

		// 4) [원본_버퍼_포인터]와 [원본_버퍼_길이]를 옮긴다.
		data_ = p;
		size_ += size_string;

		// return) 
		return base_t(p, size_string);
	}
	template <class T, class TY>
	constexpr base_t			_prepend_text_encoding(std::basic_string_view<TY> _string)
	{
		// check) _string이 nullptr이면 안된다.
		CGDK_ASSERT(_string!=nullptr, throw std::invalid_argument("_string is nullptr [0]"));

		// 1) [문자열_길이]를 구한다.
		auto length_string = std::char_traits<T>::length(_string);
		auto size_string = length_string * sizeof(T);

		// check) lower bound
		_CGD_BUFFER_BOUND_CHECK((data_ - size_string) >= get_lower_bound());

		// 2) [원본_버퍼_포인터]를 [문자열 길이] 만큼 뺀 [목표_포인터]를 구한다..
		T* p = data_ - size_string;
	
		// 3) [원본_버퍼_포인터]에 문자열을 [문자열 길이] 만큼 복사한다.
		if(size_string != 0 && p != _string)
		{
			encode_string(reinterpret_cast<T*>(p), _string);
		}

		// 4) [원본_버퍼_포인터]와 [원본_버퍼_길이]를 옮긴다.
		data_ = p;
		size_ += size_string;

		// return) 
		return base_t(p, size_string);
	}
	template <class T, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_prepend_text_format(const T* _format, TREST&&... _rest)
	{
	#ifdef _WIN32
		// check) _string이 nullptr이면 안된다.
		CGDK_ASSERT(_format!=nullptr, throw std::invalid_argument("_format is nullptr! [0]"));

		// declare)
		constexpr std::size_t TEMP_STRING_BUFFER_SIZE = 2048;

		// 1) [임시_버퍼]를 할당한다
		auto pbuf_temp = mem_alloc(TEMP_STRING_BUFFER_SIZE);
		auto str_start = reinterpret_cast<T*>(pbuf_temp->data_);

		// 3) get lower bound
		auto bound_lower = get_lower_bound();

		// 4) get max length (saturate max _XX_MAX_STRING_SIZE)
		std::size_t	max_length = _buffer_string_size_saturate((reinterpret_cast<const T*>(data_ - sizeof(COUNT_T)) - reinterpret_cast<const T*>(bound_lower)));

		// 5) [원본_버퍼_시작_크기]에 [형식_문자열]를 써넣는다.
		auto length_string = _Xsprintf(str_start, max_length, std::forward<TREST>(_rest)...);
		auto size_string = length_string*sizeof(T);

		// 6) [원본_버퍼_포인터]를 [문자열 길이] 만큼 뺀 [목표_포인터]를 구한다..
		auto p = data_ - size_string;

		// check) lower bound
		_CGD_BUFFER_BOUND_CHECK(p >= get_lower_bound());

		// 7) [문자열]을 추가한다.
		memcpy(p, str_start, size_string);

		// 9) [원본_버퍼_포인터]와 [원본_버퍼_길이]를 옮긴다.
		data_ = p;
		size_ += size_string;

		// return) 
		return base_t(p, size_string);
	#else
		// Error) Template
		check(false);

		return	base_t();
	#endif
	}
	constexpr base_t			_prepend_bytes(std::size_t _size, const void* _buffer)
	{
		// check) _size가 0이하면 안된다.
		CGDK_ASSERT(_size>=0, throw std::invalid_argument("_size is invalid [0]"));

		// check) _buffer이 nullptr이면 안된다.
		CGDK_ASSERT(_buffer!=nullptr, throw std::invalid_argument("_buffer is nullptr [1]"));

		// check) lower bound
		_CGD_BUFFER_BOUND_CHECK((data_-_size) >= get_lower_bound());

		// 1) store data_
		auto buf_old = data_;

		// 2) [원본_버퍼_포인터]의 크기를 _size만큼 줄인다.
		auto p = data() -_size;
	
		// check) _size가 0이면 끝낸다.
		if(_size == 0) return base_t(p,0);

		// 3) [원본_버퍼_포인터]에 [데이터]를 [데이터_크기]만큼 복사한다.
		if(_buffer != nullptr && _buffer!=p)
		{
			memcpy(p, _buffer, _size);
		}

		// 4) [원본_버퍼_포인터]를 갱신한다.
		data_  = p;

		// 5) [원본_버퍼_길이]를 _size만큼 늘린다. 
		size_ += _size;

		// return) 
		return base_t(data_, buf_old - data_);
	}
	constexpr basic_buffer		_prepend_buffer(const base_t& _buffer)
	{
		// check) _buffer.size_가 0이하면 안된다.
		CGDK_ASSERT(_buffer.size() >= 0, throw std::invalid_argument("_buffer.size_ is invalid [0]"));

		// 1) store data_
		auto buf_old = data_;

		// 1) [데이터_갯수]만큼 데이터들을 통채로 복사한다.
		_prepend_bytes(_buffer.size(), _buffer.data());

		// 2) [데이터_갯수]를 써넣는다.
		_prepend_general<COUNT_T>(static_cast<COUNT_T>(_buffer.size()));

		// return) 
		return	basic_buffer{ base_t{ data_, static_cast<size_type>(buf_old - data_) } };
	}
	template <class T>
	constexpr base_t			_prepend_initializer_list(const std::initializer_list<T>& _list)
	{
		auto p = data_;

		for(auto& iter:_list)
		{
			_prepend<T>(iter);
		}
		
		return base_t(p, data_ - p);
	}
	template <class T>
	constexpr base_t			_prepend_tuple(const std::tuple<T>& _data)
	{
		auto p = data_;
		_prepend<T>(std::get<0>(_data)); 
		return base_t(data_, p - data_);
	}
	template <class TFIRST, class TSECOND, class... TREST>
	constexpr base_t			_prepend_tuple(const std::tuple<TFIRST, TSECOND, TREST...>& _tupple)
	{
		auto p = data_;
		_prepend_tuple((const std::tuple<TSECOND, TREST...>&)_tupple); _prepend<TFIRST>(std::get<0>(_tupple));
		return base_t(data_, p - data_);
	}
	template <class T>
	constexpr void				_prepend_multi_(T&& _data)
	{
		_prepend<T>(std::forward<T>(_data));
	}
	template <class TFIRST, class TSECOND, class... TREST>
	constexpr void				_prepend_multi_(TFIRST&& _first, TSECOND&& _second, TREST&&... _rest)
	{
		_prepend_multi_<TSECOND, TREST...>(std::forward<TSECOND>(_second), std::forward<TREST>(_rest)...);
		_prepend<TFIRST>(std::forward<TFIRST>(_first));
	}
	template<class... T>
	constexpr base_t			_prepend_multi(T&&... _multi)
	{ 
		auto p = data_;
		_prepend_multi_<T...>(std::forward<T>(_multi)...);
		return base_t(data_, p - data_);
	}
	template <class T>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_prepend_string(std::basic_string_view<T> _string)
	{
		// 1) ...
		auto buf_old = data_;

		// 2) String길이를 구한다.(NULL길이까지 포함하므로 +1한다.)
		auto length_string = _string.length();
		auto size_string = length_string * sizeof(T);

		// check) lower bound
		_CGD_BUFFER_BOUND_CHECK((data_ - size_string - sizeof(COUNT_T)) >= get_lower_bound());

		// 3) NULL으르 제일 먼저 넣는다.
		_prepend_general<T>(0);

		// 4) [문자열]을 [문자열_길이]만큼 복사한다.
		_prepend_bytes(size_string, _string.data());

		// 5) [문자열_길이]를 써넣는다.
		_prepend_general<COUNT_T>(static_cast<COUNT_T>(length_string + 1));

		// return) String의 제일 처음 위치를 리턴함!!
		return	base_t(data_, buf_old - data_);
	}
	template <class T, class TY>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_prepend_string_encoding(std::basic_string_view<TY> _string)
	{
		// check) _string이 nullptr이면 안된다.
		CGDK_ASSERT(_string != nullptr, throw std::invalid_argument("_string is nullptr [0]"));

		// 1) [문자열_길이]를 구한다.
		const auto length_string = _string.length();
		const auto size_string = (length_string + 1) * sizeof(T);

		// check) lower bound
		_CGD_BUFFER_BOUND_CHECK((data_ - size_string - sizeof(COUNT_T)) >= get_lower_bound());

		// 2) ...
		T* p = reinterpret_cast<T*>(data_ - sizeof(T));

		// 3) append NULL
		*reinterpret_cast<COUNT_T*>(p) = 0;

		// 4) [원본_버퍼_포인터]를 [문자열 길이] 만큼 뺀 [목표_포인터]를 구한다..
		p -= length_string;

		// 5) [원본_버퍼_포인터]에 문자열을 [문자열 길이] 만큼 복사한다.
		if (length_string != 0 && p != _string.data())
		{
			encode_string(p, _string);
		}

		// 6) [문자열 길이]를 써넣는다.
		*reinterpret_cast<COUNT_T*>(reinterpret_cast<char*>(p) - sizeof(COUNT_T)) = length_string + 1;

		// 4) [원본_버퍼_포인터]와 [원본_버퍼_길이]를 옮긴다.
		data_ = p;
		size_ += size_string + sizeof(COUNT_T);

		// return) String의 제일 처음 위치를 리턴함!!
		return	base_t(data_, size_string + sizeof(COUNT_T));
	}
	template <class T>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_prepend_text(std::basic_string_view<T> _string)
	{
		// check) _string이 nullptr이면 안된다.
		CGDK_ASSERT(_string != nullptr, throw std::invalid_argument("_string is nullptr [0]"));

		// 1) [문자열_길이]를 구한다.
		auto length_string = std::char_traits<T>::length(_string);
		auto size_string = length_string * sizeof(T);

		// check) lower bound
		_CGD_BUFFER_BOUND_CHECK((data_ - size_string) >= get_lower_bound());

		// 2) [원본_버퍼_포인터]를 [문자열 길이] 만큼 뺀 [목표_포인터]를 구한다..
		auto p = data() - size_string;

		// 3) [원본_버퍼_포인터]에 문자열을 [문자열 길이] 만큼 복사한다.
		if (p != _string)
		{
			memcpy(p, _string, size_string);
		}

		// 4) [원본_버퍼_포인터]와 [원본_버퍼_길이]를 옮긴다.
		data_ = p;
		size_ += size_string;

		// return) 
		return	base_t(p, p - data_);
	}
	template <class T, class TY>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_prepend_text_encoding(std::basic_string_view<TY> _string)
	{
		// check) _string이 nullptr이면 안된다.
		CGDK_ASSERT(_string != nullptr, throw std::invalid_argument("_string is nullptr [0]"));

		// 1) [문자열_길이]를 구한다.
		auto length_string = std::char_traits<T>::length(_string);
		auto size_string = length_string * sizeof(T);

		// check) lower bound
		_CGD_BUFFER_BOUND_CHECK((data_ - size_string) >= get_lower_bound());

		// 2) [원본_버퍼_포인터]를 [문자열 길이] 만큼 뺀 [목표_포인터]를 구한다..
		auto p = data() - size_string;

		// 3) [원본_버퍼_포인터]에 문자열을 [문자열 길이] 만큼 복사한다.
		if (size_string != 0 && p != _string)
		{
			encode_string(reinterpret_cast<T*>(p), _string);
		}

		// 4) [원본_버퍼_포인터]와 [원본_버퍼_길이]를 옮긴다.
		data_ = p;
		size_ += size_string;

		// return) 
		return	base_t(p, p - data_);
	}
	template <class T>
	constexpr std::enable_if_t<is_memcopy_able<typename T::value_type>::value, base_t>
								_prepend_container_array(const T& _container)
	{
		// 1) store data_
		auto p = data();

		// 2) [데이터]들을 바로 memcopy한다.
		_prepend_bytes(_container.size() * sizeof(T::value_type), &_container.front());

		// 3) [데이터_갯수]를 써넣는다.
		_prepend_general<COUNT_T>(static_cast<COUNT_T>(_container.size()));

		// return) 
		return	base_t(data_, p-data_);
	}
	template <class T>
	constexpr std::enable_if_t<!is_memcopy_able<typename T::value_type>::value, base_t>
								_prepend_container_array(const T& _container)
	{
		// 1) store data_
		auto p = data();

		// 2) Begin/End Iterator
		auto iter = _container.rbegin();
		auto iter_end = _container.rend();

		// 3) [데이터]들을 써넣는다.
		for (; iter != iter_end; ++iter)
		{
			_prepend<typename T::value_type>(*iter);
		}

		// 4) [데이터_갯수]를 써넣는다.
		_prepend_general<COUNT_T>(static_cast<COUNT_T>(_container.size()));

		// return) 
		return	base_t(data_, p - data_);
	}
	template<class RI>
	constexpr std::enable_if_t<is_iterator<RI>::value, base_t>
								_prepend_iterator(RI _reverse_first, RI _reverse_last)
	{
		// 1) store data_
		auto p = data();

		// declare)
		COUNT_T iter_count = 0;

		// 1) 데이터들을 저장한다.
		for (; _reverse_first != _reverse_last; ++_reverse_first, ++iter_count)
		{
			_prepend<typename RI::value_type>(*_reverse_first);
		}

		// 2) 크기를 써넣을 위치 예약
		_prepend_general<COUNT_T>(iter_count);

		// return) 
		return base_t(data_, p - data_);
	}
	// append) 
	template <class T>
	constexpr appd_tr<T>		_append(const T& _data)
	{
		return APPD_t<basic_buffer,T>::_do(*this, _data);
	}
	template <class T>
	constexpr appd_tr<T>		_append(const object_ptr<std::remove_const_t<T>>& _data)
	{
		return APPD_t<basic_buffer,T>::_do(*this, *_data);
	}
	template <class T>
	constexpr base_t			_append_initializer_list(const std::initializer_list<T>& _list)
	{
		auto p = data_;
		for(auto& iter:_list)
		{
			_append<T>(iter);
		}
		return base_t(p, data_ - p);
	}
	constexpr base_t			_append_skip(std::size_t _length)
			{
				// check) _length가 0이하면 안된다.
				CGDK_ASSERT(_length >= 0, throw std::invalid_argument("_length is invalid [0]"));

				// 1) [원본_버퍼_끝_포인터]를 구한다.
				auto p = data() + size_;

				// check) lower bound
				_CGD_BUFFER_BOUND_CHECK((p + _length) <= get_upper_bound());

				// 2) [원본_버퍼_길이]를 _length만큼 늘린다. 
				size_ += _length;

				// return) [원본_버퍼_끝_포인터]값을 리턴한다.
				return base_t(p, _length);
			}
	template <class T>
	constexpr T&				_append_emplace()
	{
		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((data_ + size_ + sizeof(T)) <= get_upper_bound());

		// 1) [원본_버퍼_끝_포인터]를 구한다.
		auto p = reinterpret_cast<T*>(data() + size_);

		// 2) [원본_버퍼_끝_포인터]에 생성자를 호출한다.
	#pragma push_macro("new")
	#undef new
		new (p) T;
	#pragma pop_macro("new")

		// 3) [원본_버퍼_길이]를 _length만큼 늘린다. 
		size_ += sizeof(T);

		// return) [원본_버퍼_끝_포인터]값을 리턴한다.
		return *p;
	}
	template <class T>
	constexpr T&				_append_general(const T& _data)
	{
		// check) Pointer형이면 안됀다.
		static_assert(!std::is_pointer<T>::value, "Must not be Pointer Type!!");

		// 1) [원본_버퍼_끝_포인터]를 구한다.
		auto p = reinterpret_cast<T*>(data() + size_);
	
		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((p + 1) <= get_upper_bound());

		// 2) [원본_버퍼_끝_포인터]에 값(_data)를 써넣는다.
		*p = _data;
	
		// 3) [원본_버퍼_길이]를 sizeof(T)만큼 늘린다. 
		size_ += sizeof(T);
	
		// return) [원본_버퍼_끝_값]을 리턴한다.
		return *p;
	}
	template <class T> 
	constexpr std::enable_if_t<!is_memcopy_able<T>::value, base_t>
								_append_array(const T* _data, std::size_t _count)
	{
		// check) _data이 nullptr이면 안된다.
		CGDK_ASSERT(_data != nullptr && _count != 0, throw std::invalid_argument("_data is nullptr or _count is 0 [1]"));

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((data_ + size_ + sizeof(T) * _count) <= get_upper_bound());

		// 1) [원본_버퍼_포인터]를 저장해 놓는다.
		auto len_old = size_;

		// 2) [데이터_갯수]를 써넣는다.
		_append_general<COUNT_T>(static_cast<COUNT_T>(_count));

		// 3) [데이터]들을 추가한다.
		for(std::size_t i=0;i<_count;++i)
		{
			_append<T>(_data[i]);
		}

		// return)
		return base_t(data_ + len_old, size_ - len_old);
	}
	template <class T>
	constexpr std::enable_if_t<is_memcopy_able<T>::value, base_t>
								_append_array(const T* _data, std::size_t _count)
	{
		// check) _data이 nullptr이면 안된다.
		CGDK_ASSERT(_data != nullptr && _count != 0, throw std::invalid_argument("_data is nullptr or _count is 0 [1]"));

		// 1) [데이터_갯수]를 써넣는다.
		_append_general<COUNT_T>(static_cast<COUNT_T>(_count));

		// 2) [데이터]들을 통채로 복사한다.
		return _append_bytes(_count * sizeof(T), _data);
	}
	template <class T, std::size_t N>
	constexpr std::enable_if_t<!is_memcopy_able<T>::value, base_t>
								_append_array(const T* _data)
	{
		return _append_array<T>(_data, N);
	}
	template <class T, std::size_t N>
	constexpr std::enable_if_t<is_memcopy_able<T>::value, base_t>
								_append_array(const T* _data)
	{
		// check)
		static_assert(N != 0, "error! array size is 0");

		// 1) [데이터]들을 통채로 복사한다.
		return _append_bytes(N * sizeof(T), _data);
	}
	template <class T>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_append_string(std::basic_string_view<T> _string)
	{
		// 1) [목표_포인터]를 구한다.
		auto buf_dest = data_ + size_;

		// 2) get values
		auto length_string = _string.size();	// strnlen(, _buffer_size)
		auto bytes_copy = length_string * sizeof(T);
		auto added_length = sizeof(COUNT_T) + sizeof(T) + bytes_copy;

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((buf_dest + added_length) <= get_upper_bound());

		// declare) 
		auto buf_now = buf_dest;

		// 3) [문자열_길이]를 저장한다. (NULL포함)
		*reinterpret_cast<COUNT_T*>(buf_now) = static_cast<COUNT_T>(length_string + 1);
		buf_now += sizeof(COUNT_T);

		// 4) copy string
		if (bytes_copy != 0)
		{
			memcpy(buf_now, _string.data(), bytes_copy);
			buf_now += bytes_copy;
		}

		// 5) NULL을 추가한다.
		*reinterpret_cast<T*>(buf_now) = 0;

		// 6) [버퍼_길이]를 업데이트한다.
		size_ += added_length;

		// return)
		return	base_t(buf_dest, added_length);
	}
	template <class T, class TY>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_append_string_encoding(std::basic_string_view<TY> _string)
	{
		// 1) [목표_포인터]를 구한다.
		auto buf_dest = data_ + size_;

		// 2) get values
		auto length_string = _string.size();	// strnlen(, _buffer_size)
		auto bytes_copy = length_string * sizeof(T);
		auto added_length = sizeof(COUNT_T) + sizeof(T) + bytes_copy;

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((buf_dest + added_length) <= get_upper_bound());

		// declare) 
		auto buf_now = buf_dest;

		// 3) [문자열_길이]를 저장한다. (NULL포함)
		*reinterpret_cast<COUNT_T*>(buf_now) = static_cast<COUNT_T>(length_string + 1);
		buf_now += sizeof(COUNT_T);

		// 4) copy string
		if (bytes_copy != 0)
		{
			encode_string(reinterpret_cast<T*>(buf_now), _string);
			buf_now += bytes_copy;
		}

		// 5) NULL을 추가한다.
		*reinterpret_cast<T*>(buf_now) = 0;

		// 6) [버퍼_길이]를 업데이트한다.
		size_ += added_length;

		// return)
		return	base_t(buf_dest, added_length);
	}
	template <class T, std::size_t N>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_append_string(const T(&_string)[N])
	{
		// declare)
		constexpr auto bytes_copy = N * sizeof(T);
		constexpr auto added_length = sizeof(COUNT_T) + bytes_copy;

		// 1) 문자열을 복사할 위치를 구한다.
		auto buf_dest = data_ + size_;

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((buf_dest + added_length) <= get_upper_bound());

		// declare) 
		auto buf_now = buf_dest;

		// 3) [문자열_길이]를 저장한다. (NULL포함)
		*reinterpret_cast<COUNT_T*>(buf_now) = static_cast<COUNT_T>(N);
		buf_now += sizeof(COUNT_T);

		// 4) [문자열]을 [원본_버퍼_포인터] 위치에 복사한다.
		if (bytes_copy != 0)
		{
			memcpy(buf_now, _string, bytes_copy);
			buf_now += bytes_copy;
		}

		// 5) NULL을 추가한다.
		*reinterpret_cast<T*>(buf_now) = 0;

		// 6) [버퍼_길이]를 업데이트한다.
		size_ += added_length;

		// return) 리턴
		return	base_t(buf_dest, added_length);
	}
	template <class T, std::size_t N, class TY>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_append_string_encoding(const TY(&_string)[N])
	{
		// declare)
		constexpr auto bytes_copy = N * sizeof(T);
		constexpr auto added_length = sizeof(COUNT_T) + bytes_copy;

		// 1) 문자열을 복사할 위치를 구한다.
		auto buf_dest = data_ + size_;

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((buf_dest + added_length) <= get_upper_bound());

		// declare) 
		auto buf_now = buf_dest;

		// 3) [문자열_길이]를 저장한다. (NULL포함)
		*reinterpret_cast<COUNT_T*>(buf_now) = static_cast<COUNT_T>(N);
		buf_now += sizeof(COUNT_T);

		// 4) [문자열]을 [원본_버퍼_포인터] 위치에 복사한다.
		if (bytes_copy != 0)
		{
			encode_string(reinterpret_cast<T*>(buf_now), _string);
			buf_now += bytes_copy;
		}

		// 5) NULL을 추가한다.
		*reinterpret_cast<T*>(buf_now) = 0;

		// 6) [버퍼_길이]를 업데이트한다.
		size_ += added_length;

		// return) 리턴
		return	base_t(buf_dest, added_length);
	}
	template <class T, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_append_string_format(std::basic_string_view<T> _format, TREST&&... _rest)
	{
		// 1) [문자열]을 복사할 위치를 구한다.
		auto buf_dest = data() + size_;
		std::size_t	length_string = 1;

		// declare)
		auto bound_upper = get_upper_bound();

		// 2) get max length (saturate max _XX_MAX_STRING_SIZE)
		std::size_t	max_length = _buffer_string_size_saturate((reinterpret_cast<const T*>(bound_upper) - reinterpret_cast<const T*>(buf_dest + sizeof(COUNT_T))));

		// 3) [문자열]을 복사한다.
		if (_format.empty() == false)
		{
			length_string = _Xsprintf(reinterpret_cast<T*>(buf_dest + sizeof(COUNT_T)), max_length, _format.data(), std::forward<TREST>(_rest)...) + 1;
		}

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((buf_dest + sizeof(COUNT_T) + length_string * sizeof(T)) <= bound_upper);

		// 4) [문자열_길이]를 저장한다.
		*reinterpret_cast<COUNT_T*>(data() + size_) = static_cast<COUNT_T>(length_string);

		// 5) get added length
		auto added_length = length_string * sizeof(T) + sizeof(COUNT_T);

		// 6) [버퍼]를 업데이트한다.
		size_ += added_length;

		// return)
		return	base_t(buf_dest, added_length);
	}
	template <class T, std::size_t N, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_append_string_format(const T(&_format)[N], TREST&&... _rest)
	{
		// 1) [문자열]을 복사할 위치를 구한다.
		auto buf_dest = data() + size_;
		std::size_t	length_string = 1;

		// declare) 
		auto bound_upper = get_upper_bound();

		// 2) get max length
		std::size_t	max_length = _buffer_string_size_saturate((reinterpret_cast<const T*>(bound_upper) - reinterpret_cast<const T*>(buf_dest + sizeof(COUNT_T))));

		// 3) [문자열]을 복사한다.
		length_string = _Xsprintf(reinterpret_cast<T*>(buf_dest + sizeof(COUNT_T)), max_length, _format, std::forward<TREST>(_rest)...) + 1;

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((buf_dest + sizeof(COUNT_T) + length_string * sizeof(T)) <= bound_upper);

		// 4) [문자열_길이]를 저장한다.
		*reinterpret_cast<COUNT_T*>(buf_dest) = static_cast<COUNT_T>(length_string);

		// 5) get added length
		auto added_length = length_string * sizeof(T) + sizeof(COUNT_T);

		// 6) [버퍼]를 업데이트한다.
		size_	+= added_length;

		// return)
		return	base_t(buf_dest, added_length);
	}
	template <class T>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_append_text(std::basic_string_view<T> _string)
	{
		// 1) [문자열]의 길이를 구한다.
		auto size_string = _string.size() * sizeof(T);

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((data_ + size_ + size_string) <= get_upper_bound());

		// 2) 
		auto buf_pos = data() + size_;

		// 2) [문자열]을 복사한다.
		memcpy(buf_pos, _string.data(), size_string);

		// 3) [문자열_길이]만큼 더한다.
		size_ += size_string;

		// return) 
		return	base_t(buf_pos, size_string);
	}
	template <class T, class TY>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_append_text_encoding(std::basic_string_view<TY> _string)
	{
		// 1) [문자열]의 길이를 구한다.
		auto size_string = _string.size() * sizeof(T);

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((data_ + size_ + size_string) <= get_upper_bound());

		// 2) 
		auto buf_pos = data() + size_;

		// 3) [문자열]을 복사한다
		if (size_string != 0)
		{
			encode_string(reinterpret_cast<T*>(buf_pos), _string);
		}

		// 4) [문자열_길이]만큼 더한다.
		size_ += size_string;

		// return) 
		return	base_t(buf_pos, size_string);
	}

	template <class T, std::size_t N>
	constexpr base_t			_append_text(const T(&_text)[N])
			{
				// declare) 
				constexpr auto length_string = N-1;
				constexpr auto size_string = length_string*sizeof(T);

				// check) upper bound
				_CGD_BUFFER_BOUND_CHECK((data_ + size_ + size_string) <= get_upper_bound());

				// declare)
				auto buf_dest = data() + size_;

				// 1) [문자열]을 복사하고 [문자열_길이]만큼 더한다.
				memcpy(buf_dest, _text, size_string);

				// 2) [문자열_길이]만큼 더한다.
				size_ += size_string;

				// return) 
				return	base_t(buf_dest, size_string);
			}
	template <class T, std::size_t N, class TY>
	constexpr base_t			_append_text_encoding(const TY(&_text)[N])
	{
		// declare) 
		constexpr auto length_string = N-1;
		constexpr auto size_string = length_string*sizeof(T);

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((data_ + size_ + size_string) <= get_upper_bound());

		// declare)
		auto buf_dest = data() + size_;

		// 1) [문자열]을 복사하고 [문자열_길이]만큼 더한다.
		if (size_string != 0)
		{
			encode_string(reinterpret_cast<T*>(buf_dest), _text);
		}

		// 2) [문자열_길이]만큼 더한다.
		size_ += size_string;

		// return) 
		return	base_t(buf_dest, size_string);
	}
	template <class T, class... TREST>
	constexpr std::enable_if_t<is_string_type<T>::value, base_t>
								_append_text_format(const T* _format, TREST&&... _rest)
	{
		// 1) [문자열]을 복사할 위치를 저장한다.
		auto buf_dest = data() + size_;

		// declare) 
		auto bound_upper = get_upper_bound();

		// 2) get max length (saturate max _XX_MAX_STRING_SIZE)
		std::size_t	max_length = _buffer_string_size_saturate((reinterpret_cast<const T*>(bound_upper) - reinterpret_cast<const T*>(buf_dest + sizeof(COUNT_T))));

	#if defined(_MSC_VER)
		#pragma warning(disable:4996)
	#endif
		// 2) [문자열]을 복사한다.
		auto length_string = _Xsprintf(reinterpret_cast<T*>(buf_dest), max_length, _format, std::forward<TREST>(_rest)...);

	#if defined(_MSC_VER)
		#pragma warning(default:4996)
	#endif
		auto size_string = length_string * sizeof(T);

		// 3) [버퍼]를 업데이트한다.
		size_ += size_string;

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((data_ + size_) <= bound_upper);

		// return) 리턴
		return	base_t(buf_dest, size_string);
	}
	constexpr base_t			_append_bytes(std::size_t _size, const void* _buffer)
	{
		// check) _size가 0이하면 안된다.
		CGDK_ASSERT(_size >= 0, throw std::invalid_argument("_size is invalid [0]"));

		// check) _buffer이 nullptr이면 안된다.
		CGDK_ASSERT(_buffer != nullptr || (_buffer == nullptr && _size == 0), throw std::invalid_argument("_buffer is nullptr [1]"));

		// check) upper bound 
		_CGD_BUFFER_BOUND_CHECK((data_ + size_ + _size) <= get_upper_bound());

		// 1) [원본_버퍼_끝_포인터]를 얻는다.
		auto buf_dest = data() + size_;
	
		// check) _size가 0이면 끝낸다.
		if(_size==0) return base_t(buf_dest, 0);
	
		// 2) [원본_버퍼_끝_포인터]에 [데이터]를 [데이터_크기]만큼 복사한다.
		if(_buffer!=nullptr && _buffer != buf_dest)
		{
			memcpy(buf_dest, _buffer, _size);
		}
	
		// 3) [원본_버퍼_길이]에 [데이터_길이]만큼 더한다.
		size_ += _size;
	
		// return) 
		return base_t(buf_dest, _size);
	}
	constexpr basic_buffer		_append_buffer(const base_t& _buffer)
	{
		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((data_ + size_ + sizeof(base_t::size_type) + _buffer.size()) <= get_upper_bound());

		// check) _count가 0이하면 안된다.
		CGDK_ASSERT(_buffer.size() >= 0, throw std::invalid_argument("_buffer.size_ is invalid [0]"));

		// check) _data이 nullptr이면 안된다.
		CGDK_ASSERT(_buffer.data() != nullptr || _buffer.size() == 0, throw std::invalid_argument("_data is nullptr [1]"));

		// 1) [원본_버퍼_포인터]를 저장해 놓는다.
		auto len_old = size_;

		// 2) 데이터를 써넣는다.
		if(_buffer.data() != nullptr && _buffer.size() != 0)
		{
			// - [데이터_갯수]를 써넣는다.
			_append_general<base_t::size_type>(_buffer.size());

			// - [데이터]들을 통채로 복사한다.
			_append_bytes(_buffer.size(), _buffer.data());
		}
		else
		{
			// 2) [데이터_갯수]를 0으로 써넣는다.
			_append_general<base_t::size_type>(0);
		}

		// return) 
		return	basic_buffer{ base_t{data_ + len_old, size_ - len_old} };
	}
	template<class T>
	constexpr base_t			_append_tuple(const std::tuple<T>& _tupple)
	{
		auto len_old = size_;
		_append<std::decay_t<T>>(std::get<0>(_tupple));
		return base_t(data_ + len_old, size_ - len_old);
	}
	template<class TFIRST, class TSECOND, class... TREST>
	constexpr base_t			_append_tuple(const std::tuple<TFIRST, TSECOND, TREST...>& _tupple)
	{ 
		auto len_old = size_;
		_append<std::decay_t<TFIRST>>(std::get<0>(_tupple));
		_append_tuple<TSECOND, TREST...>((const std::tuple<TSECOND, TREST...>&)_tupple);
		return base_t(data_ + len_old, size_ - len_old);
	}
	template<class T>
	constexpr void				_append_multi_(T&& _tupple)
	{
		_append<std::decay_t<T>>(std::forward<T>(_tupple));
	}
	template<class TFIRST, class TSECOND, class... TREST>
	constexpr void				_append_multi_(TFIRST&& _first, TSECOND&& _second, TREST&&... _rest)
	{ 
		_append<std::decay_t<TFIRST>>(_first);
		_append_multi_<TSECOND, TREST...>(std::forward<TSECOND>(_second), std::forward<TREST>(_rest)...);
	}
	template<class... T>
	constexpr base_t			_append_multi(T&&... _multi)
	{ 
		auto len_old = size_;
		_append_multi_<T...>(std::forward<T>(_multi)...);
		return base_t(data_ + len_old, size_ - len_old);
	}
	template <class T>
	constexpr std::basic_string_view<T> _append_text(uint64_t _value)
	{
		// declare) 
		T temp_buf[32];
		auto pos_now = temp_buf;

		// 1) ...
		if (_value != 0)
		{
			while (_value)
			{
				*(pos_now++) = _value % 10 + '0';
				_value /= 10;
			}
		}
		else
		{
			*(pos_now++) = '0';
		}

		// 2) copy
		auto buf_dest = data() + size_;

		// declare)
		std::basic_string_view<T> result(buf_dest, pos_now - temp_buf);

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((buf_dest + result.size()) <= get_upper_bound());

		// 3) reverse copy
		for (--pos_now; pos_now >= temp_buf; ++buf_dest, --pos_now)
		{
			*buf_dest = *pos_now;
		}

		// 4) set size_
		size_ += result.size() * sizeof(T);

		// return) 
		return	result;
	}
	template <class T>
	constexpr std::basic_string_view<T> _append_text(int64_t _value)
	{
		// declare) 
		T temp_buf[32];
		auto pos_now = temp_buf;

		// 1-1) positive value
		if (_value > 0)
		{
			do
			{
				*(pos_now++) = _value % 10 + '0';
				_value /= 10;
			} while (_value);
		}
		// 1-2) zeroS
		else if (_value == 0)
		{
			*(pos_now++) = '0';
		}
		// 1-3) negative value
		else
		{
			// - get 2's completment
			_value = ~_value + 1;

			// - convert to string
			do
			{
				*(pos_now++) = _value % 10 + '0';
				_value /= 10;
			} while (_value);

			// - append '-'
			*(pos_now++) = '-';
		}

		// 2) copy
		auto buf_dest = data() + size_;

		// declare)
		std::basic_string_view<T> result(buf_dest, pos_now - temp_buf);

		// check) upper bound
		_CGD_BUFFER_BOUND_CHECK((buf_dest + result.size()) <= get_upper_bound());

		// 3) reverse copy
		for (--pos_now; pos_now >= temp_buf; ++buf_dest, --pos_now)
		{
			*buf_dest = *pos_now;
		}

		// 4) set size_
		size_ += result.size() * sizeof(T);

		// return) 
		return	result;
	}

	template<class T>
	constexpr base_t			_append_container_associative(const T& _container)
	{
		// !주의) value-key 순으로 append해야 한다.

		// 1) store data_
		auto l = size_;

		// 2) [데이터_갯수]를 써넣는다.
		_append_general<COUNT_T>(static_cast<COUNT_T>(_container.size()));

		// 3) [데이터]들을 써넣는다.
		for (auto iter = _container.begin(); iter != _container.end(); ++iter)
		{
			// - Value를 써넣는다.
			_append<typename T::mapped_type>(iter->second);
			// _ key를 써넣는다.
			_append<typename T::key_type>(iter->first);
		}

		// return) 
		return base_t(data_ + l, size_ - l);
	}
	template<class T>
	constexpr base_t			_prepend_container_associative(const T& _container)
	{
		// 주의) extract시 value->key순으로 extract되므로 prepaend시에는 key->value순으로 prepend해줘야 한다.

		// declare) 
		auto p = data_;

		// 1) Begin/End Iterator
		auto iter = _container.begin();
		auto iterEnd = _container.end();

		// 2) [데이터]들을 써넣는다.
		for(; iter!=iterEnd; ++iter)
		{
			// _ key를 써넣는다.
			_prepend<typename T::key_type>(iter->first);
			// - Value를 써넣는다.
			_prepend<typename T::mapped_type>(iter->second);
		}

		// 3) [데이터_갯수]를 써넣는다.
		_prepend_general<COUNT_T>(static_cast<COUNT_T>(_container.size()));

		// return) 
		return base_t(data_, p - data_);
	}

	template<class I>
	constexpr std::enable_if_t<is_iterator<I>::value, base_t>
								_append_iterator(I _first, I _last)
	{
		// 1) store data_
		auto l = size_;

		// 2) [데이터_갯수]를 써넣는다.
		auto buf_count = _append_skip(sizeof(COUNT_T));

		// declare)
		COUNT_T iter_count = 0;

		// 3) [데이터]들을 써넣는다.
		for (; _first != _last; ++_first, ++iter_count)
		{
			_append<typename I::value_type>(*_first);
		}

		// 4) 예약해 놓은 위치에 갯수를 써넣는다.
		buf_count.front<COUNT_T>() = iter_count;

		// return) 
		return base_t(data_ + l, size_ - l);
	}
	template <class T>
	constexpr std::enable_if_t<!is_memcopy_able<typename T::value_type>::value, base_t>
								_append_container_array(const T& _container)
	{
		// 1) [원본_버퍼_길이]을 얻어놓는다.
		auto l = size_;

		// 2) [데이터_갯수]를 써넣는다.
		_append_general<COUNT_T>(static_cast<COUNT_T>(_container.size()));

		// 3) [데이터]들을 써넣는다.
		auto iter = _container.begin();
		auto iterEnd = _container.end();

		for (; iter != iterEnd; ++iter)
		{
			_append<typename T::value_type>(*iter);
		}

		// return)
		return base_t(data_ + l, size_ - l);
	}
	template <class T>
	constexpr std::enable_if_t<is_memcopy_able<typename T::value_type>::value, base_t>
								_append_container_array(const T& _container)
	{
		// 1) [원본_버퍼_길이]을 얻어놓는다.
		auto l = size_;

		// declare)
		auto count = _container.size();

		// 1) [데이터_갯수]를 써넣는다.
		_append_general<COUNT_T>(static_cast<COUNT_T>(count));

		// 3) [데이터]들을 써넣는다.
		if (count != 0)
		{
			_append_bytes(count * sizeof(typename T::value_type), &_container.front());
		}

		// return)
		return base_t(data_ + l, size_ - l);
	}
	template <class T>
	constexpr base_t			_append_container_own_ptr_list(const T& _container)
	{
		// 1) [원본_버퍼_길이]을 얻어놓는다.
		auto l = size_;

		// 2) [데이터_갯수]를 써넣는다.
		_append_general<COUNT_T>(static_cast<COUNT_T>(_container.size()));

		// 3) [데이터]들을 써넣는다.
		auto iter = _container.begin();
		auto iterEnd = _container.end();

		for (; iter != iterEnd; ++iter)
		{
			if(iter->empty()==false)
			{
				_append<typename T::value_type>(*iter);
			}
			else
			{
				_append<COUNT_T>(COUNT_T(0)-1);
			}
		}

		// return)
		return base_t(data_ + l, size_ - l);
	}
	template <class T>
	constexpr base_t			_append_container_object_ptr_list(const T& _container)
	{
		// 1) [원본_버퍼_길이]을 얻어놓는다.
		auto l = size_;

		// 2) [데이터_갯수]를 써넣는다.
		_append_general<COUNT_T>(static_cast<COUNT_T>(_container.size()));

		// 3) [데이터]들을 써넣는다.
		auto iter = _container.begin();
		auto iterEnd = _container.end();

		for (; iter != iterEnd; ++iter)
		{
			if(iter->empty()==false)
			{
				_append<typename T::value_type>(*iter);
			}
			else
			{
				_append<COUNT_T>(COUNT_T(0)-1);
			}
		}

		// return)
		return base_t(data_ + l, size_ - l);
	}
	template <class T>
	constexpr base_t			_append_container_list(const T& _container)
	{
		// 1) [원본_버퍼_길이]을 얻어놓는다.
		auto l = size_;

		// 2) [데이터_갯수]를 써넣는다.
		_append_general<COUNT_T>(static_cast<COUNT_T>(_container.size()));

		// 3) [데이터]들을 써넣는다.
		auto iter = _container.begin();
		auto iterEnd = _container.end();

		for(; iter!=iterEnd; ++iter)
		{
			_append<typename T::value_type>(*iter);
		}

		// return)
		return base_t(data_ + l, size_ - l);
	}
	template <class T>
	constexpr base_t			_prepend_container_list(const T& _container)
	{
		// declare) 
		auto p = data_;

		// 1) Begin/End Iterator
		auto iter = _container.rbegin();
		auto iter_end = _container.rend();

		// 2) [데이터]들을 써넣는다.
		for(; iter != iter_end; ++iter)
		{
			_prepend<typename T::value_type>(*iter);
		}

		// 3) [데이터_갯수]를 써넣는다.
		_prepend_general<COUNT_T>(static_cast<COUNT_T>(_container.size()));

		// return) 
		return	base_t(data_, p - data_);
	}

public:
	// 10) begin/end)
	constexpr basic_buffer		_begin(int _offset) const noexcept { return basic_buffer(base_t::_begin(_offset), bound);}
	constexpr basic_buffer		_end(int _offset) const noexcept { return basic_buffer(base_t::_end(_offset), bound);}

	// 11) append/extarct
	constexpr basic_buffer		_extract_buffer();

	constexpr bool				_is_buffer_overflow() const
								{
									// check) 
									RETURN_IF(get_upper_bound() == nullptr, false);

									// check) Check Lower Bound
									RETURN_IF(data_ > get_lower_bound(), false);

									// check) Check Upper Bound
									RETURN_IF((data_ + size_) <= get_upper_bound(), false);

									// return) Success!
									return	true;
								}

	constexpr const void*		get_lower_bound() const noexcept { return bound.lower;}
	constexpr const void*		get_upper_bound() const noexcept { return bound.upper;}
protected:
	constexpr void				_set_bound(const buffer_bound&  _bound) { bound = _bound;}

	constexpr void				_check_bound(const _buffer_view<char>& _rhs) const
								{
									// check) 
									CGDK_ASSERT(_rhs.get_front_ptr() >= get_lower_bound(), ); 

									// check) 
									if(_rhs.get_front_ptr() < get_lower_bound())
										throw std::length_error("error! out of lower bound'");

									// check) 
									CGDK_ASSERT(_rhs.get_back_ptr() <= get_upper_bound(), );

									// check) 
									if (_rhs.get_back_ptr() > get_upper_bound())
										throw std::length_error("error! out of upper bound'");
								}
	// 11) source
			buffer_bound		bound;
};

constexpr CGDK::basic_buffer operator ^ (const CGDK::basic_buffer& _lhs, std::size_t _size)
{
	return CGDK::basic_buffer{ ((const CGDK::buffer_view&)_lhs) ^ _size, _lhs.get_bound() };
}

template <class U>
constexpr CGDK::basic_buffer operator ^ (const CGDK::basic_buffer& _lhs, CGDK::_buffer_view<U> _buffer)
{
	return CGDK::basic_buffer{ _buffer, _lhs.get_bound() };
}


}




#include "CGDK10/system/object/basic_buffer.inl"
