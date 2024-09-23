/*Grocery Inventory for Chada Tech
Develped by: Keith Pottratz
Date: 6/20/2024
*/

#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<iomanip>
#include<limits>
#include<cctype>


//Class for the item tracker part of this code
class ItemTracker {
	//Public and private for ItemTracker
private:
	std::map<std::string, int> itemFrequencies;
	bool fileLoadedSuccessfully; // To check if the file was loaded successfully

	// Constructor to load data from input file
public:
	ItemTracker(const std::string& filename) : fileLoadedSuccessfully(false) {  // Iniitialize fileLoadedSuccessfully in the member initializer list
		std::ifstream inputFile(filename);
		if (!inputFile) {
			std::cerr << "Unable to open file: " << filename << std::endl;
			return;
		}
		std::string item;
		while (inputFile >> item) {
			itemFrequencies[item]++;
		}
		inputFile.close();

		// If we get here it means the file was successfully processed
		fileLoadedSuccessfully = true;

		//create the backup file
		backupData("frequency.dat");
	}
	//function to check if the file was loaded successfully
	bool isFileLoaded() const {
		return fileLoadedSuccessfully;
	}
		

	//My function to get frequency of a specific item

	int getFrequency(const std::string& item) const {
		auto it = itemFrequencies.find(item);
		if (it != itemFrequencies.end()) {
			return it->second;
		}
		else {
			return 0;
		}
	}

	//Function to print all the items and their frequencies. I also included some pretty formatting to make it look nice. adding setw to make the output look nice. 
	void printAllItems() const {
		size_t nameWidth = 0;
		//Find the max item name length for alignment
		for (const auto& pair : itemFrequencies) {
			if (pair.first.length() > nameWidth) {
				nameWidth = pair.first.length();
			}
		}	
		for (const auto& pair : itemFrequencies) {
			std::cout << std::setw(nameWidth + 2) << std::left << pair.first << " " << pair.second << std::endl;
		}	
	}

	//function to print the histogram
	void printHistogram() const {
		for (const auto& pair : itemFrequencies) {
			std::cout << pair.first << " ";
			for (int i = 0; i < pair.second; i++) {
				std::cout << "+";
			}
			std::cout << std::endl;
		}
	}

	//Function to back up the data to a file called frequency.dat and provide error checking incase file can't open
	void backupData(const std::string& backupFilename)const {
		std::ofstream backupFile(backupFilename);
		if (!backupFile) {
			std::cerr << "Unable to open backup file: " << backupFilename << std::endl;
			return;
		}
		for (const auto& pair : itemFrequencies) {
			backupFile << pair.first << " " << pair.second << std::endl;
			if (backupFile.fail()) {
				std::cerr << "Failed to write data to backup file: " << backupFilename << std::endl;  //check to see if the file was written to	successfully if not output a message
				break; // Exit loop if write operation fails.
			}
		}
		backupFile.close();
	}
};

//Function to add validation that it is a valid string to include spaces. This is nessecary to ensure that users don't include !@##$ in their input
bool isValidItemName(const std::string& itemName) {
	if (itemName.empty()) return false;

	// check each character in the string
	for (char ch : itemName) {
		if (!isalpha(ch) && ch != ' ') {  // Allows only leters and spaces
			return false;
		}
		
	}
	return true;
}
//This function displays the menu. I wanted to make my final project look nice so I used ANSI escape codes to make the title bold and the option numbers green. 
//I also added extra whitespace for separation from previous output. Google was my friend in this. 

void displayMenu() {
	std::cout << "\n\n"; // Extra whitespace for separation from previous output
	std::cout << "\033[1mInventory Management System:\033[0m" << std::endl; // Bold title using ANSI escape codes
	std::cout << "================================" << std::endl;
	std::cout << "\033[1;32m1.\033[0m Get Frequency of an item" << std::endl; // Option numbers in green
	std::cout << "\033[1;32m2.\033[0m Print histogram" << std::endl;
	std::cout << "\033[1;32m3.\033[0m Print All the Items in the System." << std::endl;
	std::cout << "\033[1;32m4.\033[0m Exit" << std::endl;
	std::cout << "================================" << std::endl;
	std::cout << "\nPlease enter your choice: "; // Prompt for input
}


//This is to validate the users input for menu choices to ensure non negative number and to ensure not a letter

int getValidMenuchoice() {
	int choice;
	while (true) {
		std::cout << "Enter your choice: ";
		std::cin >> choice;
		if (std::cin.fail() || choice < 1 || choice > 4) { //check to ensure that the numbers are 1 - 4 exclusivly 
			std::cin.clear(); // clear error flag
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
			std::cout << "Invalid choice, Please enter a number between 1 and 4." << std::endl;
		}
		else {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard any extra input
			return choice;
		}
	}
}
//This function is to get the item from the user and validate that the item is not a number and is not empty
void handleMenuchoice(ItemTracker& tracker) {
	int choice;
	
	do {
		displayMenu();
			choice = getValidMenuchoice();

			switch (choice) {
			case 1: {
				std::string item;
				std::cout << "Enter the item name: ";
				//Handles item names tah tinclude spaces
				std::getline(std::cin, item);
				//Validate the item name
				if (!isValidItemName(item)) {
					std::cout << "Ivalid item name. Please enter a valid item name (Only spaces and letters are allowed)" << std::endl;
					break;
				}
				std::cout << "\nFrequency of "<< item << " is " << ": \033[1;31m" << tracker.getFrequency(item) << "\033[0m" << std::endl;
				break;	
			}
			case 2: {
				tracker.printHistogram(); //Print the historgram
				break;
			}
			case 3: {
				tracker.printAllItems(); //Print all the items	
				break;

			}
			case 4: {
				std::cout << "Thank you for using the Inventory Management System!" << std::endl;	
				break;
			default:
				std::cout << "Invalid choice. Please try again" << std::endl;
			}
			}

	} while (choice != 4);
}

//Main function to run the program

int main() {
	ItemTracker tracker("items.txt");

	if (!tracker.isFileLoaded()) {
		std::cerr << "Unable to load data from file" << std::endl;
		return 1; // Exit with error code

	}
	handleMenuchoice(tracker);

	return 0;
}




