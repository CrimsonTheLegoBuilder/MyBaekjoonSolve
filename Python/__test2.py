import matplotlib.pyplot as plt

# Define the points
points = [
    (129, 217),
    (169, 217),
    (169, 157),
    (219, 157),
    (219, 197),
    (259, 217),
    (209, 317),
    (169, 317),
    (169, 287),
    (149, 247),
    (129, 247),
]

# Extract x and y coordinates
x_coords, y_coords = zip(*points)

# Define dx and dy for the grid spacing
dx = 40  # Interval for x-axis
dy = 60  # Interval for y-axis

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