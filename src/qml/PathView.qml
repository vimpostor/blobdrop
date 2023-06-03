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
		}
		DragArea {
			anchors.fill: parent
			target: parent
			dragUri: PathModel.foldedUriList
			onPreDragStarted: {
				PathModel.refresh_folded_paths();
			}
			onDragFinished: (dropAction) => {
				PathModel.taint_all_used();
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
				ToolTip.visible: dragArea.containsMouse
				ToolTip.delay: 1500
				ToolTip.timeout: 2000
			}
			Button {
				id: iconButton
				anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
				width: height
				visible: thumbnail == ""
				icon.name: iconName
				icon.source: thumbnail
				icon.color: "transparent"
				icon.width: parent.height
				icon.height: parent.height
				flat: true
				enabled: false
			}
			Image {
				anchors.fill: iconButton
				visible: !iconButton.visible
				source: thumbnail
				asynchronous: true
			}
			Rectangle {
				anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
				width: 8
				color: !exists ? Material.color(Material.Red) : used ? Material.primary : Material.color(Material.Grey)
				Behavior on color { ColorAnimation { duration: 200; easing.type: Easing.InOutSine }}
			}
		}
		DragArea {
			id: dragArea
			anchors.fill: parent
			target: iconButton
			dragUri: uri
			hoverEnabled: true
			onClicked: PathModel.open(index);
			onDragFinished: (dropAction) => {
				PathModel.taint_used(index)
			}
		}
	}
}
