#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#pragma warning (disable:4996)

#define ESC 27  // константы для клавиш
#define UP 72
#define DOWN 80
#define ENTER 13

using namespace std;

struct person
{
	char* name;
	char* num;
	char* email;
	int n;
};

struct LogFile
{
	char* name;
	char* num;
	char* email;
	//int n;
};


HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // получили дискриптор консоли
void GoToXY(short x, short y) // курсор в точки х и у
{
	SetConsoleCursorPosition(hStdOut, { x, y });
}

void ConsoleCursorVisible(bool show, short size) // отображение курсора 
{
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(hStdOut, &structCursorInfo);
	structCursorInfo.bVisible = show; // отключили курсор
	structCursorInfo.dwSize = size; // размер курсора
	SetConsoleCursorInfo(hStdOut, &structCursorInfo);
}

void Show(person* mass, int size1)
{

	cout << "#\tName\t\tNumber\t\tE-mail\n";
	for (int i = 0; i < size1; i++)
	{
		if (mass[i].n == 1) {
			SetConsoleTextAttribute(hStdOut, 2);
			cout << i + 1 << "\t" << mass[i].name << "\t" << mass[i].num << "\t" << mass[i].email << endl;
		}
	}

	for (int i = 0; i < size1; i++)
	{
		if (mass[i].n == 1) continue;
		else {
			SetConsoleTextAttribute(hStdOut, 3);
			cout << i + 1 << "\t" << mass[i].name << "\t" << mass[i].num << "\t" << mass[i].email << endl;
		}
	}
}




bool CheckMail(person*& mass, int size1)
{
	int dogIndex = -1;
	int dotIndex = -1;

	if (strlen(mass[size1].email) < 8)
		return 0;

	for (int i = 0; mass[size1].email[i] != '\0'; i++)
	{
		if (strchr("*<>\?|:/'\"", mass[size1].email[i]))
			return false;
		if (mass[size1].email[i] == '@')
		{
			if (dogIndex != -1)
				return 0;
			if (i < 4)
				return 0;
			dogIndex = i;
		}
		if (mass[size1].email[i] == '.')
		{
			if (dogIndex == -1 || i - dogIndex <= 1 || dotIndex != -1)
				return 0;
			dotIndex = i;
		}
	}

	if (dogIndex == -1 || dotIndex == -1 || dotIndex == strlen(mass[size1].email) - 1)
		return 0;

	if (strlen(mass[size1].email + dotIndex) < 3)
		return 0;

	return 1;
}

bool CheckNum(person*& mass, int size1)
{
	char sym[] = R"(1234567890+-()";
	if (strlen(mass[size1].num) < 9)
		return 0;
	else
	{
		for (int i = 0; mass[size1].num[i] != '\0'; i++)
		{
			if (!isdigit(mass[size1].num[i]) && !strchr(sym, mass[size1].num[i]))
				return 0;
			if (mass[size1].num[i] == '+' && i != 0)
				return 0;
		}
		if (strspn(mass[size1].num, sym) != strlen(mass[size1].num))
			return 0;
		return 1;
	}
}

void Add(person*& mass, int& size1)
{
	char buff[100];
	person* temp = new person[size1 + 1];

	for (int i = 0; i < size1; i++)
		temp[i] = mass[i];
	delete[] mass;
	mass = temp;

	do
	{
		cout << "Enter name\n"; cin.getline(buff, 100);
		mass[size1].name = new char[strlen(buff) + 1];
		strcpy(mass[size1].name, buff);
	} while (strlen(mass[size1].name) < 7);
	do
	{
		cout << "Enter num\n"; cin.getline(buff, 100);
		mass[size1].num = new char[strlen(buff) + 1];
		strcpy(mass[size1].num, buff);
	} while (!CheckNum(mass, size1));

	do
	{
		cout << "Enter email\n"; cin.getline(buff, 100);
		mass[size1].email = new char[strlen(buff) + 1];
		strcpy(mass[size1].email, buff);
	} while (!CheckMail(mass, size1));

	size1++;

	Show(mass, size1);
}

void DelIndex(person*& mass, int& size1, int index)
{
	person* temp = new person[size1 - 1];
	for (int i = 0; i < size1; i++)
	{
		if (i <= index) temp[i] = mass[i];
		else if (i >= index) temp[i - 1] = mass[i];
	}
	delete[] mass;
	mass = temp;
	size1--;
}


