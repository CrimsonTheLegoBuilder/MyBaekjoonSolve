import sys
INPUT = sys.stdin.readline

if __name__ == "__main__":
    N: int = int(INPUT())
    P: list[tuple] = []
    for _ in range(N):
        x, y = map(int, INPUT().split())
        P.append((x, y))
    P.sort(key=lambda p: (p[0], p[1]))
    for x, y in P:
        print(x, y)
    