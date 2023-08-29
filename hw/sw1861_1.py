drc = ((1, 0), (0, 1), (-1, 0), (0, -1))

for tc in range(1, int(input())+1):
    N = int(input())
    grid = [tuple(map(int, input().split())) for _ in range(N)]
    visited = [0] * (N*N+1)
    for r in range(N):
        for c in range(N):
            for x in range(4):
                nr = r + drc[x][0]
                nc = c + drc[x][1]
                if 0 <= nr < N and 0 <= nc < N and grid[nr][nc] - grid[r][c] == 1:
                    visited[grid[r][c]] = 1

    room_no, step, cnt = 0, 0, 0
    for r in range(N*N, -1, -1):
        if visited[r]:
            cnt += 1
        elif cnt:
            if step <= cnt:
                room_no = r+1
                step = cnt
            cnt = 0

    print(f'#{tc} {room_no} {step+1}')
