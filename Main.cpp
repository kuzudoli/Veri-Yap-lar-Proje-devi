#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <iomanip>//duzenleme komut sinifi
#include <stdio.h>
#include <time.h>
#include <conio.h>
using namespace std;
#define MAX 100
static int mesafe;


struct sayilar
{
	int mesafe; //Aranan kelime ile kar��la�t�r�lan kelimelerin aras�ndaki mesafe (yak�nl�k)
	string veri;
	int satirsayisi; //Verinin bulundu�u sat�r say�s�
	string uzanti; //Verinin bulundu�u uzant� t�r�
	sayilar* next;
};

//A�a��daki iki fonksiyon -Sorgula- fonksiyonunda kullan�lmak �zere yaz�lm��t�r ba�a ve sona eleman ekler
sayilar* basaekleme(sayilar* r, int gelenmesafe, string gelenveri, int gelensatirsayisi, string uzantituru)
{
	if (r == NULL)
	{
		r = new sayilar;
		r->mesafe = gelenmesafe;
		r->veri = gelenveri;
		r->satirsayisi = gelensatirsayisi;
		r->uzanti = uzantituru;
		r->next = NULL;
	}
	else
	{
		sayilar* yrd;
		yrd = new sayilar;
		yrd->mesafe = gelenmesafe;
		yrd->veri = gelenveri;
		yrd->satirsayisi = gelensatirsayisi;
		yrd->uzanti = uzantituru;

		yrd->next = r;
		r = yrd;
		yrd = NULL;
		delete yrd;
	}
	return r;
}
sayilar* sonaekleme(sayilar* r, int gelenmesafe, string gelenveri, int gelensatirsayisi, string uzantituru)
{
	if (r == NULL)
	{
		r = new sayilar;
		r->mesafe = gelenmesafe;
		r->veri = gelenveri;
		r->satirsayisi = gelensatirsayisi;
		r->uzanti = uzantituru;
		r->next = NULL;
	}
	else
	{

		sayilar* yrd;
		yrd = r;
		while (yrd->next != NULL)
		{
			yrd = yrd->next;
		}
		sayilar* tmp;
		tmp = new sayilar;
		tmp->mesafe = gelenmesafe;
		tmp->veri = gelenveri;
		tmp->satirsayisi = gelensatirsayisi;
		tmp->uzanti = uzantituru;
		yrd->next = tmp;
		tmp->next = NULL;
		yrd = NULL;
		tmp = NULL;
		delete yrd, tmp;
	}
	return r;
}

//Levenshtein Algoritmas�nda kullan�lmak �zere 3 say�dan en k�����n� bulan fonksiyon
int MinimumBul(int a, int b, int c)
{
	int minimum = a;
	if (b <= minimum)
		minimum = b;
	if (c <= minimum)
		minimum = c;
	return minimum;
}

//Levenshtein Mesafe Algoritmas�
int mesafe_algoritmasi(char GK[], string KK, int m, int n)
{
	int Matris[MAX][MAX];
	for (int i = 0; i < m; i++)
		Matris[i][0] = i;
	for (int i = 0; i < n; i++)
		Matris[0][i] = i;
	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (GK[i - 1] == KK[j - 1])
			{
				Matris[i][j] = Matris[i - 1][j - 1];
				mesafe = Matris[i][j];
			}
			else
			{
				Matris[i][j] = MinimumBul(Matris[i - 1][j] + 1, Matris[i][j - 1] + 1, Matris[i - 1][j - 1] + 1);
				mesafe = Matris[i][j];
			}
		}
	}
	return mesafe;
}

