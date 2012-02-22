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

#include "fetcherdialog.h"
#include "ui_fetcherdialog.h"

#include "metadatafetcher.h"
#include "selectenginedialog.h"

#include <QtGui/QApplication>

FetcherDialog::FetcherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FetcherDialog)
{
    ui->setupUi(this);

    m_mdf = new MetaDataFetcher;

    connect(ui->buttonSelect, SIGNAL(clicked()), this, SLOT(selectEngine()));
    connect(ui->buttonStart, SIGNAL(clicked()), this, SLOT(startSearch()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(cancelClose()));
    connect(m_mdf, SIGNAL(progressStatus(QString)), this, SLOT(setProgressInfo(QString)));
    connect(m_mdf, SIGNAL(fileFetchingDone()), this, SLOT(fileFetchingDone()));
}

FetcherDialog::~FetcherDialog()
{
    delete ui;
}

void FetcherDialog::setInitialPathOrFile( const KUrl &url )
{
    m_mdf->lookupFiles(url);
}

void FetcherDialog::setProgressInfo(const QString &status)
{
    ui->statusText->append( status );
}

void FetcherDialog::fileFetchingDone()
{
    ui->buttonSelect->setEnabled(true);

    m_fileTypesToFetch = m_mdf->availableFileTypes();

    showNextFileTypeInfoString();
}

void FetcherDialog::showNextFileTypeInfoString()
{
    QString nextType = m_fileTypesToFetch.first();

    if( nextType == QLatin1String("publication")) {
        ui->statusText->append( i18n("Please select the engine you want to use for the publication search.") );
    }
    else if( nextType == QLatin1String("movie")) {
        ui->statusText->append( i18n("Please select the engine you want to use for the movie search.") );
    }
    else if( nextType == QLatin1String("tvshow")) {
        ui->statusText->append( i18n("Please select the engine you want to use for the tvshow search.") );
    }
}

void FetcherDialog::selectEngine()
{
    QString nextType = m_fileTypesToFetch.first();
    QVariantList engines = m_mdf->availableSearchEngines( nextType );

    QPointer<SelectEngineDialog> sed = new SelectEngineDialog;
    sed->setEngines(engines);
    sed->exec();

    QString engine = sed->selectedEngine();
    m_mdf->setUsedEngine( nextType, engine);

    ui->buttonStart->setEnabled(true);

    delete sed;
}

void FetcherDialog::startSearch()
{
    QString nextType = m_fileTypesToFetch.takeFirst();

    m_mdf->startFetching(nextType);
}

void FetcherDialog::cancelClose()
{
    qApp->quit();
}
