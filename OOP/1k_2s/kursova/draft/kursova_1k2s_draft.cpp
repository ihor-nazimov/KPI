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
#include <fstream>
#include <iomanip>
#include <cassert>
#include <typeinfo>
#include <cstring>
//#include <climits> //ULONG_MAX
//#include <limits>  //double
//#include <cstdio>

using namespace std;

const streamsize MAX_CHARS = 1024;

// static bool f_binary = false;
// std::iostream& binary(std::iostream& strm) {
//     f_binary = true;
//     return strm;
// }

// std::iostream& text(std::iostream& strm) {
//     f_binary = false;
//     return strm;
// }



// template <class C>
// std::ostream& writeArray(std::ostream& out, C c[], const size_t sz) {
//     if (out.flags() & std::ios::binary) {
//         out.write(reinterpret_cast<char*>(&sz), sizeof(sz));
//         for (size_t i = 0; i < sz; i++) {
//             out << c[i];
//         }
//     } 
//     return out;
// }

// template <class C>
// size_t readArray(std::ostream& in, C* c[], size_t max_sz) {
//     size_t readed = 0;
//     streampos lastpos = in.tellg();
//     C tmp = new C();
//     if (in.flags() & std::ios::binary) {
//         in.read(&readed, sizeof(size_t));
//         for (size_t i = 0; i < readed; i++) {
//             in >> tmp;
//             if (i < max_sz) {
//                 if (c[i]) delete c[i];
//                 c[i] = new C(tmp);
//             }
//         } 
//     }
//     return readed < max_sz ? readed : max_sz;
// }


template <class C>
std::ostream& writeTypeSign(std::ostream& out) {
    if (typeid(out) == typeid(ofstream))
    // if (out.flags() & std::ios::binary) 
        //сігнатура нe містить довжину строки. Завантаження у довільному порядку не передбачено
        out.write(typeid(C).name(), strlen(typeid(C).name()) + 1 ); //save class signature
    else 
        out << typeid(C).name();
    return out;
}

template <class C>
bool readTypeSign(std::istream& in) {
    bool fVerified = false;
    streampos lastpos = in.tellg();
    if (typeid(in) == typeid(ifstream)) {
    // if (in.flags() & std::ios::binary) {
        size_t sz = strlen(typeid(C).name()) + 1;
        char* signature = new char[sz];
        in.read(signature, sz);
        if ( !strncmp(signature, typeid(C).name(), sz) )
            fVerified = true;
        else 
            in.seekg(lastpos);
            assert("readTypeSign: Error while reading file - incorrect class signature");
        delete signature;
    }
    return fVerified;
}


class CBase {
    public:
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
        char* readChars(std::istream& in) {
            size_t sz = 0;
            in.read(reinterpret_cast<char*>(&sz), sizeof(size_t));
            char* s = new char[sz];
            in.read(s, sz);
            return s;
        }

