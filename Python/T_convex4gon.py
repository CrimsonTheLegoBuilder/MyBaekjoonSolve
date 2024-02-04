def convex4gon(L):
    # ADD ADDITIONAL CODE HERE!
    pass

######### SUBMIT THE ABOVE CODE ONLY #########

print(convex4gon([(7,5)])) # 0
print(convex4gon([(0,0),(0,1),(1,0),(1,1),(2,3)])) # 3
print(convex4gon([(-999,-999),(-999,999),(999,999),(999,-999),(1,2),(3,4)])) # 8
print(convex4gon([(i,i*i)for i in range(1,30)])) # 23751
