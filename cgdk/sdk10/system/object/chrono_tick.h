//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*                       Ver 10.0 / Release 2019.12.11                       *
//*                                                                           *
//*                             tick clock classes                            *
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
//                                                                            
// tick clock
//                                                                            
//                                                                            
//-----------------------------------------------------------------------------
namespace CGDK
{
namespace chrono
{
#if defined(_USE_TICK_CLOCK_HIGH_RESOLUTION)
	// - Standard Library Timer (High Resolution)
	using clock = std::chrono::high_resolution_clock;
#elif defined(_USE_TICK_CLOCK_MONOTONIC)
	// - Standard Library Timer (Monotonic Timer)
	using clock = std::chrono::monotonic_clock;
#elif defined(_USE_TICK_CLOCK_SYSTEM)
	// - Standard Library Timer (System Timer)
	using clock = std::chrono::system_clock;
#elif defined(_USE_TICK_CLOCK_STEADY)
	// - Standard Library TImer (Steady Clock)
	using clock = std::chrono::steady_clock;
#else
	// - Not Defined!
	#error "CGDK::tick not defined!"
#endif
	namespace tick
	{
		using rep = clock::rep;
		using period = clock::period;
		using duration = clock::duration;

		const auto ticks_per_day = std::chrono::duration_cast<clock::duration>(std::chrono::hours(1)) * 24;
		const auto ticks_per_hour = std::chrono::duration_cast<clock::duration>(std::chrono::hours(1));
		const auto ticks_per_minute = std::chrono::duration_cast<clock::duration>(std::chrono::minutes(1));
		const auto ticks_per_second = std::chrono::duration_cast<clock::duration>(std::chrono::seconds(1));
		const auto ticks_per_millisecond = std::chrono::duration_cast<clock::duration>(std::chrono::milliseconds(1));
		const auto ticks_per_microsecond = std::chrono::duration_cast<clock::duration>(std::chrono::microseconds(1));

		class time_point : public clock::time_point
		{
		public:
			constexpr time_point() = default;
			template <class TCLOCK, class TDURATION = typename TCLOCK::duration>
			constexpr time_point(const std::chrono::time_point<TCLOCK,TDURATION>& _time) noexcept : clock::time_point(_time) {}
			constexpr time_point(const duration& _other) noexcept : clock::time_point(_other) {}
			template <class TDURATION, class = std::enable_if_t<std::is_convertible_v<TDURATION, duration>>>
			constexpr time_point(const std::chrono::time_point<clock, TDURATION> & _tp) noexcept(std::is_arithmetic_v<rep> && std::is_arithmetic_v<typename TDURATION::rep>) : clock::time_point(_tp.time_since_epoch()) {}
					time_point& operator+=(const duration& _dur) noexcept(std::is_arithmetic_v<rep>) { clock::time_point::operator += (_dur); return *this; }
					time_point& operator-=(const duration& _dur) noexcept(std::is_arithmetic_v<rep>) { clock::time_point::operator -= (_dur); return *this; }
			[[nodiscard]] static constexpr time_point(min)() noexcept { return time_point((clock::duration::min)()); }
			[[nodiscard]] static constexpr time_point(max)() noexcept { return time_point((clock::duration::max)()); }
		};
		[[nodiscard]] inline		   time_point now() noexcept	{ return clock::now(); }
		[[nodiscard]] inline constexpr time_point zero()			{ return time_point(duration::zero()); }
		[[nodiscard]] inline constexpr time_point(max)()			{ return time_point((duration::max)()); }

		// durations
		using nanoseconds = std::chrono::nanoseconds;
		using microseconds = std::chrono::microseconds;
		using milliseconds = std::chrono::milliseconds;
		using seconds = std::chrono::seconds;
		using minutes = std::chrono::minutes;
		using hours = std::chrono::hours;
		using days = std::chrono::duration<int, std::ratio<3600 * 24>>;
		using weeks = std::chrono::duration<int, std::ratio<3600 * 24 * 7>>;
	}
}
}