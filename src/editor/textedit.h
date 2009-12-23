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

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QtGui/QPlainTextEdit>
#include "tabsettings.h"
#include <QtGui/QTextCharFormat>
#include <QtGui/QTextBlockUserData>
#include <QtGui/QIcon>
#include <QtCore/QBasicTimer>
#include <QtCore/QDebug>


struct Parenthesis;
typedef QVector<Parenthesis> Parentheses;
struct Parenthesis
{
    enum Type { Opened, Closed };

    inline Parenthesis() : type(Opened), pos(-1) {}
    inline Parenthesis(Type t, QChar c, int position)
        : type(t), chr(c), pos(position) {}
    Type type;
    QChar chr;
    int pos;
    static int collapseAtPos(const Parentheses &parentheses);
};

class TextBlockUserData : public QTextBlockUserData {
public:

    enum Collapse { CollapseThis, CollapseAfter, NoCollapse };

    inline TextBlockUserData()
        :m_breakpoint(false), m_pendingBreakpoint(false), m_bookmark(false),
         m_collapseIncludesClosure(false),
         m_collapse(NoCollapse) {}
    inline bool breakpoint() const { return m_breakpoint; }
    inline bool pendingBreakpoint() const { return m_pendingBreakpoint; }
    inline bool bookmark() const { return m_bookmark; }
    inline Collapse collapse() const { return m_collapse; }
    inline bool collapseIncludesClosure() const { return m_collapseIncludesClosure; }
    inline void setBookmark(bool b) { m_bookmark = b; }
    inline void setBreakpoint(bool b){ m_breakpoint = b; }
    inline void setPendingBreakpoint(bool b){ m_pendingBreakpoint = b; }
    inline void setCollapse(Collapse c) { m_collapse = c; }
    inline void setCollapseIncludesClosure(bool b) { m_collapseIncludesClosure = b; }

    inline void setParentheses(const Parentheses &parentheses) { m_parentheses = parentheses; }
    inline void clearParentheses() { m_parentheses.clear(); }
    inline const Parentheses &parentheses() const { return m_parentheses; }

    inline bool hasParentheses() const { return !m_parentheses.isEmpty(); }

    inline static bool canCollapse(const QTextBlock& block) {
        return (block.userData()
                && static_cast<TextBlockUserData*>(block.userData())->collapse()
                == CollapseAfter)
            || (block.next().userData()
                && static_cast<TextBlockUserData*>(block.next().userData())->collapse()
                == TextBlockUserData::CollapseThis);
    }

    static void setCollapse(const QTextBlock& block, bool visible);

    bool hasClosingCollapse() const;
    int collapseAtPos() const;

    enum MatchType { NoMatch, Match, Mismatch  };
    static MatchType checkOpenParenthesis(QTextCursor *cursor, QChar c);
    static MatchType checkClosedParenthesis(QTextCursor *cursor, QChar c);
    static MatchType matchCursorBackward(QTextCursor *cursor);
    static MatchType matchCursorForward(QTextCursor *cursor);



private:
    bool m_breakpoint;
    bool m_pendingBreakpoint;
    bool m_bookmark;
    bool m_collapseIncludesClosure;
    Collapse m_collapse;
    Parentheses m_parentheses;
};

class TextEditDocumentLayout : public QPlainTextDocumentLayout
{
    Q_OBJECT

public:

    TextEditDocumentLayout(QTextDocument *doc):QPlainTextDocumentLayout(doc) {
        lastSaveRevision = markCount = doubleMarkCount = 0;
        m_executionBlockNumber = -1;
    }
    ~TextEditDocumentLayout(){}

    QRectF blockBoundingRect(const QTextBlock &block) const;

    static void setParentheses(const QTextBlock &block, const Parentheses &parentheses);
    static void clearParentheses(const QTextBlock &block) { setParentheses(block, Parentheses());}
    static Parentheses parentheses(const QTextBlock &block);
    static bool hasParentheses(const QTextBlock &block);

