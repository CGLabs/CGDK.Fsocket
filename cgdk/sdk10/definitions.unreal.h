//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*              Ver 10.0 for Unreal Engine / Release 2019.12.11              *
//*                                                                           *
//*                          definitions for unreal                           *
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

namespace std
{
//-----------------------------------------------------------------------------
//
// std::string_view
//
//-----------------------------------------------------------------------------
#ifndef __cpp_lib_string_view
template <class _Elem, class _Traits = std::char_traits<_Elem>>
class basic_string_view
{
public:
		static_assert(is_same_v<_Elem, typename _Traits::char_type>,
			"Bad char_traits for basic_string_view; "
			"N4659 24.4.2 [string.view.template]/1 \"the type traits::char_type shall name the same type as charT.\"");

		using traits_type = _Traits;
		using value_type = _Elem;
		using pointer = _Elem*;
		using const_pointer = const _Elem*;
		using reference = _Elem&;
		using const_reference = const _Elem&;
		//using const_iterator = _String_view_iterator<_Traits>;
		//using iterator = const_iterator;
		//using const_reverse_iterator = _STD reverse_iterator<const_iterator>;
		//using reverse_iterator = const_reverse_iterator;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		static constexpr auto npos{ static_cast<size_type>(-1) };

		constexpr basic_string_view() noexcept : _Mydata(), _Mysize(0) {}

		constexpr basic_string_view(const basic_string_view&) noexcept = default;
		constexpr basic_string_view& operator=(const basic_string_view&) noexcept = default;
		template <class _Elem, class _Traits, class _Alloc>
		constexpr basic_string_view& operator=(const std::basic_string<_Elem, _Traits, _Alloc>& _string) noexcept
		{
			_Mydata = _string.c_str();
			_Mysize = _string.size();
			return *this;
		}
		template <class _Elem, class _Traits, class _Alloc>
		constexpr operator std::basic_string<_Elem, _Traits, _Alloc>()
		{
			return std::basic_string<_Elem, _Traits, _Alloc>(_Mydata, _Mysize);
		}

		/* implicit */ constexpr basic_string_view(_In_z_ const const_pointer _Ntcts) noexcept // strengthened
			: _Mydata(_Ntcts), _Mysize(_Traits::length(_Ntcts)) {}
		
		constexpr basic_string_view(_In_reads_(_Count) const const_pointer _Cts, const size_type _Count) noexcept // strengthened
			: _Mydata(_Cts), _Mysize(_Count)
		{
		#if _CONTAINER_DEBUG_LEVEL > 0
			_STL_VERIFY(_Count == 0 || _Cts, "non-zero size null string_view");
		#endif // _CONTAINER_DEBUG_LEVEL > 0
		}

		template <class _Elem, class _Traits, class _Alloc>
		constexpr basic_string_view(_In_z_ const std::basic_string< _Elem, _Traits, _Alloc>& _string) noexcept
			: _Mydata(_string.c_str()), _Mysize(_string.size()) {}

		//	constexpr const_iterator begin() const noexcept
		//	{
		//	#if _ITERATOR_DEBUG_LEVEL >= 1
		//		return const_iterator(_Mydata, _Mysize, 0);
		//	#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
		//		return const_iterator(_Mydata);
		//	#endif // _ITERATOR_DEBUG_LEVEL
		//	}
		//
		//	constexpr const_iterator end() const noexcept {
		//#if _ITERATOR_DEBUG_LEVEL >= 1
		//		return const_iterator(_Mydata, _Mysize, _Mysize);
		//#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
		//		return const_iterator(_Mydata + _Mysize);
		//#endif // _ITERATOR_DEBUG_LEVEL
		//	}
		//
		//	constexpr const_iterator cbegin() const noexcept {
		//		return begin();
		//	}
		//
		//	constexpr const_iterator cend() const noexcept {
		//		return end();
		//	}
		
