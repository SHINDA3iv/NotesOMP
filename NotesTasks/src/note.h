#ifndef NOTE_H
#define NOTE_H

#include "mediaitem.h"

class Note : public QObject
{
    Q_OBJECT
public:
    explicit Note(QObject *parent = nullptr);
    Note(const QString &title, const QString &content);
    Note(const QString &title, const QString &content, const int &priority);
    Note(const Note &other);
    ~Note();

    Note& operator=(const Note &other);

    Q_INVOKABLE int getId();
    Q_INVOKABLE void setId(int id);

    Q_INVOKABLE QString getTitle();
    Q_INVOKABLE void setTitle(const QString &title);

    Q_INVOKABLE QString getContent();
    Q_INVOKABLE void setContent(const QString &content);

    Q_INVOKABLE QDateTime getCreatedAt();
    Q_INVOKABLE void setCreatedAt();
    Q_INVOKABLE void setCreatedAt(QDateTime time);

    Q_INVOKABLE QDateTime getUpdatedAt();
    Q_INVOKABLE void setUpdatedAt();
    Q_INVOKABLE void setUpdatedAt(QDateTime time);

    Q_INVOKABLE int getPriority();
    Q_INVOKABLE void setPriority(const int &priority);

    Q_INVOKABLE QList<MediaItem*> mediaItems();
    Q_INVOKABLE void addMediaItem(MediaItem *mediaItem);

private:
    int m_id;
    QString m_title;
    QString m_content;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
    int m_priority;
    QList<MediaItem*> m_mediaItems;
};

Q_DECLARE_METATYPE(Note*)
#endif // NOTE_H
