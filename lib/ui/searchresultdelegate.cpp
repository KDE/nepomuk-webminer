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

#include "searchresultdelegate.h"

#include <QtGui/QApplication>
#include <QtGui/QPainter>

NepomukWebMiner::UI::SearchResultDelegate::SearchResultDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void NepomukWebMiner::UI::SearchResultDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    QString title   = index.data(Qt::DisplayRole).toString();
    QString details = index.data(Qt::UserRole).toString();

    // draw correct background
    opt.text = "";
    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    QRect rect = opt.rect;
    QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
        cg = QPalette::Inactive;

    // set pen color
    if (opt.state & QStyle::State_Selected)
        painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
    else
        painter->setPen(opt.palette.color(cg, QPalette::Text));

    // draw 2 lines of text
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(9);
    painter->setFont(titleFont);
    painter->drawText(QRect(rect.left(), rect.top(), rect.width(), rect.height() / 2),
                      opt.displayAlignment, title);

    QFont infoFont;
    infoFont.setBold(false);
    infoFont.setItalic(true);
    infoFont.setPointSize(8);
    painter->setFont(infoFont);
    painter->drawText(QRect(rect.left(), rect.top() + rect.height() / 2, rect.width(), rect.height() / 2),
                      opt.displayAlignment, details);
}

QSize NepomukWebMiner::UI::SearchResultDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QSize result = QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(result.height() * 2);
    return result;
}
