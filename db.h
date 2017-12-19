#pragma once

#include <memory>
#include <string>
#include <experimental/optional>

using namespace std::experimental;

class Database {
public:
	virtual ~Database();

	static std::unique_ptr<Database> open(const std::string& databaseFileName);
	virtual void put(const std::string& key, const std::string& data) = 0;
	virtual optional<std::string> get(const std::string& key) const = 0;

protected:
	Database();
	Database(const Database&) = delete;
	Database(Database&& database) = delete;
};
