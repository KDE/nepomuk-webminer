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
#include "nepomukpipe/tvshowpipe.h"

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
#include <QtGui/QMessageBox>

using namespace NepomukMetaDataExtractor::Pipe;
using namespace NepomukMetaDataExtractor::Extractor;
using namespace NepomukMetaDataExtractor::Dialog;

NepomukMetaDataExtractor::Dialog::FetcherDialog::FetcherDialog(QWidget *parent)
    : QDialog(parent)
    , m_webextractor(0)
    , m_currentItemToupdate(0)
{
    setupUi(this);

    m_progressLog = new QTextDocument;

    m_re = new ResourceExtractor;
    m_ef = new ExtractorFactory;

    m_resultModel = new SearchResultsModel(this);
    searchResults->setModel( m_resultModel );

    searchResults->setSelectionMode(QAbstractItemView::SingleSelection);
    searchResults->setItemDelegate(new SearchResultDelegate);

    connect(m_re, SIGNAL(resourceExtarctionDone()), this, SLOT(resourceFetchingDone()));

    connect(buttonNext, SIGNAL(clicked()), this, SLOT(selectNextResourceToLookUp()));
    connect(buttonPrevious, SIGNAL(clicked()), this, SLOT(selectPreviousResourceToLookUp()));

    connect(buttonSearch, SIGNAL(clicked()), this, SLOT(startSearch()));
    connect(searchResults->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(searchEntrySelected(QModelIndex,QModelIndex)));
    connect(buttonSearchDetails, SIGNAL(clicked()), this, SLOT(showSearchParameters()));

    connect(buttonFetchMore, SIGNAL(clicked()), this, SLOT(fetchMoreDetails()));
    connect(buttonSave, SIGNAL(clicked()), this, SLOT(saveMetaData()));

    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancelClose()));

    connect(m_re, SIGNAL(progressStatus(QString)), this, SLOT(addToProgressLog(QString)));
    connect(buttonLog, SIGNAL(clicked()), this, SLOT(showProgressLog()));
    connect(detailsUrl, SIGNAL(leftClickedUrl(QString)), this, SLOT(openDetailsLink(QString)));
    connect(cbSelectType, SIGNAL(currentIndexChanged(int)), this, SLOT(resourceTypeSelectionChanged(int)));

    QGridLayout * gridLayout = new QGridLayout;
    metaDataList->setLayout( gridLayout );
    gridLayout->setColumnStretch(1,10);

    buttonEngineSettings->setIcon(KIcon("configure"));
    buttonEngineSettings->setEnabled(false);
    buttonSearchDetails->setIcon(KIcon("system-search"));
    buttonSearch->setIcon(KIcon("edit-find"));
    buttonSave->setIcon(KIcon("document-save"));
    buttonCancel->setIcon(KIcon("dialog-cancel"));
    buttonNext->setIcon(KIcon("go-next"));
    buttonPrevious->setIcon(KIcon("go-previous"));
    buttonLog->setIcon(KIcon("tools-report-bug"));
    buttonFetchMore->setIcon(KIcon("download"));
}

