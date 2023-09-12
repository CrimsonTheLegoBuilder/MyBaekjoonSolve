# 1. 현재 폴더에서 django 프로젝트(my_pjt)와 앱(my_app)를 만들고 서버를 실행하기 위한 bash 명령어를 주석으로 작성하시오.
'''
python -m venv venv  / 가상환경 만들기
source venv/Scripts/activate  / 가상환경 활성화
pip list  / 라이브러리 있는지 확인
pip freeze > requirements.txt  / 상대방에 전달할 패키지 목록 생성
pip install Django  / 장고 설치
django-admin startproject my_pjt  / my_pjt 생성
(manage.py 경로와 같은 경로인지 확인 후)
python manage.py runserver  / 장고 서버 실행
python manage.py startapp my_app  / my_app 생성
'''


# 2. http://127.0.0.1:8000/hello로 받은 요청을 통해 my_app 앱의 views.py에 있는 hello 함수를 실행시킬 수 있도록 아래 urls.py를 작성하시오
from django.urls import path
from my_app import views
urlpatterns = [
    path('my_app/', views.hello)
]
