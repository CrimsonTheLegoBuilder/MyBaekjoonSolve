import matplotlib.pyplot as plt

# Define the points
points = H = [
    (5, 6),
    (114, 32),
    (303, 362),
    (383, 345),
    (493, 490),
    (884, 500),
    (898, 267),
    (708, 193),
    (700, 241),
    (641, 219),
    (694, 386),
    (510, 274),
    (608, 140),
    (319, 78),
    (302, 5),
    (478, 23),
    (489, 8),
    (886, 5),
    (924, 139),
    (964, 47),
    (975, 385),
    (875, 545),
    (948, 511),
    (967, 753),
    (832, 595),
    (818, 760),
    (596, 788),
    (702, 715),
    (799, 751),
    (780, 506),
    (513, 709),
    (481, 768),
    (169, 735),
    (314, 771),
    (113, 776),
    (9, 464),
    (469, 744),
    (439, 616),
    (421, 658),
    (408, 594),
    (468, 612),
    (507, 678),
    (598, 638),
    (636, 574),
    (129, 309),
    (7, 362),
]

# Extract x and y coordinates
x_coords, y_coords = zip(*points)

# Define dx and dy for the grid spacing
dx = 98  # Interval for x-axis
dy = 79  # Interval for y-axis

# Create the plot
plt.figure(figsize=(10, 8))

# Plot the points and connect them in order, including closing the loop
plt.plot(x_coords + (x_coords[0],), y_coords + (y_coords[0],), marker='o', color='blue', linestyle='-', markersize=6)

# Label each point with its index
for i, (x, y) in enumerate(points):
    plt.text(x, y, f'{i+1}', fontsize=12, ha='right', color='red')

# Set grid with specific intervals
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.xticks(range(min(x_coords) - 1, max(x_coords) + 2, dx))
plt.yticks(range(min(y_coords) - 1, max(y_coords) + 2, dy))

# Set axis labels and title
plt.title('Points with Custom Grid Spacing')
plt.xlabel('X-axis')
plt.ylabel('Y-axis')

# Show the plot
plt.show()