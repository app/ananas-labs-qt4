/****************************************************************************
**
** Copyright (C) 2007-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Script Debug project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "tabsettings.h"
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>

static const char* spacesForTabsKey="SpacesForTabs";
static const char* autoIndentKey="AutoIndent";
static const char* indentSelectionKey="IndentSelection";
static const char* indentSizeKey="IndentSize";
static const char* groupPostfix ="TabSettings";


TabSettings::TabSettings() :
    m_spacesForTabs(true),
    m_autoIndent(true),
    m_indentSelection(true),
    m_indentSize(4)
{
}

void TabSettings::toSettings(const QString &category, QSettings *s) const
{
    QString group = QLatin1String(groupPostfix);
    if (!category.isEmpty())
        group.insert(0, category);
    s->beginGroup(group);
    s->setValue(QLatin1String(spacesForTabsKey),  m_spacesForTabs);
    s->setValue(QLatin1String(autoIndentKey), m_autoIndent);
    s->setValue(QLatin1String(indentSelectionKey), m_indentSelection);
    s->setValue(QLatin1String(indentSizeKey), m_indentSize);
    s->endGroup();
}

void TabSettings::fromSettings(const QString &category, const QSettings *s)
{
    QString group = QLatin1String(groupPostfix);
    if (!category.isEmpty())
        group.insert(0, category);
    group += QLatin1Char('/');

    *this = TabSettings();

    m_spacesForTabs   = s->value(group + QLatin1String(spacesForTabsKey), m_spacesForTabs).toBool();
    m_autoIndent      = s->value(group + QLatin1String(autoIndentKey), m_autoIndent).toBool();
    m_indentSelection = s->value(group + QLatin1String(indentSelectionKey), m_indentSelection).toBool();
    m_indentSize      = s->value(group + QLatin1String(indentSizeKey), m_indentSize).toInt();
}

void TabSettings::tabify(QString &s) const
{
    const QChar tab = QLatin1Char('\t');
    const QChar blank = QLatin1Char(' ');
    const QChar newLine = QLatin1Char('\n');

    if (m_spacesForTabs) {
        s.replace(tab, QString(m_indentSize, blank));
    } else {
        int i = 0;
        forever {
            for (int j = i; j < s.length(); ++j) {
                if (s.at(j) != blank && s.at(j) != tab) {
                    if (j > i) {
                        const QString t  = s.mid(i, j - i);
                        int spaces = 0;
                        for (int k = 0; k < t.length(); ++k)
                            spaces += (t.at(k) == blank ? 1 : m_indentSize);
                        s.remove(i, t.length());
                        const int tabs = spaces / m_indentSize;
                        spaces = spaces - (m_indentSize * tabs);
                        if (spaces > 0)
                            s.insert(i, QString(spaces, blank));
                        if (tabs > 0)
                            s.insert(i, QString(tabs, tab));
                    }
                    break;
                }
            }
            i = s.indexOf(newLine, i);
            if (i == -1)
                break;
            ++i;
        }
    }
}

QString TabSettings::indentationString(int indent) const
{
    if (indent == 0)
        return QString();
    QString indentString(indent, QLatin1Char(' '));
    indentString.append(QLatin1Char('a'));
    tabify(indentString);
    indentString.truncate(indentString.length() - 1);
    return indentString;
}

TabSettings::IndentationNumCharsPair TabSettings::indentation(const QString &s, IndentationCountMode icm) const
{
    IndentationNumCharsPair rc = IndentationNumCharsPair(0, 0);
    const int size = s.size();
    if (size == 0)
        return rc;

    const QChar tab = QLatin1Char('\t');
    const QChar blank = QLatin1Char(' ');

    for ( ; rc.second < size; rc.second++) {
        const QChar c = s.at(rc.second);
        if (c == tab)
            rc.first += m_indentSize;
        else {
            if (c == blank || (icm == CountNonAlphaNumerical && !c.isLetterOrNumber()))
                rc.first++;
            else
                break;
        }
    }
    return rc;
}

void TabSettings::indentLine(QTextBlock block, int newIndent) const
{
    const QString blockText = block.text();
    const int oldBlockLength = blockText.size();

    // Quickly check whether indenting is required.
    if (oldBlockLength == 0 && newIndent == 0)
        return;

    const QString indentString = indentationString(newIndent);
    newIndent = indentString.length();

    if (oldBlockLength == indentString.length() && blockText == indentString)
        return;

    if (oldBlockLength > indentString.length() &&
        blockText.startsWith(indentString) &&
        !blockText.at(indentString.length()).isSpace()) {
        return;
    }

    QTextCursor cursor(block);
    cursor.movePosition(QTextCursor::StartOfBlock);

    const IndentationNumCharsPair oldIndentation = indentation(blockText);
    if (oldIndentation.second > 0) {
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, oldIndentation.second);
        cursor.removeSelectedText();
    }

    if (!indentString.isEmpty()) {
        cursor.insertText(indentString);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, indentString.length());
    }
}

bool TabSettings::equals(const TabSettings &ts) const
{
    return m_spacesForTabs == ts.m_spacesForTabs &&
           m_autoIndent ==  ts.m_autoIndent &&
           m_indentSelection == ts.m_indentSelection &&
           m_indentSize == ts.m_indentSize;
}
