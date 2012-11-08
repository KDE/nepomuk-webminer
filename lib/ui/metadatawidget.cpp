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

#include "metadatawidget.h"

#include "metadataparameters.h"

#include <KDE/KTabWidget>
#include <KDE/KLocalizedString>

#include <KDE/KPixmapSequence>
#include <KDE/KPixmapSequenceOverlayPainter>

#include <QtGui/QTreeView>
#include <QtGui/QStandardItemModel>

#include <QtGui/QVBoxLayout>
#include <QtCore/QVariant>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>
#include <QtGui/QHeaderView>

#include <KDE/KDebug>

namespace NepomukMetaDataExtractor
{
namespace UI
{
class MetaDataWidgetPrivate
{
public:
    const NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp;
    KTabWidget *tabWidget;
    QTreeView *newMetaDataWidget;
    QStandardItemModel *newMetaDataModel;
    KPixmapSequenceOverlayPainter *busyDataWidget;
    //QTreeView *oldMetaDataWidget;
};
}
}

using namespace NepomukMetaDataExtractor::Extractor;
using namespace NepomukMetaDataExtractor::UI;

NepomukMetaDataExtractor::UI::MetaDataWidget::MetaDataWidget(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new NepomukMetaDataExtractor::UI::MetaDataWidgetPrivate)
{
    setupWidget();
}

void NepomukMetaDataExtractor::UI::MetaDataWidget::setMetaDataParameter(const NepomukMetaDataExtractor::Extractor::MetaDataParameters *mdp)
{
    Q_D(MetaDataWidget);
    d->mdp = mdp;

    setUpNewDataWidget();
}

void NepomukMetaDataExtractor::UI::MetaDataWidget::setBusy(bool busy)
{
    Q_D(MetaDataWidget);
    if (busy) {
        d->busyDataWidget->start();
    } else {
        d->busyDataWidget->stop();
    }
}

void NepomukMetaDataExtractor::UI::MetaDataWidget::setUpNewDataWidget()
{
    Q_D(MetaDataWidget);

    d->newMetaDataModel->clear();

    d->newMetaDataModel->blockSignals(true);
    insertIntoTree(0, d->mdp->metaData());
    d->newMetaDataModel->blockSignals(false);

    d->newMetaDataWidget->expandAll();
    d->newMetaDataWidget->resizeColumnToContents(0);
}

void NepomukMetaDataExtractor::UI::MetaDataWidget::insertIntoTree(QStandardItem *root, const QVariantMap &data)
{
    Q_D(MetaDataWidget);

    QMapIterator<QString, QVariant> i(data);
    while (i.hasNext()) {
        i.next();

        QVariant value = i.value();
        if (value.convert(QVariant::String)) {

            QStandardItem *keyItem   = new QStandardItem(i18nKey(i.key()));
            keyItem->setEditable(false);
            QStandardItem *valueItem = new QStandardItem(i.value().toString());
            valueItem->setEditable(false);

            if (root) {
                keyItem->setData(QString("%1|%2").arg(root->data().toString()).arg(i.key()));
                valueItem->setData(keyItem->data());
                root->appendRow(QList<QStandardItem *>() << keyItem << valueItem);
            } else {
                keyItem->setData(i.key());
                valueItem->setData(keyItem->data());
                d->newMetaDataModel->invisibleRootItem()->appendRow(QList<QStandardItem *>() << keyItem << valueItem);
            }
        } else {
            QStandardItem *keyItem   = new QStandardItem(i18nKey(i.key()));
            keyItem->setEditable(false);
            QStandardItem *valueItem = new QStandardItem(QString(""));
            valueItem->setEditable(false);

            if (root) {
                keyItem->setData(QString("%1|%2").arg(root->data().toString()).arg(i.key()));
                valueItem->setData(keyItem->data());
                root->appendRow(keyItem);
            } else {
                keyItem->setData(i.key());
                valueItem->setData(keyItem->data());
                d->newMetaDataModel->invisibleRootItem()->appendRow(QList<QStandardItem *>() << keyItem << valueItem);
            }

            QVariantList valueList =  i.value().toList();
            if (valueList.size() == 1) {
                insertIntoTree(keyItem, valueList.first().toMap());
            } else {
                int z = 1;
                foreach (const QVariant & listValue, valueList) {
                    QStandardItem *keyItem2   = new QStandardItem(QString("%1").arg(z));
                    keyItem2->setEditable(false);
                    QStandardItem *valueItem2 = new QStandardItem(QString(""));
                    valueItem2->setEditable(false);

                    keyItem2->setData(QString("%1|%2").arg(keyItem->data().toString()).arg(QString("%1").arg(z)));
                    valueItem2->setData(keyItem2->data());

                    keyItem->appendRow(QList<QStandardItem *>() << keyItem2 << valueItem2);

                    insertIntoTree(keyItem2, listValue.toMap());
                    z++;
                }
            }
        }
    }
}

