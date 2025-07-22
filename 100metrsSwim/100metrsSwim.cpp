#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

// Структура для хранения информации о пловце
struct Swimmer {
    string name;
    double speed;  // скорость в метрах в секунду
    double distance = 0.0;  // пройденное расстояние
    double time = 0.0;     // затраченное время
};

// Функция для обновления позиции пловца
void updateSwimmer(Swimmer& swimmer) {
    if (swimmer.distance < 100.0) {
        swimmer.distance += swimmer.speed;
        swimmer.time += 1.0;
    }
}

// Функция для отображения текущего состояния заплыва
void displayStatus(const vector<Swimmer>& swimmers) {
    system("cls");  // очистка экрана
    cout << "Current race`s status:\n";
    cout << "---------------------------\n";
    for (size_t i = 0; i < swimmers.size(); ++i) {
        cout << "Road " << i + 1 << ": " << swimmers[i].name
            << " - " << fixed << setprecision(2) << swimmers[i].distance
            << " m\n";
    }
    cout << "---------------------------\n";
    this_thread::sleep_for(chrono::seconds(1));  // пауза на 1 секунду
}

// Функция для сортировки результатов
bool compareResults(const Swimmer& a, const Swimmer& b) {
    return a.time < b.time;
}

int main() {
    const int LANES = 6;  // количество дорожек
    vector<Swimmer> swimmers(LANES);

    // Ввод данных о пловцах
    cout << "Input information about a swimmers:\n";
    for (int i = 0; i < LANES; ++i) {
        cout << "Road " << i + 1 << ":\n";
        cout << "Name: ";
        cin >> swimmers[i].name;
        cout << "Speed (m/s): ";
        cin >> swimmers[i].speed;
        cout << endl;
    }

    bool raceFinished = false;

    // Основной цикл симуляции
    while (!raceFinished) {
        displayStatus(swimmers);

        // Обновление позиций всех пловцов
        for (auto& swimmer : swimmers) {
            updateSwimmer(swimmer);
        }

        // Проверка завершения заплыва
        raceFinished = true;
        for (const auto& swimmer : swimmers) {
            if (swimmer.distance < 100.0) {
                raceFinished = false;
                break;
            }
        }
    }

    // Сортировка результатов
    sort(swimmers.begin(), swimmers.end(), compareResults);

    // Вывод итоговых результатов
    cout << "\nFinal results:\n";
    cout << "---------------------\n";
    cout << setw(20) << "Postiton" << setw(20) << "Swimmer"
        << setw(15) << "Time (s)" << endl;
    cout << "----------------------------------------\n";

    for (size_t i = 0; i < swimmers.size(); ++i) {
        cout << setw(20) << i + 1 << setw(20) << swimmers[i].name
            << setw(15) << fixed << setprecision(2) << swimmers[i].time << endl;
    }

    return 0;
}
