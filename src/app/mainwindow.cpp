/****************************************************************************
 * This file is part of System Preferences.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include <QDir>
#include <QPluginLoader>
#include <QToolBar>
#include <QAction>
#include <QLineEdit>
#include <QStackedWidget>
#include <QStyledItemDelegate>
#include <QDebug>

#include <VStandardDirectories>
#include <VPreferencesModulePlugin>

#include "mainwindow.h"
#include "categorydrawer.h"
#include "categorizedview.h"
#include "menuitem.h"
#include "menumodel.h"
#include "menuproxymodel.h"

using namespace VStandardDirectories;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set window icon and title
    setWindowTitle(tr("System Preferences"));
    setWindowIcon(QIcon::fromTheme("preferences-system"));

    // Search field
    m_search = new QLineEdit(this);
    m_search->setPlaceholderText(tr("Keywords"));

    // Actions
    createActions();

    // Tool bar
    createToolBar();

    // Stacked widget
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    // Search all the modules
    m_rootItem = new MenuItem(0);
    m_model = new MenuModel(m_rootItem, this);
    populate();

    // Main view
    m_catDrawer = new CategoryDrawer();
    m_catView = new CategorizedView(m_stackedWidget);
    m_catView->setIconSize(QSize(64, 64));
    m_catView->setSpacing(6);
    ///    m_catView->setSpacing(QDialog::spacingHint());
    m_catView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_catView->setCategoryDrawer(m_catDrawer);
    m_catView->setViewMode(QListView::IconMode);
    m_catView->setMouseTracking(true);
    m_catView->setFrameShape(QFrame::NoFrame);
    m_catView->viewport()->setAttribute(Qt::WA_Hover);

    // Delegate
#if 0
    KFileItemDelegate *delegate = new KFileItemDelegate(m_catView);
    delegate->setWrapMode(QTextOption::WordWrap);
#else
    QStyledItemDelegate *delegate = new QStyledItemDelegate(m_catView);
#endif
    m_catView->setItemDelegate(delegate);

    // Setup the model
    m_proxyModel = new MenuProxyModel(this);
    m_proxyModel->setCategorizedModel(true);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->sort(0);
    m_catView->setModel(m_proxyModel);
    connect(m_catView, SIGNAL(activated(QModelIndex)),
            this, SLOT(slotListViewClicked(QModelIndex)));

    // Add it to the stack and populate the model
    m_stackedWidget->insertWidget(0, m_catView);
    m_stackedWidget->setCurrentWidget(m_catView);
}

MainWindow::~MainWindow()
{
    delete m_rootItem;
}

void MainWindow::createActions()
{
    m_overviewAction = new QAction(tr("Overview"), this);
    m_overviewAction->setEnabled(false);
    connect(m_overviewAction, SIGNAL(triggered()),
            this, SLOT(slotOverviewTriggered()));
}

void MainWindow::createToolBar()
{
    QToolBar *toolBar = new QToolBar(tr("Tool Bar"), this);
    toolBar->addAction(m_overviewAction);

    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    toolBar->addWidget(spacerWidget);

    QAction *searchAction = toolBar->addWidget(m_search);
    searchAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    connect(searchAction, SIGNAL(triggered()),
            m_search, SLOT(setFocus()));

    addToolBar(toolBar);
}

void MainWindow::populate()
{
    QStringList dirs;
    dirs << QString("%1/preferences").arg(findDirectory(CommonPluginsDirectory))
         << QString("%1/preferences").arg(findDirectory(SystemPluginsDirectory));
    foreach(QString dir, dirs) {
        QDir pluginsDir(dir);
        foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            VPreferencesModulePlugin *plugin = qobject_cast<VPreferencesModulePlugin *>(loader.instance());
            if (!plugin)
                continue;

            foreach(QString key, plugin->keys()) {
                VPreferencesModule *module = plugin->create(key);

                // Create the category if needed
                VPreferencesModule::Category category = module->category();
                MenuItem *parent = m_categories.value(category, 0);
                if (!parent) {
                    parent = new MenuItem(m_rootItem);
                    parent->setCategory(category);
                    m_categories[category] = parent;
                    m_model->addException(parent);
                    qDebug() << "Create new category" << parent->name();
                }

                // Create the item and append its widget to the stack
                MenuItem *item = new MenuItem(parent);
                item->setModule(module);
                m_stackedWidget->addWidget(module);
                parent->sortChildrenByWeight();
                qDebug() << "Added" << item->name() << "under category" << parent->name();
            }
        }
    }

    if (m_rootItem)
        m_rootItem->sortChildrenByWeight();
}

void MainWindow::slotOverviewTriggered()
{
    // Go to the first page
    m_stackedWidget->setCurrentIndex(0);

    // Now that we are on the first page the action must be disabled
    m_overviewAction->setEnabled(false);

    // Show the search field because now we need it
    m_search->show();
}

void MainWindow::slotListViewClicked(const QModelIndex &index)
{
    MenuItem *item = index.data(Qt::UserRole).value<MenuItem *>();
    if (item->module()) {
        // Show the module
        m_stackedWidget->setCurrentWidget((VPreferencesModule *)item->module());

        // Enable the action to go to the first page
        m_overviewAction->setEnabled(true);

        // Hide the search field because it cannot be used now
        m_search->hide();
    }
}

#include "moc_mainwindow.cpp"
