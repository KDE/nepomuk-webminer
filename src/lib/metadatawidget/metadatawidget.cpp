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

#include "metadatawidget.h"

#include "metadataparameters.h"
#include "metadatabasewidget.h"
#include "moviewidget.h"
#include "musicwidget.h"
#include "tvshowwidget.h"
#include "publicationwidget.h"

#include <QtGui/QVBoxLayout>

#include <KDE/KDebug>

namespace NepomukWebMiner
{
namespace UI
{
class MetaDataWidgetPrivate
{
public:
    NepomukWebMiner::UI::MetaDataBaseWidget *mdbw;
    QVariantMap metadata;
};
}
}

NepomukWebMiner::UI::MetaDataWidget::MetaDataWidget(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new NepomukWebMiner::UI::MetaDataWidgetPrivate)
{
    Q_D(MetaDataWidget);
    d->mdbw = 0;

    QVBoxLayout * vLayout = new QVBoxLayout(this);
    setLayout(vLayout);
}

void NepomukWebMiner::UI::MetaDataWidget::setMetaDataParameter(const QVariantMap &metadata)
{
    Q_D(MetaDataWidget);
    d->metadata = metadata;

    d->mdbw->setMetaData(metadata);
}

QVariantMap NepomukWebMiner::UI::MetaDataWidget::metaData() const
{
    Q_D(const MetaDataWidget);
    return d->mdbw->metaData();
}

void NepomukWebMiner::UI::MetaDataWidget::switchWidget(const QString &resourceType)
{
    Q_D(MetaDataWidget);

    // first delete any previous widget
    layout()->removeWidget(d->mdbw);
    delete d->mdbw;
    d->mdbw = 0;

    // create a new metadata widget based on the current resourcectype
    if(resourceType == QLatin1String("publication")) {
        d->mdbw = new NepomukWebMiner::UI::PublicationWidget();
    }
    else if(resourceType == QLatin1String("movie")) {
        d->mdbw = new NepomukWebMiner::UI::MovieWidget();
    }
    else if(resourceType == QLatin1String("tvshow")) {
        d->mdbw = new NepomukWebMiner::UI::TvShowWidget();
    }
    else if(resourceType == QLatin1String("music")) {
        d->mdbw = new NepomukWebMiner::UI::MusicWidget();
    }

    // TODO: add raw metadata widget again
    if(d->mdbw) {
        d->mdbw->setMetaData(d->metadata);

        layout()->addWidget(d->mdbw);
    }
    else {
        kWarning() << "Unknown resource type:" << resourceType;
    }
}
