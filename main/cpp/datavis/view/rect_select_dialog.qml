import QtQuick 2.12
import QtQuick.Window 2.2
import QtQml.Models 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

/**
 * Dialog to help the user with selecting
 * a rectangular area on the map.
 */
Window {
	// reference to the mouse overlay so we can access data
	property var selectionManager;

	id: radius_select_dialog

	title: "Rectangular Selection"

	width: 300
	height: 230
	
	minimumHeight: height
	maximumHeight: height
	
	minimumWidth: width
	maximumWidth : width
	
	flags: Qt.WindowStaysOnTopHint | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint

	// cleanup
	onClosing: {
		selectionManager.exitSelectionMode(false)
	}

	/**
	 * Message dialog for errors.
	 */
	MessageDialog {
        id: noSelectionMadeErrorDialog
        title: "Error"
        text: "Please make a selection before confirming."
    }
	
	ColumnLayout {
	
		anchors.fill: parent

		Text {
			Layout.topMargin: 20
			Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
		
			text: "Selected coord"

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
					selectionManager.exitSelectionMode(false)
					radius_select_dialog.close()
				}
			}
		
			Button {
				text: "Confirm"
				onClicked: {

					// check if a selection exists
					if(!selectionManager.isSelectionMade()) {
						noSelectionMadeErrorDialog.open()
						return;
					}

					// cleanup before confirming!
					selectionManager.exitSelectionMode(false)
					radius_select_dialog.close()

					selectionManager.confirmSelection()
				}
			}
		} // RowLayout
	} // ColumnLayout
}