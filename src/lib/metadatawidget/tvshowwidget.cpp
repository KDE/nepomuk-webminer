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

#include "tvshowwidget.h"
#include "ui_tvshowwidget.h"

namespace NepomukWebMiner
{
namespace UI
{
class TvShowWidgetPrivate
{
public:
    Ui::TvShowWidget *ui;
    QVariantMap metadata;
};
}
}

NepomukWebMiner::UI::TvShowWidget::TvShowWidget(QWidget *parent)
    : MetaDataBaseWidget(parent)
    , d_ptr(new NepomukWebMiner::UI::TvShowWidgetPrivate)
{
    Q_D(TvShowWidget);
    d->ui = new Ui::TvShowWidget();
    d->ui->setupUi(this);
}

NepomukWebMiner::UI::TvShowWidget::~TvShowWidget()
{
    Q_D(TvShowWidget);
    delete d->ui;
}

void NepomukWebMiner::UI::TvShowWidget::setMetaData(const QVariantMap &tvshow)
{
    Q_D(TvShowWidget);
    d->metadata = tvshow;

    d->ui->editShowTitle->setText(tvshow.value(QLatin1String("title")).toString());
    d->ui->editShowSynopsis->setText(tvshow.value(QLatin1String("overview")).toString());
    d->ui->editShowBanner->setText(tvshow.value(QLatin1String("banner")).toString());
    d->ui->editShowPoster->setText(tvshow.value(QLatin1String("poster")).toString());

    QVariantList seasonlist = tvshow.value(QLatin1String("seasons")).toList();
    if(seasonlist.isEmpty()) {
        return;
    }
    QVariantMap season = seasonlist.first().toMap();

    d->ui->editSeasonNumber->setValue(season.value(QLatin1String("number")).toInt());
    d->ui->editSeasonBanner->setText(season.value(QLatin1String("banner")).toString());
    d->ui->editSeasonPoster->setText(season.value(QLatin1String("poster")).toString());

    QVariantList episodeList = season.value(QLatin1String("episodes")).toList();
    if(episodeList.isEmpty()) {
        return;
    }
    QVariantMap episode = episodeList.first().toMap();

    d->ui->editEpisodeActors->setText(episode.value(QLatin1String("actors")).toString());
    d->ui->editEpisodeBanner->setText(episode.value(QLatin1String("banner")).toString());
    d->ui->editEpisodeDirector->setText(episode.value(QLatin1String("director")).toString());
    d->ui->editEpisodeFirstAired->setText(episode.value(QLatin1String("firstaired")).toString());
    d->ui->editEpisodeGenres->setText(episode.value(QLatin1String("genres")).toString());
    d->ui->editEpisodeNumber->setValue(episode.value(QLatin1String("number")).toInt());
    d->ui->editEpisodePoster->setText(episode.value(QLatin1String("poster")).toString());
    d->ui->editEpisodeSeeAlso->setText(episode.value(QLatin1String("seealso")).toString());
    d->ui->editEpisodeSynopsis->setText(episode.value(QLatin1String("overview")).toString());
    d->ui->editEpisodeTitle->setText(episode.value(QLatin1String("title")).toString());
    d->ui->editEpisodeWriter->setText(episode.value(QLatin1String("writer")).toString());
}

QVariantMap NepomukWebMiner::UI::TvShowWidget::metaData() const
{
    Q_D(const TvShowWidget);
    QVariantMap map = d->metadata;

    map.insert(QLatin1String("title"), d->ui->editShowTitle->text());
    map.insert(QLatin1String("overview"), d->ui->editShowSynopsis->document()->toPlainText());
    map.insert(QLatin1String("banner"), d->ui->editShowBanner->text());
    map.insert(QLatin1String("poster"), d->ui->editShowPoster->text());

    QVariantList seasonlist = map.value(QLatin1String("seasons")).toList();
    QVariantMap season = QVariantMap();

    if(!seasonlist.isEmpty()) {
        season = seasonlist.first().toMap();
    }

    season.insert(QLatin1String("number"), d->ui->editSeasonNumber->value());
    season.insert(QLatin1String("banner"), d->ui->editSeasonBanner->text());
    season.insert(QLatin1String("poster"), d->ui->editSeasonPoster->text());

    QVariantList episodeList = season.value(QLatin1String("episodes")).toList();
    QVariantMap episode = QVariantMap();

    if(!episodeList.isEmpty()) {
        episode = episodeList.first().toMap();
    }

    episode.insert(QLatin1String("actors"), d->ui->editEpisodeActors->text());
    episode.insert(QLatin1String("banner"), d->ui->editEpisodeBanner->text());
    episode.insert(QLatin1String("director"), d->ui->editEpisodeDirector->text());
    episode.insert(QLatin1String("firstaired"), d->ui->editEpisodeFirstAired->text());
    episode.insert(QLatin1String("genres"), d->ui->editEpisodeGenres->text());
    episode.insert(QLatin1String("number"), d->ui->editEpisodeNumber->value());
    episode.insert(QLatin1String("poster"), d->ui->editEpisodePoster->text());
    episode.insert(QLatin1String("seealso"), d->ui->editEpisodeSeeAlso->text());
    episode.insert(QLatin1String("overview"), d->ui->editEpisodeSynopsis->document()->toPlainText());
    episode.insert(QLatin1String("title"), d->ui->editEpisodeTitle->text());
    episode.insert(QLatin1String("writer"), d->ui->editEpisodeWriter->text());

    episodeList.replace(0, episode);

    // insert data back into the map
    season.insert(QLatin1String("episodes"), episodeList);
    seasonlist.replace(0, season);

    map.insert(QLatin1String("seasons"), seasonlist);

    return map;
}

