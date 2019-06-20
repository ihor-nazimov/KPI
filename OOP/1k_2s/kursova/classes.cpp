/*#define NDEBUG */
 
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <typeinfo>
#include <cstring>
//#include <climits> //ULONG_MAX
//#include <limits>  //double
//#include <cstdio>

#include "classes.h"

using namespace std;

std::ostream& writeChars(std::ostream& out, const char* s) {
    size_t sz = 0;
    if (s != 0) {
        sz = strlen(s)+1;
        out.write(reinterpret_cast<char*>(&sz), sizeof(size_t));
        out.write(s, sz);
    } else
        out.write(reinterpret_cast<char*>(&sz), sizeof(size_t));
    return out;
}

//load char* from binary stream
char* readChars(std::istream& in) {
    size_t sz = 0;
    in.read(reinterpret_cast<char*>(&sz), sizeof(size_t));
    char* s = new char[sz];
    in.read(s, sz);
    return s;
}

char* setchars(const char* src, char* dest) {
    if (src == dest) return dest;
    if (src) {
        if (!dest) delete[] dest;
        dest = new char[strlen(src)+1];
        strcpy(dest, src);
    }
    return dest;
}

char* inputChars(istream& in, ostream& out, const char* prompt, size_t n) {
    char* buf = new char[n+1];
    do {
        if (!in) {in.clear(); in.ignore(MAX_CHARS, '\n'); } 
        out << prompt;
        in.getline(buf, n, '\n');
        if ( in ) break;
        out << "Incorrect input\n";
    } while (true); 
    return buf;
}

double inputDouble(istream& in, ostream& out, const char* prompt) {
    double tmp=0;
    do {
        if (!in) {in.clear(); in.ignore(MAX_CHARS, '\n'); } 
        out << prompt;
        in >> tmp;
        if ( in ) break;
        out << "Incorrect input\n";
    } while (true); 
    in.clear(); in.ignore(MAX_CHARS, '\n');  
    return tmp;
}

char* getFilename(char* dest, const char* def_filename, size_t n) {
    if (!cin) {cin.clear(); cin.ignore(MAX_CHARS, '\n'); } 
    std::cout << "Enter file name (" << def_filename << " by default): ";
    std::cin.getline(dest, n);
    if (strlen(dest) == 0) strncpy(dest, def_filename, n);
    return dest;
}

int getChoice(const char* prompt) {
    int choice=-1;
    do {
        if (!cin) { std::cin.clear(); std::cin.ignore(MAX_CHARS, '\n'); }
        std::cout << prompt;
        std::cin >> choice;
        if ( ! std::cin ) {
            std::cout << "Incorrect input\n>" << endl;
            choice = -1;
        }
        std::cin.clear(); std::cin.ignore(MAX_CHARS, '\n'); 
    }
    while ( choice == -1 );
    return choice;
}

//CDate
CDate& CDate::input(istream& in, ostream& out, const char* prompt) {
    unsigned yy, mm, dd;
    do {
        if (!in) {in.clear(); in.ignore(MAX_CHARS, '\n'); } 
        out << prompt;
        in >> dd; 
        in.ignore(MAX_CHARS, '.'); 
        in >> mm; 
        in.ignore(MAX_CHARS, '.'); 
        in >> yy; 
        in.ignore(MAX_CHARS, '\n'); 
        if (in) break;
        out << "\nDate is incorrect" << endl;
    } while (true);
    if (!in) {in.clear(); in.ignore(MAX_CHARS, '\n'); } 
    year(yy); month(mm); day(dd);
    return *this;
}

void CDate::save(std::ofstream& out) {
    writeTypeSign<CDate> (out);
    out.write(reinterpret_cast<char*>(&_year), sizeof(_year));
    out.write(reinterpret_cast<char*>(&_month), sizeof(_month));
    out.write(reinterpret_cast<char*>(&_day), sizeof(_day));
}

CDate& CDate::load(std::ifstream& in) {
    if ( readTypeSign<CDate> (in) ) {
        in.read(reinterpret_cast<char*>(&_year), sizeof(_year));
        in.read(reinterpret_cast<char*>(&_month), sizeof(_month));
        in.read(reinterpret_cast<char*>(&_day), sizeof(_day));
    } else 
        throw;
    return *this;
}

std::ostream& CDate::header (std::ostream &out) {
    out << "   Date   ";
    return out;
};

