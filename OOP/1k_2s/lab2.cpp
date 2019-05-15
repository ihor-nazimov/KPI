/* OOP. Lab2
С++11, g++
*/
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <limits>

using namespace std;

const int MAX_ROW = 10;
const int MAX_NAME = 255;

enum MediaTypes {UNKNOWN, NEWSPAPER, MAGAZIN};
typedef unsigned int uint;

class Media {
    private:
        mutable char* _name {0}; //initially null pointer; mutable for char* name(char*)
    public:
        MediaTypes type;
        uint founded_y, founded_m;
        uint periodicity;
        double prepayPrice;
        uint index;

        Media();
        Media(const char* nm, MediaTypes tp=UNKNOWN, uint ind=0);
        Media(const Media &t);
        ~Media();

        char* name(const char* nm=0) const;
        bool operator== (const Media &t);
        Media& operator+ (const Media &t);
        operator double() const;
        const Media& operator= (const Media &t);
        friend std::ostream& operator<< (std::ostream &out, const Media &md);
        friend std::istream& operator>> (std::istream &in, Media &md);
};

const char* headLine1 = "    |               |     |          |Founded|Periodicity|Prepay\n";
const char* headLine2 = " #  |      Name     |Index|  Type    |YYYY.MM|issuse/year|price\n";
const char* headLine3 = "----+---------------+-----+----------+-------+-----------+-----------\n";

