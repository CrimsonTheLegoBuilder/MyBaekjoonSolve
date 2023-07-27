powered = [0]
t = 1
for i in range(1, 50):
    for _ in range(i):
        powered.append(t)
    t += 1
    for _ in range(i):
        powered.append(t)
    t += 1
print(powered)
