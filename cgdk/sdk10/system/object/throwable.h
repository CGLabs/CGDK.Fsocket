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
// 1. Ithrowable
//
//-----------------------------------------------------------------------------
// 1) Ithrowable
class Ithrowable
{
public:
			Ithrowable() noexcept {}
	virtual	~Ithrowable() noexcept {}
public:
	virtual	const std::string& description() const noexcept = 0;
	//virtual	TSharedPtr<LOG_RECORD> get_log_record() const noexcept PURE;
};

// 2) Nthrowable
class Nthrowable :
	public						std::exception,
	virtual public				Ithrowable
{
public:
			Nthrowable() noexcept : m_description(), m_param(), m_reason(eRESULT::NOT_DEFINED) {}
			Nthrowable(const std::string& _description) noexcept : m_description(_description), m_param(0), m_reason(eRESULT::NOT_DEFINED) {}
			Nthrowable(std::string&& _description) noexcept : m_description(std::forward<std::string>(_description)), m_param(0), m_reason(eRESULT::NOT_DEFINED) {}
			Nthrowable(eRESULT _reason, intptr_t _param = 0) noexcept : m_description(), m_param(_param), m_reason(_reason) {}
			Nthrowable(eRESULT _reason, intptr_t _param, const std::string& _description) noexcept : m_description(_description), m_param(_param), m_reason(_reason) {}
			Nthrowable(eRESULT _reason, intptr_t _param, std::string&& _description) noexcept : m_description(std::move(_description)), m_param(_param), m_reason(_reason) {}
	virtual	~Nthrowable() noexcept {}

public:
	virtual char const* what() const noexcept override { return ""; }
	virtual	const std::string& description() const noexcept override { return m_description; }
	//virtual	TSharedPtr<LOG_RECORD> get_log_record() const noexcept override;

			eRESULT		reason() const noexcept { return m_reason; }
	template <class TRESULT = uint64_t>
			TRESULT			reason() const noexcept { return static_cast<TRESULT>(m_reason); }
			intptr_t		param() const noexcept { return m_param; }
	template <class TPARAM = intptr_t>
			TPARAM			param() const noexcept { return static_cast<TPARAM>(m_param); }

			auto&			operator >> (std::ostream& _stream_out) { stream_out(_stream_out); return *this; }
			auto&			operator >> (std::ostream& _stream_out) const { stream_out(_stream_out); return *this; }
			void			stream_out(std::ostream& _stream_out) const { _stream_out << description() << std::endl; }
public:
							//operator TSharedPtr<LOG_RECORD>() const noexcept { return get_log_record();}

public:
	// 1) Exception Message
			std::string		m_description;

	// 2) Exception parameter
			intptr_t		m_param;

	// 3) Exception Result
			eRESULT			m_reason;
};


//-----------------------------------------------------------------------------
//
// 2. throwable object
//
//-----------------------------------------------------------------------------
namespace throwable
{
	// 2) failure
	class failure : public Nthrowable
	{
	public:
				failure() noexcept : Nthrowable() {}
				failure(const std::string& _description) noexcept : Nthrowable(_description) {}
				failure(std::string&& _description) noexcept : Nthrowable(std::move(_description)) {}
				failure(eRESULT _reason, intptr_t _param = 0) noexcept : Nthrowable(_reason, _param) {}
				failure(eRESULT _reason, intptr_t _param, const std::string& _description) noexcept : Nthrowable(_reason, _param, _description) {}
				failure(eRESULT _reason, intptr_t _param, std::string&& _description) noexcept : Nthrowable(_reason, _param, std::move(_description)) {}
		virtual	~failure() noexcept {}
	};
}


}