# book.py
number_of_book = 100


def decrease_book(i=int):
    global number_of_book
    number_of_book -= i
    print(f'현재 남은 책의 수: {number_of_book}')
    return number_of_book
