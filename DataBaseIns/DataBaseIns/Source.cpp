#include<iostream>
#include<vector>
#include<fstream>
#include<string>

#define OP_CNT 5
#define G_CNT 5
#define CT_CNT 10
#define CL_CNT 10
#define OR_CNT 300
#define TR_CNT 3000


using std::ofstream;
using std::vector;
using std::string;

ofstream ofstr("InsScript.sql", std::ofstream::out);
vector<string> firstname;
vector<string> lastname;
vector<string> names;
vector<string> country;

void firstFull() {
	firstname.push_back("Саша");
	firstname.push_back("Маша");
	firstname.push_back("Катя");
	firstname.push_back("Артем");
	firstname.push_back("Оля");
	firstname.push_back("Юля");
	firstname.push_back("Даша");
	firstname.push_back("Вова");
	firstname.push_back("Давид");
	firstname.push_back("Дима");
	firstname.push_back("Игорь");
	firstname.push_back("Сергей");
	firstname.push_back("Никита");
	firstname.push_back("Рита");
	firstname.push_back("Иван");
	firstname.push_back("Витя");
	firstname.push_back("Илья");
	firstname.push_back("Влад");
	firstname.push_back("Арсений");
	firstname.push_back("Юра");
	firstname.push_back("Рома");
	firstname.push_back("Семён");
	firstname.push_back("Андрей");
	firstname.push_back("Кирилл");
	firstname.push_back("Филлип");
	firstname.push_back("Нина");
	firstname.push_back("Ксюша");
	firstname.push_back("Шура");
	firstname.push_back("Лиза");
	firstname.push_back("Регина");
	firstname.push_back("Кристина");
	firstname.push_back("Настя");
	firstname.push_back("Серафима");
	firstname.push_back("Ульяна");
	firstname.push_back("Василиса");
	firstname.push_back("Таня");
	firstname.push_back("Констанция");
	firstname.push_back("Федора");
	firstname.push_back("Людмила");
	firstname.push_back("Любовь");
	firstname.push_back("Диана");
	firstname.push_back("Вера");
	firstname.push_back("Надежда");
	return;
}

void lastFull() {
	lastname.push_back("Козлов");
	lastname.push_back("Иванов");
	lastname.push_back("Шевкунов");
	lastname.push_back("Родин");
	lastname.push_back("Проскурин");
	lastname.push_back("Невструев");
	lastname.push_back("Степанов");
	lastname.push_back("Крохалев");
	lastname.push_back("Вишняков");
	lastname.push_back("Калиниченко");
	lastname.push_back("Шаруев");
	lastname.push_back("Пыркин");
	lastname.push_back("Гоелик");
	lastname.push_back("Ишханян");
	lastname.push_back("Бородин");
	lastname.push_back("Коплевайкий");
	lastname.push_back("Канер");
	lastname.push_back("Крузенштерн");
	lastname.push_back("Смирнов");
	lastname.push_back("Береза");
	lastname.push_back("Калина-Красная");
	lastname.push_back("Калинов");
	lastname.push_back("Ланцетов");
	lastname.push_back("Кирюхин");
	lastname.push_back("Бьорнсон");
	lastname.push_back("Горячев");
	lastname.push_back("Сальников");
	lastname.push_back("Анухин");
	lastname.push_back("Глебов");
	lastname.push_back("Петров");
	lastname.push_back("Поздняков");
	lastname.push_back("Богачев");
	lastname.push_back("Спирин");
	lastname.push_back("Косых");
	return;
}

void namesFull() {
	names.push_back("Мендрегар");
	names.push_back("Вектор");
	names.push_back("София");
	names.push_back("ДСБВ");
	names.push_back("Стрела");
	names.push_back("Глобус");
	names.push_back("ЗолотоеКольцо");
	names.push_back("Звезда");
	names.push_back("МакРак");
	names.push_back("Авианосец");
	names.push_back("ВокругСвета");
	names.push_back("Каравелла");
	names.push_back("Надежда");
	return;
}

void countryFull() {
	country.push_back("Россия");
	country.push_back("Турция");
	country.push_back("Украина");
	country.push_back("Франция");
	country.push_back("Бельгия");
	country.push_back("Нидерланды");
	country.push_back("Люксембург");
	country.push_back("Германия");
	country.push_back("Польша");
	country.push_back("Белоруссия");
	country.push_back("Литва");
	country.push_back("Латвия");
	country.push_back("Болгария");
	country.push_back("Эстония");
	country.push_back("Греция");
	country.push_back("Италия");
	country.push_back("Испания");
	country.push_back("Португалия");
	country.push_back("Канада");
	country.push_back("США");
	country.push_back("Бразилия");
	country.push_back("Мексика");
	country.push_back("Япония");
	country.push_back("Китай");
	country.push_back("Монголия");
	country.push_back("Индия");
	country.push_back("Автсралия");
	country.push_back("НоваяЗеландия");
	country.push_back("Казахстан");
	country.push_back("Таиланд");
	country.push_back("Египет");
	country.push_back("Швейцария");
	country.push_back("Норвегия");
	country.push_back("Финляндия");
	country.push_back("Дания");
	country.push_back("Нигерия");
	country.push_back("ЮАР");
	country.push_back("Сирия");
	country.push_back("Ливия");
	country.push_back("Чили");
	country.push_back("Куба");
	country.push_back("Монако");
	country.push_back("Сан-Марино");
	country.push_back("Швеция");
	country.push_back("Аргентина");
	country.push_back("Гондурас");
	country.push_back("Перу");
	country.push_back("Эквадор");
	country.push_back("Колумбия");
	country.push_back("Гренландия");
	country.push_back("Алжир");
	country.push_back("Судан");
	country.push_back("Сомали");
	country.push_back("Ангора");
	country.push_back("Мадагаскар");
	country.push_back("Израиль");
	country.push_back("Индонезия");
	country.push_back("Великобритания");
	return;
}

