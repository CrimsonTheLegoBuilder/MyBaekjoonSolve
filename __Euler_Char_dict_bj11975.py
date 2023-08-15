import sys


def vertex(o):
    global x, y
    if o == 'N':
        y += 1
    if o == 'E':
        x += 1
    if o == 'W':
        x -= 1
    if o == 'S':
        y -= 1


x, y = 0, 0
dic_v = {}
dic_e = {}
N = int(sys.stdin.readline().strip())
order = str(sys.stdin.readline().strip())
for i in range(N):
    a = dic_v.setdefault((x, y), i)
    vertex(order[i])
    b = dic_v.setdefault((x, y), i+1)
    p = min(a, b)
    w = max(a, b)
    dic_e[(p, w)] = 0
V = len(dic_v)
E = len(dic_e)
# print(dic_v)
# print(dic_e)
ans = 1 + E - V
# print(E, V)
print(ans)

# x,y,v,e=0,0,{},{}
# N=int(input())
# d=input()
# for i in range(N):
#  a=v.setdefault((x,y),i)
#  o=d[i]
#  if o=='N':y+=1
#  if o=='E':x+=1
#  if o=='W':x-=1
#  if o=='S':y-=1
#  b=v.setdefault((x,y),i+1)
#  e[(min(a,b),max(a,b))]=0
# print(1+len(e)-len(v))
#
# x,y,v,e=0,0,{},{}
# N=int(input())
# O=input()
# for o in O:
#  a=v.setdefault((x,y),N)
#  if o=='N':y+=1
#  if o=='E':x+=1
#  if o=='W':x-=1
#  if o=='S':y-=1
#  N-=1
#  b=v.setdefault((x,y),N)
#  e[(min(a,b),max(a,b))]=0
# print(1+len(e)-len(v))
#
# x,y,v,e=0,0,{(0,0):0},{}
# N=int(input())
# O=input()
# for o in O:
#  a=v[(x,y)]
#  if o=='N':y+=1
#  if o=='E':x+=1
#  if o=='W':x-=1
#  if o=='S':y-=1
#  N+=1
#  b=v.setdefault((x,y),N)
#  e[(min(a,b),max(a,b))]=0
# print(1+len(e)-len(v))
#
# x,y,v,e=0,0,{(0,0):0},{}
# N=int(input())
# O=input()
# for o in O:
#  a=v[(x,y)]
#  if o=='N':y+=1
#  if o=='E':x+=1
#  if o=='W':x-=1
#  if o=='S':y-=1
#  N+=1
#  b=v.setdefault((x,y),N)
#  if a>b:a,b=b,a
#  e[(a,b)]=0
# print(1+len(e)-len(v))
#
# v,e={(0,0):0},{}
# x=y=0
# N=int(input())
# for o in input():
#  a=v[(x,y)]
#  if o=='N':y+=1
#  elif o=='E':x+=1
#  elif o=='W':x-=1
#  else:y-=1
#  N+=1
#  b=v.setdefault((x,y),N)
#  e[(min(a,b),max(a,b))]=0
# print(1+len(e)-len(v))
