n = int(input())

for k in range(1, n + 1):
    a = int(input())
    arr = list(map(int, input().split()))
    dic = {}
    find_max = 0
    ans = []
    for i in arr:
        if i not in dic:
            dic[i] = 1
            if find_max < dic[i]:
                find_max = dic[i]
                ans.clear()
                ans.append(i)
            elif find_max == dic[i]:
                ans.append(i)
        else:
            dic[i] += 1
            if find_max < dic[i]:
                find_max = dic[i]
                ans.clear()
                ans.append(i)
            elif find_max == dic[i]:
                ans.append(i)

    ans.sort()
    print('#' + str(k) + ' ' + str(ans[-1]))
