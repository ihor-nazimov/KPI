//закриті внутр. поля + методи доступу та контролю
//перевантажені опер-ри, індексування
//шаблон функції стат обробки
//массив останнього похідн.класу зберігати/читати в двійк.форматі
//виключні ситуації
//меню:
// - виведення
// - вивед.табл (заголовок/рядки необх.даних)
// - запис текст, запис двійк
// - зчит. + ім'я файла за замовч.
// - пошук за текст/числ полем
//Перевірка: 10 об'єктів, зберегти.
/*
Дата {Рік, Місяць, День};
Особа {Прізвище, Ім’я, Дата (народження)};
Файл {Назва, Розширення, Дата (створення), Розмір};
Текстовий Документ {Файл, Розміщення (диск, шлях), Особа (автор), Ключові слова};
Елемент архіву {Список Текстових документ архіву, Дата (створення), ступінь архівації елемента архіву};
Склад каталогу {Масив елементів архіву}.
Середнє значення кількості Текстових документів в елементах архіву та середнє значення ступеня архівації в елементах архіву.
*/
/*#define NDEBUG */
 
#include <iostream>
#include <cassert>
#include <typeinfo>
#include <cstring>
#include <climits>
#include <limits>

template <class C>
std::ostream& writeTypeSign(std::ostream& out) {
    if (out.flags && std::ios::binary) 
        //сігнатура на містить довжину строки. Завантаження у довільному порядку не передбачено
        out.write(typeid(C).name(), strlen(typeid(C).name()) + 1 ); //save class signature
    else 
        out << typeid(C).name();
    return out;
}

template <class C>
bool readTypeSign(std::istream& in) {
    bool fVerified = false;
    streampos lastpos = in.tellg();
    if (in.flags && std::ios::binary) {
        size_t sz = strlen(typeid(C).name()) + 1;
        char* signature = new char[sz];
        in.read(signature, sz);
        if ( !strncmp(signature, typeid(C).name(), sz) )
            fVerified = true;
        else 
            in.seekg(lastpos);
            assert"readTypeSign: Error while reading file - incorrect class signature");
        delete signature;
    }
    return fVerified;
}

template <class C>
std::ostream& writeArray(std::ostream& out, C c[], const size_t sz) {
    if (out.flags && std::ios::binary) {
        out.write(reinterpret_cast<char*>(&sz), sizeof(sz));
        for (size_t i = 0; i < sz; i++) {
            out << c[i];
        }
    } 
    return out;
}

template <class C>
size_t readArray(std::ostream& in, C* c[], size_t max_sz) {
    size_t readed = 0;
    streampos lastpos = in.tellg();
    C tmp = new C();
    if (in.flags && std::ios::binary) {
        in.read(&readed, sizeof(size_t));
        for (size_t i = 0; i < readed; i++) {
            in >> tmp;
            if (i < max_sz) {
                if (c[i]) delete c[i];
                c[i] = new C(tmp);
            }
        } 
    }
    return readed < max_sz ? readed : max_sz;
}

//save char* and it's length and into binary stream
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
std::istream& readChars(std::istream& in, char* s) {
    size_t sz = 0;
    in.read(reinterpret_cast<char*>(&sz), sizeof(size_t));
    s = new char[sz];
    in.read(s, sz);
    return in;
}

template <class C>
inline C* setlocal(const C* src, C* dest) {
    if (src == dest) return dest;
    if (src) {
        if (!dest) delete dest;
        dest = new C(src);
    }
    return dest;
}

inline char* setchars(const char* src, char* dest) {
    if (src == dest) return dest;
    if (src) {
        if (!dest) delete[] dest;
        dest = new char[strlen(src)+1];
        strcpy(dest, src);
    }
    return dest;
}
    
class CDate {
    protected:
        unsigned _year = 1900;
        unsigned _month = 0;
        unsigned _day = 0;
   public:
        CDate() {};
        CDate(unsigned yy, unsigned mm, unsigned dd);
        CDate(CDate& src);
        unsigned year(unsigned yr = 0);
        unsigned month(unsigned mm = 0);
        unsigned day(unsigned dd = 0);

        friend std::ostream& header (std::ostream &out) {
            out << "   Date   ";
            return out;
        };

        friend std::ostream& operator<< (std::ostream &out, CDate &dt) {
            if (out.flags && std::ios::binary) {
                writeTypeSign<CDate> (out);
                out.write(reinterpret_cast<char*>(&_year), sizeof(_year));
                out.write(reinterpret_cast<char*>(&_month), sizeof(_month));
                out.write(reinterpret_cast<char*>(&_day), sizeof(_day));
            } else {
                out.width(2); out.precision(2); out.fixed;
                out << dt._day << '.' << dt._month << '.';
                out << fixed << right << setw(4) << setprecision(4) << dt._year;
            }
            return out;
        };

