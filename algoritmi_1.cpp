#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

// 1. Обычный линейный поиск (для несортированного массива)
int Better_Linear_Search(int A[], int n, int x, int& counter1, int& counter2) {
    for (int i = 0; i < n; i++) {
        counter2++; // сравнение i < n
        counter1++; // сравнение A[i] == x
        if (A[i] == x) {
            return i;
        }
    }
    counter2++; // последнее сравнение i < n (когда цикл завершается)
    return -1;
}

// 2. Sentinel линейный поиск (для несортированного массива)
int Sentinel_Linear_Search(int A[], int n, int x, int& counter1, int& counter2) {
    // Сохраняем последний элемент
    int last = A[n - 1];

    // Помещаем x в конец массива (страж)
    A[n - 1] = x;

    int i = 0;

    // Ищем элемент (без проверки границ благодаря стражу)
    while (true) {
        counter1++; // сравнение A[i] == x
        if (A[i] == x) {
            break;
        }
        i++;
    }

    // Восстанавливаем исходный последний элемент
    A[n - 1] = last;

    // Проверяем, нашли ли мы реальный элемент или только стража
    counter2++; // сравнение i < n-1
    if (i < n - 1) {
        return i;  // нашли реальный элемент
    }
    else {
        counter1++; // сравнение last == x
        if (last == x) {
            return n - 1;  // элемент был последним
        }
        else {
            return -1;   // элемент не найден
        }
    }
}

// 3. Линейный поиск в отсортированном массиве
int Ordered_Array_Search(int A[], int n, int x, int& counter1, int& counter2) {
    for (int i = 0; i < n; i++) {
        counter2++; // сравнение i < n
        counter1++; // сравнение A[i] == x
        if (A[i] == x) {
            return i;
        }
        counter1++; // сравнение A[i] > x
        if (A[i] > x) {
            return -1; // ранний выход благодаря сортировке
        }
    }
    counter2++; // последнее сравнение i < n
    return -1;
}

// 4. Бинарный поиск в отсортированном массиве
int Binary_Search(int A[], int n, int x, int& counter1, int& counter2) {
    int left = 0;
    int right = n - 1;

    while (left <= right) {
        counter2++; // сравнение left <= right (аналог i < n)

        int mid = left + (right - left) / 2;
        counter1++; // сравнение A[mid] == x
        if (A[mid] == x) {
            return mid;
        }

        counter1++; // сравнение A[mid] < x
        if (A[mid] < x) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return -1;
}

void createTestArray(int size, int massiv[], bool sorted = false) {
    for (int i = 0; i < size; i++) {
        massiv[i] = rand() % (size * 10) + 1; // диапазон зависит от размера
    }
    if (sorted) {
        sort(massiv, massiv + size);
    }
}

void copyArray(int source[], int destination[], int size) {
    for (int i = 0; i < size; i++) {
        destination[i] = source[i];
    }
}

void testAlgorithmForCase(int size, const char* case_name,
    int target_unsorted, int target_sorted,
    int* unsorted_arr, int* sorted_arr)
{
    cout << "\n--- " << case_name << " -------" << endl;
    cout << "Целевое значение (несорт.): " << target_unsorted << endl;
    cout << "Целевое значение (сорт.): " << target_sorted << endl;

    int* temp_arr = new int[size];
    int counters1[4] = { 0 }; // для сравнений ключа с элементами
    int counters2[4] = { 0 }; // для сравнений i < n
    long long times[4] = { 0 };
    int results[4] = { 0 };

    const char* algorithm_names[] = {
        "Better Linear Search",
        "Sentinel Linear Search",
        "Ordered Array Search",
        "Binary Search"
    };

    // Тест 1: Better Linear Search (использует target_unsorted)
    copyArray(unsorted_arr, temp_arr, size);
    auto begin = chrono::steady_clock::now();
    results[0] = Better_Linear_Search(temp_arr, size, target_unsorted, counters1[0], counters2[0]);
    auto end = chrono::steady_clock::now();
    times[0] = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

    // Тест 2: Sentinel Linear Search (использует target_unsorted)
    copyArray(unsorted_arr, temp_arr, size);
    begin = chrono::steady_clock::now();
    results[1] = Sentinel_Linear_Search(temp_arr, size, target_unsorted, counters1[1], counters2[1]);
    end = chrono::steady_clock::now();
    times[1] = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

    // Тест 3: Ordered Array Search (использует target_sorted)
    copyArray(sorted_arr, temp_arr, size);
    begin = chrono::steady_clock::now();
    results[2] = Ordered_Array_Search(temp_arr, size, target_sorted, counters1[2], counters2[2]);
    end = chrono::steady_clock::now();
    times[2] = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

    // Тест 4: Binary Search (использует target_sorted)
    copyArray(sorted_arr, temp_arr, size);
    begin = chrono::steady_clock::now();
    results[3] = Binary_Search(temp_arr, size, target_sorted, counters1[3], counters2[3]);
    end = chrono::steady_clock::now();
    times[3] = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

    // Вывод результатов
    cout << "Алгоритм\t\t\tВремя (нС)\tСравн.1\tСравн.2\tРезультат" << endl;
    cout << "-----------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < 4; i++) {
        cout << setw(25) << left << algorithm_names[i];
        cout << "\t" << times[i] << "\t\t";
        cout << counters1[i] << "\t" << counters2[i] << "\t";
        cout << results[i] << endl;
    }

    delete[] temp_arr;
}