    static TextBlockUserData *testUserData(const QTextBlock &block) {
        return static_cast<TextBlockUserData*>(block.userData());
    }
    static TextBlockUserData *userData(const QTextBlock &block) {
        TextBlockUserData *data = static_cast<TextBlockUserData*>(block.userData());
        if (!data && block.isValid())
            const_cast<QTextBlock &>(block).setUserData((data = new TextBlockUserData));
        return data;
    }

    inline void setExecutionBlockNumber(int blockNumber) {
        m_executionBlockNumber = blockNumber;
        requestUpdate();
    }

    inline int executionBlockNumber() const { return m_executionBlockNumber; }

    int lastSaveRevision;
    int markCount;
    int doubleMarkCount;

    int m_executionBlockNumber;
};

class TextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget *parent = 0);
    ~TextEdit();

    QWidget *extraArea() const;
    virtual int extraAreaWidth(int *markWidthPtr = 0) const;
    virtual void extraAreaPaintEvent(QPaintEvent *);
    virtual void extraAreaMouseEvent(QMouseEvent *);


    inline const TabSettings &tabSettings() const { return m_tabsettings; }

    QList<int> bookmarks() const;
    void setBookmarks(const QList<int> &blockNumbers);
    QList<int> breakpoints() const;
    void setBreakpoints(const QList<int> &blockNumbers);
    QList<int> pendingBreakpoints() const;
    void setPendingBreakpoints(const QList<int> &blockNumbers);


    void markBlocksAsChanged(QList<int> blockNumbers);

    int executionBlockNumber() const;
    void setExecutionBlockNumber(int blockNumber);

    int baseLineNumber() const { return baseLine; }
    void setBaseLineNumber(int lineNumber) { baseLine = lineNumber; }
    int cursorLineNumber() const { return textCursor().blockNumber() + baseLine; }

public Q_SLOTS:
    void setTabSettings(const TabSettings &);
    virtual void format();

Q_SIGNALS:
    void breakpointToggled(int lineNumber, bool set);

protected:

    bool viewportEvent(QEvent *event);
    QChar charAt(int pos) const;

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void timerEvent(QTimerEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

    // Indent a text block based on previous line. Default does nothing
    virtual void indentBlock(QTextDocument *doc, QTextBlock block, QChar typedChar);
    // Indent at cursor. Calls indentBlock for selection or current line.
    virtual void indent(QTextDocument *doc, const QTextCursor &cursor, QChar typedChar);


protected Q_SLOTS:
    virtual void slotUpdateExtraAreaWidth();
    virtual void slotModificationChanged(bool);
    virtual void slotUpdateRequest(const QRect &r, int dy);
    virtual void slotCursorPositionChanged();

public slots:
    void slotToggleBookmark();
    void slotToggleBreakpoint();
    void slotTogglePendingBreakpoint();
    void slotToggleBlockVisible();
    void slotSetExecutionBlock();
private:

    void setBookmark_helper(int blockNumber, bool b = true, bool update = true);
    void setBreakpoint_helper(int blockNumber, bool b = true, bool update = true);
    void setPendingBreakpoint_helper(int blockNumber, bool b = true, bool update = true);

    void indentSelection();
    void handleHomeKey(bool anchor);
    void handleArrowKeys(int key);
    void unIndentSelection();

    void toggleBlockVisible(const QTextBlock &block);
    QRect collapseBox(const QTextBlock &block);

    QTextBlock collapsedBlockAt(const QPoint &pos, QString *text = 0, QRect *box = 0) const;

    // parentheses matcher
private slots:
    void matchParentheses();
private:
    bool m_formatRange;
    QTextCharFormat m_matchFormat;
    QTextCharFormat m_mismatchFormat;
    QTextCharFormat m_rangeFormat;
    QTimer *m_parenthesesMatchingTimer;
    // end parentheses matcher


    QWidget *m_extraArea;
    TabSettings m_tabsettings;
    QIcon executionIcon;
    QIcon bookmarkIcon;
    QIcon breakpointIcon;
    QIcon breakpointPendingIcon;

    int extraAreaSelectionAnchorBlockNumber;
    int extraAreaToggleBreakpointBlockNumber;

    QBasicTimer autoScrollTimer;

    int baseLine;
};


#endif // CPPEDIT_H
