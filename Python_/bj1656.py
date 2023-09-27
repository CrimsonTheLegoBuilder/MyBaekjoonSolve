import sys

while True:
    n = int(sys.stdin.readline().strip())
    if n == 0:
        break
    else:
        legs = []
        ans = int(9e12)
        for _ in range(n):
            legs.append(int(sys.stdin.readline().strip()))
        if n == 3 or n == 4:
            short = min(legs)
            ans = sum(list(map(lambda x: x - short, legs)))
        elif n % 2:
            for i in range(n):
                total = 0
                opposite_1 = []
                opposite_2 = []
                for j in range(1, (n - 1)//2 - n//4 + 1):
                    opposite_1.append(legs[(i + n//4 + j) % n])
                    opposite_2.append(legs[(i + n//2 + j) % n])
                l_short = min(max(opposite_1), max(opposite_2), legs[i])
                for l in legs:
                    if l > l_short:
                        total += l - l_short
                ans = min(ans, total)
        elif not n % 2:
            for i in range(n // 2):
                total = 0
                opposite_1 = []
                opposite_2 = []
                for j in range(1, (n - 1)//2 + 1):
                    opposite_1.append(legs[(i + j) % n])
                    opposite_2.append(legs[(i + n//2 + j) % n])
                l_short = min(max(opposite_1), max(opposite_2), legs[i], legs[i + n//2])
                for l in legs:
                    if l > l_short:
                        total += l - l_short
                ans = min(ans, total)
            for i in range(n):
                total = 0
                opposite_1 = []
                opposite_2 = []
                for j in range(1, (n - 1)//2 - n//4 + 1):
                    opposite_1.append(legs[(i + n//4 + j) % n])
                    opposite_2.append(legs[(i + n//2 + j) % n])
                l_short = min(max(opposite_1), max(opposite_2), legs[i])
                for l in legs:
                    if l > l_short:
                        total += l - l_short
                ans = min(ans, total)
        print(f'{ans}\n')
