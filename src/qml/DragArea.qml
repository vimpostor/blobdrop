import QtQuick
import QtQuick.Controls

MouseArea {
	property var target
	property string dragUri: ""
	signal dragFinished(var dropAction)

	id: mouseArea
	drag.target: draggable
	hoverEnabled: true
	preventStealing: true
	onEntered: {
		target.grabToImage(function(result) {
			draggable.Drag.imageSource = result.url;
		}, Qt.size(50, 30));
	}
	Item {
		id: draggable
		anchors.fill: parent
		Binding on Drag.active {
			value: mouseArea.drag.active
			delayed: true
		}
		Drag.mimeData: { "text/uri-list": dragUri }
		Drag.dragType: Drag.Automatic
		Drag.onDragFinished: (dropAction) => {
			dragFinished(dropAction);
		}
	}
}