		//	constexpr const_reverse_iterator rbegin() const noexcept {
		//		return const_reverse_iterator{ end() };
		//	}
		//
		//	constexpr const_reverse_iterator rend() const noexcept {
		//		return const_reverse_iterator{ begin() };
		//	}
		//
		//	constexpr const_reverse_iterator crbegin() const noexcept {
		//		return rbegin();
		//	}
		//
		//	constexpr const_reverse_iterator crend() const noexcept {
		//		return rend();
		//	}
		
			constexpr const_pointer _Unchecked_begin() const noexcept {
				return _Mydata;
			}
		
			constexpr const_pointer _Unchecked_end() const noexcept {
				return _Mydata + _Mysize;
			}
		
			constexpr size_type size() const noexcept {
				return _Mysize;
			}
		
			constexpr size_type length() const noexcept {
				return _Mysize;
			}
		
			constexpr bool empty() const noexcept {
				return _Mysize == 0;
			}
		
			constexpr const_pointer data() const noexcept {
				return _Mydata;
			}
		
			constexpr size_type max_size() const noexcept {
				// bound to PTRDIFF_MAX to make end() - begin() well defined (also makes room for npos)
				// bound to static_cast<size_t>(-1) / sizeof(_Elem) by address space limits
				return (_STD min)(static_cast<size_t>(PTRDIFF_MAX), static_cast<size_t>(-1) / sizeof(_Elem));
			}
		
			constexpr const_reference operator[](const size_type _Off) const noexcept /* strengthened */ {
		#if _CONTAINER_DEBUG_LEVEL > 0
				_STL_VERIFY(_Off < _Mysize, "string_view subscript out of range");
		#endif // _CONTAINER_DEBUG_LEVEL > 0
				return _Mydata[_Off];
			}
		
			constexpr const_reference at(const size_type _Off) const {
				// get the character at _Off or throw if that is out of range
				_Check_offset_exclusive(_Off);
				return _Mydata[_Off];
			}
		
			constexpr const_reference front() const noexcept /* strengthened */ {
		#if _CONTAINER_DEBUG_LEVEL > 0
				_STL_VERIFY(_Mysize != 0, "cannot call front on empty string_view");
		#endif // _CONTAINER_DEBUG_LEVEL > 0
				return _Mydata[0];
			}
		
			constexpr const_reference back() const noexcept /* strengthened */ {
		#if _CONTAINER_DEBUG_LEVEL > 0
				_STL_VERIFY(_Mysize != 0, "cannot call back on empty string_view");
		#endif // _CONTAINER_DEBUG_LEVEL > 0
				return _Mydata[_Mysize - 1];
			}
		
			constexpr void remove_prefix(const size_type _Count) noexcept /* strengthened */ {
		#if _CONTAINER_DEBUG_LEVEL > 0
				_STL_VERIFY(_Mysize >= _Count, "cannot remove prefix longer than total size");
		#endif // _CONTAINER_DEBUG_LEVEL > 0
				_Mydata += _Count;
				_Mysize -= _Count;
			}
		
			constexpr void remove_suffix(const size_type _Count) noexcept /* strengthened */ {
		#if _CONTAINER_DEBUG_LEVEL > 0
				_STL_VERIFY(_Mysize >= _Count, "cannot remove suffix longer than total size");
		#endif // _CONTAINER_DEBUG_LEVEL > 0
				_Mysize -= _Count;
			}
		
			constexpr void swap(basic_string_view& _Other) noexcept {
				const basic_string_view _Tmp{ _Other }; // note: std::swap is not constexpr before C++20
				_Other = *this;
				*this = _Tmp;
			}
		
			constexpr size_type copy(_Out_writes_(_Count) _Elem* const _Ptr, size_type _Count, const size_type _Off = 0) const {
				// copy [_Off, _Off + Count) to [_Ptr, _Ptr + _Count)
				_Check_offset(_Off);
				_Count = _Clamp_suffix_size(_Off, _Count);
				_Traits::copy(_Ptr, _Mydata + _Off, _Count);
				return _Count;
			}
		
