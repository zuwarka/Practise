#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <malloc.h>
#define N 50

struct person {
	char famname[N];
	char sex[N];
	char date[N];
	char city[N];
	char country[N];
	char mark[N];
};

void getFamily(struct person* persons, int index, char* family)
{  //Выдление фамилии из строки ФИО
	int k = 0;
	while (1)
	{

		family[k] = (persons + index)->famname[k];
		if (family[k] == ' ')
		{
			family[k] = '\0';
			break;
		}
		k++;
	}
}

void sorting(struct person* persons, int a) //Сортировка строк методом пузырька
{     //Сортировка массива структур пофамильно
	char family_j1[N];
	char family_j2[N];

	for (int i = 0; i < a - 1; i++)
	{
		for (int j = 0; j < a - i - 1; j++)
		{
			getFamily(persons, j, family_j1);
			getFamily(persons, j + 1, family_j2);

			if (strcmp(family_j1, family_j2) > 0)
			{
				struct person temp = persons[j];
				persons[j] = persons[j + 1];
				persons[j + 1] = temp;
			}
		}
	}
}

void fopencheck(FILE* f)
{   //Проверка открытия файла
	if (!f)
	{
		printf("Ошибка открытия файла!\n");
		exit(1);
	}
}

void nullptrCheck(char* ptr)
{      //Проверка строки на нуль
	if (!ptr)
		exit(1);
}

void fillFile(FILE* F, struct person* persons, int i)
{     //Наполнение файла данными о студенте
	fputs((persons + i)->famname, F);
	fputs((persons + i)->sex, F);
	fputs((persons + i)->date, F);
	fputs((persons + i)->city, F);
	fputs((persons + i)->country, F);
	fputs((persons + i)->mark, F);
	fputs("\n", F);
}

void fillStructPersons(FILE* F, struct person* persons, int nofn, char* str)
{     //Наполнение структуры массивов о студенте
	char* este = NULL;

	este = fgets(str, N, F);
	nullptrCheck(este);
	strcpy(persons[nofn - 1].date, este);

	este = fgets(str, N, F);
	nullptrCheck(este);
	strcpy(persons[nofn - 1].city, este);

	este = fgets(str, N, F);
	nullptrCheck(este);
	strcpy(persons[nofn - 1].country, este);

	este = fgets(str, N, F);
	nullptrCheck(este);
	strcpy(persons[nofn - 1].mark, este);
}


int main()
{
	setlocale(LC_ALL, "Rus");
	int nofn = 0; //переменная строки
	char foundf[] = "Пол: Ж\n";
	char foundm[] = "Пол: М\n";
	char endstr[] = "\n";
	char str[N]; //очередная считываемая строка
	char* este; //указатель для очередной считываемой строки
	char* FIO = NULL; //фио
	char prev_str[N];
	struct person* persons = NULL;
	FILE* F1, * F2, * F3;

	printf("Программа запущена...\n");
	F1 = fopen("F1.txt", "r");
	fopencheck(F1);
	F2 = fopen("F2.txt", "w");
	fopencheck(F2);
	F3 = fopen("F3.txt", "w");
	fopencheck(F3);

	while (!feof(F1))
	{

		FIO = fgets(prev_str, N, F1);
		if (!FIO)
			break;
		if (!strcmp(FIO, endstr))
			continue;
		este = fgets(str, N, F1); //пол
		if (!este)
			break;

		if (!strcmp(este, foundf) || (!strcmp(este, foundm)))//если пол М или  Ж
		{
			++nofn;//увеличиваем строку
			if (persons == NULL)
			{
				persons = (struct person*)malloc(nofn * sizeof(struct person));
				if (!persons)
				{
					printf("Out of memory...");
					exit(1);
				}
				strcpy(persons[nofn - 1].famname, FIO);
				strcpy(persons[nofn - 1].sex, este);
				fillStructPersons(F1, persons, nofn, str);
			}
			else
			{
				struct person* newPersons = (struct person*)malloc(nofn * sizeof(struct person));
				if (!newPersons)
					exit(1);
				for (int i = 0; i < nofn - 1; i++)
					newPersons[i] = persons[i];
				free(persons);
				persons = newPersons;
				strcpy(persons[nofn - 1].famname, FIO);
				strcpy(persons[nofn - 1].sex, este);
				fillStructPersons(F1, persons, nofn, str);
			}
		}
	}

	sorting(persons, nofn);

	for (int i = 0; i < nofn; i++)
	{
		if (!strcmp(persons[i].sex, foundf)) //если женщина
			fillFile(F3, persons, i);
		else
			fillFile(F2, persons, i);
	}

	fclose(F1);
	fclose(F2);
	fclose(F3);
	free(persons);
	printf("\nПрограмма исполнена!\n");
	return 0;
}
