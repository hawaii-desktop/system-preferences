/****************************************************************************
 * This file is part of Hawaii.
 *
 * Copyright (C) 2015-2016 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL2.1+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

import QtQuick 2.1
import QtQuick.Layouts 1.0
import Qt.labs.controls 1.0
import Hawaii.SystemPreferences 1.0
import org.hawaiios.hardware 0.1

PrefletPage {
    width: 500
    height: 500

    HardwareEngine {
        id: hardwareEngine
    }

    ColumnLayout {
        anchors {
            fill: parent
            margins: 11
        }

        GroupBox {
            title: qsTr("Devices")

            ListView {
                anchors.fill: parent
                interactive: false
                model: hardwareEngine.batteries
                delegate: BatteryDelegate { battery: modelData }
            }

            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
