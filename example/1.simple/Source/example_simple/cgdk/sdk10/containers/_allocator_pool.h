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
//    말 그대로 list이다. 이 list의 특징은 ring형태의 linked list로 구현하여
//    push나 pop시 부하를 거의 최하수준이라고 할수 있다.
//    최대 rist를 넘어설 경우에만 list를 새로 만들어서 추가하는 부하가 있을
//    따름이다. 
//    최대크기의 list를 한번 만들고 나면 그다음부터는 부하가 거의 없다고 보면 된다.
//    따라서 이 linked-list 최고 성능는 최고의 성능을 나타낸다.
//    그러나 list를 새로 생성하거나 혹은 지우게 되면 많은 부하를 동반하므로 
//    Temporary로 사용되는 용도로는 이 list의 사용을 추천하지 않는다. 
//
// 2. CGNetTplHeadArray의 핵심! Function의 기능 설명.
//    1) Add                하하하
//    2) Detach             호호호
//
// 3. CGNetTplHeadArray의 주요 사용 용도
//    무화화화화
//
//
//-----------------------------------------------------------------------------
namespace CGDK
{

template<typename TYPE>
class allocator_pool
{
// ****************************************************************************
// Definitions)
// ----------------------------------------------------------------------------
public:
	// 1) Default type
	typedef TYPE				value_type;
	typedef value_type*			pointer;
	typedef value_type&			reference;
	typedef const value_type*	const_pointer;
	typedef const value_type&	const_reference;
	typedef size_t				size_type;
	typedef ptrdiff_t			difference_type;

	// 2) convert an allocator<TYPE> to an allocator <_Other>
	template<typename _Other>
	struct rebind
	{	
		typedef allocator_pool<_Other> other;
	};


// ****************************************************************************
// Constructors)
// ----------------------------------------------------------------------------
public:
	allocator_pool() noexcept : m_pHead(nullptr)							{}
	allocator_pool(const allocator_pool<TYPE>&) noexcept : m_pHead(nullptr)	{}
	template<typename _Other>
	allocator_pool(const allocator_pool<_Other>&) noexcept : m_pHead(nullptr){}

	~allocator_pool() noexcept;

	
// ****************************************************************************
// Allocator의 Opeartion
// ----------------------------------------------------------------------------
public:
	// 1) Address
	pointer				address(reference _Val) const				{	return (&_Val);}
	const_pointer		address(const_reference _Val) const			{	return (&_Val);}

	// 2) Assignment Opeator
	template<typename _Other>
	allocator_pool<TYPE>& operator=(const allocator_pool<_Other>&)	{	return (*this);}	// assign from a related allocator (do nothing)

	// 3) Allocate(할당하는 함수)
	pointer				allocate(size_type _Count);
	pointer				allocate(size_type _Count, const void*)		{	return allocate(_Count);}

	// 4) Deallocate(할당 해제하는 함수.)
	void				deallocate(const pointer _Ptr, size_type) noexcept;

	// 5) Construct/Destroy (주소지정 생성자와 소멸자 호출)
	void				construct(pointer _Ptr)
	{
	#pragma push_macro("new")
	#undef new
		new (_Ptr) TYPE();
	#pragma pop_macro("new")
	}
	void				construct(pointer _Ptr, const TYPE& _Val)	
	{
	#pragma push_macro("new")
	#undef new
		new (_Ptr) TYPE(_Val);
	#pragma pop_macro("new")
	}
	void				construct(pointer _Ptr, TYPE&& _Val)	
	{
	#pragma push_macro("new")
	#undef new
		new (_Ptr) TYPE(std::move(_Val));
	#pragma pop_macro("new")
	}
	void				destroy( pointer _Ptr)		{ (_Ptr)->~TYPE();}

	// 6) Max Size (최다 할당 갯수)
	size_type			max_size() const							{ size_type count = (size_type)(-1) / sizeof (TYPE);	return (0 < count ? count : 1);}	// estimate maximum array size


// ****************************************************************************
// Pool
// ----------------------------------------------------------------------------
private:
				TYPE*	m_pHead;

	#undef	NO_WARNNING
};


template<typename TYPE>
allocator_pool<TYPE>::~allocator_pool() noexcept
{
	while(m_pHead !=0)
	{
		TYPE*	pHead	 = m_pHead;
		m_pHead = *reinterpret_cast<TYPE**>(m_pHead);

		::operator delete(pHead);
	}
}
	


template<typename TYPE>
typename allocator_pool<TYPE>::pointer allocator_pool<TYPE>::allocate(size_type /*_Count*/)
{
	if(m_pHead ==0)
	{
#pragma push_macro("new")
#undef new
		return ((TYPE*)::operator new(sizeof(TYPE)));
#pragma pop_macro("new")
	}

	TYPE*	pHead	 = m_pHead;

	m_pHead = *reinterpret_cast<TYPE**>(m_pHead);

	// 간단하게 그냥... _Count만큼 new 한 다음에 돌려준다.
	return pHead;
}

template<typename TYPE>
void allocator_pool<TYPE>::deallocate(const pointer _Ptr, size_type) noexcept
{
	*reinterpret_cast<TYPE**>(_Ptr)	 = m_pHead;
	m_pHead			 = (TYPE*)_Ptr;
}

template<class TALLOC, class TOTHER>
bool operator==(const allocator_pool<TALLOC>&, const allocator_pool<TOTHER>&) noexcept
{
	return true;
}

template<class TALLOC, class TOTHER> 
bool operator!=(const allocator_pool<TALLOC>&, const allocator_pool<TOTHER>&) noexcept
{
	return false;
}

}