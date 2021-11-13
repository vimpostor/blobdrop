import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import Backend

ApplicationWindow {
	id: root
	visible: true
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
	Material.accent: Material.Cyan
	DropArea {
		anchors.fill: parent
		onDropped: (drop) => {
			Stdout.print_urls(drop.urls);
		}
		Rectangle {
			anchors.fill: parent
			color: parent.containsDrag && parent.drag.source === null ? Material.accent : Material.background
			Behavior on color { ColorAnimation { duration: 200; easing.type: Easing.InOutSine }}
		}
	}
	Label {
		anchors {left: parent.left; right: parent.right; margins: 48; verticalCenter: parent.verticalCenter}
		horizontalAlignment: Text.AlignHCenter
		visible: !pathView.count
		text: "Pass file names as arguments or pipe them to stdin to make them appear here and drag them anywhere from there.\nAlternatively use this window as a sink by dropping files here."
		wrapMode: Text.WordWrap
	}
	ListView {
		id: pathView
		anchors.fill: parent
		model: PathModel
		header: Rectangle {
			id: headerRect
			height: 32
			width: parent.width
			visible: pathView.count
			color: Material.color(Material.Grey)
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
		delegate: Item {
			height: 64
			width: ListView.view.width
			Rectangle {
				id: rectangle
				anchors.fill: parent
				color: !exists ? Material.color(Material.Red) : used ? Material.accent : Material.primary
				Behavior on color { ColorAnimation { duration: 200; easing.type: Easing.InOutSine }}
				Text {
					anchors.centerIn: parent
					text: path
				}
			}
			DragArea {
				anchors.fill: parent
				target: rectangle
				dragUri: uri
				onDragFinished: (dropAction) => {
					PathModel.taint_used(index)
				}
			}
		}
	}
}