			_Pre_satisfies_(_Dest_size >= _Count) constexpr size_type _Copy_s(_Out_writes_all_(_Dest_size) _Elem* const _Dest,
				const size_type _Dest_size, size_type _Count, const size_type _Off = 0) const {
				// copy [_Off, _Off + _Count) to [_Dest, _Dest + _Count)
				_Check_offset(_Off);
				_Count = _Clamp_suffix_size(_Off, _Count);
				_Traits::_Copy_s(_Dest, _Dest_size, _Mydata + _Off, _Count);
				return _Count;
			}
		
			constexpr basic_string_view substr(const size_type _Off = 0, size_type _Count = npos) const {
				// return a new basic_string_view moved forward by _Off and trimmed to _Count elements
				_Check_offset(_Off);
				_Count = _Clamp_suffix_size(_Off, _Count);
				return basic_string_view(_Mydata + _Off, _Count);
			}
		
			constexpr bool _Equal(const basic_string_view _Right) const noexcept {
				return _Traits_equal<_Traits>(_Mydata, _Mysize, _Right._Mydata, _Right._Mysize);
			}
		
			constexpr int compare(const basic_string_view _Right) const noexcept {
				return _Traits_compare<_Traits>(_Mydata, _Mysize, _Right._Mydata, _Right._Mysize);
			}
		
			constexpr int compare(const size_type _Off, const size_type _N0, const basic_string_view _Right) const {
				// compare [_Off, _Off + _N0) with _Right
				return substr(_Off, _N0).compare(_Right);
			}
		
			constexpr int compare(const size_type _Off, const size_type _N0, const basic_string_view _Right,
				const size_type _Roff, const size_type _Count) const {
				// compare [_Off, _Off + _N0) with _Right [_Roff, _Roff + _Count)
				return substr(_Off, _N0).compare(_Right.substr(_Roff, _Count));
			}
		
			constexpr int compare(_In_z_ const _Elem* const _Ptr) const { // compare [0, _Mysize) with [_Ptr, <null>)
				return compare(basic_string_view(_Ptr));
			}
		
			constexpr int compare(const size_type _Off, const size_type _N0, _In_z_ const _Elem* const _Ptr) const {
				// compare [_Off, _Off + _N0) with [_Ptr, <null>)
				return substr(_Off, _N0).compare(basic_string_view(_Ptr));
			}
		
			constexpr int compare(const size_type _Off, const size_type _N0,
				_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Count) const {
				// compare [_Off, _Off + _N0) with [_Ptr, _Ptr + _Count)
				return substr(_Off, _N0).compare(basic_string_view(_Ptr, _Count));
			}
		
		#if _HAS_CXX20
			constexpr bool starts_with(const basic_string_view _Right) const noexcept {
				const auto _Rightsize = _Right._Mysize;
				if (_Mysize < _Rightsize) {
					return false;
				}
				return _Traits::compare(_Mydata, _Right._Mydata, _Rightsize) == 0;
			}
		
			constexpr bool starts_with(const _Elem _Right) const noexcept {
				return !empty() && _Traits::eq(front(), _Right);
			}
		
			constexpr bool starts_with(const _Elem* const _Right) const noexcept /* strengthened */ {
				return starts_with(basic_string_view(_Right));
			}
		
			constexpr bool ends_with(const basic_string_view _Right) const noexcept {
				const auto _Rightsize = _Right._Mysize;
				if (_Mysize < _Rightsize) {
					return false;
				}
				return _Traits::compare(_Mydata + (_Mysize - _Rightsize), _Right._Mydata, _Rightsize) == 0;
			}
		
			constexpr bool ends_with(const _Elem _Right) const noexcept {
				return !empty() && _Traits::eq(back(), _Right);
			}
		
			constexpr bool ends_with(const _Elem* const _Right) const noexcept /* strengthened */ {
				return ends_with(basic_string_view(_Right));
			}
		#endif // _HAS_CXX20

			constexpr size_type find(const basic_string_view _Right, const size_type _Off = 0) const noexcept {
				// look for _Right beginning at or after _Off
				return _Traits_find<_Traits>(_Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize);
			}
		
