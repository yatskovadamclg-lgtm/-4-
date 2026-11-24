#include <iostream>   // Підключення бібліотеки введення/виведення у консоль
#include <fstream>    // Підключення бібліотеки для роботи з файлами[cite: 71]
#include <cstdlib>    // Підключення функцій rand(), srand(), malloc(), free()[cite: 71]
#include <ctime>      // Підключення функції time() для генератора випадкових чисел[cite: 71]
#include <windows.h>  // Підключення Windows API для налаштування кодування консолі[cite: 71]
using namespace std;  // Використання стандартного простору імен std[cite: 71]

// Перелік можливих кодів помилок класу VectorInt[cite: 71]
enum VecErr { V_OK = 0, V_ERR_MEM = 1, V_ERR_RANGE = 2, V_ERR_DIV0 = 3, V_ERR_DIM = 4 };

// Оголошення класу вектора цілих чисел VectorInt[cite: 36, 71]
class VectorInt {
private:                     // Приватні поля класу[cite: 71]
    int* data;               // Вказівник на динамічний масив цілих чисел[cite: 36, 71]
    int size;                // Кількість елементів у векторі[cite: 36, 71]
    mutable int codeError;   // Змінна стану помилки (mutable для константних методів)[cite: 36, 71]
    static int objCount;     // Статичний лічильник створених об'єктів VectorInt[cite: 37, 71]

public:                      // Публічні методи та перевантажені операції[cite: 71]
    // Конструктор без параметрів (виділяє місце для 1 елемента та обнуляє його)[cite: 36, 71]
    VectorInt() : size(1), codeError(V_OK) {
        data = new (nothrow) int[1]; // Виділення пам'яті для 1 елемента[cite: 36, 71]
        if (!data) { size = 0; codeError = V_ERR_MEM; } // Перевірка виділення пам'яті[cite: 37, 71]
        else data[0] = 0;            // Ініціалізація нулем[cite: 36, 71]
        objCount++;                  // Збільшення лічильника об'єктів[cite: 37, 71]
    }

    // Конструктор з одним параметром — розмір вектора[cite: 36, 71]
    VectorInt(int n) : size(n > 0 ? n : 1), codeError(V_OK) {
        if (n <= 0) codeError = V_ERR_RANGE; // Фіксація помилки при некоректному розмірі[cite: 37, 71]
        data = new (nothrow) int[size];      // Виділення динамічної пам'яті[cite: 36, 71]
        if (!data) { size = 0; codeError = V_ERR_MEM; } // Перевірка пам'яті[cite: 37, 71]
        else for (int i = 0; i < size; i++) data[i] = 0; // Ініціалізація нулями[cite: 36, 71]
        objCount++;                          // Збільшення лічильника об'єктів[cite: 37, 71]
    }

    // Конструктор із двома параметрами — розмір вектора та значення ініціалізації[cite: 36, 71]
    VectorInt(int n, int val) : size(n > 0 ? n : 1), codeError(V_OK) {
        if (n <= 0) codeError = V_ERR_RANGE; // Фіксація помилки розміру[cite: 37, 71]
        data = new (nothrow) int[size];      // Виділення динамічної пам'яті[cite: 36, 71]
        if (!data) { size = 0; codeError = V_ERR_MEM; } // Перевірка пам'яті[cite: 37, 71]
        else for (int i = 0; i < size; i++) data[i] = val; // Заповнення значенням val[cite: 36, 71]
        objCount++;                          // Збільшення лічильника об'єктів[cite: 37, 71]
    }

    // Конструктор копіювання[cite: 36, 71]
    VectorInt(const VectorInt& other) : size(other.size), codeError(other.codeError) {
        data = new (nothrow) int[size];      // Виділення нової ділянки пам'яті[cite: 71]
        if (!data) { size = 0; codeError = V_ERR_MEM; } // Перевірка пам'яті[cite: 37, 71]
        else for (int i = 0; i < size; i++) data[i] = other.data[i]; // Копіювання елементів[cite: 71]
        objCount++;                          // Збільшення лічильника об'єктів[cite: 37, 71]
    }

    // Деструктор класу VectorInt[cite: 36, 71]
    ~VectorInt() {
        delete[] data;                       // Звільнення динамічної пам'яті[cite: 36, 71]
        objCount--;                          // Зменшення лічильника об'єктів[cite: 37, 71]
    }

