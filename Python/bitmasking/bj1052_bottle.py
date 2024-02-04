import sys

n, k = map(int, sys.stdin.readline().strip().split())
cnt = 0
cur = 1

while n > 1 << k:
    if n % 2:
        # print(n)
        n += 1
        # print(n)
        # print(cur)
        # print(cnt)
        cnt += cur
    cur <<= 1
    n >>= 1
#     print(n)
#     print()
# print(n)
# print('#')

# remain = 0
# while n > 0:
#     if n % 2:
#         n -= 1
#         # print(n)
#         remain += 1
#         # print(remain)
#     n >>= 1
#     # print(n)
#     # print()

# if remain <= k:
print(cnt)
# else:
#     print(-1)
