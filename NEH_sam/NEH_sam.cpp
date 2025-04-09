#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

int Cmax(int n, int m, int* Dane, int* Kolejnosc, int* Czas)
{
	for (int i = 0; i <= m; i++)
	{
		Czas[i] = 0;
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			Czas[j] = max(Czas[j], Czas[j - 1]) + Dane[(j - 1) + Kolejnosc[i] * m];
		}
	}

	return Czas[m];
}

void Wagi_Obliczanie_Sortowanie(int n, int m, int* Dane, int* Kolejnosc)
{
	int* Waga = new int[n];

	for (int i = 0; i < n; i++)
	{
		Waga[i] = 0;
		for (int j = 0; j < m; j++)
		{
			Waga[i] += Dane[i * m + j];
		}
	}

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (Waga[j] < Waga[j + 1])
			{
				swap(Waga[j], Waga[j + 1]);
				swap(Kolejnosc[j], Kolejnosc[j + 1]);
			}
		}
	}

	delete[] Waga;
}

int NEH(int n, int m, int* Dane, int* Kolejnosc, int* Czas)
{
	int MAX_INT = 9999999;
	Wagi_Obliczanie_Sortowanie(n, m, Dane, Kolejnosc);

	// wstawianie
	for (int i = 0; i < n; i++)
	{
		int najlepsza_pozycja = -1;
		int najlepsze_cmax = MAX_INT;

		for (int j = i; j >= 0; j--)
		{
			int temp = Cmax(i + 1, m, Dane, Kolejnosc, Czas);

			if (najlepsze_cmax >= temp)
			{
				najlepsze_cmax = temp;
				najlepsza_pozycja = j;
			}

			if (j != 0)
			{
				swap(Kolejnosc[j], Kolejnosc[j - 1]);
			}
		}

		for (int i = 0; i < najlepsza_pozycja; i++)
		{
			swap(Kolejnosc[i], Kolejnosc[i + 1]);
		}
	}
	return Cmax(n, m, Dane, Kolejnosc, Czas);
}

int main()
{
	int n, m;			// n - ilość zadań (wiersze), m - ilość maszyn (kolumny)
	int Dane[10000];
	int Kolejnosc[502];
	int Czas[1000];
	int wynik;

	string nazwa = "data.";
	string s1, s2;

	ifstream wejscie;
	ofstream wyjscie_cmax, wyjscie_kolejnosc;

	wejscie.open("dane.txt");
	wyjscie_cmax.open("cmax.txt");
	wyjscie_kolejnosc.open("kolejnosc.txt");

	auto t = chrono::high_resolution_clock::now();
	for (int i = 0; i < 121; i++)
	{
		if (i < 10)
		{
			s1 = nazwa + "00" + to_string(i) + ":";
		}
		else if (i < 100)
		{
			s1 = nazwa + "0" + to_string(i) + ":";
		}
		else
		{
			s1 = nazwa + to_string(i) + ":";
		}

		while (s2 != s1)
		{
			wejscie >> s2;
		}

		wejscie >> n >> m;

		for (int j = 0; j < n * m; j++)
		{
			wejscie >> Dane[j];
		}

		for (int j = 0; j < n; j++)
		{
			Kolejnosc[j] = j;
		}

		auto t1 = chrono::high_resolution_clock::now();
		wynik = NEH(n, m, Dane, Kolejnosc, Czas);
		auto t2 = chrono::high_resolution_clock::now();
		auto czas_czastkowy = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
		auto czas_czastkowy_sek = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();

		// printowanie w konsoli:
		cout << s1 << "\ncmax:\t" << wynik << "\nczas:\t" << czas_czastkowy << " ms (" << czas_czastkowy_sek * 1e-9 << " s)" << endl;
		//cout << "kolejnosc:\n";

		/*for (int i = 0; i < n; i++)
		{
			cout << Kolejnosc[i] + 1 << " ";
		}
		cout << endl << endl;*/

		// zapis do pliku
		wyjscie_cmax << s1 << "\ncmax:\t" << wynik << "\nczas:\t" << czas_czastkowy << " ms (" << czas_czastkowy_sek * 1e-9 << " s)" << endl << endl;
		wyjscie_kolejnosc << s1 << endl;

		for (int i = 0; i < n; i++)
		{
			wyjscie_kolejnosc << Kolejnosc[i] + 1 << " ";
		}
		wyjscie_kolejnosc << endl << endl;
	}

	wejscie.close();

	auto t3 = chrono::high_resolution_clock::now();
	auto czas_total = chrono::duration_cast<std::chrono::microseconds>(t3 - t).count();
	auto czas_total_sek = chrono::duration_cast<chrono::nanoseconds>(t3 - t).count();

	cout << "\ntotal time: " << czas_total << " ms (" << czas_total_sek * 1e-9 << " s)" << endl;
	wyjscie_cmax << "\ntotal time: " << czas_total << " ms (" << czas_total_sek * 1e-9 << " s)" << endl;

	return 0;
}