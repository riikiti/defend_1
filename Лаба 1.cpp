#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <algorithm>

using namespace std;

class CodeCeaser {
private:
	int a = 0, k = 0, abcSize = 128;
	string decAbc = "", abc = "";
public:
	CodeCeaser() {
		ifstream keysFile("keys.txt");
		if (keysFile.is_open())
			keysFile >> a >> k;
		else
			cout << " Файл с ключами не найден!\n";
		keysFile.close();

		// заполнили базовый алфавит
		for (int i = 128; i < 256; i++)
			abc += (char)i;

		// заполнили алфавит замены
		for (int i = 0; i < abcSize; i++)
			decAbc += abc[(a * i + k) % abcSize];

		ofstream abcFile("abc.txt");
		abcFile << abc << "\n\n" << decAbc;
		abcFile.close();
	}

	~CodeCeaser() {};

	string encrypt(string msg) {
		string tempMsg = "";
		for (int i = 0, j = 0; i < msg.size(); i++) {
			for (j = 0; abc[j] != msg[i]; j++);
			tempMsg += abc[(a * j + k) % abcSize];
		}
		return tempMsg;
	}

	string decrypt(string msg) {
		string tempMsg = "";
		for (int i = 0, j = 0; i < msg.size(); i++) {
			for (j = 0; decAbc[j] != msg[i]; j++);
			tempMsg += abc[j];
		}
		return tempMsg;
	}
};

class CodeDoubleShift {
private:
	string n = "", m = "";

	string abraKadabra(string msg, bool crypt) {
		while (msg.size() < (m.size() * n.size())) {
			msg += ' ';
		}
		char** mas = new char* [n.size()];
		char** mas1 = new char* [n.size()];
		for (int i = 0; i < n.size(); i++)
		{
			mas[i] = new char[m.size()];
			mas1[i] = new char[m.size()];
		}

		for (int i = 0; i < n.size(); i++)
			for (int j = 0; j < m.size(); j++)
			{
				mas[i][j] = ' ';
				if (!crypt)
					mas1[i][j] = msg[i * m.size() + j];
				else
					mas1[i][j] = msg[j * n.size() + i];
			}

		// сортировка символов в строках-ключах по ASCII
		string sortN = n, sortM = m;
		sort(sortN.begin(), sortN.end());
		sort(sortM.begin(), sortM.end());

		// весь код ниже, до следующего коммента - старый порядковый номер символов в новых сортировонных string
		// чтобы передвинуть буквы на новые позиции для шифрования
		int* indexN = new int[n.size()];
		int* indexM = new int[m.size()];
		*indexN = *indexM = { -1 };

		int count = 0;
		for (char t : sortM)
		{
			int temp = m.find(t, 0);
			for (int i = 0; i < m.size(); i++)
				if (indexM[i] == temp)
				{
					temp = m.find(t, ++temp);
					i = -1;
				}
			indexM[count++] = temp;
		}

		count = 0;
		for (char t : sortN)
		{
			int temp = n.find(t, 0);
			for (int i = 0; i < n.size(); i++)
				if (indexN[i] == temp)
				{
					temp = n.find(t, ++temp);
					i = -1;
				}
			indexN[count++] = temp;
		}

		cout << "\n\tБыло:\n";
		for (int i = 0; i < n.size(); i++) {
			cout << "\t\t";
			for (int j = 0; j < m.size(); j++)
				cout << mas1[i][j] << ' ';
			cout << "\n";
		}

		for (int i = 0; i < n.size(); i++)
			for (int j = 0; j < m.size(); j++)
				if (!crypt)
					mas[i][j] = mas1[i][indexM[j]];
				else
					mas[indexN[i]][j] = mas1[i][j];

		cout << "\n\tПервый сдвиг:\n";
		for (int i = 0; i < n.size(); i++) {
			cout << "\t\t";
			for (int j = 0; j < m.size(); j++)
				cout << mas[i][j] << ' ';
			cout << "\n";
		}


		//  создаём шифрованную матрицу по изменённым позициям после сортировки
		for (int i = 0; i < n.size(); i++)
			for (int j = 0; j < m.size(); j++)
				if (!crypt)
					mas[i][j] = mas1[indexN[i]][indexM[j]];
				else
					mas[indexN[i]][indexM[j]] = mas1[i][j];


		cout << "\n\tВторой сдвиг:\n";
		for (int i = 0; i < n.size(); i++) {
			cout << "\t\t";
			for (int j = 0; j < m.size(); j++)
				cout << mas[i][j] << ' ';
			cout << "\n";
		}

		//  составление шифр-текста и вывод данных на экран
		msg = "";
		if (!crypt)
			for (int j = 0; j < m.size(); j++)
				for (int i = 0; i < n.size(); i++)
					msg += mas[i][j];
		else
			for (int i = 0; i < n.size(); i++)
				for (int j = 0; j < m.size(); j++)
					msg += mas[i][j];

	

	

		for (int i = 0; i < n.size(); i++)
		{
			delete mas[i];
			delete mas1[i];
		}
		delete[] mas;
		delete[] mas1;

		return msg;
	}
public:
	CodeDoubleShift() {
		ifstream keysFile("keys2.txt");
		if (keysFile.is_open())
			keysFile >> m >> n;
		else
			cout << " Файл с ключами не найден!\n";
		keysFile.close();
	}

