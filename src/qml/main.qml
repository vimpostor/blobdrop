import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import Backend

ApplicationWindow {
	id: root
	visible: true
	flags: Qt.Window | Qt.WindowStaysOnTopHint
	Connections {
		target: Stdin
		function onClosedChanged() {
			if (Stdin.closed) {
				title = "blobdrop"
			} else {
				title = "blobdrop (Reading stdin...)"
			}
		}
	}
	width: 485
	height: 300
	Material.theme: Material.System
	Material.primary: Material.Green
	Material.accent: Material.Pink
	DropArea {
		anchors.fill: parent
		onDropped: (drop) => {
			Stdout.print_urls(drop.urls);
		}
		Rectangle {
			anchors.fill: parent
			color: parent.containsDrag && parent.drag.source === null ? Material.primary : Material.background
			Behavior on color { ColorAnimation { duration: 200; easing.type: Easing.InOutSine }}
		}
	}
	Welcome {
		anchors { left: parent.left; right: parent.right; margins: 48; verticalCenter: parent.verticalCenter }
		visible: !pathView.count
	}
	Rectangle {
		id: headerRect
		anchors { left: parent.left; right: parent.right; top: parent.top }
		height: visible ? 32 : 0
		Behavior on height { NumberAnimation { duration: 300; easing.type: Easing.InOutSine }}
		visible: pathView.count > 1
		color: Material.accent
		Text {
			anchors.centerIn: parent
			text: "Drag all " + pathView.count + " items"
		}
		DragArea {
			anchors.fill: parent
			target: headerRect
			dragUri: PathModel.foldedUriList
			onPreDragStarted: {
				PathModel.refresh_folded_paths();
			}
			onDragFinished: (dropAction) => {
				PathModel.taint_all_used();
			}
		}
	}
	ListView {
		id: pathView
		anchors { left: parent.left; right: parent.right; top: headerRect.bottom; bottom: parent.bottom }
		model: PathModel
		visible: count
		clip: true
		spacing: 2
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
}
