import random


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

    # trying this way
    # ...

    guess = int(input(f'guess the weekday of this day: \n\n{month_name[M]} {D} \n'))
    
    print(f'real: {real_answer}')


    if guess == real_answer:
        print('correct!' + \
            f'{month_name[M]} {D} is a {weekday_name[real_answer - 1]}\n')
    else:
        print('incorrect... ' + \
            f'{month_name[M]} {D} is a {weekday_name[real_answer - 1]}\n')
    print('~ \n')
    # print('~~~ o(^_^o)(o^ .^)o ~~~\n')
