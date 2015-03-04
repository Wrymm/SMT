#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

using namespace std;

ifstream lire;					// Map
ifstream liresat("out", ios::in);		// Retour de SAT
ofstream ecrire("formule", ios::out);		// Formule générée
ofstream ecriremp("solution.mp", ios::out);	// Jolie sortie

const int SIZE_MAX = 1000;	// Largeur de la grille

char t[SIZE_MAX][SIZE_MAX];	// Grille
int p = 0;			// Profondeur de la grille
int l;                          // Largeur de la grille
int m = 0;			// Nombre de miroirs
int d = 0;			// Nombre de diamants
int w = 0;			// Nombre de murs
int g = 0;			// Compteur de miroirs


// Numéro d'une case
int c(int i, int j)
{
	return 4 * (i * l + j) + 1;
}

// Numéro d'un miroir
int b()
{
	return l * p * 4 + g + 1;
}

int main(int argc, char** argv)
{
	// Paramètres
	if (argc == 1)
	{
		cout << argv[0] << " <mamap>" << endl;
		return 0;
	}
	// Lecture de l'entrée
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
	/* Nombre de variables :
		- Nombre de cases * 4
		- Nombre de miroirs

	   Nombre de clauses :
		- Nombre de cases parcourables * 12
		- Nombre de miroirs * 16
		- Nombre de murs * 4
		- Nombre de diamants
		- Case de départ
	*/
	ecrire << "p cnf " << l * p * 4 + m << " " << (l * p - w - m - 2) * 12 + m * 16 + w * 4 + d + 2 << endl;
	for (int i = 0; i < p; i++)
		for (int j = 0; j < l; j++)
			if (t[i][j] == '%')
			{
				// Murs inaccessibles
				ecrire << - c(i, j)     << " 0" << endl;
				ecrire << - c(i, j) - 1 << " 0" << endl;
				ecrire << - c(i, j) - 2 << " 0" << endl;
				ecrire << - c(i, j) - 3 << " 0" << endl;
			}
			else if (t[i][j] == 'M')
			{
				// Orientation 1 :
				ecrire << - b() << " " << - c(i, j - 1) - 1 << " " << c(i,j)       << " 0" << endl;
				ecrire << - b() << " " << - c(i - 1, j) - 2 << " " << c(i,j) + 3   << " 0" << endl;
				ecrire << - b() << " " << - c(i, j + 1) - 3 << " " << c(i,j) + 2   << " 0" << endl;
				ecrire << - b() << " " << - c(i + 1, j)     << " " << c(i,j) + 1   << " 0" << endl;
				ecrire << - b() << " " << c(i, j - 1) + 1   << " " << - c(i,j)     << " 0" << endl;
                                ecrire << - b() << " " << c(i - 1, j) + 2   << " " << - c(i,j) - 3 << " 0" << endl;
                                ecrire << - b() << " " << c(i, j + 1) + 3   << " " << - c(i,j) - 2 << " 0" << endl;
                                ecrire << - b() << " " << c(i + 1, j)       << " " << - c(i,j) - 1 << " 0" << endl;
				// Orientation 2 :
				ecrire <<   b() << " " << - c(i, j - 1) - 1 << " " << c(i,j) + 2   << " 0" << endl;
                                ecrire <<   b() << " " << - c(i - 1, j) - 2 << " " << c(i,j) + 1   << " 0" << endl;
                                ecrire <<   b() << " " << - c(i, j + 1) - 3 << " " << c(i,j)       << " 0" << endl;
                                ecrire <<   b() << " " << - c(i + 1, j)     << " " << c(i,j) + 3   << " 0" << endl;
				ecrire <<   b() << " " << c(i, j - 1) + 1   << " " << - c(i,j) - 2 << " 0" << endl;
                                ecrire <<   b() << " " << c(i - 1, j) + 2   << " " << - c(i,j) - 1 << " 0" << endl;
                                ecrire <<   b() << " " << c(i, j + 1) + 3   << " " << - c(i,j)     << " 0" << endl;
                                ecrire <<   b() << " " << c(i + 1, j)       << " " << - c(i,j) - 3 << " 0" << endl;
				g++;
			}
			else if (t[i][j] == 'D' or t[i][j] == ' ')
                        {
				// Transmission de case extérieure vers case courante
				ecrire << - c(i + 1, j)     << " " << c(i, j)       << " 0" << endl;
				ecrire << - c(i, j - 1) - 1 << " " << c(i, j) + 1   << " 0" << endl;
				ecrire << - c(i - 1, j) - 2 << " " << c(i, j) + 2   << " 0" << endl;
				ecrire << - c(i, j + 1) - 3 << " " << c(i, j) + 3   << " 0" << endl;
				// Transmission de case courante vers case extérieure
				ecrire << c(i + 1, j)       << " " << - c(i, j)     << " 0" << endl;
                                ecrire << c(i, j - 1) + 1   << " " << - c(i, j) - 1 << " 0" << endl;
                                ecrire << c(i - 1, j) + 2   << " " << - c(i, j) - 2 << " 0" << endl;
                                ecrire << c(i, j + 1) + 3   << " " << - c(i, j) - 3 << " 0" << endl;
				ecrire << - c(i, j)         << " " << c(i, j) + 2   << " 0" << endl;
                                ecrire << - c(i, j) - 1     << " " << c(i, j) + 3   << " 0" << endl;
                                ecrire << - c(i, j) - 2     << " " << c(i, j)       << " 0" << endl;
                                ecrire << - c(i, j) - 3     << " " << c(i, j) + 1   << " 0" << endl;
				// Passage par diamant
				if (t[i][j] == 'D')
					ecrire << c(i, j) << " " << c(i, j) + 1 << " " << c(i, j) + 2 << " " << c(i, j) + 3 << " 0" << endl;
                        }
			else
			{
				// Direction initiale
				if (j == 0)
					ecrire << c(i, j) + 1 << " 0" << endl;
				else if (j == l - 1)
					ecrire << c(i, j) + 3 << " 0" << endl;
                                else if (i == 0)
                                        ecrire << c(i, j) + 2 << " 0" << endl;
				else
					ecrire << c(i, j)     << " 0" << endl;
			}
	// Appeler Z3
	system("z3 -dimacs formule > out");
	char s[20];
	liresat.getline(s, 20);
	// Vérification de l'existence de solution
	if (s[0] == 'u')
	{
		cout << "No Answer..." << endl;
		return 0;
	}
	// Information sur les cases inutile
	for (int i = 0; i < p; i++)
		for (int j = 0; j < l; j++)
		{
			int u1, u2, u3, u4;
			liresat >> u1 >> u2 >> u3 >> u4;
		}
	// Information sur les miroirs
	for (int i = 0; i < p; i++)
                for (int j = 0; j < l; j++)
			if (t[i][j] == 'M')
			{
				int u;
				liresat >> u;
				if (u > 0)
					t[i][j] = '/';
				else
					t[i][j] = '\\';
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
