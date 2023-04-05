import random

while True:
    a, b = random.randint(11, 99), random.randint(11, 99)
    res = a * b
    guess = int(input(f'can you gimme the answer to {a} * {b}? \n\n'))
    print(f'{guess} is right!\n') if res == guess else print(f'{guess} is wrong!\n')
    
