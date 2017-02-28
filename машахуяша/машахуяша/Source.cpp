#include<iostream>
#include<clocale>

void main() {
	setlocale(LC_CTYPE, "rus");
	int n;
	std::cout << "Привет, Маша! Вводи числа от 1 до 19." << std::endl;
	while (true) {
		std::cin >> n;
		switch (n) {
		case 1: std::cout << "Ты лапочка!" << std::endl; break;
		case 2: std::cout << "Весь мир к твоим ушам!" << std::endl; break;
		case 3: std::cout << "Твои волосы словно колосья душистые прикрывают плечули холмисто-гористые..." << std::endl; break;
		case 4: std::cout << "Глаза твои лучистые как коты пушстые!" << std::endl; break;
		case 5: std::cout << "Твой стан гусиный бесподобен будто дорога без колдобин!" << std::endl; break;
		case 6: std::cout << "Подкачанный таз радует глаз!" << std::endl; break;
		case 7: std::cout << "Будда руку свою опустил и даром спокойствия тебя наградил" << std::endl; break;
		case 8: std::cout << "Ты как будто термопот, потому что теплая, вот" << std::endl; break;
		case 9: std::cout << "Гуляй, Шальная Императрица!" << std::endl; break;
		case 10: std::cout << "Это праздничная прога, будто у слона тринога" << std::endl; break;
		case 11: std::cout << "Ты - целая Вселенная!" << std::endl; break;
		case 12: std::cout << "Словно теплая глазунья, ты вкуснее лишь к полудню" << std::endl; break;
		case 13: std::cout << "По сравнения с тобой, Джоли беззубый злой ковбой" << std::endl; break;
		case 14: std::cout << "хуирныдцать" << std::endl; break;
		case 15: std::cout << "Сильно любим мы тебя, будто пИсос кабана (P.S. имеется ввиду размер любви)" << std::endl;
		case 16: std::cout << "Если сладких взять конфет, сладких кексов и повидло, торт, мороженое, пончик, газировку 'Колокольчик' - все же слаще будешь ты, королева красоты! " << std::endl; break;
		case 17: std::cout << "Пиши, скотина, что сидишь, лентяй!!!" << std::endl; break;
		case 18: std::cout << "Моя девочка пахнет, будто бы в ней свежая мята с оттенками рая..." << std::endl; break;
		case 19: std::cout << "С днем рождения, Машутка!" << std::endl; break;
		default: std::cout << "Поиграй со мной!" << std::endl;
		}
	}
}