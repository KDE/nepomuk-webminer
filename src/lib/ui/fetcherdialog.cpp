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

#include "fetcherdialog.h"
#include "ui_fetcherdialog.h"

#include "searchresultsmodel.h"
#include "searchresultdelegate.h"
#include "metadatawidget.h"

#include <mdesettings.h>

#include "metadataparameters.h"
#include "resourceextractor/resourceextractor.h"
#include "webextractor/extractorfactory.h"
#include "webextractor/webextractor.h"

#include "nepomukpipe/nepomukpipe.h"
#include "nepomukpipe/moviepipe.h"
#include "nepomukpipe/publicationpipe.h"
#include "nepomukpipe/tvshowpipe.h"

#include "startupdialog.h"

#include <KDE/KStandardDirs>
#include <KDE/KDialog>

#include <KDE/KPixmapSequence>
#include <KDE/KPixmapSequenceOverlayPainter>

#include <KDE/KRun>
#include <KDE/KDebug>
#include <KDE/KConfigDialog>
#include <KDE/KTextEdit>
#include <KDE/KToolInvocation>
#include <KDE/KMessageBox>

#include <QtCore/QFileInfo>
#include <QtCore/QPointer>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextDocument>
#include <QtGui/QMessageBox>
#include <QtCore/QProcess>

#include <QDebug>

namespace NepomukWebMiner
{
namespace UI
{
class FetcherDialogPrivate
{
public:
    NepomukWebMiner::Extractor::WebExtractor *webextractor;
    NepomukWebMiner::Extractor::MetaDataParameters *currentItemToupdate;
    int currentItemNumber;
    SearchResultsModel *resultModel;
    QTextDocument *progressLog;
    KPixmapSequenceOverlayPainter *busySearchWidget;
    bool saveAutomatically;
};
}
}

using namespace NepomukWebMiner::Pipe;
using namespace NepomukWebMiner::Extractor;
using namespace NepomukWebMiner::UI;

NepomukWebMiner::UI::FetcherDialog::FetcherDialog(QWidget *parent)
    : QDialog(parent)
    , d_ptr(new NepomukWebMiner::UI::FetcherDialogPrivate)
{
    setupUi(this);

    Q_D(FetcherDialog);
    d->webextractor = 0;
    d->currentItemToupdate = 0;
    d->currentItemNumber = 0;
    d->resultModel = new SearchResultsModel(this);
    searchResults->setModel(d->resultModel);
    d->progressLog = new QTextDocument(this);
    d->saveAutomatically=false;

    searchResults->setSelectionMode(QAbstractItemView::SingleSelection);
    searchResults->setItemDelegate(new SearchResultDelegate);

    connect(resourceExtractor(), SIGNAL(resourceExtarctionDone()), this, SLOT(resourceFetchingDone()));

    connect(buttonNext, SIGNAL(clicked()), this, SLOT(selectNextResourceToLookUp()));
    connect(buttonPrevious, SIGNAL(clicked()), this, SLOT(selectPreviousResourceToLookUp()));

    connect(buttonSearch, SIGNAL(clicked()), this, SLOT(startSearch()));
    connect(searchResults->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(searchEntrySelected(QModelIndex,QModelIndex)));
    connect(buttonSearchDetails, SIGNAL(clicked()), this, SLOT(showSearchParameters()));

    connect(buttonFetchMore, SIGNAL(clicked()), this, SLOT(fetchMoreDetails()));
    connect(buttonFetchAndSave, SIGNAL(clicked()), this, SLOT(fetchMoreAndSave()));
    connect(buttonSave, SIGNAL(clicked()), this, SLOT(saveMetaDataSlot()));

    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancelClose()));

    connect(resourceExtractor(), SIGNAL(progressStatus(QString)), this, SLOT(addToProgressLog(QString)));
    connect(buttonLog, SIGNAL(clicked()), this, SLOT(showProgressLog()));
    connect(detailsUrl, SIGNAL(leftClickedUrl(QString)), this, SLOT(openDetailsLink(QString)));
    connect(cbSelectType, SIGNAL(currentIndexChanged(int)), this, SLOT(resourceTypeSelectionChanged(int)));

    connect(buttonHelp, SIGNAL(clicked()), this, SLOT(openHelp()));

    buttonEngineSettings->setIcon(KIcon("configure"));
    buttonSearchDetails->setIcon(KIcon("system-search"));
    buttonSearch->setIcon(KIcon("edit-find"));
    buttonSave->setIcon(KIcon("document-save"));
    buttonCancel->setIcon(KIcon("dialog-cancel"));
    buttonNext->setIcon(KIcon("go-next"));
    buttonPrevious->setIcon(KIcon("go-previous"));
    buttonLog->setIcon(KIcon("tools-report-bug"));
    buttonFetchMore->setIcon(KIcon("download"));
    buttonFetchAndSave->setIcon(KIcon("download"));
    buttonHelp->setIcon(KIcon("help-contents"));

    d->busySearchWidget = new KPixmapSequenceOverlayPainter(this);
    d->busySearchWidget->setSequence(KPixmapSequence("process-working", KIconLoader::SizeSmallMedium));
    d->busySearchWidget->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    d->busySearchWidget->setWidget(searchResults->viewport());
}

