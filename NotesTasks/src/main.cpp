#include <auroraapp.h>
#include <QtQuick>

#include "listitemmodel.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.auroraos"));
    application->setApplicationName(QStringLiteral("NotesTasks"));

    qmlRegisterType<ListItemModel>("ListItemModel", 1, 0, "ListItemModel");
    qmlRegisterType<Note>("Note", 1, 0, "Note");
    qmlRegisterType<MediaItem>("MediaItem", 1, 0, "MediaItem");

    QScopedPointer<QQuickView> view(Aurora::Application::createView());
    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/NotesTasks.qml")));
    view->show();

    return application->exec();
}
