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

#ifndef TABSETTING_H
#define TABSETTING_H
#include <QtCore/QPair>
#include <QtGui/QTextBlock>

class QSettings;
class QString;
class QTextDocument;
class QTextCursor;

// Tab settings: Data type the TabSettingsPage acts on
// with some convenience functions for formatting.
struct TabSettings {
    TabSettings();

    void toSettings(const QString &category, QSettings *s) const;
    void fromSettings(const QString &category, const QSettings *s);

    void tabify(QString &s) const;
    void indentLine(QTextBlock block, int newIndent) const;

    // Determine indentation string
    QString indentationString(int indent) const;

    // A pair of <indentation>/<number of leading space characters<, returned by indentation.
    typedef QPair<int, int> IndentationNumCharsPair;

    // How to determine the  indentation
    enum  IndentationCountMode {
        // Count space characters only.
        CountSpaceCharacters,
        // Consider non-alphanumerical characters to be blanks.
        // This can be used for plain text editors for writing bulleted lists
        // with items that span several lines.
        CountNonAlphaNumerical };

    // Determine indentation
    IndentationNumCharsPair indentation(const QString &, IndentationCountMode icm = CountSpaceCharacters) const;

    bool m_spacesForTabs;
    bool m_autoIndent;
    bool m_indentSelection;
    int m_indentSize;

    bool equals(const TabSettings &ts) const;
};

inline bool operator==(const TabSettings &t1, const TabSettings &t2) { return t1.equals(t2); }
inline bool operator!=(const TabSettings &t1, const TabSettings &t2) { return !t1.equals(t2); }


#endif
