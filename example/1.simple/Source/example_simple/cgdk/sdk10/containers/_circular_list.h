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

namespace CGDK
{

template <class TDATA, class TALLOCATOR=allocator_pool<TDATA> >
class circular_list
{
// ****************************************************************************
// Type definitions for Standard)
//
//  * Common Container Requirement
//     1) X:value_type		Contain에 담긴 값들의 형.
//     2) X:reference		X:value_type&.
//     3) X:const_reference	const X:value_type&.
//     4) X:iterator		순회할 때 사용되는 반복자.
//     5) X:const_iterator	순회할 때 사용되는 반복자(const버전)
//     6) X:difference_type	두 iterator에 담김 값의 거리를 나타내는 타입.
//     7) X:size_type		X의 크기를 나타낼때 사용되는 형.
//
//  * Reversible Requirement
//     1) X::reverse_iterator 역순환 때 사용되는 반복자.
//     2) X::const_reverse_iterator	역순환 때 사용되는 반복자.(const버전)
// ----------------------------------------------------------------------------
public:
	// Standard 1) value type
	typedef TDATA			value_type;

	// Standard 2) Pointer
	typedef TDATA*			pointer;
	typedef const TDATA*	const_pointer;

	// Standard 3) Reference
	typedef TDATA&			reference;
	typedef const TDATA&	const_reference;

	// Standard 4) size of type & difference type
	typedef size_t			size_type;
	typedef ptrdiff_t		difference_type;

	// Standard 5) Iterators
	class iterator;
	class const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	// Allocator)
	typedef TALLOCATOR		allocator_type;

	// Self) 자기자신(표준 아님.)
	typedef	circular_list<value_type, TALLOCATOR>	_mytype;

	// Rebinder) 
	template<typename TOTHER_DATA, typename TOTHER_ALLOCATOR=TALLOCATOR>
	struct rebind
	{
		typedef circular_list<TOTHER_DATA, TOTHER_ALLOCATOR> other;
	};


// ****************************************************************************
// Common Container Standard)
//
//  * Common Container Requirement
//     1) X();				Default Constructor
//     2) X(a);				Copy Constrctor.(소요시간은 선형적)
//		  X u(a);
//     3) (&a)->~X();		Destructor.(소요시간은 선형적)
//     4) a.begin(); 		Container 순회에 사용할 시작과 끝 iterator 
//		  a.end();
//     5) a==b; a!=b;		Container 모든 원소 같으면 true.(소요시간 선형적)
//     6) a.size();			Container에 담김 원소의 갯수를 리턴한다.
//     7) a.max_size();		Container가 담을 수 있는 최대 원소 개수를 리턴한다.
//     8) a.empty();		Container가 비어 있는 경우라면 true를 리턴한다.
//     9) a<b; a>b; 		비교 연산(소요시간 선형적)
//		  a<=b; a>=b;
//    10) r=a;				Container 대입연산(소요시간 선형적)
//    11) a.swap(b);		동일한 타입의 두 커테이너를 서로 맞바꾼다.
//
//  * Reversible Requirement
//     1) a.rbegin();		반대방향 순회에 사용될 시작과 끝 iterator
//		  a.rend();
//
// ----------------------------------------------------------------------------
public:
	circular_list() noexcept;
	circular_list(const _mytype& _right);
	circular_list(_mytype&& _right) noexcept;
	~circular_list() noexcept;

public:
	// Common Standard 1) Status Function
	size_type				size() const							{	return m_size;}
	size_type				max_size() const						{	return std::numeric_limits<std::size_t>::max();}
	bool					empty() const							{	return m_size==0;}

	// Common Standard 2) 
	iterator				begin()									{	return iterator(m_head);}
	const_iterator			begin() const							{	return const_iterator(m_head);}
	const_iterator			cbegin() const							{	return const_iterator(m_head);}
	iterator				end()									{	return iterator(m_tail);}
	const_iterator			end() const								{	return const_iterator(m_tail);}
	const_iterator			cend() const							{	return const_iterator(m_tail);}
	reverse_iterator		rbegin()								{	return reverse_iterator(iterator(m_tail->_pre));}
	const_reverse_iterator	rbegin() const							{	return const_reverse_iterator(const_iterator(m_tail->_pre));}
	const_reverse_iterator	crbegin() const							{	return const_reverse_iterator(const_iterator(m_tail->_pre));}
	reverse_iterator		rend()									{	return reverse_iterator(iterator(m_head->_pre));}
	const_reverse_iterator	rend() const							{	return const_reverse_iterator(const_iterator(m_head->_pre));}
	const_reverse_iterator	crend() const							{	return const_reverse_iterator(const_iterator(m_head->_pre));}

	// Common Standard 3) 
	void					swap(_mytype& _right);

	// Common Standard 5) Operators
	bool					operator==(const _mytype& _right) const;
	bool					operator!=(const _mytype& _right) const;
	bool					operator<(const _mytype& _right) const;
	bool					operator>(const _mytype& _right) const;
	bool					operator<=(const _mytype& _right) const;
	bool					operator>=(const _mytype& _right) const;
	_mytype&				operator=(const _mytype& _right);


// ****************************************************************************
// Sequence Container Standard)
//
//  * Sequence Container Requirement
//    1) X(n, t);		t의 복사본 n개로 구성된 Sequence를 생성한다.
//		 X a(n, t);	
//    2) X(i, j); 		구간[i,j]의 내용과 동일한 Sequence를 생성한다.
//		 X a(i, j);
//    3) a.insert(p,t);	p앞에 t의 복사본을 삽입한다. 리턴값은 삽입된
//						원소를 가리키는 반복자이다.
//    4) a.insert(p,n,t);p앞에 t의 복사본 n개를 삽입한다.
//    5) a.insert(p,i,j);p앞에 [i,j]에 담긴 원소들의 복사본을 삽입한다.
//						i,j는 반복자
//    6) a.erase(q);	q가 가리키는 원소를 삭제한다. 리턴값은 삭제되기
//						전에 q다음에 위치했던 원소를 가리키는 반복자
//    7) a.erase(q1,q2);구간[q1,q2]에 포함된 원소들을 삭제한다. 리턴값은
//						삭제되기 전에 q2다음에 위치했던 원소를 가리키는
//						반복자.
//    8) a.clear();		컨테이너에 담긴 뭐든 원소를 삭제한다. 
//						a.erase(a.begin(), a.end())와 동일하다.
//
// ----------------------------------------------------------------------------
public:
	circular_list(size_type _count);
	circular_list(size_type _count, const_reference _val);
	circular_list(size_type _count, const_reference _val, const TALLOCATOR& _Al);
	template<class TITERATOR>
	circular_list(TITERATOR _first, TITERATOR _last);
	template<class TITERATOR>
	circular_list(TITERATOR _first, TITERATOR _last, const TALLOCATOR& _Al);

public:
	// Sequence Standard 1) insert member function
	template<class TITERATOR>
	void					insert(const_iterator _where, TITERATOR _first, TITERATOR _last);
	iterator				insert(const_iterator _where, const_reference _val);
	void					insert(const_iterator _where, int _count, const_reference _val);

	// Sequence Standard 2) erase member function
	iterator				erase(const_iterator _where);
	iterator				erase(const_iterator _first, const_iterator _last);

	// Sequence Standard 3) clear
	void					clear();

	// CGDK Native) attach/detach function
	void					attach(const_iterator _where, const_iterator _node);
	void					detach(const_iterator _where);


// ****************************************************************************
// List Member)
//
//
//
//
// ----------------------------------------------------------------------------
public:
	// 1) Capacity
	size_type				capacity() const						{	return m_capacity;}

	// 2) Data
	reference				front()									{	return m_head->_data;}
	const_reference			front() const							{	return m_head->_data;}

	reference				back()									{	return m_tail->_pre->_data;}
	const_reference			back() const							{	return m_tail->_pre->_data;}
	
	// 3) Unique
	template<typename _Pr2>
	void					unique(_Pr2 _pred);
	void					unique();

	// 4) Push & Pop Front
	void					push_front(const_reference _val);
	void					push_front(value_type&& _val);
	reference				push_front();
	void					pop_front();

	void					push_back(const_reference _val);
	void					push_back(value_type&& _val);
	reference				push_back();
	void					pop_back();

	// 5) remove
	template<typename _Pr1>
	void					remove_if(_Pr1 _pred);
	void					remove(const_reference _data);

	// 6) Reserve & Resize
	void					reserve(size_type _count);
	void					resize(size_type _new_size)				{	resize(_new_size, value_type());}
	void					resize(size_type _new_size, const value_type& _val);

	// 7) splice
	void					splice(const_iterator _where, _mytype& _right);
	void					splice(const_iterator _where, _mytype& _right, const_iterator _first);
	void					splice(const_iterator _where, _mytype& _right, const_iterator _first, const_iterator _last);

	// 8) Assign.
	template<class ITERATOR>
	void					assign(ITERATOR _first, ITERATOR _last);
	void					assign(size_type _count, const_reference _val);

	// 9) merge(두개의 circular_list를 합치는 함수.)
	template<typename _Pr3>
	void					merge(_mytype& _right, _Pr3 _pred);
	void					merge(_mytype& _right);

	// 10) sort
	template<typename _Pr3>
	void					sort(_Pr3 _pred);
	void					sort();

	// 11) shrink(List를 size에 맞게 줄이는 함수.)
	void					shrink();

	// 12) Allocation
	allocator_type			get_allocator() const					{	return m_alloc_val;}


// ****************************************************************************
// 2. List
// ----------------------------------------------------------------------------
public:
	// Node Structure)
	struct _node
	{
		_node*				_pre;
		_node*				_next;
		value_type			_data;

		_node()				{}
		_node(const value_type& p_data) : _data(p_data)	{}
	};

protected:
	// 1) Head & Tail Pointer
	_node*					m_head;					// Linked List의 Head의 바로 앞~
	_node*					m_tail;					// Linked List의 Tail의 바로 뒤~

	// 2) Size & Capacity
	size_type				m_size;					// 현재 사용중인 Node수.
	size_type				m_capacity;				// 전체 Node수.

	// 3) Allocator
	allocator_type			m_alloc_val;
	typename TALLOCATOR::template rebind<_node>::other	m_allocNode;

private:
	_node*					_make_node(const value_type& _val);
	_node*					_make_node(value_type&& _val);
	_node*					_make_node();
	void					_merge_for_sort(_node& tempList, _node& nowList);
	template<typename _Pr3>
	void					_merge_for_sort(_node& tempList, _node& nowList, _Pr3 _pred);
	void					_slice_for_sort(_node& tempList, _node*& nodeNow);




// ****************************************************************************
// Iterator Definitions.
// 
// 1. Bi-Directional Iterator Standard Requirement
//    1) Cousturctors(Xu; X(); X(a); X u(a); X u=a;)
//    2) a==b; a!=b;
//    3) r=a; *a; a->m; 
//    4) ++r; r++; *r++;
//    5) --r; r--; *r--;
// ----------------------------------------------------------------------------
public:
	class const_iterator
	{
	public:
	// ----------------------------------------------------------------
	// Types) 
		friend	circular_list<TDATA, TALLOCATOR>;

		typedef std::bidirectional_iterator_tag		iterator_category;
		typedef TDATA								value_type;
		typedef typename _mytype::difference_type	difference_type;
		typedef typename _mytype::const_pointer		pointer;
		typedef typename _mytype::const_reference	reference;

		friend			iterator;
	// ----------------------------------------------------------------
	public:
		// 1) 생성자
		const_iterator() noexcept : _Ptr(nullptr)					{}
		explicit const_iterator(_node* _now) noexcept : _Ptr(_now)	{}

		// 2) 대입
		const_iterator&		operator=(const const_iterator& _right)	{	_Ptr=_right._Ptr; return *this;}
		const_iterator&		operator=(_node* _right)				{	_Ptr=_right; return *this;}
		pointer				operator->()								{	return &**this;}

		// 3) 반복
		reference			operator*() const						{	return _Ptr->_data;}
		const_iterator&		operator++()							{	_Ptr=_Ptr->_next; return *this;}
		const_iterator		operator++(int)							{	const_iterator tIter(_Ptr); _Ptr=_Ptr->_next; return tIter;}
		const_iterator&		operator--()								{	_Ptr=_Ptr->_pre; return *this;}
		const_iterator		operator--(int)							{	const_iterator tIter(_Ptr); _Ptr=_Ptr->_pre; return tIter;}
		const_iterator		operator+(difference_type _right) const	{	const_iterator temp(_Ptr); temp+=_right; return temp;}
		void				operator+=(difference_type _right)		{	while(_right){_Ptr=_Ptr->_next;--_right;}}
		const_iterator		operator-(difference_type _right) const	{	const_iterator temp(_Ptr); temp-=_right; return temp;}
		void				operator-=(difference_type _right)		{	while(_right){_Ptr=_Ptr->_pre;--_right;}}

		// 4) 비교
		bool				operator==(const iterator& _right) const{	return _Ptr==_right._Ptr;}
		bool				operator!=(const iterator& _right) const{	return _Ptr!=_right._Ptr;}

		bool				operator==(const const_iterator& _right) const{	return _Ptr==_right._Ptr;}
		bool				operator!=(const const_iterator& _right) const{	return _Ptr!=_right._Ptr;}

		// 5) reset
		void				reset() noexcept						{ _Ptr =nullptr;}

	public:
		_node*				_Ptr;
	};


	class iterator
	{
	public:
	// ----------------------------------------------------------------
	// Types) 
		friend	circular_list<TDATA, TALLOCATOR>;

