# a,b,c=map(int,input().split())
# print((a+b+c)//3+(not a%3==b%3==c%3))

# a,b,c=map(int,input().split())
# print((a+b+c)//3-(a%3==b%3==c%3)+1)

import sys
arr = sorted(list(map(int, sys.stdin.readline().strip().split())))
# print(arr)
a, b, c = arr
# print(a, b, c)
b -= a
c -= a
a += b//3 + c//3
# print(a)
b_ = b % 3
c_ = c % 3
if b_ == c_:
    print(a + b_)
elif b_ and c_ and b_ != c_:
    print(a + 2)
elif b_ or c_:
    print(a + 1)
