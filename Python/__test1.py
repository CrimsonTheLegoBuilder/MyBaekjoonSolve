import matplotlib.pyplot as plt

# Define the points for each set
A = [
  (4, 3),
  (2, 4),
  (2, 1),
]
B = [
  (2, 4),
  (2, 2),
  (1, 0),
]
AB = [
  (3, 3),
  (2, 4),
  (0, 5),
  (1, 0),
]
# Function to plot a polygon given a set of points
def plot_polygon(points, color, label):
    x, y = zip(*points)
    plt.plot(x, y, marker='o', color=color, label=label)
    plt.plot([x[-1], x[0]], [y[-1], y[0]], color=color)  # Close the polygon

# Plotting the polygons
plt.figure(figsize=(8, 8))

plot_polygon(A, 'blue', 'A')
plot_polygon(B, 'green', 'B')
plot_polygon(AB, 'red', 'AB')

#plt.xlim(-1, 10)
#plt.ylim(-1, 10)
plt.gca().set_aspect('equal', adjustable='box')
plt.grid(True)
plt.legend()
plt.show()