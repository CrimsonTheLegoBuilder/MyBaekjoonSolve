from decimal import *
a = Decimal(100000000) / (Decimal(2) ** Decimal(.5))
print((Decimal(2) ** Decimal(.5)))
b = a / (Decimal(3) ** Decimal(.5))
print(a + a + b + b)
print(f'{a + a + b * Decimal(2):.20f}')
