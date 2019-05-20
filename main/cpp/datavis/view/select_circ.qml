import QtQuick 2.12
import QtQuick.Window 2.2
import QtQml.Models 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12


Window {
	// reference to the mouse overlay so we can access data
	property var mapMouseOverlay;

	id: select_circ_dialog

	title: "Radius Selection"

	width: 300
	height: 230
	
	minimumHeight: height
	maximumHeight: height
	
	minimumWidth: width
	maximumWidth : width
	
	flags: Qt.WindowStaysOnTopHint | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
	
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
			
			text: "Selection Radius"

			font.pixelSize: 18
			font.bold: true
		}
		
		
		SpinBox {
			id: radius_value_select
			
			Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
		
			from: 0
			to: 1000000
			stepSize: 1
			
			value: 1000
			
			editable: true
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
					mapMouseOverlay.exitSelectionMode()
					select_circ_dialog.close()
				}
			}
		
			Button {
				text: "Confirm"
				onClicked: {
					mapMouseOverlay.exitSelectionMode()
					select_circ_dialog.close()
				}
			}
		}
	
	}
	
	function getRadius()
	{
		return radius_value_select.value
	}

}