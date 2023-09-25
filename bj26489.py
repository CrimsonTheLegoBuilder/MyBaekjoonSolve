cnt = 0
while 1:
    try:
        a = input()
        if a:
            cnt += 1
    except:
        print(cnt)
        exit(0)
