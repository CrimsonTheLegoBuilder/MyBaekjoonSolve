# def summation(a):
#     if a < 10:
#         return a
#     s = a % 10
#     d = a // 10
#     return s + summation(d)
#
#
# target = int(input())
# print(summation(target))


a = int(input())
i = 2
flag = 0

while i < a:
    if not a % i:
        print(a % i)
        print(-1)
        flag = 1
        # exit(0)
        break
    i += 1

if not flag:
    print(1)

# b = [[1, 2, 3], [3, 4, 5], [4, 5, 6]]
# a = [sum(b[i]) for i in range(3)]
# print(*a)

# n = int(input())
# arr = [1] * (n + 1)
# for i in range(2, int(n**.5) + 2):
#     if arr[i] == 1:
#         for j in range(i * 2, n + 1, i):
#             arr[j] = 0
#
# print(arr[n])

arr = [[1, 2, 3], [2, 3, 4], [4, 5, 6]]
ans = []
n = int(input())

for i in range(3):
    for j in range(3):
        if arr[i][j] == n:
            ans.append((i, j))

print(ans)
