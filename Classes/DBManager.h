#pragma once
#include "cocos2d.h"
#include "sqlite3.h"
#include "Plant.h"

class DBManager {
private:
    static DBManager* dbManager;
    DBManager();
    int exec(const char* q) { return sqlite3_exec(database, q, NULL, NULL, NULL); };
public:
    // Constants
    static const std::string DATABASE_NAME;
    static const std::string TABLE_PLANTS;
    static const std::string COLUMN_ID;
    static const std::string COLUMN_LAND_NUMBER;
    static const std::string COLUMN_PLANT_TIME;
    static const std::string COLUMN_PLANT_TYPE;
    
    static DBManager* getInstance();
    
    // Queries
    void savePlant(int landNumber, unsigned long plantTime, std::string plantType);
protected:
    sqlite3* database;
};
