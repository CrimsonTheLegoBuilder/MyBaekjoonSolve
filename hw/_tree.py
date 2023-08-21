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
def preorder(node):
    if node:
        print(node, end=' ')
        preorder(tree[node][0])
        preorder(tree[node][1])


V = int(input())
E = V - 1
tree = [[0] * 3 for _ in range(V+1)]
temp = list(map(int, input().split()))
for i in range(E):
    p = temp[i*2]
    c = temp[i*2+1]
    if tree[p][0] == 0:
        tree[p][0] = c
    else:
        tree[p][1] = c
    tree[c][2] = p
root = 1
while tree[root][2] != 0:
    root += 1
for row in tree:
    print(*row)
preorder(root)


'''
13
1 2 1 3 2 4 3 5 3 6 4 7 5 8 5 9 6 10 6 11 7 12 11 13
5
3 1 3 2 2 4 2 5
'''