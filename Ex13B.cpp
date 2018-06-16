/*Написать программу, содержащую следующие функции:
	-Функцию заполнения текстового файла N целыми случайными числами в диапазоне от -M до M. Имя файла, N и M поступают на вход функции, 
		файл возвращается функцией как результат (использовать потоки ввода-вывода). Реализовать два варианта генерации чисел — в цикле
		и с использованием алгоритма std::generate.
	-Функцию, получающую файл на вход, и возвращающую последовательный контейнер, заполненный числами из файла.
	-Функцию modify, выполняющую преобразование полученного контейнера. На вход функции должен поступать контейнер, в качестве результата
		функция должна возвращать модифицированный контейнер.
	-Перегруженную функцию modify, принимающую на вход итераторы начала и конца обрабатываемой части контейнера.
	-Функцию, реализующую требуемое преобразование с использованием алгоритма std::transform вместо функции modify.
	-Функцию, реализующую требуемое преобразование с использованием алгоритма std::for_each вместо функции modify.
	-Функции, вычисляющие сумму и среднее арифметическое чисел, содержащихся в контейнере.
	Функцию вывода результата в файл и на экран (опционально).

	Заменить все положительные числа квадратом минимального числа.

*/

#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <iterator>

using namespace std;

/*Функция заполнения текстового файла filename N целыми случайными числами в диапазоне от -M до M
(генерация в цикле)*/
ofstream CreateFileRandom(string filename, int N, int M)
{
	ofstream fs(filename);
	for (int i = 0; i < N; ++i) {
		int r = rand() % (2 * M + 1) - M;
		fs << r << " ";
	}
	fs.close();
	return fs;
}

/*Функция заполнения текстового файла N целыми случайными числами в диапазоне от -M до M
(генерация через std::generate)*/
ofstream CreateFileGenerate(string filename, int N, int M)
{
	ofstream fs;
	fs.open(filename);

	generate_n(std::ostream_iterator<int>(fs, " "), N, [M]() {int r = rand() % (2 * M + 1) - M; return r; });

	fs.close();
	return fs;
}

//Функция, получающая файл на вход, и возвращающая последовательный контейнер, заполненный числами из файла
deque<int> LoadFromFile(ifstream &in)
{

	deque<int> deq;
	if (!in) {
		cout << "File not found" << endl;
		return deq;
	}

	int a;
	if (!(in >> a)) {	//Считываем первый элемент из потока
		cout << "Invalid data format" << endl;
		return deq;
	}
		
	while (!in.eof()) 
	{
		deq.push_back(a);	//Кладем элемент в дек

		if (!(in >> a) && !in.eof())
		{
			cout << "Invalid data format" << endl;
			deq.clear();
			return deq;
		}
	}
	return deq;
}

//Функция, заменяющая все положительные числа квадратом минимального числа
deque<int> Modify(deque<int> &d)
{
	deque<int> deq = d;
	int min = deq[0];
	int len = deq.size();
	for (int i = 1; i < len; i++) {
		if (deq[i] < min) {
			min = deq[i];
		}
	}
	for (int i = 0; i < len; i++) {
		if (deq[i] > 0) {
			deq[i] = min*min;
		}
	}
	return deq;
}

/*Функция, заменяющая все положительные числа квадратом минимального числа
(принимает на вход итераторы начала и конца обрабатываемой части контейнера)*/
void Modify(deque<int>::iterator begin, deque<int>::iterator end)
{
	int min = *begin;
	for (deque<int>::iterator i = begin; i != end; i++) {
		if (*i < min) min = *i;
	}
	for (deque<int>::iterator i = begin; i != end; i++) {
		if (*i > 0) *i = min*min;
	}
}

/*Функция, заменяющая все положительные числа квадратом минимального числа
(использует алгоритм std::transform)*/
void ModifyTransform(deque<int>::iterator &begin, deque<int>::iterator end)
{
	int min = *begin;
	transform(begin, end, begin, [&min](int i) { if (i < min) min = i;	return i; });
	transform(begin, end, begin, [min](int i) {return i > 0 ? min*min : i; });
}

/*Функция, заменяющая все положительные числа квадратом минимального числа
(использует алгоритм std::for_each*/
void Modify_For_Each(deque<int>::iterator begin, deque<int>::iterator end) {
	int min = *begin;
	for_each(begin, end, [&min](int i) { if (i < min) min = i; });
	for_each(begin, end, [min](int &i) { i = i > 0 ? min*min : i; });
}

