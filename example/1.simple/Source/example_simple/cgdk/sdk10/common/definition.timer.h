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

//-----------------------------------------------------------------------------
// time function
//-----------------------------------------------------------------------------
// 1) Standard Files
#include <chrono>

namespace CGDK
{

inline std::chrono::system_clock::time_point to_system_clock(const FDateTime& _datetime)
{
	return std::chrono::system_clock::time_point(std::chrono::system_clock::duration(_datetime.GetTicks() - FDateTime(1970, 1, 1).GetTicks()));
}

inline FDateTime to_FDateTime(const std::chrono::system_clock::time_point& _time_point)
{
	return FDateTime(_time_point.time_since_epoch().count() + FDateTime(1970, 1, 1).GetTicks());
}


}