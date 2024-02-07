if __name__ == '__main__':
    AB = input().strip()
    if len(AB) == 2:
        print(int(AB[0]) + int(AB[1]))
    elif len(AB) == 3 and AB[2] == '0':
        print(int(AB[0]) + int(AB[1]) * 10)
    elif len(AB) == 3 and AB[1] == '0':
        print(int(AB[2]) + int(AB[0]) * 10)
    # elif len(AB) == 4:
    #     print(20)