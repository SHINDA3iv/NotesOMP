import QtQuick 2.0
import Sailfish.Silica 1.0
import ListItemModel 1.0
import Note 1.0
import Sailfish.Pickers 1.0

Page {
    id: mainPage
    property int clickedIndex: -1 // добавляем новое свойство для хранения индекса нажатого элемента
    property Note signalNote: null
    backgroundColor: "#aaf2f0"
    property bool desc: false
    property int status: 0

    ListItemModel{
        id: notesModel
    }
    Timer {
        id: delayTimerRemove
        interval: 1000 // Задержка в миллисекундах (2000 мс = 2 секунды)
        repeat: false  // Таймер сработает только один раз
        onTriggered: {
            notesModel.removeItem(signalNote);
        }
    }
    Timer {
        id: delayTimerChange
        interval: 1000 // Задержка в миллисекундах (2000 мс = 2 секунды)
        repeat: false  // Таймер сработает только один раз
        onTriggered: {
            notesModel.updateNote(signalNote);
        }
    }
    Connections {
        target: notesModel
        onNoteRemoved: {
            signalNote = note;
            delayTimerRemove.start();
        }
        onNoteChanged: {
            signalNote = note;
            delayTimerChange.start();
        }
    }

    SilicaFlickable {
        anchors.fill: parent

        PushUpMenu{
            id: pushUpMenu
            property string importFilePath
            property string exportFilePath
            property string exportDirectoryPath
            backgroundColor: "#2ae5e0"
            highlightColor: "#94edeb"
            MenuItem {
                text: qsTr("Импортировать")
                color: "#000000"
                onClicked:{
                     pageStack.push(importFilePicker)
                }
            }
            MenuItem {
                text: qsTr("Экспортировать")
                color: "#000000"
                onClicked:{
                    var dialog = pageStack.push(Qt.resolvedUrl("ExportFile.qml"))
                    dialog.accepted.connect(function(){
                        pushUpMenu.exportFilePath = dialog.fileName
                        notesModel.exportNotes(pushUpMenu.exportFilePath? pushUpMenu.exportFilePath : "None")
                    })
                }
            }

            FilePickerPage {
                id: importFilePicker
                nameFilters: [ '*.pdf', '*.doc', '*.txt', "*.csv" ]
                onSelectedContentPropertiesChanged: {
                    pushUpMenu.importFilePath = selectedContentProperties.filePath
                    notesModel.importNotes(pushUpMenu.importFilePath? pushUpMenu.importFilePath : "None")
                }
            }

        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter

            id: buttonRow
            width: parent.width
            height: 150
            spacing: Theme.paddingMedium

            Rectangle {
                id: rectNotes
                width: 310
                height: 100
                color: "#2ae5e0"                    //цвет бекграунда кнопки
                radius: 20
                anchors.left: parent.left
                anchors.leftMargin: 40

                Button {
                    id: notesButton
                    anchors.centerIn: parent
                    height: width/2
                    color: "#000000"                //цвет текста кнопки
                    text: "Add Note"
                    backgroundColor: "#2ae5e0"      //тоже цвет бекграунда кнопки(нельзя удалять)
                    onClicked: {
                        var newNotePage = Qt.createComponent("NewNotePage.qml").createObject(mainPage, {"notesModel": notesModel});
                        pageStack.push(newNotePage);
                    }
                }
            }

            Rectangle {
                id: sortNotes
                width: 310
                height: 100
                color: "#2ae5e0"                    //цвет бекграунда кнопки
                radius: 20
                anchors.right: parent.right
                anchors.rightMargin: 40

                Button {
                    id: sortButton
                    anchors.centerIn: parent
                    height: width/2
                    color: "#000000"                //цвет текста кнопки
                    text: "Sort"
                    backgroundColor: "#2ae5e0"      //тоже цвет бекграунда кнопки(нельзя удалять)
                    onClicked: {
                        if(sortComboBox.currentIndex !== status)
                            switch(sortComboBox.currentIndex)
                            {
                                case 0:
                                    notesModel.sorting(false, false);
                                    status = 0;
                                    break;
                                case 1:
                                    notesModel.sorting(true, false);
                                    status = 1;
                                    break;
                                case 2:
                                    notesModel.sorting(true, true);
                                    status = 2;
                                    break;
                                case 3:
                                    notesModel.sorting(false, true);
                                    status = 3;
                                    break;
                                default: break;
                            }
                    }
                }
            }
        }
        ComboBox {
            id: sortComboBox
            anchors.top: buttonRow.bottom
            menu: ContextMenu {
                MenuItem {text: "Сначала старые"; color: "#000000"}
                MenuItem {text: "Сначала новые"; color: "#000000"}
                MenuItem {text: "Больший приоритет"; color: "#000000"}
                MenuItem {text: "Меньший приоритет"; color: "#000000"}
            }
            valueColor: "#000000"
            currentIndex: 0
        }
        // Контейнер для отображения раздела "Заметки"
        Item {
            anchors.top: sortComboBox.bottom
            width: parent.width - 20
            height: parent.height - notesButton.height
            // Список заметок
            ListView {
                id: notesListView
                anchors {
                    leftMargin: 40
                    bottomMargin: 100
                    fill: parent
                }
                // Устанавливаем отступ между элементами списка
                spacing: 20
                header: PageHeader { title: qsTr("Notes"); titleColor: "#000000"}
                delegate: NoteItem {
                    id: listNotesDelegate
                    property Note note: notesModel.noteData(index) ? notesModel.noteData(index) : null // Получаем Note объект

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var newNotePage = Qt.createComponent("NewNotePage.qml").createObject(mainPage,
                                                                                                 {"notesModel": notesModel,
                                                                                                  "selectedNote": note
                                                                                                 });
                            pageStack.push(newNotePage);
                        }
                    }

                    Rectangle {
                        color: "#94edeb"            //цвет бекграунда заметки
                        radius: 20
                        anchors.centerIn: parent
                        width: 650
                        height: 75

                        Label {
                            color: "#000000"        //цвет текста заметки
                            text: note ? note.getTitle(): null
                            anchors.verticalCenter: parent.verticalCenter
                            x: 10
                            width: 400              //чем больше значение, тем больше количество отображаемых символов
                            elide: Text.ElideRight
                        }
                        Label {
                            color: "#000000"        //цвет текста заметки
                            text: note ? note.getPriority(): null
                            anchors.verticalCenter: parent.verticalCenter
                            x: 600
                            width: 400              //чем больше значение, тем больше количество отображаемых символов
                            elide: Text.ElideRight
                        }
                    }
                }
                model: {
                    notesModel
                }
                VerticalScrollDecorator {}
            }
        }
    }
}
