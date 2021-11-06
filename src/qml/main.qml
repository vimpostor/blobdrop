import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import Backend

ApplicationWindow {
	id: root
	visible: true
	width: 485
	height: 300
	Material.theme: Material.System
	Material.primary: Material.Green
	Material.accent: Material.Cyan
	ListView {
		anchors.fill: parent
		model: PathModel
		delegate: Item {
			height: 64
			anchors.left: parent.left
			anchors.right: parent.right
			Rectangle {
				anchors.fill: parent
				color: used ? Material.accent : Material.primary
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
