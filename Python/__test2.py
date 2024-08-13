import matplotlib.pyplot as plt

# 주어진 점 좌표 리스트
pos = [
    (-9, 4),
    (-8, 6),
    (-5, 0),
    (-3, 0),
    (4, -10),
]

# x, y 좌표 분리
x_coords_pos, y_coords_pos = zip(*pos)

# 시각화
plt.figure(figsize=(10, 10))
plt.plot(x_coords_pos, y_coords_pos, 'ro-', label='Points pos')  # pos 점들 표시 (빨간색)

# 각 점에 번호 및 좌표 라벨 추가 (pos 점들)
for i, (x, y) in enumerate(pos):
    plt.text(x, y, f'pos{i+1} ({x}, {y})', fontsize=10, ha='right')

plt.xlabel('X coordinate')
plt.ylabel('Y coordinate')
plt.title('Scatter Plot of Given Points A and B')
plt.grid(True)
plt.legend()
plt.gca().set_aspect('equal', adjustable='box')

plt.show()