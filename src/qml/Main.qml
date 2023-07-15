import QtQuick
import QtQuick.Controls.Material

import Backend

ApplicationWindow {
	id: root
	visible: true
	flags: Qt.Window | (Settings.alwaysOnTop ? Qt.WindowStaysOnTopHint : 0) | (Settings.alwaysOnBottom ? Qt.WindowStaysOnBottomHint : 0)
	title: Stdin.closed ? "Blobdrop" : "Reading from stdin..."
	width: 400
	height: Math.max(48, Math.min(800, pathView.count ? pathView.contentHeight + 2 * pathView.anchors.topMargin : 350))
	Material.theme: Material.System
	Material.primary: Material.Green
	Material.accent: Material.Pink
	Shortcut {
		sequence: StandardKey.Quit
		onActivated: Qt.quit();
	}
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
		anchors.leftMargin: 4
		anchors.rightMargin: 4
		anchors.topMargin: 4
	}
}
