import matplotlib.pyplot as plt

# 주어진 점 좌표 리스트
pos = [
    (0, 0),
    (6, 0),
    (6, 2),
    (5, 2),
    (5, 1),
    (1, 1),
    (1, 4),
    (0, 4),
]
A = [
    (0, 0)
]
B = [
    (0, 0)
]

# x, y 좌표 분리
x_coords_pos, y_coords_pos = zip(*pos)
x_coords_A, y_coords_A = zip(*A)
x_coords_B, y_coords_B = zip(*B)

# 시각화
plt.figure(figsize=(10, 10))
plt.plot(x_coords_pos, y_coords_pos, 'ro-', label='Points pos')  # pos 점들 표시 (빨간색)
plt.plot(x_coords_A, y_coords_A, 'bo-', label='Points A')  # A 점들 표시 (파란색)
plt.plot(x_coords_B, y_coords_B, 'go-', label='Points B')  # B 점들 표시 (녹색)

# 각 점에 번호 및 좌표 라벨 추가 (pos 점들)
for i, (x, y) in enumerate(pos):
    plt.text(x, y, f'pos{i+1} ({x}, {y})', fontsize=10, ha='right')

# 각 점에 번호 및 좌표 라벨 추가 (A 점들)
for i, (x, y) in enumerate(A):
    plt.text(x, y, f'A{i+1} ({x}, {y})', fontsize=10, ha='right')

# 각 점에 번호 및 좌표 라벨 추가 (B 점들)
for i, (x, y) in enumerate(B):
    plt.text(x, y, f'B{i+1} ({x}, {y})', fontsize=10, ha='right')

plt.xlabel('X coordinate')
plt.ylabel('Y coordinate')
plt.title('Scatter Plot of Given Points A and B')
plt.grid(True)
plt.legend()
plt.gca().set_aspect('equal', adjustable='box')

# 눈금을 정수 단위로 설정
plt.xticks(range(min(x_coords_pos + x_coords_A + x_coords_B), max(x_coords_pos + x_coords_A + x_coords_B) + 1))
plt.yticks(range(min(y_coords_pos + y_coords_A + y_coords_B), max(y_coords_pos + y_coords_A + y_coords_B) + 1))

plt.show()