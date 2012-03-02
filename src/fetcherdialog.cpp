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
#include <KDE/KDialog>
#include <KDE/KMimeType>
#include <KDE/KRun>

#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextDocument>
#include <QtGui/QTextEdit>

FetcherDialog::FetcherDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FetcherDialog)
{
    ui->setupUi(this);

    m_progressLog = new QTextDocument;

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
    connect(ui->buttonSearchDetails, SIGNAL(clicked()), this, SLOT(showSearchParameters()));

    connect(ui->buttonFetchMore, SIGNAL(clicked()), this, SLOT(fetchMoreDetails()));
    connect(m_mdf, SIGNAL(fetchedItemDetails(MetaDataParameters*,QVariantMap)), this, SLOT(fetchedItemDetails(MetaDataParameters*,QVariantMap)));
    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(saveMetaData()));

    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(cancelClose()));

    connect(m_mdf, SIGNAL(progressStatus(QString)), this, SLOT(addProgressInfo(QString)));
    connect(ui->buttonLog, SIGNAL(clicked()), this, SLOT(showProgressLog()));
    connect(ui->detailsUrl, SIGNAL(leftClickedUrl(QString)), this, SLOT(openDetailsLink(QString)));

    QGridLayout * gridLayout = new QGridLayout;
    ui->metaDataList->setLayout( gridLayout );
    gridLayout->setColumnStretch(1,10);

    ui->buttonEngineSettings->setIcon(KIcon("configure"));
    ui->buttonEngineSettings->setEnabled(false);
    ui->buttonSearchDetails->setIcon(KIcon("system-search"));
    ui->buttonSearch->setIcon(KIcon("edit-find"));
    ui->buttonSave->setIcon(KIcon("document-save"));
    ui->buttonCancel->setIcon(KIcon("dialog-cancel"));
    ui->buttonNext->setIcon(KIcon("go-next"));
    ui->buttonPrevious->setIcon(KIcon("go-previous"));
    ui->buttonLog->setIcon(KIcon("tools-report-bug"));
    ui->buttonFetchMore->setIcon(KIcon("download"));
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


void FetcherDialog::addProgressInfo(const QString &status)
{
    QTextCursor qtc(m_progressLog);
    qtc.movePosition( QTextCursor::End );
    qtc.insertText(status + QLatin1String("\n"));
}

void FetcherDialog::showProgressLog()
{
    QPointer<KDialog> log = new KDialog;
    log->setInitialSize(QSize(600,300));
    log->setButtons( KDialog::Ok );

    QTextEdit *logView = new QTextEdit(log);
    logView->setDocument(m_progressLog);
    logView->setReadOnly(true);
    log->setMainWidget(logView);

    log->exec();

    delete log;
}

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

    if(mdp->resourceType == QLatin1String("tvshow")) {
        ui->labelSeason->setVisible(true);
        ui->lineEditSeason->setVisible(true);
        ui->lineEditSeason->setText(  mdp->searchSeason );
        ui->labelEpisode->setVisible(true);
        ui->lineEditEpisode->setVisible(true);
        ui->lineEditEpisode->setText(  mdp->searchEpisode );
    }
    else {
        ui->labelSeason->setVisible(false);
        ui->lineEditSeason->setVisible(false);
        ui->labelEpisode->setVisible(false);
        ui->lineEditEpisode->setVisible(false);
    }

    ui->buttonFetchMore->setEnabled(false);

    // don't show next button if there is no next item
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

    int resourceCount = m_mdf->resourcesToUpdate( m_categoriesToFetch.at(m_currentCategory) );
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

    if(mdp->resourceType == QLatin1String("tvshow")) {
        ui->labelSeason->setVisible(true);
        ui->lineEditSeason->setVisible(true);
        ui->lineEditSeason->setText(  mdp->searchSeason );
        ui->labelEpisode->setVisible(true);
        ui->lineEditEpisode->setVisible(true);
        ui->lineEditEpisode->setText(  mdp->searchEpisode );
    }
    else {
        ui->labelSeason->setVisible(false);
        ui->lineEditSeason->setVisible(false);
        ui->labelEpisode->setVisible(false);
        ui->lineEditEpisode->setVisible(false);
    }

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
    busyFetching();

    MetaDataParameters *mdp = m_mdf->getResource( m_categoriesToFetch.at(m_currentCategory), m_currentResource);
    mdp->searchTitle = ui->lineEditTitle->text();
    mdp->searchSeason = ui->lineEditSeason->text();
    mdp->searchEpisode = ui->lineEditEpisode->text();

    int currentEngine = ui->comboBoxSearchEngine->currentIndex();
    QString engineId = ui->comboBoxSearchEngine->itemData( currentEngine ).toString();

    QWidget::setCursor( Qt::BusyCursor );

    ui->buttonSearch->setEnabled(false);
    ui->buttonNext->setEnabled(false);
    ui->buttonPrevious->setEnabled(false);

    m_mdf->searchItem( mdp, engineId );
}

