# import matplotlib.pyplot as plt
#
# # 주어진 점 좌표 리스트
# points = [
#     (7720, 2038),
#     (8160, 2661),
#     (6427, 4528),
#     (7007, 5869),
#     (7399, 6513),
#     (2037, 4071),
#     (995, 3028),
#     (3176, 881),
#     (5040, 1874),
# ]
#
# points = points[::-1]
#
# # x, y 좌표 분리
# x_coords, y_coords = zip(*points)
#
# # 시각화
# plt.figure(figsize=(10, 10))
# plt.scatter(x_coords, y_coords, color='blue')
#
# # 점을 순서대로 잇는 선 추가
# plt.plot(x_coords, y_coords, linestyle='-', color='red', marker='o')
#
# # 각 점에 번호 및 좌표 라벨 추가
# for i, (x, y) in enumerate(points):
#     plt.text(x, y, f'{i+1} ({x}, {y})', fontsize=10, ha='right')
#
# plt.xlabel('X coordinate')
# plt.ylabel('Y coordinate')
# plt.title('Scatter Plot of Given Points with Connecting Lines and Coordinates')
# plt.grid(True)
# plt.show()

print(330 / (330 + 470) * 13)
