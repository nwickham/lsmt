#pragma once

#include <memory>
#include <string>
#include <experimental/optional>

using std::experimental::optional;
using std::string;

class Database {
public:
	virtual ~Database();

	static std::unique_ptr<Database> open(const string& databaseFileName);
	virtual void put(const string& key, const string& data) = 0;
	virtual optional<string> get(const string& key) const = 0;
	virtual void remove(const string& key) = 0;

protected:
	Database();
	Database(const Database&) = delete;
	Database(Database&& database) = delete;
};
