#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility>

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

public:
	HashTable(size_t initial_size = 13)  // конструктор, по умолчанию размер 13
		: capacity(initial_size), realsize(0) {
		// инициализируем table с capacity пустыми списками
		table.resize(capacity);
	}

	~HashTable() { // деструктор
		clear();
	}

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

	bool find(const std::string& key, const TennisPlayer& value) const { // поиск элемента по ключу
		long long index = get_index(key);
		for (auto& player : table[index]) {
			if ((player).get_deleted() == true || (!((player) == value)))
				return false;
		}
		return true;
	}

	bool find(const std::string& key) const { // поиск элемента по ключу
		long long index = get_index(key);
		for (auto& player : table[index]) {
			if ((player).get_deleted())
				return false;
		}
		return true;
	}

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

	size_t get_size() const {
		return realsize;
	}

	size_t get_capacity() const {
		return capacity;
	}

	bool empty() const { // проверка, пуста ли таблица
		return realsize == 0;
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

void test1() {
	TennisPlayer play1("Sidorenko", "Petersburg", 2002, 2119);
	TennisPlayer play2("Ivonin", "Orenburg", 1999, 1996);
	TennisPlayer play3("Fedotov", "Petersburg", 1997, 1970);
	TennisPlayer play4("Artemenko", "Petersburg", 2002, 1969);
	TennisPlayer play5("Katzman", "Moscow", 2001, 1965);
	TennisPlayer play6("Ivonen", "Petersburg", 2002, 2119);
	HashTable ht;
	ht.insert(play1.get_surname(), play1);
	ht.insert(play2.get_surname(), play2);
	ht.insert(play3.get_surname(), play3);
	ht.insert(play4.get_surname(), play4);
	ht.insert(play5.get_surname(), play5);
	ht.insert(play6.get_surname(), play6);
	ht.print();
	std::cout << ht.find("Katzman", play5) << std::endl;
	ht.remove("Katzman");
	std::cout << ht.find("Katzman", play5) << std::endl;
	ht.print();
	std::cout << ht.empty() << std::endl;
	std::cout << ht.get_size() << std::endl;
	ht.clear();
	std::cout << ht.empty() << std::endl;
	ht.print();
	std::cout << ht.find("Fedotov", play3) << std::endl;
	std::cout << ht.find("Ivonin", play2) << std::endl;

}
int main() {
	test1();
}
