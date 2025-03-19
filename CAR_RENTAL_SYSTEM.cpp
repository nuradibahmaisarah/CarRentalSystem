#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

// Constants for the number of cars and maximum number of customers
const int NUM_CAR = 4;
const int MAX_CUST = 5;

// Structure to store car details (record)
struct detailCar 
{
    string carID; 	// Unique ID for the car
    string model;	// Car model
    bool isRented; 	// Rental status of the car (true if rented, false if available)
};

// Structure to store customer details (record)
struct cust {
    string name; 			// Customer's name
    string phone; 			// Customer's phone number
    int hours; 				// Number of hours the car is rented
    int days; 				// Number of days the car is rented
    double totalPrice; 		// Total price to be paid by the customer
    char rentalType; 		// Rental type: 'D' for days, 'H' for hours
};

// Structure to store price details (record)
struct Price 
{
    double priceBeforeDiscount; // Price before applying any discounts
    double discount; 			// Discount amount
    double priceAfterDiscount;  // Price after applying the discount
};

// Function prototypes (declarations)
void displayMenu();
void displayCars(const detailCar cars[][MAX_CUST], const double dailyPrices[], const double hourlyPrices[], int numCar);
void rentCar(detailCar cars[][MAX_CUST], int numCar, cust customers[], int &custCount, const double dailyPrices[], const double hourlyPrices[]);
void returnCar(detailCar cars[][MAX_CUST], int numCar);
void saveToFile(const detailCar cars[][MAX_CUST], const double dailyPrices[], const double hourlyPrices[], int numCar, const cust customers[], int custCount);
Price calculateTotalPrice(int duration, double price, int type);
void displayRentalDetails(const cust& customer, const detailCar& car, const Price& priceDetails);
double calculateTotalRevenue(const cust* customers, int custCount);

int main() {
    // Initializing arrays and variables
    detailCar cars[NUM_CAR][MAX_CUST] = {                         // 2D array
			{"1", "Axia", false}, 
			{"2", "Bezza", false}, 
			{"3", "Ativa", false}, 
			{"4", "Alza", false}
	};
    double dailyPrices[NUM_CAR] = {120.0, 140.0, 160.0, 180.0};    // 1D array
    double hourlyPrices[NUM_CAR] = {8.90, 11.80, 12.90, 14.80};    // 1D array
    cust customers[MAX_CUST]; // Array to store customer details
    int custCount = 0; // Counter for the number of customers
    int choice;

	// Welcome message
    cout << "\n~~~WELCOME TO POOH CAR RENTAL SERVICE~~~" << endl;

    // Main menu loop
    do 
	{
        displayMenu();   // Display the menu
        cin >> choice;  // Read the user's choice
        switch (choice) 
		{
            case 1:
                displayCars(cars, dailyPrices, hourlyPrices, NUM_CAR);   // Display available cars
                break;
            case 2:
                rentCar(cars, NUM_CAR, customers, custCount, dailyPrices, hourlyPrices);   // Rent a car
                break;
            case 3:
                returnCar(cars, NUM_CAR);   // Return a car
                break;
            case 4:
                saveToFile(cars, dailyPrices, hourlyPrices, NUM_CAR, customers, custCount);    // Save data to file
                cout << "\nData saved to file." << endl;
                break;
            case 5:
                cout << "\nExiting program." << endl;   // Exit the program
                break;
            default:
                cout << "\nInvalid choice. Please try again." << endl;   // Output for invalid choice
        }
    } while (choice != 5);

    return 0;
}

// Function to display the main menu
void displayMenu() 
{
    cout << "\nCar Rental System Menu:" << endl;
    cout << "1. Display available cars" << endl;
    cout << "2. Rent a car" << endl;
    cout << "3. Return a car" << endl;
    cout << "4. Save data to file" << endl;
    cout << "5. Exit" << endl;
    cout << "\nEnter your choice: ";
}

