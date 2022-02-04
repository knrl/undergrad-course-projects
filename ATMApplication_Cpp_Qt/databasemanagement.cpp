#include "databasemanagement.h"
#include <QSqlQuery>
#include <QSqlError>

databaseManagement::databaseManagement() { }

databaseManagement::databaseManagement(
                           const QString &driver,
                           const QString &server,
                           const QString &databaseName,
                           const QString &user,
                           const QString &password,
                           const bool    &trustedConnection)
{
    mDatabase          = QSqlDatabase::addDatabase("QODBC");
    mDatabaseName      = databaseName;
    mServer            = server;
    mDriver            = driver;
    mUser              = user;
    mPassword          = password;
    mTrustedConnection = trustedConnection;
}

bool databaseManagement::openDatabase(QString *error)
{
    mDatabase.setDatabaseName(QString("DRIVER={%1};SERVER=%2;"
                              "DATABASE=%3;UID=%4;PWD=%5;"
                              "Trusted_Connection=%6")
                              .arg(mDriver)
                              .arg(mServer)
                              .arg(mDatabaseName)
                              .arg(mUser)
                              .arg(mPassword)
                              .arg(mTrustedConnection ? "Yes" : "No"));
    if(!mDatabase.open())
    {
        if(error != nullptr)
        {
            *error = mDatabase.lastError().text();
        }
        return false;
    }
    return true;
}

databaseManagement::~databaseManagement()
{
    mDatabase.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}