NepomukWebMiner::UI::FetcherDialog::~FetcherDialog()
{
    Q_D(FetcherDialog);
    delete d->currentItemToupdate;
    delete d->resultModel;
    delete d->progressLog;
}

bool NepomukWebMiner::UI::FetcherDialog::startFetching(const KUrl url)
{
    //FIXME: tell user that no plugins/the krosspython plugin is missing
    QPointer<StartupDialog> sud = new StartupDialog(this);

    sud->setFetchingUrl(url);

    QTimer::singleShot(0, sud, SLOT(startFetching()));
    int ret = sud->exec();


    if(ret == QDialog::Rejected) {
        sud->cancel();
        delete sud;
        return false;
    }

    delete sud;

    // check if we have files for the webextraction
    if( resourceExtractor()->resourcesList().isEmpty() ) {

        KMessageBox::error(this, i18n("No files without metadata could be found.\n\n"
                                      "Try starting the WebMiner with forced updates to use all available files"),
                                       i18n("Nepomuk-WebMiner") );

        return false;

    }

    // check if plugins are available
    // so check for krosspython and if some plugins exist

    return true;

}

void NepomukWebMiner::UI::FetcherDialog::errorInScriptExecution(const QString &error)
{
    finishedFetching();

    int reply = KMessageBox::warningContinueCancel(this, error, i18n("Critical script error"));

    if (reply == KMessageBox::Continue) {
        startSearch();
    }

    addToProgressLog(error);
}

void NepomukWebMiner::UI::FetcherDialog::addToProgressLog(const QString &status)
{
    Q_D(FetcherDialog);

    QTextCursor qtc(d->progressLog);
    qtc.movePosition(QTextCursor::End);
    qtc.insertText(status + QLatin1String("\n"));
}

void NepomukWebMiner::UI::FetcherDialog::showProgressLog()
{
    Q_D(FetcherDialog);

    QPointer<KDialog> log = new KDialog;
    log->setInitialSize(QSize(600, 300));
    log->setButtons(KDialog::Ok);

    KTextEdit *logView = new KTextEdit(log);
    logView->setDocument(d->progressLog);
    logView->setReadOnly(true);
    log->setMainWidget(logView);

    log->exec();

    delete log;
}

void NepomukWebMiner::UI::FetcherDialog::openSettings()
{
    QProcess::startDetached(QLatin1String("kcmshell4 kcm_nepomuk-webminer"));
}

void NepomukWebMiner::UI::FetcherDialog::openHelp()
{
    KToolInvocation::invokeHelp(QString(), QLatin1String("kcontrol/nepomuk-webminer"));
}

void NepomukWebMiner::UI::FetcherDialog::resourceFetchingDone()
{
    Q_D(FetcherDialog);

    d->currentItemNumber = -1;

    if (!resourceExtractor()->resourcesList().isEmpty()) {
        selectNextResourceToLookUp();
    } else {
        addToProgressLog(i18n("all files for the metadata fetching found"));
    }
}

void NepomukWebMiner::UI::FetcherDialog::selectNextResourceToLookUp()
{
    Q_D(FetcherDialog);

    d->currentItemNumber++;

    setupCurrentResourceToLookup();
}

void NepomukWebMiner::UI::FetcherDialog::selectPreviousResourceToLookUp()
{
    Q_D(FetcherDialog);

    d->currentItemNumber--;

    setupCurrentResourceToLookup();
}