void FetcherDialog::selectSearchEntry( MetaDataParameters *mdp, QVariantList searchResults)
{
    Q_UNUSED(mdp);

    finishedFetching();

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

void FetcherDialog::showSearchParameters()
{

    QPointer<KDialog> spd = new KDialog;
    spd->setInitialSize(QSize(600,300));

    QWidget *w = new QWidget(spd);
    QGridLayout *gl = new QGridLayout;
    gl->setColumnStretch(1,10);

    QVBoxLayout *vbl = new QVBoxLayout();
    vbl->addLayout(gl);
    vbl->addStretch(0);
    w->setLayout(vbl);

    MetaDataParameters *mdp = m_mdf->getResource( m_categoriesToFetch.at(m_currentCategory), m_currentResource);
    QLabel *labelTitle = new QLabel(i18n("Title:"),w);
    QLineEdit *editTitle = new QLineEdit(mdp->searchTitle,w);
    gl->addWidget(labelTitle, 0,0);
    gl->addWidget(editTitle, 0,1);
    QLabel *labelAltTitle = new QLabel(i18n("Alternative Title:"),w);
    QLineEdit *editAltTitle = new QLineEdit(mdp->searchAltTitle,w);
    gl->addWidget(labelAltTitle, 1,0);
    gl->addWidget(editAltTitle, 1,1);

    QLabel *labelYearMin = new QLabel(i18n("Year Min:"),w);
    QLineEdit *editYearMin = new QLineEdit(mdp->searchYearMin,w);
    gl->addWidget(labelYearMin, 2,0);
    gl->addWidget(editYearMin, 2,1);

    QLabel *labelYearMax = new QLabel(i18n("Year Max:"),w);
    QLineEdit *editYearMax = new QLineEdit(mdp->searchYearMax,w);
    gl->addWidget(labelYearMax, 3,0);
    gl->addWidget(editYearMax, 3,1);

    QLabel *labelAuthor = 0;
    QLineEdit *editAuthor = 0;

    QLabel *labelJournal = 0;
    QLineEdit *editJournal = 0;

    QLabel *labelSeason = 0;
    QLineEdit *editSeason = 0;

    QLabel *labelEpisode = 0;
    QLineEdit *editEpisode = 0;

    if(mdp->resourceType == QLatin1String("publication")) {
        labelAuthor = new QLabel(i18n("Author:"),w);
        editAuthor = new QLineEdit(mdp->searchAuthor,w);
        gl->addWidget(labelAuthor, 4,0);
        gl->addWidget(editAuthor, 4,1);

        labelJournal = new QLabel(i18n("Journal:"),w);
        editJournal = new QLineEdit(mdp->searchJournal,w);
        gl->addWidget(labelJournal, 5,0);
        gl->addWidget(editJournal, 5,1);
    }
    else if(mdp->resourceType == QLatin1String("tvshow")) {
        labelSeason = new QLabel(i18n("Season:"),w);
        editSeason = new QLineEdit(mdp->searchSeason,w);
        gl->addWidget(labelSeason, 4,0);
        gl->addWidget(editSeason, 4,1);

        labelEpisode = new QLabel(i18n("Episode:"),w);
        editEpisode = new QLineEdit(mdp->searchEpisode,w);
        gl->addWidget(labelEpisode, 5,0);
        gl->addWidget(editEpisode, 5,1);
    }

    spd->setMainWidget(w);

    int ret = spd->exec();

    if(ret == KDialog::Accepted) {
        mdp->searchTitle = editTitle->text();
        mdp->searchAltTitle = editAltTitle->text();
        mdp->searchYearMin = editYearMin->text();
        mdp->searchYearMax = editYearMax->text();

        if(mdp->resourceType == QLatin1String("publication")) {
            mdp->searchAuthor = editAuthor->text();
            mdp->searchJournal = editJournal->text();
        }
        else if(mdp->resourceType == QLatin1String("tvshow")) {
            mdp->searchSeason = editSeason->text();
            ui->lineEditSeason->setText( mdp->searchSeason );

            mdp->searchEpisode = editEpisode->text();
            ui->lineEditEpisode->setText( mdp->searchEpisode );
        }

        ui->lineEditTitle->setText( mdp->searchTitle );
    }

    delete spd;
}

void FetcherDialog::openDetailsLink(const QString &url)
{
    QString mimeTypeName = QLatin1String("text/html");
    /// Ask KDE subsystem to open url in viewer matching mime type
    KRun::runUrl(url, mimeTypeName, this, false, false);
}

void FetcherDialog::fetchMoreDetails()
{
    busyFetching();

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
    Q_UNUSED(itemDetails);

    showItemDetails();

    finishedFetching();
}

void FetcherDialog::saveMetaData()
{
    MetaDataParameters *mdp = m_mdf->getResource( m_categoriesToFetch.at(m_currentCategory), m_currentResource);

    busyFetching();
    m_mdf->saveItemMetaData( mdp );
    finishedFetching();
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

void FetcherDialog::busyFetching()
{
    QWidget::setCursor( Qt::BusyCursor );

    ui->buttonSearch->setEnabled(false);
    ui->buttonFetchMore->setEnabled(false);
    ui->buttonNext->setEnabled(false);
    ui->buttonPrevious->setEnabled(false);
    ui->buttonSave->setEnabled( false );
}

void FetcherDialog::finishedFetching()
{
    QWidget::setCursor( Qt::ArrowCursor );

    MetaDataParameters *mdp = m_mdf->getResource( m_categoriesToFetch.at(m_currentCategory), m_currentResource);
    ui->buttonSave->setEnabled( !mdp->metaDataSaved );

    ui->buttonSearch->setEnabled( true );

    QModelIndex currentEntry = ui->searchResults->currentIndex();
    QVariantMap entry = m_resultModel->searchResultEntry(currentEntry);
    if( entry.contains(QLatin1String("url")) ) {
        ui->buttonFetchMore->setEnabled(true);
    }

    int resourceCount = m_mdf->resourcesToUpdate( m_categoriesToFetch.at(m_currentCategory) );

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
}
