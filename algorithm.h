#pragma once

#include <initializer_list>
#include <iterator>
#include <vector>
#include "utility.h"

namespace mystl {
	// 不修改序列的操作

	// all_of
	// 检查一元谓词 p 是否对范围 [first, last) 中所有元素返回 true
	template <class InputIt, class UnaryPred>
	bool all_of(InputIt first, InputIt last, UnaryPred p) {
		while (first != last) {
			if (!p(*first++)) {
				return false;
			}
		}
		return true;
	}

	// any_of
	// 检查一元谓词 p 是否对范围 [first, last) 中至少一个元素返回 true
	template <class InputIt, class UnaryPred>
	bool any_of(InputIt first, InputIt last, UnaryPred p) {
		while (first != last) {
			if (p(*first++)) {
				return true;
			}
		}
		return false;
	}

	// none_of
	// 检查一元谓词 p 是否不对范围 [first, last) 中任何元素返回 true
	template <class InputIt, class UnaryPred>
	bool none_of(InputIt first, InputIt last, UnaryPred p) {
		while (first != last) {
			if (p(*first++)) {
				return false;
			}
		}
		return true;
	}

	// for_each
	// 对范围 [first, last) 中每个迭代器的解引用结果应用给定的函数对象 f，忽略 f 返回的结果
	template <class InputIt, class UnaryFunc>
	UnaryFunc for_each(InputIt first, InputIt last, UnaryFunc f) {
		while (first != last) {
			f(*first++);
		}
		return f;
	}

	// for_each_n
	// 对范围 [first, first + n) 中每个迭代器的解引用结果应用给定的函数对象 f，忽略 f 返回的结果
	template <class InputIt, class Size, class UnaryFunc>
	InputIt for_each_n(InputIt first, Size n, UnaryFunc f) {
		while (n-- > 0) {
			f(*first++);
		}
		return first;
	}

	// count
	// 返回范围 [first, last) 中等于 value 的元素数
	template <class InputIt, class T>
	typename std::iterator_traits<InputIt>::difference_type
		count(InputIt first, InputIt last, const T& value) {
		using RT = typename std::iterator_traits<InputIt>::difference_type;
		RT count = 0;
		while (first != last) {
			if (*first++ == value) {
				++count;
			}
		}
		return count;
	}

	// count_if
	// 返回范围[first, last) 中满足特定判别标准 p 的元素数
	template <class InputIt, class UnaryPred>
	typename std::iterator_traits<InputIt>::difference_type
		count_if(InputIt first, InputIt last, UnaryPred p) {
		using RT = typename std::iterator_traits<InputIt>::difference_type;
		RT count = 0;
		while (first != last) {
			if (p(*first++)) {
				++count;
			}
		}
		return count;
	}

	// mismatch
	// 返回一对到两个范围中的首个不匹配元素的迭代器
	template <class InputIt1, class InputIt2>
	std::pair<InputIt1, InputIt2>
		mismatch(InputIt1 first1, InputIt1 last1,
			InputIt2 first2) {
		while (first1 != last1) {
			if (!(*first1 == *first2)) {
				break;
			}
			++first1;
			++first2;
		}
		return { first1, first2 };
	}

	template <class InputIt1, class InputIt2, class BinaryPred>
	std::pair<InputIt1, InputIt2>
		mismatch(InputIt1 first1, InputIt1 last1,
			InputIt2 first2, BinaryPred p) {
		while (first1 != last1) {
			if (!p(*first1, *first2)) {
				break;
			}
			++first1;
			++first2;
		}
		return { first1, first2 };
	}

	template <class InputIt1, class InputIt2>
	std::pair<InputIt1, InputIt2>
		mismatch(InputIt1 first1, InputIt1 last1,
			InputIt2 first2, InputIt2 last2) {
		while (first1 != last1 && first2 != last2) {
			if (!(*first1 == *first2)) {
				break;
			}
			++first1;
			++first2;
		}
		return { first1, first2 };
	}

	template <class InputIt1, class InputIt2, class BinaryPred>
	std::pair<InputIt1, InputIt2>
		mismatch(InputIt1 first1, InputIt1 last1,
			InputIt2 first2, InputIt2 last2, BinaryPred p) {
		while (first1 != last1 && first2 != last2) {
			if (!p(*first1, *first2)) {
				break;
			}
			++first1;
			++first2;
		}
		return { first1, first2 };
	}


	// find
	// 返回指向范围 [first, last) 中等于 value 的首个元素的迭代器（没有这种元素时返回 last）
	template <class InputIt, class T>
	InputIt find(InputIt first, InputIt last, const T& value) {
		while (first != last) {
			if (*first == value) {
				return first;
			}
			++first;
		}
		return last;
	}

	// find_if
	// 返回指向范围 [first, last) 中满足特定判别标准 p 的首个元素的迭代器（没有这种元素时返回 last）
	template <class InputIt, class UnaryPred>
	InputIt find_if(InputIt first, InputIt last, UnaryPred p) {
		while (first != last) {
			if (p(*first)) {
				return first;
			}
			++first;
		}
		return last;
	}

	// find_if_not
	// 返回指向范围 [first, last) 中不满足特定判别标准 q 的首个元素的迭代器（没有这种元素时返回 last）
	template <class InputIt, class UnaryPred>
	InputIt find_if_not(InputIt first, InputIt last, UnaryPred q) {
		while (first != last) {
			if (!q(*first)) {
				return first;
			}
			++first;
		}
		return last;
	}