void NepomukWebMiner::UI::FetcherDialog::setupCurrentResourceToLookup()
{
    Q_D(FetcherDialog);

    int resourceCount = resourceExtractor()->resourcesList().size();

    // get next resourceInformation
    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at(d->currentItemNumber);

    fillEngineList(mdp->resourceType());

    labelResourceCount->setText(i18n("Resource %1 of %2", d->currentItemNumber + 1, resourceCount));

    labelDescription->setText(i18n("Fetch metadata for the resource: <b>%1</b>", mdp->resourceUri().fileName()));
    lineEditTitle->setText(mdp->searchTitle());

    if (mdp->resourceType() == QLatin1String("tvshow")) {
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

        cbSelectType->setVisible(true);
        cbSelectType->setCurrentIndex(1);

        lineEditSeason->setText(mdp->searchSeason());
        lineEditEpisode->setText(mdp->searchEpisode());
        lineEditShow->setText(mdp->searchShowTitle());
    } else if (mdp->resourceType() == QLatin1String("movie")) {
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

        cbSelectType->setVisible(true);
        cbSelectType->setCurrentIndex(0);

        lineEditSeason->setText(mdp->searchSeason());
        lineEditEpisode->setText(mdp->searchEpisode());
        lineEditShow->setText(mdp->searchShowTitle());
    } else if (mdp->resourceType() == QLatin1String("publication")) {
        labelSeason->setVisible(false);
        lineEditSeason->setVisible(false);
        labelEpisode->setVisible(false);
        lineEditEpisode->setVisible(false);
        labelShow->setVisible(false);
        lineEditShow->setVisible(false);

        cbSelectType->setVisible(false);

        labelArtist->setVisible(false);
        lineEditArtist->setVisible(false);
        labelAlbum->setVisible(false);
        lineEditAlbum->setVisible(false);
    } else if (mdp->resourceType() == QLatin1String("music")) {
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

        cbSelectType->setVisible(false);

        lineEditArtist->setText(mdp->searchPerson());
        lineEditAlbum->setText(mdp->searchAlbum());
    }

    buttonFetchMore->setEnabled(false);
    buttonFetchAndSave->setEnabled(false);

    // don't show next button if there is no next item
    if (d->currentItemNumber == resourceCount - 1) {
        buttonNext->setEnabled(false);
    } else {
        buttonNext->setEnabled(true);
    }

    // don't enable previous button, if there is no previous item
    if (d->currentItemNumber == 0) {
        buttonPrevious->setEnabled(false);
    } else {
        buttonPrevious->setEnabled(true);
    }

    d->resultModel->clear();

    showItemDetails();
}

void NepomukWebMiner::UI::FetcherDialog::resourceTypeSelectionChanged(int selection)
{
    Q_D(FetcherDialog);
    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at(d->currentItemNumber);

    if (selection == 0) { // movies
        labelSeason->setVisible(false);
        lineEditSeason->setVisible(false);
        labelEpisode->setVisible(false);
        lineEditEpisode->setVisible(false);
        labelShow->setVisible(false);
        lineEditShow->setVisible(false);
        mdp->setResourceType(QLatin1String("movie"));
    } else { // tvshows
        labelSeason->setVisible(true);
        lineEditSeason->setVisible(true);
        labelEpisode->setVisible(true);
        lineEditEpisode->setVisible(true);
        labelShow->setVisible(true);
        lineEditShow->setVisible(true);
        mdp->setResourceType(QLatin1String("tvshow"));
    }

    fillEngineList(mdp->resourceType());
}

