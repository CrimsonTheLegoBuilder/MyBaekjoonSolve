import sys
sys.setrecursionlimit(int(6e8))

n = int(sys.stdin.readline().strip())

arr = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
case = []
cnt = 0


def combi_3(x):
    global cnt
    if len(case) == 3:
        d1, d2, d3 = arr[case[0]], arr[case[1]], arr[case[2]]
        side = [(d1[0] - d2[0]) ** 2 + (d1[1] - d2[1]) ** 2, (d2[0] - d3[0]) ** 2 + (d2[1] - d3[1]) ** 2, (d3[0] - d1[0]) ** 2 + (d3[1] - d1[1]) ** 2]
        side.sort()
        if side[0] + side[1] == side[2]:
            cnt += 1
        return
    for i in range(x, n):
        case.append(i)
        combi_3(i + 1)
        case.pop()


combi_3(0)
print(cnt)
