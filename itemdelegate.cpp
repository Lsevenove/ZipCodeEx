/*
    Copyright (c) 2009-10 Qtrac Ltd. All rights reserved.

    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version. It is provided
    for educational purposes and is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
    the GNU General Public License for more details.
*/

#include "global.hpp"
#include "itemdelegate.hpp"
#include "proxymodel.hpp"
#include "zipcodespinbox.hpp"
#include <QComboBox>
#include <QModelIndex>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QDebug>


// adjusted gives a 3 pixel right margin
void ItemDelegate::paint(QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{

    qDebug()<<"___________________________________________paint";
    if (index.column() == Zipcode) {
        QStyleOptionViewItem opt(option);
        initStyleOption(&opt, index);
        QString text = QString("%1").arg(opt.text.toInt(),5, 10, QChar('0'));
        //绘制之前首先保存painter 这样绘制完成后用restore函数回复之前的painter状态
        painter->save();
        //设置反走样渲染方式
        painter->setRenderHints(QPainter::Antialiasing|
                                QPainter::TextAntialiasing);
        //检查选中状态
        if (opt.state & QStyle::State_Selected) {
            painter->fillRect(opt.rect, opt.palette.highlight());
            painter->setPen(opt.palette.highlightedText().color());
        }
        else
            painter->setPen(opt.palette.windowText().color());
        painter->drawText(opt.rect.adjusted(0, 0, -3, 0), text,
                QTextOption(Qt::AlignVCenter|Qt::AlignRight));
        //回复之前的painter状态
        painter->restore();
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}


QWidget *ItemDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    qDebug()<<"___________________________________________createEditor";
    static QStringList usStates;
    if (usStates.isEmpty())
        usStates << "(Unknown)"         << "Alabama"        << "Alaska"
            << "Arizona"                << "Arkansas"       << "California"
            << "Colorado"               << "Connecticut"    << "Delaware"
            << "District Of Columbia"   << "Florida"        << "Georgia"
            << "Hawaii"                 << "Idaho"          << "Illinois"       << "Indiana"
            << "Iowa"                   << "Kansas"         << "Kentucky"       << "Louisiana"
            << "Maine"                  << "Maryland"       << "Massachusetts"
            << "Michigan"               << "Minnesota"      << "Mississippi"
            << "Missouri"               << "Montana"        << "Nebraska"       << "Nevada"
            << "New Hampshire"          << "New Jersey"     << "New Mexico"
            << "New York"               << "North Carolina" << "North Dakota"
            << "Ohio"                   << "Oklahoma"       << "Oregon"         << "Pennsylvania"
            << "Rhode Island"           << "South Carolina"
            << "South Dakota"           << "State"          << "Tennessee"      << "Texas"
            << "Utah"                   << "Vermont"        << "Virginia"       << "Washington"
            << "West Virginia"          << "Wisconsin"      << "Wyoming";

    if (index.column() == Zipcode)
        return new ZipcodeSpinBox(parent);
    if (index.column() == State) {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(usStates);
        return editor;
    }
    //为什么要返回父类方法？
    return QStyledItemDelegate::createEditor(parent, option, index);
}


void ItemDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    qDebug()<<"___________________________________________setEditorData";
    if (index.column() == Zipcode) {
        int value = index.model()->data(index).toInt();
        ZipcodeSpinBox *spinBox =
                qobject_cast<ZipcodeSpinBox*>(editor);
        Q_ASSERT(spinBox);
        spinBox->setValue(value);
    }
    else if (index.column() == State) {
        QString state = index.model()->data(index).toString();
        QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
        Q_ASSERT(comboBox);
        comboBox->setCurrentIndex(comboBox->findText(state));
    }
    else
        QStyledItemDelegate::setEditorData(editor, index);
}


void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug()<<"___________________________________________setEditorData";
    if (index.column() == Zipcode) {
        ZipcodeSpinBox *spinBox =
                qobject_cast<ZipcodeSpinBox*>(editor);
        Q_ASSERT(spinBox);
        spinBox->interpretText();
        model->setData(index, spinBox->value());
    }
    else if (index.column() == State) {
        QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
        Q_ASSERT(comboBox);
        model->setData(index, comboBox->currentText());
    }
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}
