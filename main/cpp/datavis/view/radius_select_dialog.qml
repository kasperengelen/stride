import QtQuick 2.12
import QtQuick.Window 2.2
import QtQml.Models 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

Window {
	// reference to the mouse overlay so we can access data
	property var selectionManager;

	id: radius_select_dialog

	title: "Radius Selection"

	width: 300
	height: 230
	
	minimumHeight: height
	maximumHeight: height
	
	minimumWidth: width
	maximumWidth : width
	
	flags: Qt.WindowStaysOnTopHint | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint

	// cleanup
	onClosing: {
		selectionManager.exitSelectionMode()
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
		
		Text {
			Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
			
			id: label_select_radius
			
			text: "Selection Radius (km)"

			font.pixelSize: 18
			font.bold: true
		}
		
		
		Slider {
			id: radius_value_select
			
			Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
		
			from: 0
			to: 1
			stepSize: 0.01
			
			value: 0.4 // 30km

			onValueChanged: {
				selectionManager.updateCircRadius()
			}
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
					selectionManager.exitSelectionMode()
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

					// confirm before exiting selection mode!
					selectionManager.confirmSelection()
					radius_select_dialog.close()
					selectionManager.exitSelectionMode()
				}
			}
		} // RowLayout
	} // ColumnLayout
	
	/**
	 * Returns the specied radius in meters.
	 */ 
	function getRadius()
	{
		// we use a non-linear scale so that both small and big objects
		// can be selected.
		return Math.pow(radius_value_select.value, 4) * 2000000
	}
}