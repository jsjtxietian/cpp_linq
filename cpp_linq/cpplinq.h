#pragma once

#include <iostream>
#include <vector>

using namespace std;

template<typename TIterator, typename TFunction>
class select_iterator
{
	typedef select_iterator<TIterator, TFunction> TSelf;

private:
	TIterator iterator;
	TFunction f;

public:
	select_iterator(const TIterator& i, const TFunction& _f) :
		iterator(i), f(_f)
	{}

	TSelf& operator++()
	{
		++iterator;
		return *this;
	}

	auto operator*()const->decltype(f(*iterator))
	{
		return f(*iterator);
	}

	bool operator==(const TSelf& it)const
	{
		return it.iterator == iterator;
	}

	bool operator!=(const TSelf& it)const
	{
		return it.iterator != iterator;
	}
};

template<typename TIterator>
using iterator_type = decltype(**(TIterator*)nullptr);

template<typename TIterator, typename TFunction>
class where_iterator
{
	typedef where_iterator<TIterator, TFunction> TSelf;

private:
	TIterator iterator;
	TIterator end;
	TFunction f;

public:
	where_iterator(const TIterator& i, const TIterator& e, const TFunction& _f) :
		iterator(i), end(e), f(_f)
	{
		while (iterator != end && !f(*iterator))
		{
			++iterator;
		}
	}

	TSelf& operator++()
	{
		if (iterator == end) return *this;
		++iterator;
		while (iterator != end && !f(*iterator))
		{
			++iterator;
		}
		return *this;
	}


	iterator_type<TIterator> operator*()const
	{
		return *iterator;
	}

	bool operator==(const TSelf& it)const
	{
		return it.iterator == iterator;
	}

	bool operator!=(const TSelf& it)const
	{
		return iterator != it.iterator;
	}
};

template<typename TIterator>
class linq_enumerable
{
private:
	TIterator _begin;
	TIterator _end;

public:
	linq_enumerable(const TIterator& b, const TIterator& e) :
		_begin(b), _end(e)
	{}

	TIterator begin()const
	{
		return _begin;
	}

	TIterator end()const
	{
		return _end;
	}

	template<typename TFunction>
	auto select(const TFunction& f)const->linq_enumerable<select_iterator<TIterator, TFunction>>
	{
		return linq_enumerable<select_iterator<TIterator, TFunction>>(
			select_iterator<TIterator, TFunction>(_begin, f),
			select_iterator<TIterator, TFunction>(_end, f)
			);
	}

	template<typename TFunction>
	auto where(const TFunction& f)const->linq_enumerable<where_iterator<TIterator, TFunction>>
	{
		return linq_enumerable<where_iterator<TIterator, TFunction>>(
			where_iterator<TIterator, TFunction>(_begin, _end, f),
			where_iterator<TIterator, TFunction>(_end, _end, f)
			);
	}

};


template<typename TContainer>
auto from(const TContainer& c)->linq_enumerable<decltype(std::begin(c))>
{
	return linq_enumerable<decltype(std::begin(c))>(std::begin(c), std::end(c));
}



