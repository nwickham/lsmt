#include <fstream>
#include <stdexcept>

#include <iostream>

#include "db.h"

using std::experimental::optional;
using std::string;
using std::ifstream;
using std::ofstream;
using std::unique_ptr;
using std::make_unique;

namespace {

	enum class Type : char {
		Add = 'A',
		Delete = 'D'
	};

	class Record {
	public:
		Record(const string& keyToSet, Type typeToSet, const optional<string> valueToSet)
		: key(keyToSet)
		, type(typeToSet)
		, value(valueToSet)
		{}

		const string key;
		const Type type;
		const optional<string> value;
	};

	struct Data {
		Data(const string& dataString)
		: value(dataString.data()),
		length(dataString.size()) {}
		const char* value;
		const size_t length;
	};

	class DatabaseLog {
	public:
		DatabaseLog(const string& fileName)
		: ostream(fileName, ofstream::app | ofstream::binary)
		, istream(fileName, ifstream::in | ifstream::binary)
		{
			if(!ostream.is_open())
				throw std::invalid_argument("Unable to open database file for write: " + fileName);
			if(!istream.is_open())
				throw std::invalid_argument("Unable to open database file for read: " + fileName);
		}

		void appendRecord(const string& key, const string& value) {
			appendType(Type::Add);
			append(key);
			append(value);
			ostream.flush();
		}

		void appendRecord(const string& key) {
			appendType(Type::Delete);
			append(key);
			ostream.flush();
		}

		void readReset() {
			istream.clear();
			istream.seekg(ifstream::beg);
			std::cout << istream.tellg() << "\n";
		}

		optional<Record> findNext(const string& key) {
			std::cout << "Running findNext\n";
			while(auto record = readRecord()) {
				std::cout << "Looking at record " << record->key << "\n";
				if(record->key == key)
					return record;
			}
			return {};
		}

	private:
		void append(const string& dataString) {
			appendData({dataString});
		}

		void appendData(const Data& data) {
			appendLength(data.length);
			appendDataValue(data);
		}

		void appendLength(size_t length) {
			ostream.seekp(ofstream::end);
			ostream.write(reinterpret_cast<const char*>(&length), sizeof(length));
		}

		void appendDataValue(const Data& toAppend) {
			ostream.seekp(ofstream::end);
			ostream.write(toAppend.value, toAppend.length);
		}

		void appendType(Type type) {
			ostream.seekp(ofstream::end);
			ostream.write(reinterpret_cast<const char*>(&type), sizeof(type));
		}

		optional<Record> readRecord() {
			std::cout << "Reading type!\n";
			const auto type = readType();
			if(!type)
				return {};
			std::cout << "Type is " << static_cast<char>(*type) << "\n";

			std::cout << "Reading key!\n";
			const auto key = readRecordPart();
			if(!key)
				return {};
			std::cout << "Key is " << *key << "\n";

			optional<string> value;
			if(*type == Type::Add)
				value = readRecordPart();

			return Record(*key, *type, value);
		}

		optional<Type> readType() {
			Type type;
			istream.read(reinterpret_cast<char*>(&type), sizeof(type));
			if(istream.eof())
				return {};
			return type;
		}

		optional<string> readRecordPart() {
			const auto length = readLength();
			if(!length)
				return {};
			const auto value = readData(*length);
			return value;
		}

		optional<size_t> readLength() {
			size_t length;
			istream.read(reinterpret_cast<char*>(&length), sizeof(length));
			if(istream.eof())
				return {};
			return length;
		}

		optional<string> readData(size_t lengthToRead) {
			unique_ptr<char[]> data = make_unique<char[]>(lengthToRead);
			istream.read(data.get(), lengthToRead);
			if(istream.eof())
				return {};
			return string(data.release(), lengthToRead);
		}

		ofstream ostream;
		ifstream istream;
	};

	class DefaultDatabase : public Database
	{
	public:
		DefaultDatabase(const string& databaseFileName)
		: Database()
		, file(databaseFileName) { }

		~DefaultDatabase() override = default;

		void put(const string& key, const string& value) override {
			file.appendRecord(key, value);
		}

		optional<string> get(const string& key) const override {
			optional<string> value;

			file.readReset();
			while(const auto record = file.findNext(key))
				value = record->value;

			return value;
		}

		void remove(const string& key) override {
			file.appendRecord(key);
		}

	private:
		mutable DatabaseLog file;
	};
}

Database::~Database() = default;
Database::Database() = default;

unique_ptr<Database> Database::open(const string& databaseFileName) {
	return make_unique<DefaultDatabase>(databaseFileName);
}