	template <class ForwardIt1, class ForwardIt2>
	ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
		ForwardIt2 s_first, ForwardIt2 s_last);

	// find_end
	// 在范围 [first, last) 中搜索序列 [s_first, s_last) 最后一次出现的位置
	template <class ForwardIt1, class ForwardIt2>
	ForwardIt1 find_end(ForwardIt1 first, ForwardIt1 last,
		ForwardIt2 s_first, ForwardIt2 s_last) {
		if (s_first == s_last) {
			return last;
		}
		auto result = first;
		while (true) {
			auto new_result = mystl::search(first, last, s_first, s_last);
			if (new_result == last) {
				break;
			} else {
				result = new_result;
				first = result;
				++first;
			}
		}
		return result;
	}

	template <class ForwardIt1, class ForwardIt2, class BinaryPred>
	ForwardIt1 find_end(ForwardIt1 first, ForwardIt1 last,
		ForwardIt2 s_first, ForwardIt2 s_last,
		BinaryPred p) {
		if (s_first == s_last) {
			return last;
		}
		auto result = first;
		while (true) {
			auto new_result = mystl::search(first, last, s_first, s_last, p);
			if (new_result == last) {
				break;
			} else {
				result = new_result;
				first = result;
				++first;
			}
		}
		return result;
	}

	// search
	// 搜索范围 [first, last) 中首次出现元素序列 [s_first, s_last) 的位置
	template <class ForwardIt1, class ForwardIt2>
	ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
		ForwardIt2 s_first, ForwardIt2 s_last) {
		while (true) {
			auto it = first;
			for (auto s_it = s_first; ; ++it, ++s_it) {
				if (s_it == s_last) {
					return first;
				}
				if (it == last) {
					return last;
				}
				if (!(*it == *s_it)) {
					break;
				}
			}
			++first;
		}
	}

	template <class ForwardIt1, class ForwardIt2, class BinaryPred>
	ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
		ForwardIt2 s_first, ForwardIt2 s_last,
		BinaryPred p) {
		while (true) {
			auto it = first;
			for (auto s_it = s_first; ; ++it, ++s_it) {
				if (s_it == s_last) {
					return first;
				}
				if (it == last) {
					return last;
				}
				if (!p(*it, *s_it)) {
					break;
				}
			}
			++first;
		}
	}

	// find_first_of
	// 在范围 [first, last) 中搜索范围 [s_first, s_last) 中的任何元素
	template <class InputIt, class ForwardIt>
	InputIt find_first_of(InputIt first, InputIt last,
		ForwardIt s_first, ForwardIt s_last) {
		while (first != last) {
			auto s_it = s_first;
			while (s_it != s_last) {
				if (*first == *s_it) {
					return first;
				}
				++s_it;
			}
			++first;
		}
		return last;
	}

	template <class InputIt, class ForwardIt, class BinaryPred>
	InputIt find_first_of(InputIt first, InputIt last,
		ForwardIt s_first, ForwardIt s_last,
		BinaryPred p) {
		while (first != last) {
			auto s_it = s_first;
			while (s_it != s_last) {
				if (p(*first, *s_it)) {
					return first;
				}
				++s_it;
			}
			++first;
		}
		return last;
	}

	// adjacent_find
	// 在范围 [first, last) 中搜索两个连续的相等元素
	template <class ForwardIt>
	ForwardIt adjacent_find(ForwardIt first, ForwardIt last) {
		if (first == last) {
			return last;
		}
		for (auto it = first + 1; it != last; ++it, ++first) {
			if (*it == *first) {
				return first;
			}
		}
		return last;
	}

	template <class ForwardIt, class BinaryPred>
	ForwardIt adjacent_find(ForwardIt first, ForwardIt last, BinaryPred p) {
		if (first == last) {
			return last;
		}
		for (auto it = first + 1; it != last; ++it, ++first) {
			if (p(*it, *first)) {
				return first;
			}
		}
		return last;
	}

	// search_n
	// 在范围 [first, last) 中搜索 count 个等同元素的序列，每个都等于给定的值 value
	template <class ForwardIt, class Size, class T>
	ForwardIt search_n(ForwardIt first, ForwardIt last,
		Size count, const T& value) {
		if (count <= 0) {
			return first;
		}
		for (; first != last; ++first) {
			if (!(*first == value)) {
				continue;
			}
			auto it = first;
			for (int count_cur = 1; true; ++count_cur) {
				if (count_cur>= count) {
					return it;
				}
				++first;
				if (first == last) {
					return last;
				}
				if (!(*first == value)) {
					break;
				}
			}
		}
		return last;
	}

	template <class ForwardIt, class Size, class T, class BinaryPred>
	ForwardIt search_n(ForwardIt first, ForwardIt last,
		Size count, const T& value, BinaryPred p) {
		if (count <= 0) {
			return first;
		}
		for (; first != last; ++first) {
			if (!p(*first, value)) {
				continue;
			}
			auto it = first;
			for (int count_cur = 1; true; ++count_cur) {
				if (count_cur>= count) {
					return it;
				}
				++first;
				if (first == last) {
					return last;
				}
				if (!p(*first, value)) {
					break;
				}
			}
		}
		return last;
	}

	// 修改序列的操作

	// copy
	// 复制范围 [first, last) 中的元素到从 d_first 开始的另一范围（复制目标范围）
	template <class InputIt, class OutputIt>
	OutputIt copy(InputIt first, InputIt last,
		OutputIt d_first) {
		for (; first != last; (void)++first, (void)++d_first) {
			*d_first = *first;
		}
		return d_first;
	}

	// copy_if
	// 复制范围 [first, last) 中满足 pred 的元素到从 d_first 开始的另一范围（复制目标范围）
	template <class InputIt, class OutputIt, class UnaryPred>
	OutputIt copy_if(InputIt first, InputIt last,
		OutputIt d_first, UnaryPred pred) {
		for (; first != last; (void)++first, (void)++d_first) {
			if (pred(*first))
			*d_first = *first;
		}
		return d_first;
	}

	// copy_n
	// 复制从 first 开始的范围中恰好 count 个值到从 result 开始的范围
	template <class InputIt, class Size, class OutputIt>
	OutputIt copy_n(InputIt first, Size count, OutputIt result) {
		while (count > 0) {
			*result++ = *first++;
			--count;
		}
		return result;
	}

	// copy_backward
	// 将范围[first, last) 内的元素复制到终于 d_last 的范围
	template <class BidirIt1, class BidirIt2>
	BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
		while (last != first) {
			*--d_last = *--last;
		}
		return d_last;
	}

	// move
	// 移动范围 [first, last) 中的元素到从 d_first 开始的另一范围，从 first 开始逐次到 last
	template <class InputIt, class OutputIt>
	OutputIt move(InputIt first, InputIt last,
		OutputIt d_first) {
		while (first != last) {
			*d_first = mystl::move(*first);
			++first;
			++d_first;
		}
		return d_first;
	}

	// move_backward
	// 移动来自范围 [first, last) 的元素到在 d_last 结束的另一范围。以逆序移动元素（首先复制末元素），但保持它们的相对顺序
	template <class BidirIt1, class BidirIt2>
	BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
		while (last != first) {
			*--d_last = mystl::move(*--last);
		}
		return d_last;
	}

	// fill
	// 将给定的 value 赋给 [first, last) 中的所有元素
	template <class ForwardIt, class T>
	void fill(ForwardIt first, ForwardIt last, const T& value) {
		while (first != last) {
			*first++ = value;
		}
	}

	// fill_n
	// 如果 count > 0，则将给定的 value 赋给从 first 开始的范围的前 count 个元素
	template <class OutputIt, class Size, class T>
	OutputIt fill_n(OutputIt first, Size count, const T& value) {
		while (count > 0) {
			*first++ = value;
			count--;
		}
		return first;
	}

	// transform
	// 应用给定的函数到某个/些输入范围中的元素，并将结果存储到从 d_first 开始的输出范围
	template <class InputIt, class OutputIt, class UnaryOp>
	OutputIt transform(InputIt first1, InputIt last1,
		OutputIt d_first, UnaryOp unary_op) {
		while (first1 != last1) {
			*d_first++ = unary_op(*first1++);
		}
		return d_first;
	}

	template <class InputIt1, class InputIt2,
		class OutputIt, class BinaryOp>
	OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2,
		OutputIt d_first, BinaryOp binary_op) {
		while (first1 != last1) {
			*d_first++ = binary_op(*first1++, *first2++);
		}
		return d_first;
	}

	// generate
	// 以给定函数对象 g 所生成的值对范围 [first, last) 中的每个元素赋值
	template <class ForwardIt, class Generator>
	void generate(ForwardIt first, ForwardIt last, Generator g) {
		while (first != last) {
			*first++ = g();
		}
	}

	// generate_n
	// 如果 count > 0，则将给定函数对象 g 所生成的值对始于 first 的范围的前 count 个元素赋值
	template <class OutputIt, class Size, class Generator>
	OutputIt generate_n(OutputIt first, Size count, Generator g) {
		while (count > 0) {
			*first++ = g();
			count--;
		}
		return first;
	}

	// remove
	// 从范围 [first, last) 移除所有等于 value 的元素，并返回范围新结尾的尾后迭代器
	template <class ForwardIt, class T>
	ForwardIt remove(ForwardIt first, ForwardIt last, const T& value) {
		first = mystl::find(first, last, value);
		if (first != last) {
			for (auto i = first; ++i != last;) {
				if (!(*i == value)) {
					*first++ = mystl::move(*i);
				}
			}
		}
		return first;
	}

	// remove_if
	// 从范围 [first, last) 移除所有满足特定判别标准 p 的元素，并返回范围新结尾的尾后迭代器
	template <class ForwardIt, class UnaryPred>
	ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPred p) {
		first = mystl::find_if(first, last, p);
		if (first != last) {
			for (auto i = first; ++i != last;) {
				if (!p(*i)) {
					*first++ = mystl::move(*i);
				}
			}
		}
		return first;
	}

	// remove_copy
	// 复制来自范围 [first, last) 的元素到从 d_first 开始的另一范围，省略等于 value 的元素
	template <class InputIt, class OutputIt, class T>
	OutputIt remove_copy(InputIt first, InputIt last,
		OutputIt d_first, const T& value) {
		while (first != last) {
			if (!(*first == value)) {
				*d_first++ = *first;
			} 
			++first;
		}
		return d_first;
	}

	// remove_copy_if
	// 复制来自范围 [first, last) 的元素到从 d_first 开始的另一范围，省略满足特定判别标准 p 的元素
	template <class InputIt, class OutputIt, class UnaryPred>
	OutputIt remove_copy_if(InputIt first, InputIt last,
		OutputIt d_first, UnaryPred p) {
		while (first != last) {
			if (!p(*first)) {
				*d_first++ = *first;
			}
			++first;
		}
		return d_first;
	}

	// replace
	// 以 new_value 替换范围 [first, last) 中所有等于 old_value 的元素
	template <class ForwardIt, class T>
	void replace(ForwardIt first, ForwardIt last,
		const T& old_value, const T& new_value) {
		while (first != last) {
			if (*first == old_value) {
				*first = new_value;
			}
			++first;
		}
	}

	// replace_if
	// 以 new_value 替换范围 [first, last) 中所有满足特定判别标准 p 的元素
	template <class ForwardIt, class UnaryPred, class T>
	void replace_if(ForwardIt first, ForwardIt last,
		UnaryPred p, const T& new_value) {
		while (first != last) {
			if (p(*first)) {
				*first = new_value;
			}
			++first;
		}
	}

	// replace_copy
	// 复制来自范围 [first, last) 的元素到始于 d_first 的范围，复制过程中以 new_value 替换所有等于 old_value 的元素
	template <class InputIt, class OutputIt, class T>
	OutputIt replace_copy(InputIt first, InputIt last, OutputIt d_first,
		const T& old_value, const T& new_value) {
		while (first != last) {
			if (*first == old_value) {
				*d_first++ = new_value;
			} else {
				*d_first++ = *first;
			}
			++first;
		}
		return d_first;
	}

	// replace_copy_if
	// 复制来自范围 [first, last) 的元素到始于 d_first 的范围，复制过程中以 new_value 替换所有等于 old_value 的元素
	template <class InputIt, class OutputIt, class UnaryPred, class T>
	OutputIt replace_copy_if
	(InputIt first, InputIt last, OutputIt d_first,
		UnaryPred p, const T& new_value) {
		while (first != last) {
			if (p(*first)) {
				*d_first++ = new_value;
			}
			else {
				*d_first++ = *first;
			}
			++first;
		}
		return d_first;
	}

	// swap
	// 见 utility.h

	// swap_ranges
	// 在范围 [first1, last1) 和从 first2 开始的包含 std::distance(first1, last1) 个元素的另一范围间交换元素
	template <class ForwardIt1, class ForwardIt2>
	ForwardIt2 swap_ranges(ForwardIt1 first1, ForwardIt1 last1,
		ForwardIt2 first2) {
		while (first1 != last1) {
			mystl::swap(*first1++, *first2++);
		}
		return first2;
	}

	// iter_swap
	// 交换给定的迭代器所指向的元素的值
	template <class ForwardIt1, class ForwardIt2>
	void iter_swap(ForwardIt1 a, ForwardIt2 b) {
		mystl::swap(*a, *b);
	}

	// reverse
	// 反转 [first, last) 范围中的元素顺序
	template <class BidirIt>
	void reverse(BidirIt first, BidirIt last) {
		auto n = std::distance(first, last) / 2;
		last--;
		while (n > 0) {
			mystl::iter_swap(first++, last--);
			--n;
		}
	}

	// reverse_copy
	// 将范围 [first, last)（源范围）中的元素复制到从 d_first 开始的新范围（目标范围），使得目标范围中元素以逆序排列
	template<class BidirIt, class OutputIt>
	OutputIt reverse_copy(BidirIt first, BidirIt last, OutputIt d_first) {
		while (first != last) {
			*d_first++ = *(--last);
		}
		return d_first;
	}

	// rotate
	// 进行元素范围上的左旋转
	template <class ForwardIt>
	ForwardIt rotate(ForwardIt first, ForwardIt middle, ForwardIt last) {
		if (first == middle) {
			return last;
		}
		if (middle == last) {
			return first;
		}
		auto it = middle;
		while (first != middle && it != last) {
			mystl::iter_swap(first++, it++);
		}
		if (it == last) {
			if (first != middle) {
				mystl::rotate(first, middle, last);
				return first;
			}
		}
		if (first == middle) {
			if (it != last) {
				mystl::rotate(middle, it, last);
				return middle;
			}
		}
		return middle;
	}

	// rotate_copy
	// 从范围 [first, last) 复制元素到始于 d_first 的另一范围，使得 *n_first 成为新范围的首元素，而 *(n_first - 1) 成为末元素
	template <class ForwardIt, class OutputIt>
	OutputIt rotate_copy(ForwardIt first, ForwardIt n_first,
		ForwardIt last, OutputIt d_first) {
		auto mid = n_first;
		while (mid != last) {
			*d_first++ = *mid++;
		}
		while (first != n_first) {
			*d_first++ = *first++;
		}
		return d_first;
	}

	// unique
	// 从范围 [first, last) 移除相继等价元素组中首元素以外的所有元素，并返回范围新结尾的尾后迭代器
	template <class ForwardIt>
	ForwardIt unique(ForwardIt first, ForwardIt last) {
		if (first == last) {
			return last;
		}
		auto it = first + 1;
		while ( it != last) {
			if (!(*first == *it)) {
				*++first = mystl::move(*it++);
				continue;
			}
			while (it != last && *first == *it) {
				++it;
			}
			if (it == last) {
				return ++first;
			}
			*++first = mystl::move(*it++);
		}
		return ++first;
	}

	template <class ForwardIt, class BinaryPred>
	ForwardIt unique(ForwardIt first, ForwardIt last, BinaryPred p) {
		if (first == last) {
			return last;
		}
		auto it = first + 1;
		while (it != last) {
			if (!p(*first, *it)) {
				*++first = mystl::move(*it++);
				continue;
			}
			while (it != last && p(*first, *it)) {
				++it;
			}
			if (it == last) {
				return ++first;
			}
			*++first = mystl::move(*it++);
		}
		return ++first;
	}

	// unique_copy
	// 从范围 [first, last) 复制元素到从 d_first 开始的另一范围，使得目标范围不存在连续的相等元素
	template <class InputIt, class OutputIt>
	OutputIt unique_copy(InputIt first, InputIt last, OutputIt d_first) {
		if (first == last) {
			return d_first;
		}
		*d_first = *first++;
		while (first != last) {
			if (!(*first == *d_first)) {
				*++d_first = *first;
			}
			++first;
		}
		return d_first;
	}

	template <class InputIt, class OutputIt, class BinaryPred>
	OutputIt unique_copy(InputIt first, InputIt last,
		OutputIt d_first, BinaryPred p) {
		if (first == last) {
			return d_first;
		}
		*d_first = *first++;
		while (first != last) {
			if (!p(*first, *d_first)) {
				*++d_first = *first;
			}
			++first;
		}
		return d_first;
	}

	// 划分操作

	// is_partitioned
	// 检查范围 [first, last) 是否已按谓词 p 划分：所有满足 p 的元素都会在所有不满足的元素之前出现
	template <class InputIt, class UnaryPred>
	bool is_partitioned(InputIt first, InputIt last, UnaryPred p) {
		if (first == last) {
			return true;
		}
		if (p(*first)) {
			while (first != last && p(*first)) {
				++first;
			}
			if (first == last) {
				return true;
			}
			while (first != last) {
				if (p(*first)) {
					return false;
				}
				++first;
			}
			return true;
		} else {
			while (first != last) {
				if (p(*first)) {
					return false;
				}
				++first;
			}
			return true;
		}
	}

	// partition
	// 重排序范围 [first, last) 中的元素，使得谓词 p 对其返回 true 的所有元素位于谓词 p 对其返回 false 的所有元素之前
	// 不保持元素的相对顺序
	template <class ForwardIt, class UnaryPred>
	ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPred p) {
		while (true) {
			while (first != last && p(*first)) {
				++first;
			}
			--last;
			while (last != first && !p(*last)) {
				--last;
			}
			if (first == last) {
				return first;
			}
			mystl::iter_swap(first, last);
		}
	}

	// partition_copy
	// 根据谓词 p 的返回值，将范围 [first, last) 中的元素复制到两个不同范围
	template <class InputIt, class OutputIt1,
		class OutputIt2, class UnaryPred>
	std::pair<OutputIt1, OutputIt2>
		partition_copy(InputIt first, InputIt last,
			OutputIt1 d_first_true, OutputIt2 d_first_false,
			UnaryPred p) {
		while (first != last) {
			if (p(*first)) {
				*d_first_true++ = *first++;
			} else {
				*d_first_false++ = *first++;
			}
		}
		return { d_first_true, d_first_false };
	}

	// stable_partition
	// 重排序范围 [first, last) 中的元素，使得所有谓词 p 对其返回 true 的元素均先于谓词 p 对其返回 false 的元素
	// 保持元素的相对顺序
	template <class BidirIt, class UnaryPred>
	BidirIt stable_partition(BidirIt first, BidirIt last, UnaryPred p) {
		first = mystl::find_if_not(first, last, p);
		auto it = first;
		while (it != last) {
			if (p(*it)) {
				while (it != first) {
					mystl::iter_swap(it, it - 1);
					--it;
				}
			}
			++it;
			while (p(*first)) {
				++first;
				it = first;
			}
		}
		return first;
	}

	// partition_point
	// 检验已划分范围 [first, last)，并定位第一分段的结尾，即首个不满足 p 的元素，或者在所有元素满足 p 时是 last
	template <class ForwardIt, class UnaryPred>
	ForwardIt partition_point(ForwardIt first, ForwardIt last, UnaryPred p) {
		while (first != last) {
			if (!p(*first)) {
				return first;
			}
			++first;
		}
		return last;
	}

	// 堆操作

	// is_heap
	// 检查范围 [first, last) 是否为堆
	template <class RandomIt>
	bool is_heap(RandomIt first, RandomIt last) {
		auto n = last - first;
		for (decltype(n) child = 1; child < n; ++child) {
			auto parent = (child - 1) / 2;
			if (*(first + parent) < *(first + child)) {
				return false;
			}
		}
		return true;
	}

	template <class RandomIt, class Compare>
	bool is_heap(RandomIt first, RandomIt last, Compare comp) {
		auto n = last - first;
		for (decltype(n) child = 1; child < n; ++child) {
			auto parent = (child - 1) / 2;
			if (comp(*(first + parent), *(first + child))) {
				return false;
			}
		}
		return true;
	}

	// is_heap_until
	// 检查范围 [first, last) 并寻找从 first 开始的满足堆要求的最大范围
	template <class RandomIt>
	RandomIt is_heap_until(RandomIt first, RandomIt last) {
		auto n = last - first;
		for (decltype(n) child = 1; child < n; ++child) {
			auto parent = (child - 1) / 2;
			if (*(first + parent) < *(first + child)) {
				return first + child;
			}
		}
		return last;
	}

	template <class RandomIt, class Compare>
	RandomIt is_heap_until(RandomIt first, RandomIt last, Compare comp) {
		auto n = last - first;
		for (decltype(n) child = 1; child < n; ++child) {
			auto parent = (child - 1) / 2;
			if (comp(*(first + parent), *(first + child))) {
				return first + child;
			}
		}
		return last;
	}

	// up_heap
	// 堆上浮
	template <class RandomIt>
	void up_heap(RandomIt first, RandomIt index) {
		auto child = index - first;
		auto parent = (child - 1) / 2;
		while (child > 0) {
			if (*(first + parent) < *(first + child)) {
				mystl::iter_swap(first + child, first + parent);
				child = parent;
				parent = (child - 1) / 2;
			} else {
				break;
			}
		}
	}

	template <class RandomIt, class Compare>
	void up_heap(RandomIt first, RandomIt index, Compare comp) {
		auto child = index - first;
		auto parent = (child - 1) / 2;
		while (child > 0) {
			if (comp(*(first + parent), *(first + child))) {
				mystl::iter_swap(first + child, first + parent);
				child = parent;
				parent = (child - 1) / 2;
			} else {
				break;
			}
		}
	}

	// down_heap
	// 堆下沉
	template <class RandomIt>
	void down_heap(RandomIt first, RandomIt index, RandomIt last) {
		auto n = last - first;
		auto parent = index - first;
		auto child = parent * 2 + 1;
		while (child < n) {
			if (child + 1 < n && *(first + child) < *(first + child + 1)) {
				++child;
			}
			if (*(first + parent) < *(first + child)) {
				mystl::iter_swap(first + child, first + parent);
				parent = child;
				child = parent * 2 + 1;
			} else {
				break;
			}
		}
	}

	template <class RandomIt, class Compare>
	void down_heap(RandomIt first, RandomIt index, RandomIt last, Compare comp) {
		auto n = last - first;
		auto parent = index - first;
		auto child = parent * 2 + 1;
		while (child < n) {
			if (child + 1 < n && comp(*(first + child), *(first + child + 1))) {
				++child;
			}
			if (comp(*(first + parent), *(first + child))) {
				mystl::iter_swap(first + child, first + parent);
				parent = child;
				child = parent * 2 + 1;
			} else {
				break;
			}
		}
	}

	// make_heap
	// 在范围 [first, last) 中构造堆
	template <class RandomIt>
	void make_heap(RandomIt first, RandomIt last) {
		auto n = last - first;
		for (auto i = (n - 1 - 1) / 2; i>= 0; --i) {
			mystl::down_heap(first, first + i, last);
		}
	}

	template <class RandomIt, class Compare>
	void make_heap(RandomIt first, RandomIt last, Compare comp) {
		auto n = last - first;
		for (auto i = (n - 1 - 1) / 2; i>= 0; --i) {
			mystl::down_heap(first, first + i, last, comp);
		}
	}

	// push_heap
	// 将位于位置 last - 1 的元素插入到堆 [first, last - 1) 中
	template <class RandomIt>
	void push_heap(RandomIt first, RandomIt last) {
		mystl::up_heap(first, last - 1);
	}

	template <class RandomIt, class Compare>
	void push_heap(RandomIt first, RandomIt last, Compare comp) {
		mystl::up_heap(first, last - 1, comp);
	}

	// pop_heap
	// 交换在位置 first 的值和在位置 last - 1 的值，并使得子范围 [first, last - 1) 变为堆
	template <class RandomIt>
	void pop_heap(RandomIt first, RandomIt last) {
		mystl::iter_swap(first, last - 1);
		mystl::down_heap(first, first, last - 1);
	}

	template <class RandomIt, class Compare>
	void pop_heap(RandomIt first, RandomIt last, Compare comp) {
		mystl::iter_swap(first, last - 1);
		mystl::down_heap(first, first, last - 1, comp);
	}

	// sort_heap
	// 将堆 [first, last) 转换为有序范围，不再维持堆的性质
	template <class RandomIt>
	void sort_heap(RandomIt first, RandomIt last) {
		auto n = last - first;
		while (n > 0) {
			mystl::iter_swap(first, first + n - 1);
			mystl::down_heap(first, first, first + n - 1);
			--n;
		}
	}

	template <class RandomIt, class Compare>
	void sort_heap(RandomIt first, RandomIt last, Compare comp) {
		auto n = last - first;
		while (n > 0) {
			mystl::iter_swap(first, first + n - 1);
			mystl::down_heap(first, first, first + n - 1, comp);
			--n;
		}
	}

	// 排序操作

	// is_sorted
	// 检查范围 [first, last) 中的元素是否以非降序排序
	template <class ForwardIt>
	bool is_sorted(ForwardIt first, ForwardIt last) {
		if (first == last) {
			return true;
		}
		for (auto it = first + 1; it != last; ++it, ++first) {
			if (*it < *first) {
				return false;
			}
		}
		return true;
	}

	template <class ForwardIt, class Compare>
	bool is_sorted(ForwardIt first, ForwardIt last, Compare comp) {
		if (first == last) {
			return true;
		}
		for (auto it = first + 1; it != last; ++it, ++first) {
			if (comp(*it, *first)) {
				return false;
			}
		}
		return true;
	}

	// is_sorted_until
	// 检验范围 [first, last)，并寻找从 first 开始且其中元素已按非降序排序的最大范围
	template <class ForwardIt>
	ForwardIt is_sorted_until(ForwardIt first, ForwardIt last) {
		if (first == last) {
			return last;
		}
		for (auto it = first; it + 1 != last; ++it) {
			if (*(it + 1) < *it) {
				return it;
			}
		}
		return last;
	}

	template <class ForwardIt, class Compare>
	ForwardIt is_sorted_until(ForwardIt first, ForwardIt last,
		Compare comp) {
		if (first == last) {
			return last;
		}
		for (auto it = first; it + 1 != last; ++it) {
			if (comp(*(it + 1), *it)) {
				return it;
			}
		}
		return last;
	}

	// partitial_sort
	// 重排元素，使得范围 [first, middle) 含有范围 [first, last) 中已排序的 middle - first 个最小元素
	// 不保证保持相等元素间的顺序、未指定范围 [middle, last) 中剩余元素的顺序
	template <class RandomIt>
	void partial_sort(RandomIt first, RandomIt middle, RandomIt last) {
		mystl::make_heap(first, middle);
		auto n = last - first;
		for (auto i = middle - first; i < n; ++i) {
			if (*(first + i) < *(first)) {
				mystl::iter_swap(first + i, first);
				mystl::down_heap(first, first, middle);
			}
		}
		mystl::sort_heap(first, middle);
	}

	template <class RandomIt, class Compare>
	void partial_sort(RandomIt first, RandomIt middle, RandomIt last, Compare comp) {
		mystl::make_heap(first, middle, comp);
		auto n = last - first;
		for (auto i = middle - first; i < n; ++i) {
			if (comp(*(first + i), *(first))) {
				mystl::iter_swap(first + i, first);
				mystl::down_heap(first, first, middle, comp);
			}
		}
		mystl::sort_heap(first, middle, comp);
	}

	// partitial_sort_copy
	// 以升序排序范围 [first, last) 中的某些元素，存储结果于范围 [d_first, d_last)
	template <class InputIt, class RandomIt>
	RandomIt partial_sort_copy(InputIt first, InputIt last,
		RandomIt d_first, RandomIt d_last) {
		auto m = d_last - d_first;
		auto n = last - first;
		auto i = 0;
		while (i < m && i < n) {
			*(d_first + i) = *(first + i);
			++i;
		}
		mystl::make_heap(d_first, d_first + i);
		if (i == m) {
			while (m < n) {
				if (*(first + m) < *(d_first)) {
					*d_first = *(first + m);
					mystl::down_heap(d_first, d_first, d_first + i);
				}
				++m;
			}
		}
		mystl::sort_heap(d_first, d_first + i);
		return d_last;
	}

	template <class InputIt, class RandomIt, class Compare>
	RandomIt partial_sort_copy(InputIt first, InputIt last,
		RandomIt d_first, RandomIt d_last, Compare comp) {
		auto m = d_last - d_first;
		auto n = last - first;
		auto i = first - first;
		while (i < m && i < n) {
			*(d_first + i) = *(first + i);
			++i;
		}
		mystl::make_heap(d_first, d_first + i, comp);
		if (i == m) {
			while (m < n) {
				if (comp(*(first + m), *(d_first))) {
					*d_first = *(first + m);
					mystl::down_heap(d_first, d_first, d_first + i, comp);
				}
				++m;
			}
		}
		mystl::sort_heap(d_first, d_first + i, comp);
		return d_first + i;
	}

	// merge_sort
	// 归并排序
	template <class RandomIt, class T>
	void merge_sort(RandomIt first, RandomIt last, std::vector<T>& v) {
		if (last - first <= 1) {
			return;
		}
		auto n = last - first;
		auto mid = n / 2;
		mystl::merge_sort(first, first + mid, v);
		mystl::merge_sort(first + mid, last, v);

		auto i = first - first;
		auto j = mid;
		auto k = i;
		while (i < mid && j < n) {
			if (!(*(first + j) < *(first + i))) {
				v[k++] = *(first + i);
				++i;
			}
			else {
				v[k++] = *(first + j);
				++j;
			}
		}
		while (i < mid) {
			v[k++] = *(first + i);
			i++;
		}
		while (j < n) {
			v[k++] = *(first + j);
			++j;
		}
		for (i = 0; i < n; ++i) {
			*(first + i) = v[i];
		}
	}

	template <class RandomIt, class T, class Compare>
	void merge_sort(RandomIt first, RandomIt last, std::vector<T>& v, Compare comp) {
		if (last - first <= 1) {
			return;
		}
		auto n = last - first;
		auto mid = n / 2;
		mystl::merge_sort(first, first + mid, v, comp);
		mystl::merge_sort(first + mid, last, v, comp);

		auto i = first - first;
		auto j = mid;
		auto k = i;
		while (i < mid && j < n) {
			if (!comp(*(first + j), *(first + i))) {
				v[k++] = *(first + i);
				++i;
			}
			else {
				v[k++] = *(first + j);
				++j;
			}
		}
		while (i < mid) {
			v[k++] = *(first + i);
			i++;
		}
		while (j < n) {
			v[k++] = *(first + j);
			++j;
		}
		for (i = 0; i < n; ++i) {
			*(first + i) = v[i];
		}
	}

	// stable_sort
	// 以非降序排序范围 [first, last) 中的元素
	// 保证保持等价元素间的顺序
	template <class RandomIt>
	void stable_sort(RandomIt first, RandomIt last) {
		using Ty = std::remove_reference_t<decltype(*first)>;
		std::vector<Ty> v(last - first);
		mystl::merge_sort(first, last, v);
	}

	template <class RandomIt, class Compare>
	void stable_sort(RandomIt first, RandomIt last, Compare comp) {
		using Ty = std::remove_reference_t<decltype(*first)>;
		std::vector<Ty> v(last - first);
		mystl::merge_sort(first, last, v, comp);
	}

	// median
	// 三数取中
	template <class T>
	T median(const T& left, const T& mid, const T& right) {
		if (left < mid) {
			if (right < left) {
				return left;
			}
			else if (mid < right) {
				return mid;
			}
			else {
				return right;
			}
		}
		else {
			if (right < mid) {
				return mid;
			}
			else if (left < right) {
				return left;
			}
			else {
				return right;
			}
		}
	}

	template <class T, class Compare>
	T median(const T& left, const T& mid, const T& right, Compare comp) {
		if (comp(left, mid)) {
			if (comp(right, left)) {
				return left;
			}
			else if (comp(mid, right)) {
				return mid;
			}
			else {
				return right;
			}
		}
		else {
			if (comp(right, mid)) {
				return mid;
			}
			else if (comp(left, right)) {
				return left;
			}
			else {
				return right;
			}
		}
	}

	// insertion_sort
	// 插入排序
	template <class RandomIt>
	void insertion_sort(RandomIt first, RandomIt last) {
		auto n = last - first;
		for (auto i = first - first; i < n; ++i) {
			auto value = *(first + i);
			auto j = i - 1;
			for (; j>= 0 && value < *(first + j); --j) {
				*(first + 1 + j) = *(first + j);
			}
			*(first + 1 + j) = value;
		}
	}

	template <class RandomIt, class Compare>
	void insertion_sort(RandomIt first, RandomIt last, Compare comp) {
		auto n = last - first;
		for (auto i = first - first; i < n; ++i) {
			auto value = *(first + i);
			auto j = i - 1;
			for (; j>= 0 && comp(value, *(first + j)); --j) {
				*(first + 1 + j) = *(first + j);
			}
			*(first + 1 + j) = value;
		}
	}

	// random_partition
	// partition 随机迭代器版本
	template <class RandomIt, class T>
	RandomIt random_partition(RandomIt first, RandomIt last, const T& value) {
		auto l = first - first;
		auto r = last - first;
		while (true) {
			while (*(first + l) < value) {
				++l;
			}
			--r;
			while (value < *(first + r)) {
				--r;
			}
			if (!(l < r)) {
				return first + l;
			}
			mystl::iter_swap(first + l, first + r);
			++l;
		}
	}

	template <class RandomIt, class T, class Compare>
	RandomIt random_partition(RandomIt first, RandomIt last, const T& value, Compare comp) {
		auto l = first - first;
		auto r = last - first;
		while (true) {
			while (comp(*(first + l), value)) {
				++l;
			}
			--r;
			while (comp(value, *(first + r))) {
				--r;
			}
			if (!(l < r)) {
				return first + l;
			}
			mystl::iter_swap(first + l, first + r);
			++l;
		}
	}

	// nth_emlement
	// nth_element 会重排 [first, last) 中的元素，使得在重排后
	// nth 指向的元素被更改为假如 [first, last) 已排序则该位置会出现的元素
	template <class RandomIt>
	void nth_element(RandomIt first, RandomIt nth, RandomIt last) {
		auto l = first - first;
		auto r = last - first;
		while (r - l > 3) {
			auto cur = mystl::random_partition(first + l, first + r, 
				mystl::median(*(first + l), *(first + r - 1), *(first + (r - l) / 2)));
			if (cur - first <= nth - first) {
				l = cur - first;
			} else {
				r = cur - first;
			}
		}
		mystl::insertion_sort(first + l, first + r);
	}

	template <class RandomIt, class Compare>
	void nth_element(RandomIt first, RandomIt nth, RandomIt last,
		Compare comp) {
		auto l = first - first;
		auto r = last - first;
		while (r - l > 3) {
			auto cur = mystl::random_partition(first + l, first + r,
				mystl::median(*(first + l), *(first + r - 1), *(first + (r - l) / 2)), comp);
			if (cur - first <= nth - first) {
				l = cur - first;
			}
			else {
				r = cur - first;
			}
		}
		mystl::insertion_sort(first + l, first + r, comp);
	}

	// quick_sort
	// 快速排序
	template <class RandomIt>
	void quick_sort(RandomIt first, RandomIt last, int depth) {
		if (last - first <= 1) {
			return;
		}
		if (last - first < 8) {
			mystl::insertion_sort(first, last);
			return;
		}
		if (depth > 64) {
			mystl::make_heap(first, last);
			mystl::sort_heap(first, last);
			return;
		}
		
		auto l = first - first;
		auto r = last - first;
		auto mid = mystl::random_partition(first + l, first + r,
			mystl::median(*(first + l), *(first + r - 1), *(first + (r - l) / 2)));
		++depth;
		mystl::quick_sort(first + l, mid, depth);
		mystl::quick_sort(mid, first + r, depth);
	}

	template <class RandomIt, class Compare>
	void quick_sort(RandomIt first, RandomIt last, int depth, Compare comp) {
		if (last - first <= 1) {
			return;
		}
		if (last - first < 8) {
			mystl::insertion_sort(first, last, comp);
			return;
		}
		if (depth > 64) {
			mystl::make_heap(first, last, comp);
			mystl::sort_heap(first, last, comp);
			return;
		}

		auto l = first - first;
		auto r = last - first;
		auto mid = mystl::random_partition(first + l, first + r,
			mystl::median(*(first + l), *(first + r - 1), *(first + (r - l) / 2)), comp);
		++depth;
		mystl::quick_sort(first + l, mid, depth, comp);
		mystl::quick_sort(mid, first + r, depth, comp);
	}

	// sort
	// 以非降序排序范围 [first, last) 中的元素
	// 不保证维持相等元素的顺序
	template <class RandomIt>
	void sort(RandomIt first, RandomIt last) {
		mystl::quick_sort(first, last, 0);
	}

	template <class RandomIt, class Compare>
	void sort(RandomIt first, RandomIt last, Compare comp) {
		mystl::quick_sort(first, last, 0, comp);
	}

	// （有序范围上的）二分搜索操作

	// lower_bound
	// 在已划分的范围 [first, last) 中查找第一个不先序于 value 的元素
	template <class ForwardIt, class T>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last,
		const T& value) {
		ForwardIt it;
		typename std::iterator_traits<ForwardIt>::difference_type count, step;
		count = std::distance(first, last);
		while (count > 0) {
			it = first;
			step = count / 2;
			std::advance(it, step);
			if (*it < value) {
				first = ++it;
				count -= step + 1;
			} else {
				count = step;
			}
		}
		return first;
	}

	template <class ForwardIt, class T, class Compare>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last,
		const T & value, Compare comp) {
		ForwardIt it;
		typename std::iterator_traits<ForwardIt>::difference_type count, step;
		count = std::distance(first, last);
		while (count > 0) {
			it = first;
			step = count / 2;
			std::advance(it, step);
			if (comp(*it, value)) {
				first = ++it;
				count -= step + 1;
			} else {
				count = step;
			}
		}
		return first;
	}
	// upper_bound
	// 在已划分的范围 [first, last) 中查找第一个后序于 value 的元素
	template <class ForwardIt, class T>
	ForwardIt upper_bound(ForwardIt first, ForwardIt last,
		const T & value) {
		ForwardIt it;
		typename std::iterator_traits<ForwardIt>::difference_type count, step;
		count = std::distance(first, last);
		while (count > 0) {
			it = first;
			step = count / 2;
			std::advance(it, step);
			if (value < *it) {
				first = ++it;
				count -= step + 1;
			} else {
				count = step;
			}
		}	
		return first;
	}

	template <class ForwardIt, class T, class Compare>
	ForwardIt upper_bound(ForwardIt first, ForwardIt last,
		const T& value, Compare comp) {
		ForwardIt it;
		typename std::iterator_traits<ForwardIt>::difference_type count, step;
		count = std::distance(first, last);
		while (count > 0) {
			it = first;
			step = count / 2;
			std::advance(it, step);
			if (comp(value, *it)) {
				first = ++it;
				count -= step + 1;
			} else {
				count = step;
			}
		}
		return first;
	}

	// binary_search
	// 检查在已划分范围 [first, last) 中是否出现与 value 等价的元素
	template <class ForwardIt, class T>
	bool binary_search(ForwardIt first, ForwardIt last,
		const T& value) {
		first = mystl::lower_bound(first, last, value);
		return (!(first == last) && !(value < *first));
	}
	template<class ForwardIt, class T, class Compare>
	bool binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
		first = mystl::lower_bound(first, last, value, comp);
		return (!(first == last) and !(comp(value, *first)));
	}

	// equal_range
	// 返回范围 [first, last) 中包含所有等价于 value 的元素的范围
	template <class ForwardIt, class T>
	std::pair<ForwardIt, ForwardIt>
		equal_range(ForwardIt first, ForwardIt last, const T& value) {
		return { mystl::lower_bound(first, last, value), mystl::upper_bound(first, last, value) };
	}

	template <class ForwardIt, class T, class Compare>
	std::pair<ForwardIt, ForwardIt>
		equal_range(ForwardIt first, ForwardIt last,
			const T& value, Compare comp) {
		return { mystl::lower_bound(first, last, value, comp),
			mystl::upper_bound(first, last, value, comp) };
	}

	// 其他有序范围上的操作
	// 将两个有序范围 [first1, last1) 和 [first2, last2) 合并到始于 d_first 的一个有序范围中
	template <class InputIt1, class InputIt2, class OutputIt>
	OutputIt merge(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 <= *first2) {
				*d_first++ = *first1++;
			} else {
				*d_first++ = *first2++;
			}
		}
		while (first1 != last1) {
			*d_first++ = *first1++;
		}
		while (first2 != last2) {
			*d_first++ = *first2++;
		}
		return d_first;
	}

	template <class InputIt1, class InputIt2,
		class OutputIt, class Compare>
	OutputIt merge(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first, Compare comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first1, *first2)) {
				*d_first++ = *first1++;
			}
			else {
				*d_first++ = *first2++;
			}
		}
		while (first1 != last1) {
			*d_first++ = *first1++;
		}
		while (first2 != last2) {
			*d_first++ = *first2++;
		}
		return d_first;
	}

	// inplace_merge
	// 将两个相继的有序范围 [first, middle) 和 [middle, last) 归并为一个有序范围 [first, last)
	template <class BidirIt>
	void inplace_merge(BidirIt first, BidirIt middle, BidirIt last) {
		using Ty = std::remove_reference_t<decltype(*first)>;
		std::vector<Ty> tmp(std::distance(first, last));
		auto it_1 = first;
		auto it_2 = middle;
		int index = 0;
		while (it_1 != middle && it_2 != last) {
			if (*it_1 <= *it_2) {
				tmp[index++] = *it_1++;
			} else {
				tmp[index++] = *it_2++;
			}
		}
		while (it_1 != middle) {
			tmp[index++] = *it_1++;
		}
		while (it_2 != last) {
			tmp[index++] = *it_2++;
		}
		index = 0;
		while (first != last) {
			*first++ = tmp[index++];
		}
	}

	template <class BidirIt, class Compare>
	void inplace_merge(BidirIt first, BidirIt middle, BidirIt last, Compare comp) {
		using Ty = std::remove_reference_t<decltype(*first)>;
		std::vector<Ty> tmp(std::distance(first, last));
		auto it_1 = first;
		auto it_2 = middle;
		int index = 0;
		while (it_1 != middle && it_2 != last) {
			if (comp(*it_1, *it_2)) {
				tmp[index++] = *it_1++;
			}
			else {
				tmp[index++] = *it_2++;
			}
		}
		while (it_1 != middle) {
			tmp[index++] = *it_1++;
		}
		while (it_2 != last) {
			tmp[index++] = *it_2++;
		}
		index = 0;
		while (first != last) {
			*first++ = tmp[index++];
		}
	}

	// （有序范围上的）集合操作
	
	// includes
	// 在有序范围 [first2, last2) 是有序范围 [first1, last1) 的子序列的情况下返回 true（不必是连续的子序列）
	template <class InputIt1, class InputIt2>
	bool includes(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2) {
		for (; first2 != last2; ++first1) {
			if (first1 == last1 || *first2 < *first1) {
				return false;
			}
			if (!(*first1 < *first2)) {
				++first2;
			}
		}
		return true;
	}

	template <class InputIt1, class InputIt2, class Compare>
	bool includes(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2, Compare comp) {
		for (; first2 != last2; ++first1) {
			if (first1 == last1 || comp(*first2, *first1)) {
				return false;
			}
			if (!comp(*first1, *first2)) {
				++first2;
			}
		}
		return true;
	}

	// set_difference
	// 复制来自有序范围 [first1, last1) 并且在有序范围 [first2, last2) 中未能找到的元素到始于 d_first 的范围
	// 输出范围也保持有序
	template <class InputIt1, class InputIt2, class OutputIt>
	OutputIt set_difference(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first) {
		while (first2 != last2 && first1 != last1) {
			if (*first1 < *first2) {
				*d_first++ = *first1++;
			} else {
				if (!(*first2 < *first1)) {
					++first1;
				}
				++first2;
			}
		}
		while (first1 != last1) {
			*d_first++ = *first1++;
		}
		return d_first;
	}

	template <class InputIt1, class InputIt2,
		class OutputIt, class Compare>
	OutputIt set_difference(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first, Compare comp) {
		while (first2 != last2 && first1 != last1) {
			if (comp(*first1, *first2)) {
				*d_first++ = *first1++;
			} else {
				if (!comp(*first2, *first1)) {
					++first1;
				}
				++first2;
			}
		}
		while (first1 != last1) {
			*d_first++ = *first1++;
		}
		return d_first;
	}

	// set_intersection
	// 构造始于 d_first，由在两个有序范围[first1, last1) 与[first2, last2) 中都找到的元素构成的有序范围
	template <class InputIt1, class InputIt2, class OutputIt>
	OutputIt set_intersection(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2) {
				++first1;
			} else {
				if (*first2 < *first1) {
					++first2;
				} else {
					*d_first++ = *first1;
					++first1;
					++first2;
				}
			}
		}
		return d_first;
	}

	template <class InputIt1, class InputIt2, class OutputIt, class Compare>
	OutputIt set_intersection(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first, Compare comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first1, *first2)) {
				++first1;
			}
			else {
				if (comp(*first2, *first1)) {
					++first2;
				}
				else {
					*d_first++ = *first1;
					++first1;
					++first2;
				}
			}
		}
		return d_first;
	}

	// set_symmetric_difference
	// 计算两个有序范围的对称差：将处于任一范围中，但未在两个范围中均被找到的元素，复制到始于 d_first 的范围
	// 输出范围也保持有序
	template <class InputIt1, class InputIt2, class OutputIt>
	OutputIt set_symmetric_difference
	(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2) {
				*d_first++ = *first1++;
			} else {
				if (*first2 < *first1) {
					*d_first++ = *first2++;
				} else {
					++first1;
					++first2;
				}
			}
		}
		while (first1 != last1) {
			*d_first++ = *first1++;
		}
		while (first2 != last2) {
			*d_first++ = *first2++;
		}
		return d_first;
	}

	template <class InputIt1, class InputIt2,
		class OutputIt, class Compare>
	OutputIt set_symmetric_difference
	(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first, Compare comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first1, *first2)) {
				*d_first++ = *first1++;
			}
			else {
				if (comp(*first2, *first1)) {
					*d_first++ = *first2++;
				}
				else {
					++first1;
					++first2;
				}
			}
		}
		while (first1 != last1) {
			*d_first++ = *first1++;
		}
		while (first2 != last2) {
			*d_first++ = *first2++;
		}
		return d_first;
	}

	// set_union
	// 构造从 d_first 开始的有序并集，由存在于有序范围 [first1, last1) 和 [first2, last2) 之一或二者中的所有元素构成
	template <class InputIt1, class InputIt2, class OutputIt>
	OutputIt set_union(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2) {
				*d_first++ = *first1++;
			} else {
				if (*first2 < *first1) {
					*d_first++ = *first2++;
				} else {
					*d_first++ = *first1;
					++first1;
					++first2;
				}
			}
		}
		while (first1 != last1) {
			*d_first++ = *first1++;
		}
		while (first2 != last2) {
			*d_first++ = *first2++;
		}
		return d_first;
	}

	template <class InputIt1, class InputIt2,
		class OutputIt, class Compare>
	OutputIt set_union(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first, Compare comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first1, *first2)) {
				*d_first++ = *first1++;
			}
			else {
				if (comp(*first2, *first1)) {
					*d_first++ = *first2++;
				}
				else {
					*d_first++ = *first1;
					++first1;
					++first2;
				}
			}
		}
		while (first1 != last1) {
			*d_first++ = *first1++;
		}
		while (first2 != last2) {
			*d_first++ = *first2++;
		}
		return d_first;
	}

	// 最小/最大操作

