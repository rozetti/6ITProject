-- The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. 

lua_global = "1729.42"

print('lua_global: '..lua_global)

lua_fib = function(f)
if f == 0 then return 0 end
if f == 1 then return 1 end

return lua_fib(f-1) + lua_fib(f-2)
end

