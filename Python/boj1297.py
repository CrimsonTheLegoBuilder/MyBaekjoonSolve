import sys
INPUT = sys.stdin.readline

if __name__ == '__main__':  # boj1297
    D, H, W = map(int, INPUT().split())
    X = (H ** 2 + W ** 2) ** .5
    RH = H * D / X
    RW = W * D / X
    print(f'{int(RH)} {int(RW)}')
