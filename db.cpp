#include <fstream>
#include <stdexcept>

#include "db.h"

using namespace std::experimental;

namespace {
	struct Data {
		Data(const std::string& dataString)
		: value(dataString.data()),
		length(dataString.size()) {}
		const char* value;
		const size_t length;
	};

	class DatabaseFile {
	public:
		DatabaseFile(const std::string& fileName)
		: stream(fileName, std::fstream::app | std::fstream::in | std::fstream::binary)
		{
			if(!stream.is_open())
				throw std::invalid_argument("Unable to open database file: " + fileName);
		}

		void append(const std::string& key, const std::string& data) {
			appendData({key});
			appendData({data});
			stream.flush();
		}

		void appendLength(size_t length) {
			stream.seekp(std::fstream::end);
			stream.write(reinterpret_cast<const char*>(&length), sizeof(length));
		}

		void appendDataValue(const Data& toAppend) {
			stream.seekp(std::fstream::end);
			stream.write(toAppend.value, toAppend.length);
		}

		void appendData(const Data& data) {
			appendLength(data.length);
			appendDataValue(data);
		}

		optional<size_t> readLength() {
			size_t length;
			stream.read(reinterpret_cast<char*>(&length), sizeof(length));
			if(stream.eof())
				return {};
			return length;
		}

		optional<std::string> readData(size_t lengthToRead) {
			std::unique_ptr<char[]> data = std::make_unique<char[]>(lengthToRead);
			stream.read(data.get(), lengthToRead);
			if(stream.eof())
				return {};
			return std::string(data.release(), lengthToRead);
		}

		void readReset() {
			stream.seekg(std::fstream::beg);
		}

		optional<std::string> readValue() {
			const auto length = readLength();
			if(!length)
				return {};
			const auto value = readData(*length);
			return value;
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
			const auto keyPlusDelimeter = key + ",";
			while(const auto currentKey = file.readValue()) {
				const auto currentValue = file.readValue();
				if(*currentKey == key)
					data = currentValue;
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
