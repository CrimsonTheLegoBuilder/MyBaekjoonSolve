import matplotlib.pyplot as plt
import numpy as np

# 주어진 점 좌표 리스트
points = [
  (11, 94),
  (16, 94),
  (16, 96),
  (18, 96),
  (18, 98),
  (19, 98),
  (19, 95),
  (17, 95),
  (17, 93),
  (27, 93),
  (27, 100),
  (25, 100),
  (25, 98),
  (21, 98),
  (21, 104),
  (23, 104),
  (23, 106),
  (16, 106),
  (16, 101),
  (14, 101),
  (14, 99),
  (8, 99),
  (8, 103),
  (10, 103),
  (10, 105),
  (3, 105),
  (3, 95),
  (5, 95),
  (5, 97),
  (8, 97),
  (8, 96),
  (6, 96),
  (6, 94),
  (4, 94),
  (4, 84),
  (6, 84),
  (6, 86),
  (9, 86),
  (9, 85),
  (7, 85),
  (7, 83),
  (5, 83),
  (5, 73),
  (7, 73),
  (7, 75),
  (8, 75),
  (8, 72),
  (6, 72),
  (6, 70),
  (16, 70),
  (16, 77),
  (14, 77),
  (14, 75),
  (10, 75),
  (10, 81),
  (12, 81),
  (12, 83),
  (17, 83),
  (17, 90),
  (15, 90),
  (15, 88),
  (9, 88),
  (9, 92),
  (11, 92),
]

# x, y 좌표 분리
x_coords, y_coords = zip(*points)

# 직선의 x 범위 설정 (x 좌표의 최소값과 최대값 이용)
x_line = np.linspace(min(x_coords), max(x_coords), 400)
# 직선의 y 값 계산
y_line = (60821.9840097030 - 2.5477706749 * x_line) / 9.7139154346

# 시각화
plt.figure(figsize=(10, 10))
plt.plot(x_coords, y_coords, color='blue', label='Points', marker='o') # 점들을 순서대로 이음
# 각 점에 번호 및 좌표 라벨 추가
for i, (x, y) in enumerate(points):
    plt.text(x, y, f'{i+1} ({x}, {y})', fontsize=10, ha='right')

plt.xlabel('X coordinate')
plt.ylabel('Y coordinate')
plt.title('Scatter Plot of Given Points with Line')
plt.grid(True)
plt.legend()
plt.gca().set_aspect('equal', adjustable='box')
plt.show()