void Edit(person* mass, int size1, int SField, int index)
{
	char temp[100];
	switch (SField)
	{
	case 1:
		cout << "Enter new name\n"; cin.getline(temp, 100);
		delete[] mass[index].name;
		mass[index].name = new char[strlen(temp) + 1];
		strcpy(mass[index].name, temp);
		break;
	case 2:
		cin.ignore();
		cout << "Enter new number\n"; cin.getline(temp, 100);
		delete[] mass[index].num;
		mass[index].num = new char[strlen(temp) + 1];
		strcpy(mass[index].num, temp);
		break;
	case 3:
		cin.ignore();
		cout << "\nEnter new E-mail\n"; cin.getline(temp, 100);
		delete[] mass[index].email;
		mass[index].email = new char[strlen(temp) + 1];
		strcpy(mass[index].email, temp);
		break;
	}
}

void Sort(person* mass, int size1, int direct)
{
	for (int i = 0; i < size1; i++)
	{
		for (int j = 0; j < size1 - i - 1; j++)
		{
			if (direct == 1)
			{
				if (strcmp(mass[j].name, mass[j + 1].name) == 1)
					swap(mass[j], mass[j + 1]);
			}
			else
			{
				if (strcmp(mass[j].name, mass[j + 1].name) == -1)
					swap(mass[j], mass[j + 1]);
			}
		}
	}
}


void Write(person* mass, int size1, char* filename)
{
	FILE* out = fopen(filename, "w");
	for (int i = 0; i < size1; i++)
	{
		if (mass[i].n == 1) {
			fputs(mass[i].name, out);
			fputc(';', out);
			fputs(mass[i].num, out);
			fputc(';', out);
			fputs(mass[i].email, out);
			fputc(';', out);
			fputc('\n', out);
		}
		else continue;
	}
	for (int i = 0; i < size1; i++)
	{
		if (mass[i].n == 1) continue;
		else {
			fputs(mass[i].name, out);
			fputc(';', out);
			fputs(mass[i].num, out);
			fputc(';', out);
			fputs(mass[i].email, out);
			fputc(';', out);
			fputc('\n', out);
		}
	}
	fclose(out);
}

bool Check(char* filename)
{
	char buff[260];
	char base[] = R"(con prn aux nul com1 com2 com3 com4 com5 com6 com7 com8 com9 lpt1 lpt2 lpt3 lpt4 lpt5 lpt6 lpt7 lpt8 lpt9)";
	char sym[] = R"(*<>\?|:/'")";
	if (strlen(filename) < 1)
		return 0;
	else
	{
		for (int i = 0; filename[i] != '\0'; i++)
			for (int j = 0; sym[j] != '\0'; j++)
				if (filename[i] == sym[j])
					return 0;
		for (int i = 0; filename[i] != '\0'; i++)
			buff[i] = tolower(filename[i]);
		buff[strlen(filename)] = '\0';
		char* token = strtok(base, " ");
		if (!strcmp(token, buff))
			return 0;
		while (true)
		{
			token = strtok(NULL, " ");
			if (token == NULL)
				break;
			else
			{
				if (!strcmp(buff, token))
					return 0;
			}
		}
		return 1;
	}
}

void Read(LogFile*& group, int& size1, char* filename)
{
	int counter = 0;
	char buff[500];
	FILE* in = fopen(filename, "r");
	if (!in) cout << "Wrong file\n";
	else
	{
		while (fgets(buff, 500, in))
		{
			counter++;
		}
		group = new LogFile[counter];
		rewind(in);

		for (int i = 0; i < counter;)
		{
			fgets(buff, 500, in);
			char* token = strtok(buff, ";");
			group[i].name = new char[strlen(token) + 1];
			strcpy(group[i].name, token);
			token = strtok(NULL, ";");
			group[i].num = new char[strlen(token) + 1];
			strcpy(group[i].num, token);
			token = strtok(NULL, ";");
			group[i].email = new char[strlen(token) + 1];
			strcpy(group[i].email, token);
			token = strtok(NULL, ";");
			i++;
		}
		fclose(in);
	}
}

void Show2(LogFile* group, int size1, person*& mass)
{

	for (int i = 0; i < size1; i++)
	{
		if (mass[i].n == 1) {
			SetConsoleTextAttribute(hStdOut, 2);
			cout << i + 1 << "\t" << group[i].name << "\t\t" << group[i].num << "\t";
			cout << group[i].email << "\n";
		}
		else continue;
	}
	for (int i = 0; i < size1; i++)
	{
		if (mass[i].n == 1) continue;
		else {
			SetConsoleTextAttribute(hStdOut, 5);
			cout << i + 1 << "\t" << group[i].name << "\t\t" << group[i].num << "\t";
			cout << group[i].email << "\n";
		}
	}
	cout << endl;
}




