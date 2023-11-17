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

namespace CGDK
{

constexpr u_short constexpr_htons(u_short _value) noexcept { return (_value << 8) | (_value >> 8); }
constexpr u_long constexpr_htonl(u_long _value) noexcept { return ((_value & 0xff000000u) >> 24) | ((_value & 0x00ff0000u) >> 8) | ((_value & 0x0000ff00u) << 8) | ((_value & 0x000000ffu) << 24); }

namespace net
{
namespace ip
{



}
}
}