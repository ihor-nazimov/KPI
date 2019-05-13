#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// #include <iostream>
// using namespace std;

//base classes
class Date {
    protected:
        unsigned year = 0; 
        unsigned month = 0;
        unsigned day = 0;
    public:
        Date() {}
        Date(unsigned year, unsigned month, unsigned day): year(year), month(month), day(day) {}
        void show() {
            printf("Class Date(@%p): %2i.%2i.%2i\n", this, year, month, day);
        }
};

class Position {
        char* _name = 0;
        double _salary = 0;
        bool _vacant = false;
    public:
        Position() {}
        Position(const char* name, double salary=0, bool vacant=false): 
            _salary(salary), _vacant(vacant) {
            Position::name(name);
        }
        ~Position() {
            if (_name) delete[] _name;
        }
        const char* name(const char* name=0) {
            if (!_name) delete[] _name;
            if (!name) {
                _name = new char[strlen(name)+1];
                strcpy(_name, name);
            }
            return _name;
        }
        void display() {
            printf("Class Position(@%p): %s, salary=%10.2lg, %s\n", this, _name, _salary, _vacant ? "vacant" : "occupied");
        }
};

//derived classes
class Person: public Date {
        char* _name = 0;
        char* _surname = 0;
    public:
        Person() {}
        Person(const char* name, const char* surname, unsigned year, unsigned month, unsigned day) {
            this->year = year;
            this->month = month;
            this->day = day;
        }
        ~Person() {
            if (_name) delete[] _name;
            if (_surname) delete[] _surname;
        }
        const char* name(const char* name=0) {
            if (!_name) delete[] _name;
            if (!name) {
                _name = new char[strlen(name)+1];
                strcpy(_name, name);
            }
            return _name;
        }
        void show() {
            printf("Class Person(@%p): %s %s, birthday:%2i.%2i.%2i, \n", this, _name, _surname, year, month, day);
        }
};

class Employee: public Person, public Position { //public Date when was employed - inheritance is bad idea. Use incapsulation
        Date employed;
    public:
        Employee() {}
        Employee(Person& pers, Position& pos, Date& dt) {
            (Person) *this = pers;
            (Position) *this = pos;
            employed = dt;
        }
        void show() {
            printf("Class Employee(@%p):\n", this);
            printf("\t"); Person::show();
            printf("\t"); Position::display();
        }
};

class Sector: public Employee { //Person - head of sector
        char* _name = 0;
        Employee _elist[3];
    public:
        Sector() {}
        Sector(const char* nm, Person& head) {
            strcpy(Sector::_name = new char[strlen(nm)], nm);
            (Person) *this = head;
        }
        ~Sector() {
            if (Sector::_name) delete[] Sector::_name;
        }
        void display() {
            printf("Class Sector(@%p):\n", this);
            printf("Head of sector: "); Employee::show();
            printf("Employees:\n");
            for (int i = 0; i < 3; i++) {
                _elist[i].show();
            }
        }
};

int main(int argc, char** argv) {
    Date dt(2019, 5, 8);
    //Position pos = Position("CEO", 1000, true);
    //Person pers;// = Person(1976, 6, 28);
}