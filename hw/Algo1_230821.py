def max_(a, b): return a if a >= b else b  # 둘을 비교 후 큰 값을 반환
def min_(a, b): return a if a <= b else b  # 둘을 비교 후 작은 값을 반환


def balloon(r, c):
    global ans_M, ans_m  # 최대값 M 과 최소값 m 을 전역 변수로 선언
    bonus = stage[r][c]  # 추가로 터지는 범위
    total = bonus        # 일단 가운데 하나 터추고 시작!
    for a in range(1, bonus+1):  # 범위를 늘려가며 터뜨리기
        for i in range(4):
            nr = r + dr[i] * a
            nc = c + dc[i] * a
            if 0 <= nr < n and 0 <= nc < n:  # 범위 안에 들어온다면?
                total += stage[nr][nc]       # 추가
    ans_M = max_(ans_M, total)
    ans_m = min_(ans_m, total)  # 비교 후 최대와 최소를 기록


t = int(input())
for tc in range(1, t+1):
    n = int(input())
    stage = [tuple(map(int, input().split())) for _ in range(n)]
    dr = (1, 0, -1, 0)
    dc = (0, 1, 0, -1)  # 그래프 탐색 용 방향키
    ans_M = 0
    ans_m = int(12e12)  # 말도 안 되는 높은 값을 선언
    for x in range(n):
        for y in range(n):  # 전부 스캔
            balloon(x, y)
    print(f'#{tc} {ans_M - ans_m}')  # 구힌 최대값에서 최소값을 빼기
