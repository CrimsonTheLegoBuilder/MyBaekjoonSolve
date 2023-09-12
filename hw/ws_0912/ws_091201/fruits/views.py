from django.shortcuts import render

# Create your views here.
def hf(request):
    context = {
        "fruit_list": ["귤","딸기","사과","감","바나나","파인애플","구아바", "복숭아", "망고스틴"],
        "hate": ["사과","구아바"],
        }
    return render(request, 'fruits/fruits.html', context)


# fruit_list = ["귤","딸기","사과","감","바나나","파인애플","구아바", "복숭아", "망고스틴"]
# hate = ["사과","구아바"]
