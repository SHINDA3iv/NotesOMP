#ifndef MEDIAITEM_H
#define MEDIAITEM_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>

class MediaItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)

public:
    explicit MediaItem(QObject *parent = nullptr);
    ~MediaItem();

    Q_INVOKABLE QString getType();
    Q_INVOKABLE void setType(const QString &type);

    Q_INVOKABLE QString getPath();
    Q_INVOKABLE void setPath(const QString &path);

signals:
    void typeChanged();
    void pathChanged();

private:
    QString m_type;
    QString m_path;
};

#endif // MEDIAITEM_H
