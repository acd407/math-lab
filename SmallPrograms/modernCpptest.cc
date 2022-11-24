#include <fmt/core.h>
#include <tuple>

class A {
public:
	int a;
	int b;
	A(){};
	A(int i_a, int i_b) : a(i_a), b(i_b){};
	~A() { fmt::print("a: {0}\tb: {1}\n", a, b); }
	A(const A &in) {
		a = in.a;
		b = 0;
	}
};
void test1() {
	A x(2, 5);
	A y;
	y = x;
	A z(x);
}

template <int num> int f_2() {
	fmt::print("{}", num);
	return num;
}
void test2() { f_2<15>(); }

constexpr int fibonacci(const int n) {
	return n == 1 || n == 2 ? 1 : fibonacci(n - 1) + fibonacci(n - 2);
}
void test3() { fmt::print("{}", fibonacci(17)); }

auto f_4() { return std::make_tuple(1, 2.3, "456"); }
int test4() {
	auto [x, y, z] = f_4();
	fmt::print("x: {}\ty: {}\tz: {}\n", x, y, z);
	return 0;
}

auto f_5() {
	size_t m_1_len = 5, m_2_len = 9;
	int m_1[m_1_len]{1, 5, 7, 4, 25};
	double m_2[m_2_len]{0.8, 4.5};
	bool IS_N = true;
	auto f = [IS_N](auto a, size_t al) {
		auto retval = a[0];
		for (auto i = 1; i < al; i++)
			retval += a[i];
		return IS_N ? -retval : retval;
	};
	return std::make_tuple(f(m_1, m_1_len), f(m_2, m_2_len));
}
int test5() {
	auto [x, y] = f_5();
	fmt::print("x: {}\ty: {}\n", x, y);
	return 0;
}

#define FUNC test5

int main() {
	FUNC();
	return 0;
}