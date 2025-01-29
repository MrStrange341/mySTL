#pragma once
#include "utility.h"

namespace mystl {
	// 指示迭代器类别的空类类型
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : forward_iterator_tag {};
	struct random_access_iterator_tag : bidirectional_iterator_tag {};

	// 简化迭代器所要求的类型定义而提供的基类
	template <class Category, class T, class Distance = std::ptrdiff_t, 
		class Pointer = T*, class Reference = T&>
	struct iterator {
		using iterator_category = Category;
		using value_type = T;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Reference;
	};

	// 为迭代器各项性质提供统一接口
	template <class Iter>
	struct iterator_traits {
		using difference_type = Iter::difference_type;
		using value_type = Iter::value_type;
		using pointer = Iter::pointer;
		using reference = Iter::reference;
		using iterator_category = Iter::iterator_category;
	};

	// T* 特化
	template <class T>
	struct iterator_traits<T*> {
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = random_access_iterator_tag;
	};

	// const T* 特化
	template <class T>
	struct iterator_traits<const T*> {
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = const T*;
		using referendce = const T&;
		using iterator_category = random_access_iterator_tag;
	};

	// reverse_iterator
	// 迭代器适配器，它反转给定迭代器的方向
	template <class Iter>
	class reverse_iterator {
	public:
		using iterator_type = Iter;
		using iterator_category = typename mystl::iterator_traits<Iter>::iterator_category;
		using value_type = typename mystl::iterator_traits<Iter>::value_type;
		using difference_type = typename mystl::iterator_traits<Iter>::difference_type;
		using pointer = typename mystl::iterator_traits<Iter>::pointer;
		using reference = typename mystl::iterator_traits<Iter>::reference;

		// 构造函数
		constexpr reverse_iterator() {};
		constexpr reverse_iterator(iterator_type x) : current(x) {}
		template <class U>
		constexpr reverse_iterator(const reverse_iterator<U>& other) : current(other.current) {}

		template <class U>
		constexpr reverse_iterator& operator=(const reverse_iterator<U>& other) {
			current = other.current;
		}

		constexpr iterator_type base() const {
			return current;
		}

		constexpr reference operator*() const {
			Iter tmp = current;
			return *-tmp;
		}

		constexpr pointer operator->() const {
			return std::addressof(operator*());
		}

		constexpr reference operator[] (difference_type n) const {
			return current[n - 1];
		}

		constexpr reverse_iterator& operator++() {
			--current;
			return *this;
		}
		constexpr reverse_iterator& operator--() {
			++current;
			return *this;
		}
		constexpr reverse_iterator operator++(int) {
			reverse_iterator tmp = *this;
			--current;
			return tmp;
		}
		constexpr reverse_iterator operator--(int) {
			reverse_iterator tmp = *this;
			++current;
			return tmp;
		}
		constexpr reverse_iterator operator+(difference_type n) const {
			return reverse_iterator(current - n);
		}
		constexpr reverse_iterator operator-(difference_type n) const {
			return reverse_iterator(current + n);
		}
		constexpr reverse_iterator& operator+=(difference_type n) {
			current -= n;
			return *this;
		}
		constexpr reverse_iterator& operator-=(difference_type n) {
			current += n;
			return *this;
		}
	protected:
		Iter current{};
	};

	template <class Iter1, class Iter2>
	constexpr bool operator==(const mystl::reverse_iterator<Iter1>& lhs,
		const mystl::reverse_iterator<Iter2>& rhs) {
		return lhs.base() == rhs.base();
	}
	template <class Iter1, class Iter2>
	constexpr bool operator!=(const mystl::reverse_iterator<Iter1>& lhs,
		const mystl::reverse_iterator<Iter2>& rhs) {
		return lhs.base() != rhs.base();
	}
	template <class Iter1, class Iter2>
	constexpr bool operator>(const mystl::reverse_iterator<Iter1>& lhs,
		const mystl::reverse_iterator<Iter2>& rhs) {
		return lhs.base() > rhs.base();
	}
	template <class Iter1, class Iter2>
	constexpr bool operator<(const mystl::reverse_iterator<Iter1>& lhs,
		const mystl::reverse_iterator<Iter2>& rhs) {
		return lhs.base() < rhs.base();
	}
	template <class Iter1, class Iter2>
	constexpr bool operator<=(const mystl::reverse_iterator<Iter1>& lhs,
		const mystl::reverse_iterator<Iter2>& rhs) {
		return lhs.base() <= rhs.base();
	}
	template <class Iter1, class Iter2>
	constexpr bool operator>=(const mystl::reverse_iterator<Iter1>& lhs,
		const mystl::reverse_iterator<Iter2>& rhs) {
		return lhs.base() >= rhs.base();
	}

