#ifndef LISTITEMMODEL_H
#define LISTITEMMODEL_H

#include "note.h"

#include <QAbstractListModel>
#include <QStandardItemModel>
#include <QFile>
#include <QDebug>
#include <QTextStream>

class DBManager;

class ListItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ListItemModel(QObject *parent = nullptr);
    ~ListItemModel();

    Q_INVOKABLE void addNoteToModel(Note *note);
    Q_INVOKABLE void addNoteToList(Note *note);
    Q_INVOKABLE void addNote(Note *note);
    Q_INVOKABLE void addNote(const QString &title, const QString &content, const int &priority);
    Q_INVOKABLE void updateNote(Note *note);
    Q_INVOKABLE void removeItem(Note *note);
    Q_INVOKABLE int findNoteById(int id) const;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE Note *noteData(int index);
    Q_INVOKABLE Note note(int index);
    Q_INVOKABLE void sorting(bool desc, bool status);
    Q_INVOKABLE int size();

    Q_INVOKABLE void exportNotes(const QString &path);
    Q_INVOKABLE void importNotes(const QString &path);
    Q_INVOKABLE QMap<QString, QString> NoteToMap(Note* note);
    Q_INVOKABLE Note *MapToNote(QMap<QString, QString> map);

signals:
    void noteSelected(QString title, QString content);
    void noteRemoved(Note *note);
    void noteChanged(Note *note);

private:
    QList<Note *> m_notes;
    DBManager *dbManager;
};

#endif // LISTITEMMODEL_H
