Size = 100
Q = [0] * Size
front = rear = -1

rear += 1
Q[rear] = 1
rear += 1
Q[rear] = 2
rear += 1
Q[rear] = 3

print(f'peek : {Q[front + 1]}')

while front != rear:
    front += 1
    print(Q[front])
print(front, rear)
print(Q)
