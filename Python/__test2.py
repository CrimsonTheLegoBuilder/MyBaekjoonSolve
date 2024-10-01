import matplotlib.pyplot as plt

# Given points with indices
A = [
    (3, 1),
    (8, 7),
    (9, 5),
    (11, 5),
    (12, 4),
    (9, 1),
    (13, 6),
]

N = [(9, 3)]

# Plot the points
plt.figure(figsize=(8, 6))
# Plot A points
for idx, (x, y) in enumerate(A):
    plt.scatter(x, y, color='blue', label='A점' if idx == 0 else "")
    plt.text(x, y, f'{idx}', fontsize=12, ha='right')

# Plot N points
for idx, (x, y) in enumerate(N):
    plt.scatter(x, y, color='red', label='N점' if idx == 0 else "")
    plt.text(x, y, f'N', fontsize=12, ha='right', color='red')

plt.title("A점과 N점의 위치")
plt.xlabel("X축")
plt.ylabel("Y축")
plt.grid(True)
plt.xlim(0, 15)
plt.ylim(0, 10)
plt.legend()
plt.show()