// Function to display available cars with their details
void displayCars(const detailCar cars[][MAX_CUST], const double dailyPrices[], const double hourlyPrices[], int numCar) 
{
	// Output for headers
    cout << left << setw(10) << "\nCar ID" << setw(20) << "Model" << setw(15) << "Price per Day(RM)" 
		 << setw(15) << "Price per Hour(RM)" << setw(10) << "Status" << endl;
    for (int i = 0; i < numCar; ++i) 
	{
		string status;
        if (cars[i][0].isRented) {
            status = "Rented";
        } else {
            status = "Available";
        }
        cout << left << setw(10) << cars[i][0].carID << setw(20) << cars[i][0].model 
			 << setw(15) << fixed << setprecision(2) << dailyPrices[i] << setw(15) << fixed << setprecision(2) << hourlyPrices[i] << setw(10) << status << endl;
    } 
	cout << "\nCar rental for more than 5 days will get a 20% discount | Car rental for more than 8 hours will get a 10% discount" << endl;
}

// Function to rent a car
void rentCar(detailCar cars[][MAX_CUST], int numCar, cust customers[], int &custCount, const double dailyPrices[], const double hourlyPrices[]) {    //custCount is passed by reference 
    if (custCount >= MAX_CUST) 
	{
        cout << "\nSorry, maximum number of customers reached." << endl;
        return;
    }

    string carID;
    cout << "Enter Car ID to rent: ";
    cin >> carID;
    cin.ignore();
    bool found = false;

    // Search for the car with the given ID
    for (int i = 0; i < numCar; ++i) 
	{
        if (cars[i][0].carID == carID) 
		{
            found = true;
            if (!cars[i][0].isRented)   //bool isRented is false means the car is not rented yet
			{
                cars[i][0].isRented = true;   //mark the car as rented

				// Get customer details
                cout << "Enter customer name: ";
                getline(cin, customers[custCount].name); 
                cout << "Enter customer phone number: ";
                getline(cin, customers[custCount].phone); 

				// Get rental details
                char rentType;
                cout << "Enter rent car by days (D) or by hours (H)? ";
                cin >> rentType;

                customers[custCount].rentalType = rentType;  // Initialize rentalType

                Price priceDetails;
                if (rentType == 'D' || rentType == 'd')
				{
                    cout << "Enter number of days for rent: ";
                    cin >> customers[custCount].days; // Get the rental duration
                    priceDetails = calculateTotalPrice(customers[custCount].days, dailyPrices[i], 1);    // Call calculateTotalPrice function & stored in the priceDetails variable
                    
                } 
				else if (rentType == 'H' || rentType == 'h') 
				{
                    cout << "Enter number of hours for rent: ";
                    cin >> customers[custCount].hours; // Get the rental duration
                    priceDetails = calculateTotalPrice(customers[custCount].hours, hourlyPrices[i], 2);    // Call calculateTotalPrice function & stored in the priceDetails variable
                } 
				else 
				{
                    cout << "Invalid choice. Please try again." << endl;
                    return;
                }

                customers[custCount].totalPrice = priceDetails.priceAfterDiscount;    // Update customer's total price

                custCount++; // Increment customer count
                cout << "\nCar rented successfully." << endl;

                // Display rental details
                displayRentalDetails(customers[custCount - 1], cars[i][0], priceDetails);    // Call displayRentalDetails function
            }
			else 
			{
                cout << "\nCar is already rented." << endl;
            }
            break;
        }
    }
    if (!found) 
	{
        cout << "\nCar ID not found." << endl;
    }
}

// Function to return a rented car
void returnCar(detailCar cars[][MAX_CUST], int numCar) 
{
    string carID;
    cout << "Enter Car ID to return: ";
    cin >> carID;
    bool found = false;

    // Search for the car with the given ID
    for (int i = 0; i < numCar; ++i) 
	{
        if (cars[i][0].carID == carID) 
		{
            found = true;
            if (cars[i][0].isRented) 
			{
                cars[i][0].isRented = false; // Mark the car as available
                cout << "\nCar returned successfully." << endl;
            } 
			else 
			{
                cout << "\nCar is not rented." << endl;
            }
            break;
        }
    }
    if (!found) 
	{
        cout << "\nCar ID not found." << endl;
    }
}

