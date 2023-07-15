import sys

n = int(sys.stdin.readline().strip())

for i in range(1, n + 1):
    arr = list(map(int, sys.stdin.readline().strip().split()))
    arr.sort()
    sys.stdout.write(f'Scenario #{i}\n')
    if arr[2] ** 2 == arr[1] ** 2 + arr[0] ** 2:
        sys.stdout.write('Yes\n')
    else:
        sys.stdout.write('No\n')
    sys.stdout.write('\n')
