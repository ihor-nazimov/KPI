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

#include "classes.h"

using namespace std;

int main(int argc, char const *argv[])
{
    CArchive* archive = new CArchive(200); //archive size limit

    int choice = -1;
    char filename[255];
    const char* def_filename = "kurs.dat";
    const char* def_txtfilename = "kurs.txt";
    ofstream ofile;
    ifstream ifile;
    const char* mainMenu = 
        "1. Enter new archive entry\n"
        "2. Display archive\n" 
        "3. Load from binary\n"
        "4. Save as binary\n"
        "5. Save as text\n"
        "0. Exit \n>";

    do {
        switch (choice = CBase::getChoice(mainMenu)) {
            case 1: 
            // "1. Enter new archive entry\n"
                archive->push_back((new CArchiveEntry())->input(cin, cout, "Enter new archive entry:\n"));
                break;
            case 2:
            // "2. Display archive\n" 
                std::cout << endl << CArchive::header << *archive << endl;
                break;
            case 3:
            // "3. Load from binary\n"
                CBase::getFilename(filename, def_filename, sizeof(filename));
                ifile.open(filename, ios_base::in | ios_base::binary);
                archive->load(ifile);
                ifile.close();
                std::cout << "Archive was loaded from " << filename << endl << endl;
                break;
            case 4:
            // "4. Save as binary\n"
                CBase::getFilename(filename, def_filename, sizeof(filename));
                ofile.open(filename, ios_base::out | ios_base::binary);
                archive->save(ofile);
                ofile.close();
                std::cout << "Archive was saved as binary into " << filename << endl << endl;
                break;
            case 5:
            // "5. Save as text\n"
                CBase::getFilename(filename, def_txtfilename, sizeof(filename));
                ofile.open(filename, ios_base::out);
                ofile << *archive;
                ofile.close();
                std::cout << "Archive was saved as text into " << filename << endl << endl;
                break;
            case 0:
            // "0. Exit \n>";
                std::cout << "Bye!\n" << endl;
                break;
            default:
                std::cout << "Incorrect choice" << endl;
        }

    }
    while (choice != 0);

    return 0;
}

