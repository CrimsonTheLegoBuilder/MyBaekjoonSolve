import requests
from pprint import pprint as print

API_URL = 'https://jsonplaceholder.typicode.com/users'
response = requests.get(API_URL)
parsed_data = response.json()

dummy_data = []
black_list = ['Hoeger LLC', 'Keebler LLC', 'Yost and Sons', 'Johns Group', 'Romaguera-Crona']

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
# print(dummy_data)

censored_user_list = []


def create_user(dict_x):
    for k in dict_x:
        dict1 = {
            k['company']: [k['name']]
        }
        if censorship(dict1):
            censored_user_list.append(dict1)


def censorship(x):
    # print(x)
    for c_name, u_name in x.items():
        if c_name in black_list:
            print(f'{c_name} 소속의 {u_name} 은/는 등록할 수 없습니다.')
            return False
    print('이상 없습니다')
    return True


create_user(dummy_data)
print(censored_user_list)
