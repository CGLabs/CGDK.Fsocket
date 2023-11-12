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
// Nreferenceable_no_act
//                                                                            
//-----------------------------------------------------------------------------
class Nreferenceable_no_act : virtual public Ireferenceable
{
public:
			Nreferenceable_no_act() noexcept { add_ref(); }
			Nreferenceable_no_act(const Nreferenceable_no_act& /*_copy*/ ) noexcept { add_ref(); }
			Nreferenceable_no_act(Nreferenceable_no_act&& /*_move*/ ) noexcept { add_ref(); }
	virtual ~Nreferenceable_no_act() noexcept {}

public:
	virtual void process_dispose() noexcept override {}

	//! @brief 대입연산자
			Nreferenceable_no_act& operator = (const Nreferenceable_no_act&) noexcept { return *this; }
			Nreferenceable_no_act& operator = (Nreferenceable_no_act&&) noexcept { return *this; }

};


}