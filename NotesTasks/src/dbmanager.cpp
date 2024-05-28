// dbmanager.cpp
#include "dbmanager.h"

DBManager::DBManager(QObject *parent) : QObject(parent)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName("./notes.db");

    if (!m_database.open()) {
        qDebug() << "Error: Could not open database.";
    }
    else {
        m_databaseInitialized = true; // Устанавливаем флаг в true при успешной инициализации базы данных
    }
}

DBManager::~DBManager(){
    m_database.close();
}

void DBManager::initializeDatabase()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS notes ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "title TEXT, "
               "content TEXT, "
               "created_at TEXT, "
               "updated_at TEXT, "
               "priority TEXT)");

    query.exec("CREATE TABLE IF NOT EXISTS media ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "note_id INTEGER, "
               "type TEXT, "
               "path TEXT, "
               "FOREIGN KEY(note_id) REFERENCES notes(id))");
}

void DBManager::loadNotesFromDatabase(ListItemModel *model, bool add = false)
{
    QSqlQuery query("SELECT id, title, content, created_at, updated_at, priority FROM notes");

    while (query.next()) {
        Note *note = new Note();
        note->setId(query.value("id").toInt());
        note->setTitle(query.value("title").toString());
        note->setContent(query.value("content").toString());
        note->setCreatedAt(QDateTime::fromString(query.value("created_at").toString(), "dd.MM.yyyy HH:mm:ss"));//"created_at"
        note->setUpdatedAt(QDateTime::fromString(query.value("updated_at").toString(), "dd.MM.yyyy HH:mm:ss"));//"updated_at"
        note->setPriority(query.value("priority").toInt());

        QSqlQuery mediaQuery;
        mediaQuery.prepare("SELECT type, path FROM media WHERE note_id=:note_id");
        mediaQuery.bindValue(":note_id", note->getId());
        if (mediaQuery.exec()) {
            while (mediaQuery.next()) {
                MediaItem *mediaItem = new MediaItem();
                mediaItem->setType(mediaQuery.value(0).toString());
                mediaItem->setPath(mediaQuery.value(1).toString());
                note->addMediaItem(mediaItem);
            }
        }
        if(add)
            model->addNoteToModel(note);
        else
            model->addNoteToList(note);
    }
}

void DBManager::saveNoteToDatabase(Note *note)
{
    QSqlQuery query;
    query.prepare("INSERT INTO notes (title, content, created_at, updated_at, priority) "
                  "VALUES (:title, :content, :created_at, :updated_at, :priority)");
    query.bindValue(":title", note->getTitle());
    query.bindValue(":content", note->getContent());
    query.bindValue(":created_at", note->getCreatedAt().toString("dd.MM.yyyy HH:mm:ss"));
    query.bindValue(":updated_at", note->getUpdatedAt().toString("dd.MM.yyyy HH:mm:ss"));
    query.bindValue(":priority", note->getPriority());

    if (!query.exec()) {
        qDebug() << "Error saving note:" << query.lastError().text();
        return;
    }

    // Save media items
    for (MediaItem *mediaItem : note->mediaItems()) {
        QSqlQuery mediaQuery;
        mediaQuery.prepare("INSERT INTO media (note_id, type, path) VALUES (:note_id, :type, :path)");
        mediaQuery.bindValue(":note_id", note->getId());
        mediaQuery.bindValue(":type", mediaItem->getType());
        mediaQuery.bindValue(":path", mediaItem->getPath());

        if (!mediaQuery.exec()) {
            qDebug() << "Error saving media item:" << mediaQuery.lastError().text();
        } else {
            qDebug() << "Media item successfully inserted for note ID:" << note->getId();
        }
    }
}

