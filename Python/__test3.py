# def get_answer(start):
#     cur_cost[start] = 0
#     visited = [1]+[0]*N
#
#     visited[start] = 1
#
#     for _ in range(N):
#
#         for nxt in range(1, N + 1):
#             if adj_m[start][nxt] == -1: continue
#             if visited[nxt]: continue
#             if cur_cost[nxt] <= cur_cost[start] + adj_m[start][nxt]: continue
#
#             cur_cost[nxt] = cur_cost[start] + adj_m[start][nxt]
#
#         min_value = 98765432198765432
#         for nxt in range(1, N + 1):
#             if visited[nxt]: continue
#             if min_value <= cur_cost[nxt]: continue
#
#             min_value = cur_cost[nxt]
#             start = nxt
#
#         visited[start] = 1
#
#
# N = int(input())
# M = int(input())
# adj_m = [[-1]*(N + 1) for _ in range(N + 1)]
# for _ in range(M):
#     f, e, w = map(int, input().split())
#     if adj_m[f][e] == -1 or adj_m[f][e] > w:
#         adj_m[f][e] = w
#
# s, E = map(int, input().split())
#
# INT = 987654321987654321
# cur_cost = [0]+[INT]*N
#
# get_answer(s)
#
# print(cur_cost[E])

# from math import pi
# print(pi/4)

import sys


# sys.setrecursionlimit(10 ** 6)
def input():
    return sys.stdin.readline().rstrip()


def print(*args, sep=" ", end="\n"):
    sys.stdout.write(sep.join(map(str, args)) + end)


def solution():
    N, K = map(int, input().split())

    if N >= K:
        print(N - K)
    else:
        arr = [0] * 100001
        arr[N // 2:N] = [abs(N - i) for i in range(N // 2, K * 2 + 1)]
        # print(range(N // 2, K * 2 + 1))
        # print([abs(N - i) for i in range(N // 2, K * 2 + 1)])
        # print(arr[N // 2:N])

        for x in range(N, K + 1):
            arr[x] = min(arr[x], arr[x // 2] + x % 2, arr[x // 2 + x % 2] + x % 2, arr[x - 1] + 1)
            if x * 2 <= 100000:
                arr[x * 2] = min(arr[x * 2], arr[x])
            if x - 1 >= 0:
                arr[x - 1] = min(arr[x - 1], arr[x] + 1)

        # print(arr[:K*2+1])
        print(arr[K])


if __name__ == '__main__':
    solution()