        friend std::istream& operator>> (std::istream &in, CDate &dt) {
            if (in.flags && std::ios::binary) {
                if ( readTypeSign<CDate> (in) ) {
                    in.read(reinterpret_cast<char*>(&_year), sizeof(_year));
                    in.read(reinterpret_cast<char*>(&_month), sizeof(_month));
                    in.read(reinterpret_cast<char*>(&_day), sizeof(_day));
                } else 
                    throw;
            }
            return in;
        };
};

class CPerson {
        char* _name = 0;
        char* _surname = 0;
        CDate* _birthday = 0;
    public:
        CPerson() {}
        CPerson(const char* nm, const char* surnm, CDate* bd) {
            birthday(bd);
            name(nm);
            surname(surnm);
        }
        CPerson(const CPerson& src) {
            _name = 0;
            _surname = 0;
            surname(const_cast<CPerson&>(src).surname());
            name(const_cast<CPerson&>(src).name());
            birthday(const_cast<CPerson&>(src).birthday());
        }
        ~CPerson() {
            if (_name) delete[] _name;
            if (_surname) delete[] _surname;
            if (_birthday) delete[] _birthday;
        }
        const char* name(const char* nm=0) { return setchars(nm, _name); }
        const char* surname(const char* surnm=0) { return setchars(surnm, _surname); }
        const CDate* birthday(const CDate* dt=0) { return setlocal<CDate>(dt, _birthday); }

        friend std::ostream& header (std::ostream &out) {
            out << "   Name   |  Surname | Birthday";
            return out;
        };

        friend std::ostream& operator<< (std::ostream &out, CPerson& pers) {
            if (out.flags && std::ios::binary) {
                writeTypeSign<CPerson> (out);
                assert(_name == 0);
                assert(_surname == 0);
                assert(birthday == 0);

                writeChars(out, _name); 
                // size_t sz = strlen(_name)+1;
                // out.write(reinterpret_cast<char*>(&sz), sizeof(size_t));
                // out.write(_name), sz);
                
                writeChars(out, _surname);
                // size_t sz = strlen(_surname)+1;
                // out.write(reinterpret_cast<char*>(&sz), sizeof(size_t));
                // out.write(_surname, sz);

                out << *birthday;
            } else {
                out.width(10); out.precision(10); out.fixed;
                out << left << pers._name << '|' << pers._surname << '|';
                out << fixed << right << *birthday;
            }
            return out;
        };

        friend std::istream& operator>> (std::istream &in, CPerson& pers) {
            size_t sz = 0;
            if (in.flags && std::ios::binary) {
                if ( readTypeSign<CPerson> (in) ) {
                    if (_name) delete[] _name;                    
                    if (_surname) delete[] _surname;
                    if (birthday) delete birthday;                    
                    in >> readChars(_name) >> readChars(in, _surname);
                    birthday = new CDate();
                    in >> *birthday;
                } else 
                    throw;
            }
            return in;
        };
};

class CFile {
        char* _filename = 0;
        char* _extention = 0;
        CDate* _created = 0;
        unsigned long _size = 0;
    public:
        CFile() {};
        CFile(const char* fnm, const char* ext, const CDate* crtd, unsigned long sz) {
            filename(fnm);
            extention(ext);
            date(crtd);
            size(sz);
        };
        CFile(const CFile& src) {
            filename(const_cast<CFile&>(src).filename());
            extention(const_cast<CFile&>(src).extention());
            date(const_cast<CFile&>(src).date());
            size(const_cast<CFile&>(src).size());
        }
        ~CFile() {
            if (_filename) delete[] _filename;
            if (_extention) delete[] _extention;
            if (_created) delete _created;
        }
        const char* filename(const char* fnm=0) { return setchars(fnm, _filename); }
        const char* extention(const char* ext=0) { return setchars(ext, _extention); }
        unsigned long size(unsigned long sz=ULONG_MAX) { 
            if (sz != ULONG_MAX) _size = sz;
            return _size = sz; 
        }
        const CDate* date(const CDate* dt=0) { return setlocal<CDate>(dt, _created); }

        friend std::ostream& header (std::ostream &out) {
            out << "                Filename                        |       Size      |" << date().header;
            return out;
        };

        friend std::ostream& operator<< (std::ostream &out, CFile& file) {
            if (out.flags && std::ios::binary) {
                writeTypeSign<CFile> (out);
                out << writeChars(_filename) << writeChars(_extension) << *_created << _size;
            } else {
                out.width(48); out.precision(48); out.fixed;
                out << left << strcat(_filename, ".", _extension) << '|' << _size << '|' << *_created;
            }
            return out;
        };

        friend std::istream& operator>> (std::istream &in, CFile& file) {
            size_t sz = 0;
            if (in.flags && std::ios::binary) {
                if ( readTypeSign<CPerson> (in) ) {
                    if (_filename) delete[] _filename;                    
                    if (_extension) delete[] _extention;
                    if (_created) delete _created;                    
                    in >> readChars(_filename) >> readChars(_extension) >> *_created >> _size;
                } else 
                    throw;
            }
            return in;
        };
        
};

