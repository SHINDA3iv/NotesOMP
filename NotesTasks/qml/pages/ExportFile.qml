import QtQuick 2.0
import Sailfish.Silica 1.0
import ListItemModel 1.0
import Sailfish.Pickers 1.0

Dialog{
    property string fileName
    property string fileType
    property string directory
    backgroundColor: "#aaf2f0"
    DialogHeader{
        id: header
        acceptText: "Принять"
        cancelText: "Отмена"

    }
    TextField{
        anchors.top: header.bottom
        id: fileInput
        color: "#000000"
        placeholderText: "Имя файла"
        placeholderColor: "#7f8181"
    }
    TextSwitch{
        anchors.top: fileInput.bottom
        id: txt

        text: "txt"
        onCheckedChanged:  {
            csv.checked = false
        }
    }
    TextSwitch{
        anchors.top: txt.bottom
        id: csv
        text: "csv"
        onCheckedChanged: {
            txt.checked = false
        }
    }

    Rectangle {
        anchors.top: csv.bottom
        anchors.topMargin: 20
        width: 300
        height: 100
        color: "#2ae5e0"                        //цвет бекграунда кнопки
        radius: 20
        anchors.left: parent.left
        anchors.leftMargin: 210

        Button{
            color: "#000000"
            text: "Выбрать папку"
            width: 300
            height: parent.height
            backgroundColor: "#2ae5e0"
            onClicked: pageStack.push(exportPathPicker)
        }
    }

    onAccepted:{
        if(txt.checked)
            fileType = ".txt"
        if(csv.checked)
            fileType = ".csv"
        fileName = directory + "/" + fileInput.text + fileType
        console.log(fileName)
    }

    FolderPickerPage {
        id: exportPathPicker
        dialogTitle: "Скачать в"
        onSelectedPathChanged: {
            directory = selectedPath
        }
    }
}
