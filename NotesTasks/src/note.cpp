#include "note.h"

Note::Note(QObject *parent)
    : QObject(parent)
{
    m_createdAt = QDateTime::currentDateTime();
    m_updatedAt = QDateTime::currentDateTime(); // Устанавливаем текущее время
}

Note::Note(const QString &title, const QString &content)
{
    m_title = title;
    m_content = content;
    m_createdAt = QDateTime::currentDateTime();
    m_updatedAt = QDateTime::currentDateTime(); // Устанавливаем текущее время
}

Note::Note(const QString &title, const QString &content, const int &priority)
{
    m_title = title;
    m_content = content;
    m_createdAt = QDateTime::currentDateTime();
    m_updatedAt = QDateTime::currentDateTime(); // Устанавливаем текущее время
    m_priority = priority;
}

// Конструктор копирования
Note::Note(const Note &other)
    : QObject(other.parent())
{
    m_id = other.m_id;
    m_title = other.m_title;
    m_content = other.m_content;
    m_createdAt = other.m_createdAt;
    m_updatedAt = other.m_updatedAt;
    m_priority = other.m_priority;
    m_mediaItems = other.m_mediaItems; // Здесь будет поверхностное копирование
}

Note& Note::operator=(const Note &other)
{
    if (this != &other) {
        m_id = other.m_id;
        m_title = other.m_title;
        m_content = other.m_content;
        m_createdAt = other.m_createdAt;
        m_updatedAt = other.m_updatedAt;
        m_priority = other.m_priority;
        m_mediaItems = other.m_mediaItems; // Здесь будет поверхностное копирование
    }
    return *this;
}

Note::~Note(){
}

int Note::getId()
{
    return m_id;
}

void Note::setId(int id)
{
    if (m_id != id) {
        m_id = id;
    }
}

QString Note::getTitle()
{
    return m_title;
}

void Note::setTitle(const QString &title)
{
    m_title = title;
}

QString Note::getContent()
{
    return m_content;
}

void Note::setContent(const QString &content)
{
    m_content = content;
}

QDateTime Note::getCreatedAt()
{
    return m_createdAt;
}

void Note::setCreatedAt()
{
    m_createdAt = QDateTime::currentDateTime();
}

void Note::setCreatedAt(QDateTime time)
{
    m_createdAt = time;
}


QDateTime Note::getUpdatedAt()
{
    return m_updatedAt;
}

void Note::setUpdatedAt()
{
    m_updatedAt = QDateTime::currentDateTime();
}

void Note::setUpdatedAt(QDateTime time)
{
    m_updatedAt = time;
}

int Note::getPriority()
{
    return m_priority;
}

void Note::setPriority(const int &priority)
{
    if (m_priority != priority)
    {
        m_priority = priority;
    }
}
QList<MediaItem*> Note::mediaItems()
{
    return m_mediaItems;
}

void Note::addMediaItem(MediaItem *mediaItem)
{
    m_mediaItems.append(mediaItem);
}
