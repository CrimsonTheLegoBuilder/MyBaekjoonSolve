import sys

d = ['N', 'E', 'S', 'W']
ans = 0

for _ in range(10):
    a = int(sys.stdin.readline().strip())
    if a == 1:
        ans += 1
    elif a == 2:
        ans += 2
    else:
        ans -= 1

print(d[ans % 4])
