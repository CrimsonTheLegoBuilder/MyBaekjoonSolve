# ========== CAUTION!!! ========== WRONG CODE!!! ========== #
# ========== CAUTION!!! ========== WRONG CODE!!! ========== #
# ========== CAUTION!!! ========== WRONG CODE!!! ========== #

import sys

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

N, target = map(int, sys.stdin.readline().strip().split())
high_sl = 0.0  # high concentration solute
high_sv = 0.0  # high concentration solution (solvent)
low_sl = 0.0  # low concentration solute
low_sv = 0.0  # low concentration solution
ans = 0.0
for _ in range(N):
    c, sv = map(int, sys.stdin.readline().strip().split())  # concentration, solution
    sl = sv * c / 100.0  # amount ot solute
    if c == target:
        ans += sv
    elif c > target:
        high_sl += sl
        high_sv += sv
    else:
        low_sl += sl
        low_sv += sv
# print(high_sv, high_sl, low_sv, low_sl)

if low_sv < 0.0000001:  # if low concentration solution doesn't exist:
    print(ans)
    exit(0)
x = (100 * high_sl - target * high_sv) / (target * low_sv - 100 * low_sl)
if x > 1.0:
    ans += high_sv / x + low_sv
else:
    ans += high_sv + x * low_sv
print(ans)

# ========== CAUTION!!! ========== WRONG CODE!!! ========== #
# ========== CAUTION!!! ========== WRONG CODE!!! ========== #
# ========== CAUTION!!! ========== WRONG CODE!!! ========== #
