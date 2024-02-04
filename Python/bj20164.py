import sys


def odd_sum(s):
    if len(s) == 1:
        return (1, 1) if int(s) % 2 else (0, 0)

    elif len(s) == 2:
        odd = (int(s[0]) % 2) + (int(s[1]) % 2)
        low, high = odd_sum(str(int(s[0]) + int(s[1])))
        return odd + low, odd + high

    elif len(s) >= 3:
        length = len(s)
        odd = 0
        for i in s:
            odd += (int(i) % 2)
        temp = []
        for i in range(1, length-1):
            for j in range(i+1, length):
                temp.append(odd_sum(str(int(s[:i]) + int(s[i:j]) + int(s[j:]))))
        low = sorted(temp)[0][0]
        high = sorted(temp, key=lambda x: -x[1])[0][1]
        return odd + low, odd + high


N = str(sys.stdin.readline().strip())
print(*odd_sum(N))
