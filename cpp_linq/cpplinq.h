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

	template<typename T>
	class linq;

	class linq_exception
	{
	public:
		std::string message;

		linq_exception(const std::string& m) :message(m)
		{}
	};


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

#pragma region Take&&Skip
	template<typename TIterator>
	class take_iterator
	{
		typedef take_iterator<TIterator>							TSelf;
	private:
		TIterator			iterator;
		TIterator			end;
		int					count;
		int					current;

	public:
		take_iterator(const TIterator& _iterator, const TIterator& _end, int _count)
			:iterator(_iterator), end(_end), count(_count), current(0)
		{
			if (current == count)
			{
				iterator = end;
			}
		}

		TSelf& operator++()
		{
			if (++current == count)
			{
				iterator = end;
			}
			else
			{
				iterator++;
			}
			return *this;
		}

		TSelf operator++(int)
		{
			TSelf t = *this;
			if (++current == count)
			{
				iterator = end;
			}
			else
			{
				iterator++;
			}
			return t;
		}

		iterator_type<TIterator> operator*()const
		{
			return *iterator;
		}

		bool operator==(const TSelf& it)const
		{
			return iterator == it.iterator;
		}

		bool operator!=(const TSelf& it)const
		{
			return iterator != it.iterator;
		}
	};

	template<typename TIterator>
	class skip_iterator
	{
		typedef skip_iterator<TIterator> TSelf;

	private:
		TIterator iterator;
		TIterator end;
		int count;

	public:
		skip_iterator(const TIterator& i, const TIterator& e, int c) :
			iterator(i), end(e), count(c)
		{
			while (iterator != end && count-- > 0)
			{
				++iterator;
			}
		}

		iterator_type<TIterator> operator*()const
		{
			return *iterator;
		}

		TSelf& operator++()
		{
			++iterator;
			return *this;
		}

		bool operator==(const TSelf& it)const
		{
			return iterator == it.iterator;
		}

		bool operator!=(const TSelf& it)const
		{
			return iterator != it.iterator;
		}
	};

	template<typename TIterator, typename TFunction>
	class take_while_iterator
	{
		typedef take_while_iterator<TIterator, TFunction> TSelf;

	private:
		TIterator iterator;
		TIterator end;
		TFunction function;

	public:
		take_while_iterator(const TIterator& i, const TIterator& e, const TFunction& _f) :
			iterator(i), end(e), function(_f)
		{
			if (iterator != end && !function(*iterator))
			{
				iterator = end;
			}
		}

		TSelf& operator++()
		{
			++iterator;
			if (!function(*iterator))
			{
				iterator = end;
			}
			return *this;
		}

		iterator_type<TIterator> operator*()const
		{
			return *iterator;
		}

		bool operator==(const TSelf& it)const
		{
			return iterator == it.iterator;
		}

		bool operator!=(const TSelf& it)const
		{
			return iterator != it.iterator;
		}
	};

	template<typename TIterator, typename TFunction>
	class skip_while_iterator
	{
		typedef skip_while_iterator<TIterator, TFunction> TSelf;

	private:
		TIterator iterator;
		TIterator end;
		TFunction function;

	public:
		skip_while_iterator(const TIterator& i, const TIterator& e, TFunction _f) :
			iterator(i), end(e), function(_f)
		{
			while (iterator != end && function(*iterator))
			{
				++iterator;
			}
		}

		iterator_type<TIterator> operator*()const
		{
			return *iterator;
		}

		TSelf& operator++()
		{
			++iterator;
			return *this;
		}

		bool operator==(const TSelf& it)const
		{
			return iterator == it.iterator;
		}

		bool operator!=(const TSelf& it)const
		{
			return iterator != it.iterator;
		}
	};
