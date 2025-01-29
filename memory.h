#pragma once
#include <atomic>
#include <exception>
#include <type_traits>
#include "algorithm.h"
//#include "mtmalloc.h"

namespace mystl {
	template <class Ptr>
	using pointer_traits = std::pointer_traits<Ptr>;

	enum class pointer_safety {
		relaxed,
		preferred,
		strict
	};

	template <class T>
	struct allocator {
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using propagate_on_container_move_assignment = std::true_type;

		template <class U>
		struct rebind {
			typedef allocator<U> other;
		};

		using is_always_equal = std::true_type;

		allocator() noexcept {}
		allocator(const allocator& other) noexcept {}
		
		template <class U>
		allocator(const allocator<U>& other) noexcept {}

		~allocator() {}

		// address
		// 返回 x 的实际地址，即使存在重载的 operator& 也是如此
		pointer address(reference x) const noexcept {
			return std::addressof(x);
		}
		pointer address(const_reference x) const noexcept {
			return std::addressof(x);
		}

		T* allocate(std::size_t n) {
			return static_cast<T*>(::operator new(n * sizeof(T)));
			//return static_cast<T*>(mtmalloc::malloc(n * sizeof(T)));
		}
		void deallocate(T* p, std::size_t n) {
			::operator delete(p);
			//mtmalloc::free(p);
		}

		template <class U, class... Args>
		void construct(U* p, Args&&... args) {
			::new(p) U(mystl::forward<Args>(args)...);
		}
		template <class U>
		void destroy(U* p) {
			p->~U();
		}
	};

	template <class Alloc>
	using allocator_traits = std::allocator_traits<Alloc>;

	//template <class T, class Alloc>
	//struct uses_allocator : std::_Has_allocator_type<T, Alloc>::type {
	//};

	//template <class T, class Alloc>
	//constexpr bool uses_allocator_v = uses_allocator<T, Alloc>::value;

	// unique_ptr
	template <class T>
	class unique_ptr {
	public:
		using pointer = T*;
		using element_type = T;

	private:
		pointer ptr_{};

	public:
		constexpr unique_ptr() noexcept {}
		constexpr unique_ptr(std::nullptr_t) noexcept {}
		explicit unique_ptr(pointer p) noexcept : ptr_(p) {}

		unique_ptr(unique_ptr&& other) noexcept {
			ptr_ = other.ptr_;
			other.ptr_ = nullptr;
		}

		//template <class U, class E>
		//unique_ptr(unique_ptr<U, E>&& u) noexcept {
		//	ptr = u.ptr;
		//	u.ptr = nullptr;
		//}
		
		unique_ptr(const unique_ptr&) = delete;

		~unique_ptr() {
			if (ptr_ != nullptr) {
				delete ptr_;
				ptr_ = nullptr;
			}
		}

		unique_ptr& operator=(unique_ptr&& r) noexcept {
			if (this == &r) {
				return *this;
			}
			~unique_ptr();
			ptr_ = r.ptr_;
			r.ptr_ = nullptr;
			return *this;
		}

		//template <class U, class E>
		//unique_ptr& operator=(unique_ptr<U, E>&& r) noexcept {
		//	*this = r;
		//}

		unique_ptr& operator=(std::nullptr_t) noexcept {
			~unique_ptr();
			return *this;
		}

		unique_ptr& operator=(const unique_ptr&) = delete;

		pointer get() const noexcept {
			return ptr_;
		}

		// operator bool
		explicit operator bool() const noexcept {
			return ptr_ != nullptr;
		}

		pointer release() noexcept {
			auto tmp = ptr_;
			ptr_ = nullptr;
			return tmp;
		}

		void reset(pointer ptr = pointer{}) noexcept {
			~unique_ptr();
			ptr_ = ptr;
		}

		void swap(unique_ptr& other) noexcept {
			mystl::swap(ptr_, other.ptr_);
		}