void NepomukWebMiner::UI::FetcherDialog::startSearch()
{
    Q_D(FetcherDialog);
    d->resultModel->clear();
    d->busySearchWidget->start();
    busyFetching();

    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at(d->currentItemNumber);
    mdp->setSearchTitle(lineEditTitle->text());
    mdp->setSearchSeason(lineEditSeason->text());
    mdp->setSearchEpisode(lineEditEpisode->text());
    mdp->setSearchShowTitle(lineEditShow->text());
    mdp->setSearchAlbum(lineEditAlbum->text());
    mdp->setSearchPerson(lineEditArtist->text());

    int currentEngine = comboBoxSearchEngine->currentIndex();
    QString engineId = comboBoxSearchEngine->itemData(currentEngine).toString();

    QWidget::setCursor(Qt::BusyCursor);

    buttonSearch->setEnabled(false);
    buttonNext->setEnabled(false);
    buttonPrevious->setEnabled(false);

    if (!d->webextractor || d->webextractor->info().identifier != engineId) {
        disconnect(d->webextractor, SIGNAL(searchResults(QVariantList)), this, SLOT(searchResultList(QVariantList)));
        disconnect(d->webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
        disconnect(d->webextractor, SIGNAL(log(QString)), this, SLOT(addToProgressLog(QString)));
        disconnect(d->webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));
        disconnect(d->webextractor, SIGNAL(error(QString)), this, SLOT(addToProgressLog(QString)));

        d->webextractor = extractorFactory()->getExtractor(engineId);

        if (!d->webextractor) {
            kDebug() << "search engine with identifier" << engineId << "does not exist";
            return;
        }
        connect(d->webextractor, SIGNAL(searchResults(QVariantList)), this, SLOT(searchResultList(QVariantList)));
        connect(d->webextractor, SIGNAL(itemResults(QString,QVariantMap)), this, SLOT(fetchedItemDetails(QString,QVariantMap)));
        connect(d->webextractor, SIGNAL(log(QString)), this, SLOT(addToProgressLog(QString)));
        connect(d->webextractor, SIGNAL(error(QString)), this, SLOT(errorInScriptExecution(QString)));
        connect(d->webextractor, SIGNAL(error(QString)), this, SLOT(addToProgressLog(QString)));
    }

    QVariantMap searchParameters;
    searchParameters.insert(QLatin1String("title"), mdp->searchTitle());
    searchParameters.insert(QLatin1String("alttitle"), mdp->searchAltTitle());
    searchParameters.insert(QLatin1String("author"), mdp->searchPerson());
    searchParameters.insert(QLatin1String("season"), mdp->searchSeason());
    searchParameters.insert(QLatin1String("episode"), mdp->searchEpisode());
    searchParameters.insert(QLatin1String("yearMin"), mdp->searchYearMin());
    searchParameters.insert(QLatin1String("yearMax"), mdp->searchYearMax());
    searchParameters.insert(QLatin1String("journal"), mdp->searchJournal());
    searchParameters.insert(QLatin1String("showtitle"), mdp->searchShowTitle());
    searchParameters.insert(QLatin1String("album"), mdp->searchAlbum());
    searchParameters.insert(QLatin1String("track"), mdp->searchTrack());

    //kDebug() << "webextractor->search :: " << searchParameters;

    // update kext:indexingLevel to 3
    // this ensures we are not going to check this file again in the
    // background servcie no matter if we found something or not
    updateIndexingLevel(mdp->resourceUri(), 3);


    d->webextractor->search(mdp->resourceType(), searchParameters);
}

void NepomukWebMiner::UI::FetcherDialog::searchResultList(QVariantList searchResultList)
{
    Q_D(FetcherDialog);
    finishedFetching();

    // every result with a distance bigger than 20 will be removed from the list
    // this ensures the returned item are at least similar to the search result.
    QVariantList sortedList = setLevenshteinDistance(searchResultList, resourceExtractor()->resourcesList().at(d->currentItemNumber), 20);
    d->resultModel->setSearchResults(sortedList);

    QString searchEngineName = comboBoxSearchEngine->currentText();
    labelSearchResults->setText(i18ncp("%2 is a search engine", "Found <b>1</b> result via <b>%2</b>", "Found <b>%1</b> results via <b>%2</b>", searchResultList.size(), searchEngineName));

    searchResults->setCurrentIndex(d->resultModel->index(0));
}

void NepomukWebMiner::UI::FetcherDialog::searchEntrySelected(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    Q_D(FetcherDialog);

    QVariantMap entry = d->resultModel->searchResultEntry(current);

    detailsTitle->setText(entry.value(QLatin1String("title")).toString());
    detailsText->setText(entry.value(QLatin1String("details")).toString());

    if (entry.contains(QLatin1String("url"))) {
        detailsUrl->setText(i18n("Show online"));
        detailsUrl->setUrl(entry.value(QLatin1String("url")).toString());

        detailsUrl->setVisible(true);
        buttonFetchMore->setEnabled(true);
        buttonFetchAndSave->setEnabled(true);
    } else {
        detailsUrl->setVisible(false);
        buttonFetchMore->setEnabled(false);
        buttonFetchAndSave->setEnabled(false);
    }
}

