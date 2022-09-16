import QtQuick
import QtQuick.Controls

MouseArea {
	property var target
	property string dragUri: ""
	signal dragStarted()
	signal dragFinished(var dropAction)
	signal preDragStarted()

	id: mouseArea
	drag.target: draggable
	preventStealing: true
	onPressed: {
		preDragStarted();
		target.grabToImage(function(result) {
			draggable.Drag.imageSource = result.url;
		});
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
		Drag.onDragStarted: dragStarted();
		Drag.onDragFinished: (dropAction) => {
			dragFinished(dropAction);
		}
	}
}
