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
			MouseArea {
				id: mouseArea
				anchors.fill: parent
				drag.target: draggable
				hoverEnabled: true
				preventStealing: true
				onEntered: {
					rectangle.grabToImage(function(result) {
						draggable.Drag.imageSource = result.url;
					}, Qt.size(50, 30));
				}
			}
			Item {
				id: draggable
				anchors.fill: parent
				Binding on Drag.active {
					value: mouseArea.drag.active
					delayed: true
				}
				Drag.hotSpot.x: 0
				Drag.hotSpot.y: 0
				Drag.mimeData: { "text/uri-list": uri }
				Drag.dragType: Drag.Automatic
				Drag.onDragFinished: (dropAction) => {
					PathModel.taint_used(index)
				}
			}
		}
	}
}
