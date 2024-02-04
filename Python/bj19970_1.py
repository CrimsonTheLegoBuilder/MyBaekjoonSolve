def d(o):
 j,l=-1,[0,0]
 for i in range(len(o)):
  if o[i]=='1':l[j]+=4
  else:j+=1;l[j]=ord(o[i])-ord('A')
 return l
def m(s):x=((s[0][0]+s[1][0])>>1,(s[0][1]+s[1][1])>>1,(s[0][2]+s[1][2])>>1);return x
L=((0,2,0),(0,0,0),(2,0,0),(2,2,0),(0,2,2),(0,0,2),(2,0,2),(2,2,2))
t=d(input());P=[L[t[0]],L[t[1]]]
t=d(input());Q=[L[t[0]],L[t[1]]]
d=m(P);D=m(Q)
f=P[0]==Q[0] or P[1]==Q[1] or P[0]==Q[1] or P[1]==Q[0]
F=d==D
if f or F:print("YES")
else:print("NO")
