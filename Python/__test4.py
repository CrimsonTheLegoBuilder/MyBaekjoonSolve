import matplotlib.pyplot as plt

def plot_shapes_four(h, hpi, hi, hj):
    # Unpack each shape into x and y coordinates and add the first point at the end to close the shape
    h_x, h_y = zip(*h, h[0])
    hpi_x, hpi_y = zip(*hpi, hpi[0])
    hi_x, hi_y = zip(*hi, hi[0])
    hj_x, hj_y = zip(*hj, hj[0])

    # Set up the plot
    plt.figure(figsize=(8, 6))

    # Plot each shape with different styles and labels, ensuring they are closed
    plt.plot(h_x, h_y, marker='o', linestyle='-', color='purple', label='H')
    plt.plot(hpi_x, hpi_y, marker='o', markersize=10, linestyle='-', color='b', label='HPI')
    plt.plot(hi_x, hi_y, marker='o', linestyle='-', color='g', label='HI')
    plt.plot(hj_x, hj_y, marker='o', linestyle='-', color='r', label='HJ')

    # Label settings
    plt.xlabel("X 축")
    plt.ylabel("Y 축")
    plt.title("H, HPI, HI, HJ 도형 그래프")
    plt.legend()
    plt.grid(True)
    plt.show()

# Sample data for H
H = [
(5.000000000000000, 0.000000000000000),
(17.000000000000000, 3.000000000000000),
(7.000000000000000, 10.000000000000000),
]
hpi = [
(7.969696969696971, 0.742424242424242),
(12.000000000000000, 6.499999999999999),
(7.000000000000000, 10.000000000000000),
]
HI = [
(5.000000000000000, 0.000000000000000),
(7.969696969696970, 0.742424242424242),
(12.000000000000000, 6.500000000000000),
(7.000000000000000, 10.000000000000000),
]
HJ = [
(12.000000000000000, 6.500000000000000),
(7.000000000000000, 10.000000000000000),
(7.969696969696970, 0.742424242424242),
]
# Call the function to plot all four closed shapes
plot_shapes_four(H, hpi, HI, HJ)