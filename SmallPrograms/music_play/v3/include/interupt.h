#pragma once
#include <fmt/core.h>

#ifdef _MSC_VER // for MSVC
#define forceinline __forceinline
#elif defined __GNUC__ // for gcc on Linux/Apple OS X
#define forceinline __inline__ __attribute__((always_inline))
#else
#define forceinline
#endif

// 错误处理
namespace intreupt {
	enum num_level { error = 0, warning = 1, info = 2 };

	static const char *levels[] = {"\033[91merror:\033[0m", "\033[95mwarning:\033[0m",
							"\033[96minfo:\033[0m"};
	template <int level, typename T>
	void interupt(const char *file, int line, T whicherr) {
		fmt::print("{}:{}: {} {}\n", file, line, levels[level], whicherr);
		if (!level)
			exit(EXIT_FAILURE);
	}

	template <typename T>
	void forceinline ERR(T description) {
		interupt<error>(__FILE__, __LINE__, description);
	}

	template <typename T>
	void forceinline WAR(T description) {
		interupt<warning>(__FILE__, __LINE__, description);
	}

	template <typename T>
	void forceinline INF(T description) {
		interupt<info>(__FILE__, __LINE__, description);
	}

}