std::ostream& operator<< (std::ostream &out, CDate &dt) {
    out << fixed << right << setw(2) << setprecision(2) << dt._day << '.' 
    << fixed << right << setw(2) << setprecision(2) << dt._month << '.'
    << fixed << right << setw(4) << setprecision(4) << dt._year;
    return out;
};

CDate::CDate(unsigned yy, unsigned mm, unsigned dd) {
    year(yy);
    month(mm); 
    day(dd);
}
CDate::CDate(CDate& src) {
    year(src._year);
    month(src._month);
    day(src._day);
}

unsigned CDate::year(unsigned yr) {
    if ((yr >= 1500) & (yr < 3000)) _year = yr;
        else std::cerr << "Incorrect year" << endl;
    return _year;
}
unsigned CDate::month(unsigned mm) {
    if ((mm > 0) & (mm <= 13)) _month = mm;
        else std::cerr << "Incorrect month" << endl;
    return _month;
}
unsigned CDate::day(unsigned dd) {
    if ((dd > 0) & (dd <= 31)) _day = dd;
        else std::cerr << "Incorrect day" << endl;
    return _day;
}

//CPerson
CPerson::CPerson(const char* nm, const char* surnm, CDate& bd): CPerson() {
    birthday(bd);
    name(nm);
    surname(surnm);
}

CPerson::CPerson(CPerson& src): CPerson() {
    surname(const_cast<CPerson&>(src).surname());
    name(const_cast<CPerson&>(src).name());
    birthday(const_cast<CPerson&>(src).birthday());
}

CPerson::~CPerson() {
    if (_name) delete[] _name;
    if (_surname) delete[] _surname;
}

CPerson& CPerson::input(istream& in, ostream& out, const char* prompt) {
    out << prompt;
    if (_name) delete[] _name;
    if (_surname) delete[] _surname;
    name(inputChars(in, out, "Enter name: "));
    surname(inputChars(in, out, "Enter surname: "));
    _birthday = ( (new CDate())->input(in, out, "Enter birthday: "));
    return *this;
}

void CPerson::save(std::ofstream& out) {
    writeTypeSign<CPerson> (out);
    assert(_name && _surname);
    writeChars(out, _name); 
    writeChars(out, _surname);
    _birthday.save(out);
}

CPerson& CPerson::load(std::ifstream& in) {
    if ( readTypeSign<CPerson> (in) ) {
        if (_name) delete[] _name;                    
        if (_surname) delete[] _surname;
        //if (pers._birthday) delete pers._birthday;
        _name = readChars(in);
        _surname = readChars(in);
        //pers._birthday = new CDate();
        _birthday.load(in);
    } else 
        throw;
    return *this;
}

std::ostream& CPerson::header(std::ostream &out) {
    out << "   Name   |  Surname | Birthday";
    return out;
};

std::ostream& operator<< (std::ostream &out, CPerson& pers) {
    out << fixed << setw(10) << setprecision(10) << left << pers._name << '|' 
    << fixed << setw(10) << setprecision(10) << left << pers._surname << '|' 
    << pers._birthday;
    return out;
};


//CFile
CFile::CFile(const char* fnm, const char* ext, CDate& crtd, unsigned long sz) {
    filename(fnm);
    extention(ext);
    date(crtd);
    size(sz);
}

CFile::CFile(CFile& src) {
    filename(const_cast<CFile&>(src).filename());
    extention(const_cast<CFile&>(src).extention());
    date(const_cast<CFile&>(src).date());
    size(const_cast<CFile&>(src).size());
}

CFile::~CFile() {
    if (_filename) delete[] _filename;
    if (_extention) delete[] _extention;
}

CFile& CFile::input(istream& in, ostream& out, const char* prompt) {
    out << prompt;
    if (_filename) delete[] _filename;
    if (_extention) delete[] _extention;
    filename(inputChars(in, out, "Enter file name: "));
    extention(inputChars(in, out, "Enter file extention: "));
    extention(inputChars(in, out, "Enter file size: "));
    date((new CDate)->input(in, out, "Enter date of file creation: "));
    return *this;
}

void CFile::save(std::ofstream& out) {
    writeTypeSign<CFile> (out);
    writeChars(out, _filename); 
    writeChars(out, _extention); 
    _created.save(out);
    out.write(reinterpret_cast<char*>(&_size), sizeof(_size));
}

CFile& CFile::load(std::ifstream& in) {
    if ( readTypeSign<CFile> (in) ) {
        if (_filename) delete[] _filename;                    
        if (_extention) delete[] _extention;
        // if (_created) delete _created;                    
        _filename = readChars(in);
        _extention = readChars(in);
        _created.load(in);
        in.read(reinterpret_cast<char*>(&_size), sizeof(_size));
    } else 
        throw;
    return *this;
}

