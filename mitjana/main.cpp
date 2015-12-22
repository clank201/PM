#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

const string SHIPMENTS_2014 = "shipments_2014_06am.mtp"; // Fitxer d’enviaments acabat (versió 6 del matí)
const string US_CENSUS_2014 = "census_2014.usa"; // Fitxer del cens estatal
const int US_STATES = 51; // Registres del cens estàndard d’USA: 50 estats + 1 districte
const int QUARTERS = 4; // 1 "Quarter" representa un trimestre d’un any (4 "quarters" x 3 mesos = 12 mesos)
const int WIDTH_STATE_NAME = 21; // Amplada de camp per output nom d’estats
const int WIDTH_STATE_CODE = 3; // Amplada de camp per output codi estats
const int WIDTH = 17; // Amplada de camp estàndard per outputs genèrics
const int RATE_PRECISION = 3; // Precisió decimal per ratis
const int USD_PRECISION = 2; // Precisió decimal per imports USD (dòlar d’US)

struct state{
	string name;
	string code;
	unsigned int population;
	double anual;
	double units[4];
	double price[4];
	double totalPrice;
};

struct shipment {
	unsigned int date;
	string code;
	unsigned int units;
	double price;
};
void swapStateElements(state stateArray[], int value1, int value2) {
	state temp;
	temp = stateArray[value1];
	stateArray[value1] = stateArray[value2];
	stateArray[value2] = temp;
}
void sortStateArrayCode(state statesArray[]) {
	for (int i = 0; i < US_STATES; i++) {
		for (int j = US_STATES - 1; j > i; j--) {
			if (statesArray[j].code[0] < statesArray[j - i].code[0]) {
				swapStateElements(statesArray, j, j - i);
			}
			else if ((statesArray[j].code[0] == statesArray[j - i].code[0]) && (statesArray[j].code[1] < statesArray[j - i].code[1])) {
				swapStateElements(statesArray, j, j - i);
			}
		}
	}
}

void sortStateArrayTotalPrice(state statesArray[]) {
	for (int i = 0; i < US_STATES; i++) {
		for (int j = US_STATES - 1; j > i; j--) {
			if (statesArray[j].totalPrice > statesArray[j - 1].totalPrice) {
				swapStateElements(statesArray, j, j - 1);
			}
			else if (statesArray[j].totalPrice == statesArray[j - 1].totalPrice) {
				if (statesArray[j].name < statesArray[j - 1].name) {
					swapStateElements(statesArray, j, j - 1);
				}
			}
		}
	}
}

void sortStateArrayAnual(state statesArray[]) {
	for (int i = 0; i < US_STATES; i++) {
		for (int j = US_STATES - 1; j > i; j--) {
			if (statesArray[j].anual > statesArray[j - 1].anual) {
				swapStateElements(statesArray, j, j - 1);
			}
			else if (statesArray[j].anual == statesArray[j - 1].anual) {
				if (statesArray[j].name < statesArray[j - 1].name) {
					swapStateElements(statesArray, j, j - 1);
				}
			}
		}
	}
}

int stringSearchCode(state statesArray[], int n, string name) {
	for (int i = 0; i < n; i++) {
		if (name == statesArray[i].code) {
			return i;
		}
	}
	return -1;
}
unsigned int identifyQuarter(int date) {
	date = date - (10000 * floor(date / 10000));
	return ((date < 700) ? ((date < 400) ? 0 : 1) : ((date < 1000) ? 2 : 3));
}

bool loadCensus(state statesArray[]) {
	string fitxer = US_CENSUS_2014;
	ifstream arxiu;
	arxiu.open(fitxer.c_str());
	if (arxiu.fail()) {
		cout << "NO HEM TROBAT EL FITXER.";
		return 1;
	}
	for (int i = 0; i < US_STATES; i++) {
		arxiu >> statesArray[i].name >> statesArray[i].code >> statesArray[i].population;
	}
	sortStateArrayCode(statesArray);
	cout << "File " << US_CENSUS_2014 << " loaded [ " << US_STATES << " records ]" << endl << endl;
	cout << "=====[ Report S: Census states <Begin> ]=====" << endl << endl;
	cout << setw(3) << left << "PC" << setw(21) << left << "State" << setw(17) << right << "Population" << endl;
	for (int i = 0; i < US_STATES; i++) {
		cout << setw(3) << left << statesArray[i].code << setw(21) << left << statesArray[i].name << setw(17) << right << statesArray[i].population << endl;
	}
	cout << endl << "=====[Report S : Census states <End>]=====" << endl<<endl;
	return 0;
}

