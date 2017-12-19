#include <iostream>

#include "db.h"

int main(void) {
	auto database = Database::open("");
	database->put("", "");
	auto data = database->get("");
	return 0;
}