		std::add_lvalue_reference_t<T> operator*() const noexcept(noexcept(*std::declval<pointer>())) {
			return *ptr_;
		}

		pointer operator->() const noexcept {
			return ptr_;
		}
	};

	// T[] 版本
	template <class T>
	class unique_ptr<T[]> {
		using pointer = T*;
		using element_type = T;

	private:
		pointer ptr_{};

	public:
		constexpr unique_ptr() noexcept {}
		constexpr unique_ptr(std::nullptr_t) noexcept {}

		template <class U>
		explicit unique_ptr(U p) noexcept : ptr_(p) {}

		unique_ptr(unique_ptr&& u) noexcept {
			ptr_ = u.ptr_;
			u.ptr_ = nullptr;
		}

		unique_ptr(const unique_ptr&) = delete;
	
		~unique_ptr() {
			if (ptr_ != nullptr) {
				delete[] ptr_;
				ptr_ = nullptr;
			}
		}
		
		unique_ptr& operator=(unique_ptr&& r) noexcept {
			if (this == &r) {
				return *this;
			}
			~unique_ptr();
			ptr_ = r.ptr_;
			r.ptr_ = nullptr;
			return *this;
		}

		unique_ptr& operator=(std::nullptr_t) noexcept {
			~unique_ptr();
			return *this;
		}

		unique_ptr& operator=(const unique_ptr&) = delete;
		
		pointer get() const noexcept {
			return ptr_;
		}

		explicit operator bool() const noexcept {
			return ptr_ != nullptr;
		}

		pointer release() {
			auto tmp = ptr_;
			ptr_ = nullptr;
			return tmp;
		}

		void reset(pointer ptr = pointer{}) {
			~unique_ptr();
			ptr_ = ptr;
		}

		T& operator[](std::size_t i) const {
			return *(ptr_ + i);
		}
	};

	template <class T>
	constexpr bool is_array = false;
	template <class T>
	constexpr bool is_array<T[]> = true;

	template <class T, class... Args>
		requires (mystl::is_array<T> == false)
	unique_ptr<T> make_unique(Args&&... args) {
		return unique_ptr<T>(new T(mystl::forward<Args>(args)...));
	}

	template <class T>
		requires mystl::is_array<T>
	unique_ptr<T> make_unique(std::size_t size) {
		return unique_ptr<T>(new std::remove_extent_t<T>[size]());
	}

	template <class T, class... Args>
		requires mystl::is_array<T>
	unique_ptr<T> make_unique(Args... args) = delete;

