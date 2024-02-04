import sys

n = int(input())

arr = list(map(int, sys.stdin.readline().strip().split()))

arr.sort()

print(arr[0] * arr[-1])