void NepomukWebMiner::UI::FetcherDialog::showSearchParameters()
{
    Q_D(FetcherDialog);
    QPointer<KDialog> spd = new KDialog;
    spd->setInitialSize(QSize(600, 300));

    QWidget *w = new QWidget(spd);
    QGridLayout *gl = new QGridLayout;
    gl->setColumnStretch(1, 10);

    QVBoxLayout *vbl = new QVBoxLayout();
    vbl->addLayout(gl);
    vbl->addStretch(0);
    w->setLayout(vbl);

    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at(d->currentItemNumber);
    QLabel *labelTitle = new QLabel(i18nc("The title of a publication/movie/show", "Title:"), w);
    KLineEdit *editTitle = new KLineEdit(mdp->searchTitle(), w);
    gl->addWidget(labelTitle, 0, 0);
    gl->addWidget(editTitle, 0, 1);
    QLabel *labelAltTitle = new QLabel(i18n("Alternative Title:"), w);
    KLineEdit *editAltTitle = new KLineEdit(mdp->searchAltTitle(), w);
    gl->addWidget(labelAltTitle, 1, 0);
    gl->addWidget(editAltTitle, 1, 1);

    QLabel *labelYearMin = new QLabel(i18n("Year Min:"), w);
    KLineEdit *editYearMin = new KLineEdit(mdp->searchYearMin(), w);
    gl->addWidget(labelYearMin, 2, 0);
    gl->addWidget(editYearMin, 2, 1);

    QLabel *labelYearMax = new QLabel(i18n("Year Max:"), w);
    KLineEdit *editYearMax = new KLineEdit(mdp->searchYearMax(), w);
    gl->addWidget(labelYearMax, 3, 0);
    gl->addWidget(editYearMax, 3, 1);

    QLabel *labelAuthor = 0;
    KLineEdit *editAuthor = 0;

    QLabel *labelJournal = 0;
    KLineEdit *editJournal = 0;

    QLabel *labelSeason = 0;
    KLineEdit *editSeason = 0;

    QLabel *labelEpisode = 0;
    KLineEdit *editEpisode = 0;

    QLabel *labelShow = 0;
    KLineEdit *editShow = 0;

    QLabel *labelTrack = 0;
    KLineEdit *editTrack = 0;

    QLabel *labelAlbum = 0;
    KLineEdit *editAlbum = 0;

    if (mdp->resourceType() == QLatin1String("publication")) {
        labelAuthor = new QLabel(i18nc("The author of the publication", "Author:"), w);
        editAuthor = new KLineEdit(mdp->searchPerson(), w);
        gl->addWidget(labelAuthor, 4, 0);
        gl->addWidget(editAuthor, 4, 1);

        labelJournal = new QLabel(i18nc("The name of the journal the article is in", "Journal:"), w);
        editJournal = new KLineEdit(mdp->searchJournal(), w);
        gl->addWidget(labelJournal, 5, 0);
        gl->addWidget(editJournal, 5, 1);
    } else if (mdp->resourceType() == QLatin1String("tvshow")) {
        labelSeason = new QLabel(i18nc("The number of the tv season", "Season:"), w);
        editSeason = new KLineEdit(mdp->searchSeason(), w);
        gl->addWidget(labelSeason, 4, 0);
        gl->addWidget(editSeason, 4, 1);

        labelEpisode = new QLabel(i18nc("The number of the tv show episode", "Episode:"), w);
        editEpisode = new KLineEdit(mdp->searchEpisode(), w);
        gl->addWidget(labelEpisode, 5, 0);
        gl->addWidget(editEpisode, 5, 1);

        labelShow = new QLabel(i18nc("The name of the tv Show", "Show:"), w);
        editShow = new KLineEdit(mdp->searchShowTitle(), w);
        gl->addWidget(labelShow, 6, 0);
        gl->addWidget(editShow, 6, 1);
    }

    if (mdp->resourceType() == QLatin1String("music")) {
        labelAuthor = new QLabel(i18nc("The music artist of the track", "Artist:"), w);
        editAuthor = new KLineEdit(mdp->searchPerson(), w);
        gl->addWidget(labelAuthor, 4, 0);
        gl->addWidget(editAuthor, 4, 1);

        labelTrack = new QLabel(i18nc("The single music piece/track ", "Track:"), w);
        editTrack = new KLineEdit(mdp->searchTrack(), w);
        gl->addWidget(labelTrack, 5, 0);
        gl->addWidget(editTrack, 5, 1);

        labelAlbum = new QLabel(i18nc("The name of the music album the track is in", "Album:"), w);
        editAlbum = new KLineEdit(mdp->searchAlbum(), w);
        gl->addWidget(labelAlbum, 6, 0);
        gl->addWidget(editAlbum, 6, 1);
    }

    spd->setMainWidget(w);

    int ret = spd->exec();

    if (ret == KDialog::Accepted) {
        mdp->setSearchTitle(editTitle->text());
        mdp->setSearchAltTitle(editAltTitle->text());
        mdp->setSearchYearMin(editYearMin->text());
        mdp->setSearchYearMax(editYearMax->text());

        if (mdp->resourceType() == QLatin1String("publication")) {
            mdp->setSearchPerson(editAuthor->text());
            mdp->setSearchJournal(editJournal->text());
        } else if (mdp->resourceType() == QLatin1String("tvshow")) {
            mdp->setSearchSeason(editSeason->text());
            lineEditSeason->setText(mdp->searchSeason());

            mdp->setSearchEpisode(editEpisode->text());
            lineEditEpisode->setText(mdp->searchEpisode());

            mdp->setSearchShowTitle(editShow->text());
            lineEditShow->setText(mdp->searchShowTitle());
        } else if (mdp->resourceType() == QLatin1String("music")) {
            mdp->setSearchPerson(editAuthor->text());
            mdp->setSearchAlbum(editAlbum->text());
            mdp->setSearchTrack(editTrack->text());
        }

        lineEditTitle->setText(mdp->searchTitle());
        lineEditArtist->setText(mdp->searchPerson());
        lineEditAlbum->setText(mdp->searchAlbum());
        lineEditSeason->setText(mdp->searchSeason());
        lineEditShow->setText(mdp->searchShowTitle());
        lineEditEpisode->setText(mdp->searchEpisode());
    }

    delete spd;
}