        template <class C>
        C& setlocal(const C& src, C& dest) {
            if (&src == &dest) return dest;
            if (src) {
                if ( !dest ) delete dest;
                dest = new C(src);
            }
            return *dest;
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

        static const char* inputChars(istream& in, ostream& out, const char* prompt, size_t n=255) {
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
        static double inputDouble(istream& in, ostream& out, const char* prompt) {
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

        static char* getFilename(char* dest, const char* def_filename, size_t n=255) {
            if (!cin) {cin.clear(); cin.ignore(MAX_CHARS, '\n'); } 
            std::cout << "Enter file name (" << def_filename << " by default): ";
            std::cin.getline(dest, n);
            if (strlen(dest) == 0) strncpy(dest, def_filename, n);
            return dest;
        }

        static int getChoice(const char* prompt) {
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
        }
};

class CDate: public CBase {
    protected:
        unsigned _year = 1900;
        unsigned _month = 0;
        unsigned _day = 0;
   public:
        CDate() {};
        CDate(unsigned yy, unsigned mm, unsigned dd);
        CDate(const CDate& src);

        CDate& input(istream& in, ostream& out, const char* prompt) {
            unsigned yy, mm, dd;
            char buf[255];
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
            in.clear(); in.ignore(MAX_CHARS, '\n');
            year(yy); month(mm); day(dd);
            return *this;
        }
        ~CDate() {}
       
        unsigned year(unsigned yr = 0);
        unsigned month(unsigned mm = 0);
        unsigned day(unsigned dd = 0);

        void save(std::ofstream& out) {
            writeTypeSign<CDate> (out);
            out.write(reinterpret_cast<char*>(&_year), sizeof(_year));
            out.write(reinterpret_cast<char*>(&_month), sizeof(_month));
            out.write(reinterpret_cast<char*>(&_day), sizeof(_day));
        }

        CDate& load(std::ifstream& in) {
            if ( readTypeSign<CDate> (in) ) {
                in.read(reinterpret_cast<char*>(&_year), sizeof(_year));
                in.read(reinterpret_cast<char*>(&_month), sizeof(_month));
                in.read(reinterpret_cast<char*>(&_day), sizeof(_day));
            } else 
                throw;
            return *this;
        }

        static std::ostream& header (std::ostream &out) {
            out << "   Date   ";
            return out;
        };

        friend std::ostream& operator<< (std::ostream &out, CDate &dt) {
        //    if (typeid(out) == typeid(ofstream)) {
        //         writeTypeSign<CDate> (out);
        //         out.write(reinterpret_cast<char*>(&dt._year), sizeof(dt._year));
        //         out.write(reinterpret_cast<char*>(&dt._month), sizeof(dt._month));
        //         out.write(reinterpret_cast<char*>(&dt._day), sizeof(dt._day));
        //     } else {
                out << fixed << right << setw(2) << setprecision(2) << dt._day << '.' 
                << fixed << right << setw(2) << setprecision(2) << dt._month << '.'
                << fixed << right << setw(4) << setprecision(4) << dt._year;
            // }
            return out;
        };

        // friend std::ifstream& operator>> (std::ifstream &in, CDate &dt) {
        //     // if (in.flags() & std::ios::binary) {
        //     // if (f_binary) {
        //     // }
        //     return in;
        // };
};

class CPerson: public CBase {
        char* _name;
        char* _surname;
        CDate _birthday;
    public:
        CPerson() {
            name("");
            surname("");
        }
        CPerson(const char* nm, const char* surnm, CDate& bd): CPerson() {
            birthday(bd);
            name(nm);
            surname(surnm);
        }
        CPerson(const CPerson& src): CPerson() {
            // _name = 0;
            // _surname = 0;
            surname(const_cast<CPerson&>(src).surname());
            name(const_cast<CPerson&>(src).name());
            birthday(const_cast<CPerson&>(src).birthday());
        }
        ~CPerson() {
            if (_name) delete[] _name;
            if (_surname) delete[] _surname;
            //if (_birthday) delete[] _birthday;
        }

        CPerson& input(istream& in, ostream& out, const char* prompt) {
            // CPerson* tmppers = new CPerson();
            out << prompt;
            if (_name) delete[] _name;
            if (_surname) delete[] _surname;
            name(inputChars(in, out, "Enter name: "));
            surname(inputChars(in, out, "Enter surname: "));
            birthday( (new CDate())->input(in, out, "Enter birthday: "));
            return *this;
        }

        const char* name(const char* nm=0) { return _name = setchars(nm, _name); }
        //const char* name() { return _name; }
        const char* surname(const char* surnm=0) { return _surname = setchars(surnm, _surname); }
        //const char* surname() { return _surname; }
        const CDate& birthday(const CDate& dt) { return _birthday = dt; }
        const CDate& birthday() { return _birthday; }

        void save(std::ofstream& out) {
            writeTypeSign<CPerson> (out);
            assert(_name && _surname);
            writeChars(out, _name); 
            // size_t sz = strlen(_name)+1;
            // out.write(reinterpret_cast<char*>(&sz), sizeof(size_t));
            // out.write(_name), sz);
            
            writeChars(out, _surname);
            // size_t sz = strlen(_surname)+1;
            // out.write(reinterpret_cast<char*>(&sz), sizeof(size_t));
            // out.write(_surname, sz);

            _birthday.save(out);
        }

        CPerson& load(std::ifstream& in) {
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

        static std::ostream& header(std::ostream &out) {
            out << "   Name   |  Surname | Birthday";
            return out;
        };

        friend std::ostream& operator<< (std::ostream &out, CPerson& pers) {
            // if (typeid(out) == typeid(ofstream)) {
            // // if (out.flags() & std::ios::binary) {
            // // if (f_binary) {
            //     writeTypeSign<CPerson> (out);
            //     assert(pers._name && pers._surname);
            //     pers.writeChars(out, pers._name); 
            //     // size_t sz = strlen(_name)+1;
            //     // out.write(reinterpret_cast<char*>(&sz), sizeof(size_t));
            //     // out.write(_name), sz);
                
            //     pers.writeChars(out, pers._surname);
            //     // size_t sz = strlen(_surname)+1;
            //     // out.write(reinterpret_cast<char*>(&sz), sizeof(size_t));
            //     // out.write(_surname, sz);

            //     out << pers._birthday;
            // } else {
                // out.width(10); out.precision(10); out.fixed;
                out << fixed << setw(10) << setprecision(10) << left << pers._name << '|' 
                << fixed << setw(10) << setprecision(10) << left << pers._surname << '|' 
                << pers._birthday;
            // }
            return out;
        };

        // friend std::ifstream& operator>> (std::ifstream &in, CPerson& pers) {
        //     size_t sz = 0;
        //     // if (in.flags() & std::ios::binary) {
        //     // if (f_binary) {
        //         if ( readTypeSign<CPerson> (in) ) {
        //             if (pers._name) delete[] pers._name;                    
        //             if (pers._surname) delete[] pers._surname;
        //             //if (pers._birthday) delete pers._birthday;
        //             pers._name = pers.readChars(in);
        //             pers._surname = pers.readChars(in);
        //             //pers._birthday = new CDate();
        //             in >> pers._birthday;
        //         } else 
        //             throw;
        //     // }
        //     return in;
        // };
};

class CFile: public CBase {
    protected:
        char* _filename;
        char* _extention;
        CDate _created;
        unsigned long _size = 0;
    public:
        CFile() {
            // _filename = new char[1]; _filename=0;
            // _extention = new char[1]; _extention=0;
            filename("");
            extention("");
        }
        CFile(const char* fnm, const char* ext, const CDate& crtd, unsigned long sz) {
            filename(fnm);
            extention(ext);
            date(crtd);
            size(sz);
        }
        CFile(const CFile& src) {
            filename(const_cast<CFile&>(src).filename());
            extention(const_cast<CFile&>(src).extention());
            date(const_cast<CFile&>(src).date());
            size(const_cast<CFile&>(src).size());
        }
        ~CFile() {
            if (_filename) delete[] _filename;
            if (_extention) delete[] _extention;
            //if (_created) delete _created;
        }
        CFile& input(istream& in, ostream& out, const char* prompt) {
            // CFile* tmpf = new CFile();
            out << prompt;
            if (_filename) delete[] _filename;
            if (_extention) delete[] _extention;
            filename(inputChars(in, out, "Enter file name: "));
            extention(inputChars(in, out, "Enter file extention: "));
            date((new CDate)->input(in, out, "Enter date of file creation: "));
            return *this;
        }
        const char* filename(const char* fnm=0) { return _filename = setchars(fnm, _filename); }
        //const char* filename() { return _filename; }
        const char* extention(const char* ext=0) { return _extention = setchars(ext, _extention); }
        //const char* extention() { return _extention; }
        unsigned long size(unsigned long sz) { return _size = sz; }
        unsigned long size() { return _size; }
        const CDate& date(const CDate& dt) { return _created = dt; }
        const CDate& date() { return _created; }
        
        void save(std::ofstream& out) {
            writeTypeSign<CFile> (out);
            writeChars(out, _filename); 
            writeChars(out, _extention); 
            _created.save(out);
            out.write(reinterpret_cast<char*>(&_size), sizeof(_size));
        }

        CFile& load(std::ifstream& in) {
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

        static std::ostream& header (std::ostream &out) {
            out << "       Size      | Created  |             Filename";
            return out;
        };

        friend std::ostream& operator<< (std::ostream &out, CFile& file) {
            // if (typeid(out) == typeid(ofstream)) {
            // //if (out.flags() & std::ios::binary) {
            //     writeTypeSign<CFile> (out);
            //     file.writeChars(out, file._filename); 
            //     file.writeChars(out, file._extention); 
            //     out << file._created;
            //     out.write(reinterpret_cast<char*>(&file._size), sizeof(file._size));
            // } else {
                // out.width(17); out.precision(17); out.fixed;
                out << left << fixed << setw(17) << setprecision(17) << file._size << '|' 
                << file._created << '|' 
                << left << setw(0) << setprecision(0) << file._filename << "." << file._extention;
            // }
            // return out;
        };

        // friend std::ifstream& operator>> (std::ifstream &in, CFile& file) {
        //     size_t sz = 0;
        //     // if (in.flags() & std::ios::binary) {
        //         if ( readTypeSign<CFile> (in) ) {
        //             if (file._filename) delete[] file._filename;                    
        //             if (file._extention) delete[] file._extention;
        //             // if (_created) delete _created;                    
        //             file._filename = file.readChars(in);
        //             file._extention = file.readChars(in);
        //             in >> file._created;
        //             in.read(reinterpret_cast<char*>(&file._size), sizeof(file._size));
        //         } else 
        //             throw;
        //     // }
        //     return in;
        // };
        
};

class CTextFile: public CFile {
    protected:
        char* _path = 0;
        CPerson* _author = 0;
        char* _keywords = 0;
    public:
        CTextFile(){
            path("");
            keywords("");
        }
        CTextFile(const CFile& file, const char* pth, CPerson& auth, const char* keywds): 
        CFile(file) {
            path(pth);
            author(auth);
            keywords(keywds);
        }
        CTextFile(const CTextFile& src): CFile(src) {
            CTextFile();
            path(const_cast<CTextFile&>(src).path());
            author(const_cast<CTextFile&>(src).author());
            keywords(const_cast<CTextFile&>(src).keywords());
        } 
        ~CTextFile() {
            if (_path) delete[] _path;
            if (_keywords) delete[] _keywords;
            //if (_author) delete _author;
        }
        CTextFile& input(istream& in, ostream& out, const char* prompt) {
            out << prompt;
            if (_path) delete[] _path;
            if (_keywords) delete[] _keywords;
            path(inputChars(in, out, "Enter file path: "));
            dynamic_cast<CFile*>(this) -> input(in, out, "");
            keywords(inputChars(in, out, "Enter keywords: "));
            author( (new CPerson)->input(in, out, "Enter author: \n"));
            return *this;
        }
        const char* path(const char* pth=0) { return _path = setchars(pth, _path); }
        const char* keywords(const char* keywds=0) { return _keywords = setchars(keywds, _keywords); }
        const CPerson* author(const CPerson& auth) { 
            return _author = const_cast<CPerson*>(&auth); }
        const CPerson& author() { return *_author; }
        
        void save(std::ofstream& out) {
            writeTypeSign<CTextFile> (out);
            writeChars(out, _path); 
            out << *_author;
            writeChars(out, _keywords); 
            CFile::save(out);
        }

        CTextFile& load(std::ifstream& in) {
            if ( readTypeSign<CTextFile> (in) ) {
                if ( _path) delete[]  _path;                    
                //if (_author) delete _author;
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

        static std::ostream& header(std::ostream &out) {
            out << "       Size      |" << CDate::header << "|"
            << "                     Full filename                                ";
            return out;
        };

        friend std::ostream& operator<< (std::ostream &out, CTextFile& txtfile) {
            // if (out.flags() & std::ios::binary) {
            // if (typeid(out) == typeid(ofstream)) {
            //     writeTypeSign<CTextFile> (out);
            //     txtfile.writeChars(out, txtfile._path); 
            //     out << *txtfile._author;
            //     txtfile.writeChars(out, txtfile._keywords); 
            //     out << *dynamic_cast<CFile*>(&txtfile);
            // } else {
                out.width(15); out.precision(15); out.fixed;
                out << left << fixed << setw(17) << setprecision(17) << txtfile._size << '|' 
                << txtfile._created << '|' 
                << txtfile._path << "/" << txtfile._filename << "." << txtfile._extention;
            // }
            return out;
        };

        // friend std::ifstream& operator>> (std::ifstream &in, CTextFile& txtfile) {
        //     size_t sz = 0;
        //     // if (in.flags() & std::ios::binary) {
        //     // if (typeid(in) == typeid(ifstream)) {
        //         if ( readTypeSign<CTextFile> (in) ) {
        //             if (txtfile._path) delete[] txtfile._path;                    
        //             //if (_author) delete _author;
        //             if (txtfile._keywords) delete[] txtfile._keywords;
                    
        //             txtfile._path = txtfile.readChars(in);
        //             CPerson* tmp_author = new CPerson();
        //             in >> *tmp_author;                    
        //             txtfile._author = tmp_author;
        //             txtfile._keywords = txtfile.readChars(in);

        //             in >> *dynamic_cast<CFile*>(&txtfile);
        //         } else 
        //             throw;
        //     // }
        //      return in;
        // };
        
};

// Елемент архіву {Список Текстових документ архіву, Дата (створення), ступінь архівації елемента архіву};
class CArchiveEntry: public CBase {
    protected:
        CDate _created;
        double _compress=0;
        CTextFile** _list = 0;
        size_t _listmaxsize = 0;
        size_t _listsize = 0;
    public:
        CArchiveEntry() {
            _listmaxsize = 100;
            _listsize = 0;
            _list = new CTextFile* [_listmaxsize];
        }
        CArchiveEntry(const CDate& crtd, size_t maxsize) {
            created(crtd);
            _listmaxsize = maxsize;
            _listsize = 0;
            _list = new CTextFile* [maxsize];
        }
        CArchiveEntry(const CArchiveEntry& src): _listmaxsize(src._listmaxsize), _listsize(src._listsize) {
            created(src._created);
            compress(src._compress);
            _list = new CTextFile* [_listmaxsize];
            for (size_t i=0; i < _listmaxsize; i++)
                if (i < _listsize) _list[i] = new CTextFile(* (src._list[i]) );
                    else _list[i] = 0;
        }

        ~CArchiveEntry() {
            for (size_t i=0; i < _listsize; i++)
                if (i < _listsize) delete _list[i];
            delete[] _list;
        }

        CArchiveEntry& input(istream& in, ostream& out, const char* prompt) {
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

        const CDate& created(const CDate& crtd) { return _created = crtd; }
        const CDate& created() { return _created; }
        double compress(double compr) { return _compress = compr; }
        double compress() { return _compress; }
        size_t listsize() { return _listsize; }
        void push_back(CTextFile& text) {
            if (_listsize < _listmaxsize) 
                _list[_listsize++] = &text;
        }

        void save(std::ofstream& out) {
                writeTypeSign<CArchiveEntry> (out);
                _created.save(out);
                out.write(reinterpret_cast<char*>(& _compress), sizeof( _compress));
                out.write(reinterpret_cast<char*>(& _listmaxsize), sizeof( _listmaxsize));
                out.write(reinterpret_cast<char*>(& _listsize), sizeof( _listsize));
                for (size_t i = 0; i <  _listsize; i++) 
                    _list[i]->save(out);
        }

        CArchiveEntry& load(std::ifstream& in) {
            if ( readTypeSign<CArchiveEntry> (in) ) {
                // if (_created) delete _created;

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
                    _list[i]->load(in);
                }
                    
            } else 
                throw;
            return *this;
        }

        static std::ostream& header (std::ostream &out) {
            out << " Created  |  Files Qty  ";
            return out;
        };

        friend std::ostream& operator<< (std::ostream &out, CArchiveEntry& aentry) {
            // if (out.flags() & std::ios::binary) {
            // if ( typeid(out) == typeid(ofstream) ) {
            //     writeTypeSign<CArchiveEntry> (out);
            //     out << aentry._created;
            //     out.write(reinterpret_cast<char*>(&aentry._compress), sizeof(aentry._compress));
            //     out.write(reinterpret_cast<char*>(&aentry._listmaxsize), sizeof(aentry._listmaxsize));
            //     out.write(reinterpret_cast<char*>(&aentry._listsize), sizeof(aentry._listsize));
            //     for (size_t i = 0; i < aentry._listsize; i++) 
            //         out << *aentry._list[i];
            // } else {
                out << aentry._created << '|' 
                << fixed << left << setw(17) << setprecision(17) << aentry._listsize;
                out << endl << CTextFile::header << endl;
                for (size_t i = 0; i < aentry._listsize; i++) 
                    out << *aentry._list[i] << endl;
            // }
            return out;
        };

        // friend std::ifstream& operator>> (std::ifstream &in, CArchiveEntry& aentry) {
        //     size_t sz = 0;
        //     // if (in.flags() & std::ios::binary) {
        //     // if (typeid(in) == typeid())
        //         if ( readTypeSign<CArchiveEntry> (in) ) {
        //             // if (_created) delete _created;

        //             for (size_t i=0; i < aentry._listsize; i++)
        //                 if (aentry._list[i]) delete aentry._list[i];
        //             delete[] aentry._list;                        

        //             in >> aentry._created;
        //             in.read(reinterpret_cast<char*>(&aentry._compress), sizeof(aentry._compress));
        //             in.read(reinterpret_cast<char*>(&aentry._listmaxsize), sizeof(aentry._listmaxsize));
        //             in.read(reinterpret_cast<char*>(&aentry._listsize), sizeof(aentry._listsize));
                    
        //             aentry._list = new CTextFile* [aentry._listmaxsize];
        //             for (size_t i = 0; i < aentry._listsize; i++) {
        //                 aentry._list[i] = new CTextFile();
        //                 in >> *aentry._list[i];
        //             }
                        
        //         } else 
        //             throw;
        //     // }
        //     return in;
        // };
        
        CTextFile& operator[] (size_t index) {
            if (index >= 0 & index < _listsize) 
                return * (_list[index]);
            else throw;
        }
};

class CArchive {
        CArchiveEntry** _list = 0;
        size_t _listmaxsize = 100;
        size_t _listsize = 0;
    public:
        CArchive(size_t sz=100) {
            _listmaxsize = sz;
            _list = new CArchiveEntry* [_listmaxsize];
        }  

        size_t size() { return _listsize; }
        void push_back(CArchiveEntry& src) {
            if (_listsize < _listmaxsize) 
                _list[_listsize++] = &src;
        }
        CArchiveEntry& operator[] (size_t index) {
            if (index >= 0 & index < _listsize) 
                return * (_list[index]);
            else throw;
        }

        void save(std::ofstream& out) {
            writeTypeSign<CArchive> (out);
            out.write(reinterpret_cast<char*>(&_listmaxsize), sizeof(_listmaxsize));
            out.write(reinterpret_cast<char*>(&_listsize), sizeof(_listsize));
            for (size_t i = 0; i < _listsize; i++) 
                _list[i]->save(out);
        }

        CArchive& load(std::ifstream& in) {
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

        static std::ostream& header (std::ostream &out) {
            out << "  Archive entries number \n";
            return out;
        };

        friend std::ostream& operator<< (std::ostream &out, CArchive& aentry) {
            // if (out.flags() & std::ios::binary) {
            // if (typeid(out) == typeid(ofstream)) {
            //     writeTypeSign<CArchive> (out);
            //     out.write(reinterpret_cast<char*>(&aentry._listmaxsize), sizeof(aentry._listmaxsize));
            //     out.write(reinterpret_cast<char*>(&aentry._listsize), sizeof(aentry._listsize));
            //     for (size_t i = 0; i < aentry._listsize; i++) 
            //         out << *(aentry._list[i]);
            // } else {
                out.width(10); out.precision(10); out.fixed;
                out << right << aentry._listsize;
                out << endl << "List of archive entry\n" << CArchiveEntry::header << endl;
                for (size_t i = 0; i < aentry._listsize; i++) 
                    out << *(aentry._list[i]) << endl;
            // }
            return out;
        };

        // friend std::ifstream& operator>> (std::ifstream &in, CArchive& aentry) {
        //     size_t sz = 0;
        //     // if (in.flags() & std::ios::binary) {
        //         if ( readTypeSign<CArchive> (in) ) {
        //             for (size_t i=0; i < aentry._listsize; i++)
        //                 if (aentry._list[i]) delete aentry._list[i];
        //             delete[] aentry._list;                        

        //             in.read(reinterpret_cast<char*>(&aentry._listmaxsize), sizeof(aentry._listmaxsize));
        //             in.read(reinterpret_cast<char*>(&aentry._listsize), sizeof(aentry._listsize));
        //             aentry._list = new CArchiveEntry* [aentry._listmaxsize];
        //             for (size_t i = 0; i < aentry._listsize; i++) {
        //                 aentry._list[i] = new CArchiveEntry();
        //                 in >> *(aentry._list[i]);
        //             }
        //         } else 
        //             throw;
        //     // }
        //     return in;
        // };
        
};



int main(int argc, char const *argv[])
{
    CArchive* archive = new CArchive(200); //archive size limit

    int choice = -1;
    char filename[255];
    const char* def_filename = "kurs4.dat";
    char buf[255];
    ofstream ofile;
    ifstream ifile;
    ostream* out;
    const char* mainMenu = 
        "1. Enter new archive entry\n"
        "2. Display archive\n" 
        "3. Load from binary\n"
        "4. Save as binary\n"
        "5. Save as text\n"
        "0. Exit \n>";

    CArchiveEntry* tmpae;
    do {
        switch (choice = CBase::getChoice(mainMenu)) {
            case 1: 
                tmpae = new CArchiveEntry(); 
                archive->push_back((new CArchiveEntry())->input(cin, cout, "Enter new archive entry:\n"));
                break;
            case 2:
                std::cout << endl << CArchive::header << *archive << endl;
                break;
            case 3:
                CBase::getFilename(filename, def_filename, sizeof(filename));
                ifile.open(filename, ios_base::in | ios_base::binary);
                archive->load(ifile);
                ifile.close();
                std::cout << "Archive was loaded from " << filename << endl << endl;
                break;
            case 4:
                CBase::getFilename(filename, def_filename, sizeof(filename));
                ofile.open(filename, ios_base::out | ios_base::binary);
                archive->save(ofile);
                ofile.close();
                std::cout << "Archive was saved as binary into " << filename << endl << endl;
                break;
            case 5:
                CBase::getFilename(filename, def_filename, sizeof(filename));
                ofile.open(filename, ios_base::out);
                // out = dynamic_cast<ostream*>(&ofile); 
                ofile << *archive;
                ofile.close();
                std::cout << "Archive was saved as text into " << filename << endl << endl;
                break;
            case 0:
                std::cout << "Bye!\n" << endl;
                break;
            default:
                std::cout << "Incorrect choice" << endl;
        }

    }
    while (choice != 0);

    return 0;
}


//implementation
CDate::CDate(unsigned yy, unsigned mm, unsigned dd) {
    year(yy);
    month(mm); 
    day(dd);
}
CDate::CDate(const CDate& src) {
    year(src._year);
    month(src._month);
    day(src._day);
}

unsigned CDate::year(unsigned yr) {
    if (yr >= 1500 & yr < 3000) _year = yr;
        else std::cerr << "Incorrect year" << endl;
    return _year;
}
unsigned CDate::month(unsigned mm) {
    if (mm > 0 & mm <= 13) _month = mm;
        else std::cerr << "Incorrect month" << endl;
    return _month;
}
unsigned CDate::day(unsigned dd) {
    if (dd > 0 & dd <= 31) _day = dd;
        else std::cerr << "Incorrect day" << endl;
    return _day;
}
