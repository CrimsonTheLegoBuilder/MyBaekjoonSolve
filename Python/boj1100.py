MAP: list[str] = [input() for _ in range(8)]
cnt: int = 0
for i in range(8):
    for j in range(8):
        if not (i + j) % 2:
            cnt += MAP[i][j] == 'F'
# print(cnt)
