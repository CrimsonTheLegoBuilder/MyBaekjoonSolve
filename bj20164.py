import sys


def odd_sum(s):
    if len(s) == 1:
        if int(s) % 2:
            return 1, 1
        else:
            return 0, 0
    elif len(s) == 2:
        temp = (int(s[0]) % 2)+(int(s[1]) % 2)
        ss = int(s[0])+int(s[1])
        return temp + int(odd_sum(str(ss))[0]), temp + int(odd_sum(str(ss))[1])
    elif len(s) >= 3:
        length = len(s)
        odd = 0
        for i in s:
            if int(i) % 2:
                odd += 1
        temp = []
        for i in range(1, length-1):
            for j in range(i+1, length):
                temp.append((odd_sum(s[:i])[0] + odd_sum(s[i:j])[0] + odd_sum(s[j:])[0], odd_sum(s[:i])[1] + odd_sum(s[i:j])[1] + odd_sum(s[j:])[1]))
        temp.sort()
        low = temp[0][0]
        temp.sort(key=lambda x: -x[1])
        high = temp[0][1]
        return odd+low, odd+high


N = str(sys.stdin.readline().strip())
ans = odd_sum(N)
print(ans)
