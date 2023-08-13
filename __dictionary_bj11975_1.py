x,y,v,e=0,0,{},{}
N=int(input())
d=input()
for i in range(N):
 a=v.setdefault((x,y),i)
 o=d[i]
 if o=='N':y+=1
 if o=='E':x+=1
 if o=='W':x-=1
 if o=='S':y-=1
 b=v.setdefault((x,y),i+1)
 e[(min(a,b),max(a,b))]=0
print(1+len(e)-len(v))
