import matplotlib.pyplot as plt

# 주어진 점 좌표 리스트
points = [
    (80, 20),
    (100, 20),
    (100, 30),
    (80, 30),
    (100, 40),
    (100, 50),
    (80, 40),
    (80, 50),
    (70, 50),
    (70, 0),
    (80, 0),
    (80, 10),
    (90, 0),
    (100, 0),
]

points = points[::-1]

# x, y 좌표 분리
x_coords, y_coords = zip(*points)

# 시각화
plt.figure(figsize=(10, 10))
plt.scatter(x_coords, y_coords, color='blue')

# 각 점에 번호 라벨 추가
for i, (x, y) in enumerate(points):
    plt.text(x, y, f'{i+1}', fontsize=12, ha='right')

plt.xlabel('X coordinate')
plt.ylabel('Y coordinate')
plt.title('Scatter Plot of Given Points')
plt.grid(True)
plt.show()