void datePrint() {
	int k = 1 + rand() % 12;
	ofstr << "'2016";
	if (k < 10) ofstr << 0;
	ofstr << k;
	k = 1 + rand() % 28;
	if (k < 10) ofstr << 0;
	ofstr << k << "'";
	return;
}

void idprint(int a) {
	string str = "'00000000-0000-0000-0000-000000000000'";
	int i = 1;
	while (a != 0) {
		str[str.size() - 1 - i] = (a % 10) + '0';
		a /= 10;
		i++;
	}
	ofstr << str;
}
int main() {
	firstFull();
	lastFull();
	namesFull();
	countryFull();

	for (int i = 1; i <= OP_CNT; ++i){
		if (i % 1000 == 1)
			ofstr << std::endl << "INSERT INTO Operators(OperatorID, Name, LocationCountry) VALUES" << std::endl;
		ofstr << "(";
		idprint(i);
		ofstr << ", '" << names[rand() % names.size()] << "', '" << country[rand() % country.size()] << "')";
		if (i != OP_CNT && i % 1000 != 0) ofstr << ',';
		ofstr << std::endl;
	}

	ofstr << std::endl << "GO" << std::endl;

	for (int i = 1; i <= CT_CNT; ++i) {
		if (i % 1000 == 1)
			ofstr << std::endl << "INSERT INTO CatalogueTrips(CatalogueTripID, OperatorID, Name, Country, Size, Price) VALUES" << std::endl;
		ofstr << "(";
		idprint(i);
		ofstr << ", "; 
		idprint(1 + rand() % OP_CNT);
		ofstr << ", '" << names[rand() % names.size()] << "', '" << country[rand() % country.size()] << "', " << rand()%10 << ", " << rand() << ")";
		if (i != CT_CNT && i % 1000 != 0) ofstr << ',';
		ofstr << std::endl;
	}

	ofstr << std::endl << "GO" << std::endl;

	
	for (int i = 1; i <= G_CNT; ++i) {
		if (i % 1000 == 1)
			ofstr << std::endl << "INSERT INTO Guides(GuideID, FirstName, LastName) VALUES" << std::endl;
		ofstr << "(";
		idprint(i);
		ofstr << ", '" << firstname[rand() % firstname.size()] << "', '" << lastname[rand() % lastname.size()] << "')";
		if (i != G_CNT && i % 1000 != 0) ofstr << ',';
		ofstr << std::endl;
	}

	ofstr << std::endl << "GO" << std::endl;

	for (int i = 1; i <= TR_CNT; ++i) {
		if (i % 1000 == 1)
			ofstr << std::endl << "INSERT INTO Trips(TripID, CatalogueTripID, GuideID, TripDate) VALUES" << std::endl;
		ofstr << "(";
		idprint(i);
		ofstr << ", ";
		idprint(1 + rand() % CT_CNT);
		ofstr << ", ";
		idprint(1 + rand() % G_CNT);
		ofstr << ", ";
		datePrint();
		ofstr << ')';
		if (i != TR_CNT && i % 1000 != 0) ofstr << ',';
		ofstr << std::endl;
	}

	ofstr << std::endl << "GO" << std::endl;

	for (int i = 1; i <= CL_CNT; ++i) {
		if (i % 1000 == 1)
			ofstr << std::endl << "INSERT INTO Clients(ClientID, FirstName, LastName, Email) VALUES" << std::endl;
		int k = rand() % lastname.size();
		ofstr << "(";
		idprint(i);
		ofstr <<", '" << firstname[rand() % firstname.size()] << "', '" << lastname[k] << "', '" << lastname[k] << i << "@gmail.com')";
		if (i != CL_CNT && i % 1000 != 0) ofstr << ',';
		ofstr << std::endl;
	}

	ofstr << std::endl << "GO" << std::endl;

	for (int i = 1; i <= OR_CNT; ++i) {
		if (i % 1000 == 1)
			ofstr << std::endl << "INSERT INTO Orders(OrderID, OpenedDate, PayDate, TripID, Cost) VALUES" << std::endl;
		int k = rand() % lastname.size();
		ofstr << "(";
		idprint(i);
		ofstr <<", ";
		datePrint();
		ofstr << ", ";
		datePrint();
		ofstr << ", ";
		idprint(1 + rand() % TR_CNT);
		ofstr << ", " << 1 + rand() << ")";
		if (i != OR_CNT && i % 1000 != 0) ofstr << ',';
		ofstr << std::endl;
	}

	ofstr << std::endl << "GO" << std::endl;

	
	for (int i = 1; i <= OR_CNT; ++i) {
		if (i % 1000 == 1)
			ofstr << std::endl << "INSERT INTO Orders_Clients(OrderID, ClientID) VALUES" << std::endl;
		ofstr << "(";
		idprint(i);
		ofstr << ", ";
		idprint(1 + rand() % CL_CNT);
		ofstr << ")";
		if (i != OR_CNT && i % 1000 != 0) ofstr << ',';
		ofstr << std::endl;
	}

	ofstr << std::endl << "GO" << std::endl;

	return 0;
}