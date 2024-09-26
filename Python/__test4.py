D = {
    "A": 0,
    "G": 1,
    "C": 2,
    "T": 3,
}

T = [
    ["A", "C", "A", 'G'],
    ["C", "G", "T", 'A'],
    ["A", "T", "C", 'G'],
    ["G", "A", "G", 'T'],
]

N: int = int(input())
DNA: list[str] = list(str(input()))
for i in range(N - 2, 0, -1):
    DNA[i] = T[D[DNA[i]]][D[DNA[i + 1]]]
    DNA.pop()
# print(DNA[0])
