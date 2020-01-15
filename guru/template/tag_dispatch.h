#pragma once

template <typename Iterator, typename Distance>
void advanceIterImpl(Iterator& x, Distance n, std::input_iterator_tag)
{
	while (n > 0)
	{
		++x;
		--n;
	}
}

template <typename Iterator, typename Distance>
void advanceIterImpl(Iterator& x, Distance n, std::random_access_iterator_tag)
{
	x += n;
}

template <typename Iterator, typename Distance>
void advanceIter(Iterator& x, Distance n)
{
	advanceIterImpl(x, n,
		typename std::iterator_traits<Iterator>::iterator_category());
}

// partial specialization used for tag dispatch
template <typename Iterator,
	typename Tag =
	BestMatchInSet<
	typename std::iterator_traits<Iterator>::iterator_category,
	std::input_iterator_tag,
	std::bidirectional_iterator_tag,
	std::random_access_iterator_tag>>
	class Advance;

template <typename Iterator>
class Advance<Iterator, std::input_iterator_tag>
{
};

template <typename Iterator>
class Advance<Iterator, std::bidirectional_iterator_tag>
{
};

template <typename Iterator>
class Advance<Iterator, std::random_access_iterator_tag>
{
};

// enable_if also can be used for tag dispatch through differences typename default enable_if
// but compiler consider them as same
// so you must add more default typename to sperate them
template <typename Iterator,
	typename = EnableIfT<IsInputIterator<Iterator> &&
	!IsRandomAccessIterator<Iterator>>>
	Container(Iterator first, Iterator last);

template <typename Iterator,
	typename = EnableIfT<IsRandomAccessIterator<Iterator>>,
	typename = int> // extra dummy parameter to enable both
	Container(Iterator first, Iterator last);

// C++17
template <typename Iterator, typename Distance>
void advanceIter(Iterator& x, Distance n)
{
	if constexpr(IsRandomAccessIterator<Iterator>)
	{
		x += n;
	}
	else if constexpr(IsBidirectionalIterator<Iterator>)
	{
		if (n > 0)
		{
			for (; n > 0; ++x, --n) {
			}
		}
		else {
			for (; n < 0; --x, ++n) {
			}
		}
	}
	else {
		if (n < 0)
		{
			throw "advanceIter()"
		}
		while (n > 0)
		{
			++x;
			--n;
		}
	}
}