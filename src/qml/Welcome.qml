import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import Settings

Column {
	spacing: 64
	Label {
		width: parent.width
		horizontalAlignment: Text.AlignHCenter
		text: "Pass file names as arguments or pipe them to stdin to make them appear here and drag them anywhere.\nAlternatively use this window as a sink by dropping files here."
		wrapMode: Text.WordWrap
	}
	CheckBox {
		anchors.horizontalCenter: parent.horizontalCenter
		text: "Keep dropped items"
		checked: Settings.keepDroppedFiles
		onCheckedChanged: Settings.keepDroppedFiles = checked;
	}
}
