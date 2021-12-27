#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
using namespace std;

mutex mtx;

ifstream in("input.txt");               //открываем файл для чтения
ofstream out("output.txt");             //открываем файл для записи

void read(vector <string> &v){
    string words;
    unique_lock<mutex> block(mtx);                       //блокируем всё остальное, пока не выполним следующие две строчки
    in >> words;
    v.push_back(words);
}

void number(vector <string>& p, string word, int& sum, int a, int b){            //счетчик
    unique_lock<mutex> block(mtx);
    for (int i = a; i < b; i++)
        if (p[i] == word) {
            sum++;
        }
}

int main(){
    int n = 0, k = 0, l = 0, i = 0, sum = 0, size = 0, size1 = 0;
    string c;

    in >> n;                    //количество создаваемых потоков
    in >> c;                    //искомое слово

    if (in.eof()){               //Проверка на пустоту
        out << 0;
        in.close();
        out.close();
        return 0;
    }

    vector<string> v;           //Вектор всех слов из файла
    vector<thread> a;           //Пустой вектор потоков для чтения
    vector<thread> b;           //Пустой вектор потоков для подсчёта количества вхождений

    while (!in.eof()){
        for (i = 0; i < n; i++)   //Заполняеи вектор
            a.push_back(thread(read, ref(v)));
    }

    for (i = 0; i < a.size(); i++)      //Закрываем вектор потоков чтения
        a[i].join();

    size = v.size();
    size1 = size / n;           //Находим средний интервал

    for (i = 0; i < n; i++){
        k = i * size1;
        l = (i + 1) * size1;
        if (i == (n - 1))
            l = size;               //Делим вектор на интервалы, вычисялем пределы этого интервала
        b.push_back(thread(number, ref(v), c, ref(sum), k, l));     //заполняем вектор потоков для подсчёта
    }


    for (i = 0; i < b.size(); i++)
        b[i].join();                //Закрываем вектор

    out << sum;     //Ответ

    in.close();
    out.close();

    return 0;
}