import sys


def find(name: str) -> str:
    return name if "S" in name else 0


if __name__ == '__main__':
    N = int(sys.stdin.readline().strip())
    while N:
        suspect = str(sys.stdin.readline().strip())
        ans = find(suspect)
        if ans:
            print(ans)
        N -= 1


for _ in range(int(input())):
    a = str(input())
    # a = a if "S" in a else ""
    print(a, end="")
