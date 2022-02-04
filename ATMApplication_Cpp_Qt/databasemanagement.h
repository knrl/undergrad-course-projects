#ifndef DATABASEMANAGEMENT_H
#define DATABASEMANAGEMENT_H

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQueryModel>

class QSqlQueryModel;

// Default connection parameters
// "SQL SERVER","DESKTOP-FKCTEH2","ATMDB","DESKTOP-FKCTEH2\\kaane","",true
class databaseManagement
{
public:    
    QSqlQueryModel *mModel = nullptr;

    databaseManagement();
    ~databaseManagement();
    databaseManagement(const QString &server,
                       const QString &driver,
                       const QString &user,
                       const QString &password,
                       const QString &databaseName,
                       const bool    &trustedConnection = true);
    bool openDatabase(QString *error = nullptr);

private:
    QSqlDatabase mDatabase;
    QString mServer;
    QString mDriver;
    QString mUser;
    QString mPassword;
    QString mDatabaseName;
    bool mTrustedConnection;
};

#endif // DATABASEMANAGEMENT_H