void testAlgorithmsForSize(int size) {
    cout << "\n" << string(60, '=') << endl;
    cout << "=== РАЗМЕР МАССИВА: " << size << " ===" << endl;
    cout << string(60, '=') << endl;

    // Создаем массивы для тестирования
    int* unsorted_arr = new int[size];
    int* sorted_arr = new int[size];

    // Создаем несортированный массив
    createTestArray(size, unsorted_arr, false);

    // Создаем отсортированный массив (на основе того же набора данных)
    copyArray(unsorted_arr, sorted_arr, size);
    sort(sorted_arr, sorted_arr + size);

    // Находим максимальный элемент для создания отсутствующего ключа
    int max_element = sorted_arr[size - 1];

    // Три тестовых случая:

    // 1. Ключ в начале массива
    int target_begin_unsorted = unsorted_arr[0]; // Для BLS и SLS - первый элемент несортированного
    int target_begin_sorted = sorted_arr[0];     // Для OAS и BS - первый элемент отсортированного
    testAlgorithmForCase(size, "КЛЮЧ В НАЧАЛЕ МАССИВА",
        target_begin_unsorted, target_begin_sorted,
        unsorted_arr, sorted_arr);

    // 2. Ключ в середине массива  
    int target_middle_unsorted = unsorted_arr[size / 2]; // Для BLS и SLS - средний элемент несортированного
    int target_middle_sorted = sorted_arr[size / 2];     // Для OAS и BS - средний элемент отсортированного
    testAlgorithmForCase(size, "КЛЮЧ В СЕРЕДИНЕ МАССИВА",
        target_middle_unsorted, target_middle_sorted,
        unsorted_arr, sorted_arr);

    // 3. Ключ отсутствует - используем max_element + 1 для всех алгоритмов
    int target_missing = max_element + 1; // Гарантированно отсутствует во всех массивах
    testAlgorithmForCase(size, "КЛЮЧ ОТСУТСТВУЕТ",
        target_missing, target_missing,
        unsorted_arr, sorted_arr);

    delete[] unsorted_arr;
    delete[] sorted_arr;
}

