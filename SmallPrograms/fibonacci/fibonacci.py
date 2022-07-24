def fib(n):
	if n==2 or n==1:
		return (1, 1)
	else:
		a, b = fib(n-1)
		return (b, a+b)

for i in range(1, 91):
	a, b = fib(i)
	print(i, b)