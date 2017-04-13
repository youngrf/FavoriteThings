// FavoriteThings.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;

class Favorite
{
	string ItemNumber;
	string ItemName;
	int	ItemQuantity;
	string ItemType;
public:
	Favorite();
	Favorite(string num, string name, int quantity, string type);
	void CaptureFavoriteItem();
	void ShowFavoriteItem();
	int SaveFavoriteItem(ofstream &outfile);
};

class FavoriteList
{
	Favorite **list;
	int numrecords;
	int listsize;
	int ReallocateArray();
public:
	FavoriteList();
	~FavoriteList();
	void GetUserInput();
	void ShowFavoriteList();
	int SaveFavoriteList(string filename);
	int ReadFavoriteList(string filename);
};

Favorite::Favorite()
{
	ItemNumber = ""; ItemName = ""; ItemQuantity = 0; ItemType = "";
}

Favorite::Favorite(string num, string name, int quantity, string type)
{
	ItemNumber = num;
	ItemName = name;
	ItemQuantity = quantity;
	ItemType = type;
}

void Favorite::CaptureFavoriteItem()
{
	cout << "Item Num? -->";
	getline(cin, ItemNumber);
	cout << "Item Name?  -->";
	getline(cin, ItemName);
	cout << "Item Type? -->";
	getline(cin, ItemType);
	cout << "Quanity? -->";
	cin >> ItemQuantity;

	cin.ignore();
	cin.clear();
}

void Favorite::ShowFavoriteItem()
{
	cout << "Number: " << ItemNumber << " Name: " << ItemName << " Quantity: " << ItemQuantity << "Type:" << ItemType << endl;
}

int Favorite::SaveFavoriteItem(ofstream& outfile)
{
	if (outfile.is_open())
	{
		outfile << ItemNumber << "|" << ItemName << "|" << ItemQuantity << "|" << ItemType << endl;
		return 0;
	}
	else
	{
		return WRITEERROR;
	}
}

FavoriteList::FavoriteList()
{
	list = new Favorite*[ARRAYSIZE];
	numrecords = 0;
	listsize = ARRAYSIZE;
}

FavoriteList::~FavoriteList()
{
	for (int i = 0; i < numrecords; i++)
	{
		delete list[i];
	}
	delete[]list;
}

int FavoriteList::ReallocateArray()
{
	Favorite **temp;
	temp = new Favorite*[listsize + ARRAYSIZE];
	listsize = listsize + ARRAYSIZE;
	for (int i = 0; i < numrecords; i++)
	{
		temp[i] = list[i];
	}
	delete[]list;
	list = temp;


	return 0;
}

void FavoriteList::GetUserInput()
{
	string answer = "y";
	cout << "Enter Item Y/N?" << endl;
	getline(cin, answer);
	while (answer == "Y" || answer == "y")
	{
		list[numrecords] = new Favorite();
		list[numrecords]->CaptureFavoriteItem();
		numrecords++;
		cout << "enter another item Y/N?" << endl;
		getline(cin, answer);
	}
}

void FavoriteList::ShowFavoriteList()
{
	for (int i = 0; i < numrecords; i++)
		list[i]->ShowFavoriteItem();
}

int FavoriteList::SaveFavoriteList(string filename)
{
	ofstream output(filename, ios::trunc);
	if (output)
	{
		for (int i = 0; i < numrecords; i++)
		{
			list[i]->SaveFavoriteItem(output);
		}
		output.close();

		return 0;
	}
	else
	{
		return WRITEERROR;
	}
	return 0;
}

int FavoriteList::ReadFavoriteList(string filename)
{
	string inum, iname, iqty, itype;
	ifstream infile(filename, ios::in);
	if (!infile)
	{
		cout << "File could not be opened for reading" << endl;
		return READERROR;
	}


	while (!infile.eof())
	{
		if (numrecords == listsize)
		{
			ReallocateArray();
		}
		getline(infile, inum, '|');
		if (!inum.empty())
		{


			getline(infile, iname, '|');
			getline(infile, iqty, '|');
			getline(infile, itype);
			int qty = stoi(iqty);
			list[numrecords] = new Favorite(inum, iname, qty, itype);
			numrecords++;
		}
	}
	infile.close();
	return 0;
}


int main()
{
	
		string filename = "c:\\users\\Ethan\\desktop\\favoritethings.txt";
		FavoriteList myFavorites;
		string answer;
		int error;
		error = myFavorites.ReadFavoriteList(filename);
		if (error)
		{
			cout << "Cannot read favorite item list - continue creating new list? Y/N -->";
			getline(cin, answer);
			if (answer != "Y" && answer != "y")
			{
				return 1;
			}
		}
		myFavorites.GetUserInput();
		myFavorites.SaveFavoriteList(filename);
		myFavorites.ShowFavoriteList();
		system("PAUSE");
		return 0;
}

