import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import NotesApp 1.0

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: "Simple Notes"

    NoteModel {
        id: notesModel
        controller: noteController
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        //left column
        ColumnLayout {
            Layout.preferredWidth: 300
            Layout.minimumWidth: 300
            Layout.maximumWidth: 300
            Layout.fillHeight: true

            //text notes count
            Text {
                text: "Number of notes: " + notesListView.count
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
                Layout.margins: 5
            }

            //sort buttons
            RowLayout {
                //sort by date
                Button {
                    text: "Sort by date"
                    onClicked: {
                        notesListView.currentIndex = -1
                        noteController.sortDate()
                    }
                }
                //sort by priority
                Button {
                    text: "Sort by priority"
                    onClicked: {
                        notesListView.currentIndex = -1
                        noteController.sortPriority()
                    }
                }
            }

            //list of notes
            ListView {
                id: notesListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: notesModel
                clip: true

                delegate: ItemDelegate {
                    width: notesListView.width
                    highlighted: ListView.isCurrentItem

                    //choosing of note color
                    background: Rectangle {
                        color: {
                            let baseColor = "white"
                            switch (model.priority) {
                                case 1: baseColor = "white"; break;
                                case 2: baseColor = "#e8f5e9"; break;
                                case 3: baseColor = "#fff9c4"; break;
                                case 4: baseColor = "#ffe0b2"; break;
                                case 5: baseColor = "#ffcdd2"; break;
                            }
                            return ListView.isCurrentItem ? Qt.darker(baseColor, 1.1) : baseColor
                        }

                        border.color: ListView.isCurrentItem ? "#2196f3" : "#e0e0e0"
                        border.width: ListView.isCurrentItem ? 2 : 1

                        radius: 4
                    }

                    contentItem: Column {
                        anchors.margins: 10

                        Text { text: model.title; font.pixelSize: 14; font.bold: true }
                        Text { text: model.timestamp; font.pixelSize: 13; color: "gray" }
                    }

                    onClicked: {
                        //note saving
                        if (notesListView.currentIndex !== -1 && notesListView.currentIndex !== index) {
                            let oldId = notesListView.model.data(notesListView.model.index(notesListView.currentIndex, 0), 257)
                            noteController.editNote(oldId, titleEdit.text, textEdit.text, priorityCombo.currentIndex + 1)
                        }

                        //changing the note
                        notesListView.currentIndex = index
                        notesListView.forceActiveFocus()
                    }
                }
            }

            //add new note
            Button {
                text: "+ Add new note"
                Layout.fillWidth: true
                onClicked: {
                    noteController.addNote("New", "")
                    notesListView.currentIndex = notesListView.count - 1
                    notesListView.positionViewAtEnd()
                }
            }
        }

        //spacing line
        Rectangle { width: 1; Layout.fillHeight: true; color: "#ccc" }

        // right column (note editing)
        ColumnLayout {
            id: editPanel
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 20
            visible: notesListView.currentIndex !== -1

            // Helper function to save the note
            function saveCurrentNote() {
                if (notesListView.currentIndex !== -1) {
                    let currentId = notesListView.model.data(notesListView.model.index(notesListView.currentIndex, 0), 257)
                    noteController.editNote(currentId, titleEdit.text, textEdit.text, priorityCombo.currentIndex + 1)
                }
            }

            // title of note
            TextField {
                id: titleEdit
                text: notesListView.currentItem ? notesListView.model.data(notesListView.model.index(notesListView.currentIndex, 0), 258) : "" // TitleRole
                Layout.fillWidth: true
                placeholderText: "Title"

                // Save when title editing is finished (Enter pressed or focus lost)
                onEditingFinished: editPanel.saveCurrentNote()
            }

            // choosing of priority
            ComboBox {
                id: priorityCombo
                model: ["Just Info", "Low", "Medium", "High", "Urgent"]
                currentIndex: notesListView.currentItem ? notesListView.model.data(notesListView.model.index(notesListView.currentIndex, 0), 260) - 1 : 0 // PriorityRole

                // Save when priority is changed
                onActivated: editPanel.saveCurrentNote()
            }

            // text of note
            ScrollView {
                id: textScrollView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn

                // entering the note text
                TextArea {
                    id: textEdit
                    width: textScrollView.availableWidth

                    text: notesListView.currentItem ? notesListView.model.data(notesListView.model.index(notesListView.currentIndex, 0), 259) : ""
                    placeholderText: "Text here..."
                    wrapMode: TextArea.Wrap

                    // Save when text area loses focus
                    onActiveFocusChanged: {
                        if (!activeFocus && notesListView.currentIndex !== -1) {
                            editPanel.saveCurrentNote()
                        }
                    }
                }
            }

            // delete note
            Button {
                text: "Delete note"
                Layout.fillWidth: true
                enabled: notesListView.currentIndex !== -1

                onClicked: {
                    if (notesListView.currentIndex !== -1) {
                        let currentId = notesListView.model.data(notesListView.model.index(notesListView.currentIndex, 0), 257)
                        noteController.deleteNote(currentId)
                        notesListView.currentIndex = -1
                    }
                }
            }
        }
    }
}

