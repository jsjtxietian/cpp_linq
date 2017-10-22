#pragma once

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <memory>
#include <mutex>


namespace xt
{
	template<typename TIterator>
	using iterator_type = decltype(**(TIterator*)nullptr);




#pragma region Select&&Where
	template<typename TIterator, typename TFunction>
	class where_iterator
	{
		typedef where_iterator<TIterator, TFunction> TSelf;

	private:
		TIterator iterator;
		TIterator end;
		TFunction function;

	public:
		where_iterator(const TIterator& i, const TIterator& e, const TFunction& _f) :
			iterator(i), end(e), function(_f)
		{
			while (iterator != end && !function(*iterator))
			{
				++iterator;
			}
		}

		TSelf& operator++()
		{
			if (iterator == end) return *this;
			++iterator;
			while (iterator != end && !function(*iterator))
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


#pragma endregion 

	/*template<typename TIterator>
	class take_iterator
	{
		typedef take_iterator<TIterator> Tself;

	private:
		TIterator iterator;
		TIterator end;
		int count;
		int current;

	public:
		take_iterator(const TIterator& i, const TIterator& e, int c)
			:iterator(i),end(e),count(c),current(0)
		{
			if(current == count)
			{
				iterator = end;
			}
		}

		Tself& operator++()
		{
			if(++current == count)
			{
				iterator = end;
			}
			else
			{
				++iterator;
			}
			return *this;
		}

		iterator_type<TIterator> operator*() const
		{
			return *iterator;
		}

		bool operator==(Tself& s) const
		{
			return s.iterator == iterator;
		}

		bool operator!=(Tself& s) const
		{
			return s.iterator != iterator;
		}

	};*/


	template<typename  TIterator>
	class linq_enumerable
	{

	private:
		TIterator _begin;
		TIterator _end;

	public:
		linq_enumerable(const TIterator& begin, const TIterator& end)
			:_begin(begin), _end(end)
		{}

		TIterator begin()const { return _begin; }
		TIterator end()const { return _end; }

		template<typename TFunction>
		auto select(const TFunction& f)const->linq_enumerable<select_iterator<TIterator,TFunction>>
		{
			return linq_enumerable<select_iterator<TIterator, TFunction>>(
				select_iterator<TIterator,TFunction>(_begin,f),
				select_iterator<TIterator,TFunction>(_end,f)
			);
		}

		template<typename TFunction>
		auto where(const TFunction& f)const->linq_enumerable<where_iterator<TIterator, TFunction>>
		{
			return linq_enumerable<where_iterator<TIterator, TFunction>>(
				where_iterator<TIterator, TFunction>(_begin,_end, f),
				where_iterator<TIterator, TFunction>(_end, _end,f)
				);
		}

		int count() const
		{
			int i = 0;
			for (auto s = _begin; s != _end; ++s)
				i++;
			return i;
		}

		//auto take(int c)const->linq_enumerable<take_iterator<TIterator>>
		//{
		//	return linq_enumerable<take_iterator<TIterator>>(
		//		take_iterator<TIterator>(_begin, _end, c),
		//		take_iterator<TIterator>(_end, _end, c)
		//		);
		//}

		

	};
	

	template<typename TContainer>
	auto from(const TContainer& c)->linq_enumerable<decltype(std::begin(c))>
	{
		return linq_enumerable<decltype(std::begin(c))>(std::begin(c), std::end(c));
	}


}

