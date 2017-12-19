#pragma once

#include <memory>
#include <string>

class Database {
public:
	virtual ~Database();

	static std::unique_ptr<Database> open(const std::string& databaseFileName);
	virtual void put(const std::string& key, const std::string& data) = 0;
	virtual std::string get(const std::string& key) const = 0;

protected:
	Database();	
};
