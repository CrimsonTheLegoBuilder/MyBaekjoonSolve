visited1 = [1, 0, 0, 0, 1]
visited2 = 0b10001
print(visited2)
visited = 0
print(visited)
visited |= 1 << 3
print(visited, bin(visited))
visited |= 1 << 1
print(visited, bin(visited))
visited |= 1 << 2
print(visited, bin(visited))
visited |= 1 << 6
print(visited, bin(visited))
visited &= ~(1 << 3)
print(visited, bin(visited))
print(visited & (1 << 2))
print(visited & (1 << 3))
visited ^= (1 << 3)
print(visited, bin(visited))
visited ^= (1 << 3)
print(visited, bin(visited))
