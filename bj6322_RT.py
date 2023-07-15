import sys
i = 1

while True:
    arr = list(map(int, sys.stdin.readline().strip().split()))
    if arr[0] == arr[1] == arr[2] == 0:
        break
    if arr[0] == -1:
        a2 = arr[2] ** 2 - arr[1] ** 2
        if a2 <= 0:
            print(f'Triangle #{i}')
            print('Impossible.\n')
            i += 1
            continue
        else:
            print(f'Triangle #{i}')
            print(f'a = {a2 ** 0.5}\n')
            i += 1
    if arr[1] == -1:
        b2 = arr[2] ** 2 - arr[0] ** 2
        if b2 <= 0:
            print(f'Triangle #{i}')
            print('Impossible.\n')
            i += 1
            continue
        else:
            print(f'Triangle #{i}')
            print(f'b = {b2 ** 0.5}\n')
            i += 1
    if arr[2] == -1:
        c2 = arr[0] ** 2 + arr[1] ** 2
        print(f'Triangle #{i}')
        print(f'c = {c2 ** 0.5}\n')
        i += 1
