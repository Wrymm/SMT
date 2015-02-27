#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

using namespace std;

ifstream lire;
fstream liresat("out");
ofstream ecrire("formule", ios::out);
ofstream ecriremp("solution.mp", ios::out);

const int SIZE_MAX = 1000;

char t[SIZE_MAX][SIZE_MAX];
int p = 0;
int m = 0;
int d = 0;
int w = 0;
int g = 0;
int l;

int c(int i, int j)
{
	return 4 * (i * l + j) + 1;
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		cout << argv[0] << " <mamap>" << endl;
		return 0;
	}
	lire.open(argv[1], ios::in);
	while (!lire.eof())
	{
		lire.getline(t[p], SIZE_MAX);
		l = 0;
		while (t[p][l] != '\0')
                {
			m += (t[p][l] == 'M');
			d += (t[p][l] == 'D');
			w += (t[p][l] == '%');
			l++;
		}
		p++;
	}
	p--;
	while (t[0][l] != '\0')
		l++;
	for (int i = 0; i < p; i++)
		for (int j = 0; j < l; j++)
			for (int k = 0; k < 4; k++)
				ecrire << "(declare-const c" << c(i, j) + k << " Bool)" << endl;
	for (int i = 0; i < m; i++)
		ecrire << "(declare-const m" << i << " Bool)" << endl;
	for (int i = 0; i < p; i++)
		for (int j = 0; j < l; j++)
			if (t[i][j] == '%')
			{
				ecrire << "(assert (not c" << c(i, j)     << "))" << endl;
				ecrire << "(assert (not c" << c(i, j) + 1 << "))" << endl;
				ecrire << "(assert (not c" << c(i, j) + 2 << "))" << endl;
				ecrire << "(assert (not c" << c(i, j) + 3 << "))" << endl;
			}
			else if (t[i][j] == 'M')
			{
				ecrire << "(assert (or (not m" << g << ") (not c" << c(i, j - 1) + 1 << ") c" << c(i,j)     << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i - 1, j) + 2 << ") c" << c(i,j) + 3 << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i, j + 1) + 3 << ") c" << c(i,j) + 2 << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i + 1, j)     << ") c" << c(i,j) + 1 << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i, j)     << ") c" << c(i, j - 1) + 1 << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i, j) + 3 << ") c" << c(i - 1, j) + 2 << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i, j) + 2 << ") c" << c(i, j + 1) + 3 << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i, j) + 1 << ") c" << c(i + 1, j)     << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i, j - 1) + 1 << ") c" << c(i,j) + 2 << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i - 1, j) + 2 << ") c" << c(i,j) + 1 << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i, j + 1) + 3 << ") c" << c(i,j)     << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i + 1, j)     << ") c" << c(i,j) + 3 << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i, j) + 2 << ") c" << c(i, j - 1) + 1 << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i, j) + 1 << ") c" << c(i - 1, j) + 2 << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i, j)     << ") c" << c(i, j + 1) + 3 << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i, j) + 3 << ") c" << c(i + 1, j)     << "))" << endl;
				g++;
			}
			else if (t[i][j] == 'D' or t[i][j] == ' ')
                        {
				ecrire << "(assert (or (not c" << c(i + 1, j)     << ") c" << c(i, j)     << "))" << endl;
				ecrire << "(assert (or (not c" << c(i, j - 1) + 1 << ") c" << c(i, j) + 1 << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i - 1, j) + 2 << ") c" << c(i, j) + 2 << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j + 1) + 3 << ") c" << c(i, j) + 3 << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j)     << ") c" << c(i + 1, j)     << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j) + 1 << ") c" << c(i, j - 1) + 1 << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j) + 2 << ") c" << c(i - 1, j) + 2 << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j) + 3 << ") c" << c(i, j + 1) + 3 << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j)     << ") c" << c(i, j) + 2 << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j) + 1 << ") c" << c(i, j) + 3 << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j) + 2 << ") c" << c(i, j)     << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j) + 3 << ") c" << c(i, j) + 1 << "))" << endl;
				if (t[i][j] == 'D')
					ecrire << "(assert (or c" << c(i, j) << " c" << c(i, j) + 1 << "))" << endl;
                        }
			else
			{
				if (j == 0)
					ecrire << "(assert c" << c(i, j) + 1 << ")" << endl;
				else if (j == l - 1)
                                        ecrire << "(assert c" << c(i, j) + 3 << ")" << endl;
                                else if (i == 0)
                                        ecrire << "(assert c" << c(i, j) + 2 << ")" << endl;
				else
                                        ecrire << "(assert c" << c(i, j) << ")" << endl;
			}
	ecrire << "(check-sat)"<< endl << "(get-model)" << endl;
	system("z3 -smt2 formule > out");
	char s[1000];
	liresat.getline(s, 1000);
	if (s[0] == 'u')
	{
		cout << "No Answer..." << endl;
		return 0;
	}
	liresat.getline(s, 1000);
	bool valM[1000];
	while (g > 0)
	{
		char str[1000];
		liresat.getline(str, 1000);
		while (str[14] != 'm')
		{
			liresat.getline(str, 1000);
			liresat.getline(str, 1000);
		}
		g--;
		int num = 0;
		int i = 15;
		while (str[i] != ' ')
		{
			num = num * 10 + str[i] - '0';
			i++;
		}
		liresat.getline(str, 1000);
		if (str[4] == 'f')
			valM[num] = false;
		else
			valM[num] = true;
	}
	for (int i = 0; i < p; i++)
                for (int j = 0; j < l; j++)
			if (t[i][j] == 'M')
			{
				if (valM[g])
					t[i][j] = '/';
				else
					t[i][j] = '\\';
				g++;
			}
	for (int i = 0; i < p; i++)
	{
                for (int j = 0; j < l; j++)
			cout << t[i][j];
		cout << endl;
	}
	ecriremp << "input reflexion" << endl;
	int si, sj, ei, ej;
	for (int i = 0; i < p; i++)
                for (int j = 0; j < l; j++)
			if (t[i][j] == '%')
				ecriremp << "wall(" << j << "," << i << ");" << endl;
			else if (t[i][j] == 'S')
			{
				si = i;
				sj = j;
                                ecriremp << "start(" << j << "," << i << ");" << endl;
			}
                        else if (t[i][j] == 'E')
			{
				ei = i;
				ej = j;
                                ecriremp << "target(" << j << "," << i << ");" << endl;
			}
                        else if (t[i][j] == 'D')
                                ecriremp << "diamond(" << j << "," << i << ");" << endl;
                        else if (t[i][j] == '/' or t[i][j] == '\\')
                                ecriremp << "mirror(" << j << "," << i << ",\"" << t[i][j] << "\");" << endl;
	int pi = si, pj = sj;
	int di = 0, dj = 0;
	if (si == 0)
		di = 1;
	if (si == p - 1)
		di = -1;
	if (sj == 0)
		dj = 1;
	if (sj == l - 1)
		dj = -1;
	ecriremp << "move_ball((" << pj << "," << pi << ")";
	while (pi != ei or pj != ej)
	{
		if (pi < 0 or pj < 0 or pi >= p or pj >= l)
			return 0;
		if (t[pi][pj] == '/')
		{
			ecriremp << "--(" << pj << "," << pi << ")";
			if (di != 0)
			{
				dj = -di;
				di = 0;
			}
			else
			{
				di = -dj;
				dj = 0;
			}
		}
                if (t[pi][pj] == '\\')
                {
			ecriremp << "--(" << pj << "," << pi << ")";
                        if (di != 0)
                        {
                                dj = di;
                                di = 0;
                        }
                        else
                        {
                                di = dj;
                                dj = 0;
                        }
                }
		pi += di;
		pj += dj;
	}
	ecriremp << "--(" << pj << "," << pi << "));";
	ecriremp << "draw_all;" << endl << "end.";
	ecriremp.close();
	system("mptopdf solution.mp");
	return 0;
}
