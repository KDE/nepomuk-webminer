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
#include "metadatawidget.h"

#include <mdesettings.h>
#include "configfetcher.h"
#include "configresourceextractor.h"
#include "confignepomukpipe.h"

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

#include <KDE/KRun>
#include <KDE/KDebug>
#include <KDE/KConfigDialog>

#include <QtCore/QFileInfo>
#include <QtCore/QPointer>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextDocument>
#include <QtGui/QTextEdit>
#include <QtGui/QMessageBox>

namespace NepomukMetaDataExtractor {
namespace UI {
    class FetcherDialogPrivate {
    public:
        NepomukMetaDataExtractor::Extractor::WebExtractor *webextractor;
        NepomukMetaDataExtractor::Extractor::MetaDataParameters *currentItemToupdate;
        int currentItemNumber;
        SearchResultsModel *resultModel;
        QTextDocument *progressLog;
    };
}
}

using namespace NepomukMetaDataExtractor::Pipe;
using namespace NepomukMetaDataExtractor::Extractor;
using namespace NepomukMetaDataExtractor::UI;

NepomukMetaDataExtractor::UI::FetcherDialog::FetcherDialog(QWidget *parent)
    : QDialog(parent)
    , d_ptr( new NepomukMetaDataExtractor::UI::FetcherDialogPrivate )
{
    setupUi(this);

    Q_D( FetcherDialog );
    d->webextractor = 0;
    d->currentItemToupdate = 0;
    d->currentItemNumber = 0;
    d->resultModel = new SearchResultsModel(this);
    searchResults->setModel( d->resultModel );
    d->progressLog = new QTextDocument(this);

    searchResults->setSelectionMode(QAbstractItemView::SingleSelection);
    searchResults->setItemDelegate(new SearchResultDelegate);

    connect(resourceExtractor(), SIGNAL(resourceExtarctionDone()), this, SLOT(resourceFetchingDone()));

    connect(buttonNext, SIGNAL(clicked()), this, SLOT(selectNextResourceToLookUp()));
    connect(buttonPrevious, SIGNAL(clicked()), this, SLOT(selectPreviousResourceToLookUp()));

    connect(buttonSearch, SIGNAL(clicked()), this, SLOT(startSearch()));
    connect(searchResults->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(searchEntrySelected(QModelIndex,QModelIndex)));
    connect(buttonSearchDetails, SIGNAL(clicked()), this, SLOT(showSearchParameters()));

    connect(buttonFetchMore, SIGNAL(clicked()), this, SLOT(fetchMoreDetails()));
    connect(buttonSave, SIGNAL(clicked()), this, SLOT(saveMetaDataSlot()));

    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancelClose()));

    connect(resourceExtractor(), SIGNAL(progressStatus(QString)), this, SLOT(addToProgressLog(QString)));
    connect(buttonLog, SIGNAL(clicked()), this, SLOT(showProgressLog()));
    connect(detailsUrl, SIGNAL(leftClickedUrl(QString)), this, SLOT(openDetailsLink(QString)));
    connect(cbSelectType, SIGNAL(currentIndexChanged(int)), this, SLOT(resourceTypeSelectionChanged(int)));

    buttonEngineSettings->setIcon(KIcon("configure"));
    buttonSearchDetails->setIcon(KIcon("system-search"));
    buttonSearch->setIcon(KIcon("edit-find"));
    buttonSave->setIcon(KIcon("document-save"));
    buttonCancel->setIcon(KIcon("dialog-cancel"));
    buttonNext->setIcon(KIcon("go-next"));
    buttonPrevious->setIcon(KIcon("go-previous"));
    buttonLog->setIcon(KIcon("tools-report-bug"));
    buttonFetchMore->setIcon(KIcon("download"));
}

NepomukMetaDataExtractor::UI::FetcherDialog::~FetcherDialog()
{
    Q_D( FetcherDialog );
    delete d->webextractor;
    delete d->currentItemToupdate;
    delete d->resultModel;
    delete d->progressLog;
}

void NepomukMetaDataExtractor::UI::FetcherDialog::errorInScriptExecution(const QString &error)
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

