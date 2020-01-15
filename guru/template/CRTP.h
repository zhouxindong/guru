#pragma once

template <typename Derived>
class CuriousBase {};

class Curious : public CuriousBase<Curious> {};

template <T>
class CuriousTemplate : public CuriousBase<CuriousTemplate<T>> {};

// by passing the derived class down to its base class via a template parameter,
// the base class can customize its own behavior to the derived class just like virtual
// CRTP usefully to factor out implementations that can only be member functions.
// e.g., ctor, dtor, subscript operator, or are dependent on the derived class's identity.

// 1. ��������
// ��ʵ�൱�ڲ�ͬ��������ӵ�и��Բ�ͬ�Ļ���(��������޷���������������޷��������)

// 2. �򻯻�������������������������� std::rel_ops
// Barton-Nackman trick, friend funciton
// ����std::rel_ops�������������⣬���κ����Ͷ�ӵ��!=�����
// 2.1 ����ͨ��SFINAE�����˷�����Ϊֻ����Щ������==����������Ͳ�����Ч������
// 2.2 ���ͨ�õĶ�����������û�������ض�������������������ͨ��CRTP���

// EqualityComparable<>ʹ��CRTP���������ඨ��������==�ṩ��!=�����
template <typename Derived>
class EqualityComparable
{
public:
	friend bool operator != (Derived const& x1, Derived const& x2)
	{
		return !(x1 == x2);
	}
};

class X : public EqualityComparable<X>
{
public:
	friend bool operator == (X const& x1, X const& x2)
	{
		// implement logic for comparing two objects of type X
	}
};

// 3. Facade
// CRTP���ඨ���˾��������ȫ�������ӿ�(����һЩС�ĸ�������ʵ�ֵ��ⲿ�ӿ�)
// ��׼��������������Ľӿ���ʵ�ֹ����ӿ�
template <
	typename Derived,
	typename Value,
	typename Category,
	typename Reference = Value&,
	typename Distance = std::ptrdiff_t>
	class IteratorFacade
{
	// access the CRTP derived class
	Derived& asDerived() {
		return *static_cast<Derived*>(this);
	}

	Derived const& asDerived() const {
		return *static_cast<Derived const*>(this);
	}
public:
	using value_type = typename std::remove_const<Value>::type;
	using reference = Reference;
	using pointer = Value*;
	using difference_type = Distance;
	using iterator_category = Category;

	// input iterator interface
	reference operator *() const {
		return asDerived().dereference(); // in term of Derived definition
	}
	pointer operator ->() const {}
	Derived& operator ++() {
		asDerived().increment();
		return asDerived();
	}
	Derived operator ++(int) {
		Derived result(asDerived());
		asDerived().increment();
		return result;
	}
	friend bool operator == (IteratorFacade const& lhs, IteratorFacade const& rhs) {
		return lhs->asDerived().equals(rhs->asDerived());
	}

	// bidirectional iterator interface
	Derived& operator --() {}
	Derived operator --(int) {}

	// random access iterator interface
	reference operator [](difference_type n) const {}
	Derived& operator +=(difference_type n) {}

	friend difference_type operator -(IteratorFacade const& lhs, IteratorFacade const& rhs) {}
	friend bool operator <(IteratorFacade const& lhs, IteratorFacade const& rhs) {}
};

// defining a Linked_List Iterator
template <typename T>
class ListNode {
public:
	T value;
	ListNode<T>* next = nullptr;
	~ListNode() { delete next; }
};

template <typename T>
class ListNodeIterator : public IteratorFacade<ListNodeIterator<T>, T, std::forward_iterator_tag>
{
	ListNode<T>* current = nullptr;

public:
	T& dereference() const {
		return current->value;
	}

	void increment()
	{
		current = current->next;
	}

	bool equals(ListNodeIterator const& other) const {
		return current == other.current;
	}

	ListNodeIterator(ListNode<T>* current = nullptr) : current(current) {}
};