QString NepomukMetaDataExtractor::UI::MetaDataWidget::i18nKey(const QString &key)
{
    if (key == QString("title")) {
        return i18nc("Title of the Show/Publication/Episode/Track", "Title");
    }
    if (key == QString("resourceuri")) {
        return i18nc("The Nepomuk resource Uri", "Resource");
    }
    if (key == QString("episodes")) {
        return i18nc("The list of tv show Episodes", "Episodes");
    }
    if (key == QString("writer")) {
        return i18nc("The writer of the tv show/movie script", "Writer");
    }
    if (key == QString("overview")) {
        return i18nc("Tvshow/ MoviePlot overview", "Overview");
    }
    if (key == QString("firstaired")) {
        return i18nc("First aired date of a tvshow", "First-Aired");
    }
    if (key == QString("director")) {
        return i18nc("The director of a movie/tvshow", "Director");
    }
    if (key == QString("number")) {
        return i18nc("The number of journal", "Number");
    }
    if (key == QString("actors")) {
        return i18nc("Actor names of tvshows/movies", "Actors");
    }
    if (key == QString("genre")) {
        return i18nc("The genre of the movie/tvshow/music track", "Genre");
    }
    if (key == QString("banner")) {
        return i18nc("Banner url for the movie/tvshow", "Banner");
    }
    if (key == QString("poster")) {
        return i18nc("Poster url for the movie/tvshow", "Poster");
    }
    if (key == QString("seasons")) {
        return i18nc("List of season in the tvshow", "Seasons");
    }
    if (key == QString("performer")) {
        return i18nc("The performer of a music track", "Performer");
    }
    if (key == QString("musicbrainz")) {
        return i18nc("MusicBrainz id for a music track", "MusicBrainz");
    }
    if (key == QString("tracks")) {
        return i18nc("List of tracks for a music album", "Tracks");
    }
    if (key == QString("releasedate")) {
        return i18nc("Release date of the movie", "Release date");
    }
    if (key == QString("year")) {
        return i18nc("Release/publication year", "Year");
    }
    if (key == QString("cast")) {
        return i18nc("Movie/ Tvshow cast member", "Cast");
    }
    if (key == QString("plot")) {
        return i18nc("Tvshow/ MoviePlot overview", "Plot");
    }
    if (key == QString("runtime")) {
        return i18nc("Movie/tvshow length", "Runtime");
    }
    if (key == QString("countries")) {
        return i18nc("List of countries the movie is from", "Countries");
    }

    //bibtex related
    if (key == QString("abstract")) {
        return i18nc("The publication abstract", "Abstract");
    }
    if (key == QString("author")) {
        return i18nc("Publication author", "Author");
    }
    if (key == QString("booktitle")) {
        return i18nc("Publication booktitle", "Booktitle");
    }
    if (key == QString("bibreferences")) {
        return i18nc("List of publication references", "References");
    }
    if (key == QString("pages")) {
        return i18nc("Range of pages like 15--45", "Pages");
    }
    if (key == QString("doi")) {
        return i18nc("Digital object identifier", "DOI");
    }
    if (key == QString("bibtexentrytype")) {
        return i18nc("The publication type. Like Book, Article, Blog etc", "Type");
    }
    if (key == QString("chapter")) {
        return i18nc("The chapter (name and/or number) that will be referenced", "Chapter");
    }
    if (key == QString("volume")) {
        return i18nc("The volume of the publication journal", "Volume");
    }
    if (key == QString("issue")) {
        return i18nc("The issue number of the journal", "Issue");
    }
    if (key == QString("journal")) {
        return i18nc("The name of the journal", "Journal");
    }

    return key;
}

void NepomukMetaDataExtractor::UI::MetaDataWidget::setupWidget()
{
    Q_D(MetaDataWidget);

    // main layout for the widget
    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    // Setup treeWidgets
    d->newMetaDataModel = new QStandardItemModel;
    d->newMetaDataWidget = new QTreeView;
    d->newMetaDataWidget->setModel(d->newMetaDataModel);
    d->newMetaDataWidget->setHeaderHidden(true);
    d->newMetaDataWidget->setSortingEnabled(true);
    d->newMetaDataWidget->setUniformRowHeights(true);
    layout->addWidget(d->newMetaDataWidget);

    d->busyDataWidget = new KPixmapSequenceOverlayPainter(this);
    d->busyDataWidget->setSequence(KPixmapSequence("process-working", KIconLoader::SizeSmallMedium));
    d->busyDataWidget->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    d->busyDataWidget->setWidget(d->newMetaDataWidget->viewport());

    //connect(d->newMetaDataModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(newMetaDataChanged(QStandardItem*)) );
    /*
        d->oldMetaDataWidget = new QTreeView;
        d->oldMetaDataWidget->setHeaderHidden(true);
        d->oldMetaDataWidget->setRootIsDecorated(false);
        d->oldMetaDataWidget->setSortingEnabled(true);
        QHeaderView *oldHv = d->oldMetaDataWidget->header();
        oldHv->setResizeMode(0,QHeaderView::ResizeToContents);

        // tab widget show
        d->tabWidget = new KTabWidget();
        d->tabWidget->setDocumentMode(true);
        d->tabWidget->setTabPosition( QTabWidget::South );
        layout->addWidget( d->tabWidget );

        d->tabWidget->addTab(d->newMetaDataWidget, tr("New"));
        d->tabWidget->addTab(d->oldMetaDataWidget, tr("Old"));
        d->tabWidget->addTab(d->diffMetaDataWidget, tr("Diff"));
        */
}
