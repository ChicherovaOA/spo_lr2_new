#include <iostream>
#include <stdio.h>
#include <string>
#include <windows.h>
using namespace std;
int main() {
	setlocale(LC_ALL, "Russian");
	
	cout << "Введите полный путь к mailslot:" << endl;
	string path;
	getline(cin,path);
	path = "\\\\.\\mailslot\\" + path;
	wstring path2 = wstring(path.begin(), path.end());
	LPCWSTR winpath = path2.c_str();
	//auto attributes = create_security_attributes();
	auto mailslot = CreateMailslot(winpath, 0, MAILSLOT_WAIT_FOREVER,NULL);
	auto mailslot2 = CreateFile(winpath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (mailslot != INVALID_HANDLE_VALUE) {
		cout << "Почтовый слот создан" << endl;
		cout << "Введите одну из команд: \nwrite-запись в слот\nread-чтение из слота\nquit-выход из программы" << endl;
		string command="";
		while (command.compare("quit") != 0) {
			cout << "< ";
			getline(cin, command);
			if (command.compare("quit") == 0) {
				cout << "Выход..." << endl;
			}
			else if (command.compare("write") == 0) {
				cout << "Вводите строки, пустая строка - конец ввода" << endl;
				string line = ".";
				while (line.compare("") != 0) {
					getline(cin, line);
					wstring line2 = wstring(line.begin(), line.end());
					LPCWSTR winline = line2.c_str();
					DWORD cbWritten;
					if (!WriteFile(mailslot2, winline, (DWORD)(lstrlen(winline) + 1) * sizeof(TCHAR), &cbWritten, (LPOVERLAPPED)NULL)) {
						cout << "Данные не были записаны в слот!" << endl;
						break;
					}
				}
				cout << "Данные успешно записаны!" << endl;
			}
			else if (command.compare("read") == 0){
				TCHAR str[256];
				DWORD bytesRead;
				while (1) {
					if (!ReadFile(mailslot, str, sizeof(TCHAR) * 256, &bytesRead, NULL)) {
						cout << "Данные не были прочитаны!" << endl;
						break;
					}
					cout << str << endl;
				}
			}
			else {
				cout << "Нераспознанная команда!" << endl;
			}
		}
	}
	else {
		cout << "Ошибка при создании почтового слота! Выход из программы..." << endl;
	}
	system("pause");
	return 0;
}