void NepomukMetaDataExtractor::UI::FetcherDialog::addToProgressLog(const QString &status)
{
    Q_D( FetcherDialog );

    QTextCursor qtc(d->progressLog);
    qtc.movePosition( QTextCursor::End );
    qtc.insertText(status + QLatin1String("\n"));
}

void NepomukMetaDataExtractor::UI::FetcherDialog::showProgressLog()
{
    Q_D( FetcherDialog );

    QPointer<KDialog> log = new KDialog;
    log->setInitialSize(QSize(600,300));
    log->setButtons( KDialog::Ok );

    QTextEdit *logView = new QTextEdit(log);
    logView->setDocument(d->progressLog);
    logView->setReadOnly(true);
    log->setMainWidget(logView);

    log->exec();

    delete log;
}

void NepomukMetaDataExtractor::UI::FetcherDialog::openSettings()
{
    KConfigDialog* dialog = new KConfigDialog( this, "settings", MDESettings::self() );

    ConfigFetcher *cfd = new ConfigFetcher();
    cfd->setExtractorFactory(extractorFactory());
    dialog->addPage( cfd, i18n("Fetcher"));

    ConfigResourceExtractor *cre = new ConfigResourceExtractor();
    dialog->addPage( cre, i18n("Resource Extractor"));

    ConfigNepomukPipe *cnp = new ConfigNepomukPipe();
    dialog->addPage( cnp, i18n("Nepomuk Pipe"));

    dialog->exec();
}

void NepomukMetaDataExtractor::UI::FetcherDialog::resourceFetchingDone()
{
    Q_D( FetcherDialog );

    d->currentItemNumber = -1;

    if( !resourceExtractor()->resourcesList().isEmpty() ) {
        selectNextResourceToLookUp();
    }
    else {
        addToProgressLog(i18n("all files for the metadata fetching found"));
    }
}

void NepomukMetaDataExtractor::UI::FetcherDialog::selectNextResourceToLookUp()
{
    Q_D( FetcherDialog );

    d->currentItemNumber++;

    setupCurrentResourceToLookup();
}

void NepomukMetaDataExtractor::UI::FetcherDialog::selectPreviousResourceToLookUp()
{
    Q_D( FetcherDialog );

    d->currentItemNumber--;

    setupCurrentResourceToLookup();
}

void NepomukMetaDataExtractor::UI::FetcherDialog::setupCurrentResourceToLookup()
{
    Q_D( FetcherDialog );

    int resourceCount = resourceExtractor()->resourcesList().size();

    // get next resourceInformation
    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at( d->currentItemNumber );

    fillEngineList( mdp->resourceType );

    labelResourceCount->setText(i18n("Resource %1 of %2", d->currentItemNumber + 1, resourceCount));

    labelDescription->setText( i18n("Fetch metadata for the resource: <b>%1</b>", mdp->resourceUri.fileName()));
    lineEditTitle->setText( mdp->searchTitle );

    if(mdp->resourceType == QLatin1String("tvshow")) {
        labelSeason->setVisible(true);
        lineEditSeason->setVisible(true);
        labelEpisode->setVisible(true);
        lineEditEpisode->setVisible(true);
        labelShow->setVisible(true);
        lineEditShow->setVisible(true);

        labelArtist->setVisible(false);
        lineEditArtist->setVisible(false);
        labelAlbum->setVisible(false);
        lineEditAlbum->setVisible(false);

        lineEditSeason->setText( mdp->searchSeason );
        lineEditEpisode->setText( mdp->searchEpisode );
        lineEditShow->setText(  mdp->searchShowTitle );
    }
    else if(mdp->resourceType == QLatin1String("publication")) {
        labelSeason->setVisible(false);
        lineEditSeason->setVisible(false);
        labelEpisode->setVisible(false);
        lineEditEpisode->setVisible(false);
        labelShow->setVisible(false);
        lineEditShow->setVisible(false);

        labelArtist->setVisible(false);
        lineEditArtist->setVisible(false);
        labelAlbum->setVisible(false);
        lineEditAlbum->setVisible(false);
    }
    else if(mdp->resourceType == QLatin1String("music")) {
        labelSeason->setVisible(false);
        lineEditSeason->setVisible(false);
        labelEpisode->setVisible(false);
        lineEditEpisode->setVisible(false);
        labelShow->setVisible(false);
        lineEditShow->setVisible(false);

        labelArtist->setVisible(true);
        lineEditArtist->setVisible(true);
        labelAlbum->setVisible(true);
        lineEditAlbum->setVisible(true);

        lineEditArtist->setText( mdp->searchPerson );
        lineEditAlbum->setText( mdp->searchAlbum );
    }

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
    if(d->currentItemNumber == resourceCount-1) {
        buttonNext->setEnabled(false);
    }
    else {
        buttonNext->setEnabled(true);
    }

    // don't enable previous button, if there is no previous item
    if(d->currentItemNumber == 0) {
        buttonPrevious->setEnabled(false);
    }
    else {
        buttonPrevious->setEnabled(true);
    }

    d->resultModel->clear();

    showItemDetails();
}

