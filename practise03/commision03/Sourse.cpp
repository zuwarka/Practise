#include <iostream>
#include <ctime>
#include <cmath>
#include <Windows.h>
#include "Set_Point.h"

POINT** Create_array(int N) //создаем массив случайных координат точек на плоскости
{
    int i;
    POINT** arr = new POINT * [N];
    for (i = 0; i < N; i++)
    {
        arr[i] = new POINT;
        arr[i]->x = rand() % 300 + 100;
        arr[i]->y = rand() % 100 + 100;
    }
    return arr;
}

void Delete_array(POINT** arr, int N)
{
    for (int i = 0; i < N; i++)
    {
        delete arr[i];
    }
    delete[] arr;
}

void Delete_triangle(Set_Point* t)
{
    delete t->A;
    delete t->B;
    delete t->C;
    delete t;
}

void Delete_all_triangles(Set_Point** tri, int M)
{
    for (int i = 0; i < M; i++)
    {
        Delete_triangle(tri[i]);
    }
    delete[] tri;
}

void Delete_all(POINT** arr, int N, Set_Point** tri, int M, Set_Point* t)
{
    Delete_array(arr, N);
    Delete_all_triangles(tri, M);
    Delete_triangle(t);
}

int Checking_triangle(POINT* A, POINT* B, POINT* C) //проверка, существует ли треугольник для данных точек (по теореме)
{
    return (B->x - A->x) * (C->y - A->y) - (C->x - A->x) * (B->y - A->y);
}

double Length(POINT* A, POINT* B) //измеряем длину стороны для периметра (по теореме)
{
    return sqrt(1. * (A->x - B->x) * (A->x - B->x) + (A->y - B->y) * (A->y - B->y));
}

void Show_array(HDC hDC, POINT** arr, int N, COLORREF color) //ставим пиксели
{
    for (int i = 0; i < N; i++)
    {
        SetPixel(hDC, arr[i]->x, arr[i]->y, color);
        SetPixel(hDC, arr[i]->x - 1, arr[i]->y - 1, color);
        SetPixel(hDC, arr[i]->x + 1, arr[i]->y + 1, color);
        SetPixel(hDC, arr[i]->x - 1, arr[i]->y + 1, color);
        SetPixel(hDC, arr[i]->x + 1, arr[i]->y - 1, color);
    }
}

void Show_triangle(HDC hDC, Set_Point* t) //выводим треугольник
{
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 230, 0)); //для линий треугольника
    SelectObject(hDC, hPen);
    POINT p;
    //функции рисования линий по точкам
    GetCurrentPositionEx(hDC, &p);
    MoveToEx(hDC, t->A->x, t->A->y, &p);
    LineTo(hDC, t->B->x, t->B->y);
    LineTo(hDC, t->C->x, t->C->y);
    LineTo(hDC, t->A->x, t->A->y);
    DeleteObject(hPen);
}

Set_Point* Get_perimeter(Set_Point** tri, int M)
{
    int pos = 0, i = 0;
    double max = 0.;

    for (; i < M; i++)
    {
        if (tri[i]->perimeter > max)
        {
            max = tri[i]->perimeter;
            pos = i;
        }
    }

    Set_Point* found = new Set_Point;
    found->A = new POINT;
    found->B = new POINT;
    found->C = new POINT;

    found->A->x = tri[pos]->A->x; found->A->y = tri[pos]->A->y;
    found->B->x = tri[pos]->B->x; found->B->y = tri[pos]->B->y;
    found->C->x = tri[pos]->C->x; found->C->y = tri[pos]->C->y;
    found->a = tri[pos]->a;
    found->b = tri[pos]->b;
    found->c = tri[pos]->c;
    found->perimeter = tri[pos]->perimeter;

    return found;
}

Set_Point** Create_all_triangle(POINT** arr, int N, int M)
{
    int count = 0, i, j, k;
    Set_Point** tri = new Set_Point * [M];
    for (i = 0; i < M; i++)
    {
        tri[i] = new Set_Point;
        tri[i]->A = new POINT;
        tri[i]->B = new POINT;
        tri[i]->C = new POINT;
    }

    for (i = 0; i < N - 2; i++)
    {
        for (j = i + 1; j < N - 1; j++)
        {
            for (k = j + 1; k < N; k++)
            {
                tri[count]->A->x = arr[i]->x; tri[count]->A->y = arr[i]->y;
                tri[count]->B->x = arr[j]->x; tri[count]->B->y = arr[j]->y;
                tri[count]->C->x = arr[k]->x; tri[count]->C->y = arr[k]->y;

                if (!Checking_triangle(tri[count]->A, tri[count]->B, tri[count]->C))
                {
                    tri[count]->a = tri[count]->b = tri[count]->c = 0.;
                }
                else
                {
                    tri[count]->a = Length(tri[count]->B, tri[count]->C);
                    tri[count]->b = Length(tri[count]->A, tri[count]->C);
                    tri[count]->c = Length(tri[count]->A, tri[count]->B);
                }
                tri[count]->perimeter = tri[count]->a + tri[count]->b + tri[count]->c;
                count++;
            }
        }
    }

    return tri;
}

int main()
{
    int N;
    std::cout << "Input number of dots: ";
    std::cin >> N;

    if (N < 3)
    {
       std:: cout << "Not enough dots for triagle";
       return 0;
    }

    int M = N * (N - 1) * (N - 2) / 6; //считаем количество возможных треугольников по кол-ву точек

    HDC hDC = GetDC(GetConsoleWindow()); //получаем рабочую область "рисования" через винапи
    srand((unsigned int)time(NULL)); //рандомные значения точек

    POINT** points = Create_array(N); //массив указателей структукры из винапи, создаем массив
    Show_array(hDC, points, N, RGB(255, 255, 255)); //выводим белые точки на экран

    Set_Point** tri = Create_all_triangle(points, N, M); //массив указателей на множество точек, делаем треугольник
    Set_Point* found = Get_perimeter(tri, M); //для найденного треугольника считаем периметр
    Show_triangle(hDC, found); //рисуем треугольник

    Delete_all(points, N, tri, M, found); //стираем данные массива после отображения

    getchar(); 
    return 0;
}