void comprehensiveTest() {
    cout << "=== СРАВНИТЕЛЬНЫЙ АНАЛИЗ АЛГОРИТМОВ ПОИСКА ===" << endl;
    cout << "Диапазон размеров: 50000 - 300000 элементов" << endl;
    cout << "Для каждой размерности тестируются 3 случая:" << endl;
    cout << "1. Ключ в начале массива" << endl;
    cout << "2. Ключ в середине массива" << endl;
    cout << "3. Ключ отсутствует" << endl << endl;

    // Тестируем 6 разных размеров
    int sizes[] = { 50000, 80000, 120000, 180000, 240000, 300000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num_sizes; i++) {
        testAlgorithmsForSize(sizes[i]);
    }
}

void generateSummaryTable() {
    cout << "\n" << string(80, '=') << endl;
    cout << "СВОДНАЯ ТАБЛИЦА ПРОИЗВОДИТЕЛЬНОСТИ (время в наносекундах)" << endl;
    cout << string(80, '=') << endl;

    int sizes[] = { 50000, 80000, 120000, 180000, 240000, 300000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    const char* cases[] = { "Начало", "Середина", "Отсутствует" };

    for (int case_idx = 0; case_idx < 3; case_idx++) {
        cout << "\n--- Случай: " << cases[case_idx] << " ---" << endl;
        cout << "Размер\t\tBLS\tSentinel\tOrdered\tBinary" << endl;
        cout << "--------------------------------------------------------------" << endl;

        for (int size_idx = 0; size_idx < num_sizes; size_idx++) {
            int size = sizes[size_idx];

            // Создаем тестовые массивы
            int* unsorted_arr = new int[size];
            int* sorted_arr = new int[size];
            int* temp_arr = new int[size];

            createTestArray(size, unsorted_arr, false);
            copyArray(unsorted_arr, sorted_arr, size);
            sort(sorted_arr, sorted_arr + size);

            // Находим максимальный элемент для создания отсутствующего ключа
            int max_element = sorted_arr[size - 1];

            // Определяем целевые значения для каждого случая
            int target_unsorted, target_sorted;
            switch (case_idx) {
            case 0: // Начало
                target_unsorted = unsorted_arr[0];
                target_sorted = sorted_arr[0];
                break;
            case 1: // Середина
                target_unsorted = unsorted_arr[size / 2];
                target_sorted = sorted_arr[size / 2];
                break;
            case 2: // Отсутствует
                target_unsorted = max_element + 1;
                target_sorted = max_element + 1;
                break;
            }

            long long times[4] = { 0 };
            int counter1_dummy, counter2_dummy;

            // Измеряем время для каждого алгоритма
            copyArray(unsorted_arr, temp_arr, size);
            auto begin = chrono::steady_clock::now();
            Better_Linear_Search(temp_arr, size, target_unsorted, counter1_dummy, counter2_dummy);
            auto end = chrono::steady_clock::now();
            times[0] = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

            copyArray(unsorted_arr, temp_arr, size);
            begin = chrono::steady_clock::now();
            Sentinel_Linear_Search(temp_arr, size, target_unsorted, counter1_dummy, counter2_dummy);
            end = chrono::steady_clock::now();
            times[1] = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

            copyArray(sorted_arr, temp_arr, size);
            begin = chrono::steady_clock::now();
            Ordered_Array_Search(temp_arr, size, target_sorted, counter1_dummy, counter2_dummy);
            end = chrono::steady_clock::now();
            times[2] = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

            copyArray(sorted_arr, temp_arr, size);
            begin = chrono::steady_clock::now();
            Binary_Search(temp_arr, size, target_sorted, counter1_dummy, counter2_dummy);
            end = chrono::steady_clock::now();
            times[3] = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

            // Выводим строку результатов
            cout << size << "\t\t";
            cout << times[0] << "\t";
            cout << times[1] << "\t\t";
            cout << times[2] << "\t";
            cout << times[3] << endl;

            delete[] unsorted_arr;
            delete[] sorted_arr;
            delete[] temp_arr;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    // Детальный тест для одного размера
    testAlgorithmsForSize(100000);

    // Комплексный тест для 6 размеров
    comprehensiveTest();

    // Сводная таблица для быстрого сравнения
    generateSummaryTable();

    return 0;
}
