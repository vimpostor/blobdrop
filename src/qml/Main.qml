import QtQuick
import QtQuick.Controls.Material
import Quartz

import Backend

ApplicationWindow {
	id: root
	visible: Settings.needsGui
	flags: Qt.Window | (Settings.alwaysOnTop ? Qt.WindowStaysOnTopHint : 0) | (Settings.alwaysOnBottom ? Qt.WindowStaysOnBottomHint : 0) | (Settings.frameless ? Qt.FramelessWindowHint : 0)
	title: Stdin.closed ? "Blobdrop" : "Reading from stdin..."
	width: 400
	height: Math.max(48, Math.min(800, pathView.count ? pathView.contentHeight + 2 * pathView.anchors.topMargin : 350))
	Material.theme: Material.System
	Material.primary: Material.Green
	Material.accent: Material.Pink
	Component.onCompleted: {
		if (Settings.spawnOnCursor) {
			root.x = Backend.get_mouse_pos().x - width / 2;
			root.y = Backend.get_mouse_pos().y - height / 2;
		}
	}
	Shortcut {
		sequences: [StandardKey.Quit, StandardKey.Cancel, "Q"]
		onActivated: Qt.quit();
	}
	DropArea {
		enabled: !pathView.dragActive
		onDropped: (drop) => {
			Backend.handle_dropped_urls(drop.urls);
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
