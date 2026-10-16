// Лабораторная работа №1, часть 2, вариант 1.
// Подсчет студентов, имеющих оценку 2 хотя бы по одному предмету.
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

const int MIN_DISCIPLINES = 4;

struct Discipline
{
    std::string name;
    int mark;
};

struct Student
{
    std::string fio;
    std::vector<Discipline> disciplines;
};

// getline сохраняет пробелы в ФИО и названиях предметов.
std::string readText(const std::string& prompt)
{
    std::string text;
    for (;;)
    {
        std::cout << prompt;
        if (!std::getline(std::cin, text))
            throw std::runtime_error("Input ended before all data was entered.");
        if (text.find_first_not_of(" \t\r\n") != std::string::npos)
            return text;
        std::cout << "Enter a non-empty value.\n";
    }
}

// Читаем целую строку, чтобы ошибочный ввод не блокировал поток cin.
int readInteger(const std::string& prompt, int minimum,
                int maximum = std::numeric_limits<int>::max())
{
    for (;;)
    {
        std::istringstream input(readText(prompt));
        int value;
        char extra;
        if ((input >> value) && !(input >> extra)
            && value >= minimum && value <= maximum)
            return value;
        std::cout << "Enter an integer from " << minimum
                  << " to " << maximum << ".\n";
    }
}

// Отдельная функция ввода одной структурной переменной «студент».
void inputStudent(Student& student)
{
    student.fio = readText("FIO: ");
    const int count = readInteger("Number of disciplines (>= 4): ",
                                  MIN_DISCIPLINES);
    student.disciplines.resize(count);
    for (int i = 0; i < count; ++i)
    {
        std::cout << "Discipline " << i + 1 << ":\n";
        student.disciplines[i].name = readText("  Name: ");
        student.disciplines[i].mark = readInteger("  Mark (2..5): ", 2, 5);
    }
}

// Отдельная функция вывода одной структурной переменной «студент».
void printStudent(const Student& student)
{
    std::cout << student.fio << '\n';
    for (const Discipline& discipline : student.disciplines)
        std::cout << "  " << discipline.name << ": " << discipline.mark << '\n';
}

std::size_t countFailed(const std::vector<Student>& group)
{
    std::size_t count = 0;
    for (const Student& student : group)
    {
        for (const Discipline& discipline : student.disciplines)
        {
            if (discipline.mark == 2)
            {
                ++count;
                break; // Несколько двоек у студента не увеличивают счет повторно.
            }
        }
    }
    return count;
}

int main()
{
    try
    {
        const int n = readInteger("Number of students: ", 1);
        std::vector<Student> group(n);
        for (int i = 0; i < n; ++i)
        {
            std::cout << "\nStudent " << i + 1 << ":\n";
            inputStudent(group[i]);
        }

        // Контроль всего ввода выполняется до вывода результата задачи.
        std::cout << "\nEntered data:\nNumber of students: " << n << '\n';
        for (int i = 0; i < n; ++i)
        {
            std::cout << "\nStudent " << i + 1
                      << " (disciplines: " << group[i].disciplines.size() << "):\n";
            printStudent(group[i]);
        }

        std::cout << "\nStudents with at least one unsatisfactory mark (2): "
                  << countFailed(group) << '\n';
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
