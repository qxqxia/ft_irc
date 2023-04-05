import random


class color:
    reset = '\033[0m'
    red = '\033[91m'
    green = '\033[92m'
    yellow = '\033[93m'


month = [
    31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31
] # Jan is month[ 0 ]


month_name = [
    'jan', 'feb', 'mar', 'april', 'may', 'june',
    'july', 'aug', 'sep', 'oct', 'nov', 'dec']


weekday_name = [
    'Monday', 'Tuesday', 'Wednesday', 
    'Thursday', 'Friday', 'Saturday', 'Sunday']


doomsdays = [n + 28 for n in [10, 14, 14, 4, 9, 6, 11, 8, 5, 10, 7, 12 ]]
# doomsdays = [10, 14, 14, 4, 9, 6, 11, 8, 5, 10, 7, 12 ] # depr.


doomsday_of_2023 = 2


while True:

    M = random.randint(0, 11)
    D = random.randint(1, month[M]) # month[M])


    # qxia's soln
    """
    if D > doomsdays[M]:
        real_answer = (D - doomsdays[M]) % 7 + 2 # 2 is the doomsday_of_2023
    else:
        real_answer = 2 + 7 - (doomsdays[M] - D)
    """


    # trying a new way
    real_answer = doomsday_of_2023 - (doomsdays[M] - D) % 7
    real_answer += 0 if real_answer > 0 else 7


    guess = input(f'guess the weekday of this day: \n\n{month_name[M]} {D} \n')


    if guess.lower() in ['exit', 'quit', 'end', 'part', 'kill']:
        exit()
    if not guess.isalnum():
        exit()
    guess = int(guess)


    print(f'real: {real_answer}')


    if guess == real_answer:
        print(f'{color.green}correct! {color.reset}' + \
            f'{month_name[M]} {D} is a {weekday_name[real_answer - 1]}\n')
    else:
        print(f'{color.red}incorrect... {color.reset}' + \
            f'{month_name[M]} {D} is a {weekday_name[real_answer - 1]}\n')
    print('~ \n')
    # print('~~~ o(^_^o)(o^ .^)o ~~~\n')
