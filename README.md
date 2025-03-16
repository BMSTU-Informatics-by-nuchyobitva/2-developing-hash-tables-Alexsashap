
# Семинар 2
## 20 Вариант: Метод Цепочек  

``` c++
class TennisPlayer { // класс теннисистов
	std::string surname; //фамилия спортсмена
	std::string city; // город, который представляет спортсмен
	int year; //год рождения
	int rankings; // рейтинг
	bool deleted = false; // для проверки удален объект или нет
public:
	TennisPlayer(std::string _surname, std::string _city, int _year, int _rankings) // конструктор
		: surname(_surname), city(_city), year(_year), rankings(_rankings) {
	}

	std::string get_surname() const {
		return surname;
	}
	std::string get_city() const {
		return city;
	}
	int get_year() const {
		return year;
	}
	int get_rankings() const {
		return rankings;
	}

	bool get_deleted() const {
		return deleted;
	}
	void set_deleted() { // пометка, что объект удалён
		deleted = true;
	}
	bool operator==(const TennisPlayer& other) const { // оператор сравнения двух спортсменов
		return (surname == other.get_surname() && city == other.get_city() && year == other.get_year() &&
			rankings == other.get_rankings());
	}
};
```
Был создан класс, состоющий из строк и целочисленных переменных.  Также были реализованы конструктор класса, геттеры, сеттер, перегрузка оператора ==.  

```c++
class HashTable {
	std::vector < std::list <TennisPlayer>> table; // Вектор списков для хранения данных
	size_t capacity; // Размер таблицы
	size_t realsize;

	size_t Hash(const std::string& key) const { // хэш-функция
		const int N = 461; 
		const int m = 7;     
		const int r = 47;     
		
		int len = key.length();
		long long hash=0;
		for (char i:key) {
			hash += ((int(i) + (int(i) * m ^ (len - 1) % N))%N)+r;
			len = len - 1;
		}
		return hash % capacity;
	}
```
Написан класс HashTable и реализована хэш-функция, в основе которой лежит метод полиномиального хеширования.  
Преимуществами этого метода является:  
* Легкая реализация и достаточно быстрое исполнение.  
* Обеспечивает хорошее распределение хеш-значений для большинства строк.  
* Но для уменьшения возможных коллизий необходимо правильно подобрать простые числа.  

```c++
long long get_index(const std::string& key) const { // получение индекса в хэш-таблице по ключу
	size_t index = Hash(key);
	return index;
}
void rehash() { // рехэшируем, когда таблица заполняется на 0.75
	size_t new_capacity = capacity * 2;
	std::vector<std::list <TennisPlayer>> new_table(new_capacity);
	for (size_t i = 0; i < capacity; i++) {
		if (!(table[i].empty())) { // переносим не удалённые элементы
			for (auto& player : table[i]) {
				if ((player).get_deleted() == false) {
					size_t new_index = Hash((player).get_surname());
					new_table[new_index].push_back(player);
				}
			}
		}
	}
		table = std::move(new_table); // переносим новую таблицу
		capacity = new_capacity;
}

```
Cозданы методы get_index, который позволяет найти индекс, который равен ключу после применения хэш функции, и rehash, который сщздают таблицу в 2 раза больше при заполнении таблице на 75%. Метод rehash пересчитывает индексы элементов, которые были ранее добавлены в таблицу и не были удалены, и добавляет их в новую таблицу.  

```c++
HashTable(size_t initial_size = 13)  // конструктор, по умолчанию размер 13
	: capacity(initial_size), realsize(0) {
	// инициализируем table с capacity пустыми списками
	table.resize(capacity);
}

~HashTable() { // деструктор
	clear();
}
```

```c++
void insert(const std::string& key, TennisPlayer& value) { // вставка элемента
	size_t index = Hash(key);
	for (auto& player : table[index]) {
		if (player.get_surname() == key && !player.get_deleted()) { // если ключ уже есть в таблице - перезаписываем
			player = value;
			return;
		}
	}
	table[index].push_back(value); // добавляем новый элемент
	realsize++;

	if ((double)realsize / capacity >= 0.75) { // проверка заполненности
		rehash();
	}
}
```
Вставка нового элемента реализуется через insert, который включает проверку нет ли данных на этот ключ. Если ключа нет в таблице, то добавляет элемент в нужный индекс и проверяет заполненность хэш-таблицы(при необходимости вызывает rehash)  

```c++
bool find(const std::string& key, const TennisPlayer& value) const { 
	long long index = get_index(key);
	for (auto& player : table[index]) {
		if ((player).get_deleted() == true || (!((player) == value)))
			return false;
	}
	return true;
}

bool remove(const std::string key) { // true - если удалось удалить элемент, false - если элемент не найден
	long long index = get_index(key);
	for (auto& player : table[index]) {
		if ((player).get_surname() == key) {
			(player).set_deleted();
			realsize--;
			return true;
		}
	}
	return false;
}
```

Реализован метод find - поиск по ключу и значению, и метод remove, который получает на вход ключ и при наличии такого элемента, удаляет его, переводя флаг deleted из false в true.  

```c++
	void clear() { // очистка таблицы
		for (size_t i = 0; i < capacity; ++i) {
			for (auto& player : table[i]) {
				(player).set_deleted();
			}
			if (!table[i].empty()) {
				table[i].clear();
			}
		}
		realsize = 0;
	}

	void print() const { // вывод таблицы на экран
		for (size_t i = 0; i < capacity; i++) {
			int j = 0;// флаг, что все элементы удалены
			std::cout << "table[" << i << "] --> ";
			if (!table[i].empty()) {
				for (const auto& player : table[i]) {
					if (!player.get_deleted()) {
						j = 1;
						std::cout << "Surname: " << player.get_surname() << "; ";
						std::cout << "City: " << player.get_city() << "; ";
						std::cout << "Year: " << player.get_year() << "; ";
						std::cout << "Rankings: " << player.get_rankings() << std::endl;
					}
				}
				if (j == 0) {
					std::cout << "NOT" << std::endl;
				}
			}
			else {
				std::cout << "NOT" << std::endl;
			}
		}
	}
};

```
Метод clear переводит все флаг deleted из false в true и отчищает списки, из которых состоит таблица.  
Print выводит на экран все элементы, которые находятся в таблице в момент вызова, в представленном формате.  

![Скрин симмулятора](https://github.com/BMSTU-Informatics-by-nuchyobitva/2-developing-hash-tables-Alexsashap/blob/main/Снимок%20экрана%202025-03-15%20234928.png)
