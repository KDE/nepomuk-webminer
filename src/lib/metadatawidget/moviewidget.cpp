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

#include "moviewidget.h"
#include "ui_moviewidget.h"

namespace NepomukWebMiner
{
namespace UI
{
class MovieWidgetPrivate
{
public:
    Ui::MovieWidget *ui;
    QVariantMap metadata;
};
}
}

NepomukWebMiner::UI::MovieWidget::MovieWidget(QWidget *parent)
    : MetaDataBaseWidget(parent)
    , d_ptr(new NepomukWebMiner::UI::MovieWidgetPrivate)
{
    Q_D(MovieWidget);
    d->ui = new Ui::MovieWidget();
    d->ui->setupUi(this);
}

NepomukWebMiner::UI::MovieWidget::~MovieWidget()
{
    Q_D(MovieWidget);
    delete d->ui;
}

void NepomukWebMiner::UI::MovieWidget::setMetaData(const QVariantMap &movie)
{
    Q_D(MovieWidget);
    d->metadata = movie;

    d->ui->editTitle->setText(movie.value(QLatin1String("title")).toString());
    d->ui->editGenre->setText(movie.value(QLatin1String("genre")).toString());
    d->ui->editYear->setText(movie.value(QLatin1String("year")).toString());
    d->ui->editPlot->setText(movie.value(QLatin1String("plot")).toString());
    d->ui->editDirector->setText(movie.value(QLatin1String("director")).toString());
    d->ui->editWriter->setText(movie.value(QLatin1String("writer")).toString());
    d->ui->editCast->setText(movie.value(QLatin1String("cast")).toString());
    d->ui->editSeeAlso->setText(movie.value(QLatin1String("seealso")).toString());
    d->ui->editPoster->setText(movie.value(QLatin1String("poster")).toString());
}

QVariantMap NepomukWebMiner::UI::MovieWidget::metaData() const
{
    Q_D(const MovieWidget);
    QVariantMap map = d->metadata;
    map.insert(QLatin1String("title"), d->ui->editTitle->text());
    map.insert(QLatin1String("genre"), d->ui->editGenre->text());
    map.insert(QLatin1String("year"), d->ui->editYear->text());
    map.insert(QLatin1String("plot"), d->ui->editPlot->document()->toPlainText());
    map.insert(QLatin1String("director"), d->ui->editDirector->text());
    map.insert(QLatin1String("writer"), d->ui->editWriter->text());
    map.insert(QLatin1String("cast"), d->ui->editCast->text());
    map.insert(QLatin1String("seealso"), d->ui->editSeeAlso->text());
    map.insert(QLatin1String("poster"), d->ui->editPoster->text());

    return map;
}
