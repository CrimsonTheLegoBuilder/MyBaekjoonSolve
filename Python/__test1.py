import matplotlib.pyplot as plt

# 주어진 점 좌표 리스트
points = [
    (132, 465),
    (107, 334),
    (123, 242),
    (114, 128),
    (124, 50),
    (165, 40),
    (177, 88),
    (194, 170),
    (186, 208),
    (190, 273),
    (197, 332),
    (189, 371),
    (189, 412),
    (226, 425),
    (285, 410),
    (286, 364),
    (278, 312),
    (241, 295),
    (215, 294),
    (204, 283),
    (200, 261),
    (208, 238),
    (250, 234),
    (319, 258),
    (332, 301),
    (341, 381),
    (317, 431),
    (220, 471),
    (162, 472),
]

points = points[::-1]

# x, y 좌표 분리
x_coords, y_coords = zip(*points)

# 시각화
plt.figure(figsize=(10, 10))
plt.scatter(x_coords, y_coords, color='blue')

# 점을 순서대로 잇는 선 추가
plt.plot(x_coords, y_coords, linestyle='-', color='red', marker='o')

# 각 점에 번호 및 좌표 라벨 추가
for i, (x, y) in enumerate(points):
    plt.text(x, y, f'{i+1} ({x}, {y})', fontsize=10, ha='right')

plt.xlabel('X coordinate')
plt.ylabel('Y coordinate')
plt.title('Scatter Plot of Given Points with Connecting Lines and Coordinates')
plt.grid(True)
plt.show()