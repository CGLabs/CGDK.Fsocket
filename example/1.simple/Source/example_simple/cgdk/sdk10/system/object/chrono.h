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
//----------------------------------------------------------------------------
//
//
//  Definitions for Event Classes
//
//
//----------------------------------------------------------------------------
#define _CGD_TIME
#define __CGDK_TIME_POINT

using namespace std::chrono_literals;

namespace std
{
namespace chrono
{
    using days  = duration<int, ratio<3600 * 24>>;
	using weeks = duration<int, std::ratio<3600 * 24 * 7>>;
}
}

namespace CGDK
{
namespace chrono
{

struct date_time
{
	int16_t		t_year = 0;
	uint16_t	t_month = 0;
	uint16_t	t_day = 0;
	uint16_t	t_hour = 0;
	uint16_t	t_minute = 0;
	uint16_t	t_second = 0;
	uint32_t	t_fraction = 0;
};

struct day_time
{
    uint32_t	t_day = 0;
    uint16_t	t_hour = 0;
    uint16_t	t_minute = 0;
    uint16_t	t_second = 0;
    uint32_t	t_fraction = 0;
};

struct fraction_part
{
    uint16_t	t_millisecond = 0;
    uint16_t	t_microsecond = 0;
    uint16_t	t_nanosecond = 0;
};

using nanoseconds  = std::chrono::nanoseconds;
using microseconds = std::chrono::microseconds;
using milliseconds = std::chrono::milliseconds;
using seconds      = std::chrono::seconds;
using minutes      = std::chrono::minutes;
using hours        = std::chrono::hours;
using days		   = std::chrono::days;
using weeks		   = std::chrono::weeks;

constexpr bool _is_leap_year(int _year) noexcept
{
	return _year % 4 == 0 && (_year % 100 != 0 || _year % 400 == 0);
}

constexpr uint8_t _get_last_day_of_month(int _year, int _month)
{
	//// declare) 월별 최대 일수
	////						                12   1   2   3   4   5   6   7   8   9  10  11  12
	//const std::array<uint8_t,13> byDayMax { 31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // 12월, 1월~11월

	//// return) 2월일 경우 윤연을 계산한다.
	//return	(_month != 2 || _is_leap_year(_year) == false) ? byDayMax.at(_month % 12) : 29;

    return 0;
}

template<class A = int>
constexpr A get_day_of_year(int _year, int _month, int _day) noexcept
{
	//// declare) [0] - 전달까지 지난 일 수 (평년)
	////          [1] - 전달까지 지난 일 수 (윤년)
	////      						      0    1    2    3    4    5    6    7    8    9   10   11   12  13  
	//constexpr std::array<int, 2 * 14> t { 0,   0,  31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334, 0,
	//								 	  0,   0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335, 0 };

	//return static_cast<A>(t.at(static_cast<std::size_t>(_is_leap_year(_year)) * 14 + _month) + _day);
    return 0;
}

template<class A = int>
constexpr A get_day_of_week(int _year, int _month, int _day) noexcept
{
	//// Static) 
	//constexpr std::array<int, 12> t{ 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };

	//// 1) 2월 을 초과하지 않았을 경우 1을 뺀다.
	//_year -= _month < 3;

	//// 2) 요일을 리턴한다.
	//return static_cast<A>((_year + _year / 4 - _year / 100 + _year / 400 + t.at(static_cast<std::size_t>(_month) - 1) + _day) % 7);
    return 0;
}

// from_string
template<class T = char, class A, class B>
std::chrono::duration<A, B>
from_string_db(std::basic_string_view<T> _string) noexcept;
template<class A, class B>
std::chrono::duration<A, B>
from_string_db(std::basic_string_view<char> _string) noexcept;
template<class A, class B>
std::chrono::duration<A, B>
from_string_db(std::basic_string_view<wchar_t> _string) noexcept;

template<class T1, class T2, class T3, class TDURATION>
constexpr std::tuple<T1, T2, T3>
_get_date(uint64_t _days) noexcept
{
    const typename TDURATION::rep z = _days + 719468;
    const typename TDURATION::rep era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = static_cast<unsigned>(z - era * 146097);
    const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    const auto y = static_cast<typename TDURATION::rep>(yoe) + era * 400;
    const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const unsigned mp = (5 * doy + 2) / 153;
    const unsigned result_days = doy - (153 * mp + 2) / 5 + 1;
    const unsigned result_months = mp + (mp < 10 ? 3 : -9);
    const typename TDURATION::rep result_years = y + (result_months <= 2);

	return std::tuple<T1, T2, T3>(static_cast<T3>(result_years), static_cast<T2>(result_months), static_cast<T1>(result_days));
}

template<class T1, class T2, class T3, class TDURATION>
constexpr std::tuple<T1, T2, T3>
_get_time(typename TDURATION::rep _time_epoch) noexcept
{
    const auto ticks_per_hour   = std::chrono::duration_cast<TDURATION>(std::chrono::hours(1)).count();
    const auto ticks_per_minute = std::chrono::duration_cast<TDURATION>(std::chrono::minutes(1)).count();
    const auto ticks_per_second = std::chrono::duration_cast<TDURATION>(std::chrono::seconds(1)).count();

	auto result_hours = (_time_epoch / ticks_per_hour) % 24;
	auto result_minutes = (_time_epoch / ticks_per_minute) % 60;
	auto result_seconds = (_time_epoch / ticks_per_second) % 60;

	return std::tuple<T1, T2, T3>(static_cast<T1>(result_hours), static_cast<T2>(result_minutes), static_cast<T3>(result_seconds));
}

template <class T1, class T2, class T3>
constexpr uint64_t 
_get_epoch(T1 _year, T2 _month, T3 _day)
{
    _year -= _month <= 2;
    const uint64_t era = (_year >= 0 ? _year : _year - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(_year - era * 400);
    const unsigned doy = (153 * (_month + (_month > 2 ? -3 : 9)) + 2) / 5 + _day - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return (era * 146097 + static_cast<uint64_t>(doe) - 719468);
}

template <class TCLOCK, class TDURATION>
constexpr tm
to_tm(const std::chrono::time_point<TCLOCK, TDURATION>& _time)
{
    const auto ticks_per_day = std::chrono::duration_cast<TDURATION>(std::chrono::hours(1)).count() * 24;
    const auto time_epoch = _time.time_since_epoch().count();
    const auto days = time_epoch / ticks_per_day;

    tm temp{};
    std::tie(temp.tm_year, temp.tm_mon, temp.tm_mday) = _get_date<int, int, int, TDURATION>(days);
    std::tie(temp.tm_hour, temp.tm_min, temp.tm_sec) = _get_time<int, int, int, TDURATION>(time_epoch);
	temp.tm_wday = get_day_of_week<int>(temp.tm_year, temp.tm_mon, temp.tm_mday);
	temp.tm_yday = get_day_of_year<int>(temp.tm_year, temp.tm_mon, temp.tm_mday);
	temp.tm_isdst = 0;

	return temp;
}

template <class TTIME_POINT = std::chrono::system_clock::time_point>
constexpr TTIME_POINT
from_tm(const tm& _tm)
{
    using TDURATION = typename TTIME_POINT::duration;
    const auto ticks_per_day = std::chrono::duration_cast<TDURATION>(std::chrono::hours(1)).count() * 24;
    const auto ticks_per_hour = std::chrono::duration_cast<TDURATION>(std::chrono::hours(1)).count();
    const auto ticks_per_minute = std::chrono::duration_cast<TDURATION>(std::chrono::minutes(1)).count();
    const auto ticks_per_second = std::chrono::duration_cast<TDURATION>(std::chrono::seconds(1)).count();

    uint64_t temp_tick = chrono::_get_epoch(_tm.tm_year, _tm.tm_mon, _tm.tm_mday) * ticks_per_day;
    temp_tick += _tm.tm_hour * ticks_per_hour;
    temp_tick += _tm.tm_min * ticks_per_minute;
    temp_tick += _tm.tm_sec * ticks_per_second;

	return TTIME_POINT(TDURATION(temp_tick));
}

template <class TCLOCK, class TDURATION>
constexpr date_time
to_date_time(const std::chrono::time_point<TCLOCK, TDURATION>& _time)
{
    const auto ticks_per_day = std::chrono::duration_cast<TDURATION>(std::chrono::hours(1)).count() * 24;
    const auto time_epoch = _time.time_since_epoch().count();
    const auto days = time_epoch / ticks_per_day;

    date_time temp;
    std::tie(temp.t_year, temp.t_month, temp.t_day) = _get_date<int16_t, uint16_t, uint16_t, TDURATION>(days);
    std::tie(temp.t_hour, temp.t_minute, temp.t_second) = _get_time<uint16_t, uint16_t, uint16_t, TDURATION>(time_epoch);

    const auto x = std::chrono::duration_cast<chrono::nanoseconds>(_time.time_since_epoch()).count();
    const auto y = 1'000'000'000ull;

    temp.t_fraction = static_cast<uint32_t>(x % y);

    return temp;
}

template <class TTIME_POINT = std::chrono::system_clock::time_point>
constexpr TTIME_POINT
from_date_time(const date_time& _date_time_faction)
{
    using TDURATION = typename TTIME_POINT::duration;
    const auto ticks_per_day = std::chrono::duration_cast<TDURATION>(std::chrono::hours(1)).count() * 24;
    const auto ticks_per_hour = std::chrono::duration_cast<TDURATION>(std::chrono::hours(1)).count();
    const auto ticks_per_minute = std::chrono::duration_cast<TDURATION>(std::chrono::minutes(1)).count();
    const auto ticks_per_second = std::chrono::duration_cast<TDURATION>(std::chrono::seconds(1)).count();

    uint64_t temp_tick = _get_epoch(_date_time_faction.t_year, _date_time_faction.t_month, _date_time_faction.t_day) * ticks_per_day;
    temp_tick += _date_time_faction.t_hour * ticks_per_hour;
    temp_tick += _date_time_faction.t_minute * ticks_per_minute;
    temp_tick += _date_time_faction.t_second * ticks_per_second;
    temp_tick += std::chrono::duration_cast<TDURATION>(chrono::nanoseconds(_date_time_faction.t_fraction)).count();

    return TTIME_POINT(TDURATION(temp_tick));
}

template <class TREP, class TPERIOD>
constexpr chrono::day_time
to_day_time(const std::chrono::duration<TREP, TPERIOD>& _duration)
{
    using TDURATION = std::chrono::duration<TREP, TPERIOD>;
    const auto ticks_per_day = std::chrono::duration_cast<TDURATION>(std::chrono::hours(1)).count() * 24;
    const auto ticks_per_hour = std::chrono::duration_cast<TDURATION>(std::chrono::hours(1)).count();
    const auto ticks_per_minute = std::chrono::duration_cast<TDURATION>(std::chrono::minutes(1)).count();
    const auto ticks_per_second = std::chrono::duration_cast<TDURATION>(std::chrono::seconds(1)).count();

    day_time temp;

    const auto time_epoch = _duration.count();
    temp.t_day = static_cast<uint32_t>(time_epoch / ticks_per_day);
    temp.t_hour = static_cast<uint16_t>((time_epoch / ticks_per_hour) % 24);
    temp.t_minute = static_cast<uint16_t>((time_epoch / ticks_per_minute) % 60);
    temp.t_second = static_cast<uint16_t>((time_epoch / ticks_per_second) % 60);
    temp.t_fraction = static_cast<uint32_t>(time_epoch % ticks_per_second);

    return temp;
}

template <class TREP, class TPERIOD>
constexpr chrono::fraction_part
to_faction_part(const std::chrono::duration<TREP, TPERIOD>& _duration)
{
    fraction_part temp;

    const auto fraction = std::chrono::duration_cast<chrono::nanoseconds>(_duration).count() % 1'000'000'000ull;
    const auto ticks_milli = fraction / 1'000'000ull;
    const auto ticks_micro = fraction - ticks_milli * 1'000'000ull;

    temp.t_millisecond = static_cast<uint16_t>(ticks_milli);
    temp.t_microsecond = static_cast<uint16_t>(ticks_micro / 1'000ull);
    temp.t_nanosecond = static_cast<uint16_t>(ticks_micro - temp.t_millisecond * 1'000ull);

    return temp;
}

template<class T = char>
constexpr date_time from_string_to_date_time(std::basic_string_view<T> _string, const T* _format = reinterpret_cast<const T*>(0));
template<> date_time from_string_to_date_time<char>(std::basic_string_view<char> _string, const char* _format);
template<> date_time from_string_to_date_time<wchar_t>(std::basic_string_view<wchar_t> _string, const wchar_t* _format);

template<class T = char>
constexpr day_time from_string_to_duration(std::basic_string_view<T> _string, const T* _format = reinterpret_cast<const T*>(0));
template<> day_time from_string_to_duration<char>(std::basic_string_view<char> _string, const char* _format);
template<> day_time from_string_to_duration<wchar_t>(std::basic_string_view<wchar_t> _string, const wchar_t* _format);


}

template<class T, class F = void>       class is_std_chrono_time_point : public std::false_type {};
template<class TCLOCK, class TDURATION> class is_std_chrono_time_point <std::chrono::time_point<TCLOCK, TDURATION>> : public std::true_type {};

template<class T, class F = void>       class is_std_chrono_duration : public std::false_type {};
template<class TREP, class TPERIOD>     class is_std_chrono_duration <std::chrono::duration<TREP, TPERIOD>> : public std::true_type {};

// from_string
template<class TTIME_POINT, class T = char>
inline std::enable_if_t<is_std_chrono_time_point<TTIME_POINT>::value, TTIME_POINT>
from_string(std::basic_string_view<T> _string, const T* _format = reinterpret_cast<const T*>(0)) noexcept
{
    // 1) get date_time
    auto temp_time = chrono::from_string_to_date_time<T>(_string, _format);

    // return) 
    return chrono::from_date_time<TTIME_POINT>(temp_time);
}

template<class TDURATION, class T = char>
inline std::enable_if_t<is_std_chrono_duration<TDURATION>::value, TDURATION>
from_string(const std::basic_string_view<T>& _string, const T* _format) noexcept
{
    // return) 
    //return chrono::from_date_time<TTIME_POINT>(temp_time);

    return TDURATION();
}

template<class TA, class T = char>
inline TA
from_string(const std::basic_string<T>& _string, const T* _format = reinterpret_cast<const T*>(0)) noexcept
{
    return from_string<TA, T>(static_cast<std::basic_string_view<T>>(_string), _format);
}

template<class TA, class T = char>
inline TA
from_string(const T* _string, const T* _format = reinterpret_cast<const T*>(0)) noexcept
{
    return from_string<TA, T>(std::basic_string_view<T>(_string), _format);
}

// to_string
template <class T = char>
inline std::basic_string<T>
to_string(const chrono::date_time&, /*[[maybe_unused]]*/ const T* _format = reinterpret_cast<const T*>(0)) noexcept 
{
    CGASSERT_ERROR(false);
    return std::basic_string<T>(); 
}
template <> std::basic_string<char> to_string<char>(const chrono::date_time& _time, const char* _format) noexcept;
template <> std::basic_string<wchar_t> to_string<wchar_t>(const chrono::date_time& _time, const wchar_t* _format) noexcept;

template <class T = char, class TCLOCK, class TDURATION>
inline std::basic_string<T>
to_string(std::chrono::time_point<TCLOCK, TDURATION> _time, const T* _format = reinterpret_cast<const T*>(0)) noexcept
{
    return to_string<T>(chrono::to_date_time(_time), _format);
}

std::string_view to_string(char(&_str_output)[64], const chrono::day_time& _time) noexcept;
std::wstring_view to_string(wchar_t(&_str_output)[64], const chrono::day_time& _time) noexcept;
std::u16string_view to_string(char16_t(&_str_output)[64], const chrono::day_time _time);
std::u32string_view to_string(char32_t(&_str_output)[64], const chrono::day_time _time);
std::string_view to_string(char(&_str_output)[64], const chrono::day_time _time, const chrono::fraction_part _faction) noexcept;
std::wstring_view to_string(wchar_t(&_str_output)[64], const chrono::day_time _time, const chrono::fraction_part _faction) noexcept;
std::u16string_view to_string(char16_t(&_str_output)[64], const chrono::day_time _time, const chrono::fraction_part _faction);
std::u32string_view to_string(char32_t(&_str_output)[64], const chrono::day_time _time, const chrono::fraction_part _faction);

template <class T = char>
inline std::basic_string<T>
to_string(const chrono::day_time& _time) noexcept
{
    T temp[64]{};
    return std::basic_string<T>(to_string(temp, _time));
}

template <class T = char>
inline std::basic_string<T>
to_string(const chrono::day_time _time, const chrono::fraction_part _fraction) noexcept
{
    T temp[64]{};
    return std::basic_string<T>(to_string(temp, _time, _fraction));
}

template <class T = char, class TREP, class TPERIOD>
inline std::basic_string<T>
to_string(std::chrono::duration<TREP, TPERIOD> _duration) noexcept
{
    return to_string<T>(chrono::to_day_time<TREP, TPERIOD>(_duration), chrono::to_faction_part<TREP, TPERIOD>(_duration));
}

// to_string
template <class T = char>
inline std::basic_string<T>
to_string(const tm&, /*[[maybe_unused]]*/ const T* _format = reinterpret_cast<const T*>(0)) noexcept
{
    CGASSERT_ERROR(false);
    return std::basic_string<T>(); 
}
template <> std::basic_string<char> to_string<char>(const tm& _time, const char* _format) noexcept;
template <> std::basic_string<wchar_t> to_string<wchar_t>(const tm& _time, const wchar_t* _format) noexcept;




}
