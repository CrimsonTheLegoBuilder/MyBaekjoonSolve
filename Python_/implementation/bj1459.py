x, y, w, s = map(int, input().split())
ans1 = x*w + y*w
ans2 = s*max(x, y) + (w-s)*((x+y) % 2 == 1)
ans3 = s*min(x, y) + w*abs(x-y)
print(min(ans1, ans2, ans3))
