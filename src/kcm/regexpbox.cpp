/*
 * Copyright 2013 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "regexpbox.h"
#include "kcm/ui_regexpbox.h"

#include "regexpmodel.h"
#include "regexpdialog.h"

#include <KDE/KIcon>

#include <QtCore/QPointer>

NepomukWebMiner::RegExpBox::RegExpBox(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegExpBox)
{
    ui->setupUi(this);

    m_model = new NepomukWebMiner::RegExpModel();
    ui->tableView->setModel(m_model);

    connect(ui->moveUpButton, SIGNAL(clicked()), this, SLOT(moveCurrentUp()));
    connect(ui->moveDownButton, SIGNAL(clicked()), this, SLOT(moveCurrentDown()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeCurrent()));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addRegExp()));
    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(editCurrent()));

    ui->moveUpButton->setIcon(KIcon("arrow-up"));
    ui->moveDownButton->setIcon(KIcon("arrow-down"));
    ui->removeButton->setIcon(KIcon("list-remove"));
    ui->addButton->setIcon(KIcon("list-add"));
    ui->editButton->setIcon(KIcon("document-edit"));
}

NepomukWebMiner::RegExpBox::~RegExpBox()
{
    delete ui;

    delete m_model;
}

void NepomukWebMiner::RegExpBox::setRegExpData(const QList<NepomukWebMiner::Extractor::RegExpData> &data)
{
    m_model->setRegExpData(data);
}

QList<NepomukWebMiner::Extractor::RegExpData> NepomukWebMiner::RegExpBox::regExpData() const
{
    return m_model->allRegExpData();
}

void NepomukWebMiner::RegExpBox::clear()
{
    m_model->clear();
}

void NepomukWebMiner::RegExpBox::addRegExp()
{
    QPointer<NepomukWebMiner::RegExpDialog> dlg = new NepomukWebMiner::RegExpDialog(this);

    int ret = dlg->exec();

    if(ret == QDialog::Accepted) {
        m_model->addRegExpData(dlg->regExpData());

        emit configChanged();
    }
}

void NepomukWebMiner::RegExpBox::editCurrent()
{
    int curIndex = ui->tableView->currentIndex().row();

    // nothing selected, so nothing needs to be edited
    if(curIndex < 0) {
        return;
    }

    QPointer<NepomukWebMiner::RegExpDialog> dlg = new NepomukWebMiner::RegExpDialog(this);

    dlg->setRegExpData(  m_model->regExpData(curIndex) );

    int ret = dlg->exec();

    if(ret == QDialog::Accepted) {
        m_model->replaceRegExpData(curIndex, dlg->regExpData());

        emit configChanged();
    }
}

void NepomukWebMiner::RegExpBox::removeCurrent()
{
    int curIndex = ui->tableView->currentIndex().row();

    if( curIndex >= 0) {
        m_model->removeData( ui->tableView->currentIndex().row() );

        emit configChanged();
    }
}

void NepomukWebMiner::RegExpBox::moveCurrentUp()
{
    int curIndex = ui->tableView->currentIndex().row();

    if( curIndex >= 0) {
        m_model->moveUp( curIndex );

        emit configChanged();
    }
}

void NepomukWebMiner::RegExpBox::moveCurrentDown()
{
    int curIndex = ui->tableView->currentIndex().row();

    if( curIndex >= 0) {
        m_model->moveDown( curIndex );

        emit configChanged();
    }
}
