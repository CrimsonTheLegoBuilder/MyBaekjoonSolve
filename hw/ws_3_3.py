# ws_3_3.py
from book import decrease_book


def rental_book(name, cnt):
    decrease_book(cnt)
    print(f'{name}님이 {cnt}권의 책을 대여하였습니다.')
    return


rental_book('홍길동', 3)
name1 = str(input())
books = int(input())
rental_book(name1, books)
name2, books2 = map(str, input().split())
books2 = int(books2)
rental_book(name2, books2)