	template <class T1, class T2>
	bool operator==(const unique_ptr<T1>& x, const unique_ptr<T2>& y) {
		return x.get() == y.get();
	}
	template <class T1, class T2>
	bool operator!=(const unique_ptr<T1>& x, const unique_ptr<T2>& y) {
		return !(x == y);
	}
	template <class T1, class T2>
	bool operator<(const unique_ptr<T1>& x, const unique_ptr<T2>& y) {
		return x.get() < y.get();
	}
	template <class T1, class T2>
	bool operator<=(const unique_ptr<T1>& x, const unique_ptr<T2>& y) {
		return !(y < x);
	}
	template <class T1, class T2>
	bool operator>(const unique_ptr<T1>& x, const unique_ptr<T2>& y) {
		return y < x;
	}
	template <class T1, class T2>
	bool operator>=(const unique_ptr<T1>& x, const unique_ptr<T2>& y) {
		return !(x < y);
	}
	template <class T>
	bool operator==(const unique_ptr<T>& x, std::nullptr_t) noexcept {
		return !x;
	}
	template <class T>
	bool operator==(std::nullptr_t, const unique_ptr<T>& x) noexcept {
		return !x;
	}
	template <class T>
	bool operator!=(const unique_ptr<T>& x, std::nullptr_t) noexcept {
		return x;
	}
	template <class T>
	bool operator!=(std::nullptr_t, const unique_ptr<T>& x) noexcept {
		return x;
	}
	template <class T>
	bool operator<(const unique_ptr<T>& x, std::nullptr_t) noexcept {
		return x.get() < nullptr;
	}
	template <class T>
	bool operator<(std::nullptr_t, const unique_ptr<T>& x) noexcept {
		return nullptr < x.get();
	}
	template <class T>
	bool operator<=(const unique_ptr<T>& x, std::nullptr_t) noexcept {
		return !(nullptr < x);
	}
	template <class T>
	bool operator<=(std::nullptr_t, const unique_ptr<T>& x) noexcept {
		return !(x < nullptr);
	}
	template <class T>
	bool operator>(const unique_ptr<T>& x, std::nullptr_t) noexcept {
		return nullptr < x;
	}
	template <class T>
	bool operator>(std::nullptr_t, const unique_ptr<T>& x) noexcept {
		return x < nullptr;
	}
	template <class T>
	bool operator>=(const unique_ptr<T>& x, std::nullptr_t) noexcept {
		return !(x < nullptr);
	}
	template <class T>
	bool operator>=(std::nullptr_t, const unique_ptr<T>& x) noexcept {
		return !(nullptr < x);
	}
	template <class T>
	void swap(mystl::unique_ptr<T>& lhs, mystl::unique_ptr<T>& rhs) noexcept {
		lhs.swap(rhs);
	}
	
	// 控制块
	template <class T>
	struct CtrlBlock {
		T* ptr_;
		std::atomic<long> shared_count{1};
		std::atomic<long> weak_count{1};
		CtrlBlock() : ptr_(nullptr) {}
		CtrlBlock(T* ptr) : ptr_(ptr) {}
	};

	// weak_ptr 声明
	template <class T>
	class weak_ptr;

	class bad_weak_ptr : public std::exception {
	public:
		/*virtual*/ char const* what() const override {
			return "bad weak ptr";
		}
	};

	// shared_ptr
	template <class T>
	class shared_ptr {
	public:
		using element_type = T;
		//using weak_type = mystl::weak_ptr<T>;
		CtrlBlock<T>* pcb_{};
		element_type* ptr_{};

	public:
		constexpr shared_ptr() noexcept {}
		constexpr shared_ptr(std::nullptr_t) noexcept {}

		template <class Y>
		explicit shared_ptr(Y* ptr) {
			pcb_ = new CtrlBlock<Y>(ptr);
			ptr_ = ptr;
		}

		template <class Y>
		shared_ptr(const shared_ptr<Y>& r, element_type* ptr) noexcept {
			pcb_ = reinterpret_cast<CtrlBlock<T>*>(r.pcb_);
			if (pcb_ != nullptr) {
				++pcb_->shared_count;
				++pcb_->weak_count;
			}
			ptr_ = ptr;
		}

		shared_ptr(const shared_ptr& r) noexcept {
			pcb_ = const_cast<CtrlBlock<T>*>(r.pcb_);
			if (pcb_ != nullptr) {
				++pcb_->shared_count;
				++pcb_->weak_count;
			}
			ptr_ = const_cast<T*>(r.get());
		}

		template <class Y>
			requires std::is_convertible_v<Y*, T*>
		shared_ptr(const shared_ptr<Y>& r) noexcept {
			//r.pcb_  CtrlBlock<Y>* const
			//pcb_    CtrlBlock<T>*
			//reinterpret_cast

			//共享所有权
			pcb_ = reinterpret_cast<CtrlBlock<T>*>(r.pcb_);
			if (pcb_ != nullptr) {
				++pcb_->shared_count;
				++pcb_->weak_count;
			}
			ptr_ = const_cast<Y*>(r.get());
		}

		shared_ptr(shared_ptr&& r) noexcept {
			pcb_ = r.pcb_;
			ptr_ = r.get();

			r.pcb_ = nullptr;
			r.ptr_ = nullptr;
		}

