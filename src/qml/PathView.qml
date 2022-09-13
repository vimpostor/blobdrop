import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import Backend

ListView {
	id: pathView
	model: PathModel
	visible: count
	spacing: 2
	header: Rectangle {
		width: parent.width
		height: pathView.count ? 32 : 0
		Behavior on height { NumberAnimation { duration: 300; easing.type: Easing.InOutSine }}
		visible: pathView.count
		color: Material.accent
		Text {
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
		Rectangle {
			id: rectangle
			anchors.fill: parent
			color: !exists ? Material.color(Material.Red) : used ? Material.primary : Material.color(Material.Grey)
			Behavior on color { ColorAnimation { duration: 200; easing.type: Easing.InOutSine }}
			Text {
				anchors.centerIn: parent
				text: path
				ToolTip.text: path
				ToolTip.visible: dragArea.containsMouse
				ToolTip.delay: 1500
				ToolTip.timeout: 2000
			}
		}
		DragArea {
			id: dragArea
			anchors.fill: parent
			target: rectangle
			dragUri: uri
			hoverEnabled: true
			onDragFinished: (dropAction) => {
				PathModel.taint_used(index)
			}
		}
	}
}