//Функция, вычисляющая сумму чисел, содержащихся в контейнере
int Sum(deque<int> deq)
{
	int sum = 0;
	//for_each(deq.begin(), deq.end(), [&sum](int i) {sum += i; });
	
	for (int i = 0; i < deq.size(); i++)
		sum += deq[i];
	return sum;
}

//Функция, вычисляющая среднее арифметическое чисел, содержащихся в контейнере
double Average(deque<int> deq)
{
	return (double)Sum(deq) / deq.size();
}

//Функция вывода в файл
void SaveToFile(fstream &st, deque<int> &deq)
{
	for_each(deq.begin(), deq.end(), [&st](int i) {st << i << " "; });
}

//Функция вывода на экран
void  Print(ostream &st, deque<int> deq) {
	for_each(deq.begin(), deq.end(), [&st](int i) {st << i << " "; });
	st << endl;
}

//Консольное меню
int Menu(int file_opened) {
	
	int res;

	do
	{
		cout << endl;
		cout << 1 << ": " << "create a new file with a random sequence of numbers" << endl;
		cout << 2 << ": " << "create a new file with a random sequence of numbers (std::generate)" << endl;
		cout << 3 << ": " << "load the sequence from the file" << endl;
		if (file_opened > 0) {
			cout << 4 << ": " << "replace all positive members of the sequence with a square of a minimum" << endl;
			cout << 5 << ": " << "replace all positive members of the sequence with a square of a minimum (iterators)" << endl;
			cout << 6 << ": " << "replace all positive members of the sequence with a square of a minimum (std::transform)" << endl;
			cout << 7 << ": " << "replace all positive members of the sequence with a square of a minimum (std::for_each)" << endl;
			cout << 8 << ": " << "calculate the sum of the members of the sequence." << endl;
			cout << 9 << ": " << "calculate the average value" << endl;
			cout << 10 << ": " << "save the sequence to a file" << endl;
			cout << 11 << ": " << "print the sequence" << endl;
		}
		cout << 0 << ": " << "exit" << endl << endl;
	}
	while (!(cin >> res) || file_opened <= 0 && (res > 2) || (file_opened > 0) && (res > 7));
	return res;
}

//Функция ввода имени файла
string InputFileName() {
	string name;
	cout << "Input file name" << endl;
	cin >> name;
	return name;
}

//Функция ввода числа
int InputNum(string str) {
	int Num;
	cout << str << endl;
	while (!(cin >> Num) || Num <= 0);
	return Num;
}

int main()
{
	bool exit = false;
	deque<int> deq;
	string FileName;
	ofstream ofstr;
	ifstream ifstr;
	int N;
	int M;
	do {
		switch (Menu(deq.size())) {
		case 1:
			N = InputNum("Enter the number of items");
			M = InputNum("Enter the maximum number module");
			FileName = InputFileName();
			ofstr = CreateFileRandom(FileName, N, M);

			ifstr.open(FileName);
			deq = LoadFromFile(ifstr);
			ifstr.close();

			Print(cout, deq);

			break;
		case 2:
			N = InputNum("Enter the number of items");
			M = InputNum("Enter the maximum number module");
			FileName = InputFileName();
			ofstr = CreateFileGenerate(FileName, N, M);

			ifstr.open(FileName);
			deq = LoadFromFile(ifstr);
			ifstr.close();

			Print(cout, deq);

			break;
		case 3:
			FileName = InputFileName();

			ifstr.open(FileName);
			deq = LoadFromFile(ifstr);
			ifstr.close();

			Print(cout, deq);
			break;
		case 4:
			deq = Modify(deq);
			Print(cout, deq);
			break;
		case 5:
			Modify(deq.begin(),deq.end());
			Print(cout, deq);
			break;
		case 6:
			ModifyTransform(deq.begin(),deq.end());
			Print(cout, deq);
			break;
		case 7:
			Modify_For_Each(deq.begin(),deq.end());
			Print(cout, deq);
			break;
		case 8:
			cout << "The sum of the elements is: " << Sum(deq) << endl;
			break;
		case 9:
			cout << "The average value is: " << Average(deq) << endl;
			break;
		case 10:
			FileName = InputFileName();
			ofstr.open(FileName);
			Print(ofstr, deq);
			ofstr.close();
			break;
		case 11:
			Print(cout, deq);
			break;
		case 0:
			return 0;
		}
	} while (1);
}

