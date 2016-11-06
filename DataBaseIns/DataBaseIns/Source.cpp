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
	firstname.push_back("����");
	firstname.push_back("����");
	firstname.push_back("����");
	firstname.push_back("�����");
	firstname.push_back("���");
	firstname.push_back("���");
	firstname.push_back("����");
	firstname.push_back("����");
	firstname.push_back("�����");
	firstname.push_back("����");
	firstname.push_back("�����");
	firstname.push_back("������");
	firstname.push_back("������");
	firstname.push_back("����");
	firstname.push_back("����");
	firstname.push_back("����");
	firstname.push_back("����");
	firstname.push_back("����");
	firstname.push_back("�������");
	firstname.push_back("���");
	firstname.push_back("����");
	firstname.push_back("����");
	firstname.push_back("������");
	firstname.push_back("������");
	firstname.push_back("������");
	firstname.push_back("����");
	firstname.push_back("�����");
	firstname.push_back("����");
	firstname.push_back("����");
	firstname.push_back("������");
	firstname.push_back("��������");
	firstname.push_back("�����");
	firstname.push_back("��������");
	firstname.push_back("������");
	firstname.push_back("��������");
	firstname.push_back("����");
	firstname.push_back("����������");
	firstname.push_back("������");
	firstname.push_back("�������");
	firstname.push_back("������");
	firstname.push_back("�����");
	firstname.push_back("����");
	firstname.push_back("�������");
	return;
}

void lastFull() {
	lastname.push_back("������");
	lastname.push_back("������");
	lastname.push_back("��������");
	lastname.push_back("�����");
	lastname.push_back("���������");
	lastname.push_back("���������");
	lastname.push_back("��������");
	lastname.push_back("��������");
	lastname.push_back("��������");
	lastname.push_back("�����������");
	lastname.push_back("������");
	lastname.push_back("������");
	lastname.push_back("������");
	lastname.push_back("�������");
	lastname.push_back("�������");
	lastname.push_back("�����������");
	lastname.push_back("�����");
	lastname.push_back("�����������");
	lastname.push_back("�������");
	lastname.push_back("������");
	lastname.push_back("������-�������");
	lastname.push_back("�������");
	lastname.push_back("��������");
	lastname.push_back("�������");
	lastname.push_back("��������");
	lastname.push_back("�������");
	lastname.push_back("���������");
	lastname.push_back("������");
	lastname.push_back("������");
	lastname.push_back("������");
	lastname.push_back("���������");
	lastname.push_back("�������");
	lastname.push_back("������");
	lastname.push_back("�����");
	return;
}

void namesFull() {
	names.push_back("���������");
	names.push_back("������");
	names.push_back("�����");
	names.push_back("����");
	names.push_back("������");
	names.push_back("������");
	names.push_back("�������������");
	names.push_back("������");
	names.push_back("������");
	names.push_back("���������");
	names.push_back("�����������");
	names.push_back("���������");
	names.push_back("�������");
	return;
}

void countryFull() {
	country.push_back("������");
	country.push_back("������");
	country.push_back("�������");
	country.push_back("�������");
	country.push_back("�������");
	country.push_back("����������");
	country.push_back("����������");
	country.push_back("��������");
	country.push_back("������");
	country.push_back("����������");
	country.push_back("�����");
	country.push_back("������");
	country.push_back("��������");
	country.push_back("�������");
	country.push_back("������");
	country.push_back("������");
	country.push_back("�������");
	country.push_back("����������");
	country.push_back("������");
	country.push_back("���");
	country.push_back("��������");
	country.push_back("�������");
	country.push_back("������");
	country.push_back("�����");
	country.push_back("��������");
	country.push_back("�����");
	country.push_back("���������");
	country.push_back("�������������");
	country.push_back("���������");
	country.push_back("�������");
	country.push_back("������");
	country.push_back("���������");
	country.push_back("��������");
	country.push_back("���������");
	country.push_back("�����");
	country.push_back("�������");
	country.push_back("���");
	country.push_back("�����");
	country.push_back("�����");
	country.push_back("����");
	country.push_back("����");
	country.push_back("������");
	country.push_back("���-������");
	country.push_back("������");
	country.push_back("���������");
	country.push_back("��������");
	country.push_back("����");
	country.push_back("�������");
	country.push_back("��������");
	country.push_back("����������");
	country.push_back("�����");
	country.push_back("�����");
	country.push_back("������");
	country.push_back("������");
	country.push_back("����������");
	country.push_back("�������");
	country.push_back("���������");
	country.push_back("��������������");
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