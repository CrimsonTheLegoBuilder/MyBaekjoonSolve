# a,b,c=map(int,input().split())
# print((a+b+c)//3+(not a%3==b%3==c%3))

a,b,c=map(int,input().split())
print((a+b+c)//3-(a%3==b%3==c%3)+1)