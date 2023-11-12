//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*                       Ver 10.0 / Release 2019.12.11                       *
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

namespace CGDK::net::ip
{

//-----------------------------------------------------------------------------
//
// net::ip::address
//
//
//-----------------------------------------------------------------------------
class address_v4
{
// definitions)
public:
	using	uint_type	 = uint_least32_t;
	using	bytes_type	 = std::array<unsigned char, 4>;

// constructor) 
public:
	constexpr					address_v4() noexcept : S_addr{ 0 } {}
	constexpr					address_v4(const address_v4& _copy) noexcept: bytes(_copy.bytes) {}
	constexpr					address_v4(bytes_type _bytes) noexcept : bytes{ _bytes } {}
	constexpr					address_v4(uint_type _val) noexcept			{ S_addr = constexpr_htonl(_val); }
	constexpr explicit			address_v4(uint_type _val, bool) noexcept : S_addr{ _val } {}

// public)
public:
	constexpr bool				is_class_a() const noexcept					{ return (to_uint() & 0x80000000) == 0; }
	constexpr bool				is_class_b() const noexcept					{ return (to_uint() & 0xc0000000) == 0x80000000; }
	constexpr bool				is_class_c() const noexcept					{ return (to_uint() & 0xe0000000) == 0xc0000000; }
	constexpr bool				is_private() const noexcept					{ return S_un_b.s_b1 == 10;}
	constexpr bool				is_public() const noexcept					{ return !is_private();}
	constexpr bool				is_unspecified() const noexcept				{ return to_uint() == 0;}
	constexpr bool				is_loopback() const noexcept				{ return (to_uint() & 0xff000000) == 0x7f000000; }
	constexpr bool				is_multicast() const noexcept				{ return (to_uint() & 0xf0000000) == 0xe0000000; }
	constexpr bytes_type		to_bytes() const noexcept					{ return bytes;}
	constexpr uint_type			to_uint() const noexcept					{ return constexpr_htonl(S_addr);}
	template<class T = char, class Allocator = allocator<T>>
	std::basic_string<T, std::char_traits<T>, Allocator> 
								to_string(const Allocator& _allocator = Allocator()) const;

	static constexpr address_v4	any() noexcept								{ return address_v4();}
	static constexpr address_v4 loopback() noexcept							{ return address_v4(0x7f000001); }
	static constexpr address_v4 broadcast() noexcept						{ return address_v4(0xffffffff); }

// assignment)
	constexpr address_v4&		operator = (const address_v4& _rhs) noexcept{ bytes = _rhs.bytes; return *this; }

public:
	union
	{
		bytes_type	bytes;
		struct		{ uint8_t s_b1, s_b2, s_b3, s_b4; } S_un_b;
		struct		{ uint16_t s_w1, s_w2; } S_un_w;
		uint32_t	S_addr{ 0 };
	};
};

// 21.5.5, address_v4 comparisons:
constexpr bool operator==(const address_v4& _lhs, const address_v4& _rhs) noexcept	{ return _lhs.to_uint() == _rhs.to_uint(); }
constexpr bool operator!=(const address_v4& _lhs, const address_v4& _rhs) noexcept	{ return _lhs.to_uint() != _rhs.to_uint(); }
constexpr bool operator< (const address_v4& _lhs, const address_v4& _rhs) noexcept	{ return _lhs.to_uint() <  _rhs.to_uint(); }
constexpr bool operator> (const address_v4& _lhs, const address_v4& _rhs) noexcept	{ return _lhs.to_uint() >  _rhs.to_uint(); }
constexpr bool operator<=(const address_v4& _lhs, const address_v4& _rhs) noexcept	{ return _lhs.to_uint() <= _rhs.to_uint(); }
constexpr bool operator>=(const address_v4& _lhs, const address_v4& _rhs) noexcept	{ return _lhs.to_uint() >= _rhs.to_uint(); }

// 21.5.6, address_v4 creation:
enum class v4_mapped_t { v4_mapped };
constexpr address_v4	make_address_v4(address_v4::bytes_type _bytes)				{ return address_v4(_bytes); }
constexpr address_v4	make_address_v4(address_v4::uint_type _val)					{ return address_v4(_val); }
constexpr address_v4	make_address_v4(v4_mapped_t, const address_v6& _address);
address_v4				make_address_v4(string_param _str, std::error_code& _ec) noexcept;
inline address_v4		make_address_v4(string_param _str)							{ std::error_code ec; return make_address_v4(_str, ec); }

// 21.5.7, address_v4 I/O:
template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& _output_stream, const address_v4& _address);

template<class T, class Allocator>
std::basic_string<T, std::char_traits<T>, Allocator> 
address_v4::to_string(const Allocator& _allocator) const
{
	return CGDK::to_string<T, Allocator>(*this, _allocator);
}


}