import random

class color:
    reset = '\033[0m'
    red = '\033[91m'
    green = '\033[92m'
    yellow = '\033[93m'

while True:
    a, b = random.randint(11, 99), random.randint(11, 99)
    res = a * b
    guess = int(input(f'can you gimme the answer to {a} * {b}? \n\n'))
    print(f'{guess} is {color.green}right{color.reset}\n') \
        if res == guess else \
        print(f'{guess} is {color.red}wrong{color.reset}\n')
    
