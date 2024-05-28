#include "listitemmodel.h"
#include "dbmanager.h"
#include <typeinfo>

ListItemModel::ListItemModel(QObject *parent)
    : QAbstractListModel(parent)
{
    dbManager = new DBManager(this); // Создание экземпляра DBManager
    dbManager->initializeDatabase(); // Вызов метода initializeDatabase()
    //dbManager->clearDatabase();
    //dbManager->deleteDatabase();
    dbManager->loadNotesFromDatabase(this, true); // Загрузка заметок из базы данных в модель
    sorting(false, false);
}

ListItemModel::~ListItemModel()
{
    delete dbManager; // Освобождение памяти
}

void ListItemModel::addNoteToModel(Note *note)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_notes.append(note);
    endInsertRows();
}

void ListItemModel::addNoteToList(Note *note)
{
    m_notes.append(note);
}

void ListItemModel::addNote(Note *note)
{
    dbManager->saveNoteToDatabase(note); // Сохранение заметки в базу данных
    addNoteToModel(note);
    //dbManager->loadNotesFromDatabase(this, true);
}

void ListItemModel::addNote(const QString &title, const QString &content, const int &priority)
{
    Note *note = new Note(title, content, priority);
    dbManager->saveNoteToDatabase(note); // Сохранение заметки в базу данных
    addNoteToModel(note);
}

void ListItemModel::updateNote(Note *note)
{
    int index = note->getId();
    if(index >= m_notes.size())
    {
        m_notes.append(note);
    }
    m_notes[index - 1] = note;
    dbManager->updateNoteToDatabase(note); // Обновление заметки в базе данных
    beginResetModel();
    m_notes.clear();
    dbManager->loadNotesFromDatabase(this, false);
    endResetModel();
}

void ListItemModel::removeItem(Note *note)
{
    dbManager->deleteNoteFromDatabase(note); // Удаление заметки из базы данных
    beginRemoveRows(QModelIndex(), findNoteById(note->getId()), findNoteById(note->getId()));
    m_notes.removeAt(findNoteById(note->getId()));
    delete note;
    endRemoveRows();
}

int ListItemModel::findNoteById(int id) const
{
    for (Note* note : m_notes) {
        if (note->getId() == id) {
            return m_notes.indexOf(note);
        }
    }
    return -1; // Возвращаем nullptr, если заметка не найдена
}

int ListItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_notes.count();
}

QVariant ListItemModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_notes.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return m_notes.at(index.row())->getTitle();

    return QVariant();
}

Note *ListItemModel::noteData(int index)
{
    if(index == -1){
        return nullptr;
    }
    if(index >= m_notes.size())
    {
        addNoteToModel(dbManager->loadNoteById(index + 1));
    }
    else{
        if(m_notes[index] == nullptr){
            m_notes[index] = dbManager->loadNoteById(index + 1);
        }
    }
    return m_notes.at(index);
}

Note ListItemModel::note(int index)
{
    if(index == -1){
        return Note();
    }
    if(index >= m_notes.size())
    {
        addNoteToModel(dbManager->loadNoteById(index + 1));
    }
    else{
        if(m_notes[index] == nullptr){
            m_notes[index] = dbManager->loadNoteById(index + 1);
        }
    }
    return *m_notes.at(index);// Используем конструктор копирования
}

void ListItemModel::sorting(bool desc, bool critPrior)
{
    auto compare = [desc, critPrior](Note *a, Note *b) {
        if (critPrior) {
            if (a->getPriority() == b->getPriority()) {
                // Сортируем по алфавиту, если приоритеты равны
                if (a->getTitle() == b->getTitle()) {
                    return false; // Не менять порядок, если заголовки совпадают
                }
                return a->getTitle().compare(b->getTitle(), Qt::CaseInsensitive) < 0;
            }
            return desc ? a->getPriority() > b->getPriority() : a->getPriority() < b->getPriority();
        } else {
            if (a->getUpdatedAt() == b->getUpdatedAt()) {
                // Сортируем по алфавиту, если время обновления совпадает
                if (a->getTitle() == b->getTitle()) {
                    return false; // Не менять порядок, если заголовки совпадают
                }
                return a->getTitle().compare(b->getTitle(), Qt::CaseInsensitive) < 0;
            }
            return desc ? a->getUpdatedAt() > b->getUpdatedAt() : a->getUpdatedAt() < b->getUpdatedAt();
        }
    };
    beginResetModel();
    std::stable_sort(m_notes.begin(), m_notes.end(), compare);
    endResetModel();
}

int ListItemModel::size()
{
    return m_notes.size();
}