			constexpr size_type find(const _Elem _Ch, const size_type _Off = 0) const noexcept {
				// look for _Ch at or after _Off
				return _Traits_find_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
			}
		
			constexpr size_type find(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
				const size_type _Count) const noexcept /* strengthened */ {
				// look for [_Ptr, _Ptr + _Count) beginning at or after _Off
				return _Traits_find<_Traits>(_Mydata, _Mysize, _Off, _Ptr, _Count);
			}
		
			constexpr size_type find(_In_z_ const _Elem* const _Ptr, const size_type _Off = 0) const noexcept
				/* strengthened */ {
				// look for [_Ptr, <null>) beginning at or after _Off
				return _Traits_find<_Traits>(_Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr));
			}
		
			constexpr size_type rfind(const basic_string_view _Right, const size_type _Off = npos) const noexcept {
				// look for _Right beginning before _Off
				return _Traits_rfind<_Traits>(_Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize);
			}
		
			constexpr size_type rfind(const _Elem _Ch, const size_type _Off = npos) const noexcept {
				// look for _Ch before _Off
				return _Traits_rfind_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
			}
		
			constexpr size_type rfind(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
				const size_type _Count) const noexcept /* strengthened */ {
				// look for [_Ptr, _Ptr + _Count) beginning before _Off
				return _Traits_rfind<_Traits>(_Mydata, _Mysize, _Off, _Ptr, _Count);
			}
		