std::ostream& CFile::header(std::ostream &out) {
    out << "       Size      | Created  |             Filename";
    return out;
}

std::ostream& operator<< (std::ostream &out, CFile& file) {
    out << left << fixed << setw(17) << setprecision(17) << file._size << '|' 
    << file._created << '|' 
    << left << setw(0) << setprecision(0) << file._filename << "." << file._extention;
    return out;
};

//CTextFile
CTextFile::CTextFile(CFile& file, const char* pth, CPerson& auth, const char* keywds): 
    CFile(file) {
    path(pth);
    author(auth);
    keywords(keywds);
}

CTextFile::CTextFile(CTextFile& src): CFile(src) {
    CTextFile();
    path(const_cast<CTextFile&>(src).path());
    author(const_cast<CTextFile&>(src).author());
    keywords(const_cast<CTextFile&>(src).keywords());
} 

CTextFile::~CTextFile() {
    if (_path) delete[] _path;
    if (_keywords) delete[] _keywords;
    //if (_author) delete _author;
}

CTextFile& CTextFile::input(istream& in, ostream& out, const char* prompt) {
    out << prompt;
    if (_path) delete[] _path;
    if (_keywords) delete[] _keywords;
    path(inputChars(in, out, "Enter file path: "));
    dynamic_cast<CFile*>(this) -> input(in, out, "");
    keywords(inputChars(in, out, "Enter keywords: "));
    author( (new CPerson)->input(in, out, "Enter author: \n"));
    return *this;
}

void CTextFile::save(std::ofstream& out) {
    writeTypeSign<CTextFile> (out);
    writeChars(out, _path); 
    _author->save(out);
    writeChars(out, _keywords); 
    CFile::save(out);
}

CTextFile& CTextFile::load(std::ifstream& in) {
    if ( readTypeSign<CTextFile> (in) ) {
        if ( _path) delete[]  _path;                    
        if ( _keywords) delete[]  _keywords;
        _path =  readChars(in);
        CPerson* tmp_author = new CPerson();
        tmp_author->load(in);                    
        _author = tmp_author;
        _keywords =  readChars(in);
        CFile::load(in);
    } else 
        throw;
    return *this;
}

std::ostream& CTextFile::header(std::ostream &out) {
    out << "       Size      |" << CDate::header << "|"
    << "                     Full filename                                ";
    return out;
}

std::ostream& operator<< (std::ostream &out, CTextFile& txtfile) {
    out << left << fixed << setw(17) << setprecision(17) << txtfile._size << '|' 
    << txtfile._created << '|' 
    << txtfile._path << "/" << txtfile._filename << "." << txtfile._extention;
    return out;
}


//CArchiveEntry
CArchiveEntry::CArchiveEntry() {
    _listmaxsize = 100;
    _listsize = 0;
    _list = new CTextFile* [_listmaxsize];
}

CArchiveEntry::CArchiveEntry(CDate& crtd, size_t maxsize) {
    created(crtd);
    _listmaxsize = maxsize;
    _listsize = 0;
    _list = new CTextFile* [maxsize];
}

CArchiveEntry::CArchiveEntry(CArchiveEntry& src): _listmaxsize(src._listmaxsize), _listsize(src._listsize) {
    created(src._created);
    compress(src._compress);
    _list = new CTextFile* [_listmaxsize];
    for (size_t i=0; i < _listmaxsize; i++)
        if (i < _listsize) _list[i] = new CTextFile(* (src._list[i]) );
            else _list[i] = 0;
}

CArchiveEntry::~CArchiveEntry() {
    for (size_t i=0; i < _listsize; i++)
        if (i < _listsize) delete _list[i];
    delete[] _list;
}

void CArchiveEntry::push_back(CTextFile& text) {
    if (_listsize < _listmaxsize) 
        _list[_listsize++] = &text;
}

CArchiveEntry& CArchiveEntry::input(istream& in, ostream& out, const char* prompt) {
    out << prompt;
    for (size_t i=0; i < _listsize; i++)
        if (i < _listsize) delete _list[i];
    delete[] _list;
    created((new CDate)->input(in, out, "Enter date of archive creation (dd.mm.yyyy): "));
    compress(inputDouble(in, out, "Enter compress level: "));
    out << "Add text file:\n";
    do {
        push_back((new CTextFile())->input(in, out, ""));
    } while (getChoice("1. Add next text file\n0. Complete file list\n") != 0);
    out << "Archive entry was added\n";
    return *this;
}