#pragma endregion 

	template<typename TIterator1, typename TIterator2>
	class concat_iterator
	{
		typedef concat_iterator<TIterator1, TIterator2> TSelf;

	private:
		TIterator1 iterator1;
		TIterator1 end1;
		TIterator2 iterator2;
		bool first;

	public:
		concat_iterator(const TIterator1& it1, const TIterator1& e1, const TIterator2& it2) :
			iterator1(it1), end1(e1), iterator2(it2), first(it1 != e1)
		{}

		TSelf& operator++()
		{
			if (first)
			{
				if (++iterator1 == end1)
				{
					first = false;
				}
			}
			else
			{
				++iterator2;
			}
			return *this;
		}

		iterator_type<TIterator1> operator*()const
		{
			return first ? *iterator1 : *iterator2;
		}

		bool operator==(const TSelf& it)const
		{
			if (first != it.first)
			{
				return false;
			}
			return first ? iterator1 == it.iterator1 : iterator2 == it.iterator2;
		}

		bool operator!=(const TSelf& it)const
		{
			if (first != it.first)
			{
				return true;
			}
			return first ? iterator1 != it.iterator1 : iterator2 != it.iterator2;
		}
	};


	template<typename  TIterator>
	class linq_enumerable
	{
		typedef typename std::remove_cv<typename std::remove_reference<iterator_type<TIterator>>::type>::type TElement;

	private:
		TIterator _begin;
		TIterator _end;

	public:
		linq_enumerable(const TIterator& begin, const TIterator& end)
			:_begin(begin), _end(end)
		{}

		TIterator begin()const { return _begin; }
		TIterator end()const { return _end; }

#pragma region select&&where&&take&&skip&&concat
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

		auto take(int c)const->linq_enumerable<take_iterator<TIterator>>
		{
			return linq_enumerable<take_iterator<TIterator>>(
				take_iterator<TIterator>(_begin, _end, c),
				take_iterator<TIterator>(_end, _end, c)
				);
		}

		auto skip(int c)const->linq_enumerable<skip_iterator<TIterator>>
		{
			return linq_enumerable<skip_iterator<TIterator>>(
				skip_iterator<TIterator>(_begin, _end, c),
				skip_iterator<TIterator>(_end, _end, c)
				);
		}

		template<typename TFunction>
		auto take_while(const TFunction& f)const->linq_enumerable<take_while_iterator<TIterator, TFunction>>
		{
			return linq_enumerable<take_while_iterator<TIterator, TFunction>>(
				take_while_iterator<TIterator, TFunction>(_begin, _end, f),
				take_while_iterator<TIterator, TFunction>(_end, _end, f)
				);
		}

		template<typename TFunction>
		auto skip_while(const TFunction& f)const->linq_enumerable<skip_while_iterator<TIterator, TFunction>>
		{
			return linq_enumerable<skip_while_iterator<TIterator, TFunction>>(
				skip_while_iterator<TIterator, TFunction>(_begin, _end, f),
				skip_while_iterator<TIterator, TFunction>(_end, _end, f)
				);
		}

		template<typename TIterator2>
		auto concat(const linq_enumerable<TIterator2>& e)const->linq_enumerable<concat_iterator<TIterator, TIterator2>>
		{
			return linq_enumerable<concat_iterator<TIterator, TIterator2>>(
				concat_iterator<TIterator, TIterator2>(_begin, _end, e.begin()),
				concat_iterator<TIterator, TIterator2>(_end, _end, e.end())
				);
		}

#pragma endregion

#pragma region instantly
		std::vector<TElement> to_vector()const
		{
			std::vector<TElement> v;
			for (auto it = _begin; it != _end; ++it)
			{
				v.push_back(*it);
			}
			return std::move(v);
		}

		std::list<TElement> to_list()const
		{
			std::list<TElement> l;
			for (auto it = _begin; it != _end; ++it)
			{
				l.push_back(*it);
			}
			return std::move(l);
		}

		std::set<TElement> to_set()const
		{
			std::set<TElement> s;
			for (auto it = _begin; it != _end; ++it)
			{
				s.insert(*it);
			}
			return std::move(s);
		}

		template<typename TFunction>
		auto to_map(const TFunction& f)const->std::map<decltype(f(*(TElement*)nullptr)), TElement>
		{
			std::map<decltype(f(*(TElement*)nullptr)), TElement> m;
			for (auto it = _begin; it != _end; ++it)
			{
				m.insert(std::make_pair(f(*it), *it));
			}
			return std::move(m);
		}

		bool empty()const
		{
			return _begin == _end;
		}

		TElement first()const
		{
			if (empty())
			{
				throw linq_exception("Failed to get a value from an empty collection");
			}
			return *_begin;
		}

		TElement first_or_default(const TElement& value)const
		{
			if (empty())
			{
				return value;
			}
			return *_begin;
		}

		TElement last()const
		{
			if (empty())
			{
				throw linq_exception("Failed to get a value from an empty collection");
			}
			TElement result;
			for (auto it = _begin; it != _end; ++it)
			{
				result = *it;
			}
			return result;
		}

		TElement last_or_default(const TElement& value)const
		{
			auto result = value;
			for (auto it = _begin; it != _end; ++it)
			{
				result = *it;
			}
			return result;
		}

		TElement element_at(int index)const
		{
			if (index >= 0)
			{
				int counter = 0;
				for (auto it = _begin; it != _end; ++it)
				{
					if (counter == index) return *it;
					++counter;
				}
			}
			throw linq_exception("Argument out of range: index");
		}

		int count()const
		{
			int counter = 0;
			for (auto it = _begin; it != _end; ++it)
			{
				++counter;
			}
			return counter;
		}

		template<typename TFunction>
		TElement aggregate(const TFunction& f)const
		{
			if (empty()) throw linq_exception("Failed to get a value from an empty collection");
			auto it = _begin;
			auto result = *it;

			while (++it != _end)
			{
				result = f(result, *it);
			}
			return result;
		}

		TElement sum()const
		{
			return aggregate([](const TElement& a, const TElement& b) { return a + b; });
		}

		TElement max()const
		{
			return aggregate([](const TElement& a, const TElement& b) { return a < b ? b : a; });
		}

		TElement min()const
		{
			return aggregate([](const TElement& a, const TElement& b) { return a < b ? a : b; });
		}

		template<typename TResult>
		TResult average()const
		{
			if (empty()) throw linq_exception("Failed to get a value from an empty collection");
			TResult sum = 0;
			int counter = 0;
			for (auto it = _begin; it != _end; ++it)
			{
				sum += (TResult)*it;
				++counter;
			}
			return sum / counter;
		}
#pragma endregion 

	};
	

	template<typename TContainer>
	auto from(const TContainer& c)->linq_enumerable<decltype(std::begin(c))>
	{
		return linq_enumerable<decltype(std::begin(c))>(std::begin(c), std::end(c));
	}


}

