import sys

N = int(sys.stdin.readline().strip()) // 2
cnt = 0
x, y = N-1, 0
N **= 2
while x > -1:
    while True:
        cnt += 1
        # print(x, y)
        y += 1
        n = x*x + y*y
        if n >= N:
            if n > N:
                y -= 1
            break
    x -= 1
# print(cnt)
print(cnt*4)
