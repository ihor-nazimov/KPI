#include <limits>
#include <limits.h>
#include <math.h>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <typeinfo>

using namespace std;

template <class C> class Container: public C {
    public:
        static ostream& _writechars(ostream& of, const char* s, size_t sz=0){
            const char* CHARS_SIGN = "chars";
            if (!sz) sz = strlen(s);
            of.write( CHARS_SIGN, strlen(CHARS_SIGN)+1 ); //save char* signature
            of.write(reinterpret_cast<char*>(&sz), sizeof(size_t)); //save size of char* array 
            of.write(s, sz+1); //save content of char* array
        }
        static istream& _readchars(istream& ifile, char* s){
            const char* CHARS_SIGN = "chars";
            char* signature = new char[strlen( CHARS_SIGN )+1];
            streampos lastpos = ifile.tellg();
            ifile.read(signature, strlen(signature)+1);
            if ( !strncmp(signature, CHARS_SIGN, sizeof( CHARS_SIGN )) ) {
                size_t sz = 0;
                ifile.read(reinterpret_cast<char*>(&sz), sizeof(size_t));
                if (! ifile.good()) throw;
                if (! s) delete s;
                s = new char[sz+1];
                ifile.read(s, sz+1); 
                s[sz]='\0';
                if (! ifile.good()) throw;
            } else {
                ifile.seekg(lastpos);
            }
            delete signature;
            return ifile;
        }
        Container() {}
        Container(C& content) {
            if ( &content ) {
                C& tmp = dynamic_cast<C&> (*this);
                tmp = content;
            }
        }
        ostream& save(std::ostream& of) {
            of.write(typeid(C).name(), sizeof(typeid(C).name())); //save class signature
            C::_write(of);
            return of; 
        }
        istream& load(std::istream& ifile, C* copy = 0) {
            char* signature = new char[sizeof(typeid(C).name())];
            streampos lastpos = ifile.tellg();
            ifile.read(signature, sizeof(signature));
            if ( !strncmp(signature, typeid(C).name(), sizeof(signature)) ) {
                C::_read(ifile);
                if (copy) {
                    C* thisC = dynamic_cast<C*>(this);
                    *copy = *thisC;
                }

            } 
            else {
                ifile.seekg(lastpos);
            }
            delete signature;
            return ifile;
        }
};    


//base classes
class CDate {
    protected:
        unsigned _year = 1900;
        unsigned _month = 0;
        unsigned _day = 0;
        ostream& _write(std::ostream& of) {
            of.write(reinterpret_cast<char*>(&_year), sizeof(_year));
            of.write(reinterpret_cast<char*>(&_month), sizeof(_month));
            of.write(reinterpret_cast<char*>(&_day), sizeof(_day));
        }
        istream& _read(std::istream& ifile) {
            ifile.read(reinterpret_cast<char*>(&_year), sizeof(_year));
            ifile.read(reinterpret_cast<char*>(&_month), sizeof(_month));
            ifile.read(reinterpret_cast<char*>(&_day), sizeof(_day));
        }
    public:
        CDate() {};
        CDate(unsigned yy, unsigned mm, unsigned dd): _year(yy), _month(mm), _day(dd) {};
        CDate(CDate& src): _year(src.year()), _month(src.month()), _day(src.day()) {};
        friend std::ostream& operator<< (std::ostream &out, const CDate &dt) {
            out.width(4); out.precision(4); out.fixed;
            out << dt._day << '.' << dt._month << '.';
            out << fixed << right << setw(4) << setprecision(4) << dt._year;
            return out;
        };
        unsigned year(unsigned yr = 0) {
            if (yr >= 1900 && yr < 3000) _year = yr;
            return _year;
        }
        unsigned month(unsigned mm = 0) {
            if (mm > 0 && mm <= 13) _month = mm;
            return _month;
        }
        unsigned day(unsigned dd = 0) {
            if (dd > 0 && dd <= 31) _day = dd;
            return _day;
        }
};

