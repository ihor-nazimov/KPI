// #include <limits>
// #include <limits.h>
// #include <math.h>
// #include <exception>
// #include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>

#include <cstdio>
// #include <string.h>
// #include <typeinfo>

#include "lab4_templates.h"
#include "lab4_classes.h"

// using namespace std;

template <class C> 
void save(const char* filename, C& src) {
    std::cout << src << std::endl;
    std::ofstream ofile; 
    ofile.open(filename, std::ios::binary | std::ios::out);
    (new Container<C>(src))->save(ofile);
    ofile.close();
    std::cerr << "Class " << typeid(C).name() << " was saved" << std::endl;
}

template <class C>
C& load(const char* filename) {
    static Container<C>* dest = new Container<C>();
    std::ifstream ifile; 
    ifile.open(filename, std::ios::binary | std::ios::in);
    dest->load(ifile);
    ifile.close();
    std::cout << "Class " << typeid(C).name() << " was restored" << std::endl;
    std::cout << *dynamic_cast<C*>(dest) << std::endl;
    return *dynamic_cast<C*>(dest);
} 

int main(int argc, char const *argv[])
{
    CDate* today = new CDate(2019, 5, 29);

    COperator* queen = new COperator("Liudmyla", "Dobryvechir", 1930, 9, 9);
    COperator* director = new COperator("Panas", "Petrovych", 1912, 12, 30);
    COperator* barista = new COperator("Rognida", "Karpivna", 1920, 5, 1);
     
    CFuel* diesel = new CFuel(CFuel::DIESEL, 28.3); 
    CFuel* a98 = new CFuel(CFuel::AI98, 31.2); 
    CFuel* a92 = new CFuel(CFuel::AI92, 30.1); 

    COperator* oper;
    CFuel* fuel;
    double qty=0;

    CDayBalance* sales29 = new CDayBalance(2019, 5, 29, 300);
    for (unsigned i=0; i < 3; i++) {
        switch (rand()%3) {
            case 0:  fuel = diesel; break;
            case 1:  fuel = a98; break;
            default: fuel = a92;
        }
        switch (rand()%5) {
            case 0: case 1: case 2:  
                     oper = queen; break;
            case 3:  oper = director; break;
            default: oper = barista;
        }
        qty = static_cast<double>( rand() % 1000 ) / 10 + 30; //quantity in litres
        sales29->push_back(* new COperation(*oper, *fuel, 
            qty, 
            qty * fuel->price() * (1 + static_cast<double>( rand()%200 )/10 - 0.1),
            today->year(), today->month(), today->day() )); //total price;
    }

    save<CDayBalance>("lab4.dat", *sales29);
    load<CDayBalance>("lab4.dat");
    

    getchar();

    //CDayBalance balance;
    //double res = standardDeviation(balance);
    //cout << res << endl; 
    // getchar();   
    return 0;
}