//Mesafeleri k���kten b�y��e do�ru s�ralar
sayilar* sorgula(sayilar* r, int gelenmesafe, string gelenveri, int gelensatirsayisi, string uzantituru)
{
	sayilar* yrd;
	yrd = r;
	// Liste bo� ise ekleme yapar
	if (r == NULL)
		r = basaekleme(r, gelenmesafe, gelenveri, gelensatirsayisi, uzantituru);

	//Gelen mesafe listedeki mesafeden b�y�kse sona ekler
	else if (r->mesafe < gelenmesafe)
	{
		if (r->next == NULL)
		{
			r = sonaekleme(r, gelenmesafe, gelenveri, gelensatirsayisi, uzantituru);
		}
		else
		{
			while (yrd->next->mesafe < gelenmesafe)
			{
				yrd = yrd->next;
				if (yrd->next == NULL)
				{
					sayilar* t = NULL;
					t = sonaekleme(r, gelenmesafe, gelenveri, gelensatirsayisi, uzantituru);
					return t;
				}
			}
			//Araya ekleme islemleri , 1 3 2
			sayilar* tmp;
			tmp = new sayilar;
			tmp->mesafe = gelenmesafe;
			tmp->veri = gelenveri;
			tmp->satirsayisi = gelensatirsayisi;
			tmp->uzanti = uzantituru;
			tmp->next = yrd->next;
			yrd->next = tmp;
			yrd = NULL;
			tmp = NULL;
			delete yrd, tmp;
		}
	}

	//Gelen mesafe listenin ba��ndan k���kse listenin ba��na ekler
	else if (r->mesafe > gelenmesafe)
	{
		r = basaekleme(r, gelenmesafe, gelenveri, gelensatirsayisi, uzantituru);
	}
	return r;
}

//��kt�lar aras�nda bo�luk b�rakmak i�in kullan�l�yor
void yazdir2(sayilar* r)
{
	fstream dosya7;
	dosya7.open("arama_sonuclari\\aramasonuclari.txt", ios::out | ios::in | ios::app);

	while (r != NULL)
	{
		dosya7.setf(ios::left);
		dosya7 << "          " << setw(24) << r->mesafe << setw(26) << r->veri << setw(25) << r->satirsayisi << r->uzanti << endl;
		cout.setf(ios::left);
		cout << "          " << setw(24) << r->mesafe << setw(26) << r->veri << setw(25) << r->satirsayisi << r->uzanti << endl;
		r = r->next;
	}

	dosya7 << "\n################################################################################################################################################\n";
	dosya7.close();
}



