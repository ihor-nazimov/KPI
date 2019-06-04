#pragma once

// #include <limits>
// #include <limits.h>
#include <math.h>
// #include <exception>
// #include <stdexcept>
#include <iostream>
//#include <fstream>
// #include <iomanip>
#include <string.h>
#include <typeinfo>

//using namespace std;

template <class C> class Container: public C {
    public:
        static std::ostream& _writechars(std::ostream& of, const char* s, size_t sz=0){
            const char* CHARS_SIGN = "chars";
            if (!sz) sz = strlen(s);
            of.write( CHARS_SIGN, strlen(CHARS_SIGN)+1 ); //save char* signature
            of.write(reinterpret_cast<char*>(&sz), sizeof(size_t)); //save size of char* array 
            of.write(s, sz+1); //save content of char* array
        }
        static std::istream& _readchars(std::istream& ifile, char* s){
            const char* CHARS_SIGN = "chars";
            char* signature = new char[strlen( CHARS_SIGN )+1];
            std::streampos lastpos = ifile.tellg();
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
        std::ostream& save(std::ostream& of) {
            of.write(typeid(C).name(), sizeof(typeid(C).name())); //save class signature
            C::_write(of);
            return of; 
        }
        std::istream& load(std::istream& ifile, C* copy = 0) {
            char* signature = new char[sizeof(typeid(C).name())];
            std::streampos lastpos = ifile.tellg();
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

