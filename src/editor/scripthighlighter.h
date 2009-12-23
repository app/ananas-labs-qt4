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

#ifndef SCRIPTRUNNER_SCRIPTHIGHLIGHTER_H
#define SCRIPTRUNNER_SCRIPTHIGHLIGHTER_H

#include "textedit.h"

#include <QtGui/QSyntaxHighlighter>
#include <QtGui/QTextCharFormat>
#include <QtCore/QtAlgorithms>


class ScriptHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    ScriptHighlighter(TextEdit *editor);

    virtual void highlightBlock(const QString &text);

//     // Set formats from a sequence of type QTextCharFormat
//     template <class InputIterator>
//         void setFormats(InputIterator begin, InputIterator end) {
//             qCopy(begin, end, m_formats);
//         }

    enum ScriptFormats {
        ScriptTextFormat, ScriptNumberFormat,
        ScriptStringFormat, ScriptTypeFormat,
        ScriptKeywordFormat, ScriptPreprocessorFormat,
        ScriptLabelFormat, ScriptCommentFormat,
        NumScriptFormats
    };

public Q_SLOTS:
    void matchParentheses();

private:
    enum MatchType { NoMatch, Match, Mismatch  };
    static MatchType checkOpenParenthesis(QTextCursor *cursor, QChar c);
    static MatchType checkClosedParenthesis(QTextCursor *cursor, QChar c);
    static MatchType matchCursor(QTextCursor *cursor);

    void highlightWord(int currentPos, const QString &buffer);

    QTextCharFormat m_formats[NumScriptFormats];

    // parentheses matcher
    bool m_formatRange;
    QTextCharFormat m_matchFormat;
    QTextCharFormat m_mismatchFormat;
    QTextCharFormat m_rangeFormat;

    TextEdit *textEdit;
};


#endif // SCRIPTRUNNER_SCRIPTEDIT_H