void displayShipments(state statesArray[]) {
	state US;
	US.anual = 0;
	US.population = 0;
	US.totalPrice = 0;
	US.name = "United States";
	for (int i = 0; i < 4; i++) {
		US.units[i] = 0;
		US.price[i] = 0;

	}
	for (int i = 0; i < US_STATES; i++) {
		US.anual += statesArray[i].anual;
		US.population += statesArray[i].population;
		US.totalPrice += statesArray[i].totalPrice;
		for (int j = 0; j < 4; j++) {
			US.units[j] += statesArray[i].units[j];
			US.price[j] += statesArray[i].price[j];
		}
	}
	sortStateArrayTotalPrice(statesArray);

	cout << endl << "=====[ Report 2014 Video-Games Springfield <Begin> ]=====" << endl << endl;
	cout << "=====[ Report A: Quarter values ]=====" << endl << endl;
	cout << setw(21) << left << "State" << setw(17) << right << "Population" << setw(17) << right << "Anual" << setw(17) << right << "1Q" << setw(17) << right << "2Q" << setw(17) << right << "3Q" << setw(17) << right << "4Q" << setw(WIDTH) << right << "Population/Value" <<endl;
	
	cout << setw(21) << left << US.name << setw(17) << right << US.population << setw(17) << right << setprecision(USD_PRECISION) << fixed << US.totalPrice << setw(17) << right << setprecision(USD_PRECISION) << fixed << US.price[0] << setw(17) << right << setprecision(USD_PRECISION) << fixed << US.price[1] << setw(17) << right << setprecision(USD_PRECISION) << fixed << US.price[2] << setw(17) << right << setprecision(USD_PRECISION) << fixed << US.price[3] << setw(WIDTH) << right <<setprecision(3) << fixed << US.population/ US.totalPrice<< endl;
	for (int i = 0; i < US_STATES; i++) {
		cout << setw(21) << left << statesArray[i].name << setw(17) << right << statesArray[i].population << setw(17) << right << setprecision(USD_PRECISION) << fixed << statesArray[i].totalPrice << setw(17) << right << setprecision(USD_PRECISION) << fixed << statesArray[i].price[0] << setw(17) << right << setprecision(USD_PRECISION) << fixed << statesArray[i].price[1] << setw(17) << right << setprecision(USD_PRECISION) << fixed << statesArray[i].price[2] << setw(17) << right << setprecision(USD_PRECISION) << fixed << statesArray[i].price[3] << setw(WIDTH) << right << setprecision(3) << fixed << statesArray[i].population / statesArray[i].totalPrice << endl;
	}
	sortStateArrayAnual(statesArray);
	cout << endl << "=====[ Report B: Quarter units ]=====" << endl << endl;
	cout << setw(21) << left << "State" << setw(17) << right << "Population" << setw(17) << right << "Anual" << setw(17) << right << "1Q" << setw(17) << right << "2Q" << setw(17) << right << "3Q" << setw(17) << right << "4Q" << setw(WIDTH) << right << "Population/Units" << endl;
	
	cout << setw(21) << left << US.name << setw(17) << right << US.population << setprecision(0) << fixed<<setw(17) << right <<  US.anual << setw(17) << right << US.units[0] << setw(17) << right <<  US.units[1] << setw(17) << right <<  US.units[2] << setw(17) << right <<  US.units[3] << setw(WIDTH) << right << setprecision(3) << fixed << US.population / US.anual << endl;
	for (int i = 0; i < US_STATES; i++) {
		cout << setw(21) << left << statesArray[i].name << setw(17) << right << statesArray[i].population << setw(17) << right << setprecision(0) << fixed<< statesArray[i].anual << setw(17) << right << statesArray[i].units[0] << setw(17) << right <<  statesArray[i].units[1] << setw(17) << right << statesArray[i].units[2] << setw(17) << right <<  statesArray[i].units[3] << setw(WIDTH) << right << setprecision(3) << fixed << statesArray[i].population / statesArray[i].anual << endl;
	}
}

bool loadShipments(state statesArray[]) {
	int errors = 0;
	cout << "USER INPUT: File name of shipments ?" << endl;
	string filename;
	cin >> filename;
	ifstream file;
	file.open(filename.c_str());
	if (file.fail()) {
		cout << "NO HEM TROBAT EL FITXER.";
		return 1;
	}
	int current = 0;
	shipment temp;
	int position;
	file >> temp.date >> temp.code >> temp.units >> temp.price;
	while (!file.eof()) {
		current++;
		position = stringSearchCode(statesArray, US_STATES, temp.code);
		if (position == -1) {
			errors++;
			cout << "ERROR: Invalid state code : " << temp.code << "[shipment " << current << " omitted]." << endl;
		}
		else {
			statesArray[position].totalPrice += temp.price;
			statesArray[position].units[identifyQuarter(temp.date)] += temp.units;
			statesArray[position].anual += temp.units;
			statesArray[position].price[identifyQuarter(temp.date)] += temp.price;

		}
		file >> temp.date >> temp.code >> temp.units >> temp.price;
	}
	cout << "File " << filename << " loaded[ " << current << " records loaded with " << errors << " records omitted by error = " << current - errors << " ]" << endl;
	
	//
	displayShipments(statesArray);
	return 0;
}

int main(){
	state states[US_STATES];
	for (int i = 0; i < US_STATES; i++) {
		for (int j = 0; j < 4; j++) {
			states[i].units[j] = 0;
			states[i].price[j] = 0;

		}
		states[i].anual = 0;
		states[i].totalPrice = 0;
	}
	if (loadCensus(states)) {
		return 1;
	}
	if (loadShipments(states)) {
		return 1;
	}	
	return 0;
}
