// #include <limits>
// #include <limits.h>
// #include <math.h>
// #include <exception>
// #include <stdexcept>
#include <iostream>
// #include <fstream>
#include <iomanip>
// #include <string.h>
// #include <typeinfo>

#include "lab4_classes.h"
#include "lab4_templates.h"

//using namespace std;

//CDate implementation
std::ostream& CDate::_write(std::ostream& of) {
    of.write(reinterpret_cast<char*>(&_year), sizeof(_year));
    of.write(reinterpret_cast<char*>(&_month), sizeof(_month));
    of.write(reinterpret_cast<char*>(&_day), sizeof(_day));
}

std::istream& CDate::_read(std::istream& ifile) {
    ifile.read(reinterpret_cast<char*>(&_year), sizeof(_year));
    ifile.read(reinterpret_cast<char*>(&_month), sizeof(_month));
    ifile.read(reinterpret_cast<char*>(&_day), sizeof(_day));
}

std::ostream& operator<< (std::ostream &out, const CDate &dt) {
    out.width(4); out.precision(4); out.fixed;
    out << dt._day << '.' << dt._month << '.';
    out << fixed << right << setw(4) << setprecision(4) << dt._year;
    return out;
};

unsigned CDate::year(unsigned yr = 0) {
    if (yr >= 1900 && yr < 3000) _year = yr;
    return _year;
}

unsigned CDate::month(unsigned mm = 0) {
    if (mm > 0 && mm <= 13) _month = mm;
    return _month;
}
unsigned CDate::day(unsigned dd = 0) {
    if (dd > 0 && dd <= 31) _day = dd;
    return _day;
}


//CFuel implementation
std::ostream& CFuel::_write(std::ostream& of) {
    of.write(reinterpret_cast<char*>(&_price), sizeof(_price));
    of.write(reinterpret_cast<char*>(&_ftype), sizeof(_ftype));
}

std::istream& CFuel::_read(std::istream& ifile) {
    ifile.read(reinterpret_cast<char*>(&_price), sizeof(_price));
    ifile.read(reinterpret_cast<char*>(&_ftype), sizeof(_ftype));
}

