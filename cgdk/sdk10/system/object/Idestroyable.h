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
// Idestroyable
//                                                                            
//-----------------------------------------------------------------------------
class object::Idestroyable : 
// Inherited classes)
	virtual public				Ireferenceable
{
// destructor)
public:
	virtual ~Idestroyable() noexcept {}

// Publics)
public:
	//! @brief 소멸한다. @return true 성공 @return false 실패
	virtual	bool				destroy() noexcept PURE;
};


}