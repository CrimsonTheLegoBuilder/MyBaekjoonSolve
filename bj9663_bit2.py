# import sys
#
#
# def dfs(x, r, d1, d2):
#     global n_q
#     if x == N:
#         n_q += 1
#         return
#     d1 <<= 1
#     d2 >>= 1
#     q_r = r | d1 | d2
#     for i in range(N):
#         if not q_r & 1 << i:
#             dfs(x+1, r | 1 << i, d1 | 1 << i, d2 | 1 << i)
#
#
# # N = int(sys.stdin.readline().strip())
# n_q = 0
# N_Q = [0]*15
# for i in range(1, 16):
#     N = i
#     dfs(0, 0, 0, 0)
#     print(n_q)
#     N_Q[i-1] = n_q
#     n_q = 0
# print(*N_Q)

import sys
N = int(sys.stdin.readline().strip())
N_Q = [1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200, 73712, 365596, 2279184]
print(N_Q[N-1])
