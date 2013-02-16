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

#include "configregexp.h"
#include "ui_configregexp.h"

#include "resourceextractor/filenameanalyzer.h"
#include "mdesettings.h"

NepomukWebMiner::ConfigRegExp::ConfigRegExp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConfigRegExp)
{
    ui->setupUi(this);

    loadConfig();
    setupUi();
}

NepomukWebMiner::ConfigRegExp::~ConfigRegExp()
{
    delete ui;
}

void NepomukWebMiner::ConfigRegExp::saveConfig()
{
    MDESettings::setStripNames( ui->rbStrip->items() );

    QString regExpTvShow = Extractor::FilenameAnalyzer::writeRegExpConfig( ui->rbTvShows->regExpData() );
    MDESettings::setRegExpTvShow( regExpTvShow );

    QString regExpMovies = Extractor::FilenameAnalyzer::writeRegExpConfig( ui->rbMovies->regExpData() );
    MDESettings::setRegExpMovie( regExpMovies );

    QString regExpMusic = Extractor::FilenameAnalyzer::writeRegExpConfig( ui->rbMusic->regExpData() );
    MDESettings::setRegExpMusic( regExpMusic );

    QString regExpDocument = Extractor::FilenameAnalyzer::writeRegExpConfig( ui->rbDocuments->regExpData() );
    MDESettings::setRegExpDocument( regExpDocument );

    MDESettings::self()->writeConfig();
}

void NepomukWebMiner::ConfigRegExp::loadConfig()
{
    // load current config data
    ui->rbStrip->setItems( MDESettings::stripNames() );

    ui->rbTvShows->setRegExpData(Extractor::FilenameAnalyzer::parseRegExpConfig( MDESettings::regExpTvShow() ));
    ui->rbMovies->setRegExpData(Extractor::FilenameAnalyzer::parseRegExpConfig( MDESettings::regExpMovie() ));
    ui->rbMusic->setRegExpData(Extractor::FilenameAnalyzer::parseRegExpConfig( MDESettings::regExpMusic() ));
    ui->rbDocuments->setRegExpData(Extractor::FilenameAnalyzer::parseRegExpConfig( MDESettings::regExpDocument()));
}

void NepomukWebMiner::ConfigRegExp::setupUi()
{
    connect(ui->rbTvShows, SIGNAL(configChanged()), this, SIGNAL(configChanged()));
    connect(ui->rbMovies, SIGNAL(configChanged()), this, SIGNAL(configChanged()));
    connect(ui->rbMusic, SIGNAL(configChanged()), this, SIGNAL(configChanged()));
    connect(ui->rbDocuments, SIGNAL(configChanged()), this, SIGNAL(configChanged()));
    connect(ui->rbStrip, SIGNAL(changed()), this, SIGNAL(configChanged()));
}