		template <class Y>
			requires std::is_convertible_v<Y*, T*>
		shared_ptr(shared_ptr<Y>&& r) noexcept {
			pcb_ = reinterpret_cast<CtrlBlock<T>*>(r.pcb_);
			ptr_ = r.get();

			r.pcb_ = nullptr;
			r.ptr_ = nullptr;
		}

		template <class Y>
		explicit shared_ptr(const mystl::weak_ptr<Y>& r) {
			if (r.pcb_ == nullptr) {
				pcb_ = nullptr;
				ptr_ = nullptr;
			}
			pcb_ = reinterpret_cast<CtrlBlock<T>*>(r.pcb_);
			++pcb_->shared_count;
			++pcb_->weak_count;
			ptr_ = pcb_->ptr_;
		}

		//shared_ptr<int> sp{std::move(up)};
		template <class Y>
		shared_ptr(mystl::unique_ptr<Y>&& r) {
			auto p = r.release();
			pcb_ = new CtrlBlock<Y>(p);
			ptr_ = p;
		}

		~shared_ptr() {
			if (pcb_ != nullptr) {
				if (--pcb_->shared_count == 0) {
					delete pcb_->ptr_;
					pcb_->ptr_ = nullptr;
				}
				if (--pcb_->weak_count == 0) {
					delete pcb_;
					pcb_ = nullptr;
				}
			}
			ptr_ = nullptr;
		}

		shared_ptr& operator=(const shared_ptr& r) noexcept {
			shared_ptr<T>(r).swap(*this);
			return *this;
		}

		template <class Y>
		shared_ptr& operator=(const shared_ptr<Y>& r) noexcept {
			shared_ptr<T>(r).swap(*this);
			return *this;
		}

		shared_ptr& operator=(shared_ptr&& r) noexcept {
			shared_ptr<T>(std::move(r)).swap(*this);
			return *this;
		}

		template <class Y>
		shared_ptr& operator=(shared_ptr<Y>&& r) noexcept {
			//复用移动构造
			shared_ptr<T>(std::move(r)).swap(*this);
			return *this;
		}
		
		void reset() noexcept {
			~shared_ptr();
		}

		template <class Y>
		void reset(Y* ptr) {
			~shared_ptr();
			pcb_ = new CtrlBlock<Y>(ptr);
			ptr_ = ptr;
		}

		void swap(shared_ptr& r) noexcept {
			mystl::swap(pcb_, r.pcb_);
			mystl::swap(ptr_, r.ptr_);
		}

		element_type* get() const {
			return ptr_;
		}

		T& operator*() const noexcept {
			return *ptr_;
		}

		T* operator->() const noexcept {
			return ptr_;
		}
		
		long use_count() const noexcept {
			if (pcb_ == nullptr) {
				return 0;
			}
			return pcb_->shared_count;
		}
		
		bool unique() const noexcept {
			if (pcb_ == nullptr) {
				return 0;
			}
			return pcb_->shared_count == 1;
		}
		
		explicit operator bool() const noexcept {
			return ptr_ != nullptr;
		}

		template <class Y>
		bool owner_before(const shared_ptr<Y>& other) const noexcept {
			return pcb_ < other.pcb_;
		}

		template <class Y>
		bool owner_before(const weak_ptr<Y>& other) const noexcept {
			return pcb_ < other.pcb_;
		}
	};

	template <class T, class... Args>
	mystl::shared_ptr<T> make_shared(Args&&... args) {
		return shared_ptr<T>(new T(mystl::forward<Args>(args)...));
	}

	template<class T, class U>
	mystl::shared_ptr<T> static_pointer_cast(const mystl::shared_ptr<U>& r) noexcept {
		auto p = static_cast<typename mystl::shared_ptr<T>::element_type*>(r.get());
		return mystl::shared_ptr<T>{r, p};
	}

