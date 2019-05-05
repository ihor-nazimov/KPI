/*
      ios
    /     \
istream ostream
    \     /
    iostream

int x = 3;
cout << x;

cout operator<<(int x) { printf(...); }; //перевантажений оператор

cin >> x;

cerr - помилка
clog - в лог.


Маніпулятори. 
    Діють до наступного, крім setwidth (тільки наступна)
    cout << hex << x; //в hex

    char* txt = "Example";
    cout << txt; //вивести строку
    cout << (void*) txt; //вивести що???


Precision:
    для чисел - точність, для інших - кількість символів.
    поведінка залежить від флагів та типів.
    double y = 7.0;
    double z = -37.770293; 
    cout << setprecision(3) << y; //7
    cout << setprecision(3) << z; //37.7
    
    cout.flags(ios::sciientific);
    cout.precision(5);
    cout << z; //7.00000e+000
    cout << y; //3.77029e+001

    cout.flags(ios::fixed); //фіксована точка
    cout.precision(5);
    cout << z; //7.00000
    cout << y; //37.70291
*/


#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char const *argv[])
{
    char s[] = "Example";
    char* ps = s;
    double d;
    cout << s << endl;

    cin >> s;
    cin >> d;

    cout << s << endl << d << endl;

    getchar(); getchar();    
    return 0;
}