    int getSize() const { return size; }           // Отримання розміру вектора[cite: 71]
    int getError() const { return codeError; }     // Отримання коду помилки[cite: 71]
    static int getCount() { return objCount; }     // Отримання кількості об'єктів[cite: 37, 71]

    // Перевантаження операторів розподілу пам'яті new та delete[cite: 37, 71]
    void* operator new(size_t sz) {
        void* p = malloc(sz);                // Виділення пам'яті через malloc[cite: 71]
        if (!p) throw bad_alloc();           // Виключення при нестачі пам'яті[cite: 71]
        return p;                            // Повернення вказівника[cite: 71]
    }
    void operator delete(void* p) {
        free(p);                             // Звільнення пам'яті через free[cite: 71]
    }

    // Операція індексації [] (при невірному індексі вказує на останній елемент та ставить codeError)[cite: 37, 71]
    int& operator[](int index) {
        if (index < 0 || index >= size) {    // Перевірка виходу за межі масиву[cite: 37, 71]
            codeError = V_ERR_RANGE;         // Встановлення коду помилки діапазону[cite: 37, 71]
            return data[size - 1];           // Повернення останнього елемента масиву[cite: 37, 71]
        }
        return data[index];                  // Повернення елемента за індексом[cite: 37, 71]
    }

    // Перевантаження операції виклику функції () для заповнення або зсуву значень[cite: 37, 71]
    void operator()(int addVal) {
        for (int i = 0; i < size; i++) data[i] += addVal; // Додавання значення до кожного елемента[cite: 71]
    }

    // Унарні префіксні та постфіксні ++ та --[cite: 36, 71]
    VectorInt& operator++() {                // Префіксний інкремент ++v[cite: 36, 71]
        for (int i = 0; i < size; i++) data[i]++; // Збільшення кожного елемента на 1[cite: 36, 71]
        return *this;                        // Повернення зміненого об'єкта[cite: 71]
    }
    VectorInt operator++(int) {              // Постфіксний інкремент v++[cite: 36, 71]
        VectorInt tmp(*this);                // Збереження копії до зміни[cite: 71]
        ++(*this);                           // Виклик префіксного інкременту[cite: 71]
        return tmp;                          // Повернення початкової копії[cite: 71]
    }
    VectorInt& operator--() {                // Префіксний декремент --v[cite: 36, 71]
        for (int i = 0; i < size; i++) data[i]--; // Зменшення кожного елемента на 1[cite: 36, 71]
        return *this;                        // Повернення зміненого об'єкта[cite: 71]
    }
    VectorInt operator--(int) {              // Постфіксний декремент v--[cite: 36, 71]
        VectorInt tmp(*this);                // Збереження копії до зміни[cite: 71]
        --(*this);                           // Виклик префіксного декременту[cite: 71]
        return tmp;                          // Повернення початкової копії[cite: 71]
    }

    // Унарне логічне заперечення ! (true, якщо size != 0, інакше false)[cite: 37, 71]
    bool operator!() const {
        return size != 0;                    // Повернення true при непорожньому розмірі[cite: 37, 71]
    }

    // Унарне побітове заперечення ~[cite: 37, 71]
    VectorInt operator~() const {
        VectorInt res(size);                 // Створення результуючого вектора[cite: 71]
        for (int i = 0; i < size; i++) res.data[i] = ~data[i]; // Побітова інверсія елементів[cite: 37, 71]
        return res;                          // Повернення результату[cite: 71]
    }

    // Унарний арифметичний мінус -[cite: 37, 71]
    VectorInt operator-() const {
        VectorInt res(size);                 // Створення результуючого вектора[cite: 71]
        for (int i = 0; i < size; i++) res.data[i] = -data[i]; // Зміна знаку кожного елемента[cite: 37, 71]
        return res;                          // Повернення результату[cite: 71]
    }

    // Операція простого присвоєння =[cite: 37, 71]
    VectorInt& operator=(const VectorInt& other) {
        if (this != &other) {                // Перевірка на самоприсвоєння[cite: 71]
            delete[] data;                   // Звільнення старої пам'яті[cite: 71]
            size = other.size;               // Копіювання розміру[cite: 71]
            codeError = other.codeError;     // Копіювання стану помилки[cite: 71]
            data = new int[size];            // Виділення нової пам'яті[cite: 71]
            for (int i = 0; i < size; i++) data[i] = other.data[i]; // Копіювання елементів[cite: 37, 71]
        }
        return *this;                        // Повернення посилання на об'єкт[cite: 71]
    }

