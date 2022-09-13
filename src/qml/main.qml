import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import Settings
import Backend

ApplicationWindow {
	id: root
	visible: true
	flags: Qt.Window | (Settings.alwaysOnTop ? Qt.WindowStaysOnTopHint : 0)
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
	PathView {
		id: pathView
		anchors.fill: parent
	}
}
