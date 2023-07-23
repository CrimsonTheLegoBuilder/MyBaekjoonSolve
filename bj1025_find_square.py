import sys

n, m = map(int, sys.stdin.readline().strip().split())
arr = [sys.stdin.readline().strip() for _ in range(n)]  # 0. 일단 입력 받는 줄부터 적고 시작
# print(arr)  0. 잘 받아지는지 확인하는 용도
ans = -1  # 0. 제곱수를 찾지 못하면 답은 -1이며 더 크면 갱신
square_set = set(i**2 for i in range(31623))  # 5. 최대 제곱수도 999,999,999를 넘지 못하므로 10e9의 제곱근인 31622.77...까지 전처리

for i in range(n):
    for j in range(m):
        for k in range(-i, n - i):  # 6. 모든 방향에 대해서 시작? 범위 설정은 어떻게 하는 걸까?
            for l in range(-j, m - j):  # 7. 4중 for문을 돌아야하지 않을까.
                idx_y = l
                idx_x = k  # 4. 인덱스는 등차수열을 이룬다.  초기치는 k와 l.
                a = arr[i][j]  # 1. 일단 시작 숫자를 하나 잡는다. 그 이후에 등차수열로 숫자들을 순차적으로 더해나가며 제곱수가 들어있는지 집합과 대조 확인.
                if int(a) in square_set and int(a) > ans: ans = int(a)  # 자기 자신부터 확인.
                while True:  # 8. 모든 칸에 대해 검증해야하고 모든 등차수열을 다 고려해야하므로 5중 for문을 돌게 된다.
                    if i + idx_x < 0 or i + idx_x >= n or j + idx_y < 0 or j + idx_y >= m or idx_x == 0 and idx_y == 0:  # 9. 종료 조건을 찾지 못 하는 중. 인덱스 에러가 뜰 때 끝.
                        break  # 둘 다 0이면 선택한 숫자 자체를 확인하고 끝.
                    a += arr[i + idx_x][j + idx_y]
                    num1 = int(a)
                    # num2 = int(a[::-1])  # 10. 필요 없어진 과정.
                    if num1 in square_set and num1 > ans:  # 2. 하나하나씩 다 고려하겠다.
                        ans = num1
                    # if num2 in square_set and num2 > ans:  # 3. 뒤집힌 것도 보겠다.
                    #     ans = num2
                    idx_x += k
                    idx_y += l
print(ans)
