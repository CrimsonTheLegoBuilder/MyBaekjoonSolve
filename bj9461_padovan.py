import sys
fd = [0 for _ in range(101)]


def padovan(n):
    fd[1] = 1
    fd[2] = 1
    fd[3] = 1
    fd[4] = 2
    fd[5] = 2
    for i in range(6, n + 1):
        fd[i] = fd[i - 1] + fd[i - 5]
    return fd[n]


padovan(100)

x = int(sys.stdin.readline().strip())
for _ in range(x):
    N = int(sys.stdin.readline().strip())
    print(fd[N])
