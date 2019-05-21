/* TODO:
1. Тестирование классов кроме Date
    (Person) *this = pers;
            
2. Использование show / display с ранним и поздним связываниями
3. Влияние виртуальности на корректное отображение
4. *** на размер объектов конечного класса 
*/

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
        char* _title = 0;
        double _salary = 0;
        bool _vacant = false;
    public:
        Position() {}
        Position(const char* title, double salary=0, bool vacant=false): 
            _salary(salary), _vacant(vacant) {
            Position::title(title);
        }
        Position(const Position& pos): _salary(pos._salary), _vacant(pos._vacant) {
            Position::title(pos._title);
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
            printf("Class Position(@%p): %s, salary=%10.2lg, %s\n", this, _title, _salary, _vacant ? "vacant" : "occupied");
        }
};

//derived classes
class Person: public Date {
        char* _name = 0;
        char* _surname = 0;
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
            name(pers._name);
            surname(pers._surname);
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
    public:
        Employee() {}
        Employee(Person& pers, Position& pos, Date& empld) {
            Person* tmppers = (Person*) this;
            *tmppers = pers;
            Position* tmppos = (Position*) this;
            *tmppos = pos;
            employed = empld;
        }
        Employee(const Employee &emp): employed(emp.employed) {
            Person* tmppers = (Person*) this;
            *tmppers = (Person) emp;
            Position* tmppos = (Position*) this;
            *tmppos = (Position) emp;
        }
        ~Employee() {
        }
        void show() {
            printf("Class Employee(@%p):\n", this);
            printf("\t"); Person::show();
            printf("\t"); Position::display();
        }
};

class Sector: public Employee { //Person - head of sector
        char* _sectorname = 0;
        //head of sector placed in base class Employee
    public:
        Employee _elist[MAX_EMPLOYEES-1];
        Sector() {}
        Sector(const char* sectnm, Employee& head) {
            sectorname(sectnm);
            Employee* tmphead = (Employee*) this;
            *tmphead = head;
        }
        Sector(const Sector& sect) {
            sectorname(sect._sectorname);
            Employee* tmphead = (Employee*) this;
            *tmphead = (Employee) sect;
            for (int i=0; i<MAX_EMPLOYEES-1; i++) _elist[i] = sect._elist[i];
        }
        ~Sector() {
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
            printf("\tEmployees:\n");
            for (int i = 0; i < MAX_EMPLOYEES; i++) {
                printf("\t");
                _elist[i].show();
            }
        }
};

int main(int argc, char** argv) {
    Date dt(2019, 5, 8);
    Date* dt2 = new Date(dt);
    std::cout << "sizeof(Date)" << sizeof(dt) << std::endl;
    std::cout << "sizeof(Date*)" << sizeof(dt2) << std::endl;
    std::cout <<  "sizeof(Date)" << sizeof(*dt2) << std::endl;


    Position pos = Position("CEO", 1000, true);
    std::cout << "sizeof(Position)" << sizeof(pos) << std::endl;
    //std::cout << "sizeof(Position*)" << sizeof(pos) << std::endl;

    Person pers;// = Person(1976, 6, 28);
    std::cout << "sizeof(Person)" << sizeof(pers) << std::endl;
    // delete dt2;

    //Mr. Sherlock Holmes
    //Private detectiive
    Position* holmes = new Position("detective", 2000, false);
    std::cout << "sizeof(Position*)" << sizeof(holmes) << std::endl;
    std::cout << "sizeof(Position)" << sizeof(*holmes) << std::endl;
    cout << holmes->title() << endl;
    holmes->title("consulting detective");
    cout << holmes->title() << endl;
    holmes->display();

    Person* pers1 = new Person();
    Person* sherlock = new Person("Sherlock", "Holmes", 1850, 1, 1);
    Person* pers3 = new Person(*sherlock);
    std::cout << "sizeof(Person*)" << sizeof(sherlock) << std::endl;
    std::cout << "sizeof(Person)" << sizeof(*sherlock) << std::endl;
    pers3->show();
    cout << sherlock->name() << endl;
    cout << sherlock->name("Mr.Sherlock") << endl;
    cout << sherlock->surname() << endl << sherlock->surname("HOLMES") << endl;
    sherlock->show();
    Date* bday = (Date*) sherlock;
    bday->show();
    std::cout << "sizeof(Date/Person*)" << sizeof(bday) << std::endl;
    std::cout << "sizeof(Date/Person)" << sizeof(*bday) << std::endl;

    std::cout << "sizeof(Date)" << sizeof(Date) << std::endl;
    std::cout << "sizeof(Person)" << sizeof(Person) << std::endl;
    std::cout << "sizeof(Position)" << sizeof(Position) << std::endl;
    std::cout << "sizeof(Employee)" << sizeof(Employee) << std::endl;
    std::cout << "sizeof(Sector)" << sizeof(Sector) << std::endl;

    //head of sector
    Employee* head = new Employee(
            *new Person("Artur", "Conan Doyle", 1859, 5, 22), 
            *new Position("author", 1000000, false), 
            *new Date(1891, 01, 01)); //employed
    head->show();

    Sector* bs221b = new Sector("221B Baker Street", *head);
    bs221b->_elist[0] = Employee(
            *new Person("Sherlock", "Holmes", 1850, 1, 1), 
            *new Position("consulting detective", 100000, false), 
            *new Date(1891, 01, 02)); //employed
    bs221b->_elist[1] = Employee(
            *new Person("John", "Watson", 1850, 1, 2), 
            *new Position("assistant", 100000, false), 
            *new Date(1891, 01, 02)); //employed
    bs221b->_elist[2] = Employee(
            *new Person("Martha", "Hudson", 1850, 1, 3), 
            *new Position("landlady", 100000, false), 
            *new Date(1891, 01, 03)); //employed

    bs221b->display();
    delete head;


    getchar();

    // //Dr. John Watson  
    // //detective's assistant
    // Position* pos2 = new Position("assistant", 1000, false);
    // //landlady 
    // //Mrs. Martha Louise Hudson (née Sissons)
    // //221C Baker Street
    // Position* pos3 = new Position("landlady", 1000, false);
    // //Mycroft 
    // Position* pos4 = new Position("muse", 1000, false);
    // //Jim Moriarty
    // //Dr.
    // Position* pos5 = new Position("consulting criminal", 20000, false);
    
    

    // delete holmes;
    

}