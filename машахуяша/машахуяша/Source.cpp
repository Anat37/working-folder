#include<iostream>
#include<clocale>

void main() {
	setlocale(LC_CTYPE, "rus");
	int n;
	std::cout << "������, ����! ����� ����� �� 1 �� 19." << std::endl;
	while (true) {
		std::cin >> n;
		switch (n) {
		case 1: std::cout << "�� �������!" << std::endl; break;
		case 2: std::cout << "���� ��� � ����� ����!" << std::endl; break;
		case 3: std::cout << "���� ������ ������ ������� �������� ���������� ������� ��������-��������..." << std::endl; break;
		case 4: std::cout << "����� ���� �������� ��� ���� �������!" << std::endl; break;
		case 5: std::cout << "���� ���� ������� ���������� ����� ������ ��� ��������!" << std::endl; break;
		case 6: std::cout << "����������� ��� ������ ����!" << std::endl; break;
		case 7: std::cout << "����� ���� ���� ������� � ����� ����������� ���� ��������" << std::endl; break;
		case 8: std::cout << "�� ��� ����� ��������, ������ ��� ������, ���" << std::endl; break;
		case 9: std::cout << "�����, ������� �����������!" << std::endl; break;
		case 10: std::cout << "��� ����������� �����, ����� � ����� �������" << std::endl; break;
		case 11: std::cout << "�� - ����� ���������!" << std::endl; break;
		case 12: std::cout << "������ ������ ��������, �� ������� ���� � �������" << std::endl; break;
		case 13: std::cout << "�� ��������� � �����, ����� �������� ���� ������" << std::endl; break;
		case 14: std::cout << "�����������" << std::endl; break;
		case 15: std::cout << "������ ����� �� ����, ����� ����� ������ (P.S. ������� ����� ������ �����)" << std::endl;
		case 16: std::cout << "���� ������� ����� ������, ������� ������ � �������, ����, ���������, ������, ��������� '�����������' - ��� �� ����� ������ ��, �������� �������! " << std::endl; break;
		case 17: std::cout << "����, �������, ��� ������, ������!!!" << std::endl; break;
		case 18: std::cout << "��� ������� ������, ����� �� � ��� ������ ���� � ��������� ���..." << std::endl; break;
		case 19: std::cout << "� ���� ��������, �������!" << std::endl; break;
		default: std::cout << "������� �� ����!" << std::endl;
		}
	}
}