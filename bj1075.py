Dividend = str(input())
Divisor = int(input())

if len(Dividend) <= 2:
    Dividend = int(1)
    while Dividend % Divisor:
        ans = Dividend % Divisor
        Dividend += 1
    if Dividend <= 9:
        Dividend = '0' + str(Dividend)
    print(str(Dividend)[-2:])

else:
    Dividend = int(Dividend[:-2] + '00')
    while Dividend % Divisor:
        ans = Dividend % Divisor
        Dividend += 1
    print(str(Dividend)[-2:])
