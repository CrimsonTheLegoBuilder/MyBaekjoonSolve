def moving_procedure(number_of_plate: int, x: int, y: int) -> None:
    if number_of_plate > 1:
        moving_procedure(number_of_plate - 1, x, 6 - x - y)

    print(x,y)

    if number_of_plate > 1:
        moving_procedure(number_of_plate - 1, 6 - x - y, y)

import sys
number_of_plate = int(sys.stdin.readline())

print(2**number_of_plate-1)
moving_procedure(number_of_plate, 1, 3)
