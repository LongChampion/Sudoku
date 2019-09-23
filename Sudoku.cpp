#include <iostream>
#include <set>
#include <vector>
using namespace std;

vector<vector<bool>> Row(9, vector<bool>(10, false)), Col(9, vector<bool>(10, false));
vector<vector<vector<bool>>> Area(3, vector<vector<bool>>(3, vector<bool>(10, false)));
vector<vector<long>> Table(9, vector<long>(9, 0));
vector<vector<bool>> Locked(9, vector<bool>(9, false));
vector<vector<long>> Solution(9, vector<long>(9, 0));
vector<vector<set<long>>> Candidate(9, vector<set<long>>(9));
vector<vector<string>> Reason(9, vector<string>(9));
long NUMBER_OF_SOLUTION = 0;
long NUMBER_OF_FILLED = 0;

void Initilize();
bool ReadData();
void BruteForce(long = 0, long = 0);
void ShowTable(long = -1, long = -1);
void ShowSolution();
void ShowCandidate();
void Finalize();

void Fill(long, long, long);
bool ScanByCell(long &, long &, long &);
bool ScanByRow(long &, long &, long &);
bool ScanByCol(long &, long &, long &);
bool ScanByArea(long &, long &, long &);

void OptimizeCandidate();
bool OptimizeByRow();
bool OptimizeByCol();
bool OptimizeByArea();

int main()
{
    Initilize();

    if (ReadData())
    {
        ShowTable();

        cout << endl
             << "There is the solution:" << endl;
        ShowSolution();

        cout << endl
             << "Let explain why we have that solution!" << endl;

        while (NUMBER_OF_FILLED < 81)
        {
            long x, y, k;

            OptimizeCandidate();

            if (ScanByCell(x, y, k))
            {
                Fill(x, y, k);
                ShowTable(x, y);
                cout << '[' << x + 1 << ", " << y + 1 << "] = " << k << "\t";
                cout << "Can't fill other number in this cell!" << endl;
                if (!Reason[x][y].empty()) cout << Reason[x][y] << endl;
                continue;
            }

            if (ScanByRow(x, y, k))
            {
                Fill(x, y, k);
                ShowTable(x, y);
                cout << '[' << x + 1 << ", " << y + 1 << "] = " << k << "\t";
                cout << "Can't fill number " << k << " in other cells in row " << x + 1 << '.' << endl;
                if (!Reason[x][y].empty()) cout << Reason[x][y] << endl;
                continue;
            }

            if (ScanByCol(x, y, k))
            {
                Fill(x, y, k);
                ShowTable(x, y);
                cout << '[' << x + 1 << ", " << y + 1 << "] = " << k << "\t";
                cout << "Can't fill number " << k << " in other cells in column " << y + 1 << '.' << endl;
                if (!Reason[x][y].empty()) cout << Reason[x][y] << endl;
                continue;
            }

            if (ScanByArea(x, y, k))
            {
                Fill(x, y, k);
                ShowTable(x, y);
                cout << '[' << x + 1 << ", " << y + 1 << "] = " << k << "\t";
                cout << "Can't fill number " << k << " in other cells in area " << (x / 3) * 3 + y / 3 + 1 << '.' << endl;
                continue;
            }

            cout << "Oh shit! I have no ideal now. Try again next time." << endl;
            ShowCandidate();
            break;
        }
    }

    if (NUMBER_OF_FILLED == 81)
        cout << "Congrat, you have solved a sudoku!" << endl;

    Finalize();
}

void Initilize()
{
    for (long i = 0; i < 9; ++i)
        for (long j = 0; j < 9; ++j)
            for (long k = 1; k <= 9; ++k)
                Candidate[i][j].insert(k);
}