class CFuel {
    public:
        enum FuelType {UNKNOWN, AI92, AI98, DIESEL};
    private:
        double _price = 0; //ціна за літр
        FuelType _ftype = UNKNOWN;
    protected:
        ostream& _write(std::ostream& of) {
            of.write(reinterpret_cast<char*>(&_price), sizeof(_price));
            of.write(reinterpret_cast<char*>(&_ftype), sizeof(_ftype));
        }
        istream& _read(std::istream& ifile) {
            ifile.read(reinterpret_cast<char*>(&_price), sizeof(_price));
            ifile.read(reinterpret_cast<char*>(&_ftype), sizeof(_ftype));
        }
    public:
        CFuel() {};  
        CFuel(FuelType ftype, double pr): _price(pr), _ftype(ftype) {};
        friend std::ostream& operator<< (std::ostream &out, const CFuel &df) {
            out << fixed << setw(8) << setprecision(2) << df._price << '|';
            out << fixed << setprecision(10) << left;
            switch (df._ftype) {
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
        double price(double pr = 0) {
            if (pr > 0) _price = pr;
            return _price;
        }
};

class COperator {
        char* _name = 0;
        char* _surname = 0;
        CDate birthday;
    protected:
        ostream& _write(std::ostream& of) {
            Container<COperator>::_writechars(of, _name, strlen(_name));
            Container<COperator>::_writechars(of, _surname, strlen(_surname));
            (new Container<CDate>(birthday))->save(of);
        }
        istream& _read(std::istream& ifile) {
            Container<COperator>::_readchars(ifile, _name);
            Container<COperator>::_readchars(ifile, _surname);
            (new Container<CDate>())->load(ifile, &birthday);
        }
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
            if (_name) delete[] _name;
            if (_surname) delete[] _surname;
        }
        friend std::ostream& operator<< (std::ostream &out, const COperator &op) {
            out << fixed << left << setw(12) << setprecision(12) << const_cast<COperator&>(op).surname() << '|' <<
            fixed << left << setw(8) << setprecision(8) << const_cast<COperator&>(op).name() << '|' <<
            fixed << op.birthday;
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

//inherited classes
class COperation: public CDate, public COperator, public CFuel {
    protected:
        ostream& _write(std::ostream& of) {
            of.write(reinterpret_cast<char*>(&quantity), sizeof(quantity));
            of.write(reinterpret_cast<char*>(&totalPrice), sizeof(totalPrice));
            (new Container<CDate>(* dynamic_cast<CDate*>(this) )) ->save(of);
            (new Container<COperator>(* dynamic_cast<COperator*>(this) )) ->save(of);
            (new Container<CFuel>(* dynamic_cast<CFuel*>(this) )) ->save(of);
        }
        istream& _read(std::istream& ifile) {
            ifile.read(reinterpret_cast<char*>(&quantity), sizeof(quantity));
            ifile.read(reinterpret_cast<char*>(&totalPrice), sizeof(totalPrice));
            (new Container<CDate>())->load(ifile, dynamic_cast<CDate*>(this) );
            (new Container<COperator>())->load(ifile, dynamic_cast<COperator*>(this) );
            (new Container<CFuel>())->load(ifile, dynamic_cast<CFuel*>(this) );
        }
    public:
        double quantity = 0;
        double totalPrice = 0;
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
        operator double() { return totalPrice; }
        
        friend std::ostream& operator<< (std::ostream &out, const COperation &opn) {
            COperation& opn_tmp = const_cast<COperation&>(opn);
            out << dynamic_cast<CDate&>(opn_tmp) << '|' << dynamic_cast<COperator&>(opn_tmp) << '|' << 
                dynamic_cast<CFuel&>(opn_tmp) << '|' <<
            fixed << left << setw(10) << setprecision(3) << opn.quantity << '|' <<
            fixed << left << setw(10) << setprecision(2) << opn.totalPrice;
            return out;
        };
};

//1. динамический массив?
//2. добавление єлементов
class CDayBalance: public CDate { //баланс
        unsigned maxsizeofList = 0;
        unsigned sizeofList = 0;
        COperation** _operationList; //list of operation. Must be placed at and of class!
    protected:
        ostream& _write(std::ostream& of) {
            of.write(reinterpret_cast<char*>(&maxsizeofList), sizeof(maxsizeofList));
            of.write(reinterpret_cast<char*>(&sizeofList), sizeof(sizeofList));
            for (unsigned i = 0; i < sizeofList; i++)
                (new Container<COperation>( *_operationList[i] )) ->save(of);
        }
        istream& _read(std::istream& ifile) {
            ifile.read(reinterpret_cast<char*>(&maxsizeofList), sizeof(maxsizeofList));
            ifile.read(reinterpret_cast<char*>(&sizeofList), sizeof(sizeofList));
            _operationList = new COperation* [maxsizeofList];
            for (unsigned i=0; i < maxsizeofList; i++) _operationList[i] = 0;
            Container<COperation>* cont = new Container<COperation>();
            for (unsigned i = 0; i < sizeofList; i++) 
                cont->load(ifile, _operationList[i] = new COperation());
        }
    public:
        CDayBalance() {} 
        CDayBalance(unsigned yy, unsigned mm, unsigned dd, unsigned maxsize) {
            _year = yy;
            _month = mm;
            _day = dd;
            maxsizeofList = maxsize;
            _operationList = new COperation* [maxsizeofList];
            for (unsigned i=0; i < maxsizeofList; i++) _operationList[i] = 0;
        } 
        // const double total(unsigned index) {
        //     return _operationList[index].totalPrice;
        // }; 
        ~CDayBalance() {
            for (unsigned i=0; i < maxsizeofList; i++) 
                if ( ! _operationList[i] ) delete[] _operationList[i];
        }
        unsigned push_back(COperation& value) {
            if (sizeofList < maxsizeofList) 
                _operationList[sizeofList++] = &value;
            return sizeofList;
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
        friend std::ostream& operator<< (std::ostream &out, CDayBalance &opn) {
            out << "Day balance " << dynamic_cast<CDate&>(opn) << endl;
            for (unsigned i=0; i < opn.size(); i++) 
                out << opn[i] << endl;
            return out;
        };
};

template <class A> 
double standardDeviation(A arr) {
//double standardDeviation(COperation* arr[], unsigned size) {
    double sum = 0;
    double dev = 0;
    double tmp;
    unsigned i, sizeofA; 
    //sizeofA = arr.size();
    sizeofA = arr.size();
    for (i = 0; i < sizeofA; i++) {
            sum += *arr[i];
    }
    sum /= sizeofA;
    for (i = 0; i < sizeofA; i++) {
        tmp = sum-*arr[i];
        dev += tmp*tmp;
    }
    return sqrt(dev/sizeofA);
}

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
    std::cout << *sales29 << endl << endl;

    ofstream ofile; 
    ofile.open("lab4.dat", ios::binary | ios::out);
    (new Container<CDayBalance>(*sales29))->save(ofile);
    ofile.close();
    delete sales29;
    std::cout << "Was saved and deleted" << endl;
    getchar();

    Container<CDayBalance>* dest = new Container<CDayBalance>();
    ifstream ifile; 
    ifile.open("lab4.dat", ios::binary | ios::in);
    dest->load(ifile);
    ifile.close();
    std::cout << "Was restored" << endl;
    std::cout << *dynamic_cast<CDayBalance*>(dest);
    getchar();

    delete dest;
    //CDayBalance balance;
    //double res = standardDeviation(balance);
    //cout << res << endl;    
    return 0;
}
