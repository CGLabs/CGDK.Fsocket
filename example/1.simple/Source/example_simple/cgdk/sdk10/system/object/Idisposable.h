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

namespace CGDK
{
//-----------------------------------------------------------------------------
//                                                                            
// Idisposable
//                                                                            
//-----------------------------------------------------------------------------
class Idisposable
{
// constructor/destructor) 
public:
			Idisposable() noexcept CGNEW_DEBUG_INFO_COLON CGNEW_DEBUG_INFO_INITIALIZE {}
			Idisposable(const Idisposable&) noexcept CGNEW_DEBUG_INFO_COLON CGNEW_DEBUG_INFO_INITIALIZE {}
	virtual	~Idisposable() noexcept {}

// Publics) 
public:
	// Definitions) 기본 정의 Class
			class Nno_act;
			class Ndelete;

// framework) 
protected:
	virtual	void				process_dispose() noexcept = 0;

	// Debug)
	CGNEW_DEBUG_INFO_DECLARE
};


// Releaser) 
class Idisposable::Nno_act : virtual public Idisposable
{
public: 
			Nno_act() noexcept {}
			Nno_act(Nno_act&&) noexcept {}
			Nno_act(const Nno_act&) noexcept {}
	virtual ~Nno_act() noexcept {}

	virtual void process_dispose() noexcept override {}

	Nno_act& operator=(const Nno_act&) noexcept { return *this; }
	Nno_act& operator=(const Nno_act&&) noexcept { return *this; }
};

class Idisposable::Ndelete : virtual public Idisposable
{
public:
	virtual ~Ndelete() noexcept {}

	virtual void process_dispose() noexcept override { delete this; }
};


}