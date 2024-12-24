#pragma once

namespace mystl {
	template <class T>
	struct remove_reference {
		using type = T;
	};

	template <class T>
	struct remove_reference<T&> {
		using type = T;
	};

	template <class T>
	struct remove_reference<T&&> {
		using type = T;
	};

	template <class T>
	using remove_reference_t = typename remove_reference<T>::type;

	template< class T >
	constexpr mystl::remove_reference_t<T>&& move(T&& t) noexcept {
		return static_cast<mystl::remove_reference_t<T>&&>(t);
	}
}