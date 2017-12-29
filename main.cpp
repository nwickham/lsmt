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
		std::cout << *data << "\n";
	else
		std::cout << "Couldn't resolve!\n";
	return 0;
}