    // Складені операції присвоєння (+=, -=, *=, /=, %=, |=, ^=, &=)[cite: 37, 71]
    VectorInt& operator+=(const VectorInt& other) {
        int m = (size < other.size) ? size : other.size; // Вибір мінімального розміру[cite: 71]
        for (int i = 0; i < m; i++) data[i] += other.data[i]; // Додавання елементів[cite: 37, 71]
        return *this;
    }
    VectorInt& operator-=(const VectorInt& other) {
        int m = (size < other.size) ? size : other.size;
        for (int i = 0; i < m; i++) data[i] -= other.data[i]; // Віднімання елементів[cite: 37, 71]
        return *this;
    }
    VectorInt& operator*=(int scalar) {
        for (int i = 0; i < size; i++) data[i] *= scalar; // Множення на скаляр int[cite: 37, 71]
        return *this;
    }
    VectorInt& operator/=(int scalar) {
        if (scalar == 0) { codeError = V_ERR_DIV0; return *this; } // Перевірка ділення на 0[cite: 71]
        for (int i = 0; i < size; i++) data[i] /= scalar; // Ділення на скаляр int[cite: 37, 71]
        return *this;
    }
    VectorInt& operator%=(int scalar) {
        if (scalar == 0) { codeError = V_ERR_DIV0; return *this; } // Перевірка ділення на 0[cite: 71]
        for (int i = 0; i < size; i++) data[i] %= scalar; // Остача від ділення на int[cite: 37, 71]
        return *this;
    }
    VectorInt& operator|=(const VectorInt& other) {
        int m = (size < other.size) ? size : other.size;
        for (int i = 0; i < m; i++) data[i] |= other.data[i]; // Побітове АБО[cite: 37, 71]
        return *this;
    }
    VectorInt& operator^=(const VectorInt& other) {
        int m = (size < other.size) ? size : other.size;
        for (int i = 0; i < m; i++) data[i] ^= other.data[i]; // Побітове виключне АБО[cite: 37, 71]
        return *this;
    }
    VectorInt& operator&=(const VectorInt& other) {
        int m = (size < other.size) ? size : other.size;
        for (int i = 0; i < m; i++) data[i] &= other.data[i]; // Побітове І[cite: 37, 71]
        return *this;
    }

    // Бінарні арифметичні та побітові операції (+, -, *, /, %, |, ^, &)[cite: 37, 71]
    VectorInt operator+(const VectorInt& other) const { VectorInt r(*this); r += other; return r; }
    VectorInt operator-(const VectorInt& other) const { VectorInt r(*this); r -= other; return r; }
    VectorInt operator*(int scalar) const { VectorInt r(*this); r *= scalar; return r; }
    VectorInt operator/(int scalar) const { VectorInt r(*this); r /= scalar; return r; }
    VectorInt operator%(int scalar) const { VectorInt r(*this); r %= scalar; return r; }
    VectorInt operator|(const VectorInt& other) const { VectorInt r(*this); r |= other; return r; }
    VectorInt operator^(const VectorInt& other) const { VectorInt r(*this); r ^= other; return r; }
    VectorInt operator&(const VectorInt& other) const { VectorInt r(*this); r &= other; return r; }

    // Операції порівняння (==, !=, >, >=, <, <=)[cite: 37, 71]
    bool operator==(const VectorInt& other) const {
        if (size != other.size) return false;
        for (int i = 0; i < size; i++) if (data[i] != other.data[i]) return false;
        return true;
    }
    bool operator!=(const VectorInt& other) const { return !(*this == other); }
    bool operator>(const VectorInt& other) const {
        int m = (size < other.size) ? size : other.size;
        for (int i = 0; i < m; i++) if (data[i] <= other.data[i]) return false;
        return true;
    }
    bool operator>=(const VectorInt& other) const {
        int m = (size < other.size) ? size : other.size;
        for (int i = 0; i < m; i++) if (data[i] < other.data[i]) return false;
        return true;
    }
    bool operator<(const VectorInt& other) const {
        int m = (size < other.size) ? size : other.size;
        for (int i = 0; i < m; i++) if (data[i] >= other.data[i]) return false;
        return true;
    }
    bool operator<=(const VectorInt& other) const {
        int m = (size < other.size) ? size : other.size;
        for (int i = 0; i < m; i++) if (data[i] > other.data[i]) return false;
        return true;
    }

