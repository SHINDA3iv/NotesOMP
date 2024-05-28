#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "listitemmodel.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileInfo>

class DBManager : public QObject
{
    Q_OBJECT
public:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();
    Q_INVOKABLE void initializeDatabase();
    Q_INVOKABLE void loadNotesFromDatabase(ListItemModel *model, bool add);
    Q_INVOKABLE void saveNoteToDatabase(Note *note);
    Q_INVOKABLE void updateNoteToDatabase(Note *note);
    Q_INVOKABLE void deleteNoteFromDatabase(Note *note);
    Q_INVOKABLE bool isDatabaseInitialized();
    Q_INVOKABLE void printDatabaseContents();
    Q_INVOKABLE void clearDatabase();
    Q_INVOKABLE void deleteDatabase();
    Q_INVOKABLE Note* loadNoteById(int id);

private:
    QSqlDatabase m_database;
    bool m_databaseInitialized = false; // Флаг для отслеживания инициализации базы данных
};

#endif // DBMANAGER_H
