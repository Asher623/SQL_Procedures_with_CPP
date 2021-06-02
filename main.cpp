
#include <iostream>
#include <iomanip>
#include <occi.h>

using oracle::occi::Environment;
using oracle::occi::Connection;

using namespace oracle::occi;
using namespace std;

Environment* enviroment = nullptr;
Connection* connection = nullptr;
Statement* stmt = nullptr;
ResultSet* rs = nullptr;

string str;
string user = "dbs311_203f02";
string pass = "31882135";
string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";


struct ShoppingCart {

	int product_id;

	double price;

	int quantity;

};

int struct_size = 0;

int mainMenu();
int customerLogin(Connection* conn, int customerId);
int addToCart(Connection* conn, struct ShoppingCart cart[]);
double findProduct(Connection* conn, int product_id);
void displayProducts(struct ShoppingCart cart[], int productCount);
int checkout(Connection* conn, struct ShoppingCart cart[], int customerId, int productCount);

int mainMenu(void) {
	int choice = 0;

	ShoppingCart cart[5];
	while (choice <= 0 || choice >= 1) {
		std::cout << "******************** Main Menu ********************" << endl;
		std::cout << "1) Login" << endl;
		std::cout << "0) Exit" << endl;

		std::cout << "Enter an option (0-1): ";
		std::cin >> choice;
		if (choice <= 0 && choice >= 1) {
			do {
				cout << "You entered a wrong value.Enter an option(0 - 1) : ";
				std::cin >> choice;
			} while (choice <= 0 && choice >= 1);

		}
		if (choice == 1) {
			int customerId = 0;
			std::cout << "Enter employee number: ";
			cin >> customerId;

			if (customerLogin(connection, customerId) != 1)
				std::cout << "The customer does not exist." << endl;
			else {
				int choice = addToCart(connection, cart);
				displayProducts(cart, struct_size);
				int checkOrder = checkout(connection, cart, customerId, struct_size);
			}

		}
		else if (choice == 0)
		{
			cout << "Good bye!...";
			break;
		}

	}
	return choice;
}

int customerLogin(Connection* conn, int customerId) {
	int check = 0;

	stmt = conn->createStatement("BEGIN find_customer(:1, :2); END; ");
	stmt->setInt(1, customerId);
	stmt->registerOutParam(2, OCCINUMBER);
	stmt->executeUpdate();
	check = stmt->getNumber(2);


	return check;
}

double findProduct(Connection* conn, int product_id) {

	double product_price;

	stmt = conn->createStatement("BEGIN find_product(:1, :2); END; ");
	stmt->setInt(1, product_id);
	stmt->registerOutParam(2, OCCIDOUBLE);
	stmt->executeUpdate();
	product_price = stmt->getDouble(2);

	return product_price;

}

int addToCart(Connection* conn, struct ShoppingCart cart[]) {
	cout << "-------------- Add Products to Cart --------------" << endl;
	for (int i = 0; i < 5; i++) {
		int product_id = 0;
		double product_price = 0;
		do {

			cout << "Enter the product ID: ";
			cin >> product_id;
			product_price = findProduct(conn, product_id);
			if (product_price == 0) {
				cout << "The product does not exists. Try again...";
			}
		} while (product_price == 0);
		cout << product_price << endl;
		cart[i].price = product_price;
		cart[i].product_id = product_id;
		cout << "Enter the product Quantity: ";
		cin >> cart[i].quantity;
		struct_size++;

		cout << "Enter 1 to add more products or 0 to checkout: ";
		int choice = 0;
		cin >> choice;
		if (choice == 0) {
			return 0;
		}

	}
}

void displayProducts(struct ShoppingCart cart[], int productCount) {
	double total = 0;
	for (int i = 0; i < productCount; i++) {

		cout << "------- Ordered Products ---------" << endl;
		cout << "---Item " << i + 1 << endl;
		cout << "Product ID: " << cart[i].product_id << endl;
		cout << "Price: " << cart[i].price << endl;
		cout << "Quantity: " << cart[i].quantity << endl;
		total += cart[i].price * cart[i].quantity;

	}
	cout << "----------------------------------" << endl;
	cout << "Total: " << total << endl;

}

int checkout(Connection* conn, struct ShoppingCart cart[], int customerId, int productCount) {
	string choice = "";
	do {
		cout << "Would you like to checkout? (Y/y or N/n) ";
		cin >> choice;
		if (choice == "Y" || choice == "y") {

			int new_order_id = 0;
			stmt = conn->createStatement("BEGIN add_order(:1, :2); END; ");
			stmt->setInt(1, customerId);
			stmt->registerOutParam(2, OCCINUMBER);
			stmt->executeUpdate();
			new_order_id = stmt->getNumber(2);

			for (int i = 0; i < productCount; i++) {

				stmt = conn->createStatement("BEGIN add_order_item(:1, :2, :3, :4, :5); END; ");
				stmt->setInt(1, new_order_id);
				stmt->setInt(2, i + 1);
				stmt->setInt(3, cart[i].product_id);
				stmt->setInt(4, cart[i].quantity);
				stmt->setDouble(5, cart[i].price);
				stmt->executeUpdate();

			}
		}
		else if (choice == "N" || choice == "n") {
			cout << "The order is cancelled." << endl;
			return 0;

		}
		else {
			cout << "Wrong input. Try again..." << endl;
	}

	} while (choice != "Y" && choice != "y" && choice != "N" && choice != "n");



	cout << "The order is successfully completed." << endl;
	struct_size = 0;
	return 1;

}


int main(void) {
	int choice;
	try {
		enviroment = Environment::createEnvironment(Environment::DEFAULT);
		connection = enviroment->createConnection(user, pass, constr);

		do {
			choice = mainMenu();
		} while (choice != 0);


	}
	catch (SQLException& exception) {
		std::cout << "Exception caught.";
		std::cout << exception.getErrorCode() << ": " << exception.getMessage();
	}

};
