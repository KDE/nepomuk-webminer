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

#include "searchresultsmodel.h"
#include "searchresultdelegate.h"

#include "metadataparameters.h"
#include "resourceextractor/resourceextractor.h"
#include "webextractor/extractorfactory.h"
#include "webextractor/webextractor.h"

#include "nepomukpipe/nepomukpipe.h"
#include "nepomukpipe/moviepipe.h"
#include "nepomukpipe/publicationpipe.h"

#include <KDE/KStandardDirs>
#include <KDE/KDialog>
#include <KDE/KMimeType>
#include <KDE/KRun>
#include <KDE/KDebug>

#include <QtCore/QFileInfo>
#include <QtCore/QPointer>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextDocument>
#include <QtGui/QTextEdit>

using namespace NepomukMetaDataExtractor::Pipe;
using namespace NepomukMetaDataExtractor::WebExtractor;

FetcherDialog::FetcherDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FetcherDialog)
    , m_webextractor(0)
    , m_currentItemToupdate(0)
{
    ui->setupUi(this);

    m_progressLog = new QTextDocument;

    m_re = new ResourceExtractor;
    m_ef = new ExtractorFactory;

    m_resultModel = new SearchResultsModel(this);
    ui->searchResults->setModel( m_resultModel );

    ui->searchResults->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->searchResults->setItemDelegate(new SearchResultDelegate);

    connect(m_re, SIGNAL(resourceExtarctionDone()), this, SLOT(resourceFetchingDone()));

    connect(ui->buttonNext, SIGNAL(clicked()), this, SLOT(selectNextResourceToLookUp()));
    connect(ui->buttonPrevious, SIGNAL(clicked()), this, SLOT(selectPreviousResourceToLookUp()));

    connect(ui->buttonSearch, SIGNAL(clicked()), this, SLOT(startSearch()));
    connect(ui->searchResults->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(searchEntrySelected(QModelIndex,QModelIndex)));
    connect(ui->buttonSearchDetails, SIGNAL(clicked()), this, SLOT(showSearchParameters()));

    connect(ui->buttonFetchMore, SIGNAL(clicked()), this, SLOT(fetchMoreDetails()));
    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(saveMetaData()));

    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(cancelClose()));

    connect(m_re, SIGNAL(progressStatus(QString)), this, SLOT(addProgressInfo(QString)));
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
    delete m_re;
    delete ui;
}

void FetcherDialog::setInitialPathOrFile( const KUrl &url )
{
    m_re->lookupFiles(url);
}

void FetcherDialog::setForceUpdate(bool update)
{
    m_re->setForceUpdate(update);
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

void FetcherDialog::resourceFetchingDone()
{
    m_categoriesToFetch = m_re->availableResourceTypes();
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

    int resourceCount = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).size();
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

    int resourceCount = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).size();

    if( m_currentResource >= resourceCount) {
        m_currentCategory++;
        m_currentResource = 0;

        resourceCount = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).size();
        fillEngineList( m_categoriesToFetch.at(m_currentCategory) );
    }

    ui->labelCategoryCount->setText(i18n("Category %1 of %2 (%3)", m_currentCategory + 1, m_categoriesToFetch.size(), m_categoriesToFetch.at(m_currentCategory)));
    ui->labelResourceCount->setText(i18n("Resource %1 of %2", m_currentResource + 1, resourceCount));

    // get next resourceInformation
    MetaDataParameters *mdp = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).at( m_currentResource );

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

    int resourceCount = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).size();
    if( m_currentResource < 0) {
        m_currentCategory--;
        resourceCount = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).size();
        m_currentResource = resourceCount-1;

        fillEngineList( m_categoriesToFetch.at(m_currentCategory) );
    }

    ui->labelCategoryCount->setText(i18n("Category %1 of %2 (%3)", m_currentCategory + 1, m_categoriesToFetch.size(), m_categoriesToFetch.at(m_currentCategory)));
    ui->labelResourceCount->setText(i18n("Resource %1 of %2", m_currentResource + 1, resourceCount));

    // get next resourceInformation
    MetaDataParameters *mdp = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).at( m_currentResource );

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

    MetaDataParameters *mdp = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).at( m_currentResource );
    mdp->searchTitle = ui->lineEditTitle->text();
    mdp->searchSeason = ui->lineEditSeason->text();
    mdp->searchEpisode = ui->lineEditEpisode->text();

    int currentEngine = ui->comboBoxSearchEngine->currentIndex();
    QString engineId = ui->comboBoxSearchEngine->itemData( currentEngine ).toString();

    QWidget::setCursor( Qt::BusyCursor );

    ui->buttonSearch->setEnabled(false);
    ui->buttonNext->setEnabled(false);
    ui->buttonPrevious->setEnabled(false);

    if(!m_webextractor || m_webextractor->info().identifier != engineId) {
        delete m_webextractor;
        m_webextractor = m_ef->createExtractor( engineId );

        if(!m_webextractor) {
            kDebug() << "search engine with identifier" << engineId << "does not exist";
            return;
        }
        connect(m_webextractor, SIGNAL(searchResults(QVariantList)), this, SLOT(selectSearchEntry(QVariantList)));
        connect(m_webextractor, SIGNAL(itemResults(QVariantMap)), this, SLOT(fetchedItemDetails(QVariantMap)));
        connect(m_webextractor, SIGNAL(log(QString)), this, SLOT(addProgressInfo(QString)));
    }

    QVariantMap searchParameters;
    searchParameters.insert("title", mdp->searchTitle);
    searchParameters.insert("alttitle", mdp->searchAltTitle);
    searchParameters.insert("author", mdp->searchAuthor);
    searchParameters.insert("season", mdp->searchSeason);
    searchParameters.insert("episode", mdp->searchEpisode);
    searchParameters.insert("yearMin", mdp->searchYearMin);
    searchParameters.insert("yearMax", mdp->searchYearMax);
    searchParameters.insert("journal", mdp->searchJournal);

    m_webextractor->search( searchParameters );
}