void NepomukWebMiner::UI::FetcherDialog::openDetailsLink(const QString &url)
{
    QString mimeTypeName = QLatin1String("text/html");
    /// Ask KDE subsystem to open url in viewer matching mime type
    KRun::runUrl(url, mimeTypeName, this, false, false);
}

void NepomukWebMiner::UI::FetcherDialog::fetchMoreDetails()
{
    Q_D(FetcherDialog);
    metaDataList->setBusy(true);
    busyFetching();

    // get the current item to fetch
    QModelIndex currentEntry = searchResults->currentIndex();
    QVariantMap entry = d->resultModel->searchResultEntry(currentEntry);
    kDebug() << "fetch details" << entry;

    d->currentItemToupdate = resourceExtractor()->resourcesList().at(d->currentItemNumber);
    KUrl fetchUrl(entry.value(QLatin1String("url")).toString());

    QVariantMap options;
    options.insert(QLatin1String("references"), MDESettings::downloadReferences());
    options.insert(QLatin1String("banner"), MDESettings::downloadBanner());

    d->webextractor->extractItem(fetchUrl, options);
}

void NepomukWebMiner::UI::FetcherDialog::fetchMoreAndSave()
{
    Q_D(FetcherDialog);
    d->saveAutomatically=true;
    fetchMoreDetails();
}

void NepomukWebMiner::UI::FetcherDialog::fetchedItemDetails(const QString &resourceType, QVariantMap itemDetails)
{
    Q_D(FetcherDialog);
    QVariantMap curMetaData = d->currentItemToupdate->metaData();
    curMetaData.insert(QLatin1String("resourceuri"), d->currentItemToupdate->resourceUri().url());
    curMetaData = itemDetails;
    d->currentItemToupdate->setMetaData(curMetaData);
    d->currentItemToupdate->setResourceType(resourceType);

    addResourceUriToMetaData(d->currentItemToupdate);
    d->currentItemToupdate->setMetaDataSaved(false);

    showItemDetails();
    finishedFetching();

    d->currentItemToupdate = 0;

    if(d->saveAutomatically) {
        saveMetaDataSlot();
    }

    d->saveAutomatically=false;

    //kDebug() << itemDetails;
}

