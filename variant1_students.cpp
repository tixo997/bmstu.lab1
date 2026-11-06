// Лабораторная работа №2. Вариант 1.
// Структура «студент» (поля: ФИО, группа, номер зачетной книжки, массив 4-х оценок за сессию).
// Создается массив (вектор) студентов размерности n. Сохраняется в файле и читается из файла
// в текстовом или двоичном режимах.
// Режимы работы задаются параметрами командной строки: write или read, затем text или
// binary, затем имя файла. Например:
//   students write text MyFile.txt    - запись, файл в текстовом режиме
//   students read binary MyFile.dat   - чтение, файл в двоичном режиме
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
struct STUDENT // Структура - студент
{
string fio; // ФИО
string group; // Группа
string book; // Номер зачетной книжки
int marks[4]; // Оценки за сессию (массив из 4 элементов)
};
// Печатаем массив студентов
void print_students(ostream & out, vector<STUDENT> & Students)
{
for (auto & pos : Students)
{
out << "Student: " << pos.fio << ", group " << pos.group << ", book " << pos.book << ", marks:";
for (int i = 0; i < 4; i++) out << ' ' << pos.marks[i];
out << endl;
}
}
// Запись одного студента в файл в текстовом режиме
void write_text(ostream & out, STUDENT & c)
{
out << c.fio << endl << c.group << endl << c.book << endl; // Каждая строка с новой строки
out << c.marks[0] << ' ' << c.marks[1] << ' ' << c.marks[2] << ' ' << c.marks[3] << endl; // Оценки в одну строку через пробелы
}
// Запись одного студента в файл в двоичном режиме
void write_binary(ostream & out, STUDENT & c)
{
out << c.fio << ends << c.group << ends << c.book << ends; // После строк вставляем 0, как признак конца
out.write((char *)c.marks, sizeof(c.marks)); // Массив оценок записывается целиком, по байтам
}
// Чтение одного студента из файла в текстовом режиме
void read_text(istream & in, STUDENT & c)
{
in.ignore(2, '\n'); // Пропускаем символы перехода на новую строку
getline(in, c.fio);
getline(in, c.group);
getline(in, c.book);
for (int i = 0; i < 4; i++) in >> c.marks[i];
}
// Чтение одного студента из файла в двоичном режиме
void read_binary(istream & in, STUDENT & c)
{
getline(in, c.fio, '\0'); // Символ '\0' читается из потока
getline(in, c.group, '\0');
getline(in, c.book, '\0');
in.read((char *)c.marks, sizeof(c.marks));
}
int main(int argc, const char* argv[]) {
if (argc != 4) {
cerr << "Error: Use three parameters: write|read text|binary file_name\n";
return 5;
}
const string mode(argv[1]); // Режим работы (запись или чтение)
const string open_mode(argv[2]); // Режим открытия файла (текстовый или двоичный)
const string file_name(argv[3]); // Имя файла
int n = 0; // Переменная, задающая число элементов массива
vector<STUDENT> Students; // Массив структур
if (mode == "write") { // Запись в файл
// Ввод данных с клавиатуры
cout << "n="; cin >> n; // Ввод числа студентов
Students.resize(n); // Задаем размер массива (вектора)
for (int i = 0; i < n; i++) // Цикл ввода данных о студентах с клавиатуры
{
cout << "Student N=" << (i + 1) << endl;
cin.ignore(); // Пропускаем символ перехода на новую строку (после n или после оценок)
cout << "FIO: ";
getline(cin, Students[i].fio);
cout << "Group: ";
getline(cin, Students[i].group);
cout << "Record book number: ";
getline(cin, Students[i].book);
for (int j = 0; j < 4; j++)
{
cout << "Mark " << (j + 1) << ": "; cin >> Students[i].marks[j];
}
}
// Выводим введенные данные для контроля ввода
cout << endl << "Entered data:" << endl;
print_students(cout, Students);
if (open_mode == "text")
{
// Работа с файлом в текстовом режиме
ofstream foutt(file_name); // Открываем файл для записи в текстовом режиме
if (!foutt) // Ошибка файл не открыт
{
cerr << "Error, cannot open file" << endl;
return 1;
}
foutt << n << endl; // Записываем n в текстовом режиме, далее переход на новую строку
for (auto & pos : Students) write_text(foutt, pos);
foutt.close(); // Закрываем файл
}
else
if (open_mode == "binary")
{
// Работа с файлом в двоичном режиме
ofstream foutb(file_name, ios::binary); // Открываем файл для записи в двоичном режиме
if (!foutb) // Ошибка файл не открыт
{
cerr << "Error, cannot open file" << endl;
return 1;
}
foutb.write((char *)&n, sizeof(n)); // Записываем n в двоичном режиме
for (auto & pos : Students) write_binary(foutb, pos);
foutb.close(); // Закрываем файл
}
else {
cerr << "Error: second parameter must be text or binary\n";
return 5;
}
}
else if (mode == "read") {
// Чтение из файла
if (open_mode == "text")
{
ifstream fint(file_name); // Открываем файл для чтения в текстовом режиме
if (!fint) // Ошибка файл не открыт
{
cout << endl << "Error, file not found" << endl;
return 1;
}
fint.seekg(0, ios::end); // Переходим в конец файла, чтобы узнать его размер
long size = (long)fint.tellg(); // Размер файла в байтах
fint.seekg(0); // Возвращаемся в начало файла
fint >> n; // Читаем n в текстовом режиме
if (!fint || n < 0 || n > size) // Не число, отрицательное или больше размера файла
{
cout << endl << "Error, wrong file format" << endl;
return 1;
}
Students.resize(n); // Задаем размер массива
for (auto & pos : Students) read_text(fint, pos);
if (!fint) // Данные в файле закончились раньше времени
{
cout << endl << "Error, wrong file format" << endl;
return 1;
}
fint.close(); // Закрываем файл
}
else
if (open_mode == "binary")
{
ifstream finb(file_name, ios::binary); // Открываем файл для чтения в двоичном режиме
if (!finb) // Ошибка файл не открыт
{
cout << endl << "Error, file not found" << endl;
return 1;
}
finb.seekg(0, ios::end); // Переходим в конец файла, чтобы узнать его размер
long size = (long)finb.tellg(); // Размер файла в байтах
finb.seekg(0); // Возвращаемся в начало файла
finb.read((char *)&n, sizeof(n)); // Читаем n в двоичном режиме
if (!finb || n < 0 || n > size) // Не прочиталось, отрицательное или больше размера файла
{
cout << endl << "Error, wrong file format" << endl;
return 1;
}
Students.resize(n); // Задаем размер массива
for (auto & pos : Students) read_binary(finb, pos);
if (!finb) // Данные в файле закончились раньше времени
{
cout << endl << "Error, wrong file format" << endl;
return 1;
}
finb.close(); // Закрываем файл
}
else {
cerr << "Error: second parameter must be text or binary\n";
return 5;
}
// Печать студентов, прочитанных из файла
print_students(cout, Students);
}
else {
cerr << "Error: first parameter must be write or read\n";
return 5;
}
return 0;
}