bool ReadData()
{
    long rx, ry, Temp;

    for (long i = 0; i < 9; ++i)
        for (long j = 0; j < 9; ++j)
        {
            cin >> Temp;

            if (Temp == 0) continue;

            if (Temp < 0 || Temp > 9)
            {
                cout << "Invalid input at position [" << i + 1 << ", " << j + 1 << ']' << endl;
                return false;
            }

            if (Row[i][Temp])
            {
                cout << "Invalid input at position [" << i + 1 << ", " << j + 1 << ']' << endl;
                return false;
            }

            if (Col[j][Temp])
            {
                cout << "Invalid input at position [" << i + 1 << ", " << j + 1 << ']' << endl;
                return false;
            }

            rx = (i / 3) * 3;
            ry = (j / 3) * 3;
            for (long dx = 0; dx < 3; ++dx)
                for (long dy = 0; dy < 3; ++dy)
                    if (Temp == Table[rx + dx][ry + dy])
                    {
                        cout << "Invalid input at position [" << i + 1 << ", " << j + 1 << ']' << endl;
                        return false;
                    }

            Fill(i, j, Temp);
            Row[i][Table[i][j]] = Col[j][Table[i][j]] = Area[i / 3][j / 3][Table[i][j]] = true;
        }

    BruteForce();
    if (NUMBER_OF_SOLUTION > 1)
        cout << "More than one solution found!" << endl;
    else if (NUMBER_OF_SOLUTION == 0)
        cout << "No solution found!" << endl;

    return NUMBER_OF_SOLUTION == 1;
}

void BruteForce(long x, long y)
{
    if (NUMBER_OF_SOLUTION > 1) return;

    if (y >= 9) BruteForce(x + 1, 0);

    if (x >= 9)
    {
        ++NUMBER_OF_SOLUTION;
        if (NUMBER_OF_SOLUTION == 1)
            Solution = Table;
        return;
    }

    if (!Locked[x][y])
    {
        for (long k = 1; k < 10; ++k)
        {
            if (Row[x][k] || Col[y][k] || Area[x / 3][y / 3][k]) continue;
            Table[x][y] = k;
            Row[x][k] = Col[y][k] = Area[x / 3][y / 3][k] = true;
            BruteForce(x, y + 1);
            Table[x][y] = 0;
            Row[x][k] = Col[y][k] = Area[x / 3][y / 3][k] = false;
        }
    }
    else
        BruteForce(x, y + 1);
}

void ShowTable(long x, long y)
{
    for (long i = 0; i < 9; ++i)
    {
        if (i % 3 == 0)
            cout << "\e[1;34m+---+---+---+---+---+---+---+---+---+\e[0m" << endl;
        else
            cout << "\e[1;34m+\e[0;34m---+---+---\e[1;34m+\e[0;34m---+---+---\e[1;34m+\e[0;34m---+---+---\e[1;34m+\e[0m" << endl;

        for (long j = 0; j < 9; ++j)
        {
            cout << (j % 3 == 0 ? "\e[1;34m| " : "\e[0;34m| ");
            if (Locked[i][j])
            {
                if (i == x && j == y)
                    cout << "\e[1;31m" << Table[i][j] << "\e[0m ";
                else
                    cout << "\e[1;32m" << Table[i][j] << "\e[0m ";
            }
            else
                cout << "\e[1;32m  \e[0m";
        }

        cout << "\e[1;34m|\e[0m" << endl;
    }

    cout << "\e[1;34m+---+---+---+---+---+---+---+---+---+\e[0m" << endl;
}

void ShowSolution()
{
    for (long i = 0; i < 9; ++i)
    {
        if (i % 3 == 0)
            cout << "\e[1;34m+---+---+---+---+---+---+---+---+---+\e[0m" << endl;
        else
            cout << "\e[1;34m+\e[0;34m---+---+---\e[1;34m+\e[0;34m---+---+---\e[1;34m+\e[0;34m---+---+---\e[1;34m+\e[0m" << endl;

        for (long j = 0; j < 9; ++j)
            cout << (j % 3 == 0 ? "\e[1;34m| " : "\e[0;34m| ") << (Locked[i][j] ? "\e[1;32m" : "\e[1;35m") << Solution[i][j] << "\e[0m ";
        cout << "\e[1;34m|\e[0m" << endl;
    }

    cout << "\e[1;34m+---+---+---+---+---+---+---+---+---+\e[0m" << endl;
}

