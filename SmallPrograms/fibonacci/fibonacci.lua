function fib(n)
	if(n==2 or n==1) 
	then
		return 1, 1;
	else
		local a, b = fib(n-1)
		return b, a+b;
	end
end
for i=1, 90 do
	local a, b = fib(i)
	print(i, b);
end

