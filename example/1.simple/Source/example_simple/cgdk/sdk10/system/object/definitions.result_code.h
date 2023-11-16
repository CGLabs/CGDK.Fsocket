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

 @class		NBatch
 @brief		한꺼번에 여러 개의 Executable 객체를 큐잉해 걸어주는 Executable이다. 
 @todo		
 @ref		CList<TEXECUTABLE>
 @details	\n
 여러 개의 Executable 객체를 큐잉해 실행해준다. 
 따라서 실행을 걸어주게 되면 큐잉된 Executable 객체들은 제거된다.

*///-----------------------------------------------------------------------------
class result_code
{
public:
	using type = int32_t;

public:
	constexpr result_code() noexcept {}
	constexpr result_code(type _value) noexcept : m_result_code(_value) {}
	template <class ENUM_T, std::enable_if_t<std::is_error_code_enum_v<ENUM_T>, int> = 0>
	constexpr result_code(ENUM_T _enum_code) noexcept : m_result_code(static_cast<type>(_enum_code)) {}

public:
	constexpr void				assign(type _value) noexcept { m_result_code = _value; }
	constexpr void				clear() noexcept { m_result_code = 0; }
	constexpr type				value() const noexcept { return m_result_code; }
	template<class ENUM_T>
	constexpr ENUM_T			value() const noexcept { return static_cast<ENUM_T>(m_result_code); }

	template <class ENUM_T, std::enable_if_t<std::is_error_code_enum_v<ENUM_T>, int> = 0>
	constexpr result_code&		operator = (ENUM_T _enum_code) noexcept	{ m_result_code = static_cast<uint64_t>(_enum_code);;return *this; }

	[[nodiscard]] constexpr explicit operator bool() const noexcept { return value() != 0; }
	[[nodiscard]] constexpr		operator type() const noexcept { return m_result_code; }
	template <class ENUM_T, std::enable_if_t<std::is_error_code_enum_v<ENUM_T>, int> = 0>
	[[nodiscard]] constexpr		operator const ENUM_T() const noexcept { return static_cast<const ENUM_T>(m_result_code); }

private:
			type				m_result_code = 0;
};

[[nodiscard]] inline bool			operator == (const result_code& _lhs, const result_code& _rhs) noexcept { return _lhs.value() == _rhs.value(); }
[[nodiscard]] inline bool			operator != (const result_code& _lhs, const result_code& _rhs) noexcept { return !(_lhs == _rhs); }
[[nodiscard]] inline bool			operator <  (const result_code& _lhs, const result_code& _rhs) noexcept { return _lhs.value() < _rhs.value(); }
[[nodiscard]] inline bool			operator >  (const result_code& _lhs, const result_code& _rhs) noexcept { return _rhs < _lhs; }
[[nodiscard]] inline bool			operator <= (const result_code& _lhs, const result_code& _rhs) noexcept { return !(_lhs > _rhs); }
[[nodiscard]] inline bool			operator >= (const result_code& _lhs, const result_code& _rhs) noexcept { return !(_lhs < _rhs); }
[[nodiscard]] inline result_code	operator |  (const result_code& _lhs, const result_code& _rhs) noexcept { return result_code(_lhs.value() | _rhs.value()); }
[[nodiscard]] inline result_code	operator &  (const result_code& _lhs, const result_code& _rhs) noexcept { return result_code(_lhs.value() & _rhs.value()); }
[[nodiscard]] inline result_code	operator ^  (const result_code& _lhs, const result_code& _rhs) noexcept { return result_code(_lhs.value() ^ _rhs.value()); }

inline bool DOSS_SUCCESS(result_code _result)
{
	return _result < result_code(eRESULT::FAIL);
}

inline bool DOSS_FAILED(result_code _result)
{
	return !DOSS_SUCCESS(_result);
}

}


