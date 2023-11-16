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
class address_v6
{
// definitions)
public:
	using	bytes_type		 = std::array<unsigned char, 16>;
	using	short_type		 = std::array<unsigned short, 8>;
	using	uint_type		 = std::array<uint32_t, 4>;
	using	ull_type		 = std::array<uint64_t, 2>;
	using	scope_id_type	 = unsigned long;

// contructor)
public:
	constexpr address_v6() noexcept : ulls{0,0}, sin6_scope_id(0) {}
	constexpr address_v6(const address_v6& _copy) noexcept : bytes(_copy.bytes), sin6_scope_id(_copy.sin6_scope_id) {}
	constexpr address_v6(address_v6&& _move) noexcept : bytes(_move.bytes), sin6_scope_id(_move.sin6_scope_id) {}
	constexpr address_v6(bytes_type _bytes, scope_id_type _scope = 0) noexcept : bytes{_bytes}, sin6_scope_id{ _scope } {}
	constexpr address_v6(short_type _shorts, scope_id_type _scope = 0) noexcept : shorts{ _shorts }, sin6_scope_id{ _scope } {}
	constexpr address_v6(const in6_addr& _in6_addr, scope_id_type _scope = 0) noexcept : sin6_addr { _in6_addr }, sin6_scope_id{ _scope } {}

// public)
public:
	constexpr void				scope_id(scope_id_type _scope_id) noexcept	{ sin6_scope_id = _scope_id; }
	constexpr auto				scope_id() const noexcept					{ return sin6_scope_id; }
	constexpr bool				is_private() const noexcept					{ return shorts[0] == constexpr_htons(0xfc00);}
	constexpr bool				is_public() const noexcept					{ return !is_private();}
	constexpr bool				is_unspecified() const noexcept;
	constexpr bool				is_loopback() const noexcept;
	constexpr bool				is_multicast() const noexcept				{ return (sin6_addr.s6_addr[0] == 0xff); }
	constexpr bool				is_link_local() const noexcept				{ return ((sin6_addr.s6_addr[0] == 0xfe) && ((sin6_addr.s6_addr[1] & 0xc0) == 0x80)); }
	constexpr bool				is_site_local() const noexcept				{ return ((sin6_addr.s6_addr[0] == 0xfe) && ((sin6_addr.s6_addr[1] & 0xc0) == 0xc0)); }
	constexpr bool				is_v4_mapped() const noexcept;
	constexpr bool				is_multicast_node_local() const noexcept	{ return (is_multicast() && ((sin6_addr.s6_addr[1] & 0x0f) == 0x01)); }
	constexpr bool				is_multicast_link_local() const noexcept	{ return (is_multicast() && ((sin6_addr.s6_addr[1] & 0x0f) == 0x02)); }
	constexpr bool				is_multicast_site_local() const noexcept	{ return (is_multicast() && ((sin6_addr.s6_addr[1] & 0x0f) == 0x05)); }
	constexpr bool				is_multicast_org_local() const noexcept		{ return (is_multicast() && ((sin6_addr.s6_addr[1] & 0x0f) == 0x08)); }
	constexpr bool				is_multicast_global() const noexcept		{ return (is_multicast() && ((sin6_addr.s6_addr[1] & 0x0f) == 0x0e)); }
			  bool				is_v4_compatible() const noexcept;
	constexpr address_v4		get_v4_mapped() const noexcept;

	constexpr bytes_type		to_bytes() const noexcept					{ return bytes; }
	template<class T = char, class Allocator = allocator<T>>
	std::basic_string<T, std::char_traits<T>, Allocator> 
								to_string(const Allocator& _allocator = Allocator()) const;

	static constexpr address_v6 any() noexcept								{ return address_v6(); }
	static constexpr address_v6 loopback() noexcept							{ address_v6 tmp; tmp.sin6_addr.s6_addr[15] = 1; return tmp;}

// assignment)
	constexpr address_v6&		operator = (const address_v6& _rhs) noexcept;
public:
			union
			{
				bytes_type		bytes;
				short_type		shorts;
				uint_type		uints;
				ull_type		ulls;
				in6_addr		sin6_addr;
			};
			scope_id_type		sin6_scope_id;
};

