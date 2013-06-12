/****************************************************************************
 * This file is part of System Preferences.
 *
 * Copyright (C) 2013 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:GPL2+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0
import Hawaii.SystemPreferences 0.1

ApplicationWindow {
    id: root
    title: qsTr("System Preferences")
    width: 640
    height: 640
    minimumWidth: 640
    minimumHeight: 640

    toolBar: ToolBar {
        id: mainToolBar
        height: searchEntry.height

        RowLayout {
            anchors.fill: parent
            //spacing: 10

            ToolButton {
                action: actionBack
                visible: pageStack.depth > 1
            }

            Label {
                id: prefletTitle
                font.bold: true
                horizontalAlignment: Qt.AlignHCenter
                visible: pageStack.depth > 1

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter
            }

            TextField {
                id: searchEntry
                placeholderText: qsTr("Keywords")
                //visible: pageStack.depth === 1
                visible: false

                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
            }
        }
    }

    SystemPalette {
        id: palette
    }

    Action {
        id: actionBack
        iconName: "go-previous-symbolic"
        onTriggered: pageStack.pop()
    }

    StackView {
        id: pageStack
        anchors.fill: parent

        initialItem: Item {
            width: parent.width
            height: parent.height

            ColumnLayout {
                anchors.fill: parent

                Repeater {
                    model: CategoriesModel {}

                    GroupBox {
                        title: label
                        style: GroupBoxStyle {
                            padding {
                                top: (control.title.length > 0 || control.checkable ? 16 : 0) + 20
                                left: 16
                            }

                            panel: Rectangle {
                                anchors.fill: parent
                                color: index % 2 ? palette.window : palette.alternateBase

                                Label {
                                    anchors {
                                        top: parent.top
                                        leftMargin: 4
                                        topMargin: 4
                                        rightMargin: 4
                                    }
                                    text: control.title
                                    font.bold: true
                                    renderType: Text.NativeRendering
                                }
                            }
                        }

                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        ScrollView {
                            anchors.fill: parent

                            GridView {
                                id: gridView
                                model: PrefletsProxyModel {}
                                cellWidth: width / 6
                                delegate: GridDelegate {
                                    width: gridView.cellWidth

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            prefletTitle.text = model.title;
                                            pageStack.push({item: model.item, properties: {stackView: pageStack}})
                                        }
                                    }
                                }

                                Component.onCompleted: gridView.model.setFilterFixedString(name)
                            }
                        }
                    }
                }
            }
        }
    }
}