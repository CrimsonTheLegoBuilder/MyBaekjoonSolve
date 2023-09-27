import sys


def cal_length(d1, d2):
    return (d2[0]-d1[0])**2 + (d2[1]-d1[1])**2


N = int(sys.stdin.readline().strip())
conv = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(N)]

ans = 0, 0
max_d = int(12e12)

for i in range(N):
    tmp_d = 0
    for j in range(N):
        if j != i:
            if tmp_d < cal_length(conv[i], conv[j]):
                tmp_d = cal_length(conv[i], conv[j])
    if max_d > tmp_d:
        max_d = tmp_d
        ans = conv[i]

print(*ans)
