// #include <limits>
// #include <limits.h>
// #include <math.h>
// #include <exception>
// #include <stdexcept>
#include <iostream>
#include <fstream>
// #include <iomanip>
// #include <string.h>
// #include <typeinfo>

#include "lab4_templates.h"
#include "lab4_classes.h"

// using namespace std;

int main(int argc, char const *argv[])
{
    CDate* today = new CDate(2019, 5, 29);
    CDate* yesterday = new CDate(2019, 5, 28);
    
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
    std::cout << *sales29 << std::endl << std::endl;

    std::ofstream ofile; 
    ofile.open("lab4.dat", std::ios::binary | std::ios::out);
    (new Container<CDayBalance>(*sales29))->save(ofile);
    ofile.close();
    delete sales29;
    std::cout << "Was saved and deleted" << std::endl;
    getchar();

    Container<CDayBalance>* dest = new Container<CDayBalance>();
    std::ifstream ifile; 
    ifile.open("lab4.dat", std::ios::binary | std::ios::in);
    dest->load(ifile);
    ifile.close();
    std::cout << "Was restored" << std::endl;
    std::cout << *dynamic_cast<CDayBalance*>(dest);
    getchar();

    delete dest;
    //CDayBalance balance;
    //double res = standardDeviation(balance);
    //cout << res << endl;    
    return 0;
}
