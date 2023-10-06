count = x = y = result = 0
p, slopeList, Light = [], [], []


def search(x, y, pos, angle):
    pos = pos[:]
    if x == -1 or x == w or y == -1 or y == h or box[y][x] == '#':  # 장외는 교차 없이 끝까지 도착한 것임. 점 수집
        p.append(pos)
        return
    x1 = x + 1 if angle[0] > 0 else x - 1  # 옆칸
    x2 = x + 1 if angle[0] > 0 else x  # 교점
    y1 = y + 1 if angle[0] * angle[1] > 0 else y - 1  # 옆칸
    y2 = y + 1 if angle[0] * angle[1] > 0 else y  # 교점
    x_ny = pos[0] + ((y2 - pos[1]) / (angle[1]))
    y_nx = pos[1] + (angle[1]) * (x2 - pos[0])
    if y + 1e-7 < y_nx and y_nx < y + 1 - 1e-7 and (x_ny < x - 1e-7 or x_ny > x + 1 + 1e-7):
        pos = [x2, y_nx]
        search(x1, y, pos, angle)
    elif x + 1e-7 < x_ny and x_ny < x + 1 - 1e-7 and (y - 1e-7 > y_nx or y_nx > y + 1 + 1e-7):
        pos = [x_ny, y2]
        search(x, y1, pos, angle)
    else:
        pos = [x2, y2]
        if angle[1] > 0 and (x1 == -1 or x1 == w or box[y][x1] == '#'):
            p.append(pos)
        if angle[1] < 0 and (y1 == -1 or y1 == h or box[y1][x] == '#'):
            p.append(pos)
        search(x1, y1, pos, angle)  # 왼쪽부터 쓸고 나서 재귀로 끝까지 찍고 난 뒤에 오른쪽으로 다시 돌아간다
        if angle[1] > 0 and (y1 == -1 or y1 == h or box[y1][x] == '#'):
            p.append(pos)
        if angle[1] < 0 and (x1 == -1 or x1 == w or box[y][x1] == '#'):
            p.append(pos)


h, w = map(int, input().split())
box = [input() for _ in range(h)]
for i in range(h):
    for j in range(w):
        if box[i][j] == '*':
            x = j
            y = i
            Light = [j + 0.5, i + 0.5]  # 광원의 위치
        elif box[i][j] == '#':  # 벽 세기
            count += 1

for i in range(2):
    for j in range(2):
        slope = [0, 0]
        j = w * j
        i = h * i
        slope = [1 if j - Light[0] > 0 else -1, (i - Light[1]) / (j - Light[0])]  # 네 귀퉁이 각도 수집
        slopeList.append(slope)

for i in range(h + 1):
    for j in range(w + 1):
        slope = [1 if j - Light[0] > 0 else -1, (i - Light[1]) / (j - Light[0])]  # 모든 점에 대해 각도 수집
        if slope not in slopeList:
            slopeList.append(slope)

for i in sorted(slopeList):  # 시계 방향 정렬
    search(x, y, Light, i)  # 오목 다각형 hull 수집


for i in range(len(p)):
    j = (i + 1) % len(p)
    result += abs((Light[0] * p[i][1] + p[i][0] * p[j][1] + p[j][0] * Light[1]) - (p[i][0] * Light[1] + p[j][0] * p[i][1] + Light[0] * p[j][1])) / 2
print(f'{w * h - result - count:.13f}')