			constexpr size_type rfind(_In_z_ const _Elem* const _Ptr, const size_type _Off = npos) const noexcept
				/* strengthened */ {
				// look for [_Ptr, <null>) beginning before _Off
				return _Traits_rfind<_Traits>(_Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr));
			}
		
			constexpr size_type find_first_of(const basic_string_view _Right,
				const size_type _Off = 0) const noexcept { // look for one of _Right at or after _Off
				return _Traits_find_first_of<_Traits>(
					_Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize, _Is_specialization<_Traits, char_traits>{});
			}
		
			constexpr size_type find_first_of(const _Elem _Ch, const size_type _Off = 0) const noexcept {
				// look for _Ch at or after _Off
				return _Traits_find_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
			}
		
			constexpr size_type find_first_of(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
				const size_type _Count) const noexcept /* strengthened */ {
				// look for one of [_Ptr, _Ptr + _Count) at or after _Off
				return _Traits_find_first_of<_Traits>(
					_Mydata, _Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{});
			}
		
			constexpr size_type find_first_of(
				_In_z_ const _Elem* const _Ptr, const size_type _Off = 0) const noexcept /* strengthened */ {
				// look for one of [_Ptr, <null>) at or after _Off
				return _Traits_find_first_of<_Traits>(
					_Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{});
			}
		
			constexpr size_type find_last_of(const basic_string_view _Right,
				const size_type _Off = npos) const noexcept { // look for one of _Right before _Off
				return _Traits_find_last_of<_Traits>(
					_Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize, _Is_specialization<_Traits, char_traits>{});
			}
		
			constexpr size_type find_last_of(const _Elem _Ch, const size_type _Off = npos) const noexcept {
				// look for _Ch before _Off
				return _Traits_rfind_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
			}
		
			constexpr size_type find_last_of(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
				const size_type _Count) const noexcept /* strengthened */ {
				// look for one of [_Ptr, _Ptr + _Count) before _Off
				return _Traits_find_last_of<_Traits>(
					_Mydata, _Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{});
			}
		
			constexpr size_type find_last_of(
				_In_z_ const _Elem* const _Ptr, const size_type _Off = npos) const noexcept /* strengthened */ {
				// look for one of [_Ptr, <null>) before _Off
				return _Traits_find_last_of<_Traits>(
					_Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{});
			}
		
			constexpr size_type find_first_not_of(const basic_string_view _Right,
				const size_type _Off = 0) const noexcept { // look for none of _Right at or after _Off
				return _Traits_find_first_not_of<_Traits>(
					_Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize, _Is_specialization<_Traits, char_traits>{});
			}
		
			constexpr size_type find_first_not_of(const _Elem _Ch, const size_type _Off = 0) const noexcept {
				// look for any value other than _Ch at or after _Off
				return _Traits_find_not_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
			}
		
			constexpr size_type find_first_not_of(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
				const size_type _Count) const noexcept /* strengthened */ {
				// look for none of [_Ptr, _Ptr + _Count) at or after _Off
				return _Traits_find_first_not_of<_Traits>(
					_Mydata, _Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{});
			}
		
			constexpr size_type find_first_not_of(
				_In_z_ const _Elem* const _Ptr, const size_type _Off = 0) const noexcept /* strengthened */ {
				// look for none of [_Ptr, <null>) at or after _Off
				return _Traits_find_first_not_of<_Traits>(
					_Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{});
			}
		
			constexpr size_type find_last_not_of(const basic_string_view _Right,
				const size_type _Off = npos) const noexcept { // look for none of _Right before _Off
				return _Traits_find_last_not_of<_Traits>(
					_Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize, _Is_specialization<_Traits, char_traits>{});
			}
		
			constexpr size_type find_last_not_of(const _Elem _Ch, const size_type _Off = npos) const noexcept {
				// look for any value other than _Ch before _Off
				return _Traits_rfind_not_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
			}
		
			constexpr size_type find_last_not_of(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
				const size_type _Count) const noexcept /* strengthened */ {
				// look for none of [_Ptr, _Ptr + _Count) before _Off
				return _Traits_find_last_not_of<_Traits>(
					_Mydata, _Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{});
			}
		
			constexpr size_type find_last_not_of(
				_In_z_ const _Elem* const _Ptr, const size_type _Off = npos) const noexcept /* strengthened */ {
				// look for none of [_Ptr, <null>) before _Off
				return _Traits_find_last_not_of<_Traits>(
					_Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{});
			}
		
			constexpr bool _Starts_with(const basic_string_view _View) const noexcept {
				return _Mysize >= _View._Mysize && _Traits::compare(_Mydata, _View._Mydata, _View._Mysize) == 0;
			}
		
		private:
			constexpr void _Check_offset(const size_type _Off) const { // checks whether _Off is in the bounds of [0, size()]
				if (_Mysize < _Off) {
					_Xran();
				}
			}
		
			constexpr void _Check_offset_exclusive(const size_type _Off) const {
				// checks whether _Off is in the bounds of [0, size())
				if (_Mysize <= _Off) {
					_Xran();
				}
			}
		
			constexpr size_type _Clamp_suffix_size(const size_type _Off, const size_type _Size) const noexcept {
				// trims _Size to the longest it can be assuming a string at/after _Off
				return (_STD min)(_Size, _Mysize - _Off);
			}
		
			static void _Xran() {
				_Xout_of_range("invalid string_view position");
			}

		const_pointer _Mydata;
		size_type _Mysize;
};

	// TYPEDEFS FOR basic_string_view
	typedef basic_string_view<char> string_view;
#ifdef __cpp_lib_char8_t
	typedef basic_string_view<char8_t> u8string_view;
#endif // __cpp_lib_char8_t
	typedef basic_string_view<char16_t> u16string_view;
	typedef basic_string_view<char32_t> u32string_view;
	typedef basic_string_view<wchar_t> wstring_view;
#endif


