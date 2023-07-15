import sys

n = int(input())

stack = []
arr = []
last = 0
answer = []

for i in range(n):
    a = int(sys.stdin.readline().strip())
    if len(arr) == 0 or arr[-1] < a:
        for j in range(last + 1, a + 1, 1):
            arr.append(j)
            answer.append('+')
        stack.append(arr.pop())
        answer.append('-')
    elif arr[-1] == a:
        stack.append(arr.pop())
        answer.append('-')
    elif arr[-1] > a:
        print('NO')
        break

    if last < a:
        last = a

if len(arr) == 0:
    for k in answer:
        print(k)

