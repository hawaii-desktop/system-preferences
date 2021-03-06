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

#ifndef OUTPUTSMODEL_H
#define OUTPUTSMODEL_H

#include <QtCore/QAbstractItemModel>

#include "waylandconfig.h"

class OutputsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool configurationEnabled READ isConfigurationEnabled NOTIFY configurationEnabledChanged)
public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        NumberRole,
        ManufacturerRole,
        ModelRole,
        PrimaryRole,
        EnabledRole,
        AspectRatioRole,
        AspectRatioStringRole,
        PositionRole,
        DiagonalSizeRole,
        ResolutionRole,
        CurrentModeRole,
        ModesRole,
        TransformRole
    };
    Q_ENUM(Roles)

    enum Transform {
        TransformNormal = 0,
        Transform90,
        Transform180,
        Transform270
    };
    Q_ENUM(Transform)

    OutputsModel(QObject *parent = 0);
    ~OutputsModel();

    bool isConfigurationEnabled() const;

    QHash<int, QByteArray> roleNames() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE void applyConfiguration(int outputNumber, int modeId, const Transform &transform);

Q_SIGNALS:
    void error(const QString &msg);
    void configurationEnabledChanged(bool value);

private:
    WaylandConfig *m_config;
    QVector<Output *> m_list;
};

#endif // OUTPUTSMODEL_H