void ShowCandidate()
{
    for (long row = 0; row < 9; ++row)
    {
        if (row % 3 == 0)
            cout << "+-----------------------+-----------------------+-----------------------+" << endl;
        else
            cout << "|                       |                       |                       |" << endl;


        for (long i = 0; i < 3; ++i)
        {
            for (long col = 0; col < 9; ++col)
            {
                if (col % 3 == 0)
                    cout << "| ";
                else
                    cout << "  ";
                for (long j = 0; j < 3; ++j)
                    if (Candidate[row][col].find(3 * i + j + 1) != Candidate[row][col].end())
                        cout << 3 * i + j + 1 << ' ';
                    else
                        cout << "  ";
            }
            cout << '|' << endl;
        }
    }
    cout << "+-----------------------+-----------------------+-----------------------+" << endl;
}

void Finalize()
{
}

void Fill(long x, long y, long key)
{
    if (Locked[x][y])
    {
        cout << "Can't fill locked cell" << endl;
        return;
    }

    if (key != Solution[x][y] && Solution[x][y] != 0)
    {
        cout << "FATAL ERROR: Wrong filling at [" << x + 1 << ", " << y + 1 << ']' << endl;
        ShowCandidate();
    }

    Table[x][y] = key;
    Locked[x][y] = true;
    Candidate[x][y].clear();
    ++NUMBER_OF_FILLED;

    for (long k = 0; k < 9; ++k)
    {
        Candidate[x][k].erase(key);
        Candidate[k][y].erase(key);
    }

    long rx = (x / 3) * 3, ry = (y / 3) * 3;
    for (long i = 0; i < 3; ++i)
        for (long j = 0; j < 3; ++j)
            Candidate[rx + i][ry + j].erase(key);
}

bool ScanByCell(long &x, long &y, long &k)
{
    for (long i = 0; i < 9; ++i)
        for (long j = 0; j < 9; ++j)
            if (Candidate[i][j].size() == 1)
            {
                x = i;
                y = j;
                k = *Candidate[i][j].begin();
                return true;
            }
    return false;
}

bool ScanByRow(long &x, long &y, long &k)
{
    for (long key = 1; key <= 9; ++key)
    {
        for (long row = 0; row < 9; ++row)
        {
            long col = -1;
            for (long j = 0; j < 9; ++j)
                if (Candidate[row][j].find(key) != Candidate[row][j].end())
                    if (col == -1)
                        col = j;
                    else
                        col = -2;
            if (col >= 0)
            {
                x = row;
                y = col;
                k = key;
                return true;
            }
        }
    }
    return false;
}

bool ScanByCol(long &x, long &y, long &k)
{
    for (long key = 1; key <= 9; ++key)
    {
        for (long col = 0; col < 9; ++col)
        {
            long row = -1;
            for (long i = 0; i < 9; ++i)
                if (Candidate[i][col].find(key) != Candidate[i][col].end())
                    if (row == -1)
                        row = i;
                    else
                        row = -2;
            if (row >= 0)
            {
                x = row;
                y = col;
                k = key;
                return true;
            }
        }
    }
    return false;
}

bool ScanByArea(long &x, long &y, long &k)
{
    for (long key = 1; key <= 9; ++key)
    {
        for (long rx = 0; rx < 9; rx += 3)
            for (long ry = 0; ry < 9; ry += 3)
            {
                long count = 0;
                for (long i = 0; i < 3; ++i)
                    for (long j = 0; j < 3; ++j)
                        if (Candidate[rx + i][ry + j].find(key) != Candidate[rx + i][ry + j].end())
                        {
                            ++count;
                            if (count == 1)
                            {
                                x = rx + i;
                                y = ry + j;
                                k = key;
                            }
                        }
                if (count == 1) return true;
            }
    }
    return false;
}

void OptimizeCandidate()
{
    while (true)
    {
        if (OptimizeByRow()) continue;
        if (OptimizeByCol()) continue;
        if (OptimizeByArea()) continue;
        break;
    }
}

