#pragma once

#include <type_traits>
#include <tuple>

namespace mystl {

	template <class T>
	constexpr std::remove_reference_t<T>&& move(T&& t) noexcept {
		return static_cast<std::remove_reference_t<T>&&>(t);
	}

	template <class T>
	constexpr T&& forward(std::remove_reference_t<T>& t) noexcept {
		return static_cast<T&&>(t);
	}

	template <class T>
	constexpr T&& forward(std::remove_reference_t<T>&& t) noexcept {
		return static_cast<T&&>(t);
	}

	// swap
	// ���� a �� b
	template <class T>
	void swap(T& a, T& b) noexcept(
		std::is_nothrow_move_constructible<T>::value&&
		std::is_nothrow_move_assignable<T>::value
		) {
		auto tmp = mystl::move(a);
		a = mystl::move(b);
		b = tmp;
	}

	// swap
	// �������� a �� b
	template <class T2, size_t N>
	void swap(T2(&a)[N], T2(&b)[N]) noexcept(std::is_nothrow_swappable_v<T2>) {
		size_t count = 0;
		while (count < N) {
			mystl::swap(*(a + count), *(b + count));
			count++;
		}
	}

	// pair
	template <class T1, class T2>
	struct pair {
		using first_type = T1;
		using second_type = T2;

		T1 first;
		T2 second;

		// ���캯��
		template <class U1 = T1, class U2 = T2>
			requires (std::is_default_constructible_v<U1> && std::is_default_constructible_v<U2>)
		pair() : first(), second() {}

		template <class U1 = T1, class U2 = T2>
			requires (std::is_copy_constructible_v<U1> && std::is_copy_constructible_v<U2>)
		constexpr pair(const T1& x, const T2& y) : first(x), second(y) {}

		template <class U1, class U2>
			requires (std::is_constructible_v<T1, U1> && std::is_constructible_v<T2, U2>)
		pair(U1&& x, U2&& y) : first(mystl::move(x)), second(mystl::move(y)) {}

		template <class U1, class U2>
			requires (std::is_constructible_v<T1, const U1&> && std::is_constructible_v<T2, const U2&>)
		constexpr pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}

		template <class U1, class U2>
			requires (std::is_constructible_v<T1, U1> && std::is_constructible_v<T2, U2>)
		pair(pair<U1, U2>&& p) : first(mystl::forward<U1>(p.first)), second(mystl::forward<U2>(p.second)) {}

		pair(const pair& p) = default;
		pair(pair&& p) = default;

		// ��ֵ = 
		template <class U1 = T1, class U2 = T2>
			requires (std::is_copy_assignable_v<U1> && std::is_copy_assignable_v<U2>)
		pair& operator=(const pair& other) {
			first = other.first;
			second = other.second;
			return *this;
		}

		template <class U1 = T1, class U2 = T2>
			requires (!(std::is_copy_assignable_v<U1>) || !(std::is_copy_assignable_v<U2>))
		pair& operator=(const pair& other) = delete;

		template <class U1, class U2>
			requires (std::is_assignable_v<T1&, const U1&> && std::is_assignable_v<T2&, const U2&>)
		pair& operator=(const pair<U1, U2>& other) {
			first = other.first;
			second = other.second;
			return *this;
		}

		template <class U1 = T1, class U2 = T2>
			requires (std::is_move_assignable_v<T1> && std::is_move_assignable_v<T2>)
		pair& operator=(pair&& other) noexcept(std::is_nothrow_move_assignable_v<T1>&&
			std::is_nothrow_move_assignable_v<T2>) {
			first = mystl::move(other.first);
			second = mystl::move(other.second);
			return *this;
		}

		template< class U1, class U2 >
			requires (std::is_assignable_v<T1&, U1> && std::is_assignable_v<T2&, U2>)
		pair& operator=(pair<U1, U2>&& p) {
			first = mystl::forward<U1>(p.first);
			second = mystl::forward<U2>(p.second);
			return *this;
		}

