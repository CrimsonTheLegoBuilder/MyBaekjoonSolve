for tc in range(1, int(input())+1):
    N = int(input())
    
    tree = [0] * 1001
    for i in range(2, N+1):
        tree[i] = i//2
