import matplotlib.pyplot as plt
import numpy as np
import control as ctrl

# Corrected parameters
K_corrected = 2000 / 33488  # Gain corrected to reflect heater's actual output to the water
T = 16.74  # Time constant in seconds
L_delayed = 30  # Adjusted time delay in seconds

# Corrected plant transfer function with adjusted gain
num_corrected = [K_corrected]  # Corrected numerator with proper gain
den = [T, 1]  # Denominator (Ts + 1)

# Define Pade approximation of time delay (5 seconds) using a first-order approximation
num_delay = [-L_delayed / 2, 1]  # Numerator of Pade approximation
den_delay = [L_delayed / 2, 1]   # Denominator of Pade approximation

# Corrected plant with time delay using Pade approximation
plant_with_delay = ctrl.TransferFunction(np.polymul(num_corrected, num_delay), np.polymul(den, den_delay))

# PID controller parameters
Kp = 3.0
Ki = 0.15
Kd = 0.8

# PID transfer function
pid_num = [Kd, Kp, Ki]  # Kd * s^2 + Kp * s + Ki
pid_den = [1, 0]  # Integral action -> s
pid = ctrl.TransferFunction(pid_num, pid_den)

# Open-loop transfer function by multiplying PID and plant with delay
open_loop_delayed = ctrl.series(pid, plant_with_delay)

# Closed-loop transfer function with feedback
closed_loop_delayed = ctrl.feedback(open_loop_delayed)

# Simulating over 1 hour (3600 seconds) with the delayed system
t_long = np.linspace(0, 3600, 5000)  # Time vector for 1 hour
t_long, response_long_delayed = ctrl.step_response(closed_loop_delayed, T=t_long)

# Setting the target temperature to 92 degrees
set_point = 92  # Target temperature in degrees Celsius

# Scaling the delayed response to the target temperature
scaled_response_long_delayed = response_long_delayed * set_point

# Plotting the delayed temperature response over a longer period (1 hour) with the set point of 92 degrees
plt.figure(figsize=(14, 7))
plt.plot(t_long, scaled_response_long_delayed, label='PID Control Response (5s Delay)', color='b')
plt.axhline(set_point, color='r', linestyle='--', label='Set Point (92°C)')
plt.title('PID Control Step Response over 1 Hour with 5s Time Delay')
plt.xlabel('Time (seconds)')
plt.ylabel('Temperature (°C)')
plt.grid(True)
plt.legend()
plt.show()


# import matplotlib.pyplot as plt
#
# # Define the points for each set
# A = [
#   (2, 6),
#   (0, 4),
#   (8, 0)
# ]
# B = [
#   (4, 6),
#   (9, 4),
#   (7, 1),
#   (9, 7),
#   (2, 6),
#   (5, 5),
# ]
# AB = [
#   (7, 1),
#   (0, 4),
#   (8, 0),
# ]
# # Function to plot a polygon given a set of points
# def plot_polygon(points, color, label):
#     x, y = zip(*points)
#     plt.plot(x, y, marker='o', color=color, label=label)
#     plt.plot([x[-1], x[0]], [y[-1], y[0]], color=color)  # Close the polygon
#
# # Plotting the polygons
# plt.figure(figsize=(8, 8))
#
# plot_polygon(A, 'blue', 'A')
# plot_polygon(B, 'green', 'B')
# plot_polygon(AB, 'red', 'AB')
#
# #plt.xlim(-1, 10)
# #plt.ylim(-1, 10)
# plt.gca().set_aspect('equal', adjustable='box')
# plt.grid(True)
# plt.legend()
# plt.show()