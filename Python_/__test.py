# def facto(n):
#     return n*facto(n-1) if n else 1
#
#
# print(facto(10))

# def bi_search(l, r, t):
#     if l > r:
#         return None
#     m = (l+r)//2
#     if arr[m] == t:
#         return m
#     elif arr[m] < t:
#         return bi_search(m+1, r, t)
#     elif arr[m] > t:
#         return bi_search(l, m-1, t)
#
#
# arr = [0, 0, 0, 1, 1, 2, 3, 4, 4, 5, 6, 6, 7, 7, 7, 7, 8, 8, 9]
# print(bi_search(0, len(arr)-1, 4))
print(sum([int(input()) for _ in range(int(input()))]))