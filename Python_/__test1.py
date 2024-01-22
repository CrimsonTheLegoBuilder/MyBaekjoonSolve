import sys


def decode(string: str) -> None:
    print(string[::-1])
    return


if __name__ == '__main__':
    while 1:
        a = sys.stdin.readline().strip()
        if a == 'END':
            exit(0)
        # decode(a)
