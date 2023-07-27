# ws_8_4.py
# ws_8_3.py
# ws_8_2.py
# ws_8_1.py

# 아래 클래스를 수정하시오.
class Animal:
    num_of_animal = 0


class Dog(Animal):
    def __init__(self):
        super().__init__()
        Animal.num_of_animal += 1

    @staticmethod
    def bark():
        print('bark! bark!')


class Cat(Animal):
    def __init__(self):
        super().__init__()
        Animal.num_of_animal += 1

    @staticmethod
    def meow():
        print('meow~')


class Pet(Dog, Cat):
    def __init__(self, sound):
        super().__init__()
        self.sound = sound

    def make_sound(self):
        print(f'{self.sound}')

    @staticmethod
    def play():
        print('play with pet')

    @staticmethod
    def access_num_of_animal():
        return f'{Animal.num_of_animal}'


dog1 = Dog()
# dog1.bark()
# print(Pet.access_num_of_animal())
cat1 = Cat()
# cat1.meow()
# print(Pet.access_num_of_animal())
pet1 = Pet("growl~")
pet1.make_sound()
pet1.bark()
pet1.meow()
pet1.play()
