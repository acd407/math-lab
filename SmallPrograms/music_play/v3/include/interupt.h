#pragma once
#include <fmt/core.h>

// 错误处理
enum num_level { error = 0, warning = 1, info = 2 };

static const char *levels[] = {"\033[91merror:\033[0m", "\033[95mwarning:\033[0m",
						"\033[96minfo:\033[0m"};
						
template <int level, typename T>
void interupt(const char *file, int line, T whicherr) {
	fmt::print("{}:{}: {} {}\n", file, line, levels[level], whicherr);
	if (!level)
		exit(EXIT_FAILURE);
}

#define ERR(description) interupt<error>(__FILE__, __LINE__, description)
#define WAR(description) interupt<warning>(__FILE__, __LINE__, description)
#define INF(description) interupt<info>(__FILE__, __LINE__, description)
