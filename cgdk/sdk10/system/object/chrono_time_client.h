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
namespace CGDK
{
namespace chrono
{

namespace time
{
namespace client
{
    class time_point;
}
}

struct client_clock
{
    using rep                        = long long;
    static const rep nanoseconds_per_tick = 100;
    static const rep ticks_per_time_t = 10000000LL;
    static const rep diff_tick       = 621355968000000000;
    using period                     = std::ratio_multiply<std::ratio<nanoseconds_per_tick, 1>, std::nano>;

    using duration = std::chrono::duration<rep, period>;
    using time_point = time::client::time_point;

    static constexpr bool is_steady  = false;

    [[nodiscard]] constexpr static time_t     to_time_t(const time_point& _time) noexcept;
    [[nodiscard]] constexpr static time_point from_time_t(time_t _tm) noexcept;
};

namespace time
{
namespace client
{
    using duration = client_clock::duration;

    template<class TDURATION> constexpr static duration _to_client_tick(TDURATION _duration) 
    {
        const auto diff = duration(client_clock::diff_tick);
        return std::chrono::duration_cast<duration>(_duration) + diff;
    }
    template<class TDURATION> constexpr static TDURATION _from_client_tick(duration _duration)
    {
        const auto diff = duration(client_clock::diff_tick);
        return std::chrono::duration_cast<TDURATION>(_duration - diff);
    }

    class time_point
    {
    public:
        using clock    = client_clock;
        using duration = client_clock::duration;
        using rep      = client_clock::rep;
        using period   = client_clock::period;
    
        constexpr time_point() = default;
        constexpr explicit time_point(const duration& _Other) noexcept : m_duration(_Other) {}
        constexpr time_point(const time_point& _time_point) noexcept : m_duration(_time_point.time_since_epoch()) {}
        template <class TCLOCK, class TDURATION>
        constexpr time_point(const std::chrono::time_point<TCLOCK, TDURATION>& _time_point) noexcept(std::is_arithmetic_v<typename TDURATION::rep>): m_duration(_to_client_tick(_time_point.time_since_epoch())) {}

        [[nodiscard]] constexpr duration time_since_epoch() const noexcept { return m_duration; }
    
        template <class TCLOCK, class TDURATION>
        constexpr time_point& operator =(const std::chrono::time_point<TCLOCK, TDURATION>& _time_point) noexcept { m_duration = _to_client_tick(_time_point.time_since_epoch()); return *this; }
                time_point& operator+=(const duration& _duration) noexcept { m_duration += _duration; return *this; }
                time_point& operator-=(const duration& _duration) noexcept { m_duration -= _duration; return *this; }

        template <class TTIME_POINT, class TFLAG = std::enable_if_t<is_std_chrono_time_point<TTIME_POINT>::value, void>>
        constexpr TTIME_POINT timepoint_cast() const noexcept { return TTIME_POINT(_from_client_tick<typename TTIME_POINT::duration>(m_duration)); }

        template <class TTIME_POINT, class TFLAG = std::enable_if_t<is_std_chrono_time_point<TTIME_POINT>::value, void>>
        constexpr operator TTIME_POINT() const noexcept { return timepoint_cast<TTIME_POINT>(); }

        [[nodiscard]] static constexpr time_point(min)() noexcept { return time_point((duration::min)()); }
        [[nodiscard]] static constexpr time_point(max)() noexcept { return time_point((duration::max)()); }

    private:
        duration m_duration{duration::zero()};
    };

    [[nodiscard]] constexpr std::time_t to_time_t(const time_point& _time) noexcept { return client_clock::to_time_t(_time); }
    [[nodiscard]] constexpr time_point from_time_t(std::time_t _tm) noexcept { return client_clock::from_time_t(_tm); }
}

constexpr utc::time_point::time_point(const client::time_point& _client_time) : clock::time_point(client::_from_client_tick<clock::duration>(_client_time.time_since_epoch())) {}
constexpr utc::time_point& utc::time_point::operator = (const client::time_point& _rhs) { clock::time_point::operator=(clock::time_point(client::_from_client_tick<clock::duration>(_rhs.time_since_epoch()))); return *this; }

constexpr local::time_point::time_point(const client::time_point& _client_time) : clock::time_point(client::_from_client_tick<clock::duration>(_client_time.time_since_epoch())) {}
constexpr local::time_point& local::time_point::operator = (const client::time_point& _rhs) { clock::time_point::operator=(clock::time_point(client::_from_client_tick<clock::duration>(_rhs.time_since_epoch()))); return *this; }

}

[[nodiscard]] constexpr time_t client_clock::to_time_t(const client_clock::time_point& _time) noexcept { return static_cast<time_t>((_time.time_since_epoch().count() - diff_tick) / ticks_per_time_t); }
[[nodiscard]] constexpr client_clock::time_point client_clock::from_time_t(time_t _tm) noexcept { return client_clock::time_point(client_clock::duration((_tm + diff_tick) * ticks_per_time_t)); }


inline date_time
to_date_time(const client_clock::time_point& _time)
{
    auto duractioin_system = chrono::time::client::_from_client_tick<std::chrono::system_clock::duration>(_time.time_since_epoch());
    return to_date_time(std::chrono::system_clock::time_point(duractioin_system));
}

template<>
inline client_clock::time_point from_date_time<client_clock::time_point>(const date_time& _date_time_faction)
{
    return client_clock::time_point(from_date_time<std::chrono::time_point<std::chrono::system_clock, client_clock::duration>>(_date_time_faction));
}

}

template <class T = char>
inline std::basic_string<T>
to_string(const chrono::client_clock::time_point& _time, const T* _format = reinterpret_cast<const T*>(0)) noexcept
{
    return to_string<T>(chrono::to_date_time(_time), _format);
}



}
