#include "listitemmodel.h"
const QStringList properties = {"title", "content", "priority", "createdat", "updatedat"};

QMap<QString, QString> ListItemModel::NoteToMap(Note* note){
    QMap<QString, QString> map;
    map["title"] = note->getTitle();
    map["content"] = (note->getContent()).simplified();
    map["priority"] = QString::number(note->getPriority());
    map["createdat"] = note->getCreatedAt().toString("dd.MM.yyyy HH:mm:ss");
    map["updatedat"] = note->getUpdatedAt().toString("dd.MM.yyyy HH:mm:ss");

    return map;
}
Note* ListItemModel::MapToNote(QMap<QString, QString> map){
    Note* note = new Note();
    note->setTitle(map["title"]);
    note->setContent(map["content"]);
    note->setPriority(map["priority"].toInt());
    note->setCreatedAt(QDateTime::fromString(map["createdat"], "dd.MM.yyyy HH:mm:ss"));
    note->setUpdatedAt(QDateTime::fromString(map["updateat"], "dd.MM.yyyy HH:mm:ss"));

    return note;
}


// Экспорт заметок в txt или csv
void ListItemModel::exportNotes(const QString &path)
{
    qDebug() << path;
    if (path != "None"){
        QString fpath = path;

        QFile file(fpath);
        qDebug() << fpath;


        if(file.open(QFile::WriteOnly | QFile::Truncate))
                {
                    QTextStream out(&file);  //Create a write stream

                    for(int i = 0; i < rowCount(); i++){
                         QMap noteMap = NoteToMap(noteData(i));
                         //QMap<QString,QString>::iterator it = noteMap.begin();

                         if (fpath.endsWith(".txt")){
                             for(int i = 0; i < noteMap.count(); i++)
                             {
                                  out << properties[i] + ":" + noteMap[properties[i]]+"\n";
                                  qDebug() << properties[i] + ":" + noteMap[properties[i]]+"\n";
                             }
                         }

                         else if (fpath.endsWith(".csv")){
                             QString line;
                             for(int i = 0; i < noteMap.count(); i++){
                                 line += noteMap[properties[i]] + ",";
                             }
                             qDebug() << line;
                             out << line;
                         }
                    }
                }
         file.close();
    }
}


//импорт из файла
void ListItemModel::importNotes(const QString &path)
{
    qDebug() << path;
    if (path != "None"){
        QFile file(path);
        if(! file.open(QFile::ReadOnly | QIODevice::Text))
            qDebug() << file.errorString();

        QTextStream in(&file); //Create an output stream

        while(!in.atEnd())
        {
            if (path.endsWith(".txt")){
                QMap<QString, QString> noteMap;
                for (int i = 0; i < properties.length(); i++){
                    QString line = in.readLine();
                    QString key = line.section(':', 0, 0);
                    qDebug() << key;
                    QString value = line.section(':', 1);
                    qDebug() << value;
                    noteMap[key] = value;
                }
                addNote(MapToNote(noteMap));
                qDebug() << "we're good";
            }
            if (path.endsWith("csv")){

                qDebug() << "entered csv";
                QMap<QString, QString> noteMap;
                QStringList list = (in.readLine()).split(",");
                qDebug() << list;
                for(int i = 0; i < list.length(); i++ ){
                    noteMap[properties[i]] = list[i];
                    qDebug() << noteMap[properties[i]];
                }
                addNote(MapToNote(noteMap));
            }
        }
    }
}
