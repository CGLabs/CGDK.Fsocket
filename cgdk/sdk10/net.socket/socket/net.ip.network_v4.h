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
class network_v4
{
// constructor) 
public:
	constexpr network_v4() noexcept : address_(), prefix_length_(0) {}
	constexpr network_v4(const address_v4& _addr, unsigned short _prefix_len);
	constexpr network_v4(const address_v4& _addr, const address_v4& _mask);

// public) 
	constexpr address_v4		address() const noexcept				{ return address_; }
	constexpr int				prefix_length() const noexcept			{ return prefix_length_; }
	constexpr address_v4		netmask() const noexcept;
	constexpr address_v4		network() const noexcept				{ return address_v4(address_.to_uint() & netmask().to_uint()); }
	constexpr address_v4		broadcast() const noexcept				{ return address_v4(network().to_uint() | (netmask().to_uint() ^ 0xffffffff)); }
	//address_v4_range			hosts() const noexcept;
	constexpr network_v4		canonical() const noexcept				{ return network_v4(network(), netmask()); }
	constexpr bool				is_host() const noexcept				{ return prefix_length_ == 32; }
	constexpr bool				is_subnet_of(const network_v4& _other) const noexcept;
	template<class TALLOCATOR = std::allocator<char>>
	std::basic_string<char, std::char_traits<char>, TALLOCATOR>
								to_string(const TALLOCATOR& _allocator = TALLOCATOR()) const;

public:
			address_v4			address_;
			unsigned short		prefix_length_;
};

// network_v4 comparisons)
constexpr bool				operator == (const network_v4& _lhs, const network_v4& _rhs) noexcept { return _lhs.address_ == _rhs.address_ && _lhs.prefix_length_ == _rhs.prefix_length_; }
constexpr bool				operator != (const network_v4& _lhs, const network_v4& _rhs) noexcept { return !(_lhs == _rhs); }

// network_v4 creation)
constexpr network_v4		make_network_v4(const address_v4& _address, int prefix_len)			{ return network_v4(_address, static_cast<unsigned short>(prefix_len)); }
constexpr network_v4		make_network_v4(const address_v4& _address, const address_v4& _mask){ return network_v4(_address, _mask); }
network_v4					make_network_v4(const char* str);
network_v4					make_network_v4(const char* str, std::error_code& ec) noexcept;
network_v4					make_network_v4(const std::string& str);
network_v4					make_network_v4(const std::string& str, std::error_code& ec) noexcept;
network_v4					make_network_v4(std::string_view str);
network_v4					make_network_v4(std::string_view str, std::error_code& ec) noexcept;

// network_v4 I/O)
template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const network_v4& net);


constexpr network_v4::network_v4(const address_v4& _addr, unsigned short _prefix_len) :
	address_(_addr),
	prefix_length_(_prefix_len)
{
	if (_prefix_len > 32)
		throw	std::out_of_range("prefix length too large");
}
constexpr network_v4::network_v4(const address_v4& _addr, const address_v4& _mask) :
	address_(_addr),
	prefix_length_(0)
{
	address_v4::bytes_type mask_bytes = _mask.to_bytes();
	bool finished = false;
	for (std::size_t i = 0; i < mask_bytes.size(); ++i)
	{
		if (finished)
		{
			if (mask_bytes[i])
			{
				throw std::invalid_argument("non-contiguous netmask");
			}
			continue;
		}
		else
		{
			switch (mask_bytes[i])
			{
			case 255:	prefix_length_ += 8; break;
			case 254:	prefix_length_ += 1; [[fallthrough]]; // prefix_length_ += 7
			case 252:	prefix_length_ += 1; [[fallthrough]]; // prefix_length_ += 6
			case 248:	prefix_length_ += 1; [[fallthrough]]; // prefix_length_ += 5
			case 240:	prefix_length_ += 1; [[fallthrough]]; // prefix_length_ += 4
			case 224:	prefix_length_ += 1; [[fallthrough]]; // prefix_length_ += 3
			case 192:	prefix_length_ += 1; [[fallthrough]]; // prefix_length_ += 2
			case 128:	prefix_length_ += 1; [[fallthrough]]; // prefix_length_ += 1
			case	0:	finished = true;		break;	// nbits += 0
			default:	throw std::out_of_range("non-contiguous netmask");
			}
		}
	}
}

constexpr address_v4 network_v4::netmask() const noexcept
{
	uint32_t nmbits = 0xffffffff;

	if (prefix_length_ == 0)
		nmbits = 0;
	else
		nmbits = nmbits << (32 - prefix_length_);

	return address_v4(nmbits);
}

constexpr bool network_v4::is_subnet_of(const network_v4& _other) const noexcept
{
	// check) 
	if (_other.prefix_length_ >= prefix_length_)
		return false; // Only real subsets are allowed.

	network_v4 me(address_, _other.prefix_length_);

	// return) 
	return _other.canonical() == me.canonical();
}


}