		typedef std::bidirectional_iterator_tag		iterator_category;
		typedef TDATA								value_type;
		typedef typename _mytype::difference_type	difference_type;
		typedef typename _mytype::pointer			pointer;
		typedef typename _mytype::reference			reference;

		friend			const_iterator;
	// ----------------------------------------------------------------
	public:
		// 1) 생성자
		iterator() noexcept : _Ptr(nullptr)							{}
		iterator(const iterator& _Copy) noexcept : _Ptr(_Copy._Ptr)	{}
		explicit iterator(_node* _now) noexcept : _Ptr(_now)		{}

		// 2) 대입
		iterator&			operator=(const iterator& _right)		{	_Ptr=_right._Ptr; return *this;}
		iterator&			operator=(const const_iterator& _right)	{	_Ptr=_right._Ptr; return *this;}
		void				operator=(_node* _right)				{	_Ptr=_right;}
		pointer				operator->()								{	return &**this;}
							operator const_iterator()				{	return const_iterator(_Ptr);}

		// 3) 반복
		reference			operator*() const						{	return _Ptr->_data;}
		iterator&			operator++()							{	_Ptr=_Ptr->_next; return *this;}
		iterator			operator++(int)							{	iterator tIter(_Ptr); _Ptr=_Ptr->_next; return tIter;}
		iterator&			operator--()								{	_Ptr=_Ptr->_pre; return *this;}
		iterator			operator--(int)							{	iterator tIter(_Ptr); _Ptr=_Ptr->_pre; return tIter;}
		iterator			operator+(difference_type _right) const	{	iterator temp(_Ptr); temp+=_right; return temp;}
		void				operator+=(difference_type _right)		{	while(_right){_Ptr=_Ptr->_next;--_right;}}
		iterator			operator-(difference_type _right) const	{	iterator temp(_Ptr); temp-=_right; return temp;}
		void				operator-=(difference_type _right)		{	while(_right){_Ptr=_Ptr->_pre;--_right;}}

		// 4) 비교
		bool				operator==(const iterator& _right) const{	return _Ptr==_right._Ptr;}
		bool				operator!=(const iterator& _right) const{	return _Ptr!=_right._Ptr;}

		bool				operator==(const const_iterator& _right) const{	return _Ptr==_right._Ptr;}
		bool				operator!=(const const_iterator& _right) const{	return _Ptr!=_right._Ptr;}

		// 5) reset
		void				reset() noexcept { _Ptr = nullptr; }

