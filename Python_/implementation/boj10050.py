def solve(n, i):
    if n == 3:
        print(f"{i + 1} to {i - 2}")
        print(f"{i + 4} to {i + 1}")
        print(f"{i + 2} to {i - 4}")
        return
    if n == 4:
        print(f"{i + 5} to {i - 2}")
        print(f"{i + 2} to {i + 5}")
        print(f"{i - 1} to {i + 2}")
        print(f"{i + 6} to {i - 1}")
        return
    if n == 5:
        print(f"{i + 7} to {i - 2}")
        print(f"{i + 2} to {i + 7}")
        print(f"{i + 5} to {i + 2}")
        print(f"{i - 1} to {i + 5}")
        print(f"{i + 8} to {i - 1}")
        return
    if n == 6:
        print(f"{i + 9} to {i - 2}")
        print(f"{i + 6} to {i + 9}")
        print(f"{i + 1} to {i + 6}")
        print(f"{i + 5} to {i + 1}")
        print(f"{i - 1} to {i + 5}")
        print(f"{i + 10} to {i - 1}")
        return
    if n == 7:
        print(f"{i + 11} to {i - 2}")
        print(f"{i + 4} to {i + 11}")
        print(f"{i + 7} to {i + 4}")
        print(f"{i + 2} to {i + 7}")
        print(f"{i + 8} to {i + 2}")
        print(f"{i - 1} to {i + 8}")
        print(f"{i + 12} to {i - 1}")
        return
    print(f"{i + n * 2 - 3} to {i - 2}")
    print(f"{i + 2} to {i + n * 2 - 3}")
    solve(n - 4, i + 4)
    print(f"{i - 1} to {i + n * 2 - 6}")
    print(f"{i + n * 2 - 2} to {i - 1}")
    return


if __name__ == "__main__":  # boj10050 from wnwoghd22
    N = int(input())
    solve(N, 1)