// 21.6.5, address_v6 comparisons:
constexpr bool operator==(const address_v6& _lhs, const address_v6& _rhs) noexcept
{
	return _lhs.ulls[0] == _rhs.ulls[0] && _lhs.ulls[1] == _rhs.ulls[1] && _lhs.scope_id() == _rhs.scope_id();
}
constexpr bool operator!=(const address_v6& _lhs, const address_v6& _rhs) noexcept { return !(_lhs == _rhs); }
constexpr bool operator< (const address_v6& _lhs, const address_v6& _rhs) noexcept
{
	if (_lhs.ulls[0] < _rhs.ulls[0])
		return true;

	if (_lhs.ulls[0] > _rhs.ulls[0])
		return false;

	if (_lhs.ulls[1] < _rhs.ulls[1])
		return true;

	if (_lhs.ulls[1] > _rhs.ulls[1])
		return false;

	return _lhs.sin6_scope_id < _rhs.sin6_scope_id;
}
constexpr bool operator> (const address_v6& _lhs, const address_v6& _rhs) noexcept { return _rhs < _lhs; }
constexpr bool operator<=(const address_v6& _lhs, const address_v6& _rhs) noexcept { return !(_rhs < _lhs); }
constexpr bool operator>=(const address_v6& _lhs, const address_v6& _rhs) noexcept { return !(_lhs < _rhs); }

// 21.5.6, address_v4 creation:
constexpr address_v4	make_address_v4(v4_mapped_t, const address_v6& _address) { address_v6::bytes_type v6b = _address.to_bytes(); address_v4::bytes_type v4b{ v6b[12], v6b[13], v6b[14], v6b[15] }; return address_v4(v4b); }

// 21.6.6, address_v6 creation:
constexpr address_v6	make_address_v6(const address_v6::bytes_type& _bytes, address_v6::scope_id_type _scope_id = 0) { return address_v6 { _bytes , _scope_id }; }
constexpr address_v6	make_address_v6(v4_mapped_t, const address_v4& _address) noexcept { address_v4::bytes_type v4b = _address.to_bytes(); address_v6::bytes_type v6b{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, v4b[0], v4b[1], v4b[2], v4b[3] }; return address_v6(v6b); }
address_v6				make_address_v6(string_param _str, std::error_code& _ec) noexcept;
inline address_v6		make_address_v6(string_param _str) { std::error_code error_code; return make_address_v6(_str, error_code); }

constexpr bool address_v6::is_unspecified() const noexcept
{
	return ulls[0] == 0 && ulls[1] == 0;
}
constexpr bool address_v6::is_loopback() const noexcept
{
	if (is_v4_mapped() == false)
	{
		return ulls[0] == 0 && ulls[1] == 0x0100000000000000;
	}
	else
	{
		auto temp_addr_v4 = *(uint32_t*)&sin6_addr.s6_addr[12];
		return (temp_addr_v4 & 0xff000000) == 0x7f000000;
	}
}
constexpr bool address_v6::is_v4_mapped() const noexcept
{
	return ulls[0] == 0 && ulls[1] == 0xffff000000000000;
}

constexpr address_v6& address_v6::operator = (const address_v6& _rhs) noexcept
{
	sin6_addr = _rhs.sin6_addr;
	sin6_scope_id = _rhs.sin6_scope_id;

	return *this;
}

// 21.6.7, address_v6 I/O:
template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator << (std::basic_ostream<CharT, Traits>& os, const address_v6& addr);

template<class T, class Allocator>
std::basic_string<T, std::char_traits<T>, Allocator> 
address_v6::to_string(const Allocator& _allocator) const
{
	return CGDK::to_string<T, Allocator>(*this, _allocator);
}


}