void NepomukMetaDataExtractor::UI::FetcherDialog::resourceTypeSelectionChanged(int selection)
{
    Q_D( FetcherDialog );
    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at( d->currentItemNumber );

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

void NepomukMetaDataExtractor::UI::FetcherDialog::startSearch()
{
    Q_D( FetcherDialog );
    busyFetching();

    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at( d->currentItemNumber );
    mdp->searchTitle = lineEditTitle->text();
    mdp->searchSeason = lineEditSeason->text();
    mdp->searchEpisode = lineEditEpisode->text();
    mdp->searchShowTitle = lineEditShow->text();
    mdp->searchAlbum = lineEditAlbum->text();
    mdp->searchPerson = lineEditArtist->text();

    int currentEngine = comboBoxSearchEngine->currentIndex();
    QString engineId = comboBoxSearchEngine->itemData( currentEngine ).toString();

    QWidget::setCursor( Qt::BusyCursor );

    buttonSearch->setEnabled(false);
    buttonNext->setEnabled(false);
    buttonPrevious->setEnabled(false);

    if(!d->webextractor || d->webextractor->info().identifier != engineId) {
        delete d->webextractor;
        d->webextractor = extractorFactory()->createExtractor( engineId );

        if(!d->webextractor) {
            kDebug() << "search engine with identifier" << engineId << "does not exist";
            return;
        }
        connect(d->webextractor, SIGNAL(searchResults(QVariantList)), this, SLOT(selectSearchEntry(QVariantList)));
        connect(d->webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
        connect(d->webextractor, SIGNAL(log(QString)), this, SLOT(addToProgressLog(QString)));
        connect(d->webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));
        connect(d->webextractor, SIGNAL(error(QString)), this, SLOT(addToProgressLog(QString)));
    }

    QVariantMap searchParameters;
    searchParameters.insert("title", mdp->searchTitle);
    searchParameters.insert("alttitle", mdp->searchAltTitle);
    searchParameters.insert("author", mdp->searchPerson);
    searchParameters.insert("season", mdp->searchSeason);
    searchParameters.insert("episode", mdp->searchEpisode);
    searchParameters.insert("yearMin", mdp->searchYearMin);
    searchParameters.insert("yearMax", mdp->searchYearMax);
    searchParameters.insert("journal", mdp->searchJournal);
    searchParameters.insert("showtitle", mdp->searchShowTitle);
    searchParameters.insert("album", mdp->searchAlbum);
    searchParameters.insert("track", mdp->searchTrack);

    //kDebug() << "webextractor->search :: " << searchParameters;

    d->webextractor->search( mdp->resourceType, searchParameters );
}

void NepomukMetaDataExtractor::UI::FetcherDialog::selectSearchEntry( QVariantList searchResults)
{
    Q_D( FetcherDialog );
    finishedFetching();

    d->resultModel->setSearchResults( searchResults );

    QString searchEngineName = comboBoxSearchEngine->currentText();
    labelSearchResults->setText( i18n("Found <b>%1</b> results via <b>%2</b>", searchResults.size(), searchEngineName));
}

void NepomukMetaDataExtractor::UI::FetcherDialog::searchEntrySelected(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    Q_D( FetcherDialog );

    QVariantMap entry = d->resultModel->searchResultEntry(current);

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

void NepomukMetaDataExtractor::UI::FetcherDialog::showSearchParameters()
{
    Q_D( FetcherDialog );
    QPointer<KDialog> spd = new KDialog;
    spd->setInitialSize(QSize(600,300));

    QWidget *w = new QWidget(spd);
    QGridLayout *gl = new QGridLayout;
    gl->setColumnStretch(1,10);

    QVBoxLayout *vbl = new QVBoxLayout();
    vbl->addLayout(gl);
    vbl->addStretch(0);
    w->setLayout(vbl);

    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at( d->currentItemNumber );
    QLabel *labelTitle = new QLabel(i18nc("@item:the title of a publication/movie/show", "Title:"),w);
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

    QLabel *labelTrack = 0;
    QLineEdit *editTrack = 0;

    QLabel *labelAlbum = 0;
    QLineEdit *editAlbum = 0;

    if(mdp->resourceType == QLatin1String("publication")) {
        labelAuthor = new QLabel(i18n("Author:"),w);
        editAuthor = new QLineEdit(mdp->searchPerson,w);
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

    if(mdp->resourceType == QLatin1String("music")) {
        labelAuthor = new QLabel(i18n("Artist:"),w);
        editAuthor = new QLineEdit(mdp->searchPerson,w);
        gl->addWidget(labelAuthor, 4,0);
        gl->addWidget(editAuthor, 4,1);

        labelTrack = new QLabel(i18n("Track:"),w);
        editTrack = new QLineEdit(mdp->searchTrack,w);
        gl->addWidget(labelTrack, 5,0);
        gl->addWidget(editTrack, 5,1);

        labelAlbum = new QLabel(i18n("Album:"),w);
        editAlbum = new QLineEdit(mdp->searchAlbum,w);
        gl->addWidget(labelAlbum, 6,0);
        gl->addWidget(editAlbum, 6,1);
    }

    spd->setMainWidget(w);

    int ret = spd->exec();

    if(ret == KDialog::Accepted) {
        mdp->searchTitle = editTitle->text();
        mdp->searchAltTitle = editAltTitle->text();
        mdp->searchYearMin = editYearMin->text();
        mdp->searchYearMax = editYearMax->text();

        if(mdp->resourceType == QLatin1String("publication")) {
            mdp->searchPerson = editAuthor->text();
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
        else if(mdp->resourceType == QLatin1String("music")) {
            mdp->searchPerson = editAuthor->text();
            mdp->searchAlbum = editAlbum->text();
            mdp->searchTrack = editTrack->text();
        }

        lineEditTitle->setText( mdp->searchTitle );
        lineEditArtist->setText( mdp->searchPerson );
        lineEditAlbum->setText( mdp->searchAlbum );
        lineEditSeason->setText( mdp->searchSeason );
        lineEditShow->setText( mdp->searchShowTitle );
        lineEditEpisode->setText( mdp->searchEpisode );
    }

    delete spd;
}

void NepomukMetaDataExtractor::UI::FetcherDialog::openDetailsLink(const QString &url)
{
    QString mimeTypeName = QLatin1String("text/html");
    /// Ask KDE subsystem to open url in viewer matching mime type
    KRun::runUrl(url, mimeTypeName, this, false, false);
}

void NepomukMetaDataExtractor::UI::FetcherDialog::fetchMoreDetails()
{
    Q_D( FetcherDialog );
    busyFetching();

    // get the current item to fetch
    QModelIndex currentEntry = searchResults->currentIndex();
    QVariantMap entry = d->resultModel->searchResultEntry(currentEntry);
    kDebug() << "fetch details" << entry;

    d->currentItemToupdate = resourceExtractor()->resourcesList().at( d->currentItemNumber );
    KUrl fetchUrl( entry.value(QLatin1String("url")).toString() );


    QVariantMap options;
    options.insert(QString("references"), MDESettings::downloadReferences());
    options.insert(QString("banner"), MDESettings::downloadBanner());

    d->webextractor->extractItem( fetchUrl, options );
}

void NepomukMetaDataExtractor::UI::FetcherDialog::fetchedItemDetails(const QString &resourceType, QVariantMap itemDetails)
{
    Q_D( FetcherDialog );
    d->currentItemToupdate->metaData.insert(QLatin1String("resourceuri"), d->currentItemToupdate->resourceUri.url());
    d->currentItemToupdate->metaData = itemDetails;
    d->currentItemToupdate->resourceType = resourceType;

    addResourceUriToMetaData( d->currentItemToupdate );
    d->currentItemToupdate->metaDataSaved = false;

    showItemDetails();
    finishedFetching();

    d->currentItemToupdate = 0;

    //kDebug() << itemDetails;
}

void NepomukMetaDataExtractor::UI::FetcherDialog::saveMetaDataSlot()
{
    Q_D( FetcherDialog );
    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at( d->currentItemNumber );

    busyFetching();

    saveMetaData( mdp );
    buttonSave->setEnabled(false);
    mdp->metaDataSaved = true;

    finishedFetching();
}

void NepomukMetaDataExtractor::UI::FetcherDialog::cancelClose()
{
    close();
}

void NepomukMetaDataExtractor::UI::FetcherDialog::fillEngineList(const QString &category)
{
    // don't fetch information we already have
    if( comboBoxSearchEngine->property("currentListCategory").toString() == category)
        return;

    comboBoxSearchEngine->setProperty("currentListCategory", category);
    comboBoxSearchEngine->clear();

    QList<WebExtractor::Info> engines = extractorFactory()->listAvailablePlugins( category );

    foreach(const WebExtractor::Info &engine, engines) {

        QFileInfo fileInfo(engine.file);

        QString iconPath = fileInfo.absolutePath() + QLatin1String("/") + engine.icon;

        comboBoxSearchEngine->addItem(QIcon( iconPath ),
                                          engine.name,
                                          engine.identifier);
    }

    if( category == QString("movie")) {
        comboBoxSearchEngine->setCurrentIndex(comboBoxSearchEngine->findData(MDESettings::favoriteMoviePlugin()));
    }
    else if( category == QString("tvshow")) {
        comboBoxSearchEngine->setCurrentIndex(comboBoxSearchEngine->findData(MDESettings::favoriteTvShowPlugin()));
    }
    else if( category == QString("music")) {
        comboBoxSearchEngine->setCurrentIndex(comboBoxSearchEngine->findData(MDESettings::favoriteMusicPlugin()));
    }
    else if( category == QString("publication")) {
        comboBoxSearchEngine->setCurrentIndex(comboBoxSearchEngine->findData(MDESettings::favoritePublicationPlugin()));
    }

}

void NepomukMetaDataExtractor::UI::FetcherDialog::showItemDetails()
{
    Q_D( FetcherDialog );
    // current Item metadata
    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at( d->currentItemNumber );

    metaDataList->setMetaDataParameter(mdp);

    kDebug() << mdp->metaDataSaved;
    if( mdp->metaDataSaved == true) {
        buttonSave->setEnabled(false);
    }
    else {
        buttonSave->setEnabled(true);
    }
}

void NepomukMetaDataExtractor::UI::FetcherDialog::busyFetching()
{
    QWidget::setCursor( Qt::BusyCursor );

    buttonSearch->setEnabled(false);
    buttonFetchMore->setEnabled(false);
    buttonNext->setEnabled(false);
    buttonPrevious->setEnabled(false);
    buttonSave->setEnabled( false );
}

void NepomukMetaDataExtractor::UI::FetcherDialog::finishedFetching()
{
    Q_D( FetcherDialog );
    QWidget::setCursor( Qt::ArrowCursor );

    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at( d->currentItemNumber );
    buttonSave->setEnabled( !mdp->metaDataSaved );

    buttonSearch->setEnabled( true );

    QModelIndex currentEntry = searchResults->currentIndex();
    QVariantMap entry = d->resultModel->searchResultEntry(currentEntry);

    if( entry.contains(QLatin1String("url")) ) {
        buttonFetchMore->setEnabled(true);
    }

    int resourceCount = resourceExtractor()->resourcesList().size();
    if(d->currentItemNumber == resourceCount-1) {
        buttonNext->setEnabled(false);
    }
    else {
        buttonNext->setEnabled(true);
    }

    // don't enable previous button, if there is no previous item
    if(d->currentItemNumber == 0) {
        buttonPrevious->setEnabled(false);
    }
    else {
        buttonPrevious->setEnabled(true);
    }
}
