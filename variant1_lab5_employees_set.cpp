// Лабораторная работа №5. Вариант 1.
// Класс «сотрудник» из лабораторной работы №4 (поля: ФИО, дата приема на работу, должность, базовый оклад).
// Объекты добавляются в контейнер set (сортировка по ФИО, как в ЛР4)
// и в контейнер unordered_set. Исходные данные читаются из input.txt (как в ЛР4),
// объекты контейнеров выводятся в output.txt для контроля.
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <unordered_set>
using namespace std;

class Employee // Класс - сотрудник
{
    string fio;      // ФИО
    string date;     // Дата приема на работу
    string position; // Должность
    double salary;   // Базовый оклад
public:
    Employee() // Конструктор без параметров
    {
        fio = ""; date = ""; position = ""; salary = 0;
    }
    Employee(string fio, string date, string position, double salary) // Конструктор для инициализации полей
    {
        this->fio = fio; this->date = date; this->position = position; this->salary = salary;
    }
    // Конструктор копирования, конструктор перемещения и операции присваивания не нужны:
    // в классе нет указателей и динамической памяти, поэтому подходят стандартные, которые
    // создает компилятор.
    bool operator <(const Employee & c) const // Сравнение по ФИО (используется при упорядочивании в set)
    {
        return fio < c.fio;
    }
    string getKey() const // Ключ (нужен для хеш-функции)
    {
        return fio;
    }
    bool operator ==(const Employee & c) const // Для unordered_set: объекты равны, если равны ключи
    {
        return fio == c.fio;
    }
    friend ostream & operator <<(ostream & out, const Employee & c); // Дружественная функция вставки в поток
};
ostream & operator <<(ostream & out, const Employee & c) // Вставка объекта в поток
{
    out << c.fio << ", hired " << c.date << ", position " << c.position << ", salary " << c.salary;
    return out;
}
struct EmployeeHash // Хеш-функция для unordered_set: хеш от ключа
{
    size_t operator()(const Employee & c) const
    {
        return hash<string>()(c.getKey());
    }
};

int main()
{
    ifstream fin("input.txt"); // Входной файл
    ofstream fout("output.txt"); // Выходной файл
    if (!fin) // Ошибка файл не открыт
    {
        cerr << "Error, input.txt not found" << endl;
        return 1;
    }
    int n; // Число объектов в файле
    fin >> n;
    fin.ignore(2, '\n'); // Пропускаем символы перехода на новую строку
    set<Employee> s; // Упорядоченный контейнер
    unordered_set<Employee, EmployeeHash> u; // Неупорядоченный контейнер
    for (int i = 0; i < n; i++) // Читаем объекты из файла и добавляем в оба контейнера
    {
        string fio, date, position;
        double salary;
        getline(fin, fio); // Строки могут содержать пробелы
        getline(fin, date);
        getline(fin, position);
        fin >> salary; // Оклад
        fin.ignore(2, '\n');
        Employee c(fio, date, position, salary);
        s.insert(c); // Если объект с таким ключом уже есть, второй раз не добавляется
        u.insert(c);
    }
    fout << "Read from file: " << n << " objects" << endl << endl;
    fout << "set (sorted by full name), " << s.size() << " objects:" << endl;
    for (const Employee & c : s) fout << c << endl; // Используем перегруженную операцию <<
    fout << endl << "unordered_set (order is not defined), " << u.size() << " objects:" << endl;
    for (const Employee & c : u) fout << c << endl;
    return 0;
}
