import sys

N = int(sys.stdin.readline().strip()) // 2
cnt = 0
x, y = N-1, 0
while x >= 0:
    while x**2 + y**2 <= N**2:
        cnt += 1
        if x**2 + (y+1)**2 >= N**2:
            if x**2 + (y+1)**2 == N**2:
                y += 1
            break
        y += 1
    x -= 1
# print(cnt)
print(cnt*4)
