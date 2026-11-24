#include <iostream>   // Підключення бібліотеки введення/виведення у консоль
#include <fstream>    // Підключення бібліотеки для роботи з файлами[cite: 47]
#include <string>     // Підключення бібліотеки рядків std::string[cite: 47]
#include <cstdlib>    // Підключення функцій rand() та srand()[cite: 47]
#include <ctime>      // Підключення функції time() для генератора випадкових чисел[cite: 47]
#include <windows.h>  // Підключення Windows API для налаштування кодування консолі[cite: 47]
using namespace std;  // Використання стандартного простору імен std[cite: 47]

// Перелік кодів стану асоціативного класу[cite: 47]
enum AssocState { A_OK = 0, A_NOT_FOUND = -1, A_FULL = -2 };

// Асоціативний клас: Назва товару (string) <-> Інвентарний номер на складі (int)[cite: 25, 47]
class WarehouseAssoc {
private:                     // Приватні поля класу[cite: 47]
    string* productNames;    // Динамічний масив назв товарів[cite: 47]
    int* invNumbers;         // Динамічний масив інвентарних номерів[cite: 47]
    int capacity;            // Максимальна місткість складу[cite: 47]
    int count;               // Поточна кількість записів[cite: 47]
    mutable int codeError;   // Змінна стану помилки (mutable для константного пошуку)[cite: 25, 47]

public:                      // Публічні методи та перевантажені операції[cite: 47]
    // Конструктор без параметрів[cite: 47]
    WarehouseAssoc(int maxCap = 30) : capacity(maxCap), count(0), codeError(A_OK) {
        productNames = new string[capacity]; // Виділення пам'яті під назви товарів[cite: 47]
        invNumbers = new int[capacity];      // Виділення пам'яті під інвентарні номери[cite: 47]
    }

    // Конструктор копіювання[cite: 47]
    WarehouseAssoc(const WarehouseAssoc& other)
        : capacity(other.capacity), count(other.count), codeError(other.codeError) {
        productNames = new string[capacity]; // Виділення нової пам'яті під назви[cite: 47]
        invNumbers = new int[capacity];      // Виділення нової пам'яті під номери[cite: 47]
        for (int i = 0; i < count; i++) {    // Поелементне копіювання пар[cite: 47]
            productNames[i] = other.productNames[i];
            invNumbers[i] = other.invNumbers[i];
        }
    }

    // Деструктор звільняє динамічну пам'ять[cite: 47]
    ~WarehouseAssoc() {
        delete[] productNames;               // Звільнення масиву назв товарів[cite: 47]
        delete[] invNumbers;                 // Звільнення масиву інвентарних номерів[cite: 47]
    }

    int getError() const { return codeError; } // Отримання стану помилки[cite: 47]
    void clearError() { codeError = A_OK; }    // Скидання коду помилки[cite: 47]

    // Функція створення/додавання асоціативної пари (Назва товару, Інвентарний номер)[cite: 25, 47]
    void addItem(const string& name, int invNum) {
        for (int i = 0; i < count; i++) {    // Перевірка, чи товар уже існує[cite: 47]
            if (productNames[i] == name) {
                invNumbers[i] = invNum;      // Оновлення інвентарного номера[cite: 47]
                codeError = A_OK;
                return;
            }
        }
        if (count >= capacity) {             // Перевірка переповнення масиву[cite: 47]
            codeError = A_FULL;              // Встановлення помилки переповнення[cite: 47]
            return;
        }
        productNames[count] = name;          // Додавання назви товару[cite: 47]
        invNumbers[count] = invNum;          // Додавання інвентарного номера[cite: 47]
        count++;                             // Збільшення лічильника записів[cite: 47]
        codeError = A_OK;                    // Нормальний стан[cite: 47]
    }

    // Перевантаження операції індексації [] за назвою товару -> повертає інвентарний номер[cite: 25, 47]
    int operator[](const string& name) const {
        for (int i = 0; i < count; i++) {    // Пошук товару за назвою[cite: 47]
            if (productNames[i] == name) {
                codeError = A_OK;            // Товар знайдено[cite: 47]
                return invNumbers[i];        // Повернення інвентарного номера[cite: 47]
            }
        }
        codeError = A_NOT_FOUND;             // Встановлення коду помилки: товар не знайдено[cite: 25, 47]
        return -1;                           // Повернення -1 як ознаки відсутності[cite: 47]
    }

    // Перевантаження операції індексації [] за інвентарним номером -> повертає назву товару[cite: 25, 47]
    string operator[](int invNum) const {
        for (int i = 0; i < count; i++) {    // Пошук товару за інвентарним номером[cite: 47]
            if (invNumbers[i] == invNum) {
                codeError = A_OK;            // Товар знайдено[cite: 47]
                return productNames[i];      // Повернення назви товару[cite: 47]
            }
        }
        codeError = A_NOT_FOUND;             // Встановлення коду помилки: номер не знайдено[cite: 25, 47]
        return "NE_ZNAYDENO";                // Повернення повідомлення про відсутність[cite: 47]
    }

