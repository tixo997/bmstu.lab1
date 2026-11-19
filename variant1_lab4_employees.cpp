// Лабораторная работа №4. Вариант 1.
// Объект - сотрудник (поля: ФИО, дата приема на работу, должность, базовый оклад).
// Исходный контейнер vector, копируем в deque. Сортировка по ФИО.
// Элементы контейнеров - объекты класса, а не указатели.
// Исходные данные читаются из input.txt, результаты выводятся в output.txt.
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm> // copy, sort
using namespace std;

class Employee // Класс - сотрудник
{
    string fio;      // ФИО
    string date;     // Дата приема на работу
    string position; // Должность
    double salary;   // Базовый оклад
public:
    Employee() // Конструктор без параметров (нужен, потому что контейнер для копирования создается сразу нужного размера)
    {
        fio = ""; date = ""; position = ""; salary = 0;
    }
    Employee(string fio, string date, string position, double salary) // Конструктор для инициализации полей
    {
        this->fio = fio; this->date = date; this->position = position; this->salary = salary;
    }
    // Конструктор копирования, конструктор перемещения и операции присваивания не нужны:
    // в классе нет указателей и динамической памяти, поля копируются и перемещаются
    // корректно стандартными средствами, которые создает компилятор.
    bool operator <(const Employee & c) const // Сравнение по ФИО (используется при сортировке)
    {
        return fio < c.fio;
    }
    friend ostream & operator <<(ostream & out, const Employee & c); // Дружественная функция вставки в поток
};
ostream & operator <<(ostream & out, const Employee & c) // Вставка объекта в поток
{
    out << c.fio << ", hired " << c.date << ", position " << c.position << ", salary " << c.salary;
    return out;
}

int main()
{
    ifstream fin("input.txt"); // Входной файл
    ofstream fout("output.txt"); // Выходной файл
    if (!fin) // Ошибка файл не открыт
    {
        cerr << "Error, input.txt not found" << endl;
        return 1;
    }
    int n; // Число объектов
    fin >> n;
    fin.ignore(2, '\n'); // Пропускаем символы перехода на новую строку
    vector<Employee> items; // Исходный контейнер vector
    for (int i = 0; i < n; i++) // Читаем объекты из файла
    {
        string fio, date, position;
        double salary;
        getline(fin, fio); // Строки могут содержать пробелы
        getline(fin, date);
        getline(fin, position);
        fin >> salary; // Оклад
        fin.ignore(2, '\n');
        items.push_back(Employee(fio, date, position, salary));
    }

    // Копируем исходный контейнер vector в deque алгоритмом copy (до сортировки)
    deque<Employee> copy_items(items.size()); // Нужный размер контейнера задаем заранее
    copy(items.begin(), items.end(), copy_items.begin());

    fout << "Source container (vector):" << endl;
    for (const Employee & c : items) fout << c << endl; // Используем перегруженную операцию <<

    sort(items.begin(), items.end()); // Сортировка алгоритмом sort: итератор на начало и итератор на конец
    fout << endl << "Container after sorting (std::sort by full name):" << endl;
    for (const Employee & c : items) fout << c << endl;

    fout << endl << "Copy of the source container (deque, algorithm copy):" << endl;
    for (const Employee & c : copy_items) fout << c << endl;

    if (n > 0) // Демонстрация копирования объекта (стандартные конструктор копирования и присваивание)
    {
        Employee first = items.front(); // Конструктор копирования
        Employee other;
        other = first; // Присваивание с копированием
        fout << endl << "Copy constructor: " << first << endl;
        fout << "Assignment:       " << other << endl;
    }
    return 0;
}
