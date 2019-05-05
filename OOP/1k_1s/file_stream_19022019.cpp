/*Файлові потоки:
    Ієрархія класів потоків:
              ios
            /     \
        istream ostream
        /    \     /  \
  ifstream   iostream  ofstream
                |
             fstream

    Відкриття потоків (конструктори классів):
        ifstream(const char* name, int mode = ios_base::in);
        ofstream(const char* name, int mode = ios_base::out | ios_base::trunc);
        fstream(const char* name, int mode = ios_base::in | ios:base::out);
        
        //для читання тексту
        ifstream Mnfile("example.txt");
        Mnfile >> buf >> buf1;
        Mnfile.close;

        //для запису тексту
        ofstream Mnfile("example.txt");
        Mnfile << buf;
        Mnfile.close;

        //для потокового запису у двійковому вигляді
        class Tovar{..};
        Tovar tt;
        ofstream Mnfile("example.txt", ios_base::append | ios_base::binary);
        Mnfile.write ( (char*) & tt, sizeof(tt));
        Mnfile.close;

        //потокове читання у двійковому вигляді
        class Tovar{..};
        Tovar td;
        ifstream Mnfile("example.txt", ios_base::binary);
        Mnfile.read ( (char*) &td, sizeof(td));
        Mnfile.close;

        //произвольное чтение/запись
        //можно одновременно (ios_base::in | ios_base::in | ios_base::binary)
        Функції для довільного запису до будь-якої позиції файлу. 
        seekg(); //g=get, p=put
        seerp();
        tellg();
        tellp();

        istream &seekg()
*/

#include <iostream>

class Person {
        char name[255]; 
    public:
        void showData() { cout << name; }
}

int main(int argc, char const *argv[])
{
    Person pers;
    ifstream infile;
    infile.open("group.dat", ios_base::binary);
    infile.seekg(0, ios_base::end);
    int endposition = infile.tellg();
    int n = endposition/sizeof(Person);
    
    cin >> n;
    int position = (n-1)*sizeof(Person);
    infile seekg(position);
    infile.read((char*)&pers, sizeof(pers));
    pers.showData();
    return 0;
}
