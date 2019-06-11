int main(int argc, char const *argv[])
{
    CDate* dt = new CDate(2000, 12, 1);
    CDate* dt2 = new CDate(*dt);
    dt2->year(1800);
    
    cout << CDate::header << endl << *dt << endl;
    cout << *dt2 << endl;

    


    CPerson* p = new CPerson("Name", "Surname", *dt);
    CPerson* p2 = new CPerson(*p);
    p2->name("Noname");
    
    ofstream ofile;
    ofile.open("kurs.dat", ios_base::out | ios_base::binary);
    ofile << *p;
    ofile.close();

    ifstream ifile;
    ifile.open("kurs.dat", ios_base::in | ios_base::binary);
    CPerson p3;
    ifile >> p3;
    ifile.close();

    cout << "\nCPerson test\n" << CPerson::header << endl << p3 << endl; 


    CFile* f1 = new CFile();
    f1->filename("Testfname");
    f1->date(*dt2);
    f1->size(12345);
    cout << "\nCFile test:\n" << CFile::header << endl << *f1 << endl;
    CFile* f2 = new CFile("abcfile", "json", *dt, 1000123);
    cout << *f2 << endl;
    CFile* f3 = new CFile(*f1);
    f3->extention("ext");
    cout << *f3 << endl;

    ofile.open("kurs.dat", ios_base::out | ios_base::binary);
    ofile << *f3;
    ofile.close();

    ifile.open("kurs.dat", ios_base::in | ios_base::binary);
    CFile* f4 = new CFile();
    ifile >> *f4;
    ifile.close();

    cout << *f4 << endl;

    cout << "\nCTextFile test\n";

    CTextFile* tf1 = new CTextFile();
    tf1->path("tf1_path");
    tf1->author(p2);
    tf1->filename("fname");
    tf1->extention("ext");
    tf1->keywords("a, b, c");

    CTextFile* tf2 = new CTextFile(*tf1);
    tf2->author(p);
    tf2->path("new_path");

    cout << CTextFile::header << endl << *tf1 << endl << *tf2 << endl;

    ofile.open("kurs.dat", ios_base::out | ios_base::binary);
    ofile << *tf2;
    ofile.close();

    ifile.open("kurs.dat", ios_base::in | ios_base::binary);
    CTextFile* tf3 = new CTextFile();
    ifile >> *tf3;
    ifile.close();

    cout << *tf3 << endl;

    tf3->filename("tf3_file");
    tf3->size(123456);

    CArchiveEntry* ae1 = new CArchiveEntry();
    ae1->created(*dt);
    ae1->compress(0.5); 
    ae1->push_back(*tf1);
    ae1->push_back(*tf2);
    ae1->push_back(*tf3);

    CArchiveEntry* ae2 = new CArchiveEntry(*ae1);
    ae2->compress(0.7);
    ae2->push_back(*tf3);

    cout << "\nCArchiveEntry test\n" 
    << CArchiveEntry::header << endl 
    << *ae1 << endl << *ae2 << endl << endl;

    cout << (*ae2)[1] << endl; 

    ofile.open("kurs.dat", ios_base::out | ios_base::binary);
    ofile << *ae2;
    ofile.close();

    ifile.open("kurs.dat", ios_base::in | ios_base::binary);
    CArchiveEntry* ae3 = new CArchiveEntry();
    ifile >> *ae3;
    ifile.close();

    cout << endl << "Reloaded CArchiveEntry" << endl << *ae3 << endl;

    CArchive* a1 = new CArchive(200);
    a1->push_back(*ae1);
    a1->push_back(*ae2);
    a1->push_back(*ae3);

    cout << CArchive::header << endl << *a1 << endl;

    ofile.open("kurs.dat", ios_base::out | ios_base::binary);
    ofile << *a1;
    ofile.close();

    ifile.open("kurs.dat", ios_base::in | ios_base::binary);
    CArchive* a2 = new CArchive();
    ifile >> *a2;
    ifile.close();

    cout << "\nReloaded CArchive\n" << CArchive::header << endl << *a1 << endl;

    // cout << text << CPerson::header << endl << *p << endl;
    // cout << *p2 << endl;
    
    getchar();

}