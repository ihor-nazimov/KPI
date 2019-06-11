#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    char s[256];
    int i;
    double d;

    //read one word only and clear buffer
    cout << "Input word: "; 
    cin >> s; 
    //if (!cin) cout << "Error" << endl;
    cin.clear();
    cin.ignore(250, '\n');
    cout << s << endl;

    //read line to \n
    cout << "Input string: "; 
    cin.getline(s, 255);
    cout << s << endl;
    
    //read integer and clear buffer
    cout << "Input integer: ";
    cin >> i; 
    if (!cin) cout << "Error" << endl;
    cin.clear();
    cin.ignore(250, '\n');
    cout << i << endl;

    //read line to \n
    cout << "Input string: "; 
    cin.getline(s, 255);
    cout << s << endl;

    if (!cin) cout << "Error" << endl;

    cout << "Input double: ";
    cin >> d;
    if (!cin) cout << "Error" << endl;
    cin.clear();
    cin.ignore(250, '\n');
    cout << d << endl;
    
    //read line to \n
    cout << "Input string: "; 
    cin.getline(s, 255);
    cout << s << endl;

    if (!cin) cout << "Error" << endl;
    
    
    return 0;
}
