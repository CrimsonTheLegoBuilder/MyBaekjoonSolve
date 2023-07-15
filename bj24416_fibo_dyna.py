import sys


def fibo_rec(n):
    global cnt_r
    if n == 1 or n == 2:
        cnt_r += 1
        return 1
    return fibo_rec(n - 1) + fibo_rec(n - 2)
# 피보나치 수를 재귀로 구할 때 실행되는 함수의 횟수는 피보나치 수와 같음. 1을 뱉은 횟수라고 할 수 있음.


def fibo_dyna(n):
    if n == 1:
        return 1  # 인덱스 에러 방지
    global cnt_d
    fd = [0 for _ in range(n + 1)]
    fd[1] = fd[2] = 1
    for i in range(3, n + 1):
        cnt_d += 1
        fd[i] = fd[i - 1] + fd[i - 2]
    return fd[n]


N = int(sys.stdin.readline().strip())
cnt_r = 0
cnt_d = 0
print(fibo_dyna(N), cnt_d)
