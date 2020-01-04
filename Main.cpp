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
	int mesafe; //Aranan kelime ile karþýlaþtýrýlan kelimelerin arasýndaki mesafe (yakýnlýk)
	string veri;
	int satirsayisi; //Verinin bulunduðu satýr sayýsý
	string uzanti; //Verinin bulunduðu uzantý türü
	sayilar* next;
};

//Aþaðýdaki iki fonksiyon -Sorgula- fonksiyonunda kullanýlmak üzere yazýlmýþtýr baþa ve sona eleman ekler
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

//Levenshtein Algoritmasýnda kullanýlmak üzere 3 sayýdan en küçüðünü bulan fonksiyon
int MinimumBul(int a, int b, int c)
{
	int minimum = a;
	if (b <= minimum)
		minimum = b;
	if (c <= minimum)
		minimum = c;
	return minimum;
}

//Levenshtein Mesafe Algoritmasý
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

//Mesafeleri küçükten büyüðe doðru sýralar
sayilar* sorgula(sayilar* r, int gelenmesafe, string gelenveri, int gelensatirsayisi, string uzantituru)
{
	sayilar* yrd;
	yrd = r;
	// Liste boþ ise ekleme yapar
	if (r == NULL)
		r = basaekleme(r, gelenmesafe, gelenveri, gelensatirsayisi, uzantituru);

	//Gelen mesafe listedeki mesafeden büyükse sona ekler
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

	//Gelen mesafe listenin baþýndan küçükse listenin baþýna ekler
	else if (r->mesafe > gelenmesafe)
	{
		r = basaekleme(r, gelenmesafe, gelenveri, gelensatirsayisi, uzantituru);
	}
	return r;
}

//Çýktýlar arasýnda boþluk býrakmak için kullanýlýyor
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
	char devam; //Programýn döngüde kalmasý için gerekli cevap deðiþkeni
	do {
		system("cls");
		static char kelime[MAX];

		cout << "# Aranacak Kelimeyi Giriniz : ";
		cin.getline(kelime, MAX);
		clock_t t1, t2; //Geçen süreyi tutmak için gerekli deðiþkenler
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
			//Bütün verilerle mesafe kontrolü gerçekleþtiriliyor
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
			//Bütün verilerle mesafe kontrolü gerçekleþtiriliyor
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

			if (i >= 4 && i <= 100003) { //HTML kodlarýný okumamasý için aralýk belirleniyor					
				kacincisatir3++;
				//Bütün verilerle mesafe kontrolü gerçekleþtiriliyor
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
			//Bütün verilerle mesafe kontrolü gerçekleþtiriliyor
			if (mesafe_algoritmasi(kelime, okunan5, (strlen(kelime) + 1), (okunan5.size() + 1)) <= 3) //Mesafesi 3 yada daha az olanlar bizi ilgilendiriyor
				root = sorgula(root, mesafe_algoritmasi(kelime, okunan5, (strlen(kelime) + 1), (okunan5.size() + 1)), okunan5, kacincisatir4, rtf);
		};

		//Arama sonuçlarýnýn yazdýrýldýðý dosya oluþturuluyor
		fstream dosya6;
		dosya6.open("arama_sonuclari\\aramasonuclari.txt", ios::out | ios::in | ios::app);

		sayilar* r;
		r = root;
		if (r == NULL) //Tam veya yakýn eþleþme olmadýðý durumda liste boþ kalýyor ve aþaðýdaki çýktýyý veriyor
		{
			cout << "Aranan Kelime Bulunamadý!.." << endl;
		}

		else if (r->mesafe == 0) //Listenin ilk mesafesi 0 ise tam eþleþme oluyor
		{
			dosya6.setf(ios::left);
			dosya6 << "\n# Tam Eþleþme --> " << setw(15) << r->veri << setw(26) << "   KELÝMENÝN BULUNDUÐU SATIR " << setw(5) << r->satirsayisi
				<< "  DOSYA UZANTISI " << r->uzanti << endl;
			dosya6 << "\n################################################################################################################################################\n";
			dosya6.close();
			cout.setf(ios::left);
			cout << "# Tam Eþleþme --> " << setw(15) << r->veri << setw(26) << "   KELÝMENÝN BULUNDUÐU SATIR " << setw(5) << r->satirsayisi << "    DOSYA UZANTISI " << r->uzanti << endl;
		}
		else //0 deðilse yakýn eþleþme oluyor
		{
			dosya6.setf(ios::left);
			dosya6 << "\n# Aranan Kelime Ýle Ýlgili < " << kelime << " > Bulunan Yakýn Eþleþmeler #" << "\n\n";
			dosya6 << "ARANAN KELÝMEYE OLAN UZAKLIK |    YAKIN EÞLEÞME       | KELÝMENÝN BULUNDUÐU SATIR  | DOSYA UZANTISI\n";
			dosya6.close();
			cout << "# Aranan Kelime Ýle Ýlgili < " << kelime << " > Bulunan Yakýn Eþleþmeler #" << "\n\n";
			cout << "ARANAN KELÝMEYE OLAN UZAKLIK |    YAKIN EÞLEÞME       | KELÝMENÝN BULUNDUÐU SATIR  | DOSYA UZANTISI\n\n";
			yazdir2(root);
		}

		t2 = clock();
		float diff = (((float)t2 - (float)t1) / 1000000.0F) * 1000;
		cout << diff << " saniyede bulundu." << endl;

		system("arama_sonuclari\\aramasonuclari.txt");

		cout << "Devam Etmek Ýçin (E/e) , Uygulamayý Sonlandýrmak Ýçin Herhangi Bir Tuþa Basýnýz." << endl << "Seçiminiz :";
		cin >> devam;
		cin.ignore();
	} while (devam == 'e' || devam == 'E');
	cout << "\n# Uygulama Sonlandýrýldý .. #\n";
	return 0;
}
