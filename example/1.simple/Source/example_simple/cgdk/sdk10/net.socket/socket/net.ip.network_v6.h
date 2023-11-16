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
class network_v6
{
// constructor) 
public:
	constexpr network_v6() noexcept : address_(), prefix_length_(0) {}
	constexpr network_v6(const address_v6& _addr, unsigned short _prefix_len) noexcept : address_(_addr), prefix_length_(_prefix_len) {}

// public) 
public:
	constexpr address_v6		address() const noexcept		{ return address_; }
	constexpr int				prefix_length() const noexcept	{ return prefix_length_; }
	constexpr address_v6		network() const noexcept
	{
		address_v6::bytes_type bytes(address_.to_bytes());

		for (std::size_t i = 0; i < 16; ++i)
		{
			if (prefix_length_ <= i * 8)
				bytes[i] = 0;
			else if (prefix_length_ < (i + 1) * 8)
				bytes[i] &= 0xFF00 >> (prefix_length_ % 8);
		}

		// return) 
		return address_v6(bytes, address_.scope_id());
	}
	//address_v6_range			hosts() const noexcept;
	constexpr network_v6		canonical() const noexcept		{ return network_v6(network(), prefix_length_); }
	constexpr bool				is_host() const noexcept		{ return prefix_length_ == 128; }
	constexpr bool				is_subnet_of(const network_v6& other) const noexcept;
	template<class Allocator = std::allocator<char>>
	std::basic_string<char, std::char_traits<char>, Allocator>
								to_string(const Allocator& a = Allocator()) const;

public:
		  address_v6			address_;
		  unsigned short		prefix_length_;
};

// network_v6 comparisons)
constexpr bool				operator == (const network_v6& _lhs, const network_v6& _rhs) noexcept { return _lhs.address_ == _rhs.address_ && _lhs.prefix_length_ == _rhs.prefix_length_; }
constexpr bool				operator != (const network_v6& _lhs, const network_v6& _rhs) noexcept { return !(_lhs == _rhs); }

// network_v6 creation)
constexpr network_v6		make_network_v6(const address_v6& addr, int prefix_len);
	   network_v6			make_network_v6(const std::string& _str);
	   network_v6			make_network_v6(const std::string& _str, std::error_code& _error_code) noexcept;
inline network_v6			make_network_v6(const char* _str)													{ return make_network_v6(std::string(_str)); }
inline network_v6			make_network_v6(const char* _str, std::error_code& _error_code) noexcept			{ return make_network_v6(std::string(_str), _error_code); }
inline network_v6			make_network_v6(std::string_view _str)												{ return make_network_v6(static_cast<std::string>(_str)); }
inline network_v6			make_network_v6(std::string_view _str, std::error_code& _error_code) noexcept		{ return make_network_v6(static_cast<std::string>(_str), _error_code); }

// network_v6 I/O)
template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& _os, const network_v6& _net);

constexpr bool network_v6::is_subnet_of(const network_v6& _other) const noexcept
{
	// check) 
	if (_other.prefix_length_ >= prefix_length_)
		return false; // Only real subsets are allowed.

	const network_v6 me(address_, _other.prefix_length_);

	// return) 
	return _other.canonical() == me.canonical();
}


}