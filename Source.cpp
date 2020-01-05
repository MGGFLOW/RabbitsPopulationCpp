#include "pch.h"
#include <iostream>
#include <time.h>

using namespace std;

struct node{ // структура, хранящая количество тиков для кролика
	int value;                             
	node *next;                     
};

class List{ // класс, реализующий список структур    
public:
	node *Head;
	List() { Head = NULL; } // инициализация списка         
	~List(); // очищение памяти, занятой списком в деструкторе     
	void Add(int value); // метод для добавления элемента в список (в начало) 
};

List::~List(){
	while (Head != NULL)           
	{
		node *temp = Head->next; 
		delete Head;                
		Head = temp;      
	}
}

void List::Add(int value){
	node *temp = new node;      
	temp->value = value; 
	temp->next = Head;
	
	Head = temp;
}

class Forest { // класс, реализующий основную логику поставленной задачи
public:
	void init(); // ввод входных значений поставленной задачи
	void emulate(); // метод, осуществляющий процесс решения
	void viewPopulation(); // метод для покак текущего числа особей
	Forest(int, int); // создание матрицы списков структур в конструкторе
	~Forest(); // очищение занятой памяти в деструкторе
private:
	int population; // текущее количество особей
	int live_ticks; // начальное количество тиков для каждой особи
	float reproduct_p; // вероятность появления новой особи
	int width; // ширина леса
	int height; // длина леса
	int ticks; // количество общих тиков
	List **cells; // матрица спсиков

	void addConey(List *cell,int rem_ticks); // добавление особи в клетку леса
	void pool(); // заполнение леса случайным образом на основе начального количества особей
	void allMove(); // перемещение особей за один тик
	void populationUp(); // появление новых особей
	int ccombo(int n, int k); // разбиение особей на пары
};

Forest::Forest(int w=10, int h=10) {
	width = w;
	height = h;

	cells = new List*[width];
	for (int i = 0; i < width; i++) {
		cells[i] = new List[height];
	}
}

Forest::~Forest() {
	for (int i = 0; i < width; i++) {
		delete[] cells[i];
	}

	delete cells;
}



int Forest::ccombo(int n, int k) {
	if (n > k)
		return n / k;
	else 
		return 0;
	
}

void Forest::init() {
	//cout << "Input:\n";
	cout << "Rabbits: "; cin >> population;
	cout << "Live ticks: "; cin >> live_ticks;
	cout << "Ticks: "; cin >> ticks;
	cout << "P: "; cin >> reproduct_p; cout << endl;
}

void Forest::addConey(List *cell, int rem_ticks) {
	cell->Add(rem_ticks);
}

void Forest::pool() {
	int x, y;

	for (int i = 0; i < population; i++) {
		x = rand() % width;
		y = rand() % height;

		addConey(&cells[x][y],live_ticks);
	}
}

void Forest::allMove() {
	node *current;
	int x, y;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			current = cells[i][j].Head;
			if (current == nullptr) continue;
			do {
				if (current->value < 0) {
					current->value = abs(current->value);
				}else if(current->value!=0){
					do {
						x = i - 1 + rand() % 3;
						y = j - 1 + rand() % 3;
					} while (!(x<width and x>-1 and y<height and y>-1));

					if (x != i and y != j) {
						addConey(&cells[x][y], -1 * current->value);
						current->value = 0;
					}
				}

				current = current->next;
				
			} while (current!=NULL);
		}
	}
}

void Forest::populationUp() {
		node *current;
		int k = 0;
		population = 0;
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				current = cells[i][j].Head;
				k = 0;
				if (current == nullptr) continue;
				do {
					if (current->value < 0) current->value = abs(current->value);

					if (current->value != 0) {
						current->value--;
						k++;
						population++;
					}
	
					current = current->next;
	
				} while (current != NULL);

				if (k > 1) {
					k = ccombo(k, 2);
					for (int c = 0; c < k; c++) {
						if (rand() % 100 <= static_cast<int>(reproduct_p * 100)) {
							addConey(&cells[i][j], live_ticks);
							population++;
						}
					}
				}
			}
		}
}

void Forest::emulate() {
	pool();
	for (int i = 0; i < ticks; i++) {
		allMove();
		populationUp();
		//viewPopulation();
	}
}

void Forest::viewPopulation() {
	cout << "\nPopulation now: " << population << endl;
}




int main()
{
	srand(time(NULL));
	Forest *F;
	int w, h;
	cout << "Input\n";
	cout << "Forest width: "; cin >> w;
	cout << "Forest height: "; cin >> h;

	F = new Forest(w,h);
	
	F->init();
	F->emulate();
	
	F->viewPopulation();
}
