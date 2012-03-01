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
#include "src/ui_fetcherdialog.h"

#include "metadatafetcher.h"
#include "searchresultsmodel.h"
#include "searchresultdelegate.h"

#include <KDE/KStandardDirs>

#include <QtGui/QGridLayout>

FetcherDialog::FetcherDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FetcherDialog)
{
    ui->setupUi(this);

    m_mdf = new MetaDataFetcher;

    m_resultModel = new SearchResultsModel(this);
    ui->searchResults->setModel( m_resultModel );

    ui->searchResults->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->searchResults->setItemDelegate(new SearchResultDelegate);

    connect(m_mdf, SIGNAL(fileFetchingDone()), this, SLOT(fileFetchingDone()));

    connect(ui->buttonNext, SIGNAL(clicked()), this, SLOT(selectNextResourceToLookUp()));
    connect(ui->buttonPrevious, SIGNAL(clicked()), this, SLOT(selectPreviousResourceToLookUp()));

    connect(ui->buttonSearch, SIGNAL(clicked()), this, SLOT(startSearch()));
    connect(m_mdf, SIGNAL(selectSearchEntry(MetaDataParameters*,QVariantList)), this, SLOT(selectSearchEntry(MetaDataParameters*,QVariantList)));
    connect(ui->searchResults->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(searchEntrySelected(QModelIndex,QModelIndex)));

    connect(ui->buttonFetchMore, SIGNAL(clicked()), this, SLOT(fetchMoreDetails()));
    connect(m_mdf, SIGNAL(fetchedItemDetails(MetaDataParameters*,QVariantMap)), this, SLOT(fetchedItemDetails(MetaDataParameters*,QVariantMap)));
    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(saveMetaData()));

    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(cancelClose()));

    //connect(m_mdf, SIGNAL(progressStatus(QString)), this, SLOT(setProgressInfo(QString)));
    //connect(m_mdf, SIGNAL(progress(int,int)), this, SLOT(setProgress(int,int)));


    QGridLayout * gridLayout = new QGridLayout;
    ui->metaDataList->setLayout( gridLayout );
}

FetcherDialog::~FetcherDialog()
{
    delete m_mdf;
    delete ui;
}

void FetcherDialog::setInitialPathOrFile( const KUrl &url )
{
    m_mdf->lookupFiles(url);
}

void FetcherDialog::setForceUpdate(bool update)
{
    m_mdf->setForceUpdate(update);
}

/*
void FetcherDialog::setProgressInfo(const QString &status)
{
    ui->statusText->append( status );
}

void FetcherDialog::setProgress(int current, int max)
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(current);
}
*/
void FetcherDialog::fileFetchingDone()
{
    m_categoriesToFetch = m_mdf->availableResourceTypes();
    m_currentCategory = 0;
    m_currentResource = -1;

    if( m_categoriesToFetch.size() > 1) {
        ui->labelCategoryCount->setVisible(true);
        ui->line->setVisible(true);
    }
    else {
        ui->labelCategoryCount->setVisible(false);
        ui->line->setVisible(false);
    }

    int resourceCount = m_mdf->resourcesToUpdate( m_categoriesToFetch.at(m_currentCategory) );
    if(resourceCount > 1 || m_categoriesToFetch.size() > 1) {
        ui->labelResourceCount->setVisible(true);
        ui->buttonPrevious->setVisible(true);
        ui->buttonNext->setVisible(true);
    }
    else {
        ui->labelResourceCount->setVisible(false);
        ui->buttonPrevious->setVisible(false);
        ui->buttonNext->setVisible(false);
    }

    // fill search engine combobox with the right items for the first category
    fillEngineList( m_categoriesToFetch.at(m_currentCategory) );

    selectNextResourceToLookUp();
}

