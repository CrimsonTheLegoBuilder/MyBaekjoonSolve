def sel_sort(arr):
    for i in range(len(arr)):
        min_i = i
        for j in range(i+1, len(arr)):
            if arr[j] < arr[min_i]:
                min_i = j
        arr[min_i], arr[i] = arr[i], arr[min_i]


def recur_sel_sort(x, arr):
    if x == 0:
        return
    max_i = x-1
    for i in range(x-1):
        if arr[max_i] < arr[i]:
            max_i = i
    arr[max_i], arr[x-1] = arr[x-1], arr[max_i]
    print(max_i)
    print(arr)
    recur_sel_sort(x-1, arr)
    return


a = [2, 3, 1, 4, 5, 5, 3, 6, 7]
recur_sel_sort(len(a), a)
print(a)
b = [2, 3, 1, 4, 5, 5, 3, 6, 7]
sel_sort(b)
print(b)
