# def preorder(n):
#     if n:
#         print(n)
#         preorder(ch1[n])
#         preorder(ch2[n])
#
#
# V = int(input())
# E = V - 1
# arr = list(map(int, input().split()))
# ch1 = [0] * (V+1)
# ch2 = [0] * (V+1)
# par = [0] * (V+1)
# for i in range(E):
#     p, c = arr[i*2], arr[i*2+1]
#     if ch1[p] == 0:
#         ch1[p] = c
#     else:
#         ch2[p] = c
#     par[c] = p
# print(ch1)
# print(ch2)
# print(par)
# root = 1
# while par[root] != 0:
#     root += 1
# preorder(root)
# def preorder(node):
#     if node:
#         print(node, end=' ')
#         preorder(tree[node][0])
#         preorder(tree[node][1])
#
#
# V = int(input())
# E = V - 1
# tree = [[0] * 3 for _ in range(V+1)]
# temp = list(map(int, input().split()))
# for i in range(E):
#     p = temp[i*2]
#     c = temp[i*2+1]
#     if tree[p][0] == 0:
#         tree[p][0] = c
#     else:
#         tree[p][1] = c
#     tree[c][2] = p
# root = 1
# while tree[root][2] != 0:
#     root += 1
# for row in tree:
#     print(*row)
# preorder(root)
# import sys
# sys.stdin=open('input_inorder.txt', 'r')
#
#
# def inorder(p, n):
#     if p <= n:
#         inorder(p*2, n)
#         print(tree[p], end='')
#         inorder(p*2+1, n)
#
#
# T = 10
# for tc in range(1, T+1):
#     N = int(input())
#     tree = [0] * (N+1)
#     for _ in range(N):
#         arr = list(input().split())
#         tree[int(arr[0])] = arr[1]
#     print(f'#{tc}')
#     inorder(1, N)
#     print()
# '''
# 13
# 1 2 1 3 2 4 3 5 3 6 4 7 5 8 5 9 6 10 6 11 7 12 11 13
# 5
# 3 1 3 2 2 4 2 5
# '''


# def deq():
#     global last
#     tmp = heap[1]
#     heap[1] = heap[last]
#     last -= 1
#     p = 1
#     c = p * 2
#     while c <= last:
#         if c + 1 <= last and heap[c] < heap[c + 1]:
#             c += 1
#         if heap[p] < heap[c]:
#             heap[p], heap[c] = heap[c], heap[p]
#             p = c
#             c = p * 2
#         else:
#             break
#     return tmp
#
#
# heap = [0]*100
# last = 0


import heapq
heap = [7, 2, 5, 3, 4, 6]
heapq.heapify(heap)
print(heap)
heapq.heappush(heap, 1)
print(heap)
while heap:
    print(heapq.heappop(heap), end=' ')
print()

temp = [7, 2, 5, 3, 4, 6]
heap2 = []
for i in range(len(temp)):
    heapq.heappush(heap2, -temp[i])
print(heap2)
heapq.heappush(heap2, -1)
while heap2:
    print(heapq.heappop(heap2) * -1, end=' ')