int main(int argc, char const *argv[])
{
    Media* meds[MAX_ROW];

    for (int i=0; i <= MAX_ROW; i++) meds[i] = 0; //set all rows pointers to null 

    meds[0] = new Media(); //default constructor
    meds[0]->type = NEWSPAPER;
    meds[0]->name("NewsZero");
    meds[0]->index = 10000;

    meds[1] = new Media("MagazinOne", MAGAZIN, 11111); //constructor by parameters
    meds[1]->periodicity = 12;
    meds[1]->prepayPrice = 02345;

    meds[2] = new Media(*meds[0]); //constructror by copy
    meds[2]->name("NewsTwo");
    meds[2]->index += 12222;

    meds[3] = new Media("TestThree");
    *meds[3] = *meds[1]; //overloaded = makes new copy
    //overloaded compare operator
    cout << "( meds[3] == meds[1] ): " << (*meds[3] == *meds[1]) << endl;
    meds[3]->index = 33333;
    meds[3]->name("MagazinThree");
    cout << "After some changes\n( meds[3] == meds[1] ): " << (*meds[3] == *meds[2]) << endl;
    
    //overloaded + increment prepayPrice and periodicity
    meds[4] = new Media(*meds[3]);
    meds[4] = &((*meds[4]) + (*meds[1]));
    meds[4]->type = NEWSPAPER; 
    meds[4]->index = 44444;
    meds[4]->name("NewsFour");

    Media statmed = Media();
    statmed.name("StaticMediaFive");
    meds[5] = &statmed;
    meds[5]->index = 55555; 

    //menu
    int mode=1;
    do {
        //print menu
        switch (mode)
        {
            case 1: //print table
                cout << headLine1 << headLine2 << headLine3;
                for (int i = 0; i<=MAX_ROW; i++) 
                    if (meds[i]) cout << setw(4) << setprecision (4) << right << i << "|" <<
                                *meds[i] << endl; //overloaded out operator <<
                break;
            case 2: //add new row
            {
                int i;
                for (i = 0; i<=MAX_ROW; i++) 
                    if (!meds[i]) break;
                if (meds[i]) {
                    cerr << "\nCan't add row: table is full" << endl;
                    break;
                }
                meds[i] = new Media();
                cin >> *meds[i]; //overloaded in operator >>
                if (cin.fail()) {
                    delete meds[i];
                    meds[i] = 0;
                    cerr << "\nCan't add row: Incorrect input" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //free input buffer
                }
                break;
            }
            case 3: //copy row 
            {
                int srcRow=-1;
                int i;
                do {
                    cout << "Enter source row ";
                    cin >> srcRow;
                } while (  !(srcRow>=0 && srcRow<=MAX_ROW && meds[srcRow]) );
                for (i = 0; i<=MAX_ROW; i++) 
                    if (!meds[i]) break;
                if (!meds[i]) {
                    meds[i] = new Media(*meds[srcRow]);
                    cout << "Row was copied into #" << i << endl;
                } else {
                    cerr << "\nCan't copy row: Table is full";
                }
                break;
            }
            case 4: //change row
                cout << "Row change hasn't implemented yet\n";
                break;
            case 5: //delete row
            {
                int rowToDelete=-1;
                do {
                    cout << "Enter row number to delete ";
                    cin >> rowToDelete;
                } while (!(rowToDelete>=0 && rowToDelete<=MAX_ROW && meds[rowToDelete]));
                cout << "Delete row #" << rowToDelete << "? (Y/N)";
                char answer=0; 
                cin >> answer;
                if (answer == 'Y' || answer == 'y') {
                    delete meds[rowToDelete];
                    meds[rowToDelete] = 0;
                    cout << "Row #" << rowToDelete << " was deleted\n" << endl;
                } else 
                    cout << "Table was left unchanged\n" << endl;
                break;
            }
            case 0: //exit
                cout << "Exiting...\n";
                break;
            default:
                cerr << "Incorrect mode! Please try again\n";
                break;
        }
        //cout << "Select mode:\n1.Show table\n2.Add row\n3.Copy row\n4.Change row\n5.Delete row\n0.Exit" << endl;
        cout << "\nSelect mode:\n1.Show table\n2.Add row\n3.Copy row\n4.Change row\n5.Delete row\n0.Exit\n>";
        mode = -1;
        do {
            cin >> mode;
            if (cin.fail()) {
                cin.clear();
                mode = -1;
                cout << "Incorrect input" << endl;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //free input buffer
        } while (mode == -1);
    } while (mode); 

    for (int i = 0; i<=5; i++) {delete meds[i];};

    return 0;
    
}


//implementation
Media::Media(): type(UNKNOWN), founded_y(1900), founded_m(0), 
        periodicity(0), prepayPrice(0.0), index(0) {}

Media::Media(const char* nm, MediaTypes tp, uint ind): type(tp), index(ind),
        founded_y(1900), founded_m(0), periodicity(0), prepayPrice(0.0) {
    name(nm);
}

Media::Media(const Media &t): founded_y(t.founded_y), founded_m(t.founded_m), 
        periodicity(t.periodicity), prepayPrice(t.prepayPrice) {
    name(t.name());
    type = t.type;
    index = t.index;
    //*this = t; //alternative: copy pointer despite of copy values 
}

Media::~Media() { 
    if (_name) delete[] _name; 
}

char* Media::name(const char* nm) const { 
    if (nm) {
        if (_name) 
            delete[] _name;
        _name = new char[strlen(nm)+1]; //length+1 bytes for correct delete[]
        strcpy(_name, nm);
    }
    return _name;
};

bool Media::operator== (const Media &t){
    return strcmp(_name, t.name())==0 && type==t.type && founded_y==t.founded_y && founded_m==t.founded_m &&
            periodicity==t.periodicity && prepayPrice==t.prepayPrice && index==t.index;
}

Media& Media::operator+ (const Media &t){
    //Media* result = *this; //constructor by copy
    prepayPrice += t.prepayPrice;
    periodicity += t.periodicity; 
    return *this;
}

Media::operator double() const{
    return prepayPrice;
}

const Media& Media::operator= (const Media &t) {
    if (!this) {
        return t;
    }
    if ( *this == t ) {  //overloaded operator==
        printf("The same!\n");
        return *this;
    } else
    {
        name(t.name()); 
        type = t.type;
        index = t.index;
        founded_y = t.founded_y; 
        founded_m = t.founded_m; 
        periodicity = t.periodicity; 
        prepayPrice = t.prepayPrice;
        return *this;
    } 
}

std::ostream& operator<< (std::ostream &out, const Media &md) {
    out.width(15);    out.precision(0);
    out << left << md.name() << "|" << md.index << "|" << 
    setprecision(10) << setw(10); 
    switch (md.type) {
        case MAGAZIN:   out << "magazin"; break;
        case NEWSPAPER: out << "newspaper"; break;
        default:        out << "unknown";
    } 
    out << "|" << right << setw(4) << md.founded_y << "." << setw(2) << md.founded_m << "|" << 
    fixed << setw(11) << setprecision(0) << md.periodicity << "|" << 
    fixed << setw(8) << setprecision(2) << (double)md; //overloaded conversion to double
    return out;
};

std::istream& operator>> (std::istream &in, Media &md) {
    cout << "Enter name: ";
    char* strbuf = new char[MAX_NAME];
    //in >> strbuf; //gets first word only
    //in.getline(strbuf, MAX_NAME, '\n'); //incorrect
    in.get(strbuf, MAX_NAME, '\n'); //gets line
    //cin >> setw(MAX_NAME) >> setprecision(MAX_NAME) >> strbuf;
    //cin.rdbuf();
    md.name(strbuf);
    //in.ignore(numeric_limits<streamsize>::max());
    delete[] strbuf;
    
    int medType = -1;
    do {
        cout << "Enter media type (0=UNKNOWN, 1=NEWSPAPER, 2=MAGAZIN): ";
        in >> medType;
    } while (medType < 0 || medType > 2);
    md.type = (MediaTypes) medType;

    cout << "Enter index (00000): ";
    in >> md.index;
    cout << "Enter year and month of foundation (yyyy mm): ";
    in >> md.founded_y >> md.founded_m;
    cout << "Enter periodicity: ";
    in >> md.periodicity;
    cout << "Enter subscription price: ";
    in >> md.prepayPrice;
};
