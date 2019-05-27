#include <limits>
#include <limits.h>
#include <math.h>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <string.h>

using namespace std;

//base classes
class CDate {
    protected:
        unsigned _year = 1900;
        unsigned _month = 0;
        unsigned _day = 0;
//        void _copy(const CDate src) {}
    public:
        CDate() {};
        CDate(unsigned yy, unsigned mm, unsigned dd): _year(yy), _month(mm), _day(dd) {};
        // const CDate& set(unsigned yy, unsigned mm, unsigned dd) {
        //     _year = yy;
        //     _month = mm;
        //     _day = dd;
        // }
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

//inherited classes
class COperator {
        char* _name = 0;
        char* _surname = 0;
        CDate birthday;
    // protected:
    //     const COperator& _copy(const COperator& src) {
    //         *this = src;
    //         _name = 0;
    //         _surname = 0;
    //         surname(const_cast<COperator&>(src).surname());
    //         name(const_cast<COperator&>(src).name());
    //         //CDate::_copy((CDate)*this);
    //         return *this; 
    //     }
    public:
        COperator() {}
        COperator(const char* nm, const char* snm, unsigned yy, unsigned mm, unsigned dd) {
            birthday = CDate(yy, mm, dd);
            // _year = yy;
            // _month = mm;
            // _day = dd;
            name(nm);
            surname(snm);
        }
        COperator(const COperator& src) {
            _name = 0;
            _surname = 0;
            surname(const_cast<COperator&>(src).surname());
            name(const_cast<COperator&>(src).name());
        }
        
        ~COperator() {
            if (name) delete[] _name;
            if (surname) delete[] _surname;
        }
        friend std::ostream& operator<< (std::ostream &out, const COperator &op) {
            out << fixed << left << setw(12) << setprecision(12) << op.surname << '|' <<
            fixed << left << setw(8) << setprecision(8) << op.name << '|' <<
            fixed << (CData)op;
            return out;
        };
        const char* name(const char* nm=0) {
            if (nm == _name) return _name;
            if (nm) {
                if (!_name) delete[] _name;
                _name = new char[strlen(nm)+1];
                strcpy(_name, nm);
            }
            return _name;
        }
        const char* surname(const char* surnm=0) {
            if (surnm == _surname) return _surname;
            if (surnm) {
                if (!_surname) delete[] _surname;
                _surname = new char[strlen(surnm)+1];
                strcpy(_surname, surnm);
            }
            return _surname;
        }

};

class COperation: public CDate, public COperator, public CFuel {
        // const COperator* _operator = 0;
        // const CFuel* _fuel;
        double quantity = 0;
        double totalPrice = 0;
    public:
        COperation() {};
        COperation(const COperator& op, const CFuel& fuel, double qnt, double total, unsigned yy, unsigned mm, unsigned dd):
            quantity(qnt), totalPrice(total) {
            COperator* tmpop = (COperator*) this;
            *tmpop = op;
            CFuel* tmpfuel = (CFuel*) this;
            *tmpfuel = fuel;
            _year = yy;
            _month = mm;
            _day = dd;
        } 
        COperation(const COperation& op) {
            COperator* tmpop = (COperator*) this;
            *tmpop = (COperator) op;
            CFuel* tmpfuel = (CFuel*) this;
            *tmpfuel = (CFuel) op;
        } 
        friend std::ostream& operator<< (std::ostream &out, const COperation &opn) {
            out << CDate(opn) << '|' << (COperator) opn << '|' << (CFuel) opn << '|' <<
            fixed << left << setw(10) << setprecision(3) << opn.quantity << '|' <<
            fixed << left << setw(10) << setprecision(2) << opn.totalPrice;
            return out;
        };
};

//1. динамический массив?
//2. добавление єлементов
class CDayBalance: public CDate { //баланс
       unsigned sizeofList = 0;
        COperation* _operationList[]; //list of operation. Must be placed at and of class!
    public:
        CDayBalance() {} 
        CDayBalance(unsigned yy, unsigned mm, unsigned dd) {
            _year = yy;
            _month = mm;
            _day = dd;
        } 
        // const double total(unsigned index) {
        //     return _operationList[index].totalPrice;
        // }; 
        ~CDayBalance() {
            for (unsigned i=0; i < sizeofList; i++) delete[] _operationList[i];
        }
        unsigned push_back(COperation& value) {
                        
        }


        const COperation& operator[] (unsigned index){
            cout << "index:" << index << endl;
            if (index >= sizeofList) 
                throw std::out_of_range("CDayBalance out of range");
            return *_operationList[index];
        };
        const size_t size() {
            return sizeofList;
        };
};

/*template <class A> 
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
} */

int main(int argc, char const *argv[])
{
    CDayBalance balance;
    //double res = standardDeviation(balance);
    //cout << res << endl;    
    return 0;
}
