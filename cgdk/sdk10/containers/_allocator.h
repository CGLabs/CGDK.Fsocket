//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*                       Ver 6.1 / Release 2012.05.28                        *
//*                                                                           *
//*                           Data Template Classes                           *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*  This Program is programmed by Cho SangHyun. sangducks@gmail.com          *
//*  Best for Game Developement and Optimized for Game Developement.          *
//*                                                                           *
//*                   (C) 2008 CGLabs All right reserved.                     *
//*                           http://www.CGDK.co.kr                           *
//*                                                                           *
//*****************************************************************************

#pragma once
//-----------------------------------------------------------------------------
//
// CGDK::allocator<T>
//
// 1. CGDK::list란!
//    말 그대로 list이다. 이 list의 특징은 circular linked list로 구현하여
//    push나 pop시 부하를 거의 최하수준으로 줄였다고 할수 있다. 실질적으로
//    vector와 거의 같은 수준의 부하로 줄일 수 있다.
//     node들은 circulare linked list로 연결되어 있고 head와 tail의 포인터들을
//    가지고 있다. 
//     이때 push를 하면 head의 포인터를 한칸 뒤로 밀뿐이며 pop할때는 한칸 땅길
//    뿐이다. 다 이때 한칸 밀공간이 남아 있지 않으면 그때서야 생성해서 삽입한다.
//     한번 push할때 생성된 Node는 pop때 사라지지 않고 pointer만 한칸 밀어버림
//    으로써 실질적인 삭제가 일어나지 않으며 다음에 push할때 포인터를 한칸 밀어
//    버림으로써 pop을 해서 사용하지 않게된 node를 다시 사용하는 식이 된다.
//     단 중간의 node를 insert할때나 erase하거나 remove하게 될 경우 삽입이나 
//    삭제가 일어나므로 이 부분에 대해서는 기존의 list와 성능이 똑같다.
//
// 2. CGDK::list의 핵심! Function의 기능 설명.
//    1) Add                하하하
//    2) Detach             호호호
//
// 3. CGDK::list의 주요 사용 용도
//    프로그래머 맘대로...
//
//
//-----------------------------------------------------------------------------
#include <new>

namespace CGDK	{

template<typename TDATA>
class allocator
{
public:
	typedef TDATA				value_type;
	typedef value_type*			pointer;
	typedef value_type&			reference;
	typedef const value_type*	const_pointer;
	typedef const value_type&	const_reference;
	typedef size_t				size_type;
	typedef ptrdiff_t			difference_type;

	template<typename _Other>
	struct rebind
	{
		typedef allocator<_Other> other;
	};


	allocator()														{}
	allocator(const allocator<TDATA>&) 								{}
	
	template<typename _Other>
	allocator(const allocator<_Other>&) 							{}


	template<typename _Other>
	allocator<TDATA>& operator=(const allocator<_Other>&)			{	return (*this);}

	pointer				address(reference _Val) const				{	return &_Val;}
	const_pointer		address(const_reference _Val) const			{	return &_Val;}

	pointer				allocate(size_type _Count)					{	return ((TDATA*)::operator new(_Count * sizeof (TDATA)));}
	pointer				allocate(size_type _Count, const void*)		{	return allocate(_Count);}
	void				deallocate(pointer _ptr, size_type)			{	::operator delete(_ptr);}

	void				construct(pointer _Ptr)						{	::new (_Ptr) TDATA;}
	void				construct(pointer _Ptr, const TDATA& _Val)	{	::new (_Ptr) TDATA(_Val);}
	void				construct(pointer _Ptr, TDATA&& _Val)		{	::new (_Ptr) TDATA(std::forward(_Val)); }

	void				destroy(pointer _ptr)						{	_ptr->~TDATA(); _ptr;}
	
	static size_type	max_size()		 							{	size_t _Count = (size_t)(-1) / sizeof (TDATA);	return (0 < _Count ? _Count : 1);}
};

template<class TALLOC, class TOTHER>
bool operator==(const allocator<TALLOC>&, const allocator<TOTHER>&) throw()
{
	return true;
}

template<class TALLOC, class TOTHER> 
bool operator!=(const allocator<TALLOC>&, const allocator<TOTHER>&) throw()
{
	return false;
}

}
