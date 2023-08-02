import sys
n = int(sys.stdin.readline().strip())
time = list(map(int, sys.stdin.readline().strip().split()))
time.sort()
for i in range(1, n):
    time[i] = time[i - 1] + time[i]
print(sum(time))
