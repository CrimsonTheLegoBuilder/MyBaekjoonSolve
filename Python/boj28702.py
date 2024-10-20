a: str = input()
b: str = input()
c: str = input()

ret: int = 0

if a.isdigit():
    ret = int(a) + 3
elif b.isdigit():
    ret = int(b) + 2
elif c.isdigit():
    ret = int(c) + 1

if not ret % 15:
    print("FizzBuzz")
elif not ret % 5:
    print("Buzz")
elif not ret % 3:
    print("Fizz")
else:
    print(ret)