	template <class Iter>
	constexpr reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference n,
		const reverse_iterator<Iter>& it) {
		return reverse_iterator<Iter>(it.base() - n);
	}

	template <class Iter1, class Iter2>
	auto operator-(
		const reverse_iterator<Iter1>& lhs,
		const reverse_iterator<Iter2>& rhs
		) -> decltype(rhs.base(), lhs.base()) {
		return rhs.base() - lhs.base();
	}

	template <class Iter>
	constexpr reverse_iterator<Iter> make_reverse_iterator(Iter i) {
		return reverse_iterator<Iter>(i);
	}

	template <typename T>
	struct is_reference {
		using type = T;
	};

	template <typename T>
	struct is_reference<T&> {
		using type = T;
	};

	template <typename T>
	struct is_reference<T&&> {
		using type = std::remove_reference_t<T>&&;
	};

	template <typename T>
	using is_reference_t = is_reference<T>;

	template <class Iter>
	class move_iterator {
	public:
		using iterator_type = Iter;
		using iterattor_category = mystl::iterator_traits<Iter>::iterator_category;
		using value_type = mystl::iterator_traits<Iter>::value_type;
		using difference_type = mystl::iterator_traits<Iter>::difference_type;
		using pointer = mystl::iterator_traits<Iter>::pointer;
		using reference = is_reference_t<Iter>;

		// 构造函数
		constexpr move_iterator() {};
		constexpr explicit move_iterator(iterator_type x) : current(x) {}
		template <class U>
		constexpr move_iterator(const move_iterator<U>& other) {
			current = other.current;
		}

		template <class U>
		constexpr move_iterator& operator=(const move_iterator<U>& other) {
			current = other.current;
			return *this;
		}

		constexpr iterator_type base() const {
			return current;
		}

		constexpr reference operator*() const {
			return static_cast<reference>(*current);
		}
		constexpr pointer operator->() const {
			return current;
		}

		constexpr reference operator[](difference_type n) const {
			return mystl::move(current[n]);
		}

		constexpr move_iterator& operator++() {
			++current;
			return *this;
		}
		constexpr move_iterator& operator--() {
			--current;
			return *this;
		}
		constexpr move_iterator operator++(int) {
			move_iterator tmp = *this;
			++current;
			return tmp;
		}
		constexpr move_iterator operator--(int) {
			move_iterator tmp = *this;
			--current;
			return tmp;
		}
		constexpr move_iterator operator+(difference_type n) const {
			return move_iterator(current + n);
		}
		constexpr move_iterator operator-(difference_type n) const {
			return move_iterator(current - n);
		}
		constexpr move_iterator& operator+=(difference_type n) {
			current += n;
			return *this;
		}
		constexpr move_iterator& operator-=(difference_type n) {
			current -= n;
			return *this;
		}
	private:
		Iter current{};
	};


	template <class Iter1, class Iter2>
	constexpr bool operator==(const mystl::move_iterator<Iter1>& lhs,
		const mystl::move_iterator<Iter2>& rhs) {
		return lhs.base() == rhs.base();
	}
	template <class Iter1, class Iter2>
	constexpr bool operator!=(const mystl::move_iterator<Iter1>& lhs,
		const mystl::move_iterator<Iter2>& rhs) {
		return lhs.base() != rhs.base();
	}
	template <class Iter1, class Iter2>
	constexpr bool operator>(const mystl::move_iterator<Iter1>& lhs,
		const mystl::move_iterator<Iter2>& rhs) {
		return lhs.base() > rhs.base();
	}
	template <class Iter1, class Iter2>
	constexpr bool operator<(const mystl::move_iterator<Iter1>& lhs,
		const mystl::move_iterator<Iter2>& rhs) {
		return lhs.base() < rhs.base();
	}
	template <class Iter1, class Iter2>
	constexpr bool operator<=(const mystl::move_iterator<Iter1>& lhs,
		const mystl::move_iterator<Iter2>& rhs) {
		return lhs.base() <= rhs.base();
	}
	template <class Iter1, class Iter2>
	constexpr bool operator>=(const mystl::move_iterator<Iter1>& lhs,
		const mystl::move_iterator<Iter2>& rhs) {
		return lhs.base() >= rhs.base();
	}

	template <class Iter>
	constexpr move_iterator<Iter> operator+(typename move_iterator<Iter>::difference_type n,
		const move_iterator<Iter>& it) {
		return it + n;
	}
	template <class Iter1, class Iter2>
	auto operator-(const move_iterator<Iter1>& lhs,
		const move_iterator<Iter2>& rhs)
		-> decltype(lhs.base() - rhs.base()) {
		return lhs.base() - rhs.base();
	}

	template <class Iter>
	constexpr move_iterator<Iter> make_move_iterator(Iter i) {
		return move_iterator<Iter>(i);
	}

	template <class Container>
	class back_insert_iterator :
		public mystl::iterator<mystl::output_iterator_tag, void, void, void, void> {
		explicit back_insert_iterator(Container& c) : container(std::addressof(c)) {}

		back_insert_iterator<Container>&
			operator=(const typename Container::value_type& value) {
			container->push_back(value);
			return *this;
		}
		back_insert_iterator<Container>&
			operator=(typename Container::value_type&& value) {
			container->push_back(mystl::move(value));
			return *this;
		}
		back_insert_iterator& operator*() {
			return *this;
		}
		back_insert_iterator& operator++() {
			return *this;
		}
		back_insert_iterator operator++(int) {
			return *this;
		}

	protected:
		Container* container{};
	};

	template <class Container>
	class front_insert_iterator :
		public mystl::iterator<mystl::output_iterator_tag, void, void, void, void> {
		explicit front_insert_iterator(Container& c) : container(std::addressof(c)) {}

		front_insert_iterator<Container>& 
			operator=(const typename Container::value_type& value) {
			container->push_front(value);
			return *this;
		}
		front_insert_iterator<Container>&
			operator=(typename Container::value_type&& value) {
			container->push_front(mystl::move(value));
			return *this;
		}
		front_insert_iterator& operator*() {
			return *this;
		}
		front_insert_iterator& operator++() {
			return *this;
		}
		front_insert_iterator operator++(int) {
			return *this;
		}

	protected:
		Container* container{};
	};
	
	template <class Container>
	class insert_iterator :
		public mystl::iterator<mystl::output_iterator_tag, void, void, void, void> {

		insert_iterator(Container& c, typename Container::iterator i) : container(std::addressof(c)), iter(i) {}

		insert_iterator<Container>&
			operator=(const typename Container::value_type& value) {
			iter = container->itsert(iter, value);
			++iter;
			return *this;
		}
		insert_iterator<Container>&
			operator=(typename Container::value&& value) {
			iter = container->insert(iter, value);
			++iter;
			return *this;
		}
		insert_iterator& operator*() {
			return *this;
		}
		insert_iterator& operator++() {
			return *this;
		}
		insert_iterator operator++(int) {
			return *this;
		}

	protected:
		Container* container{};
		Container::iterator iter{};
	};

	template <class Container>
	mystl::front_insert_iterator<Container> front_inserter(Container& c) {
		return mystl::front_insert_iterator<Container>(c);
	}
	template <class Container>
	mystl::back_insert_iterator<Container> back_inserter(Container& c) {
		return mystl::back_insert_iterator<Container>(c);
	}
	template <class Container>
	mystl::insert_iterator<Container> inserter(Container& c) {
		return mystl::insert_iterator<Container>(c);
	}

	template <class InputIt, class Distance>
	constexpr void advance(InputIt& it, Distance n) {
		using category = typename mystl::iterator_traits<InputIt>::iterator_category;
		static_assert(std::is_base_of_v<std::input_iterator_tag, category> || 
			std::is_base_of_v<mystl::input_iterator_tag, category>);
		auto dist = typename mystl::iterator_traits<InputIt>::difference_type(n);
		if constexpr (std::is_base_of_v<std::random_access_iterator_tag, category> ||
			std::is_base_of_v<mystl::random_access_iterator_tag, category>) {
			it += dist;
		} else {
			while (dist > 0) {
				--dist;
				++it;
			}
			if constexpr (std::is_base_of_v<std::bidirectional_iterator_tag, category> || 
				std::is_base_of_v<mystl::bidirectional_iterator_tag, category>) {
				while (dist < 0) {
					++dist;
					--it;
				}
			}
		}
	}
	
	template <class InputIt>
	typename mystl::iterator_traits<InputIt>::differnece_type
		distance(InputIt first, InputIt last) {
		using category = typename mystl::iterator_traits<InputIt>::iterator_category;
		static_assert(std::is_base_of_v<std::input_iterator_tag, category> ||
			std::is_base_of_v<mystl::input_iterator_tag, category>);
		if constexpr (std::is_base_of_v<std::random_access_iterator_tag, category> ||
			std::is_base_of_v<mystl::random_access_iterator_tag, category>) {
			return last - first;
		} else {
			typename mystl::iterator_traits<InputIt>::difference_type result = 0;
			while (first != last) {
				++first;
				++result;
			}
			return result;
		}
	}

	template <class InputIt>
	constexpr InputIt next(InputIt it, typename mystl::iterator_traits<InputIt>::difference_type n = 1) {
		mystl::advance(it, n);
		return it;
	}

	template <class InputIt>
	constexpr InputIt prev(InputIt it, typename mystl::iterator_traits<InputIt>::difference_type n = 1) {
		mystl::advance(it, -n);
		return it;
	}
	// begin, cbegin
	template <class C>
	constexpr auto begin(C& c) -> decltype(c.begin()) {
		return c.begin();
	}
	template <class C>
	constexpr auto begin(const C& c) -> decltype(c.begin()) {
		return c.begin();
	}
	template <class T, std::size_t N>
	constexpr T* begin(T(&array)[N]) noexcept {
		return array;
	}
	template <class C>
	constexpr auto cbegin(const C& c) noexcept(noexcept(mystl::begin(c))) 
		-> decltype(mystl::begin(c)) {
		return c.begin();
	}
	// end, cend
	template <class C>
	constexpr auto end(C& c) -> decltype(c.end()) {
		return c.end();
	}
	template <class C>
	constexpr auto end(const C& c) -> decltype(c.end()) {
		return c.end();
	}
	template <class T, std::size_t N>
	constexpr T* end(T(&array)[N]) {
		return array;
	}
	template <class C>
	constexpr auto cend(const C& c) noexcept(noexcept(mystl::end(c)))
		-> decltype(mystl::end(c)) {
		return c.end();
	}
	// rbegin, crbegin
	template <class C>
	constexpr auto rbegin(C& c) -> decltype(c.rbegin()) {
		return c.rbegin();
	}
	template <class C>
	constexpr auto rbegin(const C& c) -> decltype(c.rbegin()) {
		return c.rbegin();
	}
	template <class T, std::size_t N>
	constexpr mystl::reverse_iterator<T*> rbegin(T(&array)[N]) {
		return mystl::reverse_iterator<T*>(array + N);
	}
	template <class T>
	constexpr mystl::reverse_iterator<const T*> rbegin(std::initializer_list<T> il) {
		return mystl::reverse_iterator<const T*>(il.end());
	}
	template  <class C>
	constexpr auto crbegin(const C& c) -> decltype(mystl::rbegin(c)) {
		return c.rbegin();
	}
	// rend, crend
	template <class C>
	constexpr auto rend(C& c) -> decltype(c.rend()) {
		return c.rend();
	}
	template <class C>
	constexpr auto rend(const C& c) -> decltype(c.rend()) {
		return c.rend();
	}
	template <class T, std::size_t N>
	constexpr mystl::reverse_iterator<T*> rend(T(&array)[N]) {
		return mystl::reverse_iterator<T*>(array);
	}
	template <class T>
	constexpr mystl::reverse_iterator<const T*> rend(std::initializer_list<T> il) {
		return mystl::reverse_iterator<const T*>(il.begin());
	}
	template <class C>
	constexpr auto crend(const C& c) -> decltype(mystl::rend(c)) {
		return c.rend();
	}
}