class CTextFile: public CFile {
        char* _path = 0;
        CPerson* _author = 0;
        char* _keywords = 0;
    public:
        CTextFile(){}
        CTextFile(const CFile& file, const char* pth, const CPerson& auth, const char* keywds) {
            CFile* tmpf = (CFile*) this;
            *tmpf = file; 
            path(pth);
            author(&auth);
            keywords(keywds);
        }
        CTextFile(const CTextFile& src) {
            CFile* tmpf = (CFile*) this;
            *tmpf = (CFile) src;
            path(const_cast<CTextFile&>(src).path());
            author(const_cast<CTextFile&>(src).author());
            keywords(const_cast<CTextFile&>(src).keywords());
        } 
        ~CTextFile() {
            if (_path) delete[] _path;
            if (_keywords) delete[] _keywords;
            if (_author) delete _author;
        }
        const char* path(const char* pth=0) { return setchars(pth, _path); }
        const char* keywords(const char* keywds=0) { return setchars(keywds, _keywords); }
        const CPerson* author(const CPerson* auth=0) { return setlocal<CPerson>(auth, _author); }

        friend std::ostream& header (std::ostream &out) {
            out << "                Full filename                                    |       Size      |" << date().header;
            return out;
        };

        friend std::ostream& operator<< (std::ostream &out, CTextFile& txtfile) {
            if (out.flags && std::ios::binary) {
                writeTypeSign<CFile> (out);
                out << writeChars(_path) << *_author << writeChars(_keywords) << *dynamic_cast<CFile*>(this);
            } else {
                out.width(65); out.precision(65); out.fixed;
                out << left << strcat(_path, "/", _filename, ".", _extension) << '|' << _size << '|' << *_created;
            }
            return out;
        };

        friend std::istream& operator>> (std::istream &in, CTextFile& txtfile) {
            size_t sz = 0;
            if (in.flags && std::ios::binary) {
                if ( readTypeSign<CPerson> (in) ) {
                    if (_path) delete[] _path;                    
                    if (_author) delete _author;
                    if (_keywords) delete[] _keywords;                    
                    in >> readChars(_path) >> *_author << readChars(_keywords) >> *_created >> _size;
                } else 
                    throw;
            }
            return in;
        };
        
};

// Елемент архіву {Список Текстових документ архіву, Дата (створення), ступінь архівації елемента архіву};
// Склад каталогу {Масив елементів архіву}.
class CArchiveEntry {
        CDate* _created=0;
        double _compress=0;
        CTextFile** _list;
        size_t _listmaxsize = 0;
        size_t _listsize = 0;
    public:
        CArchiveEntry() {}
        CArchiveEntry(const CDate* crtd, size_t maxsize) {
            created(crtd);
            _listmaxsize = maxsize;
            _list = new CTextFile* [maxsize];
        }

        const CDate* created(const CDate* crtd=0) { return setlocal<CDate>(crtd, _created); }
        double compress(double compr=std::numeric_limits<double>::max()) { 
            if (compr != std::numeric_limits<double>::max())
                if (compr < 0) throw;
                    else _compress = compr;
            return _compress;
        }
        size_t size() { return _listsize; }
        void push_back(const CTextFile& text) {
            if (_listsize < _listmaxsize) 
                _list[_listsize++] = new CTextFile(text);
        }

        friend std::ostream& operator<< (std::ostream &out, CArchiveEntry& aentry) {
            if (out.flags && std::ios::binary) {
                writeTypeSign<CFile> (out);
                out << writeChars(_path) << *_author << writeChars(_keywords) << *dynamic_cast<CFile*>(this);
            } else {
                out.width(65); out.precision(65); out.fixed;
                out << left << strcat(_path, "/", _filename, ".", _extension) << '|' << _size << '|' << *_created;
            }
            return out;
        };

        friend std::istream& operator>> (std::istream &in, CArchiveEntry& aentry) {
            size_t sz = 0;
            if (in.flags && std::ios::binary) {
                if ( readTypeSign<CPerson> (in) ) {
                    if (_path) delete[] _path;                    
                    if (_author) delete _author;
                    if (_keywords) delete[] _keywords;                    
                    in >> readChars(_path) >> *_author << readChars(_keywords) >> *_created >> _size;
                } else 
                    throw;
            }
            return in;
        };
        
        //индексация, вывод в поток
        

};


//implementation
CDate::CDate(unsigned yy, unsigned mm, unsigned dd) {
    year(yy);
    month(mm); 
    day(dd);
}
CDate::CDate(CDate& src) {
    year(src.year());
    month(src.month());
    day(src.day());
}

unsigned CDate::year(unsigned yr) {
    if (yr >= 1900 && yr < 3000) _year = yr;
    return _year;
}
unsigned CDate::month(unsigned mm) {
    if (mm > 0 && mm <= 13) _month = mm;
    return _month;
}
unsigned CDate::day(unsigned dd) {
    if (dd > 0 && dd <= 31) _day = dd;
    return _day;
}