		// swap
		// ���� first �� other.first �� second �� other.second
		void swap(pair& other) noexcept(std::is_nothrow_swappable_v<first_type>&&
			std::is_nothrow_swappable_v<second_type>) {
			mystl::swap(first, other.first);
			mystl::swap(second, other.second);
		}

	};
	
	// make_pair
	// ���� std::pair ���󣬴�ʵ�������Ƶ�Ŀ������
	template <class T1, class T2>
	constexpr pair<std::unwrap_ref_decay_t<T1>,
					std::unwrap_ref_decay_t<T2>>
		make_pair(T1&& x, T2&& y) {
		return pair<std::unwrap_ref_decay_t<T1>, std::unwrap_ref_decay_t<T2>>
			(mystl::forward<T1>(x), mystl::forward<T2>(y));
	}

	// ���رȽϲ�����
	template <class T1, class T2, class U1, class U2>
	constexpr bool operator==(const mystl::pair<T1, T2>& lhs, const mystl::pair<U1, U2>& rhs) {
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	template <class T1, class T2, class U1, class U2>
	constexpr bool operator!=(const mystl::pair<T1, T2>& lhs, const mystl::pair<U1, U2>& rhs) {
		return !(lhs == rhs);
	}

	template <class T1, class T2, class U1, class U2>
	constexpr bool operator<(const mystl::pair<T1, T2>& lhs, const mystl::pair<U1, U2>& rhs) {
		return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
	}

	template <class T1, class T2, class U1, class U2>
	constexpr bool operator>(const mystl::pair<T1, T2>& lhs, const mystl::pair<U1, U2>& rhs) {
		return rhs < lhs;
	}

	template <class T1, class T2, class U1, class U2>
	constexpr bool operator<=(const mystl::pair<T1, T2>& lhs, const mystl::pair<U1, U2>& rhs) {
		return !(rhs < lhs);
	}

	template <class T1, class T2, class U1, class U2>
	constexpr bool operator>=(const mystl::pair<T1, T2>& lhs, const mystl::pair<U1, U2>& rhs) {
		return !(lhs < rhs);
	}

	// get
	// ��Ԫ��ʽ�ӿڴӶ�ż����ȡԪ��
	template <std::size_t I, class T1, class T2>
	constexpr std::tuple_element_t<I, pair<T1, T2>>& 
		get(pair<T1, T2>& p) noexcept {
		if constexpr (I == 0) {
			return p.first;
		} else {
			return p.second;
		}
	}

	template <std::size_t I, class T1, class T2>
	constexpr const std::tuple_element_t<I, pair<T1, T2>>&
		get(const pair<T1, T2>& p) noexcept {
		if constexpr (I == 0) {
			return p.first;
		} else {
			return p.second;
		}
	}

	template <std::size_t I, class T1, class T2>
	constexpr std::tuple_element_t<I, pair<T1, T2>>&& 
		get(pair<T1, T2>&& p) {
		if constexpr (I == 0) {
			return mystl::forward<T1>(p.first);
		} else {
			return mystl::forward<T2>(p.second);
		}
	}

	template <std::size_t I, class T1, class T2>
	constexpr const std::tuple_element_t<I, pair<T1, T2> >&&
		get(const pair<T1, T2>&& p) noexcept {
		if constexpr (I == 0) {
			return mystl::forward<const T1>(p.first);
		} else {
			return mystl::forward<const T2>(p.second);
		}
	}

	template <class T, class U>
	constexpr T& get(pair<T, U>& p) noexcept {
		return p.first;
	}

	template <class T, class U>
	constexpr const T& get(pair<T, U>& p) noexcept {
		return p.first;
	}

	template <class T, class U>
	constexpr T&& get(pair<T, U>&& p) noexcept {
		return mystl::forward<T>(p.first);
	}

	template <class T, class U>
	constexpr const T&& get(pair<T, U>&& p) noexcept {
		return mystl::forward<const T>(p.first);
	}

	template <class T, class U>
	constexpr T& get(pair<U, T>& p) noexcept {
		return p.second;
	}

	template <class T, class U>
	constexpr const T& get(const pair<U, T>& p) noexcept {
		return p.second;
	}

	template <class T, class U>
	constexpr T&& get(pair<U, T>&& p) noexcept {
		return mystl::forward<T>(p.second);
	}

	template <class T, class U>
	constexpr const T&& get(const pair<U, T>&& p) noexcept {
		return mystl::forward<const T>(p.second);
	}

	// exchange
	// �� new_value �滻 obj ��ֵ�������� obj �ľ�ֵ
	template <class T, class U = T>
	T exchange(T& obj, U&& new_value) {
		T tmp = mystl::move(obj);
		obj = mystl::forward<U>(new_value);
		return tmp;
	}

	template <class T>
		requires (std::is_nothrow_move_constructible<T>::value || !std::is_copy_constructible<T>::value)
	std::remove_reference_t<T>&& move_if_noexcept(T& x) noexcept {
		return mystl::move(x);
	}

	template <class T>
		requires (!std::is_nothrow_move_constructible<T>::value && std::is_copy_constructible<T>::value)
	const T& move_if_noexcept(T& x) noexcept {
		return static_cast<const T&>(x);
	}

	// declval
	// �����ڲ���ֵ�ﾳ�б�д���ʽ�ĸ���ģ�壬ͨ��Ϊ decltype �Ĳ�����
	// �ڲ���ֵ�ﾳ�У��������ģ����������� T����Ϊ���������ͣ�ת��Ϊ�����͵�һ�����ʽ��ʹ�ÿ��Բ��������캯������ʹ�� T �ĳ�Ա����
	//template <class T>
	//std::add_rvalue_reference_t<T> declval() noexcept {
	//	static_assert(false, "declval �������������ֵ�ﾳ");
	//}


}