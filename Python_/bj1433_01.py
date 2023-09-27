# ========== CAUTION!!! ========== WRONG CODE!!! ========== #
# ========== CAUTION!!! ========== WRONG CODE!!! ========== #
# ========== CAUTION!!! ========== WRONG CODE!!! ========== #
import sys

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

import sys

N, target = map(int, sys.stdin.readline().strip().split())
# N은 병의 수, target은 목표 농도

solutions = [0] * 101  # 농도는 정수로 주어지기 때문에 농도를 인덱스로 하여 받았다.
ans = 0.0              # 총 용액의 양

for _ in range(N):
    c, sl = map(int, sys.stdin.readline().strip().split())

    solutions[c] += sl  # 용액의 데이터 수집. 해당 인덱스의 농도를 가진 용액이 모인다.
    if c == target:     # 만들고자 하는 용액과 농도가 같다면?
        ans += sl       # 답에도 더하자.

low, high = target - 1, target + 1  # 두 포인터로 조사하겠다.

'''
해당 농도의 용액을 구하기 위해서는 농도가 목표 농도에 가장 근접한 각각의 높은 농도의 용액과 낮은 농도의 용액부터 하나씩 섞을 떄 가장 많은 양의 용액을 만들 수 있다. 가장 덜 희석시킬수록 더 많은 양의 용액을 사용할 수 있기 때문. 그래서 이 문제는 두 포인터로 풀 때 가장 쉽게 풀리며, 해당 농도의 두 용액 중 사용량이 더 적은 용액은 전부 섞으면 되기 때문에 그리디 알고리즘도 사용된다.
'''

while low >= 0 and high <= 100:         # 두 포인터의 종료 조건

    while solutions[low] <= 0.000001:   # 해당 농도의 용액이 더 이상 남아 있지 않다면?
        low -= 1                        # 더 낮은 농도를 조사.
        if low < 0:                     # 더 낮은 게 없다면?
            break                       # 찾을 게 없네?

    while solutions[high] <= 0.000001:  # 해당 농도의 용액이 더 이상 남아 있지 않다면?
        high += 1                       # 더 높은 농도를 조사.
        if high > 100:                  # 더 높은 게 없다면?
            break                       # 없어. 돌아가.
    if low < 0 or high > 100:           # 다 섞었다~
        break                           # 끝.

    high_sv = solutions[high]       # 높은 농도의 용액의 양
    low_sv = solutions[low]         # 낮은 농도의 용액의 양
    high_sl = high * high_sv / 100  # 높은 농도의 용질의 양
    low_sl = low * low_sv / 100     # 낮은 농도의 용질의 양

    x = (100 * high_sl - target * high_sv) / (target * low_sv - 100 * low_sl)
		# x = 낮은 농도의 용액이 사용되는 비율
		# 1보다 작으면 낮은 농도의 용액이 남는다. 반대는 낮은 농도의 용액을 전부 써야 한다.
		# 화학에서 해당 농도를 가지는 용액을 구하기 위해 두 용액을 섞을 때 쓰는 식.

    if x > 1:                           # 낮은 농도의 용액을 전부 써야 한다면?
        ans += high_sv / x + low_sv     # 만들어진 용액을 더한다.
        solutions[high] -= high_sv / x  # 높은 농도의 용액의 남은 양
        solutions[low] = 0              # 낮은 농도의 용액은 소진.
        low -= 1                        # 더 낮은 농도의 용액을 보자.

    else:                               # 낮은 농도의 용액이 남는다면?
        ans += high_sv + low_sv * x     # 만들어진 용액을 더한다.
        solutions[high] = 0             # 높은 농도의 용액은 남지 않았다.
        solutions[low] -= low_sv * x    # 닞은 농도의 용액은 이 만큼 남았다.
        high += 1                       # 더 높은 농도의 용액을 보자.

print(ans)