void FetcherDialog::selectNextResourceToLookUp()
{
    m_currentResource++;

    int resourceCount = m_mdf->resourcesToUpdate( m_categoriesToFetch.at(m_currentCategory) );

    if( m_currentResource >= resourceCount) {
        m_currentCategory++;
        m_currentResource = 0;

        resourceCount = m_mdf->resourcesToUpdate( m_categoriesToFetch.at(m_currentCategory) );
        fillEngineList( m_categoriesToFetch.at(m_currentCategory) );
    }

    ui->labelCategoryCount->setText(i18n("Category %1 of %2 (%3)", m_currentCategory + 1, m_categoriesToFetch.size(), m_categoriesToFetch.at(m_currentCategory)));
    ui->labelResourceCount->setText(i18n("Resource %1 of %2", m_currentResource + 1, resourceCount));

    // get next resourceInformation
    MetaDataParameters *mdp = m_mdf->getResource( m_categoriesToFetch.at(m_currentCategory), m_currentResource);

    ui->labelDescription->setText( i18n("Fetch metadata for the resource: <b>%1</b>", mdp->resourceUri.fileName()));
    ui->lineEditTitle->setText( mdp->searchTitle );

    // don't show next button if there is no next item
    qDebug() << m_currentCategory << m_currentResource;
    if(m_currentCategory == m_categoriesToFetch.size()-1 &&
       m_currentResource == resourceCount-1) {
        ui->buttonNext->setEnabled(false);
    }
    else {
        ui->buttonNext->setEnabled(true);
    }

    // don't enable previous button, if there is no previous item
    if(m_currentCategory == 0 && m_currentResource == 0) {
        ui->buttonPrevious->setEnabled(false);
    }
    else {
        ui->buttonPrevious->setEnabled(true);
    }

    m_resultModel->clear();

    showItemDetails();
}

void FetcherDialog::selectPreviousResourceToLookUp()
{
    m_currentResource--;

    int resourceCount = m_mdf->resourcesToUpdate( m_categoriesToFetch.at(m_currentCategory) );;
    if( m_currentResource < 0) {
        m_currentCategory--;
        resourceCount = m_mdf->resourcesToUpdate( m_categoriesToFetch.at(m_currentCategory) );
        m_currentResource = resourceCount-1;

        fillEngineList( m_categoriesToFetch.at(m_currentCategory) );
    }

    ui->labelCategoryCount->setText(i18n("Category %1 of %2 (%3)", m_currentCategory + 1, m_categoriesToFetch.size(), m_categoriesToFetch.at(m_currentCategory)));
    ui->labelResourceCount->setText(i18n("Resource %1 of %2", m_currentResource + 1, resourceCount));

    // get next resourceInformation
    MetaDataParameters *mdp = m_mdf->getResource( m_categoriesToFetch.at(m_currentCategory), m_currentResource);

    ui->labelDescription->setText( i18n("Fetch metadata for the resource: <b>%1</b>", mdp->resourceUri.fileName()));
    ui->lineEditTitle->setText( mdp->searchTitle );

    // don't enable next button if there is no next item
    if(m_currentCategory == m_categoriesToFetch.size()-1 &&
       m_currentResource == resourceCount-1) {
        ui->buttonNext->setEnabled(false);
    }
    else {
        ui->buttonNext->setEnabled(true);
    }

    // don't enable previous button, if there is no previous item
    if(m_currentCategory == 0 && m_currentResource == 0) {
        ui->buttonPrevious->setEnabled(false);
    }
    else {
        ui->buttonPrevious->setEnabled(true);
    }

    m_resultModel->clear();

    showItemDetails();
}

void FetcherDialog::startSearch()
{
    ui->buttonNext->setEnabled(false);
    ui->buttonSearch->setEnabled(false);

    MetaDataParameters *mdp = m_mdf->getResource( m_categoriesToFetch.at(m_currentCategory), m_currentResource);

    int currentEngine = ui->comboBoxSearchEngine->currentIndex();
    QString engineId = ui->comboBoxSearchEngine->itemData( currentEngine ).toString();

    m_mdf->searchItem( mdp, engineId );
}

void FetcherDialog::selectSearchEntry( MetaDataParameters *mdp, QVariantList searchResults)
{
    Q_UNUSED(mdp);

    ui->buttonNext->setEnabled(true);
    ui->buttonSearch->setEnabled(true);

    m_resultModel->setSearchResults( searchResults );

    QString searchEngineName = ui->comboBoxSearchEngine->currentText();
    ui->labelSearchResults->setText( i18n("Found <b>%1</b> results via <b>%2</b>", searchResults.size(), searchEngineName));
}

