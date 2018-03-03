#include <iostream>
#include <cstdio>

#include "db.h"

int main(void) {
	remove("test.log");
	auto database = Database::open("test.log");
	database->put("plato", "test");
	database->put("plato", "test2");
	std::cout << "Done putting!\n";
	auto data = database->get("plato");
	if(data)
		std::cout << "FOund key: " << *data << "\n";
	else
		std::cout << "Couldn't resolve!\n";

	data = database->get("plato");
	if(data)
		std::cout << "Found key:" << *data << "\n";
	else
		std::cout << "Couldn't resolve!\n";

	database->remove("plato");
	std::cout << "Key removed!\n";
	data = database->get("plato");
	if(!data)
		std::cout << "Properly deleted!\n";
	else
		std::cout << "Key wasn't deleted!\n";
	return 0;
}