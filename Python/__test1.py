import matplotlib.pyplot as plt

# 주어진 점 좌표 리스트
points = [
    (828697006, 828697142),
    (828696973, 828696992),
    (828696695, 828696708),
    (828696461, 828696948),
    (828696473, 828696841),
    (828696315, 828697173),
    (828697073, 828696418),
    (828696309, 828696498),
    (828697265, 828696733),
    (828696764, 828697009),
    (828697052, 828696529),
    (828696609, 828696980),
    (828696576, 828697265)
]

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
