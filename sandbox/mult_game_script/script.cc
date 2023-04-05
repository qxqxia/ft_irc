#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(NULL));

    int a, b, res, guess;

    while (true)
    {
        a = rand() % 89 + 11;
        b = rand() % 89 + 11;
        res = a * b;

        std::cout << "Can you gimme the answer to " << a << " * " << b << "?\n\n";
        std::cin >> guess;

        if (guess == res){
            std::cout << guess << " is right!\n\n";
        }
        else{
            std::cout << guess << " is wrong!\n\n";
        }
        return 0;
    }
}