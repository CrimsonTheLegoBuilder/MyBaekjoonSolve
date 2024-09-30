import sys
INPUT = sys.stdin.readline

if __name__ == "__main__":
    while 1:
        A = list(map(int, INPUT().strip().split()))
        A.sort()
        if A[0] == 0 and A[1] == 0 and A[2] == 0:
            break
        else:
            if A[2] ** 2 == A[0] ** 2 + A[1] ** 2:
                print("right")
            else:
                print("wrong")
