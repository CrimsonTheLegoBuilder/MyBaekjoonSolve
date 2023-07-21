# ws_4_2.py
# ws_4_3.py
import requests
from pprint import pprint as print

# 무작위 유저 정보 요청 경로
API_URL = 'https://jsonplaceholder.typicode.com/users'
# API 요청
response = requests.get(API_URL)
# JSON -> dict 데이터 변환
parsed_data = response.json()
# 응답 데이터 출력
# print(response)
# 변환 데이터 출력
# print(parsed_data)
# 변환 데이터의 타입
# print(type(parsed_data))

dummy_data = []

# 특정 데이터 출력
for i in parsed_data:
    if (float(i['address']['geo']['lat']) > -80) &\
        (float(i['address']['geo']['lat']) < 80) &\
            (float(i['address']['geo']['lng']) > -80) &\
            (float(i['address']['geo']['lng']) < 80):
        dic = {
            'company': i['company']['name'],
            'lat': i['address']['geo']['lat'],
            'lng': i['address']['geo']['lng'],
            'name': i['username']
        }
        dummy_data.append(dic)

print(dummy_data)
