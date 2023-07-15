import sys
import collections as cl

n = int(input())

for _ in range(n):
    flag = 0
    cnt = 1
    arr = [i for i in str(sys.stdin.readline().strip())]
    _ = sys.stdin.readline()
    q = sys.stdin.readline().strip('\n ][')
    q1 = q.split(',')
    if q1[0] == '':
        q1.clear()
    dq1 = cl.deque(q1)

    for j in arr:
        if j == 'D':
            if not dq1:
                flag += 1
                print('error')
                break
            elif cnt == 1:
                dq1.popleft()
            elif cnt == -1:
                dq1.pop()
        elif j == 'R':
            cnt *= -1

    if len(dq1) != 0 and cnt == -1:
        dq1.reverse()

    answer = str(','.join(dq1))

    if flag == 0:
        print('[' + answer + ']')