void CArchiveEntry::save(std::ofstream& out) {
        writeTypeSign<CArchiveEntry> (out);
        _created.save(out);
        out.write(reinterpret_cast<char*>(& _compress), sizeof( _compress));
        out.write(reinterpret_cast<char*>(& _listmaxsize), sizeof( _listmaxsize));
        out.write(reinterpret_cast<char*>(& _listsize), sizeof( _listsize));
        for (size_t i = 0; i <  _listsize; i++) 
            _list[i]->save(out);
}

CArchiveEntry& CArchiveEntry::load(std::ifstream& in) {
    if ( readTypeSign<CArchiveEntry> (in) ) {
        for (size_t i=0; i < _listsize; i++)
            if (_list[i]) delete _list[i];
        delete[] _list;                        
        _created.load(in);
        in.read(reinterpret_cast<char*>(&_compress), sizeof(_compress));
        in.read(reinterpret_cast<char*>(&_listmaxsize), sizeof(_listmaxsize));
        in.read(reinterpret_cast<char*>(&_listsize), sizeof(_listsize));
        _list = new CTextFile* [_listmaxsize];
        for (size_t i = 0; i < _listsize; i++) {
            _list[i] = new CTextFile();
            try{
                _list[i]->load(in);
            } 
            catch (int a) {
                cerr << "Error while loading. Nothing changed";
                delete _list[i];
            }
        }
    } else 
        throw;
    return *this;
}

std::ostream& CArchiveEntry::header (std::ostream &out) {
    out << " Created  |  Files Qty  |   Compress";
    return out;
}

std::ostream& operator<< (std::ostream &out, CArchiveEntry& aentry) {
    out << aentry._created << '|' 
    << fixed << left << setw(13) << setprecision(13) << aentry._listsize << '|'
    << fixed << left << setw(17) << setprecision(3) << aentry._compress;
    out << endl << endl << CTextFile::header << endl;
    for (size_t i = 0; i < aentry._listsize; i++) 
        out << aentry[i] << endl;
    return out;
}

CTextFile& CArchiveEntry::operator[] (size_t index) {
    if ( (index >= 0) & (index < _listsize) ) 
        return * (_list[index]);
    else throw;
}

//CArchive
CArchive::CArchive(size_t sz) {
    _listmaxsize = sz;
    _list = new CArchiveEntry* [_listmaxsize];
}  

void CArchive::push_back(CArchiveEntry& src) {
    if (_listsize < _listmaxsize) 
        _list[_listsize++] = &src;
}

CArchiveEntry& CArchive::operator[] (size_t index) {
    if ( (index >= 0) & (index < _listsize) ) 
        return * (_list[index]);
    else throw;
}

void CArchive::save(std::ofstream& out) {
    writeTypeSign<CArchive> (out);
    out.write(reinterpret_cast<char*>(&_listmaxsize), sizeof(_listmaxsize));
    out.write(reinterpret_cast<char*>(&_listsize), sizeof(_listsize));
    for (size_t i = 0; i < _listsize; i++) 
        _list[i]->save(out);
}

CArchive& CArchive::load(std::ifstream& in) {
    if ( readTypeSign<CArchive> (in) ) {
        for (size_t i=0; i < _listsize; i++)
            if (_list[i]) delete _list[i];
        delete[] _list;                        
        in.read(reinterpret_cast<char*>(&_listmaxsize), sizeof(_listmaxsize));
        in.read(reinterpret_cast<char*>(&_listsize), sizeof(_listsize));
        _list = new CArchiveEntry* [_listmaxsize];
        for (size_t i = 0; i < _listsize; i++) {
            _list[i] = new CArchiveEntry();
            _list[i]->load(in);
        }
    } else 
        throw;
    return *this;
}

std::ostream& CArchive::header (std::ostream &out) {
    out << "Current archive entries: ";
    return out;
}

std::ostream& operator<< (std::ostream &out, CArchive& aentry) {
    out << left << setw(3) << setprecision(3) << aentry._listsize;
    out << endl << "*** List of archive entry ***\n" << endl; 
    for (size_t i = 0; i < aentry._listsize; i++) {
        out << "Entry #" << left << setw(3) << i << endl << CArchiveEntry::header << endl
        << aentry[i] << endl;
    }
    //out << endl;
    return out;
}
