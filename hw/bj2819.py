# def dfs(r, c, depth, tmp):
#     if depth > 6:
#         num.add(tmp)
#         return
#     for i in range(4):
#         nr = r + drc[i][0]
#         nc = c + drc[i][1]
#         if 0 <= nr < 4 and 0 <= nc < 4:
#             dfs(nr, nc, depth+1, tmp*10 + grid[r][c])
#
#
def dfs2(r, c, depth, tmp):
    if depth > 6:
        num.add(tmp)
        return
    if 0 <= r < 4 and 0 <= c < 4:
        dfs2(r+1, c, depth+1, tmp*10 + grid[r][c])
        dfs2(r, c+1, depth+1, tmp*10 + grid[r][c])
        dfs2(r-1, c, depth+1, tmp*10 + grid[r][c])
        dfs2(r, c-1, depth+1, tmp*10 + grid[r][c])


drc = ((1, 0), (0, 1), (-1, 0), (0, -1))
for tc in range(int(input())):
    num = set()
    grid = [tuple(map(int, input().split())) for _ in range(4)]
    for x in range(4):
        for y in range(4):
            dfs2(x, y, 0, 0)
    print(f"#{tc+1} {len(num)}")
