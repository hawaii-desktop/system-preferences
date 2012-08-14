/****************************************************************************
 * This file is part of System Preferences.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 * Copyright (c) 2007 Will Stephenson
 * Copyright (c) 2009 Ben Cooksley
 *
 * Author(s):
 *    Ben Cooksley <bcooksley@kde.org>
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *    Will Stephenson <wstephenson@kde.org>
 *
 * $BEGIN_LICENSE:GPL$
 *
 * System Preferences is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * System Preferences is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with System Preferences.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef MENUPROXYMODEL_H
#define MENUPROXYMODEL_H

#include <VCategorizedSortFilterProxyModel>

/**
 * @brief Provides a filter model for MenuModel
 *
 * Provides a standardised model to be used with views to filter a MenuModel.\n
 * It automatically sorts the items appropriately depending on if it is categorised
 * or not.
 * Call setFilterRegExp(QString) with the desired text to filter to perform searching.
 * Items that do not match the search parameters will be disabled, not hidden.
 *
 * @author Will Stephenson <wstephenson@kde.org>
 * @author Ben Cooksley <bcooksley@kde.org>
 */
class MenuProxyModel : public VCategorizedSortFilterProxyModel
{
    Q_OBJECT
public:
    /**
     * Constructs a MenuProxyModel with the specified parent.
     *
     * @param parent The QObject to use as a parent.
     */
    MenuProxyModel(QObject *parent = 0);

    /**
     * Please see the Qt QSortFilterProxyModel documentation for further information.\n
     * Provides information on whether or not the QModelIndex specified by left is below right.
     *
     * @param left the QModelIndex that is being used for comparing.
     * @param right the QModelIndex to compare aganist.
     * @returns true if the left is below the right.
     */
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

    /**
     * Please see the KDE KCategorizedSortFilterProxyModel documentation for futher information.\n
     * Provides information on whether or not the QModelIndex specified by left is below right.
     *
     * @param left the QModelIndex that is being used for comparing.
     * @param right the QModelIndex to compare aganist.
     * @returns true if the left is below the right.
     */
    virtual bool subSortLessThan(const QModelIndex &left, const QModelIndex &right) const;

    /**
     * Please see the Qt QSortFilterProxyModel documentation for futher information.\n
     * Provides additional filtering of the MenuModel to only show categories which contain modules.
     *
     * @param source_column Please see QSortFilterProxyModel documentation.
     * @param source_parent Please see QSortFilterProxyModel documentation.
     * @returns true if the row should be displayed, false if it should not.
     */
    virtual bool filterAcceptsRow(int source_column, const QModelIndex &source_parent) const;

    /**
     * Please see Qt QAbstractItemModel documentation for more details.\n
     * Provides the status flags for the QModelIndex specified.
     * The item will be selectable and enabled for its status unless invalid or filtered by search terms.
     *
     * @returns The flags for the QModelIndex provided.
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /**
     * Please see Qt QAbstractItemModel documentation for more details.\n
     * Reimplemented for internal reasons.
     */
    void setFilterRegExp(const QRegExp &regExp);

    /**
     * Please see Qt QAbstractItemModel documentation for more details.\n
     * Reimplemented for internal reasons.
     */
    void setFilterRegExp(const QString &pattern);

};

#endif // MENUPROXMODEL_H