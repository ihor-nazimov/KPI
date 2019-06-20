#ifndef KURSOVA_1K2S_CLASSES_H
#define KURSOVA_1K2S_CLASSES_H

/*#define NDEBUG */
 
#include <iostream>
#include <fstream>
#include <typeinfo>

//#include <climits> //ULONG_MAX
//#include <limits>  //double
//#include <cstdio>

using namespace std;

const streamsize MAX_CHARS = 1024;

template <class C>
std::ostream& writeTypeSign(std::ostream& out) {
    if (typeid(out) == typeid(ofstream))
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


template <class C>
C& setlocal(const C& src, C& dest) {
    if (&src == &dest) return dest;
    if (src) {
        if ( !dest ) delete dest;
        dest = new C(src);
    }
    return *dest;
}

//save char* and it's length and into binary stream
std::ostream& writeChars(std::ostream& out, const char* s);
//load char* from binary stream
char* readChars(std::istream& in);
char* setchars(const char* src, char* dest);
char* inputChars(istream& in, ostream& out, const char* prompt, size_t n=255);
double inputDouble(istream& in, ostream& out, const char* prompt);
char* getFilename(char* dest, const char* def_filename, size_t n=255);
int getChoice(const char* prompt);

class CDate {
    protected:
        unsigned _year = 1900;
        unsigned _month = 0;
        unsigned _day = 0;
   public:
        CDate() {};
        CDate(unsigned yy, unsigned mm, unsigned dd);
        CDate(CDate& src);
        CDate& input(istream& in, ostream& out, const char* prompt);
        ~CDate() {}
        unsigned year(unsigned yr = 0);
        unsigned month(unsigned mm = 0);
        unsigned day(unsigned dd = 0);
        void save(std::ofstream& out);
        CDate& load(std::ifstream& in);
        static std::ostream& header (std::ostream &out);
        friend std::ostream& operator<< (std::ostream &out, CDate &dt);
        CDate& operator=(CDate& dt) { 
            _year = dt.year();
            _month = dt.month();
            _day = dt.day(); 
            return *this; }
};

class CPerson {
        char* _name;
        char* _surname;
        CDate _birthday;
    public:
        CPerson() { name(""); surname(""); }
        CPerson(const char* nm, const char* surnm, CDate& bd);
        CPerson(CPerson& src);
        ~CPerson();
        CPerson& input(istream& in, ostream& out, const char* prompt);
        const char* name(const char* nm=0) { return _name = setchars(nm, _name); }
        const char* surname(const char* surnm=0) { return _surname = setchars(surnm, _surname); }
        CDate& birthday(CDate& dt) { return _birthday = dt; }
        CDate& birthday() { return _birthday; }
        void save(std::ofstream& out);
        CPerson& load(std::ifstream& in);
        static std::ostream& header(std::ostream &out);
        friend std::ostream& operator<< (std::ostream &out, CPerson& pers);
};

class CFile {
    protected:
        char* _filename;
        char* _extention;
        CDate _created;
        unsigned long _size = 0;
    public:
        CFile() { filename(""); extention(""); }
        CFile(const char* fnm, const char* ext, CDate& crtd, unsigned long sz);
        CFile(CFile& src);
        ~CFile();
        CFile& input(istream& in, ostream& out, const char* prompt);
        const char* filename(const char* fnm=0) { return _filename = setchars(fnm, _filename); }
        const char* extention(const char* ext=0) { return _extention = setchars(ext, _extention); }
        unsigned long size(unsigned long sz) { return _size = sz; }
        unsigned long size() { return _size; }
        CDate& date(CDate& dt) { return _created = dt; }
        CDate& date() { return _created; }
        void save(std::ofstream& out);
        CFile& load(std::ifstream& in);
        static std::ostream& header (std::ostream &out);
        friend std::ostream& operator<< (std::ostream &out, CFile& file);
};

class CTextFile: public CFile {
    protected:
        char* _path = 0;
        CPerson* _author = 0;
        char* _keywords = 0;
    public:
        CTextFile(){ path(""); keywords(""); }
        CTextFile(CFile& file, const char* pth, CPerson& auth, const char* keywds);
        CTextFile(CTextFile& src);
        ~CTextFile();
        CTextFile& input(istream& in, ostream& out, const char* prompt);
        const char* path(const char* pth=0) { return _path = setchars(pth, _path); }
        const char* keywords(const char* keywds=0) { return _keywords = setchars(keywds, _keywords); }
        CPerson* author(CPerson& auth) { return _author = const_cast<CPerson*>(&auth); }
        CPerson& author() { return *_author; }
        void save(std::ofstream& out);
        CTextFile& load(std::ifstream& in);
        static std::ostream& header(std::ostream &out);
        friend std::ostream& operator<< (std::ostream &out, CTextFile& txtfile);
};

// Елемент архіву {Список Текстових документ архіву, Дата (створення), ступінь архівації елемента архіву};
class CArchiveEntry {
    protected:
        CDate _created;
        double _compress=0;
        CTextFile** _list = 0;
        size_t _listmaxsize = 0;
        size_t _listsize = 0;
    public:
        CArchiveEntry();
        CArchiveEntry(CDate& crtd, size_t maxsize);
        CArchiveEntry(CArchiveEntry& src);
        ~CArchiveEntry();
        CArchiveEntry& input(istream& in, ostream& out, const char* prompt);
        CDate& created(CDate& crtd) { return _created = crtd; }
        CDate& created() { return _created; }
        double compress(double compr) { return _compress = compr; }
        double compress() { return _compress; }
        size_t listsize() { return _listsize; }
        void push_back(CTextFile& text);
        void save(std::ofstream& out);
        CArchiveEntry& load(std::ifstream& in);
        static std::ostream& header (std::ostream &out);
        friend std::ostream& operator<< (std::ostream &out, CArchiveEntry& aentry);
        CTextFile& operator[] (size_t index);
        double operator+=(double compr) { return _compress += compr; }
        double operator-=(double compr) { return _compress -= compr; }
};

class CArchive {
        CArchiveEntry** _list = 0;
        size_t _listmaxsize = 100;
        size_t _listsize = 0;
    public:
        CArchive(size_t sz=100);
        size_t size() { return _listsize; }
        void push_back(CArchiveEntry& src);
        CArchiveEntry& operator[] (size_t index);
        void save(std::ofstream& out);
        CArchive& load(std::ifstream& in);
        static std::ostream& header (std::ostream &out);
        friend std::ostream& operator<< (std::ostream &out, CArchive& aentry);
        CArchiveEntry& operator+=(CArchiveEntry& newEntry) { push_back(newEntry); return *_list[_listsize-1]; }
};


#endif