import QtQuick 2.0
import Sailfish.Silica 1.0
import ListItemModel 1.0
import Note 1.0
import MediaItem 1.0

Page {
    id: newNotePage
    objectName: "newNotePage"
    backgroundColor: "#aaf2f0"
    allowedOrientations: Orientation.All

    property ListItemModel notesModel
    property Note selectedNote: null

    Column {
        width: parent.width
        height: parent.height
        spacing: Theme.paddingMedium

        Row {
            anchors.horizontalCenter: parent.horizontalCenter

            id: buttonRow
            width: parent.width
            height: 150
            spacing: Theme.paddingMedium

            Rectangle {
                width: 300
                height: 100
                color: "#2ae5e0"                        //цвет бекграунда кнопки
                radius: 20
                anchors.left: parent.left
                anchors.leftMargin: 50

                Button {
                    width: 300
                    height: parent.height
                    text: "Удалить"
                    onClicked: {
                        pageStack.pop();
                        // Проверяем, что выбранная заметка существует
                        if (selectedNote) {
                            notesModel.noteRemoved(selectedNote);
                        }
                    }
                }
            }

            Rectangle {
                width: 300
                height: 100
                color: "#2ae5e0"                        //цвет бекграунда кнопки
                radius: 20
                anchors.right: parent.right
                anchors.rightMargin: 50

                Button {
                    width: 300
                    height: parent.height
                    text: "Сохранить"
                    onClicked: {
                        if (selectedNote) {
                            selectedNote.setTitle(titleField.text);
                            selectedNote.setContent(contentArea.text);
                            selectedNote.setUpdatedAt();
                            selectedNote.setPriority(parseInt(comboBox.value));
                            notesModel.noteChanged(selectedNote);
                        } else {
                            notesModel.addNote(titleField.text, contentArea.text, parseInt(comboBox.value));
                        }
                        pageStack.pop();
                    }
                }
            }
        }

        Label {
            color: "#000000"
            text: "Выберите приоритет"
            x: 200
        }

        ComboBox {
            id: comboBox
            menu: ContextMenu {
                MenuItem {text: "1"}
                MenuItem {text: "2"}
                MenuItem {text: "3"}
                MenuItem {text: "4"}
                MenuItem {text: "5"}
            }
            currentIndex: selectedNote ? selectedNote.getPriority() - 1 : 0
        }

        TextField {
            id: titleField
            placeholderText: "Заголовок"
            placeholderColor: "#7f8181"
            width: parent.width
            color: "#000000"                //цвет текста заголовка
            text: selectedNote ? selectedNote.getTitle() : ""
        }

        TextArea {
            id: contentArea
            placeholderText: "Введите текст"
            placeholderColor: "#7f8181"
            width: parent.width
            height: parent.height - titleField.height
            color: "#000000"                //цвет текста заметки
            text: selectedNote ? selectedNote.getContent() : ""
        }
    }
}
