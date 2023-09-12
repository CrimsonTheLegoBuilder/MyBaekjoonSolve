from django.shortcuts import render

# Create your views here.
def throw(request):
    context = {}
    return render(request, "calculators/throw.html", context)


def catch(request):
    num1 = request.GET.get('num1')
    num2 = request.GET.get('num2')
    num1 = int(num1)
    num2 = int(num2)
    context = {
        'num1' : num1,
        'num2' : num2,
        'num3' : num1 + num2,
        'num4' : num1 - num2,
        'num5' : num1 * num2,
        'num6' : num1 / num2 if num2 != 0 else 'error',
    }
    return render(request, "calculators/catch.html", context)