void FetcherDialog::selectSearchEntry( QVariantList searchResults)
{
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

    MetaDataParameters *mdp = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).at( m_currentResource );
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

    m_currentItemToupdate = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).at( m_currentResource );
    KUrl fetchUrl( entry.value(QLatin1String("url")).toString() );

    m_webextractor->extractItem( fetchUrl );
}

void FetcherDialog::fetchedItemDetails(QVariantMap itemDetails)
{
    m_currentItemToupdate->metaData.insert(QLatin1String("resourceuri"), m_currentItemToupdate->resourceUri.url());

    QMapIterator<QString, QVariant> i(itemDetails);
    while (i.hasNext()) {
        i.next();
        m_currentItemToupdate->metaData.insert(i.key(), i.value());
    }
    m_currentItemToupdate = 0;

    showItemDetails();

    finishedFetching();
}

void FetcherDialog::saveMetaData()
{
    MetaDataParameters *mdp = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).at( m_currentResource );

    busyFetching();

    QString type = mdp->resourceType;

    NepomukPipe *nepomukPipe = 0;
    if(type == QLatin1String("publication")) {
        nepomukPipe = new PublicationPipe;
    }
    else if(type == QLatin1String("tvshow")) {
        nepomukPipe = 0;
        //m_nepomukPipe = new PubEntryToNepomukPipe;
    }
    else if(type == QLatin1String("movie")) {
        nepomukPipe = new MoviePipe;
    }

    if(nepomukPipe) {
        nepomukPipe->pipeImport( mdp->metaData );
        mdp->metaDataSaved = true;
    }
    else {
        addProgressInfo(i18n("No nepomuk pipe available for the resoure type %1", type));
    }

    finishedFetching();
}

void FetcherDialog::cancelClose()
{
    close();
}

void FetcherDialog::fillEngineList(const QString &category)
{
    ui->comboBoxSearchEngine->clear();

    QList<WebExtractor::Info> engines = m_ef->listAvailablePlugins( category );

    foreach(const WebExtractor::Info &engine, engines) {

        QFileInfo fileInfo(engine.file);

        QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;
        //QString iconPath = KStandardDirs::locate("data", searchString);

        ui->comboBoxSearchEngine->addItem(QIcon( iconPath ),
                                          engine.name,
                                          engine.identifier);
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
    MetaDataParameters *mdp = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).at( m_currentResource );

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

    MetaDataParameters *mdp = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).at( m_currentResource );
    ui->buttonSave->setEnabled( !mdp->metaDataSaved );

    ui->buttonSearch->setEnabled( true );

    QModelIndex currentEntry = ui->searchResults->currentIndex();
    QVariantMap entry = m_resultModel->searchResultEntry(currentEntry);
    if( entry.contains(QLatin1String("url")) ) {
        ui->buttonFetchMore->setEnabled(true);
    }

    int resourceCount = m_re->resourcesToFetch( m_categoriesToFetch.at(m_currentCategory) ).size();

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
