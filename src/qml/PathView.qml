import QtQuick
import QtQuick.Controls.Material

import Backend

ListView {
	id: pathView
	model: PathModel
	visible: count
	spacing: 6
	header: Button {
		width: parent.width
		height: pathView.count > 1 ? 40 : 0
		Behavior on height { NumberAnimation { duration: 300; easing.type: Easing.InOutSine }}
		visible: pathView.count > 1
		highlighted: true
		Label {
			anchors.centerIn: parent
			text: "Drag all " + pathView.count + " items"
			Button {
				id: dragallDummy
				visible: false
				icon.name: "emblem-documents"
				icon.color: "transparent"
				width: 64
				height: 64
			}
		}
		DragArea {
			anchors.fill: parent
			target: dragallDummy
			dragUri: PathModel.foldedUriList
			Component.onCompleted: {
				if (Settings.immediateDrag) {
					PathModel.drag_immediately();
				} else if (Settings.printHyperlinks) {
					PathModel.print_hyperlinks();
				} else if (Settings.sendNotification) {
					PathModel.send_notification();
				}
			}
			onPreDragStarted: {
				PathModel.refresh_folded_paths();
			}
			onDragStarted: Settings.alwaysOnBottom = true
			onDragFinished: (dropAction) => {
				PathModel.taint_all_used();
				Settings.alwaysOnBottom = false;
			}
		}
	}
	delegate: Item {
		height: 64
		width: ListView.view.width
		Pane {
			id: pane
			anchors.fill: parent
			Material.elevation: 6
			padding: 0
			Label {
				anchors.centerIn: parent
				text: pretty
				ToolTip.text: path
				ToolTip.visible: dragArea.containsMouse && (count > 1)
				ToolTip.delay: 1500
				ToolTip.timeout: 2000
			}
			Button {
				id: iconButton
				anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
				width: height
				visible: thumbnail == ""
				icon.name: iconName
				icon.source: "qrc:///blobdrop" // fallback icon
				icon.color: "transparent"
				icon.width: 2 * parent.height
				icon.height: 2 * parent.height
				flat: true
				enabled: false
			}
			Image {
				anchors.fill: iconButton
				visible: !iconButton.visible
				source: thumbnail
				fillMode: Image.PreserveAspectCrop
				asynchronous: true
			}
			Rectangle {
				anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
				width: 8
				color: used ? Material.primary : exists ? Material.color(Material.Grey) : Material.color(Material.Red)
				Behavior on color { ColorAnimation { duration: 200; easing.type: Easing.InOutSine }}
			}
		}
		DragArea {
			id: dragArea
			anchors.fill: parent
			target: iconButton
			dragUri: uri
			hoverEnabled: true
			acceptedButtons: Qt.LeftButton | Qt.RightButton
			onClicked: PathModel.open(index);
			onDragStarted: Settings.alwaysOnBottom = true
			onDragFinished: (dropAction) => {
				PathModel.taint_used(index)
				Settings.alwaysOnBottom = false;
			}
		}
	}
}