int main()
{
	int size1 = 0;
	int index = 0, SField, direct;
	int choice1;
	bool job = 1;
	person* mass = nullptr;
	LogFile* group = nullptr;

	char filename[260];
	char ch; // переменная для клавиш


	SetConsoleTitle(L"Phone book"); // название заголовка окна
	ConsoleCursorVisible(false, 100); // отключили курсор
	string Menu[] = { "Add","Show","Delete","Edit","Sort","Write","Wish","Exit==Esc" }; // пункты меню
	int active_menu = 0; // пункт для подсветки меню


	while (job)
	{
		int x = 50, y = 12; // размещение меню
		GoToXY(x, y);


		for (int i = 0; i < size(Menu); i++) // вывод меню
		{
			if (i == active_menu)
				// если активное меню - подсветка ярко зеленая
				SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			// иначе просто зеленая
			else SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
			GoToXY(x, y++);
			cout << Menu[i] << endl;
		}

		ch = _getch(); // код нажатой клавиши
		if (ch == -32) ch = _getch(); // "отлов клавиш"
		// -32 для распознавание стрелочек 
		switch (ch)
		{
		case ESC: job = 0; break;
		case UP:
			if (active_menu > 0) // чтобы не "вылететь" за пределы меню
				active_menu--; // если нажимаем стрелочку вверх, то активное меню меняется
			break;
		case DOWN:
			if (active_menu < size(Menu) - 1) // такое же самое ограничение для меню
				active_menu++; // тоже самое, но только для стрелочки вниз
			break;
		case ENTER:
			switch (active_menu)
			{
			case 0:
				system("CLS");
				GoToXY(x, y);
				// желтый цвет
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				cin.ignore();
				Add(mass, size1);
				system("pause");
				system("CLS");
				break;
			case 1:
				system("CLS");
				GoToXY(x, y);
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

				if (size1 == 0) {
					break;
				}
				else {
					do
					{
						cout << "Enter choice:\n1-Show contacts\n2-Read from file\n"; cin >> choice1;
					} while (choice1 < 1 && choice1>2);
					if (choice1 == 1)
						Show(mass, size1);
					else {
						Read(group, size1, filename);
						Show2(group, size1, mass);
					}
				}
				system("pause");
				system("CLS");
				break;
			case 2:
				system("CLS");
				GoToXY(x, y);
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

				if (size1 == 0) {
					break;
				}
				else {
					do
					{
						cout << "Enter number person\n"; cin >> index;
					} while (index<size1 && index>size1);
					DelIndex(mass, size1, index);
				}
				system("pause");
				system("CLS");
				break;
			case 3:system("CLS");
				GoToXY(x, y);
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

				if (size1 == 0) {
					break;
				}
				else {
					do
					{
						cout << "Enter Field: \n1-Name\n2-Number\n3-E-mail\n";
						cin >> SField;
						cout << "Enter index\n";
						cin >> index;

					} while (SField > 4 && SField < 1 && index>size1 && index < size1);

					Edit(mass, size1, SField, index);
				}
				system("pause");
				system("CLS");
				break;
			case 4: system("CLS");
				GoToXY(x, y);
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				if (size1 <= 1)
				{
					break;
				}
				else {
					do
					{
						cout << "\nEnter direct: \n1- A->Z\n2- Z->A\n";
						cin >> direct;
					} while (direct < 1 && direct>2);
					Sort(mass, size1, direct);
				}
				system("pause");
				system("CLS");
				break;
			case 5: system("CLS");
				GoToXY(x, y);
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				if (size1 == 0) {
					break;
				}
				else {

					do
					{
						cout << "Enter filename\n";
						cin.getline(filename, 100);
					} while (!Check(filename));

					Write(mass, size1, filename);
					cout << "Writed successfully\n";
				}
				system("pause");
				system("CLS");
				break;
			case 6: system("CLS");
				GoToXY(x, y);
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				if (size1 == 0)
				{
					break;
				}
				else {
					do
					{
						cout << "Enter index to add wish list\n";
						cin >> index;

					} while (index > size1 && index < size1);
					mass[index].n = 1;
				}
				system("pause");
				system("CLS");
				break;
			}
		}
	}


	for (int i = 0; i < size1; i++)
	{
		delete mass[i].name;
		delete mass[i].num;
		delete mass[i].email;
	}
	delete[] mass;

}