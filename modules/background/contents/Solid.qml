/****************************************************************************
 * This file is part of Hawaii Shell.
 *
 * Copyright (C) 2013-2015 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import Hawaii.Themes 1.0 as Themes
import org.hawaii.settings 0.1 as Settings
import org.hawaii.systempreferences.background 1.0

Item {
    property int columns: 6
    property int cellPadding: 5
    property real aspectRatio: root.width / root.height

    id: root
    width: Themes.Units.dp(640)
    height: Themes.Units.dp(480)

    Settings.ConfigGroup {
        id: bgConfig
        file: "hawaii/shellrc"
        group: "Background"

        function loadSettings() {
            bgSettings.primaryColor = bgConfig.readEntry("PrimaryColor");
        }

        function saveSettings() {
            bgConfig.writeEntry("Mode", "solid");
            bgConfig.writeEntry("PrimaryColor", bgSettings.primaryColor);
        }
    }

    QtObject {
        id: bgSettings

        property color primaryColor
    }

    SystemPalette {
        id: palette
    }

    ScrollView {
        anchors.fill: parent

        GridView {
            id: gridView
            model: ColorsModel {}
            cellWidth: parent.width / columns
            cellHeight: cellWidth / aspectRatio
            currentIndex: -1
            highlightMoveDuration: 0
            delegate: Item {
                width: gridView.cellWidth
                height: gridView.cellHeight

                Rectangle {
                    anchors {
                        fill: parent
                        margins: cellPadding
                    }
                    color: model.color

                    MouseArea {
                        id: mouse
                        anchors.fill: parent
                        onClicked: {
                            gridView.currentIndex = index;
                            bgSettings.primaryColor = parent.color;
                            bgConfig.saveSettings();
                        }
                    }
                }
            }
            highlight: Rectangle {
                radius: Themes.Units.dp(4)
                color: palette.highlight
            }
        }

        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    Component.onCompleted: {
        // Load settings
        bgConfig.loadSettings();
        for (var i = 0; i < gridView.count; i++) {
            var color = gridView.model.get(i).color;
            if (bgSettings.primaryColor === color) {
                gridView.currentIndex = i;
                return;
            }
        }
        gridView.currentIndex = -1;
    }
}