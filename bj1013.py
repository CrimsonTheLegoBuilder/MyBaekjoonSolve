import sys

while True:
    try:
        a, b = map(int, sys.stdin.readline().rstrip().split())
        ans = b // (a + 1)
        sys.stdout.write(str(ans)+'\n')
    except Exception as e:
        # print(f'{e}')
        break
