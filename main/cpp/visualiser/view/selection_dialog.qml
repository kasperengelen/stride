import QtQuick 2.12
import QtQuick.Window 2.2
import QtQml.Models 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

/**
 * Dialog to help the user with selecting areas on the map.
 */
Window {
    // reference to the mouse overlay so we can access data
    property var selectionManager;

    id: selection_dialog

    title: "Selection"

    width: 300
    height: 150
    
    minimumHeight: height
    maximumHeight: height
    
    minimumWidth: width
    maximumWidth : width
    
    flags: Qt.WindowStaysOnTopHint | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint

    // cleanup
    onClosing: {
        selectionManager.terminate(false) // terminate but don't close window yet.
    }

    /**
     * Message dialog for errors.
     */
    MessageDialog {
        id: invalidSelectionError
        title: "Error"
        text: "Please make a selection before confirming."
    }
    
    ColumnLayout {
    
        anchors.fill: parent

        Text {
            Layout.topMargin: 20
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        
            text: "Select using right-click and drag."

            font.pixelSize: 18
            font.bold: true
        }
        
        Item {
            Layout.fillHeight: true
        }
        
        RowLayout {
        
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            
            Layout.bottomMargin: 10
        
            Button {
                text: "Cancel"
                onClicked: {
                    selectionManager.terminate(true)
                }
            }
        
            Button {
                text: "Confirm"
                onClicked: {

                    if(!selectionManager.validateSelection())
                    {
                        invalidSelectionError.open()
                        return;
                    }

                    selectionManager.confirm(true)
                }
            }
        } // RowLayout
    } // ColumnLayout
}