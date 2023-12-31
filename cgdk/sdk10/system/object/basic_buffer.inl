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
namespace CGDK
{

// extract) 
template<class B>	
class serializer_extract<B, basic_buffer>
{	
public:
	using type = basic_buffer;
	template<class S>
	constexpr static type _do(S& _s)
	{
		return S::_extract_basic_buffer(_s);
	}
};

template<class B, class T>
class serializer_extract<B, _shared_buffer<T>>
{	using TX = _shared_buffer<T>;
public:	
	using type = TX;
	template<class S> 
	constexpr static type _do(S& _s)
	{
		return _s.template _extract_shared_buffer<T>();
	}
};

// front)
template<class B>	
class serializer_peek<B, basic_buffer>
{	
public:
	using type = basic_buffer;
	template<class S> 
	constexpr static type _do(const S& _s, int64_t& _offset)
	{
		// 1) create temprary buffer
		S temp_buffer = _s + static_cast<std::size_t>(_offset);

		// 2) extract data
		auto temp = S::_extract_buffer(temp_buffer);

		// 3) calculate offset
		_offset = (int64_t)(temp_buffer.data() - _s.data());

		// return) 
		return temp;
	}
};

template<class B>	
class serializer_size_of<B, basic_buffer>
{	
public:	
	constexpr static std::size_t  _do(const basic_buffer& _object)
	{ 
		// return) 
		return	sizeof(std::size_t) + _object.size();
	}
};

template<class B>
class serializer_peek<B, _buffer_view<B>>
{
public:
	using type = basic_buffer;
	template<class S> 
	constexpr static type _do(const S& _s, int64_t& _offset)
	{
		// 1) create temprary buffer
		S temp_buffer = _s + static_cast<std::size_t>(_offset);

		// 2) extract data
		auto temp = temp_buffer._extract_buffer();

		// 3) calculate offset
		_offset = (int64_t)(temp_buffer.data() - _s.data());

		// return) 
		return temp;
	}
};

template<class B>
class serializer_size_of<B, _buffer_view<B>>
{	
public:
	constexpr static std::size_t  _do(const _buffer_view<B>& _object)
	{ 
		return	sizeof(std::size_t) + _object.size();
	}
};

template <class T>
bool validate_message(const T* _buffers, std::size_t _count)
{
	// declare)
	std::size_t	iTotalLength = 0;

	// 1) 전체 Message 길이를 구한다.
	for (std::size_t i = 0; i < _count; ++i)
	{
		iTotalLength += _buffers[i].size_;
	}

	// 2) buffer를 설정한다.
	std::size_t	idx_buffer = 0;
	const_buffer_view temp_buf = _buffers[idx_buffer];

	while (iTotalLength != 0)
	{
		// check) Message의 크기가 실제 버퍼의 크기보다 작으면 안됀다.
		check(idx_buffer <= _count);
		if (idx_buffer > _count) return false;

		// - Message의 길이를 구한다.
		int32_t	message_size = *temp_buf.data<int32_t>();

		// check) Message의 크기가 0Byte면 안됀다.
		check(message_size != 0);
		if (message_size == 0) return false;

		// check) Message의 크기가 실제 버퍼의 크기보다 작으면 안됀다.
		check(message_size <= static_cast<int32_t>(iTotalLength));
		if (message_size > static_cast<int32_t>(iTotalLength)) return false;

		// - 다음 Message
		iTotalLength -= message_size;

		// check) Total Bytes보다 더 긴 메시지를 요구할 경우 false를 리턴한다.
		check(iTotalLength >= 0);
		if (iTotalLength < 0) return false;

		while (message_size >= static_cast<int32_t>(temp_buf.size_))
		{
			message_size -= temp_buf.size_;

			++idx_buffer;

			BREAK_IF(message_size == 0);

			check(idx_buffer < _count);
			if (idx_buffer >= _count) return false;

			temp_buf = _buffers[idx_buffer];
		}

		temp_buf.data_ += message_size;
		temp_buf.size_ -= message_size;
	}

	return	true;
}

template <std::size_t OFFSET, std::size_t IALIGN>
struct any_constructor_append
{
	const char* source;
	std::size_t& offset;
	basic_buffer& buf_dest;

	template <class Type>
	constexpr operator Type () const
	{
		// 1) pre-offset
		offset = alliened_offset_pre<IALIGN>(offset, sizeof(Type));

		// 1) append data
		buf_dest.append<std::decay_t<Type>>(*reinterpret_cast<const Type*>(source + offset));

		// 2) add offset
		offset += sizeof(Type);

		// return) 
		return Type();
	}
};

template <class T, std::size_t... I>
constexpr void append_member_impl(const char* _source, basic_buffer& _buf_dest, std::index_sequence<I...>)
{
	std::size_t offset = 0;
	T{ any_constructor_append<I, alignof(T)>{_source, offset, _buf_dest}... };
}

template<class B, class T>
class serializer_append<B, T, std::enable_if_t<is_struct_serializable<T>::value>>
{
public:	
	using type = _buffer_view<typename B::element_t>;
	template<class S> 
	constexpr static type _do(S& _s, const T& _data)
	{
		// check) T must be default construcable
		static_assert(std::is_default_constructible<T>::value, "T must be default constructable");

		// check) 'Ibuffer_serializable' attribue is priorier than 'struct_serializable' attribute
		static_assert(!std::is_base_of_v<Ibuffer_serializable, T>, "'Ibuffer_serializable' attribue is priorier than 'struct_serializable' attribute");

		// check) T must derive 'is_struct_serializable_v<T>'
		static_assert(is_struct_serializable_v<T>, "T must have 'struct_serializable'");

		// check) T must be 'is_default_contructable<T>' attribute
		static_assert(std::is_default_constructible_v<T>, "'is_default_constructible<T>' class must have default constructor (remove construtor)");

		// 1) ...
		auto len_old = _s.size();
		append_member_impl<T>(reinterpret_cast<const char*>(&_data), _s, std::make_index_sequence<constexpr_field_count<T>>());

		// return) 
		return _buffer_view<typename B::element_t>(_s.data() + len_old, _s.size() - len_old);
	}
};


}