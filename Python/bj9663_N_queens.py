import sys

n = int(sys.stdin.readline().strip())
ans = 0  # 답을 더할 변수
board = [0] * n  # 2차원 보드의 1차원 표현


def check_q(x):
    for i in range(x):
        if board[x] == board[i] or abs(board[x] - board[i]) == abs(x - i):  # 위아래 혹은 대각선에서 만나는지 판정
            return False
    return True


def n_queens(x):
    global ans
    if x == n:  # 모두 잘 놓을 수 있게 되었다면
        ans += 1  # 1 추가
        return
    for i in range(n):
        board[x] = i  # x + 1번 행의 i번 열에 퀸을 놓겠다
        if check_q(x):  # 대각선, 위아래에서 만나지 않는다면
            n_queens(x + 1)  # n - 1까지 반복 ㄱㄱ


n_queens(0)
print(ans)
