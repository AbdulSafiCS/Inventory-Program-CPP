// Abdul Safi CS136 LAB 2
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdlib.h>
#include <string>
using namespace std;

class Record {
private:
	string id;
	string name;
	int quantity;
	double price;

public:
	// getters
	string const getName(void) {
		return name;
	}
	string const getId(void) {
		return id;
	}
	int const getQtn(void) {
		return quantity;
	}
	double const getPrice(void) {
		return price;
	}
	// setters
	// precondtion: data needs to be validated.
	// postcondition: data is read into member variables.
	void setRecord(string i, string n, int q, double p) {
		id = i;
		name = n;
		quantity = q;
		price = p;
	}
	void setName(string n) {
		setRecord(id, n, quantity, price);
	}
	void setId(string i) {
		setRecord(i, name, quantity, price);
	}
	void setQuantity(int q) {
		setRecord(id, name, q, price);
	}
	void setPrice(double p) {
		setRecord(id, name, quantity, p);
	}
	const string toString() const {
		stringstream ss;
		ss << left << setw(12) << id << setw(12) << name << setw(12) << quantity
		   << right << setw(5) << price << endl;
		return ss.str();
	}
};

const int MAX_SIZE = 300;

void getDta(Record Inventory[], Record *PointerArray[], int &count);
void print(Record Inventory[], int count);
bool isValidRec(
	string id,
	string name,
	int qtn,
	double price,
	string &outputError);
void printError();
int search(Record Inventory[], int count);
void sort(Record *PointerArray[MAX_SIZE], int count);
void printSorted(Record *PointerArray[], int count);
void printPointerArray(Record *PointerArray[], int count);
void toUpper(string &s);

