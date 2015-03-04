#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

using namespace std;

ifstream lire;                                  // Map
ifstream liresat("out", ios::in);               // Retour de SAT
ofstream ecrire("formule", ios::out);           // Formule générée
ofstream ecriremp("solution.mp", ios::out);     // Jolie sortie

const int SIZE_MAX = 1000;      // Largeur de la grille

char t[SIZE_MAX][SIZE_MAX];     // Grille
int p = 0;                      // Profondeur de la grille
int l;                          // Largeur de la grille
int m = 0;                      // Nombre de miroirs
int d = 0;                      // Nombre de diamants
int w = 0;                      // Nombre de murs
int g = 0;                      // Compteur de miroirs
int h = 0;			// Compteur de diamants

// Numéro d'une case
int c(int i, int j)
{
	return 4 * (i * l + j) + 1;
}

int main(int argc, char** argv)
{
	// Paramètres
	if (argc == 1)
	{
		cout << argv[0] << " <mamap>" << endl;
		return 0;
	}
	lire.open(argv[1], ios::in);
	// Lecture
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
	// Variables directionnelles
	for (int i = 0; i < p; i++)
		for (int j = 0; j < l; j++)
			for (int k = 0; k < 4; k++)
				ecrire << "(declare-const c" << c(i, j) + k << " Bool)" << endl;
	// Variables d'orientation de miroirs
	for (int i = 0; i < m; i++)
		ecrire << "(declare-const m" << i << " Bool)" << endl;
	// Variables de double passage par diamants
	for (int i = 0; i < d; i++)
                ecrire << "(declare-const u" << i << " Int)" << endl;
	// Variables de nombre de diamants rencontrés
	for (int i = 0; i < p; i++)
                for (int j = 0; j < l; j++)
                        for (int k = 0; k < 4; k++)
                                ecrire << "(declare-const d" << c(i, j) + k << " Int)" << endl;
	for (int i = 0; i < p; i++)
		for (int j = 0; j < l; j++)
			if (t[i][j] == '%')
			{
				// Murs inaccessibles
				ecrire << "(assert (not c" << c(i, j)     << "))" << endl;
				ecrire << "(assert (not c" << c(i, j) + 1 << "))" << endl;
				ecrire << "(assert (not c" << c(i, j) + 2 << "))" << endl;
				ecrire << "(assert (not c" << c(i, j) + 3 << "))" << endl;
			}
			else if (t[i][j] == 'M')
			{
				// Orientation 1 - propagation de la direction :
				ecrire << "(assert (or (not m" << g << ") (not c" << c(i, j - 1) + 1 << ") c" << c(i,j)     << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i - 1, j) + 2 << ") c" << c(i,j) + 3 << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i, j + 1) + 3 << ") c" << c(i,j) + 2 << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i + 1, j)     << ") c" << c(i,j) + 1 << "))" << endl;
				ecrire << "(assert (or (not m" << g << ") (not c" << c(i, j)     << ") c" << c(i, j - 1) + 1 << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i, j) + 3 << ") c" << c(i - 1, j) + 2 << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i, j) + 2 << ") c" << c(i, j + 1) + 3 << "))" << endl;
                                ecrire << "(assert (or (not m" << g << ") (not c" << c(i, j) + 1 << ") c" << c(i + 1, j)     << "))" << endl;
				// Orientation 2 - propagation de la direction :
                                ecrire << "(assert (or m" << g << " (not c" << c(i, j - 1) + 1 << ") c" << c(i,j) + 2 << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i - 1, j) + 2 << ") c" << c(i,j) + 1 << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i, j + 1) + 3 << ") c" << c(i,j)     << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i + 1, j)     << ") c" << c(i,j) + 3 << "))" << endl;
				ecrire << "(assert (or m" << g << " (not c" << c(i, j) + 2 << ") c" << c(i, j - 1) + 1 << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i, j) + 1 << ") c" << c(i - 1, j) + 2 << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i, j)     << ") c" << c(i, j + 1) + 3 << "))" << endl;
                                ecrire << "(assert (or m" << g << " (not c" << c(i, j) + 3 << ") c" << c(i + 1, j)     << "))" << endl;
				// Orientation 1 - conservation du nombre de diamants :
				ecrire << "(assert (or (not m" << g << ") (or (not c" << c(i, j)     << ") (= d" << c(i, j)     << " d" << c(i, j - 1) + 1 << "))))" << endl;
				ecrire << "(assert (or (not m" << g << ") (or (not c" << c(i, j) + 1 << ") (= d" << c(i, j) + 1 << " d" << c(i + 1, j)     << "))))" << endl;
				ecrire << "(assert (or (not m" << g << ") (or (not c" << c(i, j) + 2 << ") (= d" << c(i, j) + 2 << " d" << c(i, j + 1) + 3 << "))))" << endl;
				ecrire << "(assert (or (not m" << g << ") (or (not c" << c(i, j) + 3 << ") (= d" << c(i, j) + 3 << " d" << c(i - 1, j) + 2 << "))))" << endl;
				// Orientation 2 - conservation du nombre de diamants :
				ecrire << "(assert (or m" << g << " (or (not c" << c(i, j)     << ") (= d" << c(i, j)     << " d" << c(i, j + 1) + 3 << "))))" << endl;
                                ecrire << "(assert (or m" << g << " (or (not c" << c(i, j) + 1 << ") (= d" << c(i, j) + 1 << " d" << c(i - 1, j) + 2 << "))))" << endl;
                                ecrire << "(assert (or m" << g << " (or (not c" << c(i, j) + 2 << ") (= d" << c(i, j) + 2 << " d" << c(i, j - 1) + 1 << "))))" << endl;
                                ecrire << "(assert (or m" << g << " (or (not c" << c(i, j) + 3 << ") (= d" << c(i, j) + 3 << " d" << c(i+ 1, j)      << "))))" << endl;
				g++;
			}
			else if (t[i][j] == 'D' or t[i][j] == ' ')
                        {
				// Propagation de la direction :
				ecrire << "(assert (or (not c" << c(i + 1, j)     << ") c" << c(i, j)     << "))" << endl;
				ecrire << "(assert (or (not c" << c(i, j - 1) + 1 << ") c" << c(i, j) + 1 << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i - 1, j) + 2 << ") c" << c(i, j) + 2 << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j + 1) + 3 << ") c" << c(i, j) + 3 << "))" << endl;
				ecrire << "(assert (or (not c" << c(i, j)     << ") c" << c(i + 1, j)     << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j) + 1 << ") c" << c(i, j - 1) + 1 << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j) + 2 << ") c" << c(i - 1, j) + 2 << "))" << endl;
                                ecrire << "(assert (or (not c" << c(i, j) + 3 << ") c" << c(i, j + 1) + 3 << "))" << endl;
				if (t[i][j] == 'D')
                                {
					// Incrémentation du nombre de diamants rencontrés :
                                        ecrire << "(assert (or (not c" << c(i, j)     << ") (= d" << c(i, j)     << " (+ d" << c(i + 1, j)     << " 1))))" << endl;
					ecrire << "(assert (or (not c" << c(i, j) + 1 << ") (= d" << c(i, j) + 1 << " (+ d" << c(i, j - 1) + 1 << " 1))))" << endl;
					ecrire << "(assert (or (not c" << c(i, j) + 2 << ") (= d" << c(i, j) + 2 << " (+ d" << c(i - 1, j) + 2 << " 1))))" << endl;
					ecrire << "(assert (or (not c" << c(i, j) + 3 << ") (= d" << c(i, j) + 3 << " (+ d" << c(i, j + 1) + 3 << " 1))))" << endl;
					// Double passage :
					ecrire << "(assert (or (not (and (or c" << c(i, j) << " c" << c(i, j) + 2 << ") (or c" << c(i, j) + 1 << " c" << c(i, j) + 3 << "))) (= u" << h << " 1)))" << endl;
					ecrire << "(assert (or (not (= u" << h << " 1)) (and (or c" << c(i, j) << " c" << c(i, j) + 2 << ") (or c" << c(i, j) + 1 << " c" << c(i, j) + 3 << "))))" << endl;
					ecrire << "(assert (or (= u" << h << " 1) (= u" << h << " 0)))" << endl;
					h++;
                                }
                                else
                                {
					// Conservation du nombre de diamants :
                                        ecrire << "(assert (or (not c" << c(i, j)     << ") (= d" << c(i, j)     << " d" << c(i + 1, j)     << ")))" << endl;
                                        ecrire << "(assert (or (not c" << c(i, j) + 1 << ") (= d" << c(i, j) + 1 << " d" << c(i, j - 1) + 1 << ")))" << endl;
					ecrire << "(assert (or (not c" << c(i, j) + 2 << ") (= d" << c(i, j) + 2 << " d" << c(i - 1, j) + 2 << ")))" << endl;
					ecrire << "(assert (or (not c" << c(i, j) + 3 << ") (= d" << c(i, j) + 3 << " d" << c(i, j + 1) + 3 << ")))" << endl;
                                }
                        }
                        else if (t[i][j] == 'S')
			{
				// Direction et nombre de diamant de départ
				if (j == 0)
				{
                                       	ecrire << "(assert c" << c(i, j) + 1 << ")" << endl;
					ecrire << "(assert (= d" << c(i, j) + 1 << " 0))" << endl;
				}
                                else if (j == l - 1)
				{
                                        ecrire << "(assert c" << c(i, j) + 3 << ")" << endl;
					ecrire << "(assert (= d" << c(i, j) + 3 << " 0))" << endl;
				}
                                else if (i == 0)
				{
                                        ecrire << "(assert c" << c(i, j) + 2 << ")" << endl;
					ecrire << "(assert (= d" << c(i, j) + 2 << " 0))" << endl;
	                        }
				else
				{
                                       	ecrire << "(assert c" << c(i, j) << ")" << endl;
					ecrire << "(assert (= d" << c(i, j) << " 0))" << endl;
				}
			}
                        else
			{
				// Direction et nombre de diamant de fin
				if (j == 0)
				{
					ecrire << "(assert c" << c(i, j + 1) + 3 << ")" << endl;
                                        ecrire << "(assert (= d" << c(i, j + 1) + 3 << " ";
				}
                                else if (j == l - 1)
				{
					ecrire << "(assert c" << c(i, j - 1) + 1 << ")" << endl;
                                        ecrire << "(assert (= d" << c(i, j - 1) + 1 << " ";
				}
                                else if (i == 0)
				{
					ecrire << "(assert c" << c(i + 1, j) << ")" << endl;
                                        ecrire << "(assert (= d" << c(i + 1, j) << " ";
				}
                                else
				{
					ecrire << "(assert c" << c(i - 1, j) + 2 << ")" << endl;
                                        ecrire << "(assert (= d" << c(i - 1, j) + 2 << " ";
				}
				ecrire << "(+ " << d << " ";
				// Double passage :
				for (int k = 0; k < d; k++)
					ecrire << "u" << k << " ";
				ecrire << ")))" << endl;
			}
	ecrire << "(check-sat)"<< endl << "(get-model)" << endl;
	// Appel de Z3
	system("z3 -smt2 formule > out");
	char s[1000];
	liresat.getline(s, 1000);
	// Vérification de l'existence de solution
	if (s[0] == 'u')
	{
		cout << "No Answer..." << endl;
		return 0;
	}
	liresat.getline(s, 1000);
	bool valM[1000];
	// Recherche de l'orientation des miroirs
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
	// Mise à jour des miroirs
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
	// Sortie ASCII
	for (int i = 0; i < p; i++)
	{
                for (int j = 0; j < l; j++)
			cout << t[i][j];
		cout << endl;
	}
	// Jolie sortie
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