NepomukMetaDataExtractor::Dialog::FetcherDialog::~FetcherDialog()
{
    delete m_re;
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::setInitialPathOrFile( const KUrl &url )
{
    m_re->lookupFiles(url);
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::setForceUpdate(bool update)
{
    m_re->setForceUpdate(update);
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::setTvShowMode(bool tvshowmode)
{

}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::errorInScriptExecution(const QString &error)
{
    finishedFetching();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::critical(this, i18n("Critical script error"),
                                  error,
                                  QMessageBox::Abort | QMessageBox::Retry);

    if (reply == QMessageBox::Retry) {
        startSearch();
    }

    addToProgressLog(error);
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::addToProgressLog(const QString &status)
{
    QTextCursor qtc(m_progressLog);
    qtc.movePosition( QTextCursor::End );
    qtc.insertText(status + QLatin1String("\n"));
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::showProgressLog()
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

void NepomukMetaDataExtractor::Dialog::FetcherDialog::resourceFetchingDone()
{
    m_currentResource = -1;

    if( !m_re->resourcesList().isEmpty() ) {
        selectNextResourceToLookUp();
    }
    else {
        addToProgressLog(i18n("now files for the metadata fetching found"));
    }
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::selectNextResourceToLookUp()
{
    m_currentResource++;

    setupCurrentResourceToLookup();
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::selectPreviousResourceToLookUp()
{
    m_currentResource--;

    setupCurrentResourceToLookup();
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::setupCurrentResourceToLookup()
{
    int resourceCount = m_re->resourcesList().size();

    // get next resourceInformation
    MetaDataParameters *mdp = m_re->resourcesList().at( m_currentResource );

    fillEngineList( mdp->resourceType );

    labelResourceCount->setText(i18n("Resource %1 of %2", m_currentResource + 1, resourceCount));

    labelDescription->setText( i18n("Fetch metadata for the resource: <b>%1</b>", mdp->resourceUri.fileName()));
    lineEditTitle->setText( mdp->searchTitle );

    if(mdp->resourceType == QLatin1String("tvshow")) {
        labelSeason->setVisible(true);
        lineEditSeason->setVisible(true);
        labelEpisode->setVisible(true);
        lineEditEpisode->setVisible(true);
        labelShow->setVisible(true);
        lineEditShow->setVisible(true);
    }
    else {
        labelSeason->setVisible(false);
        lineEditSeason->setVisible(false);
        labelEpisode->setVisible(false);
        lineEditEpisode->setVisible(false);
        labelShow->setVisible(false);
        lineEditShow->setVisible(false);
    }

    lineEditSeason->setText(  mdp->searchSeason );
    lineEditEpisode->setText(  mdp->searchEpisode );
    lineEditShow->setText(  mdp->searchShowTitle );

    if( mdp->resourceType == QLatin1String("tvshow")) {
        cbSelectType->setVisible(true);
        cbSelectType->setCurrentIndex(1);
    }
    else if( mdp->resourceType == QLatin1String("movie")) {
        cbSelectType->setVisible(true);
        cbSelectType->setCurrentIndex(0);
    }
    else {
        cbSelectType->setVisible(false);
    }

    buttonFetchMore->setEnabled(false);

    // don't show next button if there is no next item
    if(m_currentResource == resourceCount-1) {
        buttonNext->setEnabled(false);
    }
    else {
        buttonNext->setEnabled(true);
    }

    // don't enable previous button, if there is no previous item
    if(m_currentResource == 0) {
        buttonPrevious->setEnabled(false);
    }
    else {
        buttonPrevious->setEnabled(true);
    }

    m_resultModel->clear();

    showItemDetails();
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::resourceTypeSelectionChanged(int selection)
{
    MetaDataParameters *mdp = m_re->resourcesList().at( m_currentResource );

    if (selection == 0) { // movies
        labelSeason->setVisible(false);
        lineEditSeason->setVisible(false);
        labelEpisode->setVisible(false);
        lineEditEpisode->setVisible(false);
        labelShow->setVisible(false);
        lineEditShow->setVisible(false);
        mdp->resourceType = QLatin1String("movie");
    }
    else { // tvshows
        labelSeason->setVisible(true);
        lineEditSeason->setVisible(true);
        labelEpisode->setVisible(true);
        lineEditEpisode->setVisible(true);
        labelShow->setVisible(true);
        lineEditShow->setVisible(true);
        mdp->resourceType = QLatin1String("tvshow");
    }

    fillEngineList( mdp->resourceType );
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::startSearch()
{
    busyFetching();

    MetaDataParameters *mdp = m_re->resourcesList().at( m_currentResource );
    mdp->searchTitle = lineEditTitle->text();
    mdp->searchSeason = lineEditSeason->text();
    mdp->searchEpisode = lineEditEpisode->text();
    mdp->searchShowTitle = lineEditShow->text();

    int currentEngine = comboBoxSearchEngine->currentIndex();
    QString engineId = comboBoxSearchEngine->itemData( currentEngine ).toString();

    QWidget::setCursor( Qt::BusyCursor );

    buttonSearch->setEnabled(false);
    buttonNext->setEnabled(false);
    buttonPrevious->setEnabled(false);

    if(!m_webextractor || m_webextractor->info().identifier != engineId) {
        delete m_webextractor;
        m_webextractor = m_ef->createExtractor( engineId );

        if(!m_webextractor) {
            kDebug() << "search engine with identifier" << engineId << "does not exist";
            return;
        }
        connect(m_webextractor, SIGNAL(searchResults(QVariantList)), this, SLOT(selectSearchEntry(QVariantList)));
        connect(m_webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
        connect(m_webextractor, SIGNAL(log(QString)), this, SLOT(addToProgressLog(QString)));
        connect(m_webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));
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
    searchParameters.insert("showtitle", mdp->searchShowTitle);

    m_webextractor->search( mdp->resourceType, searchParameters );
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::selectSearchEntry( QVariantList searchResults)
{
    finishedFetching();

    m_resultModel->setSearchResults( searchResults );

    QString searchEngineName = comboBoxSearchEngine->currentText();
    labelSearchResults->setText( i18n("Found <b>%1</b> results via <b>%2</b>", searchResults.size(), searchEngineName));
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::searchEntrySelected(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    QVariantMap entry = m_resultModel->searchResultEntry(current);

    detailsTitle->setText( entry.value(QLatin1String("title")).toString() );
    detailsText->setText( entry.value(QLatin1String("details")).toString() );

    if( entry.contains(QLatin1String("url"))) {
        detailsUrl->setText( i18n("Show online"));
        detailsUrl->setUrl( entry.value(QLatin1String("url")).toString() );

        detailsUrl->setVisible(true);
        buttonFetchMore->setEnabled(true);
    }
    else {
        detailsUrl->setVisible(false);
        buttonFetchMore->setEnabled(false);
    }
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::showSearchParameters()
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

    MetaDataParameters *mdp = m_re->resourcesList().at( m_currentResource );
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

    QLabel *labelShow = 0;
    QLineEdit *editShow = 0;

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

        labelShow = new QLabel(i18n("Show:"),w);
        editShow = new QLineEdit(mdp->searchShowTitle,w);
        gl->addWidget(labelShow, 6,0);
        gl->addWidget(editShow, 6,1);
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
            lineEditSeason->setText( mdp->searchSeason );

            mdp->searchEpisode = editEpisode->text();
            lineEditEpisode->setText( mdp->searchEpisode );

            mdp->searchShowTitle = editShow->text();
            lineEditShow->setText( mdp->searchShowTitle );
        }

        lineEditTitle->setText( mdp->searchTitle );
    }

    delete spd;
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::openDetailsLink(const QString &url)
{
    QString mimeTypeName = QLatin1String("text/html");
    /// Ask KDE subsystem to open url in viewer matching mime type
    KRun::runUrl(url, mimeTypeName, this, false, false);
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::fetchMoreDetails()
{
    busyFetching();

    // get the current item to fetch
    QModelIndex currentEntry = searchResults->currentIndex();
    QVariantMap entry = m_resultModel->searchResultEntry(currentEntry);

    m_currentItemToupdate = m_re->resourcesList().at( m_currentResource );
    KUrl fetchUrl( entry.value(QLatin1String("url")).toString() );

    //QVariantMap options;
    m_webextractor->extractItem( fetchUrl, entry );
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::fetchedItemDetails(const QString &resourceType, QVariantMap itemDetails)
{
    m_currentItemToupdate->metaData.insert(QLatin1String("resourceuri"), m_currentItemToupdate->resourceUri.url());

    QMapIterator<QString, QVariant> i(itemDetails);
    while (i.hasNext()) {
        i.next();
        m_currentItemToupdate->metaData.insert(i.key(), i.value());
    }

    // TODO: support batch download of many episodes at once
    if( resourceType == QLatin1String("tvshow")) {
        QVariantList seasons = m_currentItemToupdate->metaData.value(QLatin1String("seasons")).toList();
        if(!seasons.isEmpty()) {
            QVariantMap season = seasons.takeFirst().toMap();
            QVariantList episodes = season.value(QLatin1String("episodes")).toList();

            if(!episodes.isEmpty()) {
                QVariantMap episodesMap = episodes.takeFirst().toMap();
                kDebug() << "add to episode" << episodesMap.value(QLatin1String("title")) << "url" << m_currentItemToupdate->resourceUri.url();
                episodesMap.insert(QLatin1String("resourceuri"), m_currentItemToupdate->resourceUri.url());

                episodes << episodesMap;
                season.insert( QLatin1String("episodes"), episodes);
                seasons << season;
                m_currentItemToupdate->metaData.insert( QLatin1String("seasons"), seasons);
            }
        }
    }
    m_currentItemToupdate = 0;

    showItemDetails();

    finishedFetching();
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::saveMetaData()
{
    MetaDataParameters *mdp = m_re->resourcesList().at( m_currentResource );

    busyFetching();

    QString type = mdp->resourceType;

    NepomukPipe *nepomukPipe = 0;
    if(type == QLatin1String("publication")) {
        nepomukPipe = new PublicationPipe;
    }
    else if(type == QLatin1String("tvshow")) {
        nepomukPipe = new TvShowPipe;
    }
    else if(type == QLatin1String("movie")) {
        nepomukPipe = new MoviePipe;
    }

    if(nepomukPipe) {
        nepomukPipe->pipeImport( mdp->metaData );
        mdp->metaDataSaved = true;
    }
    else {
        addToProgressLog(i18n("No nepomuk pipe available for the resoure type %1", type));
    }

    finishedFetching();
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::cancelClose()
{
    close();
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::fillEngineList(const QString &category)
{
    // don't fetch information we already have
    if( comboBoxSearchEngine->property("currentListCategory").toString() == category)
        return;

    comboBoxSearchEngine->setProperty("currentListCategory", category);
    comboBoxSearchEngine->clear();

    QList<WebExtractor::Info> engines = m_ef->listAvailablePlugins( category );

    foreach(const WebExtractor::Info &engine, engines) {

        QFileInfo fileInfo(engine.file);

        QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;

        comboBoxSearchEngine->addItem(QIcon( iconPath ),
                                          engine.name,
                                          engine.identifier);
    }

    //TODO: select the default engine from some config file
    comboBoxSearchEngine->setCurrentIndex( 0 );
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::showItemDetails()
{
    QLayout *mdlayout = metaDataList->layout();

    QLayoutItem *child;
    while ((child = mdlayout->takeAt(0)) != 0) {
        QWidget *w = child->widget();
        delete w;
        delete child;
    }

    QGridLayout * gridLayout = qobject_cast<QGridLayout *>(mdlayout);

    // current Item metadata
    MetaDataParameters *mdp = m_re->resourcesList().at( m_currentResource );

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
        buttonSave->setEnabled(false);
    }
    else {
        buttonSave->setEnabled(true);
    }
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::busyFetching()
{
    QWidget::setCursor( Qt::BusyCursor );

    buttonSearch->setEnabled(false);
    buttonFetchMore->setEnabled(false);
    buttonNext->setEnabled(false);
    buttonPrevious->setEnabled(false);
    buttonSave->setEnabled( false );
}

void NepomukMetaDataExtractor::Dialog::FetcherDialog::finishedFetching()
{
    QWidget::setCursor( Qt::ArrowCursor );

    MetaDataParameters *mdp = m_re->resourcesList().at( m_currentResource );
    buttonSave->setEnabled( !mdp->metaDataSaved );

    buttonSearch->setEnabled( true );

    QModelIndex currentEntry = searchResults->currentIndex();
    QVariantMap entry = m_resultModel->searchResultEntry(currentEntry);

    if( entry.contains(QLatin1String("url")) ) {
        buttonFetchMore->setEnabled(true);
    }

    int resourceCount = m_re->resourcesList().size();
    if(m_currentResource == resourceCount-1) {
        buttonNext->setEnabled(false);
    }
    else {
        buttonNext->setEnabled(true);
    }

    // don't enable previous button, if there is no previous item
    if(m_currentResource == 0) {
        buttonPrevious->setEnabled(false);
    }
    else {
        buttonPrevious->setEnabled(true);
    }
}