enum choices { Inv = 1, Err, Srch, Sort, Quit };
int main() {
	int choice;
	int count = 0;
	Record Inventory[MAX_SIZE];
	Record *PointerArray[MAX_SIZE];
	getDta(Inventory, PointerArray, count);
	string userSearch;
	while (choice != Quit) {
		cout << "\nMenu: \n";
		cout << "1. Print DataBase\n"
			 << "2. Print Errors\n"
			 << "3. Search\n"
			 << "4. Sort\n"
			 << "5. Quit\n"; // ADD SORT OPTION.

		cin >> choice;

		switch (choice) {
		case Inv:
			cout << "Printing DataBase: " << endl;
			print(Inventory, count);
			break;

		case Err:
			cout << "Printing DataBase Errors Files: " << endl;
			printError();
			break;

		case Srch:
			int position;
			position = search(Inventory, count);
			if (position == -1) {
				cout << "The item was not found.\n";
			} else {
				cout << Inventory[position].toString();
			}
			break;

		case Sort:
			sort(PointerArray, count);
			break;

		case Quit:
			cout << "Exiting Program..." << endl;
			break;

		default:
			cout << "You entered an invalid choice. Try again." << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	return 0;
}

// function: returns true if ID, name, quantity, and price are valid entries.
// Item Id must be 5 characters long; must be alpha-numeric. The name may contain
// only alpha characters. Quantity and price may be zero. post-condition: returns
// true or false based on the validity of the input data.
bool isValidRec(
	string id,
	string name,
	int qtn,
	double price,
	string &outputError) {
	outputError = "";
	bool isAlphaNum = true;
	for (int i = 0; i < id.size() && isAlphaNum; i++) {
		if (!isalnum(id[i])) {
			outputError += "ID is not Alpha Numeric, ";
			isAlphaNum = false;
		}
	}
	if (id.size() != 5) {
		outputError += "Invalid id lenght, ";
	}
	bool isAlpha = true;
	for (int i = 0; i < name.size() && isAlpha; i++) {
		if (!isalpha(name[i])) {
			outputError += "Invalid Name, ";
			isAlpha = false;
		}
	}
	if (qtn < 0) {
		outputError += "Invalid qtn, ";
	}
	if (price < 0) {
		outputError += "Invalid price";
	}
	return outputError == "" ? true : false;
}

// function: opens inventory and error files. Reads data from the inventory file
// to Inventory[]. writes invalid data to errorFile. precondition: count cant be
// negative and Inventory[] is instantiated postcondition: valid data is entered
// into Inventory[]. Invalid data is written to erroFile.
void getDta(Record Inventory[], Record *PointerArray[], int &count) {
	ifstream inventory;
	inventory.open("inventory.txt");
	if (!inventory.is_open()) {
		cout << "error, cannot open inventory file!\n";
		exit(1);
	}

	ofstream errorFile;
	errorFile.open("errorFile.txt");
	if (!errorFile.is_open()) {
		cout << "error, cannot open error file!\n";
		exit(1);
	}

	string id, name;
	int qtn;
	double price;
	string outputError;
	while (!inventory.eof() && inventory >> id >> name >> qtn >> price &&
		   count < MAX_SIZE) {
		outputError = "";
		toUpper(id);
		toUpper(name);

		if (isValidRec(id, name, qtn, price, outputError)) {
			Inventory[count].setRecord(id, name, qtn, price);
			PointerArray[count] = &Inventory[count];
			count++;
		} else {
			errorFile << left << setw(12) << id << setw(12) << name << setw(12)
					  << qtn << setw(12) << price;
			errorFile << outputError << endl;
		}
	}
	if (count >= MAX_SIZE) {
		cout << "WARNING: Number of records exceeded the limit. ";
	}
	if (count == 0) {
		cout << "WARNING: File is Empty";
	}
	errorFile.close();
	inventory.close();
}

// function: prints valid data from Inventory[].
// precondition: count cant be negative and Inventory[] is instantiated.
// Post-condition: valid data is printed from Inventory[].
void print(Record Inventory[], int count) {
	cout << left << setw(12) << "ID" << setw(12) << "Name" << setw(12) << "Qtn"
		 << setw(12) << "price" << endl;
	for (int i = 0; i < count; i++) {
		cout << Inventory[i].toString();
	}
}
// function: prints the content of errorFile.
// Post: Data along with error messages from errorFile is printed. erroFile is
// closed.
void printError() {
	string errorMessage;
	ifstream errorFile;

	errorFile.open("errorFile.txt");
	if (!errorFile.is_open()) {
		cout << "error, cannot open error file!\n";
	} else {
		cout << left << setw(12) << "ID" << setw(12) << "Name" << setw(12)
			 << "Qtn" << setw(12) << "Price" << endl;
		while (!errorFile.eof()) {
			getline(errorFile, errorMessage);
			cout << errorMessage << endl;
		}
	}

	errorFile.close();
}
// sorts the array of objects using parallel array of pointers.
// precondition: Inventory[] needs to be instantiated. Array of parallel
// pointers points to inventory[]. postcondition: array of parallel pointers is
// sorted ascending by name or ID.
void sort(Record *PointerArray[MAX_SIZE], int count) {
	enum choice { ID = 1, Name, Qtn, Price, Exit };
	Record *temp;
	bool toSwap = false;
	bool validResponse = false;
	int choices = 0;
	while (!validResponse) {
		cout << "\n\n1. Sort by ID\n"
			 << "2. Sort by Name\n"
			 << "3. Sort by Quantity\n"
			 << "4. Sort by Price\n"
			 << "5. Exit Sort\n";
		cin >> choices;
		if (choices == 1 || choices == 2 || choices == 3 || choices == 4 ||
			choices == 5) {
			validResponse = true;
		} else {
			cout << "Invalid choice, please try again!";
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	for (int i = 0; i < count - 1; i++) {
		for (int j = 0; j < count - 1 - i; j++) {
			switch (choices) {
			case ID:
				toSwap =
					PointerArray[j]->getId() > PointerArray[j + 1]->getId();
				break;

			case Name:
				toSwap =
					PointerArray[j]->getName() > PointerArray[j + 1]->getName();
				break;

			case Qtn:
				toSwap =
					PointerArray[j]->getQtn() > PointerArray[j + 1]->getQtn();
				break;

			case Price:
				toSwap = PointerArray[j]->getPrice() >
					PointerArray[j + 1]->getPrice();
				break;

			case Exit:
				break;
			}

			if (toSwap) {
				temp = PointerArray[j + 1];
				PointerArray[j + 1] = PointerArray[j];
				PointerArray[j] = temp;
			}
		}
	}
	if (choices == 5) {
		cout << "exiting sort\n";
	} else {
		printSorted(PointerArray, count);
	}
}
// prints the array of parallel pointers sorted in ascending.
// precondition: inventory[] is instantiated. array of parallel pointer is
// pointed to inventory[]. count is positive. arrray of parallel pointer is
// printed.
void printSorted(Record *PointerArray[], int count) {
	cout << left << setw(12) << "ID" << setw(12) << setw(12) << "Name"
		 << setw(12) << "Qtn" << setw(12) << "Price" << endl;
	for (int i = 0; i < count; i++) {
		cout << (PointerArray[i])->toString();
	}
}
// searches the array of objects for a specific object by name or ID.
// precondition: count is not negative. inventory[] is instantiated.
// postcondition: the index of the found object is returned.
int search(Record Inventory[], int count) {
	string searchKey;

	bool found = false;
	cout << "please enter name or ID to search: ";
	cin >> searchKey;
	toUpper(searchKey);
	for (int i = 0; i < count && !found; i++) {
		if (searchKey == Inventory[i].getId() ||
			searchKey == Inventory[i].getName()) {
			found = true;
			return i;
		}
	}
	return -1;
}
// converts a lower case string to uppercase
// precondition: string needs to be delcared and passed by reference.
// postconditon: converts a string to uppercase.
void toUpper(string &s) {
	for (int i = 0; i < s.length(); i++) {
		s[i] = toupper(s[i]);
	}
}