	template<class T, class U>
	mystl::shared_ptr<T> dynamic_pointer_cast(const mystl::shared_ptr<U>& r) noexcept {
		if (auto p = dynamic_cast<typename mystl::shared_ptr<T>::element_type*>(r.get())) {
			return mystl::shared_ptr<T>{r, p};
		} else {
			return mystl::shared_ptr<T>{};
		}
	}

	template<class T, class U>
	mystl::shared_ptr<T> const_pointer_cast(const mystl::shared_ptr<U>& r) noexcept {
		auto p = const_cast<typename mystl::shared_ptr<T>::element_type*>(r.get());
		return mystl::shared_ptr<T>{r, p};
	}

	template<class T, class U>
	mystl::shared_ptr<T> reinterpret_pointer_cast(const mystl::shared_ptr<U>& r) noexcept {
		auto p = reinterpret_cast<typename mystl::shared_ptr<T>::element_type*>(r.get());
		return mystl::shared_ptr<T>{r, p};
	}

	template <class T, class U>
	bool operator==(const mystl::shared_ptr<T>& lhs, 
		const mystl::shared_ptr<U>& rhs) noexcept {
		return lhs.get() == rhs.get();
	}
	template <class T, class U>
	bool operator!=(const mystl::shared_ptr<T>& lhs,
		const mystl::shared_ptr<U>& rhs) noexcept {
		return !(lhs == rhs);
	}
	template <class T, class U>
	bool operator<(const mystl::shared_ptr<T>& lhs,
		const mystl::shared_ptr<U>& rhs) noexcept {
		return lhs.get() < rhs.get();
	}
	template <class T, class U>
	bool operator>(const mystl::shared_ptr<T>& lhs,
		const mystl::shared_ptr<U>& rhs) noexcept {
		return rhs < lhs;
	}
	template <class T, class U>
	bool operator<=(const mystl::shared_ptr<T>& lhs,
		const mystl::shared_ptr<U>& rhs) noexcept {
		return !(rhs < lhs);
	}
	template <class T, class U>
	bool operator>=(const mystl::shared_ptr<T>& lhs,
		const mystl::shared_ptr<U>& rhs) noexcept {
		return !(lhs < rhs);
	}

	template <class T>
	bool operator==(const mystl::shared_ptr<T>& lhs, std::nullptr_t) noexcept {
		return lhs == nullptr;
	}
	template <class T>
	bool operator==(std::nullptr_t, const mystl::shared_ptr<T>& lhs) noexcept {
		return nullptr == lhs;
	}
	template <class T>
	bool operator!=(const mystl::shared_ptr<T>& lhs, std::nullptr_t) noexcept {
		return !(lhs);
	}
	template <class T>
	bool operator!=(std::nullptr_t, const mystl::shared_ptr<T>& lhs) noexcept {
		return !(lhs);
	}
	template <class T>
	bool operator<(const mystl::shared_ptr<T>& lhs, std::nullptr_t) noexcept {
		return lhs.get() < nullptr;
	}
	template <class T>
	bool operator<(std::nullptr_t, const mystl::shared_ptr<T>& lhs) noexcept {
		return nullptr < lhs;
	}
	template <class T>
	bool operator>(const mystl::shared_ptr<T>& lhs, std::nullptr_t) noexcept {
		return nullptr < lhs;
	}
	template <class T>
	bool operator>(std::nullptr_t, const mystl::shared_ptr<T>& lhs) noexcept {
		return lhs < nullptr;
	}
	template <class T>
	bool operator<=(const mystl::shared_ptr<T>& lhs, std::nullptr_t) noexcept {
		return !(nullptr < lhs);
	}
	template <class T>
	bool operator<=(std::nullptr_t, const mystl::shared_ptr<T>& lhs) noexcept {
		return !(lhs < nullptr);
	}
	template <class T>
	bool operator>=(const mystl::shared_ptr<T>&lhs, std::nullptr_t) noexcept {
		return !(lhs < nullptr);
	}
	template <class T>
	bool operator>=(std::nullptr_t, const mystl::shared_ptr<T>& lhs) noexcept {
		return !(nullptr < lhs);
	}