void FetcherDialog::searchEntrySelected(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    QVariantMap entry = m_resultModel->searchResultEntry(current);

    ui->detailsTitle->setText( entry.value(QLatin1String("title")).toString() );
    ui->detailsText->setText( entry.value(QLatin1String("details")).toString() );

    if( entry.contains(QLatin1String("url"))) {
        ui->detailsUrl->setText( i18n("Show online"));
        ui->detailsUrl->setUrl( entry.value(QLatin1String("url")).toString() );

        ui->detailsUrl->setVisible(true);
        ui->buttonFetchMore->setEnabled(true);
    }
    else {
        ui->detailsUrl->setVisible(false);
        ui->buttonFetchMore->setEnabled(false);
    }
}

void FetcherDialog::fetchMoreDetails()
{
    // get the current item to fetch
    QModelIndex currentEntry = ui->searchResults->currentIndex();
    QVariantMap entry = m_resultModel->searchResultEntry(currentEntry);

    MetaDataParameters *mdp = m_mdf->getResource( m_categoriesToFetch.at(m_currentCategory), m_currentResource);
    KUrl fetchUrl( entry.value(QLatin1String("url")).toString() );
    m_mdf->fetchItem(mdp, fetchUrl);
}

void FetcherDialog::fetchedItemDetails(MetaDataParameters *mdp, QVariantMap itemDetails)
{
    Q_UNUSED(mdp);

    showItemDetails();
}

void FetcherDialog::saveMetaData()
{
    MetaDataParameters *mdp = m_mdf->getResource( m_categoriesToFetch.at(m_currentCategory), m_currentResource);

    ui->buttonSave->setEnabled(false);
    m_mdf->saveItemMetaData( mdp );
}

void FetcherDialog::cancelClose()
{
    close();
}

void FetcherDialog::fillEngineList(const QString &category)
{
    ui->comboBoxSearchEngine->clear();

    QVariantList engines = m_mdf->availableSearchEngines( category );

    foreach(const QVariant &engine, engines) {
        QVariantMap engineMap = engine.toMap();

        QString searchString = QLatin1String("metadataextractor/webengines/") + engineMap.value(QLatin1String("icon")).toString();
        QString iconPath = KStandardDirs::locate("data", searchString);

        ui->comboBoxSearchEngine->addItem(QIcon( iconPath ),
                                          engineMap.value(QLatin1String("name")).toString(),
                                          engineMap.value(QLatin1String("identification")).toString());
    }

    //TODO select the default engine from some config file
    ui->comboBoxSearchEngine->setCurrentIndex( 0 );
}

void FetcherDialog::showItemDetails()
{
    QLayout *mdlayout = ui->metaDataList->layout();

    QLayoutItem *child;
    while ((child = mdlayout->takeAt(0)) != 0) {
        QWidget *w = child->widget();
        delete w;
        delete child;
    }

    QGridLayout * gridLayout = qobject_cast<QGridLayout *>(mdlayout);

    // current Item metadata
    MetaDataParameters *mdp = m_mdf->getResource( m_categoriesToFetch.at(m_currentCategory), m_currentResource);

    QMapIterator<QString, QVariant> i(mdp->metaData);
    int line = 0;
    while (i.hasNext()) {
        i.next();

        QString keyString = QLatin1String("<b>") + i.key() + QLatin1String(":</b>");
        QLabel *key = new QLabel(keyString);
        key->setWordWrap(true);
        QLabel *value = new QLabel(i.value().toString());
        value->setWordWrap(true);

        gridLayout->addWidget(key, line, 0, Qt::AlignTop);
        gridLayout->addWidget(value, line, 1, Qt::AlignTop);
        line++;
    }

    if(line == 0 || mdp->metaDataSaved == true) {
        ui->buttonSave->setEnabled(false);
    }
    else {
        ui->buttonSave->setEnabled(true);
    }
}
