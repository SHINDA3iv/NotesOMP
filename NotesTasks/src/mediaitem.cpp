#include "mediaitem.h"

MediaItem::MediaItem(QObject *parent) : QObject(parent) {}
MediaItem::~MediaItem() {}

QString MediaItem::getType()
{
    return m_type;
}

void MediaItem::setType(const QString &type)
{
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
    }
}

QString MediaItem::getPath()
{
    return m_path;
}

void MediaItem::setPath(const QString &path)
{
    if (m_path != path) {
        m_path = path;
        emit pathChanged();
    }
}
