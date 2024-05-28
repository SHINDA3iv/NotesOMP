import QtQuick 2.0
import Sailfish.Silica 1.0
import ListItemModel 1.0
import Sailfish.Pickers 1.0

Dialog{
    property string fileName
    property string fileType
    property string directory
    DialogHeader{
        id: header
        acceptText: "Принять"
        cancelText: "Отмена"
    }
    TextField{
        anchors.top: header.bottom
        id: fileInput
        placeholderText: "Имя файла"
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
    Button{
        text: "Выбрать папку"
        anchors.top: csv.bottom
        anchors.topMargin: 20
        onClicked: pageStack.push(exportPathPicker)
    }

    onAccepted:{
        if(txt.checked)
            fileType = ".txt"
        if(csv.checked)
            fileType = ".csv"
        fileName = directory + "/" + fileInput.text + fileType
    }

    FolderPickerPage {
        id: exportPathPicker
        dialogTitle: "Скачать в"
        onSelectedPathChanged: {
            directory = selectedPath
        }
    }
}
