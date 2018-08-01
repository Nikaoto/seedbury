#include "cocos2d.h"
#include "DBManager.h"
#include "sqlite3.h"
#include "Plant.h"

// Constants
const std::string DBManager::DATABASE_NAME = "seedbury.db";
const std::string DBManager::TABLE_PLANTS = "Plants";
const std::string DBManager::COLUMN_ID = "ID";
const std::string DBManager::COLUMN_LAND_NUMBER = "LandNumber";
const std::string DBManager::COLUMN_PLANT_TIME = "PlantTime";
const std::string DBManager::COLUMN_PLANT_TYPE = "PlantType";

// Point instance to null
DBManager* DBManager::dbManager = nullptr;

// Private constructor, called only once upon initialization
DBManager::DBManager() {
    // Init database and path
    database = nullptr;
    // Create/open database
    openDatabase();
    // Create tables if not present
    {
        std::ostringstream query;
        query<<"CREATE TABLE IF NOT EXISTS "<<TABLE_PLANTS<<" ("<<COLUMN_ID<<" INTEGER PRIMARY KEY AUTOINCREMENT"
            <<", "<<COLUMN_LAND_NUMBER<<" INTEGER, "<<COLUMN_PLANT_TIME<<" INTEGER, "<<COLUMN_PLANT_TYPE<<" TEXT);";
        int result = sqlite3_exec(database, query.str().c_str(), NULL, NULL, NULL);
        CCASSERT(result == SQLITE_OK, "Failed to create Plants table!");
    }
    // Run test statement
//    {
//        sqlite3_stmt *stmt;
//        auto query = std::string("SELECT name FROM sqlite_master WHERE type='table' AND name='").append(TABLE_PLANTS).append("'");
//        int result = sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, NULL);
//        CCASSERT(result == SQLITE_OK, "Failed to select Plants table name!");
//        sqlite3_step(stmt);
//        const char* table_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
//        CCASSERT(std::string(table_name) == TABLE_PLANTS, "Plants table doesn't exist");
//        sqlite3_finalize(stmt);
//    }
}

DBManager* DBManager::getInstance() {
    if (dbManager == nullptr) {
        dbManager = new DBManager();
    }
    return dbManager;
}

void DBManager::openDatabase() {
    if (database == nullptr) {
        std::string dbPath = cocos2d::FileUtils::getInstance()->getWritablePath().append(DATABASE_NAME);
        int result = sqlite3_open_v2(dbPath.c_str(), &database, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
        CCASSERT(result == SQLITE_OK, "Failed to open/create database!");
    }
}

void DBManager::closeDatabase() {
    if (database != nullptr) {
        int result = sqlite3_close_v2(database);
        CCASSERT(result == SQLITE_OK, "Failed to close database!");
        database = nullptr;
    }
}

void DBManager::savePlant(int landNumber, unsigned long plantTime, std::string plantType) {
    std::ostringstream query;
    query << "INSERT INTO " << TABLE_PLANTS << " VALUES (null, " << landNumber << ", " << plantTime << ", '" << plantType <<"');";
    int result = sqlite3_exec(database, query.str().c_str(), NULL, NULL, NULL);
    CCASSERT(result == SQLITE_OK, std::string("Failed sql command: ").append(query.str()).c_str());
}

Plant* DBManager::getPlant(int landNumber) {
    std::ostringstream query;
    query << "SELECT * FROM " << TABLE_PLANTS << " WHERE " << COLUMN_LAND_NUMBER << "=" << landNumber;
    sqlite3_stmt* stmt;
    
    int result = sqlite3_prepare_v2(database, query.str().c_str(), -1, &stmt, NULL);
    CCASSERT(result == SQLITE_OK, std::string("Failed query: ").append(query.str()).c_str());
    sqlite3_step(stmt);
    if (sqlite3_column_type(stmt, 0) == SQLITE_NULL) {
        sqlite3_finalize(stmt);
        return nullptr;
    } else {
        unsigned long plantTime = sqlite3_column_int(stmt, 2);
        std::string plantType(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        sqlite3_finalize(stmt);
        return Plant::create(plantTime, plantType);
    }
}

cocos2d::Map<int, Plant*> DBManager::getPlants() {
    std::ostringstream query;
    query << "SELECT * FROM " << TABLE_PLANTS;
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(database, query.str().c_str(), -1, &stmt, NULL);
    CCASSERT(result == SQLITE_OK, std::string("Failed query: ").append(query.str()).c_str());
    sqlite3_step(stmt);
    cocos2d::Map<int, Plant*> plantMap = cocos2d::Map<int, Plant*>();
    while (result != SQLITE_DONE) {
        if (sqlite3_column_type(stmt, 0) != SQLITE_NULL) {
            int landNumber = sqlite3_column_int(stmt, 1);
            unsigned long plantTime = sqlite3_column_int(stmt, 2);
            const char* plantType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            // Add plant to map
            plantMap.insert(landNumber, Plant::create(plantTime));
            CCLOG("plant data: %li, %i", plantTime, landNumber);
        }
        result = sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    return plantMap;
}
