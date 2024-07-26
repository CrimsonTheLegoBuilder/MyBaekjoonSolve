import matplotlib.pyplot as plt
import numpy as np

# 주어진 점 좌표 리스트
points = [
    (1, 5),
    (-1, 1),
    (0, -1),
    (2, 1),
    (4, 5),
    (3, 7),
]

# x, y 좌표 분리
x_coords, y_coords = zip(*points)

# 직선의 x 범위 설정 (x 좌표의 최소값과 최대값 이용)
x_line = np.linspace(min(x_coords), max(x_coords), 400)
# 직선의 y 값 계산
y_line = (60821.9840097030 - 2.5477706749 * x_line) / 9.7139154346

# 시각화
plt.figure(figsize=(10, 10))
plt.scatter(x_coords, y_coords, color='blue', label='Points')
# 각 점에 번호 및 좌표 라벨 추가
for i, (x, y) in enumerate(points):
    plt.text(x, y, f'{i+1} ({x:.3f}, {y:.3f})', fontsize=10, ha='right')

plt.xlabel('X coordinate')
plt.ylabel('Y coordinate')
plt.title('Scatter Plot of Given Points with Line')
plt.grid(True)
plt.legend()
plt.gca().set_aspect('equal', adjustable='box')
plt.show()