void NepomukWebMiner::UI::FetcherDialog::saveMetaDataSlot()
{
    Q_D(FetcherDialog);
    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at(d->currentItemNumber);

    metaDataList->setBusy(true);
    busyFetching();

    saveMetaData(mdp);
    buttonSave->setEnabled(false);
    mdp->setMetaDataSaved(true);

    metaDataList->setBusy(false);
    finishedFetching();
}

void NepomukWebMiner::UI::FetcherDialog::cancelClose()
{
    close();
}

void NepomukWebMiner::UI::FetcherDialog::fillEngineList(const QString &category)
{
    // don't fetch information we already have
    if (comboBoxSearchEngine->property("currentListCategory").toString() == category) {
        return;
    }

    comboBoxSearchEngine->setProperty("currentListCategory", category);
    comboBoxSearchEngine->clear();

    QList<WebExtractor::Info> engines = extractorFactory()->listAvailablePlugins(category);

    foreach (const WebExtractor::Info & engine, engines) {
        comboBoxSearchEngine->addItem(QIcon(engine.icon),
                                      engine.name,
                                      engine.identifier);
    }

    if (category == QLatin1String("movie")) {
        comboBoxSearchEngine->setCurrentIndex(comboBoxSearchEngine->findData(MDESettings::favoriteMoviePlugin()));
    } else if (category == QLatin1String("tvshow")) {
        comboBoxSearchEngine->setCurrentIndex(comboBoxSearchEngine->findData(MDESettings::favoriteTvShowPlugin()));
    } else if (category == QLatin1String("music")) {
        comboBoxSearchEngine->setCurrentIndex(comboBoxSearchEngine->findData(MDESettings::favoriteMusicPlugin()));
    } else if (category == QLatin1String("publication")) {
        comboBoxSearchEngine->setCurrentIndex(comboBoxSearchEngine->findData(MDESettings::favoritePublicationPlugin()));
    }
}

void NepomukWebMiner::UI::FetcherDialog::showItemDetails()
{
    Q_D(FetcherDialog);
    // current Item metadata
    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at(d->currentItemNumber);

    metaDataList->setMetaDataParameter(mdp);

    kDebug() << mdp->metaDataSaved();
    if (mdp->metaDataSaved() == true) {
        buttonSave->setEnabled(false);
    } else {
        buttonSave->setEnabled(true);
    }
}

void NepomukWebMiner::UI::FetcherDialog::busyFetching()
{
    QWidget::setCursor(Qt::BusyCursor);

    buttonSearch->setEnabled(false);
    buttonFetchMore->setEnabled(false);
    buttonFetchAndSave->setEnabled(false);
    buttonNext->setEnabled(false);
    buttonPrevious->setEnabled(false);
    buttonSave->setEnabled(false);
}

void NepomukWebMiner::UI::FetcherDialog::finishedFetching()
{
    Q_D(FetcherDialog);
    d->busySearchWidget->stop();
    metaDataList->setBusy(false);

    QWidget::setCursor(Qt::ArrowCursor);

    MetaDataParameters *mdp = resourceExtractor()->resourcesList().at(d->currentItemNumber);
    buttonSave->setEnabled(!mdp->metaDataSaved());

    buttonSearch->setEnabled(true);

    QModelIndex currentEntry = searchResults->currentIndex();
    QVariantMap entry = d->resultModel->searchResultEntry(currentEntry);

    if (entry.contains(QLatin1String("url"))) {
        buttonFetchMore->setEnabled(true);
        buttonFetchAndSave->setEnabled(true);
    }

    int resourceCount = resourceExtractor()->resourcesList().size();
    if (d->currentItemNumber == resourceCount - 1) {
        buttonNext->setEnabled(false);
    } else {
        buttonNext->setEnabled(true);
    }

    // don't enable previous button, if there is no previous item
    if (d->currentItemNumber == 0) {
        buttonPrevious->setEnabled(false);
    } else {
        buttonPrevious->setEnabled(true);
    }
}
