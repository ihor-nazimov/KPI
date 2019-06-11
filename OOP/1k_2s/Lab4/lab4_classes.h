//#pragma once
#ifndef LAB4_CLASSES
#define LAB4_CLASSES


// #include <limits>
// #include <limits.h>
// #include <math.h>
// #include <exception>
// #include <stdexcept>
#include <iostream>
// #include <fstream>
// #include <iomanip>
// #include <string.h>
// #include <typeinfo>

// #include "lab4_classes.h"
// #include "lab4_templates.h"

// using namespace std;

//base classes
class CDate {
    protected:
        unsigned _year = 1900;
        unsigned _month = 0;
        unsigned _day = 0;
        std::ostream& _write(std::ostream& of);
        std::istream& _read(std::istream& ifile);
    public:
        CDate() {};
        CDate(unsigned yy, unsigned mm, unsigned dd): _year(yy), _month(mm), _day(dd) {};
        CDate(const CDate& src): _year(src._year), _month(src._month), _day(src._day) {};
        unsigned year(unsigned yr = 0);
        unsigned month(unsigned mm = 0);
        unsigned day(unsigned dd = 0);
        friend std::ostream& operator<< (std::ostream &out, const CDate &dt);
};


class CFuel {
    public:
        enum FuelType {UNKNOWN, AI92, AI98, DIESEL};
    private:
        double _price = 0; //ціна за літр
        FuelType _ftype = UNKNOWN;
    protected:
        std::ostream& _write(std::ostream& of);
        std::istream& _read(std::istream& ifile);
    public:
        CFuel() {};  
        CFuel(FuelType ftype, double pr): _price(pr), _ftype(ftype) {};
        CFuel(const CFuel& src): _price(src._price), _ftype(src._ftype) {};
        friend std::ostream& operator<< (std::ostream &out, const CFuel &df);
        double price(double pr = 0) { if (pr > 0) _price = pr;  return _price; }
};

class COperator {
        char* _name = 0;
        char* _surname = 0;
        CDate birthday;
    protected:
        std::ostream& _write(std::ostream& of);
        std::istream& _read(std::istream& ifile);
    public:
        COperator() {}
        COperator(const char* nm, const char* snm, unsigned yy, unsigned mm, unsigned dd);
        COperator(const COperator& src);
        ~COperator();
        friend std::ostream& operator<< (std::ostream &out, const COperator &op);
        const char* name(const char* nm=0);
        const char* surname(const char* surnm=0);
};

//inherited classes
class COperation: public CDate, public COperator, public CFuel {
    protected:
        std::ostream& _write(std::ostream& of);
        std::istream& _read(std::istream& ifile);
    public:
        double quantity = 0;
        double totalPrice = 0;
        COperation() {};
        COperation(const COperator& op, const CFuel& fuel, double qnt, double total, unsigned yy, unsigned mm, unsigned dd):
            CDate(yy, mm, dd), COperator(op), CFuel(fuel), quantity(qnt), totalPrice(total) {}; 
        COperation(const COperation& op): CDate(op), COperator(op), CFuel(op) {}; 
        operator double() { return totalPrice; }
        friend std::ostream& operator<< (std::ostream &out, const COperation &opn);
        const size_t size();
};

//1. динамический массив?
//2. добавление єлементов
class CDayBalance: public CDate { //баланс
        unsigned maxsizeofList = 0;
        unsigned sizeofList = 0;
        COperation** _operationList; //list of operation. Must be placed at and of class!
    protected:
        std::ostream& _write(std::ostream& of);
        std::istream& _read(std::istream& ifile);
    public:
        CDayBalance() {} 
        CDayBalance(unsigned yy, unsigned mm, unsigned dd, unsigned maxsize); 
        CDayBalance(const CDayBalance& src);
        ~CDayBalance();
        unsigned push_back(COperation& value);
        const COperation& operator[] (unsigned index);
        const size_t size() { return sizeofList; };
        friend std::ostream& operator<< (std::ostream &out, CDayBalance &opn);
};

#endif