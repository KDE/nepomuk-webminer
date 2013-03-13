/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
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

#include "startupdialog.h"
#include "ui_startupdialog.h"

#include "fetcherdialog.h"
#include "resourceextractor/resourceextractor.h"

#include <QtCore/QtConcurrentRun>

StartupDialog::StartupDialog(NepomukWebMiner::UI::Fetcher *f, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupDialog)
{
    m_fetcher = f;
    ui->setupUi(this);
}

StartupDialog::~StartupDialog()
{
    delete ui;
}

void StartupDialog::setFetchingUrl(const KUrl url)
{
    m_url = url;
}

void StartupDialog::cancel()
{
    m_fetcher->resourceExtractor()->cancel();
}

void StartupDialog::addStatusOutput(const QString &message)
{
    ui->textBrowser->insertPlainText(message);
    ui->textBrowser->insertPlainText(QLatin1String("\n"));
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void StartupDialog::startFetching()
{
    connect(m_fetcher->resourceExtractor(), SIGNAL(progressStatus(QString)), this, SLOT(addStatusOutput(QString)) );
    connect(m_fetcher->resourceExtractor(), SIGNAL(resourceExtarctionDone()), this, SLOT(fetchingDone()));

    m_Future = QtConcurrent::run(m_fetcher,&NepomukWebMiner::UI::Fetcher::addFetcherPath,m_url);
    m_FutureWatcher.setFuture(m_Future);
}

void StartupDialog::fetchingDone()
{
    emit accept();
}
