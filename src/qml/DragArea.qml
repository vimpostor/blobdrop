import QtQuick

MouseArea {
	property var target
	property string dragUri: ""
	property bool active: draggable.Drag.active
	signal dragStarted()
	signal dragFinished(var dropAction)
	signal preDragStarted()

	id: mouseArea
	drag.target: draggable
	preventStealing: true
	onPressed: {
		preDragStarted();
		var w = target.width;
		var h = target.height;
		if (w > 64) {
			w = 64;
			h = h / w * 64;
		}
		target.grabToImage(function(result) {
			draggable.Drag.imageSource = result.url;
		}, Qt.size(w, h));
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