void DBManager::updateNoteToDatabase(Note *note)
{
    QSqlQuery query;
    query.prepare("UPDATE notes SET title=:title, content=:content, updated_at=:updated_at, priority=:priority "
                  "WHERE id=:id");

    query.bindValue(":id", note->getId());
    query.bindValue(":title", note->getTitle());
    query.bindValue(":content", note->getContent());
    query.bindValue(":created_at", note->getCreatedAt().toString("dd.MM.yyyy HH:mm:ss"));
    query.bindValue(":updated_at", note->getUpdatedAt().toString("dd.MM.yyyy HH:mm:ss"));
    query.bindValue(":priority", note->getPriority());

    if (!query.exec()) {
        qDebug() << "Error updating note:" << query.lastError().text();
    }
    // Save new media items
    for (MediaItem *mediaItem : note->mediaItems()) {
        QSqlQuery mediaQuery;
        mediaQuery.prepare("UPDATE media SET type=:type, path=:path "
                           "WHERE note_id=:note_id");
        mediaQuery.bindValue(":note_id", note->getId());
        mediaQuery.bindValue(":type", mediaItem->getType());
        mediaQuery.bindValue(":path", mediaItem->getPath());

        if (!mediaQuery.exec()) {
            qDebug() << "Error saving media item:" << mediaQuery.lastError().text();
        }
    }
}

void DBManager::deleteNoteFromDatabase(Note *note)
{
    // Delete media items associated with the note
    QSqlQuery deleteMediaQuery;
    deleteMediaQuery.prepare("DELETE FROM media WHERE note_id = :note_id");
    deleteMediaQuery.bindValue(":note_id", note->getId());
    if (!deleteMediaQuery.exec()) {
        qDebug() << "Error deleting media items:" << deleteMediaQuery.lastError().text();
    }

    // Delete the note itself
    QSqlQuery query;
    query.prepare("DELETE FROM notes WHERE id = :id");
    query.bindValue(":id", note->getId());
    if (!query.exec()) {
        qDebug() << "Error deleting note:" << query.lastError().text();
    }
}

bool DBManager::isDatabaseInitialized()
{
    return m_databaseInitialized;
}

void DBManager::printDatabaseContents()
{
    QSqlQuery query("SELECT * FROM notes");
    qDebug() << "Notes Table:";
    while (query.next()) {
        int id = query.value("id").toInt();
        QString title = query.value("title").toString();
        QString content = query.value("content").toString();
        QString createdAt = QDateTime::fromString(query.value("created_at").toString(), "dd.MM.yyyy HH:mm:ss").toString();
        QString updatedAt = QDateTime::fromString(query.value("updated_at").toString(), "dd.MM.yyyy HH:mm:ss").toString();
        QString priority = query.value("priority").toString();
        qDebug() << "ID:" << id << "Title:" << title << "Content:" << content
                 << "Created At:" << createdAt << "Updated At:" << updatedAt
                 << "Priority:" << priority;
    }

    query.exec("SELECT * FROM media");
    qDebug() << "Media Table:";
    while (query.next()) {
        int id = query.value("id").toInt();
        int noteId = query.value("note_id").toInt();
        QString type = query.value("type").toString();
        QString path = query.value("path").toString();
        qDebug() << "ID:" << id << "Note ID:" << noteId << "Type:" << type << "Path:" << path;
    }
}

void DBManager::clearDatabase()
{
    QSqlQuery query;
    query.exec("DELETE FROM notes");
    query.exec("DELETE FROM media");
}

void DBManager::deleteDatabase()
{
    QSqlQuery query;
    query.exec("DROP TABLE notes");
    query.exec("DROP TABLE media");
}

Note* DBManager::loadNoteById(int id)
{
    QSqlQuery query;
    query.prepare("SELECT id, title, content, created_at, updated_at, priority FROM notes WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        Note *note = new Note();
        note->setId(query.value("id").toInt());
        note->setTitle(query.value("title").toString());
        note->setContent(query.value("content").toString());
        note->setCreatedAt(QDateTime::fromString(query.value("created_at").toString(), "dd.MM.yyyy HH:mm:ss"));//"created_at"
        note->setUpdatedAt(QDateTime::fromString(query.value("updated_at").toString(), "dd.MM.yyyy HH:mm:ss"));//"updated_at"
        note->setPriority(query.value("priority").toInt());

        QSqlQuery mediaQuery;
        mediaQuery.prepare("SELECT type, path FROM media WHERE note_id=:note_id");
        mediaQuery.bindValue(":note_id", note->getId());
        if (mediaQuery.exec()) {
            while (mediaQuery.next()) {
                MediaItem *mediaItem = new MediaItem();
                mediaItem->setType(mediaQuery.value(0).toString());
                mediaItem->setPath(mediaQuery.value(1).toString());
                note->addMediaItem(mediaItem);
            }
        }
        return note;
    }
    return nullptr;
}
