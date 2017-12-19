#include <iostream>

#include "db.h"

int main(void) {
	auto database = Database::open("test.txt");
	database->put("plato", "test");
	auto data = database->get("plato");
	if(data)
		std::cout << *data << "\n";
	else
		std::cout << "Couldn't resolve!\n";
	return 0;
}