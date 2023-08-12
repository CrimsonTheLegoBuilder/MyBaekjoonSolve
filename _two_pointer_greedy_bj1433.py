import sys

N, target = map(int, sys.stdin.readline().strip().split())
solutions = [0] * 101
ans = 0.0
for _ in range(N):
    c, sl = map(int, sys.stdin.readline().strip().split())  # concentration, solution (L)
    solutions[c] += float(sl)
    if c == target:
        ans += sl
print(solutions)
print(f'solution: {target}% {ans}L')
print()
low, high = target - 1, target + 1
while low >= 0 and high <= 100:
    # while solutions[low] <= 0.000001:
    while solutions[low] == 0:
        low -= 1
        if low < 0:
            break
    # while solutions[high] <= 0.000001:
    while solutions[high] == 0:
        high += 1
        if high > 100:
            break
    if low < 0 or high > 100:
        break
    high_sv = solutions[high]
    low_sv = solutions[low]
    high_sl = high * high_sv / 100
    low_sl = low * low_sv / 100
    x = (100 * high_sl - target * high_sv) / (target * low_sv - 100 * low_sl)
    if x > 1:
        print(f'low: {low}% sol {solutions[low]}L use 100.0% / high: {high}% sol {solutions[high]}L use {100 / x}%')
        print(f'used / low: {low}% {solutions[low]}L - {solutions[low]}L / high: {high}% {solutions[high]}L - {high_sv / x}L / solution: {ans} + {solutions[low] + high_sv / x}L')
        ans += high_sv / x + low_sv
        solutions[high] -= high_sv / x
        solutions[low] = 0
        print(f'remain / low: {low}% 0.0L / high: {high}% {solutions[high]}L / solution: {ans}L')
        print()
        low -= 1
    else:
        print(f'low: {low}% sol {solutions[low]}L use {x * 100}% / high: {high}% sol {solutions[high]}L use 100.0%')
        print(f'used / low: {low}% {solutions[low]}L - {low_sv * x}L / high: {high}% {solutions[high]}L - {solutions[high]}L / solution: {ans} + {low_sv * x + solutions[high]}L')
        ans += high_sv + low_sv * x
        solutions[high] = 0
        solutions[low] -= low_sv * x
        print(f'remain / low: {low}% {solutions[low]}L / high: {high}% 0.0L / solution: {ans}L')
        print()
        high += 1

# print(ans)
print(f'total: {ans}L')

'''
2 50
1 100
99 1

4 50
0 50
100 50
1 100
99 1

2 50
0 50
100 50

42 50
50 100
0 15
100 16
1 17
99 18
2 19
98 20
3 21
97 22
4 23
96 24
5 25
98 25
20 60
80 40
30 40
70 60
40 60
60 40
45 100
55 100
50 50
51 51
49 49
52 52
48 48
53 53
47 47
54 54
46 46
55 55
45 45
56 56
57 57
58 58
59 59
60 60
44 44
43 43
42 42
41 41
40 40
'''
# import sys
#
# N, target = map(int, sys.stdin.readline().strip().split())
# solutions = [0] * 101
# ans = 0.0
# for _ in range(N):
#     c, sl = map(int, sys.stdin.readline().strip().split())
#     solutions[c] += sl
#     if c == target:
#         ans += sl
# low, high = target - 1, target + 1
# while low >= 0 and high <= 100:
#     while solutions[low] <= 0.000001:
#         low -= 1
#         if low < 0:
#             break
#     while solutions[high] <= 0.000001:
#         high += 1
#         if high > 100:
#             break
#     if low < 0 or high > 100:
#         break
#     high_sv = solutions[high]
#     low_sv = solutions[low]
#     high_sl = high * high_sv / 100
#     low_sl = low * low_sv / 100
#     x = (100 * high_sl - target * high_sv) / (target * low_sv - 100 * low_sl)
#     if x > 1:
#         ans += high_sv / x + low_sv
#         solutions[high] -= high_sv / x
#         solutions[low] = 0
#         low -= 1
#     else:
#         ans += high_sv + low_sv * x
#         solutions[high] = 0
#         solutions[low] -= low_sv * x
#         high += 1
# print(ans)