std::ostream& operator<< (std::ostream &out, const CFuel &df) {
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

double CFuel::price(double pr = 0) {
    if (pr > 0) _price = pr;
    return _price;
}


//COperator implementation
std::ostream& COperator::_write(std::ostream& of) {
    Container<COperator>::_writechars(of, _name, strlen(_name));
    Container<COperator>::_writechars(of, _surname, strlen(_surname));
    (new Container<CDate>(birthday))->save(of);
}

std::istream& COperator::_read(std::istream& ifile) {
    Container<COperator>::_readchars(ifile, _name);
    Container<COperator>::_readchars(ifile, _surname);
    (new Container<CDate>())->load(ifile, &birthday);
}

COperator::COperator(const char* nm, const char* snm, unsigned yy, unsigned mm, unsigned dd) {
    birthday = CDate(yy, mm, dd);
    name(nm);
    surname(snm);
}

COperator::COperator(const COperator& src) {
    _name = 0;
    _surname = 0;
    surname(const_cast<COperator&>(src).surname());
    name(const_cast<COperator&>(src).name());
}

COperator::~COperator() {
    if (_name) delete[] _name;
    if (_surname) delete[] _surname;
}

std::ostream& operator<< (std::ostream &out, const COperator &op) {
    out << fixed << left << setw(12) << setprecision(12) << const_cast<COperator&>(op).surname() << '|' <<
    fixed << left << setw(8) << setprecision(8) << const_cast<COperator&>(op).name() << '|' <<
    fixed << op.birthday;
    return out;
};

const char* COperator::name(const char* nm=0) {
    if (nm == _name) return _name;
    if (nm) {
        if (!_name) delete[] _name;
        _name = new char[strlen(nm)+1];
        strcpy(_name, nm);
    }
    return _name;
}

const char* COperator::surname(const char* surnm=0) {
    if (surnm == _surname) return _surname;
    if (surnm) {
        if (!_surname) delete[] _surname;
        _surname = new char[strlen(surnm)+1];
        strcpy(_surname, surnm);
    }
    return _surname;
}


//COperation implementation
std::ostream& COperation::_write(std::ostream& of) {
    of.write(reinterpret_cast<char*>(&quantity), sizeof(quantity));
    of.write(reinterpret_cast<char*>(&totalPrice), sizeof(totalPrice));
    (new Container<CDate>(* dynamic_cast<CDate*>(this) )) ->save(of);
    (new Container<COperator>(* dynamic_cast<COperator*>(this) )) ->save(of);
    (new Container<CFuel>(* dynamic_cast<CFuel*>(this) )) ->save(of);
}

std::istream& COperation::_read(std::istream& ifile) {
    ifile.read(reinterpret_cast<char*>(&quantity), sizeof(quantity));
    ifile.read(reinterpret_cast<char*>(&totalPrice), sizeof(totalPrice));
    (new Container<CDate>())->load(ifile, dynamic_cast<CDate*>(this) );
    (new Container<COperator>())->load(ifile, dynamic_cast<COperator*>(this) );
    (new Container<CFuel>())->load(ifile, dynamic_cast<CFuel*>(this) );

}

COperation::COperation(const COperator& op, const CFuel& fuel, double qnt, double total, unsigned yy, unsigned mm, unsigned dd):
    quantity(qnt), totalPrice(total) {
    COperator* tmpop = (COperator*) this;
    *tmpop = op;
    CFuel* tmpfuel = (CFuel*) this;
    *tmpfuel = fuel;
    _year = yy;
    _month = mm;
    _day = dd;
} 

COperation::COperation(const COperation& op) {
    COperator* tmpop = (COperator*) this;
    *tmpop = (COperator) op;
    CFuel* tmpfuel = (CFuel*) this;
    *tmpfuel = (CFuel) op;
} 

COperation::operator double() { return totalPrice; }

std::ostream& operator<< (std::ostream &out, const COperation &opn) {
    COperation& opn_tmp = const_cast<COperation&>(opn);
    out << dynamic_cast<CDate&>(opn_tmp) << '|' << dynamic_cast<COperator&>(opn_tmp) << '|' << 
        dynamic_cast<CFuel&>(opn_tmp) << '|' <<
    fixed << left << setw(10) << setprecision(3) << opn.quantity << '|' <<
    fixed << left << setw(10) << setprecision(2) << opn.totalPrice;
    return out;
};


//CDayBalance implementation
//1. динамический массив?
//2. добавление єлементов
ostream& CDayBalance::_write(std::ostream& of) {
    of.write(reinterpret_cast<char*>(&maxsizeofList), sizeof(maxsizeofList));
    of.write(reinterpret_cast<char*>(&sizeofList), sizeof(sizeofList));
    for (unsigned i = 0; i < sizeofList; i++)
        (new Container<COperation>( *_operationList[i] )) ->save(of);
}

istream& CDayBalance::_read(std::istream& ifile) {
    ifile.read(reinterpret_cast<char*>(&maxsizeofList), sizeof(maxsizeofList));
    ifile.read(reinterpret_cast<char*>(&sizeofList), sizeof(sizeofList));
    _operationList = new COperation* [maxsizeofList];
    for (unsigned i=0; i < maxsizeofList; i++) _operationList[i] = 0;
    Container<COperation>* cont = new Container<COperation>();
    for (unsigned i = 0; i < sizeofList; i++) 
        cont->load(ifile, _operationList[i] = new COperation());
}

CDayBalance::CDayBalance(unsigned yy, unsigned mm, unsigned dd, unsigned maxsize) {
    _year = yy;
    _month = mm;
    _day = dd;
    maxsizeofList = maxsize;
    _operationList = new COperation* [maxsizeofList];
    for (unsigned i=0; i < maxsizeofList; i++) _operationList[i] = 0;
} 

CDayBalance::~CDayBalance() {
    for (unsigned i=0; i < maxsizeofList; i++) 
        if ( ! _operationList[i] ) delete[] _operationList[i];
}

unsigned CDayBalance::push_back(COperation& value) {
    if (sizeofList < maxsizeofList) 
        _operationList[sizeofList++] = &value;
    return sizeofList;
}

const COperation& CDayBalance::operator[] (unsigned index){
    cout << "index:" << index << endl;
    if (index >= sizeofList) 
        throw std::out_of_range("CDayBalance out of range");
    return *_operationList[index];
};

const size_t CDayBalance::size() {
    return sizeofList;
};

std::ostream& operator<< (std::ostream &out, CDayBalance &opn) {
    out << "Day balance " << dynamic_cast<CDate&>(opn) << endl;
    for (unsigned i=0; i < opn.size(); i++) 
        out << opn[i] << endl;
    return out;
};

