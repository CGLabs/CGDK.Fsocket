//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*                       Ver 10.0 / Release 2019.12.11                       *
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
/**

 @class		Iattachable
 

*/
//-----------------------------------------------------------------------------
template <class TTYPE>
class object::Iattachable : virtual public Ireferenceable
{
public:
	virtual ~Iattachable() noexcept {}

public:
	//! @ ...
			bool				attach(TTYPE* _pchild) { return this->process_attach(_pchild);}
			bool				detach(TTYPE* _pchild) noexcept	{ return this->process_detach(_pchild);}

	#if defined(_WIN32)
			template <class T>
			std::enable_if_t<std::is_base_of_v<Iattachable<TTYPE>, T>, bool>
								attach(TTYPE* _pchild) { return this->process_attach(_pchild);}
			template <class T>
			std::enable_if_t<std::is_base_of_v<Iattachable<TTYPE>, T>, bool>
								detach(TTYPE* _pchild) noexcept { return this->process_detach(_pchild);}
	#endif

protected:
	virtual bool				process_attach(TTYPE* _pchild) PURE;
	virtual bool				process_detach(TTYPE* _pchild) noexcept PURE;
};


}