    // Дружні функції введення (>>) та виведення (<<) у потік[cite: 37, 71]
    friend istream& operator>>(istream& is, VectorInt& v) {
        for (int i = 0; i < v.size; i++) is >> v.data[i]; // Зчитування всіх елементів[cite: 71]
        return is;                                         // Повернення потоку вводу[cite: 71]
    }
    friend ostream& operator<<(ostream& os, const VectorInt& v) {
        os << "[ ";                                        // Початок форматування вектора[cite: 71]
        for (int i = 0; i < v.size; i++) os << v.data[i] << " "; // Виведення елементів[cite: 71]
        os << "] (err=" << v.codeError << ")";             // Виведення стану помилки[cite: 71]
        return os;                                         // Повернення потоку виводу[cite: 71]
    }
};

// Ініціалізація статичного лічильника об'єктів[cite: 71]
int VectorInt::objCount = 0;

int main() {
    SetConsoleOutputCP(65001);               // Налаштування кодування консолі[cite: 71]
    SetConsoleCP(65001);                     // Налаштування кодування вводу[cite: 71]
    srand(static_cast<unsigned int>(time(nullptr))); // Ініціалізація генератора випадкових чисел[cite: 36, 71]

    cout << "=== LABA 4: ZAVDANNYA 1 (Zadacha 1.1 - VectorInt) ===" << endl;

    // 1. Введення вектора A (розмір 3) з клавіатури[cite: 36, 71]
    VectorInt vA(3);                         // Створення вектора A розміром 3[cite: 71]
    cout << "Vvedit 3 tsili chysla dlya vektora A z klaviatury: ";
    cin >> vA;                               // Використання перевантаженого оператора >>[cite: 37, 71]

    // 2. Запис та зчитування вектора B (розмір 3) з файлу[cite: 36, 71]
    ofstream fout("vec4_input.txt");         // Відкриття файлу для запису[cite: 71]
    fout << "10 20 30";                      // Запис тестових значень у файл[cite: 71]
    fout.close();                            // Закриття файлу[cite: 71]

    ifstream fin("vec4_input.txt");          // Відкриття файлу для читання[cite: 71]
    VectorInt vB(3);                         // Створення вектора B розміром 3[cite: 71]
    fin >> vB;                               // Зчитування вектора B з файлу через >>[cite: 37, 71]
    fin.close();                             // Закриття файлу[cite: 71]

    // 3. Генерація випадкового вектора C (розмір 3)[cite: 36, 71]
    VectorInt vC(3);                         // Створення вектора C розміром 3[cite: 71]
    for (int i = 0; i < 3; i++) vC[i] = rand() % 9 + 1; // Заповнення через оператор [][cite: 36, 71]

    cout << "\nVektor A (klaviatura): " << vA << endl;
    cout << "Vektor B (fayl):       " << vB << endl;
    cout << "Vektor C (vypadkovyy): " << vC << endl;
    cout << "Kilkist obyektiv VectorInt: " << VectorInt::getCount() << endl;

    // 4. Обчислення складних векторних виразів (понад 5 перевантажених операцій у виразі!)[cite: 36, 71]
    VectorInt expr1 = (++vA * 2 + vB - (-vC)) / 2; // Вираз 1: ++, *, +, -, унарний -, / (6 операцій)[cite: 71]
    VectorInt expr2 = ((vB % 7) | vC) ^ (vA & vB); // Вираз 2: %, |, ^, & (4 операції)[cite: 71]
    vC(5);                                         // Виклик оператора () — додавання 5 до елементів vC[cite: 37, 71]

    cout << "\nVyraz 1 ((++vA * 2 + vB - (-vC)) / 2) = " << expr1 << endl;
    cout << "Vyraz 2 (((vB % 7) | vC) ^ (vA & vB)) = " << expr2 << endl;
    cout << "Vektor C pislya vyklyku vC(5):          " << vC << endl;

    // 5. Перевірка порівнянь, індексації з помилкою та операторів new/delete[cite: 37, 71]
    cout << "\nPorivnyannya vB > vA: " << (vB > vA ? "True" : "False") << endl;
    cout << "Dostup za nevirnym indeksom vA[99]: " << vA[99] << " -> novyy stan: " << vA << endl;

    VectorInt* dynVec = new VectorInt(2, 77); // Перевірка перевантаженого new[cite: 37, 71]
    cout << "Dynamichnyy vektor (new): " << *dynVec << endl;
    delete dynVec;                            // Перевірка перевантаженого delete[cite: 37, 71]

    return 0;                                 // Завершення програми[cite: 71]
}