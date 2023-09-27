# class TreeNode:
#     def __init__(self, value):
#         self.value = value
#         self.left = None
#         self.right = None
#
#     def insert(self, childNode):
#         if not self.left:
#             self.left = childNode
#             return
#         if not self.right:
#             self.right = childNode
#             return
#         return
#
#     def preorder(self):
#         if self is not None:
#             print(self.value, end=' ')
#             if self.left:
#                 self.left.preorder()
#             if self.right:
#                 self.right.preorder()


arr = [1, 2, 1, 3, 2, 4, 3, 5, 3, 6]
# nodes = [TreeNode(i) for i in range(0, 7)]
nodes = [[] for i in range(0, 14)]
for i in range(0, len(arr), 2):
    parentNode = arr[i]
    childNode = arr[i+1]
    nodes[parentNode].append(childNode)

for i in range(len(nodes)):
    print(nodes[i])

for li in nodes:
    for _ in range(len(li), 2):
        li.append(None)


def preorder(noNum):
    if noNum is None:
        return
    print(noNum, end=' ')
    preorder(nodes[noNum][0])
    preorder(nodes[noNum][1])


preorder(1)
