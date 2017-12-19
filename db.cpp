#include <fstream>
#include <stdexcept>
#include <iostream>

#include "db.h"

using namespace std::experimental;

namespace {
	class DatabaseFile {
	public:
		DatabaseFile(const std::string& fileName)
		: stream(fileName, std::fstream::app | std::fstream::in)
		{
			if(!stream.is_open())
				throw std::invalid_argument("Unable to open database file: " + fileName);
		}

		void append(const std::string& key, const std::string& data) {
			stream.seekp(std::fstream::end);
			stream << key << "," << data << "\n";
			stream.flush();
		}

		void readReset() {
			stream.seekg(std::fstream::beg);
		}

		bool getLine(std::string& line) {
			return static_cast<bool>(std::getline(stream, line));
		}

	private:
		std::fstream stream;
	};

	class DefaultDatabase : public Database
	{
	public:
		DefaultDatabase(const std::string& databaseFileName)
		: Database()
		, file(databaseFileName) { }

		~DefaultDatabase() override = default;

		void put(const std::string& key, const std::string& data) override {
			file.append(key, data);
		}

		optional<std::string> get(const std::string& key) const override {
			file.readReset();
			optional<std::string> data;
			std::string entry;
			const auto keyPlusDelimeter = key + ",";
			std::cout << "Looking for key: " << key << "\n";
			while(file.getLine(entry)) {
				std::cout << "considering entry: " << entry << "\n";
				if(entry.compare(0, keyPlusDelimeter.length(), keyPlusDelimeter) == 0)
					data = entry.substr(keyPlusDelimeter.length());
			}
			return data;
		}

	private:
		mutable DatabaseFile file;
	};
}

Database::~Database() = default;
Database::Database() = default;

std::unique_ptr<Database> Database::open(const std::string& databaseFileName) {
	return std::make_unique<DefaultDatabase>(databaseFileName);
}