    // Альтернативне звернення через перевантаження операції виклику функції ()[cite: 25, 47]
    // Виводить усі товари з інвентарними номерами у заданому діапазоні [minInv; maxInv][cite: 47]
    void operator()(int minInv, int maxInv) const {
        cout << "Poshuk tovariv z inventarnym nomerom vid " << minInv << " do " << maxInv << ":" << endl;
        bool found = false;                  // Прапорець наявності результатів[cite: 47]
        for (int i = 0; i < count; i++) {    // Перебір усіх записів складу[cite: 47]
            if (invNumbers[i] >= minInv && invNumbers[i] <= maxInv) {
                cout << "  -> Tovar: " << productNames[i] << " | Inv. nomer: " << invNumbers[i] << endl;
                found = true;                // Зафіксовано знайдений товар[cite: 47]
            }
        }
        if (!found) {                        // Якщо нічого не знайдено в діапазоні[cite: 47]
            codeError = A_NOT_FOUND;         // Встановлення коду помилки[cite: 47]
            cout << "  Tovariv u zadanomu diapazoni nemaye!" << endl;
        }
    }

    // Дружня операція введення >> однієї асоціативної пари з потоку[cite: 25, 47]
    friend istream& operator>>(istream& is, WarehouseAssoc& db) {
        string name;                         // Змінна для назви товару[cite: 47]
        int num;                             // Змінна для інвентарного номера[cite: 47]
        if (is >> name >> num) {             // Зчитування пари з потоку[cite: 47]
            db.addItem(name, num);           // Додавання пари в асоціативний масив[cite: 47]
        }
        return is;                           // Повернення потоку вводу[cite: 47]
    }

    // Дружня операція виведення << усієї таблиці асоціативного класу[cite: 25, 47]
    friend ostream& operator<<(ostream& os, const WarehouseAssoc& db) {
        os << "=== SKLAD (Asotsiatyvna tablytsya tovariv, state=" << db.codeError << ") ===" << endl;
        for (int i = 0; i < db.count; i++) { // Виведення кожної пари ключ-значення[cite: 47]
            os << i + 1 << ". Tovar: " << db.productNames[i]
               << "\t-> Inv. nomer: " << db.invNumbers[i] << endl;
        }
        return os;                           // Повернення потоку виводу[cite: 47]
    }
};

int main() {
    SetConsoleOutputCP(65001);               // Налаштування кодування виводу[cite: 47]
    SetConsoleCP(65001);                     // Налаштування кодування вводу[cite: 47]
    srand(static_cast<unsigned int>(time(nullptr))); // Ініціалізація датчика випадкових чисел[cite: 12, 47]

    cout << "=== LABA 4: ZAVDANNYA 2 (Zadacha 2.11 - Asotsiatyvnyy klas, Variant 11) ===" << endl;

    WarehouseAssoc sklad;                    // Створення об'єкта асоціативного класу[cite: 47]

    // 1. Введення асоціативної пари з клавіатури через дружній оператор >>[cite: 12, 47]
    cout << "Vvedit nazvu tovaru (odnym slovom) ta yogo inventarnyy nomer z klaviatury: ";
    cin >> sklad;                            // Виклик friend operator>>[cite: 47]

    // 2. Запис та зчитування набору товарів з файлу[cite: 12, 47]
    ofstream fout("warehouse.txt");          // Створення текстового файлу[cite: 47]
    fout << "Monitor 1005\nKlaviatura 1012\nMyshka 1025\n"; // Запис 3 товарів[cite: 47]
    fout.close();                            // Закриття файлу[cite: 47]

    ifstream fin("warehouse.txt");           // Відкриття файлу для читання[cite: 12, 47]
    while (fin >> sklad) {}                  // Построкове зчитування пар із файлу через >>[cite: 47]
    fin.close();                             // Закриття файлу[cite: 47]

    // 3. Додавання товарів із випадково згенерованими інвентарними номерами[cite: 12, 47]
    sklad.addItem("Noutbuk", 2000 + rand() % 500);  // Випадковий інвентарний номер[cite: 47]
    sklad.addItem("Prynter", 2000 + rand() % 500);  // Випадковий інвентарний номер[cite: 47]

    cout << endl << sklad << endl;           // Виведення всієї таблиці через friend operator<<[cite: 47]

    // 4. Перевірка пошуку через перевантажену операцію індексації [][cite: 25, 47]
    cout << "Poshuk za nazvoyu sklad[\"Monitor\"]: Inv. nomer = " << sklad["Monitor"]
         << " (codeError=" << sklad.getError() << ")" << endl;

    cout << "Poshuk za nomerom sklad[1012]:      Tovar = " << sklad[1012]
         << " (codeError=" << sklad.getError() << ")" << endl;

    // Перевірка пошуку неіснуючого товару (фіксація помилки codeError)[cite: 25, 47]
    int missing = sklad["Televizor"];        // Спроба знайти відсутній товар[cite: 47]
    cout << "Poshuk vidsutnyogo tovaru sklad[\"Televizor\"]: " << missing
         << " (codeError=" << sklad.getError() << ")" << endl << endl;

    // 5. Альтернативне звернення через перевантажений оператор виклику функції ()[cite: 25, 47]
    sklad(1000, 1030);                       // Виведення товарів з інв. номерами від 1000 до 1030[cite: 47]

    return 0;                                // Завершення роботи програми[cite: 47]
}