#include <cstdlib>
//#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
using namespace std;

const int MAX_EMPLOYEES = 3;

//base classes
class Date {
    protected:
        unsigned year = 0; 
        unsigned month = 0;
        unsigned day = 0;
        const Date& _copy(const Date& src) { return *this = src; }
    public:
        Date() {}
        Date(unsigned year, unsigned month, unsigned day): year(year), month(month), day(day) {}
        Date(const Date &dt): year(dt.year), month(dt.month), day(dt.day) {}
        ~Date(){}
        virtual void show() {
            printf("Class Date(@%p): %2i.%2i.%2i\n", this, year, month, day);
        }
};

class Position {
    private:
        bool _vacant = false;
        double _salary = 0;
        char* _title = 0;
    protected:
        const Position& _copy(const Position& src) {
            *this = src;
            _title = 0;
            title(const_cast<Position&>(src).title());
            return *this;
        } 
    public:
        Position() {}
        Position(const char* title, double salary=0, bool vacant=false): 
            _salary(salary), _vacant(vacant) {
            Position::title(title);
        }
        Position(const Position& pos): _salary(pos._salary), _vacant(pos._vacant) {
            _copy(pos);
        }
        ~Position() {
            if (_title) delete[] _title;
        }
        const char* title(const char* title=0) {
            if (title==_title) return _title; 
            if (title) {
                if (!_title) delete[] _title; 
                _title = new char[strlen(title)+1];
                strcpy(_title, title);
            }
            return _title;
        }
        virtual void display() {
            printf("Class Position(@%p): %s, salary=%10.2lf, %s\n", this, _title, _salary, _vacant ? "vacant" : "occupied");
        }
};

//derived classes
class Person: public Date {
        char* _name = 0;
        char* _surname = 0;
    protected:
        const Person& _copy(const Person& src) {
            *this = src;
            _name = 0;
            _surname = 0;
            surname(const_cast<Person&>(src).surname());
            name(const_cast<Person&>(src).name());
            Date::_copy((Date)*this);
            return *this; 
        }
    public:
        Person() {}
        Person(const char* nm, const char* surnm, unsigned year, unsigned month, unsigned day) {
            this->year = year;
            this->month = month;
            this->day = day;
            name(nm);
            surname(surnm);
        }
        Person(const Person& pers) {
            _copy(pers);
        }
        ~Person() {
            if (_name) delete[] _name;
            if (_surname) delete[] _surname;
        }
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
        void show() {
            printf("Class Person(@%p): %s %s, birthday is: \n\t", this, _name, _surname);
            Date::show();
        }
};

class Employee: public Person, public Position { //public Date when was employed - inheritance is bad idea. Use incapsulation
        Date employed;
    protected:
        const Employee& _copy(const Employee& src) {
            *this = src;
            Person::_copy((Person) src);
            Position::_copy((Position) src);
        }
    public:
        Employee() {}
        Employee(Person& pers, Position& pos, Date& empld) {
            Person::_copy((Person) pers);
            Position::_copy((Position) pos);
            employed = empld; //copy Date fields
        }
        Employee(const Employee &emp): employed(emp.employed) {
            _copy(emp);        
        }
        ~Employee() {
        }
        void show() {
            printf("Class Employee(@%p):\n", this);
            printf("\t"); Person::show();
            printf("\t"); Position::display();
        }
};

class Sector: public Employee { //Person is head of sector
        char* _sectorname = 0;
        //head of sector placed in base class Employee
    public:
        Employee* _elist[MAX_EMPLOYEES];
        Sector() {}
        Sector(const char* sectnm, Employee& head) {
            sectorname(sectnm);
            Employee::_copy(head);
            for (int i=0; i<MAX_EMPLOYEES-1; i++) 
                _elist[i] = 0;

        }
        Sector(const Sector& sect) {
            sectorname(sect._sectorname);
            Employee::_copy((Employee) sect);
            for (int i=0; i<MAX_EMPLOYEES-1; i++) 
                _elist[i] = new Employee(*sect._elist[i]);
        }
        ~Sector() {
            for (int i=0; i<MAX_EMPLOYEES-1; i++) 
                if (_elist[i]) delete _elist[i];
            if (Sector::_sectorname) delete[] Sector::_sectorname;
        }
        const char* sectorname(const char* sectorname=0) {
            if (!_sectorname) delete[] _sectorname;
            if (sectorname) {
                _sectorname = new char[strlen(sectorname)+1];
                strcpy(_sectorname, sectorname);
            }
            return _sectorname;
        }
        void display() {
            printf("Class Sector(@%p):\n", this);
            printf("\tHead of sector: "); Employee::show();
            printf("\tEmployee list:\n");
            for (int i = 0; i < MAX_EMPLOYEES; i++) {
                printf("\t#%i\n", i);
                _elist[i]->show();
            }
        }
};

int main(int argc, char** argv) {

    std::cout << "sizeof(Date)" << sizeof(Date) << std::endl;
    std::cout << "sizeof(Person)" << sizeof(Person) << std::endl;
    std::cout << "sizeof(Position)" << sizeof(Position) << std::endl;
    std::cout << "sizeof(Employee)" << sizeof(Employee) << std::endl;
    std::cout << "sizeof(Sector)" << sizeof(Sector) << std::endl << std::endl;

    //head of sector
    Employee* head = new Employee(
            *new Person("Artur", "Conan Doyle", 1859, 5, 22), 
            *new Position("author", 1000000, false), 
            *new Date(1891, 01, 01)); //employed
    cout << "Employee head is:\n";
    head->show();
    cout << endl;

    Sector* bs221b = new Sector("221B Baker Street", *head);
    bs221b->_elist[0] = new Employee(
            *new Person("Sherlock", "Holmes", 1850, 1, 1), 
            *new Position("consulting detective", 100000, false), 
            *new Date(1891, 01, 02)); //employed
    bs221b->_elist[1] = new Employee(
            *new Person("John", "Watson", 1850, 1, 2), 
            *new Position("assistant", 100000, false), 
            *new Date(1891, 01, 02)); //employed
    bs221b->_elist[2] = new Employee(
            *new Person("Martha", "Hudson", 1850, 1, 3), 
            *new Position("landlady", 100000, false), 
            *new Date(1891, 01, 03)); //employed

    cout << "Sector is:\n";
    bs221b->display();
    cout << endl;
    getchar();

    Date* dt = bs221b;
    cout << "Date::show() "; dt->show(); cout << endl; 
    Position* pos = bs221b;
    cout << "Position::display() "; pos->display(); cout << endl; 
    Person* pers = bs221b;
    cout << "Person::show() "; pers->show(); cout << endl; 
    Employee* empl = bs221b;
    cout << "Employee::show() "; empl->show(); cout << endl; 
    cout << "Employee::display() "; empl->display(); cout << endl; 

    delete bs221b;
    delete head;
    
    getchar();


}