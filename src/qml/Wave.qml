import QtQuick
import QtQuick.Controls.Material
import QtQuick.Effects

Rectangle {
	id: wave
	property real size: 0.0
	property int centreX: width / 2
	property int centreY: height / 2
	color: Material.primary
	visible: size > 0.0
	Behavior on size {
		NumberAnimation { duration: 300; easing.type: Easing.OutCirc; }
	}
	layer.enabled: true
	layer.effect: MultiEffect {
		maskEnabled: true
		maskSource: ShaderEffectSource {
			sourceItem: Item {
				width: wave.width
				height: wave.height
				Rectangle {
					radius: Math.ceil(Math.sqrt(Math.pow(wave.width / 2 + Math.abs(wave.centreX - wave.width / 2), 2) + Math.pow(wave.height / 2 + Math.abs(wave.centreY - wave.height / 2), 2)))
					width: wave.size * 2 * radius
					height: width
					x: wave.centreX - width / 2
					y: wave.centreY - height / 2
				}
			}
		}
	}
}
