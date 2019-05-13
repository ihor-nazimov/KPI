#include <limits>
#include <limits.h>
#include <math.h>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <string.h>

using namespace std;

class CDate {
        unsigned _year = 1900;
        unsigned _month = 0;
        unsigned _day = 0;
    public:
        CDate() {};
        CDate(unsigned yy, unsigned mm, unsigned dd): _year(yy), _month(mm), _day(dd) {};
        const CDate& set(unsigned yy, unsigned mm, unsigned dd) {
            _year = yy;
            _month = mm;
            _day = dd;
            return this;
        }
        friend std::ostream& operator<< (std::ostream &out, const CDate &dt) {
            out.width(4); out.precision(4); out.fixed;
            out << dt._day << '.' << dt._month << '.';
            out << fixed << right << setw(4) << setprecision(4) << dt._year;
            return out;
        };
};

class CFuel {
    public:
        enum FuelType {UNKNOWN, AI92, AI98, DIESEL};
    private:
        double price = 0; //ціна за літр
        FuelType ftype = UNKNOWN;
    public:
        CFuel() {};  
        CFuel(FuelType ftype, double price): price(price), ftype(ftype) {};
        friend std::ostream& operator<< (std::ostream &out, const CFuel &df) {
            out << fixed << setw(8) << setprecision(2) << df.price << '|';
            out << fixed << setprecision(10) << left;
            switch (df.ftype) {
                case CFuel::UNKNOWN:   
                    out << "unknown"; break;
                case CFuel::AI92:
                    out << "AI-92"; break;
                case CFuel::AI98:
                    out << "AI-98"; break;
                case CFuel::DIESEL:
                    out << "diesel"; break;
                default:
                    out << "error"; 
            }
            return out;
        };
};

class COperator {
        char* name = 0;
        char* surname = 0;
        CDate birthday;
    public:
        COperator() {}
        COperator(const char* nm, const char* snm, unsigned yy, unsigned mm, unsigned dd) {
            birthday.set(yy, mm, dd);
            name = new char[strlen(nm)+1]; strcpy(name, nm);
            surname = new char[strlen(snm)+1]; strcpy(surname, snm);
        }
        ~COperator() {
            if (name) delete[] name;
            if (surname) delete[] surname;
        }
        friend std::ostream& operator<< (std::ostream &out, const COperator &op) {
            out << fixed << left << setw(12) << setprecision(12) << op.surname << '|' <<
            fixed << left << setw(8) << setprecision(8) << op.name << '|' <<
            fixed << op.birthday;
            return out;
        };
};

class COperation {
        const COperator* _operator = 0;
        const CFuel* _fuel;
        double quantity = 0;
        double totalPrice = 0;
        CDate saleDate;
    public:
        COperation(): _fuel(0) {};
        COperation(const COperator &op, const CFuel &fuel, double qnt, double total, unsigned yy, unsigned mm, unsigned dd):
            quantity(qnt), totalPrice(total) {
            _operator = &op;
            _fuel = &fuel;
            saleDate.set(yy, mm, dd);
        } 
        friend std::ostream& operator<< (std::ostream &out, const COperation &opn) {
            out << saleDate << '|' << opn._operator << '|' << opn._fuel << '|' <<
            fixed << left << setw(10) << setprecision(3) << opn.quantity << '|' <<
            fixed << left << setw(10) << setprecision(2) << opn.totalPrice;
            return out;
        };
};

class CDayBalance { //баланс
        CDate date;
        unsigned sizeofList = 0;
        COperation* _operationList[]; //list of operation. Must be placed at and of class!
    public:
        CDayBalance() {
            _operationList = new COperation [0];
        } 
        // const double total(unsigned index) {
        //     return _operationList[index].totalPrice;
        // }; 
        double operator[] (unsigned index){
            cout << "index:" << index << endl;
            if (index >= sizeofList) 
                throw std::out_of_range("CDayBalance out of range");
            return _operationList[index].totalPrice;
        };
        // const size_t size() {
        //     return sizeofList;
        // };
};

template <class A> 
double standardDeviation(A arr) {
    double sum = 0;
    double dev = 0;
    double tmp;

// double standardDeviation(CDayBalance& arr) {
//     double sum = 0;
//     double dev = 0;
//     double tmp;
    unsigned i, sizeofA; 
    //sizeofA = arr.size();

    try {
        for (i = 0; i <= INT_MAX; i++) {
                sum += arr[i];
        }
    }
    catch (std::out_of_range& e) {
        //cout << e.what() << endl;
    }
    sizeofA = i;
    sum /= sizeofA;
    for (i = 0; i < sizeofA; i++) {
        tmp = sum-arr[i];
        dev += tmp*tmp;
    }
    return sqrt(dev/sizeofA);
} 

int main(int argc, char const *argv[])
{
    CDayBalance balance;
    double res = standardDeviation(balance);
    cout << res << endl;    
    return 0;
}
