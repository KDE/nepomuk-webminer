/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "selectenginedialog.h"
#include "ui_selectenginedialog.h"

#include <KDE/KStandardDirs>

#include <QtGui/QListWidgetItem>

SelectEngineDialog::SelectEngineDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectEngineDialog)
{
    ui->setupUi(this);
}

SelectEngineDialog::~SelectEngineDialog()
{
    delete ui;
}

void SelectEngineDialog::setEngines(const QVariantList &engines)
{
    foreach(const QVariant &engine, engines) {
        QVariantMap engineMap = engine.toMap();

        QString searchString = QLatin1String("metadataextractor/webengines/") + engineMap.value(QLatin1String("icon")).toString();
        QString iconPath = KStandardDirs::locate("data", searchString);

        QListWidgetItem *i = new QListWidgetItem(QIcon( iconPath ), engineMap.value(QLatin1String("name")).toString() );
        i->setData( Qt::UserRole, engineMap.value(QLatin1String("identification")).toString() );

        ui->engineList->addItem( i );
    }

    ui->engineList->setCurrentRow( 0 );
}

QString SelectEngineDialog::selectedEngine()
{
    return ui->engineList->currentItem()->data( Qt::UserRole ).toString();
}
