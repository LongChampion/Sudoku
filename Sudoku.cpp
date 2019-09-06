#include <iostream>

bool **Row, **Col, ***Area, **Locked;
long **Table;
long Counter = 0;

void Initialization();
bool ReadData();
void Solve(long = 0, long = 0);
void ShowTable();
void Finalization();

int main()
{
    Initialization();
    if (ReadData()) Solve();
    Finalization();
}

void Initialization()
{
    Row = new bool *[9];
    for (long i = 0; i < 9; ++i)
    {
        Row[i] = new bool[10];
        for (long j = 0; j < 10; ++j) Row[i][j] = false;
    }

    Col = new bool *[9];
    for (long i = 0; i < 9; ++i)
    {
        Col[i] = new bool[10];
        for (long j = 0; j < 10; ++j) Col[i][j] = false;
    }

    Area = new bool **[3];
    for (long i = 0; i < 3; ++i)
    {
        Area[i] = new bool *[3];
        for (long j = 0; j < 3; ++j)
        {
            Area[i][j] = new bool[10];
            for (long k = 0; k < 10; ++k) Area[i][j][k] = false;
        }
    }

    Locked = new bool *[9];
    for (long i = 0; i < 9; ++i)
    {
        Locked[i] = new bool[9];
        for (long j = 0; j < 9; ++j)
            Locked[i][j] = false;
    }

    Table = new long *[9];
    for (long i = 0; i < 9; ++i)
    {
        Table[i] = new long[9];
        for (long j = 0; j < 9; ++j)
            Table[i][j] = false;
    }
}

bool ReadData()
{
    long rx, ry;

    for (long i = 0; i < 9; ++i)
        for (long j = 0; j < 9; ++j)
            std::cin >> Table[i][j];

    for (long i = 0; i < 9; ++i)
        for (long j = 0; j < 9; ++j)
        {
            if (Table[i][j] == 0) continue;

            if (Table[i][j] < 0 || Table[i][j] > 9)
            {
                std::cout << "1: Invalid input at position [" << i + 1 << ", " << j + 1 << ']' << std::endl;
                return false;
            }

            if (Row[i][Table[i][j]])
            {
                std::cout << "2: Invalid input at position [" << i + 1 << ", " << j + 1 << ']' << std::endl;
                return false;
            }

            if (Col[j][Table[i][j]])
            {
                std::cout << "3: Invalid input at position [" << i + 1 << ", " << j + 1 << ']' << std::endl;
                return false;
            }

            rx = (i / 3) * 3;
            ry = (j / 3) * 3;
            for (long dx = 0; dx < 3; ++dx)
            {
                for (long dy = 0; dy < 3; ++dy)
                    if ((rx + dx != i || ry + dy != j) && Table[i][j] == Table[rx + dx][ry + dy])
                    {
                        std::cout << "4: Invalid input at position [" << i + 1 << ", " << j + 1 << ']' << std::endl;
                        return false;
                    }
            }

            Locked[i][j] = true;
            Row[i][Table[i][j]] = Col[j][Table[i][j]] = Area[i / 3][j / 3][Table[i][j]] = true;
        }

    return true;
}

void Solve(long x, long y)
{
    if (y >= 9)
        Solve(x + 1, 0);

    if (x >= 9)
    {
        ShowTable();
        return;
    }

    if (!Locked[x][y])
    {
        for (long k = 1; k < 10; ++k)
        {
            if (Row[x][k] || Col[y][k] || Area[x / 3][y / 3][k]) continue;
            Table[x][y] = k;
            Row[x][k] = Col[y][k] = Area[x / 3][y / 3][k] = true;
            Solve(x, y + 1);
            Table[x][y] = 0;
            Row[x][k] = Col[y][k] = Area[x / 3][y / 3][k] = false;
        }
    }
    else
        Solve(x, y + 1);
}

void ShowTable()
{
    long Space = 14, Temp = ++Counter;
    while (Temp > 99)
    {
        --Space;
        Temp /= 100;
    }

    std::cout << std::endl;
    while (--Space) std::cout << ' ';
    std::cout << "\e[1;33mSolution " << Counter << "\e[0m" << std::endl;

    for (long i = 0; i < 9; ++i)
    {
        if (i % 3 == 0)
            std::cout << "\e[1;34m+---+---+---+---+---+---+---+---+---+\e[0m" << std::endl;
        else
            std::cout << "\e[1;34m+\e[0;34m---+---+---\e[1;34m+\e[0;34m---+---+---\e[1;34m+\e[0;34m---+---+---\e[1;34m+\e[0m" << std::endl;


        for (long j = 0; j < 9; ++j)
            std::cout << (j % 3 == 0 ? "\e[1;34m| " : "\e[0;34m| ") << (Locked[i][j] ? "\e[1;34m" : "\e[1;32m") << Table[i][j] << "\e[0m ";
        std::cout << "\e[1;34m|\e[0m" << std::endl;
    }

    std::cout << "\e[1;34m+---+---+---+---+---+---+---+---+---+\e[0m" << std::endl;
}

void Finalization()
{
    for (long i = 0; i < 9; ++i)
        delete[] Row[i];
    delete[] Row;

    for (long i = 0; i < 9; ++i)
        delete[] Col[i];
    delete[] Col;

    for (long i = 0; i < 3; ++i)
    {
        for (long j = 0; j < 3; ++j)
            delete[] Area[i][j];
        delete[] Area[i];
    }
    delete[] Area;

    for (long i = 0; i < 9; ++i)
        delete[] Locked[i];
    delete[] Locked;

    for (long i = 0; i < 9; ++i)
        delete[] Table[i];
    delete[] Table;
}