#ifdef max
#undef max
#endif // max

#ifdef min
#undef min
#endif // min

	// max
	// 返回给定值中的较大者
	template <class T>
	constexpr const T& max(const T& a, const T& b) {
		return b < a ? a : b;
	}

	template <class T, class Compare>
	constexpr const T& max(const T& a, const T& b, Compare comp) {
		return comp(b, a) ? a : b;
	}

	template <class T>
	constexpr T max(std::initializer_list<T> ilist) {
		T maximum = *ilist.begin();
		for (auto& n : ilist) {
			maximum = max(maximum, n);
		}
		return maximum;
	}

	template <class T, class Compare>
	constexpr T max(std::initializer_list<T> ilist, Compare comp) {
		T maximum = *ilist.begin();
		for (auto& n : ilist) {
			maximum = max(maximum, n, comp);
		}
	}

	// max_element
	// 寻找范围 [first, last) 中的最大元素
	template <class ForwardIt>
	constexpr ForwardIt max_element(ForwardIt first, ForwardIt last) {
		auto maximum = first;
		while (++first != last) {
			maximum = *first < *maximum ? maximum : first;
		}
		return maximum;
	}

	template <class ForwardIt, class Compare>
	constexpr ForwardIt max_element(ForwardIt first, ForwardIt last,
		Compare comp) {
		auto maximum = first;
		while (++first != last) {
			maximum = comp(*first, *maximum) ? maximum : first;
		}
		return maximum;
	}

	// min
	// 返回给定值中的较小者
	template <class T>
	constexpr const T& min(const T& a, const T& b) {
		return a < b ? a : b;
	}

	template <class T, class Compare>
	constexpr const T& min(const T& a, const T& b, Compare comp) {
		return comp(a, b) ? a : b;
	}

	template <class T>
	constexpr T min(std::initializer_list<T> ilist) {
		T minimum = *ilist.begin();
		for (auto& n : ilist) {
			minimum = min(minimum, n);
		}
		return minimum;
	}

	template <class T, class Compare>
	constexpr T min(std::initializer_list<T> ilist, Compare comp) {
		T minimum = *ilist.begin();
		for (auto& n : ilist) {
			minimum = min(minimum, n, comp);
		}
		return minimum;
	}

	// min_element
	// 寻找范围 [first, last) 中的最小元素
	template <class ForwardIt>
	constexpr ForwardIt min_element(ForwardIt first, ForwardIt last) {
		auto minimum = first;
		while (++first != last) {
			minimum = *minimum < *first ? minimum : first;
		}
		return minimum;
	}

	template <class ForwardIt, class Compare>
	constexpr ForwardIt min_element(ForwardIt first, ForwardIt last,
		Compare comp) {
		auto minimum = first;
		while (++first != last) {
			minimum = comp(*minimum, *first) ? minimum : first;
		}
		return minimum;
	}

	// minmax
	// 返回给定值的最小和最大者
	template <class T>
	constexpr std::pair<const T&, const T&> minmax(const T& a, const T& b) {
		if (a < b) {
			return { a, b };
		}
		return { b, a };
	}

	template <class T, class Compare>
	constexpr std::pair<const T&, const T&> minmax(const T& a, const T& b,
		Compare comp) {
		if (comp(a, b)) {
			return { a, b };
		}
		return { b, a };
	}

	template <class T>
	constexpr std::pair<T, T> minmax(std::initializer_list<T> ilist) {
		T maximum = *ilist.begin();
		T minimum = *ilist.begin();
		for (auto& n : ilist) {
			maximum = maximum < n ? n : maximum;
			minimum = minimum < n ? minimum : n;
		}
		return { minimum, maximum };
	}

	template <class T, class Compare>
	constexpr std::pair<T, T> minmax(std::initializer_list<T> ilist, Compare comp) {
		T maximum = *ilist.begin();
		T minimum = *ilist.begin();
		for (auto& n : ilist) {
			maximum = comp(maximum, n) ? n : maximum;
			minimum = comp(minimum, n) ? minimum : n;
		}
		return { minimum, maximum };
	}

	// minmax_element
	// 寻找范围 [first, last) 中最小和最大的元素
	template <class ForwardIt>
	constexpr std::pair<ForwardIt, ForwardIt>
		minmax_element(ForwardIt first, ForwardIt last) {
		auto maximum = first;
		auto minimum = first;
		while (++first != last) {
			maximum = *maximum < *first ? *first : maximum;
			minimum = *minimum < *first ? minimum : * first;
		}
		return { minimum, maximum };
	}

	template <class ForwardIt, class Compare>
	constexpr std::pair<ForwardIt, ForwardIt>
		minmax_element(ForwardIt first, ForwardIt last, Compare comp) {
		auto maximum = first;
		auto minimum = first;
		while (first-- != last) {
			maximum = comp(*maximum, *first) ? *first : maximum;
			minimum = comp(*minimum, *first) ? minimum : *first;
		}
		return { minimum, maximum };
	}

	// clamp
	// 如果 v 属于 [lo, hi]，则返回 v
	// 否则返回最临近的边界
	template <class T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
		if (!(v < lo)) {
			if (!(hi < v)) {
				return v;
			} else {
				return hi;
			}
		} else {
			return lo;
		}
	}

	template <class T, class Compare>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi,
		Compare comp) {
		if (!comp(v, lo)) {
			if (!comp(hi, v)) {
				return v;
			} else {
				return hi;
			}
		} else {
			return lo;
		}
	}

	// 比较操作

	// equal
	// 检查 [first1, last1) 与从 first2 开始的另一个范围是否相等
	template <class InputIt1, class InputIt2>
	bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2) {
		while (first1++ != last1) {
			if (!(*first1 == *first2)) {
				return false;
			}
		}
		return true;
	}

	template <class InputIt1, class InputIt2, class BinaryPred>
	bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, BinaryPred p) {
		while (first1++ != last1) {
			if (!p(*first1, *first2)) {
				return false;
			}
		}
		return true;
	}

	template <class InputIt1, class InputIt2>
	bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2) {
		if (std::distance(first1, last1) != std::distance(first2, last2)) {
			return false;
		}
		while (first1++ != last1) {
			if (!(*first1 == *first2)) {
				return false;
			}
		}
		return true;
	}

	template <class InputIt1, class InputIt2, class BinaryPred>
	bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2, BinaryPred p) {
		if (std::distance(first1, last1) != std::distance(first2, last2)) {
			return false;
		}
		while (first1++ != last1) {
			if (!p(*first1, *first2)) {
				return false;
			}
		}
		return true;
	}

	// lexicographical_compare
	// 检查第一个范围 [first1, last1) 是否按字典序小于 ﻿第二个范围 [first2, last2)
	template <class InputIt1, class InputIt2>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2) {
				return true;
			}
			if (*first2 < *first1) {
				return false;
			}
			++first1;
			++first2;
		}
		if (first1 != last1) {
			return true;
		}
		if (first2 != last2) {
			return false;
		}
		return false;
	}

	template <class InputIt1, class InputIt2, class Compare>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		Compare comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first1, *first2)) {
				return true;
			}
			if (comp(*first2, *first1)) {
				return false;
			}
			++first1;
			++first2;
		}
		if (first1 != last1) {
			return true;
		}
		if (first2 != last2) {
			return false;
		}
		return false;
	}
} //namespace mystl
