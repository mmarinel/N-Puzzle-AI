import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
	visible: true
	width: 640
	height: 480
	title: "N-Puzzle"

	property int boardSize: 4 // Change this to your desired board size
	property int tileSize: 100 // Change this to your desired tile size

	GridLayout {
		id: grid
		columns: boardSize
		width: boardSize * tileSize
		height: boardSize * tileSize

		Repeater {
			model: boardSize * boardSize

			Rectangle {
				width: tileSize
				height: tileSize
				color: (modelData === boardSize * boardSize - 1) ? "red" : "blue"
				border.color: "black"

				Text {
					anchors.centerIn: parent
					text: modelData + 1
					color: "white"
				}
			}
		}
	}
}