// Function to save data to a file
void saveToFile(const detailCar cars[][MAX_CUST], const double dailyPrices[], const double hourlyPrices[], int numCar, const cust customers[], int custCount) {
    ofstream outFile("summary.txt"); // Create and open a text file for writing
    if (outFile.is_open()) 
	{
		// Write car details to the file
        outFile << left << setw(10) << "Car ID" << setw(20) << "Model" << setw(15) << "Price per Day(RM)" 
				<< setw(15) << "Price per Hour(RM)" << setw(10) << "Status" << endl;
        for (int i = 0; i < numCar; ++i) 
		{
            string status;
            if (cars[i][0].isRented) {
                status = "Rented";
            } else {
                status = "Available";
            }
           outFile << left << setw(10) << cars[i][0].carID << setw(20) << cars[i][0].model << setw(15) << fixed << setprecision(2) << dailyPrices[i] 
				   << setw(15) << fixed << setprecision(2) << hourlyPrices[i] << setw(10) << status << endl;
        }    
        
        outFile << endl;
        // Write customer details to the file
        for (int i = 0; i < custCount; ++i) 
		{
            outFile << "Customer name: " << customers[i].name << endl;
            outFile << "Customer phone: " << customers[i].phone << endl;
            if (customers[i].days > 0) 
			{
                outFile << "Duration rented (days): " << customers[i].days << endl;
            } 
			else 
			{
                outFile << "Duration rented (hours): " << customers[i].hours << endl;
            }
            outFile << "Total price: RM" << fixed << setprecision(2) << customers[i].totalPrice << endl;
            outFile << endl;
        }

        double totalRevenue = calculateTotalRevenue(customers, custCount);     // Calculate total revenue (call calculateTotalRevenue function & pass 2 parameter's value)
        outFile << "Total Revenue: RM" << fixed << setprecision(2) << totalRevenue << endl; // Write total revenue to file

        outFile.close(); // Close the file after writing
    } 
	else 
	{
        cout << "Unable to open file for writing." << endl;
    }
}

// Struct function to calculate total price based on rental type and apply discount if applicable
Price calculateTotalPrice(int duration, double price, int type) 
{
    Price priceDetails;   // declares a variable priceDetails of type Price, which is a structure that will be used to store the price details.
    priceDetails.priceBeforeDiscount = duration * price;     // Calculate price before discount
    priceDetails.discount = 0.0;

    if (type == 1) 
	{ // daily
        if (duration > 5) 
		{
            priceDetails.discount = priceDetails.priceBeforeDiscount * 0.2;    // Apply 20% discount for more than 5 days
        }
    } 
	else if (type == 2) 
	{ // hourly
        if (duration > 8) 
		{
            priceDetails.discount = priceDetails.priceBeforeDiscount * 0.1;   // Apply 10% discount for more than 8 hours
        }
    }

    priceDetails.priceAfterDiscount = priceDetails.priceBeforeDiscount - priceDetails.discount;    // Calculate price after discount

    return priceDetails; // Return the priceDetails as a returning data type to the called function
}

// Function to display rental details
void displayRentalDetails(const cust& customer, const detailCar& car, const Price& priceDetails) 
{
    cout << "\nRental Details:" << endl;
    cout << "Name: " << customer.name << endl;
    cout << "Phone: " << customer.phone << endl;
    cout << "Car Model: " << car.model << endl;
    if (customer.rentalType == 'D' || customer.rentalType == 'd') 
	{
        cout << "Days Rented: " << customer.days << endl;
    } 
	else if (customer.rentalType == 'H' || customer.rentalType == 'h') 
	{
        cout << "Hours Rented: " << customer.hours << endl;
    }
    cout << "Price before discount: RM" << fixed << setprecision(2) << priceDetails.priceBeforeDiscount << endl;
    cout << "Discount: RM" << fixed << setprecision(2) << priceDetails.discount << endl;
    cout << "Total Price: RM" << fixed << setprecision(2) << priceDetails.priceAfterDiscount << endl;
}

// Function to calculate total revenue from all rentals
double calculateTotalRevenue(const cust* customers, int custCount)      // Uses the pointer customers to access the elements of an array of cust struct.
{
    double totalRevenue = 0.0;
    for (int i = 0; i < custCount; ++i) 
	{
        totalRevenue += customers[i].totalPrice;     // Sum up the total prices of all customers 
    }												// customers[i] is equivalent to *(customers + i), which dereferences the pointer customers and accesses the i-th element of the array.
    return totalRevenue;   // Return value the total revenue
}