	~CodeDoubleShift() {};

	string encrypt(string msg) {
		return abraKadabra(msg, false);
	}

	string decrypt(string msg) {
		return abraKadabra(msg, true);
	}
};

template <typename T>
void file(string fileRead, string fileWrite, T code) {
	ifstream read(fileRead);
	string k = "", msg = "";
	while (getline(read, k))
		msg += k;
	read.close();

	ofstream write(fileWrite);
	if (fileWrite == "encrypt.txt")
		write << code.encrypt(msg);
	else
		write << code.decrypt(msg);
	write.close();
}

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	int ch1 = 0;

	while (ch1 != 3) {
		cout << "\n 1) аффинная система подстановок Цезаря;\n"
			" 2) двойная перестановка по ключу;\n"
			" 3) выход.\n"
			" Номер: ";
		do {
			cin >> ch1;
		} while (ch1 < 1 || ch1 > 3);

		int ch2 = 0;
		string msg = "";

		while (ch2 != 3) {
			cout << "\n\t1) зашифровать сообщение;\n"
				"\t2) дешифровать сообщение;\n"
				"\t3) выход.\n"
				"\tНомер: ";
			do {
				cin >> ch2;
				if (ch2 < 1 || ch2 > 3)
					cout << "\t";
			} while (ch2 < 1 || ch2 > 3);
			cin.ignore(666, '\n');

			switch (ch1)
			{
			case 1: {
				CodeCeaser code1;

				switch (ch2)
				{
				case 1: {
					file<CodeCeaser>("decrypt.txt", "encrypt.txt", code1);
					
					/*cout << "\n\tВведите сообщение: ";
					getline(cin, msg);
					cout << "\tЗашифрованное сообщение: " << code1.encrypt(msg) << "\n";*/
					break;
				}
				case 2: {
					file<CodeCeaser>("encrypt.txt", "decrypt.txt", code1);

					/*cout << "\n\tВведите зашифрованное сообщение: ";
					getline(cin, msg);
					cout << "\tДешифрованное сообщение: " << code1.decrypt(msg) << "\n";*/
					break;
				}
				}
				break;
			}
			case 2: {
				CodeDoubleShift code2;

				switch (ch2)
				{
				case 1: {
					file<CodeDoubleShift>("decrypt.txt", "encrypt.txt", code2);
					
					/*cout << "\n\tВведите сообщение: ";
					getline(cin, msg);
					cout << "\n\tЗашифрованное сообщение: " << code2.encrypt(msg) << "\n";
					*/
					break;
				}
				case 2: {
					file<CodeDoubleShift>("encrypt.txt", "decrypt.txt", code2);
					
					/*cout << "\n\tВведите зашифрованное сообщение: ";
					getline(cin, msg);
					cout << "\n\tДешифрованное сообщение: " << code2.decrypt(msg) << "\n";
					*/
					break;
				}
				}
				break;
			}
			}
		}
	}

	system("pause");
	return 0;
}