	template <class T>
	void swap(mystl::shared_ptr<T>& lhs, mystl::shared_ptr<T>& rhs) noexcept {
		lhs.swap(rhs);
	}

	// 向日葵连线 review 到此
	
	// weak_ptr
	template <class T>
	class weak_ptr {
	public:
		using element_type = T;

		CtrlBlock<T>* pcb_{};

		constexpr weak_ptr() noexcept {}
		weak_ptr(const weak_ptr& r) noexcept {
			pcb_ = const_cast<CtrlBlock<T>*>(r.pcb_);
			if (pcb_ != nullptr) {
				++pcb_->weak_count;
			}
		}
		template <class Y>
			requires std::is_convertible_v<Y*, T*>
		weak_ptr(const weak_ptr<Y>& r) noexcept {
			pcb_ = reinterpret_cast<CtrlBlock<T>*>(r.pcb_);
			if (pcb_ != nullptr) {
				++pcb_->weak_count;
			}
		}

		template <class Y>
			requires std::is_convertible_v<Y*, T*>
		weak_ptr(const mystl::shared_ptr<Y>& r) noexcept {
			pcb_ = reinterpret_cast<CtrlBlock<T>*>(r.pcb_);
			if (pcb_ != nullptr) {
				++pcb_->weak_count;
			}
		}

		weak_ptr(weak_ptr&& r) noexcept {
			pcb_ = r.pcb_;

			r.pcb_ = nullptr;
		}
		template <class Y>
			requires std::is_convertible_v<Y*, T*>
		weak_ptr(weak_ptr<Y>&& r) noexcept {
			pcb_ = reinterpret_cast<CtrlBlock<T>*>(r.pcb_);

			r.pcb_ = nullptr;
		}

		~weak_ptr() {
			if (pcb_ != nullptr) {
				--pcb_->weak_count;
				pcb_ = nullptr;
			}
		}

		weak_ptr& operator=(const weak_ptr& r) noexcept {
			mystl::weak_ptr<T>(r).swap(*this);
			return *this;
		}
		template <class Y>
		weak_ptr& operator=(const weak_ptr<Y>& r) noexcept {
			mystl::weak_ptr<T>(r).swap(*this);
			return *this;
		}

		weak_ptr& operator=(weak_ptr&& r) noexcept {
			mystl::weak_ptr<T>(mystl::move(r)).swap(*this);
			return *this;
		}
		template <class Y>
		weak_ptr& operator=(weak_ptr<Y>&& r) noexcept {
			mystl::weak_ptr<T>(mystl::move(r)).swap(*this);
			return *this;
		}

		void reset() noexcept {
			~weak_ptr();
		}

		void swap(weak_ptr& r) noexcept {
			mystl::swap(pcb_, r.pcb_);
		}

		long use_count() const noexcept {
			if (pcb_ == nullptr) {
				return 0;
			}
			return pcb_->shared_count;
		}

		bool expried() const noexcept {
			return use_count() == 0;
		}

		mystl::shared_ptr<T> lock() const noexcept {
			return expried() ? shared_ptr<T>{} : shared_ptr<T>{ *this };
		}

		template <class Y>
		bool owner_before(const weak_ptr<Y>& other) const noexcept {
			return pcb_ < other.pcb_;
		}
		template <class Y>
		bool owner_before(const mystl::shared_ptr<Y>& other) const noexcept {
			return pcb_ < other.pcb_;
		}
	};

	template <class T>
	void swap(mystl::weak_ptr<T>& lhs, mystl::weak_ptr<T>& rhs) noexcept {
		lhs.swap(rhs);
	}
}