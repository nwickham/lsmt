#include "db.h"

namespace {
	class DefaultDatabase : public Database
	{
		void put(const std::string& /*key*/, const std::string& /*data*/) override {

		}

		std::string get(const std::string& /*key*/) const {
			return {};
		}
	};
}

Database::~Database() = default;
Database::Database() = default;

std::unique_ptr<Database> Database::open(const std::string& /*databaseFileName*/) {
	return std::make_unique<DefaultDatabase>();
}
