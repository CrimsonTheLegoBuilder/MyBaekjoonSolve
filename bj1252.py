import sys

A, B = map(str, sys.stdin.readline().split())

A, B = int(A, 2), int(B, 2)

print(str(bin(A + B))[2:])


# A, B = list(map(int, A[::-1])), list(map(int, B[::-1]))
# cnt = min(len(A), len(B))
# if len(A) >= len(B):
#     L = A + [0]
#     S = B
# else:
#     L = B + [0]
#     S = A
#
# for i in range(cnt):
#     if L[i] + S[i] == 0:
#         continue
#     elif L[i] + S[i] == 1:
#         L[i] = 1
#     elif L[i] + S[i] == 2:
#         L[i] = 0
#         L[i + 1] += 1
#     elif L[i] + S[i] == 3:
#         L[i] = 1
#         L[i + 1] += 1
#
# for j in range(len(L)):
#     if L[j] > 1:
#         L[j] -= 2
#         L[j + 1] += 1
#
# ans = list(map(str, L[::-1]))
# print(''.join(ans).lstrip('0'))
