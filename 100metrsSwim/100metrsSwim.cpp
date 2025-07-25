#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <chrono>
#include <mutex>
#include <limits>

using namespace std;

// Структура для хранения информации о пловце
struct Swimmer {
    string name;
    double speed; // скорость в метрах в секунду
    double distance = 0.0; // пройденное расстояние
    double time = 0.0; // затраченное время
};

// Глобальный вектор для хранения результатов
vector<Swimmer> results;
mutex resultMutex; // мьютекс для синхронизации доступа к results

// Функция для валидации ввода скорости
double getValidSpeed() {
    double speed;
    while (true) {
        cout << "Speed (m/s): ";
        if (cin >> speed) {
            if (speed > 0) {
                return speed;
            }
            else {
                cout << "Error: the speed must be positive!\n";
            }
        }
        else {
            cout << "Error: enter a number!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Функция для симуляции заплыва одного пловца
void swim(Swimmer swimmer) {
    while (swimmer.distance < 100.0) {
        system("cls");  // очистка экрана
        // Обновление позиции пловца
        swimmer.distance += swimmer.speed;
        swimmer.time += 1.0;

        // Вывод информации о текущем положении
        {
            lock_guard<mutex> lock(resultMutex);
            cout << "Swimmer " << swimmer.name << " swimmed "
                << fixed << setprecision(2) << swimmer.distance
                << " m\n";
        }

        this_thread::sleep_for(chrono::seconds(1));
    }

    // Добавление результата в общий вектор
    {
        lock_guard<mutex> lock(resultMutex);
        results.push_back(swimmer);
    }
}

// Функция сравнения для сортировки по времени
bool compareByTime(const Swimmer& a, const Swimmer& b) {
    return a.time < b.time;
}

int main() {
    const int LANES = 6; // количество дорожек
    vector<Swimmer> swimmers;
    vector<thread> threads;

    // Сбор информации о всех пловцах
    cout << "Enter information about the swimmers:\n";
    for (int i = 0; i < LANES; ++i) {
        Swimmer swimmer;
        cout << "Lane " << i + 1 << ":\n";
        cout << "Name: ";
        cin >> swimmer.name;
        swimmer.speed = getValidSpeed();
        cout << endl;
        swimmers.push_back(swimmer);
    }

    // Запуск симуляции заплыва
    cout << "\nThe swim begins!\n";
    for (const auto& swimmer : swimmers) {
        threads.emplace_back(swim, swimmer);
    }

    // Ожидание завершения всех потоков
    for (auto& th : threads) {
        th.join();
    }

    // Вывод итоговых результатов
    cout << "\nFinal results:\n";
    cout << "---------------------\n";
    cout << setw(20) << "Position" << setw(20) << "Swimmer"
        << setw(15) << "Time (s)" << endl;
    cout << "----------------------------------------\n";

    for (size_t i = 0; i < results.size(); ++i) {
        cout << setw(20) << i + 1 << setw(20) << results[i].name
            << setw(15) << fixed << setprecision(2) << results[i].time << endl;
    }

    return 0;
}