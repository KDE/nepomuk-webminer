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

#include "musicwidget.h"
#include "ui_musicwidget.h"

#include "metadataparameters.h"

namespace NepomukWebMiner
{
namespace UI
{
class MusicWidgetPrivate
{
public:
    Ui::MusicWidget *ui;
    QVariantMap metadata;
};
}
}

NepomukWebMiner::UI::MusicWidget::MusicWidget(QWidget *parent)
    : MetaDataBaseWidget(parent)
    , d_ptr(new NepomukWebMiner::UI::MusicWidgetPrivate)
{
    Q_D(MusicWidget);
    d->ui = new Ui::MusicWidget();
    d->ui->setupUi(this);
}

NepomukWebMiner::UI::MusicWidget::~MusicWidget()
{
    Q_D(MusicWidget);
    delete d->ui;
}

void NepomukWebMiner::UI::MusicWidget::setMetaData(const QVariantMap &musicAlbum)
{
    Q_D(MusicWidget);
    d->metadata = musicAlbum;

    d->ui->editAlbumName->setText(musicAlbum.value(QLatin1String("title")).toString());
    d->ui->editAlbumPerformer->setText(musicAlbum.value(QLatin1String("performer")).toString());
    d->ui->editAlbumTracks->setValue(musicAlbum.value(QLatin1String("trackcount")).toInt());
    d->ui->editAlbumCover->setText(musicAlbum.value(QLatin1String("cover")).toString());

    QVariantList trackList = musicAlbum.value(QLatin1String("tracks")).toList();

    if(!trackList.isEmpty()) {
        QVariantMap track = trackList.first().toMap();

        d->ui->editTrackName->setText(track.value(QLatin1String("title")).toString());
        d->ui->editTrackNumber->setValue(track.value(QLatin1String("number")).toInt());
        d->ui->editTrackGenre->setText(track.value(QLatin1String("genre")).toString());
        d->ui->editTrackPerformer->setText(track.value(QLatin1String("performer")).toString());
        d->ui->editTrackReleaseDate->setText(track.value(QLatin1String("releasedate")).toString());
        d->ui->editTrackSeeAlso->setText(track.value(QLatin1String("seealso")).toString());
        d->ui->editTrackLyrics->setText(track.value(QLatin1String("lyrics")).toString());
    }
}

QVariantMap NepomukWebMiner::UI::MusicWidget::metaData() const
{
    Q_D(const MusicWidget);
    QVariantMap map = d->metadata;
    map.insert(QLatin1String("title"), d->ui->editAlbumName->text());
    map.insert(QLatin1String("performer"), d->ui->editAlbumPerformer->text());
    map.insert(QLatin1String("trackcount"), d->ui->editAlbumTracks->value());
    map.insert(QLatin1String("cover"), d->ui->editAlbumCover->text());

    QVariantList trackList = map.value(QLatin1String("tracks")).toList();

    QVariantMap track = QVariantMap();

    if(!trackList.isEmpty()) {
        track = trackList.first().toMap();
    }

    track.insert(QLatin1String("title"), d->ui->editTrackName->text());
    track.insert(QLatin1String("number"), d->ui->editTrackNumber->value());
    track.insert(QLatin1String("genre"), d->ui->editTrackGenre->text());
    track.insert(QLatin1String("performer"), d->ui->editTrackPerformer->text());
    track.insert(QLatin1String("releasedate"), d->ui->editTrackReleaseDate->text());
    track.insert(QLatin1String("seealso"), d->ui->editTrackSeeAlso->text());
    track.insert(QLatin1String("lyrics"), d->ui->editTrackLyrics->document()->toPlainText());

    map.insert(QLatin1String("tracks"), track);

    return map;
}