	public:
		_node*				_Ptr;
	};
};




//-----------------------------------------------------------------
// Constructor/Destructor 함수들.
//
// - 다양한 종류의 constructor와 destructor함수들이다.
//-----------------------------------------------------------------
template <class TDATA, class TALLOCATOR>
circular_list<TDATA, TALLOCATOR>::circular_list() noexcept :
	m_head(nullptr),
	m_tail(nullptr),
	m_size(0),
	m_capacity(0)
{
	// 1) 
	m_head	 = m_allocNode.allocate(1);
	m_tail	 = m_head;

	// 2) Head와 Tail을 연결~(head를 head로 link~)
	LINK_NODE(m_head, m_head);
}


template <class TDATA, class TALLOCATOR>
circular_list<TDATA, TALLOCATOR>::circular_list(const _mytype& _right) : circular_list()
{
	// 1) 삽입해넣는다.
	try
	{
		insert(begin(), _right.begin(), _right.end());
	}
	catch(...)
	{
		m_allocNode.deallocate(m_head, 1);

		// Throw) 다시 Thread!!!
		throw;
	}
}

template <class TDATA, class TALLOCATOR>
circular_list<TDATA, TALLOCATOR>::circular_list(_mytype&& _right) noexcept : circular_list()
{
	// 1) Head를 할당한다.
	std::swap(m_head, _right.m_head);
	std::swap(m_tail, _right.m_tail);
	std::swap(m_size, _right.m_size);
	std::swap(m_capacity, _right.m_capacity);
}

template <class TDATA, class TALLOCATOR>
typename circular_list<TDATA, TALLOCATOR>::_mytype& circular_list<TDATA, TALLOCATOR>::operator=(const _mytype& _right)
{
	// 1) 일단 모조리 다 지운다.
	clear();

	// 2) 삽입해넣는다.
	try
	{
		insert(begin(), _right.begin(), _right.end());
	}
	catch(...)
	{
		// Throw) 다시 Thread!!!
		throw;
	}

	// Return) 자기 자신 Return
	return	*this;
}


template <class TDATA, class TALLOCATOR>
circular_list<TDATA, TALLOCATOR>::circular_list(size_type _count) : circular_list()
{
	// 1) 할당받은 새로운 Node에 Construct를 수행한다.
	try
	{
		for(;_count>0;--_count)
		{
			push_back();
		}
	}
	catch(...)
	{
		m_allocNode.deallocate(m_head, 1);

		// Throw) 다시 Thread!!!
		throw;
	}
}


template <class TDATA, class TALLOCATOR>
circular_list<TDATA, TALLOCATOR>::circular_list(size_type _count, const_reference _val) : circular_list()
{
	// 1) 삽입해넣는다.
	try
	{
		insert(begin(), _count, _val);
	}
	catch(...)
	{
		m_allocNode.deallocate(m_head, 1);

		// Throw) 다시 Thread!!!
		throw;
	}
}


template <class TDATA, class TALLOCATOR>
circular_list<TDATA, TALLOCATOR>::circular_list(size_type _count, const_reference _val, const TALLOCATOR& _Al) : circular_list()
{
	// 1) 삽입해넣는다.
	try
	{
		insert(begin(), _count, _val);
	}
	catch(...)
	{
		m_allocNode.deallocate(m_head, 1);

		// Throw) 다시 Thread!!!
		throw;
	}
}


template <class TDATA, class TALLOCATOR>
template<class TITERATOR>
circular_list<TDATA, TALLOCATOR>::circular_list(TITERATOR _first, TITERATOR _last) : circular_list()
{
	// 1) 삽입해넣는다.
	try
	{
		insert(begin(), _first, _last);
	}
	catch(...)
	{
		m_allocNode.deallocate(m_head, 1);

		// Throw) 다시 Thread!!!
		throw;
	}
}


template <class TDATA, class TALLOCATOR>
template<class TITERATOR>
circular_list<TDATA, TALLOCATOR>::circular_list(TITERATOR _first, TITERATOR _last, const TALLOCATOR& _Al) : circular_list()
{
	// 1) 삽입해넣는다.
	try
	{
		insert(begin(), _first, _last);
	}
	catch(...)
	{
		m_allocNode.deallocate(m_head, 1);

		// Throw) 다시 Thread!!!
		throw;
	}
}

template <class TDATA, class TALLOCATOR>
circular_list<TDATA, TALLOCATOR>::~circular_list() noexcept
{
	//-----------------------------------------------------------------
	// 1. 먼저 clear를 해서 item마다 destory를 호출한다.
	//-----------------------------------------------------------------
	clear();

	//-----------------------------------------------------------------
	// 2. 모든 Node들을 지운다.
	//-----------------------------------------------------------------
	_node*	temp = m_head;
	do
	{
		// - Next node를 밀 저장해 놓는다.
		_node*	tempNext	 = temp->_next;

		// - Destruct를 호출하고 pnode_temp를 지운다.
		//   (Destroy는 호출하지 않는다.)
		m_allocNode.deallocate(temp, 1);

		// - 다음 Node
		temp	 = tempNext;
	}while(temp != m_head);
}


template <class TDATA, class TALLOCATOR>
typename circular_list<TDATA, TALLOCATOR>::_node* circular_list<TDATA, TALLOCATOR>::_make_node(const value_type& _val)
{
	// 1) 새로운 Node를 Allocator로부터 할당 받는다.
	_node* new_node = m_allocNode.allocate(1);

	// 2) 할당받은 새로운 Node에 Construct를 수행한다.
	try
	{
		m_alloc_val.construct(&new_node->_data, _val);
	}
	// Exception) Exception이 발생했을 경우!!!
	catch(...)
	{
		// - deallocate를 수행한다.
		m_allocNode.deallocate(new_node, 1);

		// Re-Throw를 한다.
		throw;
	}

	// Return) 새로운 Node를 Return한다.
	return	new_node;
}

template <class TDATA, class TALLOCATOR>
typename circular_list<TDATA, TALLOCATOR>::_node* circular_list<TDATA, TALLOCATOR>::_make_node(value_type&& _val)
{
	// 1) 새로운 Node를 Allocator로부터 할당 받는다.
	_node* new_node = m_allocNode.allocate(1);

	// 2) 할당받은 새로운 Node에 Construct를 수행한다.
	try
	{
		m_alloc_val.construct(&new_node->_data, std::move(_val));
	}
	// Exception) Exception이 발생했을 경우!!!
	catch(...)
	{
		// - deallocate를 수행한다.
		m_allocNode.deallocate(new_node, 1);

		// Re-Throw를 한다.
		throw;
	}

	// Return) 새로운 Node를 Return한다.
	return new_node;
}

template <class TDATA, class TALLOCATOR>
typename circular_list<TDATA, TALLOCATOR>::_node* circular_list<TDATA, TALLOCATOR>::_make_node()
{
	// 1) 새로운 Node를 Allocator로부터 할당 받는다.
	_node* new_node = m_allocNode.allocate(1);

	// 2) 할당받은 새로운 Node에 Construct를 수행한다.
	try
	{
		m_alloc_val.construct(&new_node->_data);
	}
	// Exception) Exception이 발생했을 경우!!!
	catch(...)
	{
		// - deallocate를 수행한다.
		m_allocNode.deallocate(new_node, 1);

		// Re-Throw를 한다.
		throw;
	}

	// Return) 새로운 Node를 Return한다.
	return new_node;
}

// 비교함수들
template <class TDATA, class TALLOCATOR>
bool circular_list<TDATA, TALLOCATOR>::operator==(const _mytype& _right) const
{
	// 1) 처음부터 찾기 위해 준비한다.
	iterator iter_src=begin();
	typename _mytype::iterator iter_dst = _right.begin();

	for(;;)
	{
		// check) Src가 끝인가???
		if(iter_src == end())
		{
			return (iter_dst == _right.end());
		}

		// check) Dst가 끝인가?
		if(iter_dst == end())
		{
			return false;
		}

		// 2) 만약 Src와 Dst가 같지 않으면 바로 false리턴!
		if(*iter_src!=*iter_dst)
		{
			return false;
		}

		// 3) 다음값~
		++iter_src;
		++iter_dst;
	}
}

template <class TDATA, class TALLOCATOR>
bool circular_list<TDATA, TALLOCATOR>::operator!=(const _mytype& _right) const	
{
	// 1) 처음부터 찾기 위해 준비한다.
	iterator iter_src=begin();
	typename _mytype::iterator iter_dst = _right.begin();

	for(;;)
	{
		// check) Src가 끝인가???
		if(iter_src == end())
		{
			return (iter_dst != _right.end());
		}

		// check) Dst가 끝인가?
		if(iter_dst == end())
		{
			return true;
		}

		// 2) 만약 Src와 Dst가 같지 않으면 바로 true리턴!
		if(*iter_src != *iter_dst)
		{
			return true;
		}

		// 3) 다음값~
		++iter_src;
		++iter_dst;
	}
}

template <class TDATA, class TALLOCATOR>
bool circular_list<TDATA, TALLOCATOR>::operator<(const _mytype& _right) const	
{
	// 1) 처음부터 찾기 위해 준비한다.
	iterator iter_src=begin();
	typename _mytype::iterator iter_dst = _right.begin();

	for(;;)
	{
		// check) Dst가 끝인가?
		if(iter_dst == end())
		{
			return false;
		}

		// check) Src가 끝인가???
		if(iter_src == end())
		{
			return true;
		}


		// 2) 만약 Src와 Dst가 같지 않으면 바로 true리턴!
		if(*iter_src != *iter_dst)
		{
			return (*iter_src<*iter_dst);
		}

		// 3) 다음값~
		++iter_src;
		++iter_dst;
	}
}

template <class TDATA, class TALLOCATOR>
bool circular_list<TDATA, TALLOCATOR>::operator>(const _mytype& _right) const	
{
	// 1) 처음부터 찾기 위해 준비한다.
	iterator iter_src=begin();
	typename _mytype::iterator iter_dst = _right.begin();

	for(;;)
	{
		// check) Src가 끝인가???
		if(iter_src==end())
		{
			return false;
		}

		// check) Dst가 끝인가?
		if(iter_dst==end())
		{
			return true;
		}

		// 2) 만약 Src와 Dst가 같지 않으면 바로 true리턴!
		if(*iter_src != *iter_dst)
		{
			return (*iter_src>*iter_dst);
		}

		// 3) 다음값~
		++iter_src;
		++iter_dst;
	}
}

template <class TDATA, class TALLOCATOR>
bool circular_list<TDATA, TALLOCATOR>::operator<=(const _mytype& _right) const	
{
	// 1) 처음부터 찾기 위해 준비한다.
	iterator iter_src=begin();
	typename _mytype::iterator iter_dst = _right.begin();

	for(;;)
	{
		// check) Src가 끝인가???
		if(iter_src == end())
		{
			return true;
		}

		// check) Dst가 끝인가?
		if(iter_dst == end())
		{
			return false;
		}

		// 2) 만약 Src와 Dst가 같지 않으면 바로 true리턴!
		if(*iter_src!=*iter_dst)
		{
			return (*iter_src<*iter_dst);
		}

		// 3) 다음값~
		++iter_src;
		++iter_dst;
	}
}

template <class TDATA, class TALLOCATOR>
bool circular_list<TDATA, TALLOCATOR>::operator>=(const _mytype& _right) const	
{
	// 1) 처음부터 찾기 위해 준비한다.
	iterator iter_src=begin();
	typename _mytype::iterator iter_dst = _right.begin();

	for(;;)
	{
		// check) Dst가 끝인가?
		if(iter_dst == end())
		{
			return true;
		}

		// check) Src가 끝인가???
		if(iter_src == end())
		{
			return false;
		}


		// 2) 만약 Src와 Dst가 같지 않으면 바로 true리턴!
		if(*iter_src != *iter_dst)
		{
			return (*iter_src>*iter_dst);
		}

		// 3) 다음값~
		++iter_src;
		++iter_dst;
	}
}



template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::reserve(size_type _count)
{
	//-----------------------------------------------------------------
	// 1. 추가로 확보할 Node갯수를 계산한다.
	//-----------------------------------------------------------------
	// check) 추가할 item의 갯수가 0개 이하라면 그냥 끝낸다.
	CGD_RETURN_IF(_count<=m_capacity,);

	//-----------------------------------------------------------------
	// 2. 추가할 Node수만큼 생성한다.
	//-----------------------------------------------------------------
	// 1) 새로 추가될 item의 갯수를 계산한다.
	size_type added = _count - m_capacity;

	// 1) 생성한 객체를 붙일 Node를 얻는다.
	_node* end = m_head->_pre;
	try
	{
		// 2) 추가할 item의 갯수만큼 node를 만들어 붙힌다.
		for(; added>0; --added)
		{
			// - Node를 새로 생성한다.
			_node* pnode_new = m_allocNode.allocate(1);

			// - 생성한 Node를 end에 붙힌다.
			LINK_NODE(end, pnode_new);

			// - 붙힌 Node를 새로운 end로 설정한다.
			end = pnode_new;
		}
	}
	catch(...)
	{
		// 1) Roll back할 첫번째 Node를 얻는다.
		_node*	nodeDealloc = m_head->_pre->_next;

		// 2) Loop를 돌며 생성된 모든 Node를 제거한다.
		while(nodeDealloc != end)
		{
			// - 임시로 다음 Node를 얻어 놓는다.
			_node* tempNext = nodeDealloc->_next;

			// 할당했던 pnode_new를 해제한다.
			m_allocNode.deallocate(nodeDealloc, 1);

			// - 다음 Node로~
			nodeDealloc = tempNext;
		}

		// 3) m_head의 pre의 Next를 다시 m_head로 되돌린다.
		m_head->_pre->_next = m_head;

		// Throw) 다시 Thread!!!
		throw;
	}

	//-----------------------------------------------------------------
	// 3. Head앞 쪽에다가 붙힌다.
	//-----------------------------------------------------------------
	// 1) 만든 node를 head의 앞쪽에 붙힌다.
	LINK_NODE(end, m_head);

	//-----------------------------------------------------------------
	// 4. 생성하여 추가한만큼 여분node의 수를 증가시킨다.
	//-----------------------------------------------------------------
	m_capacity = _count;
}


template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::resize(size_type _new_size, const value_type& _val)
{
	// 크기 차를 구함.
	int	differ = _new_size - m_size;

	//-----------------------------------------------------------------
	// CaseA) 크기를 늘린다.
	//-----------------------------------------------------------------
	// 만약 현재 크기가 NewSize보다 작으면 _val을 그 차이만큼 삽입한다.
	if (differ>0)
	{
		insert(end(), differ, _val);
	}
	//-----------------------------------------------------------------
	// CaseB) 크기를 줄인다.
	//-----------------------------------------------------------------
	//  만약 현재 크기가 NewSize보다 크면 뒷쪽 Item들을 제거한다.
	else
	{
		// 1) 돌면처 differ만큼 뒷쪽에서부터 제거한다.
		while (differ!=0)
		{
			// - Tail을 한칸 땡긴다.
			m_tail = m_tail->_pre;

			// - Destroy를 호출한다.
			m_alloc_val.destroy(&m_tail->_data);

			differ++;
		}

		// 2) 크기는 무조건 _NewSize되는 거다.
		m_size	 = _new_size;
	}
}

template <class TDATA, class TALLOCATOR>
template<class ITERATOR>
void circular_list<TDATA, TALLOCATOR>::assign(ITERATOR _first, ITERATOR _last)
{
	// 1) 먼저 List를 모두 지운다.
	clear();

	// 2) 삽입한다.
	for(; _first != _last; ++_first)
	{
		push_back(*_first);
	}
}

template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::assign(size_type _count, const_reference _val)
{
	// 1) 먼저 지운다.
	clear();

	// 2) _val값을 _count만큼 삽입한다.
	for (; 0 < _count; --_count)
	{
		push_back(_val);
	}
}

template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::clear()
{
	// 1) 모든 node를 할당해제한다.
	for(iterator iter = begin(); iter != end(); ++iter)
	{
		// - Destruct를 호출한다.
		m_alloc_val.destroy(&(*iter));
	}

	// 2) clear한다. (head와 next를 설정한다.)
	m_tail = m_head;

	// 3) 크기를 0으로..
	m_size = 0;
}


template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::push_front(const_reference _val)
{
	//-----------------------------------------------------------------
	// 1. 빈노드가 남아 있을 경우 (Node를 밀어서 추가한다.)
	//
	// - Circulare List의 바깥쪽에 남은 Node가 있는 경우다.
	// - 그냥 하나 밀고 거기에다가 값을 추가만 하면 된다.
	//
	// 주의) 1)항에서 pre를 먼저 얻은 다음 construct를 처리하고 3)항에서
	//       m_head에 얻어놓은 _pre를 넣은 이유는 Exception-safe처리를
	//       위해서이다. construct과정에서 exception이 발생할수 있고
	//       만약 m_head를 미리 땡겨놔버리면 exception이 발생했을 때 다시
	//       roll-back을 해줘야 한다. 그래서 construct가 끝난 이후에 
	//       m_head 값을 바꾸는 처리를 한 것이다.
	//       (또 이 처리는 code가 속도에 전혀 지장이 없다.)
	//-----------------------------------------------------------------
	if(m_size!=m_capacity)
	{
		// 1) 새로운 Head가 될 Node를 얻는다.
		_node* pnow = m_head->_pre;

		// 2) Constructor를 호출한다.(Copy constructor의 호출)
		m_alloc_val.construct(&pnow->_data, _val);

		// 3) 그냥 하나 땡긴다.
		m_head = pnow;

		// 4) 크기(size)를 1 늘인다.
		++m_size;
	}
	//-----------------------------------------------------------------
	// 2. 꽉차 있을 경우 새 노드를 생성해서 추가한다.
	//
	// - 남은 node의 수가(m_size와 allocated같으면 남은 Node가 없는 것이다.)
	//   없으면 새로 만들어 추가한다.
	//   (이것이 수행되지 않도록 최대한 reserve를 미래 해둘것을 권한다.)
	//-----------------------------------------------------------------
	else
	{
		// 1) 새로운 Node를 Allocator로부터 할당받는다.
		_node* pnode_new = _make_node(_val);

		// 2) Node를 Head에 삽입한다.
		LINK_NODE(m_head->_pre, pnode_new);
		LINK_NODE(pnode_new, m_head);

		// 3) 새로 할당받은 pNodeNode가 새로운 Head간된다.
		m_head = pnode_new;

		// 4) 할당받은 수와 크기를 1 늘린다.
		++m_size;
		++m_capacity;
	}
}

template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::push_front(value_type&& _val)
{
	//-----------------------------------------------------------------
	// push_front()함수와 동일(std::move를 사용한다는 것만 다름)
	//-----------------------------------------------------------------
	// Case-A) 빈노드가 남아 있을 경우
	if (m_size!=m_capacity)
	{
		// 2) 남아 있을 경우...
		_node* pnow = m_head->_pre;

		// 2) Constructor를 호출한다.(Copy constructor의 호출)
		m_alloc_val.construct(&pnow->_data, std::move(_val));

		// 3) 그냥 하나 땡긴다.
		m_head = pnow;

		// 4) 크기(size)를 1 늘인다.
		++m_size;
	}
	// Case-B) 꽉차 있을 경우 새 노드를 생성해서 추가한다.
	else
	{
		// - 새로운 Node를 Allocator로부터 할당받는다.
		_node* pnode_new = _make_node(std::move(_val));

		// - Node를 Head에 삽입한다.
		LINK_NODE(m_head->_pre, pnode_new);
		LINK_NODE(pnode_new, m_head);

		// - 새로 할당받은 pNodeNode가 새로운 Head간된다.
		m_head = pnode_new;

		// - 할당받은 수와 크기를 1 늘린다.
		++m_size;
		++m_capacity;
	}

}


template <class TDATA, class TALLOCATOR>
typename circular_list<TDATA, TALLOCATOR>::reference circular_list<TDATA, TALLOCATOR>::push_front()
{
	//-----------------------------------------------------------------
	// 1. 새로 생성해서 추가한다.
	//
	// - 남은 node(m_size와 allocated같으면 남은 Node가 없는 것이다.)가
	//   없으면 새로 만들어 추가한다.
	//   (이것이 수행되지 않도록 최대한 reserve를 미래 해둘것을 권한다.)
	//-----------------------------------------------------------------
	if(m_size==m_capacity)
	{
		// 1) 새로운 Node를 Allocator로부터 할당받는다.
		_node* pnode_new = _make_node();

		// 2) Node를 Head에 삽입한다.
		LINK_NODE(m_head->_pre, pnode_new);
		LINK_NODE(pnode_new, m_head);

		// 3) 새로 할당받은 pNodeNode가 새로운 Head가 된다.
		m_head = pnode_new;

		// 4) 할당받은 수와 크기를 1씩 늘린다.
		++m_size;
		++m_capacity;

		// Return) Data의 Reference를 되돌린다.
		return pnode_new->_data;
	}

	//-----------------------------------------------------------------
	// 2. Node를 밀어서 추가한다.
	//
	// - Circulare List의 바깥쪽에 남은 Node가 있는 경우다.
	// - 그냥 하나 밀고 거기에다가 값을 추가만 하면 된다.
	//
	// 주의) 1)항에서 pre를 먼저 얻은 다음 construct를 처리하고 3)항에서
	//       m_head에 얻어놓은 _pre를 넣은 이유는 Exception-safe한 처리를
	//       위해서이다. construct과정에서 exception이 발생할수 있고
	//       만약 m_head를 미리 땡겨놔버리면 exception이 발생했을 때 다시
	//       roll-back을 해줘야 한다. 그래서 construct가 끝난 이후에 
	//       m_head 값을 바꾸는 처리를 한 것이다.
	//       (또 이 처리는 code가 속도에 전혀 지장이 없다.)
	//-----------------------------------------------------------------
	// 1) 새로운 Head가 될 Node를 얻는다.
	_node* pnow = m_head->_pre;

	// 2) 위치를 얻는다.
	TDATA& tempData = pnow->_data;

	// 3) Constructor를 호출한다.
	m_alloc_val.construct(&pnow->_data);

	// 4) 그냥 하나 땡긴다.
	m_head = pnow;

	// 5) 크기(size)를 1 늘인다.
	++m_size;

	// Return) 
	return tempData;
}


template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::push_back(const_reference _val)
{
	//-----------------------------------------------------------------
	// 1. Node를 밀어서 추가한다.
	//
	// - Circulare List의 바깥쪽에 남은 Node가 있는 경우다.
	// - 그냥 하나 밀고 거기에다가 값을 추가만 하면 된다.
	//-----------------------------------------------------------------
	// 1) Constructor를 호출한다.(Copy constructor의 호출)
	m_alloc_val.construct(&m_tail->_data, _val);

	// 2-1) 여분 Node가 있을 경우
	if(m_size!=m_capacity)
	{
		// - 그냥 하나 땡긴다.
		m_tail = m_tail->_next;

		// - 크기(size)를 1 늘인다.
		++m_size;
	}
	// 2-2) 여분 Node가 없을 경우
	else
	{
		// - 새로운 Node를 Allocator로부터 할당받는다.
		_node* pnode_new = m_allocNode.allocate(1);

		// - 할당받은 새로운 Node에 값을 넣는다.
		LINK_NODE(pnode_new, m_tail->_next);
		LINK_NODE(m_tail, pnode_new);

		// - m_tail값을 변경한다.
		m_tail = pnode_new;

		// - 할당받은 수와 크기를 1 늘린다.
		++m_size;
		++m_capacity;
	}
}

template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::push_back(value_type&& _val)
{
	//-----------------------------------------------------------------
	// 위의 push_back과 동일 costruct()호출시 std::move를 사용한다는 
	// 것만 다름.
	//-----------------------------------------------------------------
	// 1) Constructor를 호출한다.(Copy constructor의 호출)
	m_alloc_val.construct(&m_tail->_data, std::move(_val));

	// 2-1) 여분 Node가 있을 경우
	if (m_size!=m_capacity)
	{
		// - 그냥 하나 땡긴다.
		m_tail = m_tail->_next;

		// - 크기(size)를 1 늘인다.
		++m_size;
	}
	// 2-2) 여분 Node가 없을 경우
	else
	{
		// 2) 새로운 Node를 Allocator로부터 할당받는다.
		_node* pnode_new = m_allocNode.allocate(1);

		// 3) 할당받은 새로운 Node에 값을 넣는다.
		LINK_NODE(pnode_new, m_tail->_next);
		LINK_NODE(m_tail, pnode_new);

		// 4) m_tail값을 변경한다.
		m_tail = pnode_new;

		// 5) 할당받은 수와 크기를 1 늘린다.
		++m_size;
		++m_capacity;
	}
}


template <class TDATA, class TALLOCATOR>
typename circular_list<TDATA, TALLOCATOR>::reference circular_list<TDATA, TALLOCATOR>::push_back()
{
	//-----------------------------------------------------------------
	// 1. Node를 밀어서 추가한다.
	//
	// - Circulare List의 바깥쪽에 남은 Node가 있는 경우다.
	// - 그냥 하나 밀고 거기에다가 값을 추가만 하면 된다.
	//-----------------------------------------------------------------
	// 1) 값(value)의 위치를 저장해놓는다.
	value_type&	tempData = m_tail->_data;

	// 2-1) 여분 Node가 있을 경우
	if(m_size!=m_capacity)
	{
		// 1) 그냥 하나 땡긴다.
		m_tail = m_tail->_next;

		// 2) 크기(size)를 1 늘인다.
		++m_size;
	}
	// 2-2) 여분 Node가 없을 경우
	else
	{
		// 1) 새로운 Node를 Allocator로부터 할당받는다.
		_node* pnode_new = m_allocNode.allocate(1);

		// 2) 할당받은 새로운 Node에 값을 넣는다.
		LINK_NODE(pnode_new, m_tail->_next);
		LINK_NODE(m_tail, pnode_new);

		// 3) m_tail값을 변경한다.
		m_tail = pnode_new;

		// 4) 할당받은 수와 크기를 1 늘린다.
		++m_size;
		++m_capacity;
	}

	// Return) 
	return	tempData;
}


template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::pop_front()
{
	// check) 비어 있는데 pop을 하면 안된다.
	#ifdef _CGD_NOCHECK_EMPTY_POP
	// * Empty를 Check를 하지 않을 경우에 Debug 모드일때 Assert를 띄워준다.
	//   (Release Mode일 때는 아무일도 동작하지 않는다.)
	CGDASSERT_ERROR(!empty());
	#else
	// Empty를 Check를 할 경우 Empty를 Check하고 비어 있을 경우 아무동작하지 않고 그냥 끝낸다.
	if(empty())
	{
		return;
	}
	#endif

	// 1) Destroy를 호출한다.
	m_alloc_val.destroy(&m_head->_data);

	// 2) Head를 한칸 땡긴다.
	m_head = m_head->_next;

	// 3) 크기(size)를 1 줄인다.
	--m_size;
}


template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::pop_back()
{
	// check) Empty검사.
	#ifdef _CGD_NOCHECK_EMPTY_POP
	// * Empty를 Check를 하지 않을 경우에 Debug 모드일때 Assert를 띄워준다.
	//   (Release Mode일 때는 아무일도 동작하지 않는다.)
	CGDASSERT_ERROR(!empty());
	#else
	// Empty를 Check를 할 경우 Empty를 Check하고 비어 있을 경우 아무동작하지 않고 그냥 끝낸다.
	CGD_RETURN_IF(empty(),);
	#endif

	// 1) Tail을 한칸 땡긴다.
	m_tail = m_tail->_pre;

	// 2) Destroy를 호출한다.
	m_alloc_val.destroy(&m_tail->_data);

	// 3) 크기(size)를 1 줄인다.
	--m_size;
}


template <class TDATA, class TALLOCATOR>
typename circular_list<TDATA, TALLOCATOR>::iterator circular_list<TDATA, TALLOCATOR>::insert(const_iterator _where, const_reference _val)
{
	//-----------------------------------------------------------------
	// 1. Head일 경우.
	//-----------------------------------------------------------------
	// - Head일 경우 push_front를 함.
	if(_where==begin())
	{
		// - push_front를 함.
		push_front(_val);

		// - begin()을 return함~
		return begin();
	}

	//-----------------------------------------------------------------
	// 2. 새로 node를 생성하고 삽입한다.
	//-----------------------------------------------------------------
	// 1) 새로운 Node를 Allocator로부터 할당받는다.
	_node* pnode_new;

	if(m_size==m_capacity)
	{
		// - 새로운 Node를 할당받는다.
		pnode_new = _make_node(_val);

		// - Capacity를 증가한다.(새로 할당받는 거니까...)
		++m_capacity;
	}
	else
	{
		// - 떼어낼 Node를 얻어낸다.
		pnode_new = m_tail->_next;

		// - 초기화한다.
		m_alloc_val.construct(&pnode_new->_data, _val);

		// - 그 Node를 제거한다. List에서 제거한다.
		LINK_NODE(m_tail, pnode_new->_next);
	}

	// 2) 삽입할 앞 Node
	_node* pnode = _where._Ptr;

	// 3) Link를 건다.
	LINK_NODE(pnode->_pre, pnode_new);
	LINK_NODE(pnode_new, pnode);

	// 4) 할당받은 갯수(allocated)와 크기(size)를 1 늘린다.
	++m_size;

	// Return) 
	return iterator(pnode_new);
}


//-----------------------------------------------------------------
// CGDK::circular_list)
//
//  insert: _val값을 circular_list에서 제거한다.
//
// 설명)
//  Iist Insert 성능 개선의 핵심은 바로 circular의 backside에 여분의
//  Node들이 이미 circular_list로 연결되어진 것이므로 이것에다가 차례로 
//  값만 설정한 후 Insert할만큼을 떼어내서 Insert를 원하는 곳(_where)에 
//  끼어넣는 다는 것이다.
//   여기서 만약 여분의 Node가 부족할 경우에만 새로 생성하여 Node를
//  연결처리를 한다.
//   처리 알고리즘 역시 그와 같이 작성되어져 있다.
//  한꺼번에 삽입하려는 node가 많으면 많을 수록 기존의 std::list의
//  insert와 많은 성능차이를 낸다.
//-----------------------------------------------------------------
template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::insert(const_iterator _where, int _count, const_reference _val)
{
	// check) _count가 0이거나 보다 작으면 바로 리턴한다.
	CGD_RETURN_IF(_count<=0,);


	//-----------------------------------------------------------------
	// 1. 돌면서 삽입한다.
	//-----------------------------------------------------------------
	// declare) 
	iterator iter_insert;
	iterator iter_end;
	int count;
	size_type capacity_inc = 0;

	try
	{
		// 1) 추가를 하기 위해 남는 Node를 얻는다.
		//    (tail의 next부터가 남은 Node이다.)
		iter_insert = ++end();	// m_tail의 _next.

		// 2) 남은 Node에 값을 Insert하기 시작한다.
		for (count=_count; (count >0) && (iter_insert!=begin()); --count, ++iter_insert)
		{
			// - Construct한다.
			m_alloc_val.construct(&(*iter_insert), _val);
		}

		// 3) 사용한 마지막 Node를 저장한다.(떼어낸 Node의 마무리를 위해...)
		//    (iter_insert의 이전 Node에 이어서 붙혀야 한다.)
		iter_end = iter_insert--;

		// 4) 추가할당 되는 Node 갯수를 저장한다.
		//    (남은 count는 추가해야할 Node수이다.)
		capacity_inc = count;

		// 5) 생성해서 나머지 Node들을 추가한다.
		for (; count >0; --count)
		{
			// - Node를 새로 생성한다.
			_node* pnode_new = _make_node(_val);

			// - Node를 추가한다.
			LINK_NODE(iter_insert._Ptr, pnode_new);

			// - iter_insert를 pnode_new로 한다.
			iter_insert = pnode_new;
		}
	}
	// Exception) Roll back한다.
	catch(...)
	{
		// 1) 전 Node로..
		iterator	iter_rollback = ++end();

		// 2) Role back하며 destroy한다.
		while(iter_rollback!=iter_insert)
		{
			// - node를 destroy한다.
			m_alloc_val.destroy(&(*iter_rollback));

			// - 다음 Node를 설정한다.
			--iter_rollback;
		}

		// 3) 추가할당했던 Node들은 모두 Deallocate한다.
		while(iter_end!=iter_insert)
		{
			// - 지우기전에 미리 Next를 얻어놓는다.
			iterator temp = iter_end++;

			// - node를 destroy한다.
			m_alloc_val.destroy(&(*temp));
		}

		// Throw) 다시 Throw!!!
		throw;
	}


	//-----------------------------------------------------------------
	// 2. Link추가하기.
	//-----------------------------------------------------------------
	// 1) 삽입할 첫번째 Node를 얻기.
	_node* pnode_start = m_tail->_next;

	// 2) _where이 Head일 경우 Head를 바꾸기.
	if(_where._Ptr == m_head)
	{
		m_head = pnode_start;
	}

	// 3) 추가할 List를 일단 떼내고 Circular List를 닫는다.
	LINK_NODE(m_tail, iter_end._Ptr);

	// 4) _where에 집어넣기
	LINK_NODE(_where._Ptr->_pre, pnode_start);
	LINK_NODE(iter_insert._Ptr, _where._Ptr);


	//-----------------------------------------------------------------
	// 3. Node갯수를 계산한다.
	//-----------------------------------------------------------------
	// 1) 할당받은 갯수(allocated)와 크기(size)를 _count만큼 늘린다.
	m_size += _count;

	// 2) capacity를 증가한다.
	m_capacity	+= capacity_inc;
}


template <class TDATA, class TALLOCATOR>
template <class TITERATOR>
void circular_list<TDATA, TALLOCATOR>::insert(const_iterator _where, TITERATOR _first, TITERATOR _last)
{
	// check) 하나도 없다면 바로 리턴...
	CGD_RETURN_IF(_first==_last, );


	//-----------------------------------------------------------------
	// 1. 돌면서 삽입한다.
	//-----------------------------------------------------------------
	// declare) 
	iterator iter_insert;
	iterator iter_end;
	int count = 0;
	size_type capacity_inc = 0;

	try
	{
		// 1) 추가를 하기 위해 남는 Node를 얻는다.
		//    (tail의 next부터가 남은 Node이다.)
		iter_insert = ++end();	// m_tail의 _next.

		// 2) 남은 Node에 값을 Insert하기 시작한다.
		for (count=0; (_first != _last) && (iter_insert!=begin()); ++_first, ++iter_insert, ++count)
		{
			// - Construct한다.
			m_alloc_val.construct(&(*iter_insert), *_first);
		}

		// 3) 사용한 마지막 Node를 저장한다.(떼어낸 Node의 마무리를 위해...)
		//    (iter_insert의 이전 Node에 이어서 붙혀야 한다.)
		iter_end = iter_insert--;

		// 4) 생성해서 나머지 Node들을 추가한다.
		for (capacity_inc=0; _first != _last; ++_first, ++capacity_inc)
		{
			// - Node를 새로 생성한다.
			_node* pnode_new = _make_node(*_first);

			// - Node를 추가한다.
			LINK_NODE(iter_insert._Ptr, pnode_new);

			// - iter_insert를 pnode_new로 한다.
			iter_insert = pnode_new;
		}
	}
	// Exception) Roll back한다.
	catch(...)
	{
		// 1) 전 Node로..
		iterator iter_rollback = ++end();

		// 2) Role back하며 destroy한다.
		while(iter_rollback!=iter_insert)
		{
			// - node를 destroy한다.
			m_alloc_val.destroy(&(*iter_rollback));

			// - 다음 Node를 설정한다.
			--iter_rollback;
		}

		// 3) 추가할당했던 Node들은 모두 Deallocate한다.
		while(iter_end != iter_insert)
		{
			// - 지우기전에 미리 Next를 얻어놓는다.
			iterator temp = iter_end++;

			// - node를 destroy한다.
			m_alloc_val.destroy(&(*temp));
		}

		// Throw) 다시 Throw!!!
		throw;
	}


	//-----------------------------------------------------------------
	// 2. Link추가하기.
	//-----------------------------------------------------------------
	// 1) 삽입할 첫번째 Node를 얻기.
	_node* pnode_start = m_tail->_next;

	// 2) _where이 Head일 경우 Head를 바꾸기.
	if(_where._Ptr == m_head)
	{
		m_head = pnode_start;
	}

	// 3) 추가할 List를 일단 떼내고 Circular List를 닫는다.
	LINK_NODE(m_tail, iter_end._Ptr);

	// 4) _where에 집어넣기
	LINK_NODE(_where._Ptr->_pre, pnode_start);
	LINK_NODE(iter_insert._Ptr, _where._Ptr);


	//-----------------------------------------------------------------
	// 3. Node갯수를 계산한다.
	//-----------------------------------------------------------------
	// 1) 할당받은 갯수(allocated)와 크기(size)를 _count만큼 늘린다.
	m_size += (count+capacity_inc);

	// 2) capacity를 증가한다.
	m_capacity	+= capacity_inc;
}



//-----------------------------------------------------------------
// CGDK::circular_list)
//
//  erase: 선택된 node들을 제거한다.
//
// 설명)
//   erase함수는 _where에 의해서 지정한 위치나 혹은 정해진 구간에
//  node를 제거하는 함수이다. 
//   erase함수는 해당하는 부분의 node들을 제거를 하지만 해당 node
//  들을 deallocate시키지는 않는다. 제거된 node들은 tail의 뒷족에
//  여분의 node로 붙혀저 추후 insert나 push 등을 할 때 재활용된다.
//  따라서 deallocate에 걸리는 시간 만큼 시간을 절약할 수 있다.
//-----------------------------------------------------------------
template <class TDATA, class TALLOCATOR>
typename circular_list<TDATA, TALLOCATOR>::iterator circular_list<TDATA, TALLOCATOR>::erase(const_iterator _where)
{
	//-----------------------------------------------------------------
	// ChecK)
	//-----------------------------------------------------------------
	// 1) _where이 tail이면 안된다.
	CGDASSERT_ERROR(_where != end());

	// 2) 이건 뭐.. 당연하다~
	CGDASSERT(_where._Ptr != 0, iterator());


	//-----------------------------------------------------------------
	// 1. Head일 경우 pop_front를 함.
	//-----------------------------------------------------------------
	// - Head일 경우 push_front를 함.
	if(_where == begin())
	{
		// - push_front를 함.
		pop_front();

		// - begin()을 return함.
		return	begin();
	}


	//-----------------------------------------------------------------
	// 2. List에서 제거
	//-----------------------------------------------------------------
	// 1) 떼낼 Node
	_node* pnode_temp = _where._Ptr;

	// 2) 기존 Node에서 떼낸다.
	_node* pnode_pre = pnode_temp->_pre;
	_node* pnode_next = pnode_temp->_next;

	// 3) Link한다.
	LINK_NODE(pnode_pre, pnode_next);


	//-----------------------------------------------------------------
	// 3. 떼낸 Node를 할당해제한다.
	//-----------------------------------------------------------------
	// 1) Destruct를 호출한다.
	m_allocNode.destroy(pnode_temp);

	// 2) 지운다.
	LINK_NODE(pnode_temp, m_tail->_next); 
	LINK_NODE(m_tail, pnode_temp);


	//-----------------------------------------------------------------
	// 4. Node갯수 처리
	//-----------------------------------------------------------------
	// 1) 크기를 1 줄인다.
	--m_size;


	// Return) Next Node의 Iterator를 되돌려준다.
	return	iterator(pnode_next);
}


template <class TDATA, class TALLOCATOR>
typename circular_list<TDATA, TALLOCATOR>::iterator circular_list<TDATA, TALLOCATOR>::erase(const_iterator _first, const_iterator _last)
{
	//-----------------------------------------------------------------
	// 1. Destroy하며 갯수 세아리기...
	//-----------------------------------------------------------------
	// 1) First Node의 PreNode를 미리 구해놓는다.
	_node* tempNodeFirst = _first._Ptr->_pre;

	// 2) 단지... 줄어드는 숫자만 확인하기 위해 돌린다.
	size_type count = 0;
	const_iterator temp = _first;
	while(temp != _last)
	{
		// check) temp가 end까지 가면 안된다.
		CGDASSERT_ERROR(temp!=end());

		// declare) 지울 Node를 저장할 변수.
		iterator tempDel;

		// - Delete할 Node를 받고 다음 Node로...
		tempDel = temp++;

		// - Destruct를 호출한다.
		m_alloc_val.destroy(&(*tempDel));

		// - 숫자를 더한다.
		++count;
	}

	// check) 만약에 갯수가 0개라면 그냥 return
	CGD_RETURN_IF(count==0, iterator(temp._Ptr));


	//-----------------------------------------------------------------
	// 2. 실제 제거하기.
	//-----------------------------------------------------------------
	// 1) erase한 만큼 크기를 줄인다.
	m_size -= count;

	// 2) 처음일 경우 Head를 땡긴다.
	if(_first==begin())
	{
		// - Head를 땡기기...
		m_head = _last._Ptr;
		
		// Return) 되돌리기..
		return	iterator(temp._Ptr);;
	}

	// 3) 끝일 경우 Tail을 땡긴다.
	if(_last==end())
	{
		// - Tail을 땡기기...
		m_tail = temp._Ptr;
		
		// Return) 되돌리기..
		return iterator(temp._Ptr);
	}

	// 4) Tail뒤에 붙이기.
	LINK_NODE(temp._Ptr->_pre, m_tail->_next); 
	LINK_NODE(m_tail, _first._Ptr);

	// 5) 떼낸 List를 붙힌다.
	LINK_NODE(tempNodeFirst, temp._Ptr);


	// Return) 
	return	iterator(temp._Ptr);
}



//-----------------------------------------------------------------
// CGDK::circular_list)
//
//  attach: _where의 _node를 node 통채로 붙인다.
//  detach : _where의 _node를 node 통채로 떼낸다.
//
// 설명)
//  insert나 erase는 값을 지우는 함수이다. 그러나 attach나 detach는
//  node 통채로 떼어내는 함수이다.
//  splice는 그 대상이 같은 circular_list라고 한다면 이 함수는 그냥 해당 node
//  만을 대상으로 한다. 즉 node자체를 떼놓았다가 node자체를 붙인다.
//-----------------------------------------------------------------
template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::attach(const_iterator _where, const_iterator _pnode)
{
	// check) _where이 tail이면 안된다.
	CGDASSERT_ERROR(_where != end());

	// check) 이건 뭐.. 당연하다~
	CGDASSERT(_where._Ptr!=0,);

	// 1) 삽입할 앞 Node
	_node* pnode = _where._Ptr;
	_node* pnode_new = _pnode._Ptr;

	// 2) Link를 건다.
	LINK_NODE(pnode->_pre, pnode_new);
	LINK_NODE(pnode_new, pnode);

	// 3) _where이 Head일 경우 m_head를 _pnode로 맞춘다.
	if(_where == begin())
	{
		m_head = pnode_new;
	}

	// 4) 할당받은 갯수(allocated)와 크기(size)를 1 늘린다.
	++m_size;
	++m_capacity;
}

template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::detach(const_iterator _where)
{
	// check) _where이 tail이면 안된다.
	CGDASSERT_ERROR(_where != end());

	// check) 이건 뭐.. 당연하다~
	CGDASSERT(_where._Ptr != 0,);

	// 1) head일 경우 한칸 민다.
	if(_where == begin())
	{
		m_head = m_head->_next;
	}

	// 2) 떼낼 Node
	_node* pnode_temp = _where._Ptr;

	// 2) 기존 Node에서 떼낸다.
	_node* pnode_pre = pnode_temp->_pre;
	_node* pnode_next = pnode_temp->_next;

	LINK_NODE(pnode_pre, pnode_next);

	// 3) 크기를 1 줄인다.
	--m_size;
	--m_capacity;
}


//-----------------------------------------------------------------
// CGDK::circular_list)
//
//  remove: _val값을 circular_list에서 제거한다.
//
// 설명)
//  일반적으로 vector같은 경우 remove를 수행하게 되면 실제로 값을
//  지우지 않지만 circular_list의 경우 remove를 한 이후에 size를 줄인다.
//  알고리즘과 성능은 std::list의 unique와 차이가 없다.
//  (단 erase자체의 처리속도가 빨라서 약간의 성능향상은 있을 수 있다.)
//-----------------------------------------------------------------
template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::remove(const_reference _val)
{
	//-----------------------------------------------------------------
	// 1. 돌리며... 해당하는 값을 제거한다.
	//-----------------------------------------------------------------
	int count = 0;
	iterator iter_end = end();
	for(iterator iter=begin();iter!=iter_end;)
	{
		if(*iter == _val)
		{
			// - iterator는 다음을 가르키게하고 현재 iterator는 받아 놓는다.
			iterator iter_remove = iter++;

			// - Destruct를 호출하고 pnode_temp를 지운다.
			m_alloc_val.destroy(&(*iter_remove));

			// - 지울 Node 수 계산.
			++count;

			// check) 처음일 경우 Head를 땡긴다.
			if(iter_remove == begin())
			{
				// - Head를 땡기기...
				m_head = m_head->_next;
				continue;
			}

			// check) 끝일 경우 Tail을 땡긴다.
			if(iter==end())
			{
				// - Tail을 땡기기...
				m_tail = m_tail->_pre;
				continue;
			}

			// - 기존 Node에서 떼낸다.
			LINK_NODE(iter_remove._Ptr->_pre, iter_remove._Ptr->_next);

			// - Node를 Deallocate한다.
			LINK_NODE(iter_remove._Ptr, m_tail->_next); 
			LINK_NODE(m_tail, iter_remove._Ptr);

			// 다음~
			continue;
		}

		// - 다음...
		++iter;
	}


	//-----------------------------------------------------------------
	// 2. 제거된 갯수만큼 줄인다.
	//-----------------------------------------------------------------
	// 지운만큼 크기를 줄인다.
	m_size -= count;
	m_capacity-= count;
}


//-----------------------------------------------------------------
//
//  remove_if: _pred함수의 결과가 TRUE인 함수를 제거한다.
//
// 설명)
//  같은지를 검사하는 부분을 _pred함수를 호출하도록 대체하여 해당
//  함수의 결과가 TRUE인 경우 해당 node를 지우는 remove함수이다.
//  특정 조건에 부함하는 node를 제거하고 싶을 때 사용하면 된다.
//  _pred함수에는 해당 node의 iterator가 제공되며 return되는 값이
//  true이면 해당 node는 제거한다.
//-----------------------------------------------------------------
template <class TDATA, class TALLOCATOR>
template<typename _Pr1>
void circular_list<TDATA, TALLOCATOR>::remove_if(_Pr1 _pred)
{
	int count = 0;
	iterator iter_end = end();
	for(iterator iter=begin();iter!=iter_end;)
	{
		if(_pred(*iter))
		{
			// - iterator는 다음을 가르키게하고 현재 iterator는 받아 놓는다.
			iterator iter_remove = iter++;

			// - Destruct를 호출하고 pnode_temp를 지운다.
			m_alloc_val.destroy(&(*iter_remove));

			// - 지울 Node 수 계산.
			++count;

			// check) 처음일 경우 Head를 땡긴다.
			if(iter_remove == begin())
			{
				// - Head를 땡기기...
				m_head = m_head->_next;
				continue;
			}

			// check) 끝일 경우 Tail을 땡긴다.
			if(iter==end())
			{
				// - Tail을 땡기기...
				m_tail = m_tail->_pre;
				continue;
			}

			// - 기존 Node에서 떼낸다.
			LINK_NODE(iter_remove._Ptr->_pre, iter_remove._Ptr->_next);

			// - Node를 Deallocate한다.
			LINK_NODE(iter_remove._Ptr, m_tail->_next); 
			LINK_NODE(m_tail, iter_remove._Ptr);

			// 다음~
			continue;
		}

		// - 다음...
		++iter;
	}

	// 지운만큼 크기를 줄인다.
	m_size -= count;
	m_capacity -= count;
}




template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::swap(_mytype& _right)
{
	// check) 같은 Container일 경우 그냥 되돌린다.
	CGD_RETURN_IF(&_right==this,);


	// CaseA) Allocator가 같으면...
	if (m_alloc_val == _right.m_alloc_val)
	{
		// 바꾼다~(모조리~)
		CGD_SWAP(m_head, _right.m_head);
		CGD_SWAP(m_tail, _right.m_tail);
		CGD_SWAP(m_size, _right.m_size);
		CGD_SWAP(m_capacity,_right.m_capacity);
	}
	// CaseB) Allocator가 다르면...
	else
	{
		iterator _where = begin();
		splice(_where, _right);
		_right.splice(_right.begin(), *this, _where, end());
	}
}


//-----------------------------------------------------------------
// CGDK::circular_list)
//
//  unique: List에서 연속적으로 같은 값을 가지는 Node를 제거한다.
//
// 설명)
//  List에서 연속적으로 같은 값을 가지는 node를 제거하는 함수로
//  sort를 한 이후에 사용해야 정확히 unique한 값들만 남길 수 있다.
//  알고리즘과 성능은 std::list의 unique와 차이가 없다.
//  (단 erase자체의 처리속도가 빨라서 약간의 성능향상은 있을 수 있다.)
//-----------------------------------------------------------------
template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::unique()
{
	//-----------------------------------------------------------------
	// 설명) Unique함수.
	//   Unique함수는 연속적인 중복값들을 제거하는 함수이다.
	//   따라서 원리는 처음부터 끝까지 iteration을 돌면서 다음과 같으
	//   면 지운다. 끝~
	//-----------------------------------------------------------------
	// check) 크기가 2보다 작으면 그냥 되돌린다.
	CGD_RETURN_IF(m_size<2,);

	// 1) 같은 것 제거하기...
	iterator _first = begin();
	iterator _after = _first;

	// 2) Iteration을 돈다.
	for (++_after; _after != end(); )
	{
		// check) _first와 _after가 같으면 _after를 지운다.
		if (_pred(*_first, *_after))
		{
			_after = erase(_after);
			continue;
		}

		// - 다음~(_after를 _first에 넣는다.)
		_first = _after++;
	}
}

template <class TDATA, class TALLOCATOR>
template<typename _Pr2>
void circular_list<TDATA, TALLOCATOR>::unique(_Pr2 _pred)
{
	// check) 크기가 2보다 작으면 그냥 되돌린다.
	CGD_RETURN_IF(m_size < 2,);

	// 1) 같은 것은 제거한다.
	iterator _first = begin();
	iterator _after = _first;

	// 2) Iteration을 돈다.
	for (++_after; _after != end();)
	{
		// check) _first와 _after가 같으면 _after를 지운다.
		if (*_first == *_after)
		{
			_after = erase(_after);
			continue;
		}

		// - 다음~(_after를 _first에 넣는다.)
		_first = _after++;
	}
}



//-----------------------------------------------------------------
// CGDK::circular_list)
//
//  splice: 정해진 node들을 통채로 옮긴다.
//
// 설명)
//   circular_list와 같은 node기반의 container가 가진 장점 중에 하나로 
//  특정 node를 move할때 해당하는 node의 연결만 딱 떼어 내서 원하는 곳으로
//  이동이 가능하다. splice는 바로 이런 동작을 하는 함수이다.
//  std::list에서는 sort등지에 많이 사용된다.
//-----------------------------------------------------------------
template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::splice(const_iterator _where, _mytype& _right)
{
	// check) 
	CGD_RETURN_IF(this == &_right || _right.empty(),);

	// 1) Right에서 떼내기.
	_node* right_first = _right.m_head;
	_node* right_last = _right.m_tail->_pre;

	// 2) List를 0로...
	_node* right_pre = right_first->_pre;
	LINK_NODE(right_pre, _right.m_tail);
	_right.m_head = _right.m_tail;

	// 3) Size와 Capasity를 계산한다.
	_right.m_capacity -= _right.m_size;
	m_size += _right.m_size; // 붙을 꺼니까~
	m_capacity += _right.m_size;

	// 4) Right의 Size를 0으로..
	_right.m_size = 0;

	// 5) 연결시킬 앞과 뒤 Node
	_node* pnode_next = _where._Ptr;
	_node* pnode_pre = pnode_next->_pre;

	// 6) right First와 pnode_pre의 연결
	LINK_NODE(pnode_pre, right_first);

	// 7) right Last와 pnode_next의 연결
	LINK_NODE(right_last, pnode_next);

	// 8) _where가 Head일 경우...
	if(_where._Ptr == m_head)
	{
		m_head = right_first;
	}
}


template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::splice(const_iterator _where, _mytype& _right, const_iterator _first)
{
	// check) 만약 _right가 복사할 것이 하나도 없으면 Return
	CGD_RETURN_IF(_first == _right.end(),);

	// check) _right가 this이면서 처음이나 끝이면 Return~
	CGD_RETURN_IF(this == &_right && (_where == _first || _where == end()), );


	// 1) 추후에 처리를 위해 떼어낼 부분을 저장해놓는다.
	_node* right_first = _first._Ptr;
	_node* right_last = _right.m_tail->_pre;

	// 2) 떼낼 객체의 갯수를 세아린다.
	int count = 0;
	for(; _first!=_right.end(); ++_first)
	{
		++count;
	}

	// 3) Right의 List에서 해당 부분을 떼어내고 그 앞뒤를연결~
	LINK_NODE(_first._pre, _right.m_tail);

	// 4) Size와 Capasity를 계산한다.
	_right.m_capacity -= count;
	_right.m_size -= count;
	m_size += count;	// 붙을 꺼니까~
	m_capacity += count;


	// 5) 연결시킬 앞과 뒤 Node
	_node* pnode_next = _where._Ptr;
	_node* pnode_pre = pnode_next->_pre;

	// 6) right First와 pnode_pre의 연결
	LINK_NODE(pnode_pre, right_first);

	// 7) right Last와 pnode_next의 연결
	LINK_NODE(right_last, pnode_next);

	// 8) _where가 Head일 경우...
	if (_where._Ptr == m_head)
	{
		m_head = right_first;
	}
}


template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::splice(const_iterator _where, _mytype& _right, const_iterator _first, const_iterator _last)
{
	// check) 
	CGD_RETURN_IF(_first == _last || (this == &_right && _where == _last),);


	// 1) 추후에 처리를 위해 떼어낼 부분을 저장해놓는다.
	_node* right_first = _first._Ptr;
	_node* right_last = _last._Ptr->_pre;

	// 2) 떼낼 객체의 갯수를 세아린다.
	size_type count = 0;

	// 3-1) 같은 Container라면 갯수변화는 없다! 따라서 셀 필요없다.
	if (this == &_right)
	{
		;
	}
	// 3-2) 통채로라도 셀 필요없다.
	else if (_first == _right.begin() && _last == _right.end())
	{
		count = _right.m_size;	
	}
	else
	{
		for(; _first!=_last; ++_first)
		{
			++count;
		}
	}


	// 4) Right의 List에서 해당 부분을 떼어내고 그 앞뒤를연결~
	LINK_NODE(_first._Ptr->_pre, _last._Ptr);

	// 5) Size와 Capasity를 계산한다.
	_right.m_capacity -= count;
	_right.m_size -= count;
	m_size += count; // 붙을 꺼니까~
	m_capacity += count;


	// 6) 연결시킬 앞과 뒤 Node
	_node* pnode_next = _where._Ptr;
	_node* pnode_pre = pnode_next->_pre;

	// 7) right First와 pnode_pre의 연결
	LINK_NODE(pnode_pre, right_first);

	// 8) right Last와 pnode_next의 연결
	LINK_NODE(right_last, pnode_next);
}




//-----------------------------------------------------------------
// CGDK::circular_list)
//
//  merge: _right list를 _pred조건에 따라 현재 list에 합친다.
//
// 설명)
//   _right list를 _pred 조건에 따라 현재 list에 합친다. 
//  두 list가 모두 sort가 되어 있어야 원하는 동적을 정확히 수행해 
//  낸다.
//  기본적인 알고리즘은 std::list와 동일하다.
//-----------------------------------------------------------------
template <class TDATA, class TALLOCATOR>
template<typename _Pr3>
void circular_list<TDATA, TALLOCATOR>::merge(_mytype& _right, _Pr3 _pred)
{
	// check) 같은 list라면 Merge를 할 필요 없다.
	CGD_RETURN_IF(&_right == this,);

	// 1) 완전포함인지 확인한다.
	if(front() >= _right.back())
	{
		// - Right를 m_head 앞에 통채로 붙이기.
		LINK_NODE(m_head->_pre, _right.m_head)
		LINK_NODE(_right.m_tail->pre, m_head)
	}
	// 2) 완전포함인지 확인한다.
	else if(back() <= _right.front())
	{
		// - _right를 m_tail 뒤에 통채로 붙이기
		LINK_NODE(m_tail->_pre, _right.m_head)
		LINK_NODE(_right.m_tail->_pre, m_tail)
	}
	// 3) 차례로 붙이기...(tempList가 Main이고 binList가 여기에 붙는 것임.)				
	else
	{
		// NodeA와 NodeB를 설정한다.
		_node*	nodeA		 = m_head;
		nodeA->_pre			 = m_head->_pre;
		_node*	nodeB		 = _right.m_head;

		for(;;)
		{
			// B를 Next로(B를 A앞에 끼워 넣는다.)
			if(_pred(nodeA->_data, nodeB->_data))
			{
				// - nodeB를 잠시 얻어 놓는다.
				_node*	tempBNext	 = nodeB->_next;

				// - nodeB를 NodeA앞에 붙인다.
				LINK_NODE(nodeA->_pre, nodeB);
				LINK_NODE(nodeB, nodeA);

				// - nodeB를 다음으로 옮긴다.
				nodeB	 = tempBNext;

				// - nodeB가 끝인지 확인한다.
				CGD_BREAK_IF(tempBNext==_right.m_tail);
			}
			// A를 Next로(그냥 넘긴다.)
			else
			{
				// nodeA를 다음으로 옮긴다.
				_node*	tempANext	 = nodeA->_next;

				// nodeA가 끝인지 확인한다.
				if(tempANext==m_tail)
				{
					// nodeA뒤에 nodeB를 붙인다.
					LINK_NODE(nodeA, nodeB);
					LINK_NODE(nodeB, m_tail);

					break;
				}

				nodeA		 = tempANext;
			}
		}
	}


	// 4) _right.m_size를 임시로 저장해 놓는다.
	size_type	tempSize	 = _right.m_size;

	// 5) List의 Size와 count를 정리한다.
	m_size	+= tempSize;
	m_size	+= tempSize;

	// 6) Right List의 Size와 count를 정리한다.
	_right.m_size	 = 0;
	_right.m_size	-= tempSize;

	// 7) Right List를 정리한다.
	LINK_NODE(_right.m_head, _right.m_head);
	_right.m_tail	 = _right.m_head;

}

template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::merge(_mytype& _right)
{
	// check) 같은 list라면 Merge를 할 필요 없다.
	CGD_RETURN_IF(&_right == this,);


	// 1) 완전포함인지 확인한다.
	if(front() >= _right.back())
	{
		// - Right를 m_head 앞에 통채로 붙이기.
		LINK_NODE(m_head->_pre, _right.m_head)
		LINK_NODE(_right.m_tail->pre, m_head)
	}
	// 2) 완전포함인지 확인한다.
	else if(back() <= _right.front())
	{
		// - _right를 m_tail 뒤에 통채로 붙이기
		LINK_NODE(m_tail->_pre, _right.m_head)
		LINK_NODE(_right.m_tail->_pre, m_tail)
	}
	// 3) 차례로 붙이기...(tempList가 Main이고 binList가 여기에 붙는 것임.)				
	else
	{
		// NodeA와 NodeB를 설정한다.
		_node*	nodeA		 = m_head;
		nodeA->_pre			 = m_head->_pre;
		_node*	nodeB		 = _right.m_head;

		for(;;)
		{
			// B를 끼워놓고 B를 Next로...
			if(nodeA->_data > nodeB->_data)
			{
				// - nodeB를 잠시 얻어 놓는다.
				_node*	tempBNext	 = nodeB->_next;

				// - nodeB를 NodeA앞에 붙인다.
				LINK_NODE(nodeA->_pre, nodeB);
				LINK_NODE(nodeB, nodeA);

				// - nodeB를 다음으로 옮긴다.
				nodeB	 = tempBNext;

				// - nodeB가 끝인지 확인한다.
				CGD_BREAK_IF(tempBNext==_right.m_tail);
			}
			// A를 Next로(그냥 넘긴다.)
			else
			{
				// nodeA를 다음으로 옮긴다.
				_node*	tempANext	 = nodeA->_next;

				// nodeA가 끝인지 확인한다.
				if(tempANext==m_tail)
				{
					// nodeA뒤에 nodeB를 붙인다.
					LINK_NODE(nodeA, nodeB);
					LINK_NODE(nodeB, m_tail);

					break;
				}

				nodeA = tempANext;
			}
		}
	}


	// 4) _right.m_size를 임시로 저장해 놓는다.
	size_type tempSize = _right.m_size;

	// 5) List의 Size와 count를 정리한다.
	m_size += tempSize;
	m_size += tempSize;

	// 6) Right List의 Size와 count를 정리한다.
	_right.m_size = 0;
	_right.m_size -= tempSize;

	// 7) Right List를 정리한다.
	LINK_NODE(_right.m_head, _right.m_head);
	_right.m_tail = _right.m_head;
}

#ifdef _CGD_LIST_FAST_SORT
template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::_slice_for_sort(_node& tempList, _node*& nodeNow)
{
	//-----------------------------------------------------------------
	// 설명)
	//  merge sort를 위해 list를 일정 단위로 끊는 역할을 하는 이 함수이다.
	//  이 함수는 4개를 정렬해서 끊어주는 함수이다.
	//  한꺼번에 4개의 node를 개개별에 따라 sort를 하여 끊어주기 때문에
	//  2개씩 끊는 함수에 비해 대략 10%~20%정도 속도향상을 가져다 준다.
	//  하지만 code가 지나치게 방대해 질수 있으므로 code의 량이 중요할
	//  경우 2개씩 끊는 함수를 사용하기를 권한다.
	//  (std::list의 경우 1개씩 끊어서 처리하도록 해서 속도상의 손해가
	//  있다.)
	//-----------------------------------------------------------------

	//-----------------------------------------------------------------
	// 1. 첫번재 데이터
	//-----------------------------------------------------------------
	// 1) 첫번째 Node를 얻는다.
	tempList._next = nodeNow;

	// 2) 다음 Node
	nodeNow	 = nodeNow->_next;

	// 3) 1개 뿐일 경우 끝낸다.
	if(nodeNow==0)
	{
		_node* temp = tempList._next;

		tempList._pre = temp;
		temp->_next = 0;

		return;
	}


	//-----------------------------------------------------------------
	// 2. 두번째 데이터
	//-----------------------------------------------------------------
	// 1) 2개째 Node를 얻는다.
	_node* pnode_second = nodeNow;

	// 2) 다음 Node
	nodeNow	 = nodeNow->_next;

	// 3) 2개를 Sort한다.
	if(tempList._next->_data > pnode_second->_data)
	{
		LINK_NODE(pnode_second, tempList._next)

		// check) 2개 뿐일 경우 끝낸다.
		if(nodeNow==0)
		{
			_node* temp = tempList._next;

			tempList._pre = temp;
			temp->_next = 0;
			tempList._next = pnode_second;

			return;
		}

		LINK_NODE(tempList._next, nodeNow)
		CGD_SWAP(pnode_second, tempList._next);
	}

	// check) 2개 뿐일 경우 끝낸다.
	if(nodeNow==0)
	{
		tempList._pre = pnode_second;
		pnode_second->_next	 = 0;
		return;
	}


	//-----------------------------------------------------------------
	// 3. 세번째 데이터
	//-----------------------------------------------------------------
	// 1) 3개째 Node를 얻는다.
	_node*	pnode_third	 = nodeNow;

	// 2) 다음 Node
	nodeNow	 = nodeNow->_next;

	// 3) 3개를 Sort한다.
	if(pnode_second->_data > pnode_third->_data)
	{
		CGD_SWAP(pnode_third, pnode_second);

		if(tempList._next->_data > pnode_second->_data)
		{
			LINK_NODE(pnode_second, tempList._next)
			LINK_NODE(tempList._next, pnode_third)

			CGD_SWAP(pnode_second, tempList._next);
		}
		else
		{
			LINK_NODE(tempList._next, pnode_second);
			LINK_NODE(pnode_second, pnode_third);
		}
	}

	// 4) 3개 뿐일 경우 끝낸다.
	if(nodeNow!=0)
	{
		LINK_NODE(pnode_third, nodeNow)
	}
	else
	{
		tempList._pre	 = pnode_third;
		pnode_third->_next = 0;
		return;
	}


	//-----------------------------------------------------------------
	// 3. 네번째 데이터
	//-----------------------------------------------------------------
	// 1) nodwNow가 4번째것이다.
	_node*	nodeForth	 = nodeNow;

	// 2) 다음 Node
	nodeNow	 = nodeNow->_next;

	// Sort 4) 4개를 Sort한다.
	if(pnode_third->_data > nodeForth->_data)
	{
		tempList._pre		 = pnode_third;
		pnode_third->_next	 = 0;

		if(pnode_second->_data > nodeForth->_data)
		{
			if(tempList._next->_data > nodeForth->_data)
			{
				LINK_NODE(nodeForth, tempList._next);
				tempList._next	 = nodeForth;
			}
			else
			{
				LINK_NODE(tempList._next, nodeForth);
				LINK_NODE(nodeForth, pnode_second);
			}
		}
		else
		{
			LINK_NODE(pnode_second, nodeForth);
			LINK_NODE(nodeForth, pnode_third);
		}
	}
	else
	{
		tempList._pre		 = nodeForth;
		nodeForth->_next	 = 0;
	}
}
#else

template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::_slice_for_sort(_node& tempList, _node*& nodeNow)
{
	//-----------------------------------------------------------------
	// 설명)
	//  merge sort를 위해 list를 일정 단위로 끊는 역할을 하는 이 함수이다.
	//  이 함수는 2개를 정렬해서 끊어주는 함수이다.
	//  위의 함수는 4개씩 끊어주는 함수여서 빠르기는 하지만 코드가 커질수
	//  있기 때문에 간단한 code를 원할 경우 이 함수를 사용하기를 권한다.
	//  속도는 10~20%정도 느리지만 code의 량은 1/3도 되지 않는다.
	//-----------------------------------------------------------------

	// 1) 두개의 Node를 읽어들인다.
	_node*	nodeFirst	 = nodeNow;
	_node*	pnode_second	 = nodeNow->_next;

	// ChecK) 1개뿐인가를 검사한다.
	if(pnode_second == 0)
	{
		tempList._next	 = nodeFirst;
		tempList._pre	 = nodeFirst;
		nodeFirst->_next = 0;

		return;
	}

	// 2) 다음 Node~
	nodeNow	 = pnode_second->_next;

	// Case A) First가 더 크면 바꾼다.
	if(nodeFirst->_data > pnode_second->_data)
	{
		LINK_NODE(pnode_second, nodeFirst);

		tempList._next	 = pnode_second;
		tempList._pre	 = nodeFirst;
		nodeFirst->_next = 0;
	}
	// Case B) Second가 더 크거나 같으면 그대로 둔다.
	else
	{
		tempList._next	 = nodeFirst;
		tempList._pre	 = pnode_second;
		pnode_second->_next = 0;
	}
}
#endif

template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::_merge_for_sort(_node& tempList, _node& nowList)
{
	//-----------------------------------------------------------------
	// 설명)
	//   merge sort를 위해 두개의 나누어진 list를 합쳐주는 함수이다.
	//  하나의 list를 기준으로 다른 하나의 list값들을 기준이 되는 list
	//  사이에 끼워넣는 형태로 code가 구성된다.
	//   기존의 lstd::list에 비해서 0비교를 하게 함으로써 잦은 비교에
	//  에 의한 속도 향상을 꽤하였고 결정적으로 한쪽 list가 다른 쪽
	//  list에 비해 완전히 크거나 작은 node인가를 검사하는 부분을 넣어
	//  만약 거의 정렬된 경우 매우 빠른 sort를 수행하도록 알고리즘이
	//  개선되었다. 또 이 부분은 정렬이 완전히 되지 않는 data라 할지라도
	//  node의 수가 적은 경우 hit되는 확률이 높아 sort의 성능을 많이
	//  개선해 준다.
	//-----------------------------------------------------------------

	// ChecK) 완전포함인지 확인한다.
	if(tempList._next->_data >= nowList._pre->_data)
	{
		// - Link~
		LINK_NODE(nowList._pre, tempList._next)

		// - Tail 설정~
		tempList._next	 = nowList._next;

		// - Merge했으면 binlist를 Reset한다.
		nowList._next	 = 0;

		// - binList Clear
		return;
	}

	// ChecK) 
	if(tempList._pre->_data <= nowList._next->_data)
	{
		// - Link~
		LINK_NODE(tempList._pre, nowList._next)

		// - Tail 설정~
		tempList._pre	 = nowList._pre;

		// - Merge했으면 binlist를 Reset한다.
		nowList._next	 = 0;

		// - binList Clear
		return;
	}

	// 3) 차례로 붙이기...(tempList가 Main이고 binList가 여기에 붙는 것임.)				
	_node*	nodeA		 = tempList._next;
	nodeA->_pre			 = &tempList;
	_node*	nodeB		 = nowList._next;
	for(;;)
	{
		// - B를 A사이에 끼워놓고 B를 Next로...
		if(nodeA->_data > nodeB->_data)
		{
			// declare) 
			_node*	nodeBPre;

			// - nodeB를 NodeA앞에 붙인다.
			LINK_NODE(nodeA->_pre, nodeB);

			// - 다음 B로...
			nodeBPre = nodeB;
			nodeB	 = nodeB->_next;

			// - B가 클때까지... 돌리기..
			HERE:

			if(nodeB==0)
			{
				// - Node를 연결한다.
				LINK_NODE(nodeBPre, nodeA);

				// - Merge했으면 binlist를 Reset한다.
				nowList._next	 = 0;

				return;
			}

			if(nodeA->_data > nodeB->_data)
			{
				// 다음~
				nodeBPre = nodeB;
				nodeB	 = nodeB->_next;

				goto HERE;
			}

			// - Node를 연결한다.
			LINK_NODE(nodeBPre, nodeA);
		}

		// - A를 다음으로...
		_node*	tempANext	 = nodeA->_next;

		// nodeA가 끝인지 확인한다.
		if(tempANext==0)
		{
			// nodeA뒤에 nodeB를 붙인다.
			LINK_NODE(nodeA, nodeB);
			tempList._pre	 = nowList._pre;

			// - Merge했으면 binlist를 Reset한다.
			nowList._next	 = 0;

			return;
		}

		nodeA		 = tempANext;
	}
}


template <class TDATA, class TALLOCATOR>
template <typename _Pr3>
void circular_list<TDATA, TALLOCATOR>::_merge_for_sort(_node& tempList, _node& nowList, _Pr3 _pred)
{
	//-----------------------------------------------------------------
	// 설명)
	//   merge sort를 위해 두개의 나누어진 list를 합쳐주는 함수이다.
	//  하나의 list를 기준으로 다른 하나의 list값들을 기준이 되는 list
	//  사이에 끼워넣는 형태로 code가 구성된다.
	//   기존의 lstd::list에 비해서 0비교를 하게 함으로써 잦은 비교에
	//  에 의한 속도 향상을 꽤하였고 결정적으로 한쪽 list가 다른 쪽
	//  list에 비해 완전히 크거나 작은 node인가를 검사하는 부분을 넣어
	//  만약 거의 정렬된 경우 매우 빠른 sort를 수행하도록 알고리즘이
	//  개선되었다. 또 이 부분은 정렬이 완전히 되지 않는 data라 할지라도
	//  node의 수가 적은 경우 hit되는 확률이 높아 sort의 성능을 많이
	//  개선해 준다.
	//-----------------------------------------------------------------

	// ChecK) 완전포함인지 확인한다.
	if(!_pred(tempList._next->_data, nowList._pre->_data))	// if(tempList._next->_data >= nowList._pre->_data)
	{
		// - Link~
		LINK_NODE(nowList._pre, tempList._next)

		// - Tail 설정~
		tempList._next	 = nowList._next;

		// - Merge했으면 binlist를 Reset한다.
		nowList._next	 = 0;

		// - binList Clear
		return;
	}

	// ChecK) 
	if(!_pred(nowList._next->_data, tempList._pre->_data))	// if(tempList._pre->_data <= nowList._next->_data)
	{
		// - Link~
		LINK_NODE(tempList._pre, nowList._next)

		// - Tail 설정~
		tempList._pre	 = nowList._pre;

		// - Merge했으면 binlist를 Reset한다.
		nowList._next	 = 0;

		// - binList Clear
		return;
	}

	// 3) 차례로 붙이기...(tempList가 Main이고 binList가 여기에 붙는 것임.)				
	_node*	nodeA		 = tempList._next;
	nodeA->_pre			 = &tempList;
	_node*	nodeB		 = nowList._next;
	for(;;)
	{
		// - B를 A사이에 끼워놓고 B를 Next로...
		if(_pred(nodeB->_data, nodeA->_data))	// if(nodeA->_data > nodeB->_data)
		{
			// declare) 
			_node*	nodeBPre;

			// - nodeB를 NodeA앞에 붙인다.
			LINK_NODE(nodeA->_pre, nodeB);

			// - 다음 B로...
			nodeBPre = nodeB;
			nodeB	 = nodeB->_next;

			// - B가 클때까지... 돌리기..
			HERE:

			if(nodeB==0)
			{
				// - Node를 연결한다.
				LINK_NODE(nodeBPre, nodeA);

				// - Merge했으면 binlist를 Reset한다.
				nowList._next	 = 0;

				return;
			}

			if(_pred(nodeB->_data, nodeA->_data))	// if(nodeA->_data > nodeB->_data)
			{
				// 다음~
				nodeBPre = nodeB;
				nodeB	 = nodeB->_next;

				goto HERE;
			}

			// - Node를 연결한다.
			LINK_NODE(nodeBPre, nodeA);
		}

		// - A를 다음으로...
		_node*	tempANext	 = nodeA->_next;

		// nodeA가 끝인지 확인한다.
		if(tempANext==0)
		{
			// nodeA뒤에 nodeB를 붙인다.
			LINK_NODE(nodeA, nodeB);
			tempList._pre	 = nowList._pre;

			// - Merge했으면 binlist를 Reset한다.
			nowList._next	 = 0;

			return;
		}

		nodeA		 = tempANext;
	}
}

//-----------------------------------------------------------------
// CGDK::circular_list)
//
//  sort: 정렬을 수행한다.
//
// 설명)
//  vector와 같은 임의접근이 가능한 경우 standard algorithm을 사용
//  하여 sort를 사용하면 효과적으로 수행된다. 그러나 list의 경우
//  매우 비효율적으로 동작하기 때문에 내부적으로 자체적인 sort를
//  가지고 있다.
// 
//  * 알고리즘 개선 내용
//   - 기본적 알고리즘은 Merge Sort를 사용하며  std::list의 sort와
//     동일하다. 하지만 세부적인 알고리즘의 구현에서 개선되었다.
//   - 최악의 경우 O(N) = NlogN를 준수한다.(std::list와 동일.)
//   - 최선의 경우 시간복잡도는 획기적으로 줄였다.
//     (최선의 경우 N/2+logN수준이다.)
//-----------------------------------------------------------------
template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::sort()
{
	//-----------------------------------------------------------------
	// check) 
	//-----------------------------------------------------------------
	// 1) 크기가 2이하일 경우 그냥 되돌린다.
	CGD_RETURN_IF(m_size < 2,);


	//-----------------------------------------------------------------
	// 1. Sort를 수행한다.
	//-----------------------------------------------------------------
	size_type		_Maxbin	 = 0;
	_node			binlist[32];

	// declare) Sliced Node.
	_node			tempList;
	//size_type		bin;

	// 1) 현재 Size
	size_type	nowSize		 = m_size;

	// 2) 제일 처음~
	_node*		nodeHeadPre	 = m_head->_pre;
	_node*		nodeNow		 = m_head;
	size_type	LastBin;

	if(nowSize>=4)
	{
		for(;;)	// 4개씩 merge한다.
		{
			//-------------------------------------
			// Step1. 4개씩 잘라낸다.
			//-------------------------------------
			// 1) Now Node
			tempList._next	 = nodeNow;	nodeNow=nodeNow->_next;

			// 2) First와 Second를 비교한다.
			_node*	pSecond	 = nodeNow;	nodeNow=nodeNow->_next;
			if(tempList._next->_data>pSecond->_data)
			{
				LINK_NODE(pSecond, tempList._next);
				LINK_NODE(tempList._next, nodeNow);
				CGD_SWAP(tempList._next, pSecond);
			}

			// 3) Second와 Third를 비교한다.
			_node*	pThird	 = nodeNow;	nodeNow=nodeNow->_next;
			if(pSecond->_data>pThird->_data)
			{
				// - Third<First<Second
				if(tempList._next->_data>=pThird->_data)
				{
					LINK_NODE(pThird, tempList._next);

					_node*	temp	 = pSecond;
					pSecond			 = tempList._next;
					tempList._next	 = pThird;
					pThird			 = temp;
				}
				// - First<Third<Second
				else
				{
					LINK_NODE(tempList._next, pThird);
					LINK_NODE(pThird, pSecond);
					CGD_SWAP(pSecond, pThird);
				}

				LINK_NODE(pThird, nodeNow);
			}

			// 4) Third와 Forth를 비교한다.
			tempList._pre	 = nodeNow;	nodeNow=nodeNow->_next;
			if(pThird->_data>tempList._pre->_data)
			{
				// - First<Second<=Forth<Third
				if(pSecond->_data<=tempList._pre->_data)
				{
					LINK_NODE(pSecond, tempList._pre);
					LINK_NODE(tempList._pre, pThird);
				}
				// - First<Forth<Second<Third
				else if(tempList._next->_data<tempList._pre->_data)
				{
					LINK_NODE(tempList._next, tempList._pre);
					LINK_NODE(tempList._pre, pSecond);
				}
				// - Forth<=First<Second<Third
				else
				{
					LINK_NODE(tempList._pre, tempList._next);
					tempList._next	 = tempList._pre;
				}

				tempList._pre	 = pThird;
			}

			tempList._pre->_next = 0;


			//-------------------------------------
			// Step2. Merge를 한다.
			//-------------------------------------
			size_type bin=0;
			for(; binlist[bin]._next!=0; ++bin)
			{
				// ChecK) MaxBin값과 같으면 Loop를 탈출한다.(이때 Max Bin값을 증가시킨다.)
				if(bin==_Maxbin)
				{
					_Maxbin	 = bin+1;
					break;
				}

				// 1) tempList와 Merge~
				_merge_for_sort(tempList, binlist[bin]);
			}

			// 1) 
			binlist[bin]	 = tempList;

			//-------------------------------------
			// Step3. Merge결과를 binList에 저장한다.
			//-------------------------------------
			// 1) Size를 줄인다.
			nowSize		-=4;

			// check) 끝인지 아닌지 확인
			if(nowSize<4)
			{
				LastBin	 = bin+1;
				break;
			}
		}
	}
	else
	{
	}

	// 3개 이해 Merge
	if(nowSize!=0)
	{
		//-------------------------------------
		// Step1. Node를 만든다.
		//-------------------------------------
		for(;;)
		{
			// 1) 1개 뿐일 경우...
			tempList._next	 = nodeNow;

			if(nowSize==1)
			{
				tempList._pre	 = nodeNow;
				break;
			}

			// 2) 2개일 경우...
			nodeNow	 = nodeNow->_next; tempList._pre = nodeNow->_next;
			if(tempList._next->_data>nodeNow->_data)
			{
				LINK_NODE(nodeNow, tempList._next);
				LINK_NODE(tempList._next, tempList._pre);
				CGD_SWAP(tempList._next, nodeNow);
			}

			if(nowSize==2)
			{
				tempList._pre	 = nodeNow;
				break;
			}

			// 3) 3개일 경우...
			if(nodeNow->_data>tempList._pre->_data)
			{
				// third<first<second
				if(tempList._next->_data>tempList._pre->_data)
				{
					LINK_NODE(tempList._pre, tempList._next);
					tempList._next	 = tempList._pre;
				}
				// first<third<second
				else
				{
					LINK_NODE(tempList._next, tempList._pre);
					LINK_NODE(tempList._pre, nodeNow);
				}

				tempList._pre	 = nodeNow;
			}
			break;
		}
		tempList._pre->_next = 0;

		//-------------------------------------
		// Step2. Merge를 한다.
		//-------------------------------------
		size_type bin=0;
		for(; binlist[bin]._next!=0; ++bin)
		{
			// ChecK) MaxBin값과 같으면 Loop를 탈출한다.(이때 Max Bin값을 증가시킨다.)
			if(bin==_Maxbin)
			{
				_Maxbin	 = bin+1;
				break;
			}

			// 1) tempList와 Merge~
			_merge_for_sort(tempList, binlist[bin]);
		}

		LastBin	 = bin+1;
	}

	//-------------------------------------
	// Step4. 모조리 Merge해서 합한다.
	//-------------------------------------
	// 2) 합치기~
	for(; LastBin<_Maxbin; ++LastBin)
	{
		// 1) Check binlist가 비어 있으면 다음~
		CGD_CONTINUE_IF(binlist[LastBin]._next==0);

		// 2) tempList와 Merge~
		_merge_for_sort(tempList, binlist[LastBin]);
	}

	//-------------------------------------
	// Step5. 마지막으로 List에 연결하기.
	//-------------------------------------
	// 1) Sort완료된 List Link를 건다.
	LINK_NODE(nodeHeadPre, tempList._next);
	LINK_NODE(tempList._pre, m_tail);

	// 2) Head를 새로 설정한다.
	m_head	 = tempList._next;
}


template <class TDATA, class TALLOCATOR>
template<typename _Pr3>
void circular_list<TDATA, TALLOCATOR>::sort(_Pr3 _pred)
{
	//-----------------------------------------------------------------
	// check) 
	//-----------------------------------------------------------------
	// 1) 크기가 2이하일 경우 그냥 되돌린다.
	CGD_RETURN_IF(m_size < 2,);


	//-----------------------------------------------------------------
	// 1. Sort를 수행한다.
	//-----------------------------------------------------------------
	size_type		_Maxbin	 = 0;
	_node			binlist[32];

	// declare) Sliced Node.
	_node			tempList;
	//size_type		bin;

	// 1) 현재 Size
	size_type	nowSize		 = m_size;

	// 2) 제일 처음~
	_node*		nodeHeadPre	 = m_head->_pre;
	_node*		nodeNow		 = m_head;
	size_type	LastBin;

	if(nowSize>=4)
	{
		for(;;)	// 4개씩 merge한다.
		{
			//-------------------------------------
			// Step1. 4개씩 잘라낸다.
			//-------------------------------------
			// 1) Now Node
			tempList._next	 = nodeNow;	nodeNow=nodeNow->_next;

			// 2) First와 Second를 비교한다.
			_node*	pSecond	 = nodeNow;	nodeNow=nodeNow->_next;
			if(_pred(pSecond->_data, tempList._next->_data)) //if(tempList._next->_data>pSecond->_data)
			{
				LINK_NODE(pSecond, tempList._next);
				LINK_NODE(tempList._next, nodeNow);
				CGD_SWAP(tempList._next, pSecond);
			}

			// 3) Second와 Third를 비교한다.
			_node*	pThird	 = nodeNow;	nodeNow=nodeNow->_next;
			if(_pred(pThird->_data, pSecond->_data)) // if(pSecond->_data>pThird->_data)
			{
				// - Third<First<Second
				if(!_pred(tempList._next->_data, pThird->_data))	// if(tempList._next->_data>=pThird->_data)
				{
					LINK_NODE(pThird, tempList._next);

					_node*	temp	 = pSecond;
					pSecond			 = tempList._next;
					tempList._next	 = pThird;
					pThird			 = temp;
				}
				// - First<Third<Second
				else
				{
					LINK_NODE(tempList._next, pThird);
					LINK_NODE(pThird, pSecond);
					CGD_SWAP(pSecond, pThird);
				}

				LINK_NODE(pThird, nodeNow);
			}

			// 4) Third와 Forth를 비교한다.
			tempList._pre	 = nodeNow;	nodeNow=nodeNow->_next;
			if(_pred(tempList._pre->_data, pThird->_data))	// if(pThird->_data>tempList._pre->_data)
			{
				// - First<Second<=Forth<Third
				if(!_pred(pSecond->_data, tempList._pre->_data))	// if(pSecond->_data<=tempList._pre->_data)
				{
					LINK_NODE(pSecond, tempList._pre);
					LINK_NODE(tempList._pre, pThird);
				}
				// - First<Forth<Second<Third
				else if(_pred(tempList._next->_data, tempList._pre->_data))	// else if(tempList._next->_data<tempList._pre->_data)
				{
					LINK_NODE(tempList._next, tempList._pre);
					LINK_NODE(tempList._pre, pSecond);
				}
				// - Forth<=First<Second<Third
				else
				{
					LINK_NODE(tempList._pre, tempList._next);
					tempList._next	 = tempList._pre;
				}

				tempList._pre	 = pThird;
			}

			tempList._pre->_next = 0;


			//-------------------------------------
			// Step2. Merge를 한다.
			//-------------------------------------
			size_type bin=0;
			for(; binlist[bin]._next!=0; ++bin)
			{
				// ChecK) MaxBin값과 같으면 Loop를 탈출한다.(이때 Max Bin값을 증가시킨다.)
				if(bin==_Maxbin)
				{
					_Maxbin	 = bin+1;
					break;
				}

				// 1) tempList와 Merge~
				_merge_for_sort(tempList, binlist[bin], _pred);
			}

			// 1) 
			binlist[bin]	 = tempList;

			//-------------------------------------
			// Step3. Merge결과를 binList에 저장한다.
			//-------------------------------------
			// 1) Size를 줄인다.
			nowSize		-=4;

			// check) 끝인지 아닌지 확인
			if(nowSize<4)
			{
				LastBin	 = bin+1;
				break;
			}
		}
	}
	else
	{
	}

	// 3개 이해 Merge
	if(nowSize!=0)
	{
		//-------------------------------------
		// Step1. Node를 만든다.
		//-------------------------------------
		for(;;)
		{
			// 1) 1개 뿐일 경우...
			tempList._next	 = nodeNow;

			if(nowSize==1)
			{
				tempList._pre	 = nodeNow;
				break;
			}

			// 2) 2개일 경우...
			nodeNow	 = nodeNow->_next; tempList._pre = nodeNow->_next;
			if(_pred(nodeNow->_data, tempList._next->_data))	// if(tempList._next->_data>nodeNow->_data)
			{
				LINK_NODE(nodeNow, tempList._next);
				LINK_NODE(tempList._next, tempList._pre);
				CGD_SWAP(tempList._next, nodeNow);
			}

			if(nowSize==2)
			{
				tempList._pre	 = nodeNow;
				break;
			}

			// 3) 3개일 경우...
			if(_pred(tempList._pre->_data, nodeNow->_data))	// if(nodeNow->_data>tempList._pre->_data)
			{
				// third<first<second
				if(_pred(tempList._pre->_data, tempList._next->_data))	// if(tempList._next->_data>tempList._pre->_data)
				{
					LINK_NODE(tempList._pre, tempList._next);
					tempList._next	 = tempList._pre;
				}
				// first<third<second
				else
				{
					LINK_NODE(tempList._next, tempList._pre);
					LINK_NODE(tempList._pre, nodeNow);
				}

				tempList._pre	 = nodeNow;
			}
			break;
		}
		tempList._pre->_next = 0;


		//-------------------------------------
		// Step2. Merge를 한다.
		//-------------------------------------
		size_type bin=0;
		for(; binlist[bin]._next!=0; ++bin)
		{
			// ChecK) MaxBin값과 같으면 Loop를 탈출한다.(이때 Max Bin값을 증가시킨다.)
			if(bin==_Maxbin)
			{
				_Maxbin	 = bin+1;
				break;
			}

			// 1) tempList와 Merge~
			_merge_for_sort(tempList, binlist[bin], _pred);
		}

		LastBin	 = bin+1;
	}

	//-------------------------------------
	// Step4. 모조리 Merge해서 합한다.
	//-------------------------------------
	for(; LastBin<_Maxbin; ++LastBin)
	{
		// 1) Check binlist가 비어 있으면 다음~
		CGD_CONTINUE_IF(binlist[LastBin]._next==0);

		// 2) tempList와 Merge~
		_merge_for_sort(tempList, binlist[LastBin], _pred);
	}

	//-------------------------------------
	// Step5. 마지막으로 List에 연결하기.
	//-------------------------------------
	// 1) Sort완료된 List Link를 건다.
	LINK_NODE(nodeHeadPre, tempList._next);
	LINK_NODE(tempList._pre, m_tail);

	// 2) Head를 새로 설정한다.
	m_head	 = tempList._next;
}




//-----------------------------------------------------------------
// CGDK::circular_list)
//
//  shrink: 여분의 node를 제거한다.
//
// 설명)
//  CGDK::circular_list의 경우 pop이나 clear, erase 혹은 remove를 수행할 때  
//  실제 node를 제거하지 않고 옮기기만 한다. 따라서 한번 생성된 node는 
//  container를 파괴하기 전에는 지워지지 않는다.
//   shrink는 여분의 node를 제거하는 함수이다. 여분의 node를 제거할
//  필요가 있으면 이 함수를 사용하면 size만큼의 node를 제외하고는
//  모두 제거한다.
//-----------------------------------------------------------------
template <class TDATA, class TALLOCATOR>
void circular_list<TDATA, TALLOCATOR>::shrink()
{
	//-----------------------------------------------------------------
	// 1. 돌면서 남은 node들을 모두 제거한다.
	//-----------------------------------------------------------------
	// 1) 단지... 줄어드는 숫자만 확인하기 위해 돌린다.
	size_type	count	 = 0;

	// 2) 지울 head와 tail을 설정.
	_node*	tempFirst	 = m_tail->_next;
	_node*	tempLast	 = m_head;
	while(tempFirst != tempLast)
	{
		// - Destruct를 호출한다.
		m_alloc_val.destroy(&tempFirst->_data);

		// - Destruct를 호출하고 pnode_temp를 지운다.
		//   (Destroy는 호출하지 않는다.)
		m_allocNode.deallocate(tempFirst, 1);

		// - 숫자를 더한다.
		++count;

		// - 다음...
		tempFirst	= tempFirst->_next;
	}


	//-----------------------------------------------------------------
	// 2. Node를 정리한다.
	//-----------------------------------------------------------------
	// 1) m_head와 m_tail을 뒤로 붙이기.
	LINK_NODE(m_tail, m_head);

	// 2) 할당받은 수와 크기를 1 줄인다.
	m_size		-= count;
}


}