bool OptimizeByRow()
{
    for (long key = 1; key <= 9; ++key)
    {
        for (long row = 0; row < 9; ++row)
        {
            vector<long> col;
            for (long j = 0; j < 9; ++j)
                if (Candidate[row][j].find(key) != Candidate[row][j].end())
                    col.push_back(j);
            if (col.size() > 1 && col.size() < 4 && col.back() - col[0] < 3 && col.back() / 3 == col[0] / 3)
            {
                long rx = (row / 3) * 3;
                long ry = (col[0] / 3) * 3;
                long count = 0;

                for (long i = 0; i < 3; ++i)
                {
                    if (rx + i == row) continue;
                    for (long j = 0; j < 3; ++j)
                        if (Candidate[rx + i][ry + j].find(key) != Candidate[rx + i][ry + j].end())
                        {
                            Candidate[rx + i][ry + j].erase(key);
                            Reason[rx + i][ry + j] += "Optimize by row " + to_string(row + 1) + " with number " + to_string(key) + ". ";
                            ++count;
                        }
                }

                if (count > 0) return true;
            }
        }
    }
    return false;
}

bool OptimizeByCol()
{
    for (long key = 1; key <= 9; ++key)
    {
        for (long col = 0; col < 9; ++col)
        {
            vector<long> row;
            for (long i = 0; i < 9; ++i)
                if (Candidate[i][col].find(key) != Candidate[i][col].end())
                    row.push_back(i);
            if (row.size() > 1 && row.size() < 4 && row.back() - row[0] < 3 && row.back() / 3 == row[0] / 3)
            {
                long rx = (row[0] / 3) * 3;
                long ry = (col / 3) * 3;
                long count = 0;

                for (long j = 0; j < 3; ++j)
                {
                    if (ry + j == col) continue;
                    for (long i = 0; i < 3; ++i)
                        if (Candidate[rx + i][ry + j].find(key) != Candidate[rx + i][ry + j].end())
                        {
                            Candidate[rx + i][ry + j].erase(key);
                            Reason[rx + i][ry + j] += "Optimize by column " + to_string(col + 1) + " with number " + to_string(key) + ". ";
                            ++count;
                        }
                }

                if (count > 0) return true;
            }
        }
    }
    return false;
}

bool OptimizeByArea()
{
    for (long key = 1; key <= 9; ++key)
    {
        for (long rx = 0; rx < 9; rx += 3)
            for (long ry = 0; ry < 9; ry += 3)
            {
                vector<pair<long, long>> A;
                long row = -1, col = -1, count = 0;

                for (long i = 0; i < 3; ++i)
                    for (long j = 3; j < 3; ++j)
                        if (Candidate[rx + i][ry + j].find(key) != Candidate[rx + i][ry + j].end())
                            A.push_back({rx + i, ry + j});

                if (A.size() < 2 || A.size() > 3) continue;

                if (A[0].first == A[1].first)
                    row = A[0].first;
                else if (A[0].second == A[1].second)
                    col = A[0].second;
                else
                    continue;

                if (A.size() == 3)
                {
                    if (row >= 0 && A[2].first != row) continue;
                    if (col >= 0 && A[2].second != col) continue;
                }

                if (row >= 0)
                {
                    for (long j = 0; j < 9; ++j)
                    {
                        if (j / 3 == ry / 3)
                            for (pair<long, long> &X : A)
                                if (j == X.second) continue;

                        Candidate[row][j].erase(key);
                        Reason[row][j] += "Optimize by area " + to_string(rx + ry / 3 + 1) + " with number " + to_string(key) + ". ";
                        ++count;
                    }
                }
                else
                {
                    for (long i = 0; i < 9; ++i)
                    {
                        if (i / 3 == rx / 3)
                            for (pair<long, long> &X : A)
                                if (i == X.first) continue;

                        Candidate[i][col].erase(key);
                        Reason[i][col] += "Optimize by area " + to_string(rx + ry / 3 + 1) + " with number " + to_string(key) + ". ";
                        ++count;
                    }
                }

                if (count > 0) return true;
            }
    }
    return false;
}