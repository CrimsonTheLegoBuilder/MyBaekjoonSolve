RS: dict = {
    "black": (0, 1),
    "brown": (1, 10),
    "red": (2, 100),
    "orange": (3, 1000),
    "yellow": (4, 10000),
    "green": (5, 100000),
    "blue": (6,	1000000),
    "violet": (7, 10000000),
    "grey":	(8,	100000000),
    "white": (9, 1000000000),
}

rs: int = 0
A: str = input()
rs += RS[A][0] * 10
B: str = input()
rs += RS[B][0] * 1
C: str = input()
rs *= RS[C][1]

# print(rs)