//// CLASS TEMPLATE array
//template <class _Ty, size_t _Size>
//class array { // fixed size array of values
//public:
//    using value_type      = _Ty;
//    using size_type       = size_t;
//    using difference_type = ptrdiff_t;
//    using pointer         = _Ty*;
//    using const_pointer   = const _Ty*;
//    using reference       = _Ty&;
//    using const_reference = const _Ty&;
//
//    //using iterator       = _Array_iterator<_Ty, _Size>;
//    //using const_iterator = _Array_const_iterator<_Ty, _Size>;
//
//    //using reverse_iterator       = _STD reverse_iterator<iterator>;
//    //using const_reverse_iterator = _STD reverse_iterator<const_iterator>;
//
//#ifdef _HAS_TR1_NAMESPACE
//    _DEPRECATE_TR1_NAMESPACE void assign(const _Ty& _Value) {
//        _STD fill_n(_Elems, _Size, _Value);
//    }
//#endif // _HAS_TR1_NAMESPACE
//
//    void fill(const _Ty& _Value) {
//        std::fill_n(_Elems, _Size, _Value);
//    }
//
//    void swap(array& _Other) noexcept {
//        _Swap_ranges_unchecked(_Elems, _Elems + _Size, _Other._Elems);
//    }
//
//	//constexpr iterator begin() noexcept { return iterator(_Elems, 0); }
//	//constexpr const_iterator begin() const noexcept { return const_iterator(_Elems, 0); }
//	//constexpr iterator end() noexcept { return iterator(_Elems, _Size); }
//	//constexpr const_iterator end() const noexcept { return const_iterator(_Elems, _Size); }
//	//constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
//	//constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
//	//constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
//	//constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
//	//constexpr const_iterator cbegin() const noexcept { return begin(); }
//	//constexpr const_iterator cend() const noexcept { return end(); }
//	//constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
//	//constexpr const_reverse_iterator crend() const noexcept { return rend(); }
//	constexpr _Ty* _Unchecked_begin() noexcept { return _Elems; }
//	constexpr const _Ty* _Unchecked_begin() const noexcept { return _Elems; }
//	constexpr _Ty* _Unchecked_end() noexcept { return _Elems + _Size; }
//	constexpr const _Ty* _Unchecked_end() const noexcept { return _Elems + _Size; }
//	constexpr size_type size() const noexcept { return _Size; }
//	constexpr size_type max_size() const noexcept { return _Size; }
//	constexpr bool empty() const noexcept { return false; }
//	constexpr reference at(size_type _Pos) { if (_Size <= _Pos) { _Xran(); } return _Elems[_Pos]; }
//	constexpr const_reference at(size_type _Pos) const { if (_Size <= _Pos) { _Xran(); } return _Elems[_Pos]; }
//	constexpr reference operator[](size_type _Pos) noexcept /* strengthened */ {
//#ifdef _CONTAINER_DEBUG_LEVEL
//	#if _CONTAINER_DEBUG_LEVEL > 0
//        _STL_VERIFY(_Pos < _Size, "array subscript out of range");
//	#endif // _CONTAINER_DEBUG_LEVEL > 0
//#endif
//        return _Elems[_Pos];
//    }
//	[[nodiscard]] constexpr const_reference operator[](size_type _Pos) const noexcept
//    /* strengthened */ {
//#ifdef _CONTAINER_DEBUG_LEVEL
//	#if _CONTAINER_DEBUG_LEVEL > 0
//        _STL_VERIFY(_Pos < _Size, "array subscript out of range");
//	#endif // _CONTAINER_DEBUG_LEVEL > 0
//#endif
//
//        return _Elems[_Pos];
//    }
//
//	constexpr reference front() noexcept /* strengthened */ { return _Elems[0]; }
//	constexpr const_reference front() const noexcept /* strengthened */ { return _Elems[0]; }
//	constexpr reference back() noexcept /* strengthened */ { return _Elems[_Size - 1]; }
//	constexpr const_reference back() const noexcept /* strengthened */ { return _Elems[_Size - 1]; }
//	constexpr _Ty* data() noexcept { return _Elems; }
//	constexpr const _Ty* data() const noexcept { return _Elems; }
//    void _Xran() const {}
//
//    _Ty _Elems[_Size];
//};
//


}

#include <array>

// 2) char8_t
#ifndef __cpp_char8_t
typedef uint8_t	char8_t;

namespace std
{
	typedef basic_string<char8_t>		u8string;
	typedef basic_string_view<char8_t>	u8string_view;
}
#endif



//-----------------------------------------------------------------------------
//                                                                            
// memory functions
//                                                                            
//                                                                            
//-----------------------------------------------------------------------------
namespace CGDK
{
	inline void	__zero_memory(void* _dest, size_t _size) noexcept
	{
		memset(_dest, 0, _size);
	}
}