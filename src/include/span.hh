/* span.hh - Rough approximation of the C++20 <span> container/view */
/*
	There is *just* enough of this implemented to allow SNS to use it and
	when upgrading to C++20 to remove this stub and drop in the STL version.

	So don't rely on this too much
*/
#pragma once
#if !defined(__SNS_SPAN_HH__)
#define __SNS_SPAN_HH__
#include <cstdint>
#include <limits>

constexpr size_t dynamic_extent = std::numeric_limits<size_t>::max();

template<typename T, size_t extent = dynamic_extent>
struct span final {
public:
	/* Super jank, pleas ignore */
	template<typename U = T>
	struct contiguous_iterator final {
	private:
		U* const _data;
		size_t _index;
		const size_t _len;

		constexpr contiguous_iterator(U* const data, const size_t index, const size_t len) noexcept :
			_data{data}, _index{index}, _len{len} { /* NOP */ }
	public:
		constexpr contiguous_iterator(U* const data, const size_t len) noexcept :
			_data{data}, _index{}, _len{len} { /* NOP */ }

		contiguous_iterator(const contiguous_iterator& itr) noexcept :
			_data{itr._data}, _index{itr._index}, _len{itr._len} { /* NOP */ }

		U& operator*() const noexcept { return *(_data + _index); }
		U* operator->() const noexcept { return (_data + _index); }

		contiguous_iterator& operator--() noexcept {
			if(_index > 0)
				--_index;

			return *this;
		}

		contiguous_iterator operator--(int) noexcept {
			if(_index > 0)
				return {_data, _index--, _len};

			return *this;
		}

		contiguous_iterator& operator-=(const size_t off) noexcept {
			if(off < _index)
				_index -= off;
			else
				_index = 0;

			return *this;
		}

		contiguous_iterator operator-(const size_t off) const noexcept {
			return contiguous_iterator{*this} -= off;
		}


		contiguous_iterator& operator++() noexcept {
			if(_index < _len)
				++_index;

			return *this;
		}

		contiguous_iterator operator++(int) noexcept {
			if(_index < _len)
				return {_data, _index++, _len};

			return *this;
		}

		contiguous_iterator& operator+=(const size_t off) noexcept {
			if(((_index + off) < _len) && ((_index + off) >= _index))
				_index += off;
			else
				_index = _len;

			return *this;
		}

		contiguous_iterator operator+(const size_t off) const noexcept {
			return contiguous_iterator{*this} += off;
		}

		bool operator==(const contiguous_iterator& itr) const noexcept {
			return ((_data == itr._data) &&
					(_index == itr._index) &&
					(_len == itr._len));
		}

		bool operator!=(const contiguous_iterator& itr) const noexcept { return !(*this == itr); }

		T& operator[](const size_t index) const {
			if(index >= _len)
				throw std::out_of_range("contiguous_iterator<T>[] index out of range");

			return *(_data + index);
		}

	};
private:
	T* _data;
	size_t _len;
public:
	constexpr span() noexcept { /* NOP */ }
	constexpr span(T* data, size_t len) noexcept :
		_data{data}, _len{len} { /* NOP */ }

	~span() noexcept = default;

	constexpr size_t size() const noexcept { return _len; }

	constexpr size_t size_bytes() const noexcept { return (_len * sizeof(T)); }

	[[nodiscard]]
	constexpr bool empty() const noexcept { return (_len == 0); }

	constexpr T& operator[](size_t idx) const {
		if(idx > _len - 1 || empty())
			throw std::out_of_range("index out of range in span<T>[] or span is empty");

		return *(_data + idx);
	}

	constexpr T& front() const {
		if(empty())
			throw std::out_of_range("span is empty");

		return *_data;
	}

	constexpr T& back() const {
		if(empty())
			throw std::out_of_range("span is empty");

		return (*_data + _len - 1);
	}

	constexpr T* data() const noexcept { return _data; }

	constexpr contiguous_iterator<T> begin() const noexcept { return {_data, _len}; }
	constexpr contiguous_iterator<T> end() const noexcept { return (begin() + _len); }

	constexpr contiguous_iterator<const T> cbegin() const noexcept { return {_data, _len}; }
	constexpr contiguous_iterator<const T> cend() const noexcept { return (begin() + _len); }
};

#endif /* __SNS_SPAN_HH__ */
