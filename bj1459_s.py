x,y,w,s=map(int,input().split());print(min(x*w+y*w,s*max(x,y)+(w-s)*((x+y)%2>0),s*min(x,y)+w*abs(x-y)))