int main()
{
	setlocale(LC_ALL, "Turkish");
	char devam; //Program�n d�ng�de kalmas� i�in gerekli cevap de�i�keni
	do {
		system("cls");
		static char kelime[MAX];

		cout << "# Aranacak Kelimeyi Giriniz : ";
		cin.getline(kelime, MAX);
		clock_t t1, t2; //Ge�en s�reyi tutmak i�in gerekli de�i�kenler
		sayilar* root = NULL;
		t1 = clock();

		//TXT DOSYASINDA ARAMA
		ifstream dosya("data_set\\100k_1.txt");
		string okunan;
		int kacincisatir = 0;
		string txt = "TXT";
		while (!dosya.eof())
		{
			getline(dosya, okunan);
			kacincisatir++;
			//B�t�n verilerle mesafe kontrol� ger�ekle�tiriliyor
			if (mesafe_algoritmasi(kelime, okunan, (strlen(kelime) + 1), (okunan.size() + 1)) <= 3) //Mesafesi 3 yada daha az olanlar bizi ilgilendiriyor
				root = sorgula(root, mesafe_algoritmasi(kelime, okunan, (strlen(kelime) + 1), (okunan.size() + 1)), okunan, kacincisatir, txt);
		};

		//CSV (EXCEL) DOSYASINDA ARAMA
		ifstream dosya3("data_set\\100k_2.csv");
		string okunan3;
		string csv = "CSV";
		int kacincisatir2 = 0;
		while (!dosya3.eof())
		{
			getline(dosya3, okunan3);
			kacincisatir2++;
			//B�t�n verilerle mesafe kontrol� ger�ekle�tiriliyor
			if (mesafe_algoritmasi(kelime, okunan3, (strlen(kelime) + 1), (okunan3.size() + 1)) <= 3) //Mesafesi 3 yada daha az olanlar bizi ilgilendiriyor
				root = sorgula(root, mesafe_algoritmasi(kelime, okunan3, (strlen(kelime) + 1), (okunan3.size() + 1)), okunan3, kacincisatir2, csv);
		};

		//HTML DOSYASINDA ARAMA
		ifstream dosya4("data_set\\100k_3.html");
		string okunan4;
		string html = "HTML";
		int kacincisatir3 = 0;
		int i = 0;
		while (!dosya4.eof())
		{
			getline(dosya4, okunan4);

			if (i >= 4 && i <= 100003) { //HTML kodlar�n� okumamas� i�in aral�k belirleniyor					
				kacincisatir3++;
				//B�t�n verilerle mesafe kontrol� ger�ekle�tiriliyor
				if (mesafe_algoritmasi(kelime, okunan4, (strlen(kelime) + 1), (okunan4.size() + 1)) <= 3) //Mesafesi 3 yada daha az olanlar bizi ilgilendiriyor
					root = sorgula(root, mesafe_algoritmasi(kelime, okunan4, (strlen(kelime) + 1), (okunan4.size() + 1)), okunan4, kacincisatir3, html);
			}
			i++;
		};

		//RTF DOSYASINDA ARAMA
		ifstream dosya5("data_set\\100k_4.rtf");
		string okunan5;
		string rtf = "RTF";
		int kacincisatir4 = 0;
		while (!dosya5.eof())
		{
			getline(dosya5, okunan5);
			kacincisatir4++;
			//B�t�n verilerle mesafe kontrol� ger�ekle�tiriliyor
			if (mesafe_algoritmasi(kelime, okunan5, (strlen(kelime) + 1), (okunan5.size() + 1)) <= 3) //Mesafesi 3 yada daha az olanlar bizi ilgilendiriyor
				root = sorgula(root, mesafe_algoritmasi(kelime, okunan5, (strlen(kelime) + 1), (okunan5.size() + 1)), okunan5, kacincisatir4, rtf);
		};

		//Arama sonu�lar�n�n yazd�r�ld��� dosya olu�turuluyor
		fstream dosya6;
		dosya6.open("arama_sonuclari\\aramasonuclari.txt", ios::out | ios::in | ios::app);

		sayilar* r;
		r = root;
		if (r == NULL) //Tam veya yak�n e�le�me olmad��� durumda liste bo� kal�yor ve a�a��daki ��kt�y� veriyor
		{
			cout << "Aranan Kelime Bulunamad�!.." << endl;
		}

		else if (r->mesafe == 0) //Listenin ilk mesafesi 0 ise tam e�le�me oluyor
		{
			dosya6.setf(ios::left);
			dosya6 << "\n# Tam E�le�me --> " << setw(15) << r->veri << setw(26) << "   KEL�MEN�N BULUNDU�U SATIR " << setw(5) << r->satirsayisi
				<< "  DOSYA UZANTISI " << r->uzanti << endl;
			dosya6 << "\n################################################################################################################################################\n";
			dosya6.close();
			cout.setf(ios::left);
			cout << "# Tam E�le�me --> " << setw(15) << r->veri << setw(26) << "   KEL�MEN�N BULUNDU�U SATIR " << setw(5) << r->satirsayisi << "    DOSYA UZANTISI " << r->uzanti << endl;
		}
		else //0 de�ilse yak�n e�le�me oluyor
		{
			dosya6.setf(ios::left);
			dosya6 << "\n# Aranan Kelime �le �lgili < " << kelime << " > Bulunan Yak�n E�le�meler #" << "\n\n";
			dosya6 << "ARANAN KEL�MEYE OLAN UZAKLIK |    YAKIN E�LE�ME       | KEL�MEN�N BULUNDU�U SATIR  | DOSYA UZANTISI\n";
			dosya6.close();
			cout << "# Aranan Kelime �le �lgili < " << kelime << " > Bulunan Yak�n E�le�meler #" << "\n\n";
			cout << "ARANAN KEL�MEYE OLAN UZAKLIK |    YAKIN E�LE�ME       | KEL�MEN�N BULUNDU�U SATIR  | DOSYA UZANTISI\n\n";
			yazdir2(root);
		}

		t2 = clock();
		float diff = (((float)t2 - (float)t1) / 1000000.0F) * 1000;
		cout << diff << " saniyede bulundu." << endl;

		system("arama_sonuclari\\aramasonuclari.txt");

		cout << "Devam Etmek ��in (E/e) , Uygulamay� Sonland�rmak ��in Herhangi Bir Tu�a Bas�n�z." << endl << "Se�iminiz :";
		cin >> devam;
		cin.ignore();
	} while (devam == 'e' || devam == 'E');
	cout << "\n# Uygulama Sonland�r�ld� .. #\n";
	return 0;
}
