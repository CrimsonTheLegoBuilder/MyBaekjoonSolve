import matplotlib.pyplot as plt

# Input data
data = """
12 99998 0 -99998 0
99999 -99999 99999 99999
-99999 -99999 99999 -99999
-99999 -99999 99999 99999
-99999 99999 -99999 -99998
99998 99999 -99999 99999
-99999 -99998 99998 99999
99999 -99999 99998 1
99998 1 99997 -99998
99997 -99998 99999 -99999
-99999 99999 -99998 -1
-99998 -1 -99997 99998
-99997 99998 -99999 99999
"""

# Parsing the input data
lines = data.strip().split('\n')
n_segments = int(lines[0].split()[0])  # Number of segments
segment_data = [list(map(int, line.split())) for line in lines[1:]]

# Plotting
fig, ax = plt.subplots()

# Loop through the segments and plot them
for idx, (x1, y1, x2, y2) in enumerate(segment_data):
    ax.plot([x1, x2], [y1, y2], marker='o', label=f'Segment {idx+1}')
    ax.text((x1 + x2) / 2, (y1 + y2) / 2, f'{idx+1}', fontsize=12, color='blue')

# Set labels and title
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_title('Segments with Points')

# Add a grid
ax.grid(True)

# Set aspect ratio to be equal
ax.set_aspect('equal')

# Show legend
# ax.legend()

# Show plot
plt.show()
