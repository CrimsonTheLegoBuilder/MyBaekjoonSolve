import sys

n, m = map(int, sys.stdin.readline().strip().split())

while m <= 100:  # 종료 조건 1. 문제에서 제시된 값.
    # if n % 2 == 1 and m % 2 == 1:  # n과 m이 동시에 홀수이면 무조건 안 될 거라 생각했는데 생각해보니 6 7 8 있음. 폐기
    # m += 1
    # continue
    # if n < m:  # 종료 조건 2. 어떤 구간합보다 큰 간격으로 1씩 증가하는 양의 등차수열을 구할 수는 없다. 틀린 조건. 반례 발생.
    #     print(-1)
    #     exit(0)
    # else:
    if n == 1 and m == 1 or n == 0 and m == 1:  # 0 1 일 때 0인 것이 반례라서 꼭 적어주어야 한다
        print(n)
        exit(0)
    elif n == 1 and m == 2:  # 역시 반례이다. 숫자가 작을 때는 나머지 연산이 통하지 않기 때문에 반드시 챙겨주자.
        print(0, 1)
        exit(0)
    elif (n * 2) % m == 0 and n >= m:  # 1씩 증가하는 등차수열의 구간합 공식 조건을 만족할 때, n > m이 아닌 n >= m
        a_b = (n * 2) // m  # 중앙값
        if m % 2 == 0 and a_b % 2 == 0:  # 구간의 항이 짝수개인데 중앙의 두 값의 합이 짝수인 경우는 존재하지 않는다.
            m += 1
            continue  # 한 번 더!
        if m % 2 == 0:
            f_a = a_b//2 - m//2 + 1  # 항이 짝수개일 때와 홀수개일 때를 따로 고려. 항이 짝수개이면 중앙값이 버림됨. 초항은 += 1
        else:
            f_a = a_b//2 - m//2  # 항이 홀수개이면 양쪽 최대값과 최소값의 합은 짝수이고 중앙값이 살게 됨.
        # f_a = int((a_b+1)//2) - int(m//2) 보다 간단하게 만든 조건.
        if f_a < 0:  # 그럴 일 없겠지만 혹시라도 계산한 초항이 0보다 작다면
            m += 1
            continue  # 안 돼 돌아가
        ans = [i for i in range(f_a, f_a + m)]  # 구간은 다음과 같다.
        print(*ans)
        exit(0)
    else:
        m += 1

print(-1)  # 종료 조건 3. 모두 돌아봤는데도 만족하는 조건이 없더라.

# 맞추고 난 이후 볼 수 있게 된 다른 사람의 풀이 중 재밌어보이는 풀이
# n, l = map(int, input().split())
# 
# sm = sum([i for i in range(l)])
# 
# while True:
#     if l>100 or n-sm<0:
#         print(-1)
#         break
# 
#     if (n-sm)%l==0:
#         lst = [((n-sm)//l)+i for i in range(l)]
#         print(*lst)
#         break
# 
#     sm+=l
#     l+=1
#     