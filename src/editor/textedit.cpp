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

#include "textedit.h"

#include <QtGui/QTextDocument>
#include <QtGui/QTextBlock>
#include <QtGui/QStyle>
#include <QtGui/QPainter>
#include <QtGui/QShortcut>
#include <QtGui/QScrollBar>
#include <QtGui/QToolTip>
#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QDebug>

#include <limits.h>


int Parenthesis::collapseAtPos(const Parentheses &parentheses)
{
    int result = -1;

    int depth = 0;
    for (int i = 0; i < parentheses.size(); ++i) {
        const Parenthesis &p = parentheses.at(i);
        if (p.chr == QLatin1Char('{')) {
            if (depth == 0)
                result = p.pos;
            ++depth;
        } else if (p.chr == QLatin1Char('}')) {
            if (--depth < 0)
                depth = 0;
            result = -1;
        }
    }
    return result;
}


bool TextBlockUserData::hasClosingCollapse() const
{
    int depth = 0;
    for (int i = 0; i < m_parentheses.size(); ++i) {
        const Parenthesis &p = m_parentheses.at(i);
        if (p.chr == QLatin1Char('{')) {
            ++depth;
        } else if (p.chr == QLatin1Char('}')) {
            if (--depth < 0)
                return true;
        }
    }
    return false;
}

int TextBlockUserData::collapseAtPos() const
{
    return Parenthesis::collapseAtPos(m_parentheses);
}


void TextEditDocumentLayout::setParentheses(const QTextBlock &block, const Parentheses &parentheses)
{
    if (parentheses.isEmpty()) {
        if (TextBlockUserData *userData = testUserData(block))
            userData->clearParentheses();
    } else {
        userData(block)->setParentheses(parentheses);
    }
}

Parentheses TextEditDocumentLayout::parentheses(const QTextBlock &block)
{
    if (TextBlockUserData *userData = testUserData(block))
        return userData->parentheses();
    return Parentheses();
}

bool TextEditDocumentLayout::hasParentheses(const QTextBlock &block)
{
    if (TextBlockUserData *userData = testUserData(block))
        return userData->hasParentheses();
    return false;
}





QRectF TextEditDocumentLayout::blockBoundingRect(const QTextBlock &block) const
{
    QRectF r = QPlainTextDocumentLayout::blockBoundingRect(block);
    return r;
}

class TextEditExtraArea : public QWidget {
    TextEdit *textEdit;
public:
    TextEditExtraArea(TextEdit *edit):QWidget(edit) {
        textEdit = edit;
        setAutoFillBackground(true);
    }
public:

    QSize sizeHint() const {
        return QSize(textEdit->extraAreaWidth(), 0);
    }
protected:
    void paintEvent(QPaintEvent *event){
        textEdit->extraAreaPaintEvent(event);
    }
    void mousePressEvent(QMouseEvent *event){
        textEdit->extraAreaMouseEvent(event);
    }
    void mouseMoveEvent(QMouseEvent *event){
        textEdit->extraAreaMouseEvent(event);
    }
    void mouseReleaseEvent(QMouseEvent *event){
        textEdit->extraAreaMouseEvent(event);
    }

    void wheelEvent(QWheelEvent *event) {
        QCoreApplication::sendEvent(textEdit->viewport(), event);
    }
};



TextEdit::TextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    viewport()->setMouseTracking(true);
    extraAreaSelectionAnchorBlockNumber = extraAreaToggleBreakpointBlockNumber = -1;
    QTextDocument *doc = document();
    doc->setDocumentLayout(new TextEditDocumentLayout(doc));

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(slotUpdateExtraAreaWidth()));
    connect(this, SIGNAL(modificationChanged(bool)), this, SLOT(slotModificationChanged(bool)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));
    m_extraArea = new TextEditExtraArea(this);
    m_extraArea->setMouseTracking(true);
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(slotUpdateRequest(QRect, int)));

    baseLine = 1;

    executionIcon = QIcon(":/images/location.svg");
    bookmarkIcon = QIcon(":/images/bookmark.svg");
    breakpointIcon = QIcon(":/images/breakpoint.svg");
    breakpointPendingIcon = QIcon(":/images/pending.svg");

    // parentheses matcher
    m_formatRange = true;
    m_matchFormat.setForeground(Qt::red);
    m_rangeFormat.setBackground(QColor(0xb4, 0xee, 0xb4));
    m_mismatchFormat.setBackground(Qt::magenta);
    m_parenthesesMatchingTimer = new QTimer(this);
    m_parenthesesMatchingTimer->setSingleShot(true);
    connect(m_parenthesesMatchingTimer, SIGNAL(timeout()), this, SLOT(matchParentheses()));

    slotUpdateExtraAreaWidth();
    slotCursorPositionChanged();
}

TextEdit::~TextEdit()
{
}


bool TextEdit::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        QString text;
        QRect box;
        if (collapsedBlockAt(static_cast<QHelpEvent*>(event)->pos(), &text, &box).isValid()) {
            QToolTip::showText(static_cast<QHelpEvent*>(event)->globalPos(), text, viewport(), box);
        }
        return true;
    }
    return QPlainTextEdit::viewportEvent(event);
}


void TextEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    m_extraArea->setGeometry(
        QStyle::visualRect(layoutDirection(), cr,
                           QRect(cr.left(), cr.top(), extraAreaWidth(), cr.height())));
}

QRect TextEdit::collapseBox(const QTextBlock &block)
{
    QRectF br = blockBoundingGeometry(block).translated(contentOffset());
    int collapseBoxWidth = fontMetrics().lineSpacing();
    return QRect(m_extraArea->width() - collapseBoxWidth + collapseBoxWidth/4,
                 int(br.top()) + collapseBoxWidth/4,
                  2 * (collapseBoxWidth/4) + 1, 2 * (collapseBoxWidth/4) + 1);

}

QTextBlock TextEdit::collapsedBlockAt(const QPoint &pos, QString *text, QRect *box) const {
    QPointF offset(contentOffset());
    QTextBlock block = firstVisibleBlock();
    int top = (int)blockBoundingGeometry(block).translated(offset).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    int viewportHeight = viewport()->height();

    while (block.isValid() && top <= viewportHeight) {
        QTextBlock nextBlock = block.next();
        if (block.isVisible() && bottom >= 0) {
            if (nextBlock.isValid() && !nextBlock.isVisible()) {
                QTextLayout *layout = block.layout();
                QTextLine line = layout->lineAt(layout->lineCount()-1);
                QRectF lineRect = line.naturalTextRect().translated(offset.x(), top);
                lineRect.adjust(0, 0, -1, -1);

                QRectF collapseRect(lineRect.right() + 8, lineRect.top(),
                                    fontMetrics().width(QLatin1String(" {...}; ")),
                                    lineRect.height());
                if (collapseRect.contains(pos)) {
                    QTextBlock result = block;
                    if (box)
                        *box = collapseRect.toAlignedRect();
                    if (text) {
                        *text = QString();
                        block = nextBlock;
                        int lineCount = 15;
                        while (nextBlock.isValid() && !nextBlock.isVisible()) {
                            block = nextBlock;
                            if (lineCount) {
                                --lineCount;
                                if (!text->isEmpty())
                                    *text += QLatin1Char('\n');
                                *text += block.text();
                            }
                            nextBlock = block.next();
                        }
                        if (lineCount == 0)
                            *text += QLatin1String("\n...");
                    }
                    return result;
                } else {
                    block = nextBlock;
                    while (nextBlock.isValid() && !nextBlock.isVisible()) {
                        block = nextBlock;
                        nextBlock = block.next();
                    }
                }
            }
        }

        block = nextBlock;
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
    }
    return QTextBlock();
}

void TextEdit::paintEvent(QPaintEvent *e)
{
    QPlainTextEdit::paintEvent(e);

    QPainter painter(viewport());
    QPointF offset(contentOffset());
    QTextBlock block = firstVisibleBlock();
    int top = (int)blockBoundingGeometry(block).translated(offset).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    while (block.isValid() && top <= e->rect().bottom()) {
        QTextBlock nextBlock = block.next();
        if (block.isVisible() && bottom >= e->rect().top()) {
            if (nextBlock.isValid() && !nextBlock.isVisible()) {
                QTextLayout *layout = block.layout();
                QTextLine line = layout->lineAt(layout->lineCount()-1);
                QRectF lineRect = line.naturalTextRect().translated(offset.x(), top);
                lineRect.adjust(0, 0, -1, -1);

                QRectF collapseRect(lineRect.right() + 8, lineRect.top(),
                                    fontMetrics().width(QLatin1String(" {...}; ")),
                                    lineRect.height());
                painter.drawRect(collapseRect.adjusted(0, 0, 0, -1));


                QString replacement = QLatin1String("...");

                QTextBlock info = block;
                if (block.userData()
                    && static_cast<TextBlockUserData*>(block.userData())->collapse() == TextBlockUserData::CollapseAfter)
                    ;
                else if (block.next().userData()
                         && static_cast<TextBlockUserData*>(block.next().userData())->collapse()
                         == TextBlockUserData::CollapseThis) {
                    replacement.prepend(nextBlock.text().trimmed().left(1));
                    info = nextBlock;
                }

                block = nextBlock;
                while (nextBlock.isValid() && !nextBlock.isVisible()) {
                    block = nextBlock;
                    nextBlock = block.next();
                }
                if (static_cast<TextBlockUserData*>(info.userData())->collapseIncludesClosure()) {
                    QString right = block.text().trimmed();
                    if (right.endsWith(QLatin1Char(';'))) {
                        right.chop(1);
                        right = right.trimmed();
                        replacement.append(right.right(1));
                        replacement.append(QLatin1Char(';'));
                    } else {
                        replacement.append(right.right(1));
                    }
                }
                painter.drawText(collapseRect, Qt::AlignCenter, replacement);
            }
        }

        block = nextBlock;
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
    }


}

void TextEdit::slotUpdateExtraAreaWidth()
{
    if (isLeftToRight())
        setViewportMargins(extraAreaWidth(), 0, 0, 0);
    else
        setViewportMargins(0, 0, extraAreaWidth(), 0);
}


QWidget *TextEdit::extraArea() const {
    return m_extraArea;
}

int TextEdit::extraAreaWidth(int *markWidthPtr) const {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    QFontMetrics fm(fontMetrics());
    int space = fm.width(QLatin1Char('9')) * digits;
    TextEditDocumentLayout *documentLayout = qobject_cast<TextEditDocumentLayout*>(document()->documentLayout());
    Q_ASSERT(documentLayout);
    int markWidth = 0;
    markWidth += fm.lineSpacing();
    if (documentLayout->doubleMarkCount)
        markWidth += fm.lineSpacing() / 3;

    if (markWidthPtr)
        *markWidthPtr = markWidth;

    space += markWidth;


    space += 4;

    space += fm.lineSpacing();

    return space;
}

void TextEdit::slotModificationChanged(bool m)
{
    if (!m) {
        QTextDocument *doc = document();
        TextEditDocumentLayout *documentLayout = qobject_cast<TextEditDocumentLayout*>(doc->documentLayout());
        Q_ASSERT(documentLayout);
        int oldLastSaveRevision = documentLayout->lastSaveRevision;
        documentLayout->lastSaveRevision = doc->revision();
        QTextBlock block = doc->begin();
        while (block.isValid()) {
            if (block.revision() < 0 || block.revision() != oldLastSaveRevision) {
                block.setRevision(-documentLayout->lastSaveRevision - 1);
            } else {
                block.setRevision(documentLayout->lastSaveRevision);
            }
            block = block.next();
        }
        m_extraArea->update();
    }
}

void TextEdit::slotUpdateRequest(const QRect &r, int dy)
{
    if (dy)
        m_extraArea->scroll(0, dy);
    else
        m_extraArea->update(0, r.y(), m_extraArea->width(), r.height());

    if (r.contains(viewport()->rect()))
        slotUpdateExtraAreaWidth();
}


int TextEdit::executionBlockNumber() const
{
    QTextDocument *doc = document();
    TextEditDocumentLayout *documentLayout = qobject_cast<TextEditDocumentLayout*>(doc->documentLayout());
    Q_ASSERT(documentLayout);
    return documentLayout->executionBlockNumber();
}

void TextEdit::setExecutionBlockNumber(int blockNumber)
{
    QTextDocument *doc = document();
    TextEditDocumentLayout *documentLayout = qobject_cast<TextEditDocumentLayout*>(doc->documentLayout());
    Q_ASSERT(documentLayout);
    documentLayout->setExecutionBlockNumber(blockNumber);

    if (true) {
        QTextBlock block = document()->findBlockByNumber(blockNumber);
        if (block.isValid()) {
            QTextCursor cursor = textCursor();
            cursor.setPosition(block.position());
            setTextCursor(cursor);
        }
    }
}

void TextEdit::extraAreaPaintEvent(QPaintEvent *e)
{
    TextEditDocumentLayout *documentLayout = qobject_cast<TextEditDocumentLayout*>(document()->documentLayout());
    Q_ASSERT(documentLayout);

    QPalette pal = palette();
    QPainter painter(m_extraArea);
    QFontMetrics fm(painter.fontMetrics());
    int fmLineSpacing = fm.lineSpacing();

    int markWidth = 0;
    markWidth += fm.lineSpacing();
    if (documentLayout->doubleMarkCount)
        markWidth += fm.lineSpacing() / 3;


    int collapseBoxWidth = fmLineSpacing;
    int extraAreaWidth = m_extraArea->width() - collapseBoxWidth;

    QLinearGradient gradient(QPointF(extraAreaWidth - 10, 0), QPointF(extraAreaWidth, 0));
    gradient.setColorAt(0, pal.color(QPalette::Background));
    gradient.setColorAt(1, pal.color(QPalette::Base));
    painter.fillRect(e->rect(), gradient);

    QLinearGradient gradient2(QPointF(0, 0), QPointF(markWidth, 0));
    gradient2.setColorAt(0, pal.color(QPalette::Dark));
    gradient2.setColorAt(1, pal.color(QPalette::Background));
    painter.fillRect(e->rect().intersected(QRect(0, 0, markWidth, INT_MAX)), gradient2);


    painter.setPen(QPen(pal.color(QPalette::Background), 2));
    painter.drawLine(extraAreaWidth-1, e->rect().top(), extraAreaWidth-1, e->rect().bottom());
    painter.setRenderHint(QPainter::Antialiasing);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    int executionBlock = executionBlockNumber();

    while (block.isValid() && top <= e->rect().bottom()) {
        bool collapseThis = false;
        bool collapseAfter = false;
        bool hasClosingCollapse = false;
        QTextBlock nextBlock = block.next();
        if (block.isVisible() && bottom >= e->rect().top()) {
            QString number = QString::number(blockNumber + baseLine);
            painter.setPen(pal.color(QPalette::Highlight));

            if (TextBlockUserData *userData = static_cast<TextBlockUserData*>(block.userData())) {
                int x = 0;
                int radius = fmLineSpacing - 1;
                if (userData->bookmark()) {
                    QRect r(x, top, radius, radius);
                    bookmarkIcon.paint(&painter, r);
                    x += fmLineSpacing / 3;
                }
                if (userData->breakpoint() || userData->pendingBreakpoint()) {
                    QRect r(x, top, radius, radius);
                    breakpointIcon.paint(&painter, r);
                    if (!userData->breakpoint()) { // pending only
                        breakpointPendingIcon.paint(&painter, r);
                    }
                }

                collapseAfter = (userData->collapse() == TextBlockUserData::CollapseAfter);
                collapseThis = (userData->collapse() == TextBlockUserData::CollapseThis);
                hasClosingCollapse = userData->hasClosingCollapse();
            }


            if (executionBlock == blockNumber) {
                QRect br(0, top, markWidth, fmLineSpacing);
                executionIcon.paint(&painter, br);
            }


            int previousBraceDepth = block.previous().userState();
            if (previousBraceDepth >= 0)
                previousBraceDepth >>= 8;
            else
                previousBraceDepth = 0;
            int braceDepth = block.userState();
            if (!nextBlock.isVisible()) {
                QTextBlock b = nextBlock;
                while (b.isValid() && !b.isVisible())
                    b = b.next();
                braceDepth = b.userState();
            }
            if (braceDepth >= 0)
                braceDepth >>= 8;
            else
                braceDepth = 0;

            QRect box(extraAreaWidth + collapseBoxWidth/4, top + collapseBoxWidth/4,
                      2 * (collapseBoxWidth/4) + 1, 2 * (collapseBoxWidth/4) + 1);
            QPoint boxCenter = box.center();
            painter.save();
            painter.setPen(pal.text().color());
            painter.setRenderHint(QPainter::Antialiasing, false);

            bool collapseNext = (nextBlock.userData() && static_cast<TextBlockUserData*>(nextBlock.userData())->collapse() == TextBlockUserData::CollapseThis);

            if (previousBraceDepth || collapseThis)
                painter.drawLine(boxCenter.x(), top, boxCenter.x(), boxCenter.y());

            if (braceDepth || (collapseNext && nextBlock.isVisible()))
                painter.drawLine(boxCenter.x(), boxCenter.y(), boxCenter.x(), bottom-1);

            if (collapseAfter || collapseNext) {
                    painter.setBrush(pal.background());
                    painter.drawRect(box.adjusted(0, 0, -1, -1));
                    if (!nextBlock.isVisible())
                        painter.drawLine(boxCenter.x(), box.top() + 2, boxCenter.x(), box.bottom() - 2);
                    painter.drawLine(box.left() + 2, boxCenter.y(), box.right() - 2, boxCenter.y());
            } else if (hasClosingCollapse) {
                painter.drawLine(boxCenter.x(), boxCenter.y(), box.right() - 1, boxCenter.y());
            }

            painter.restore();


//             qDebug() << "block" << block.blockNumber() << "revision:" << block.revision() << "lastSaved:" << documentLayout->lastSaveRevision;
            if (block.revision() != documentLayout->lastSaveRevision) {
                painter.save();
                painter.setRenderHint(QPainter::Antialiasing, false);
                if (block.revision() < 0)
                    painter.setPen(QPen(Qt::darkGreen, 2));
                else
                    painter.setPen(QPen(Qt::red, 2));
                painter.drawLine(extraAreaWidth-1, top, extraAreaWidth-1, bottom-1);
                painter.restore();
            }

            painter.drawText(markWidth, top, extraAreaWidth - markWidth - 4, fm.height(), Qt::AlignRight, number);
//            painter.drawText(markWidth + 4, top + fm.ascent() + fm.leading(), number);


        }
        block = nextBlock;
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }

}

void TextEdit::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == autoScrollTimer.timerId()) {
        const QPoint globalPos = QCursor::pos();
        const QPoint pos = m_extraArea->mapFromGlobal(globalPos);
        QRect visible = m_extraArea->rect();
        verticalScrollBar()->triggerAction( pos.y() < visible.center().y() ?
                                            QAbstractSlider::SliderSingleStepSub
                                            : QAbstractSlider::SliderSingleStepAdd);
        QMouseEvent ev(QEvent::MouseMove, pos, globalPos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        extraAreaMouseEvent(&ev);
        int delta = qMax(pos.y() - visible.top(), visible.bottom() - pos.y()) - visible.height();
        if (delta < 7)
            delta = 7;
        int timeout = 4900 / (delta * delta);
        autoScrollTimer.start(timeout, this);

    }
    QPlainTextEdit::timerEvent(e);
}


void TextEdit::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == 0) {
        QTextBlock collapsedBlock = collapsedBlockAt(e->pos());
        viewport()->setCursor(collapsedBlock.isValid() ? Qt::PointingHandCursor : Qt::IBeamCursor);
    } else {
        QPlainTextEdit::mouseMoveEvent(e);
    }
}

void TextEdit::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        QTextBlock collapsedBlock = collapsedBlockAt(e->pos());
        if (collapsedBlock.isValid()) {
            toggleBlockVisible(collapsedBlock);
            viewport()->setCursor(Qt::IBeamCursor);
        }
    }
    QPlainTextEdit::mousePressEvent(e);
}

void TextEdit::extraAreaMouseEvent(QMouseEvent *e)
{
    QTextCursor cursor = cursorForPosition(QPoint(0, e->pos().y()));
    cursor.setPosition(cursor.block().position());

    int markWidth;
    extraAreaWidth(&markWidth);

    if (e->type() == QEvent::MouseMove && e->buttons() == 0) { // mouse tracking
        bool hand = (e->pos().x() <= markWidth
                     || (TextBlockUserData::canCollapse(cursor.block())
                         && collapseBox(cursor.block()).contains(e->pos())));
        if (hand != (m_extraArea->cursor().shape() == Qt::PointingHandCursor))
            m_extraArea->setCursor(hand ? Qt::PointingHandCursor : Qt::ArrowCursor);
        return;
    }


    if (e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonDblClick) {
        if (e->button() == Qt::LeftButton) {
            if (TextBlockUserData::canCollapse(cursor.block())
                && collapseBox(cursor.block()).contains(e->pos())) {
                setTextCursor(cursor);
                toggleBlockVisible(cursor.block());
            } else if (e->pos().x() > markWidth) {
                QTextCursor selection = cursor;
                extraAreaSelectionAnchorBlockNumber = selection.blockNumber();
                selection.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
                setTextCursor(selection);
            } else {
                extraAreaToggleBreakpointBlockNumber = cursor.blockNumber();
            }
        }
    } else if (extraAreaSelectionAnchorBlockNumber >= 0) {
        QTextCursor selection = cursor;
        if (e->type() == QEvent::MouseMove) {
            QTextBlock anchorBlock = document()->findBlockByNumber(extraAreaSelectionAnchorBlockNumber);
            if (extraAreaSelectionAnchorBlockNumber == cursor.blockNumber()) {
                selection.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
            } else {
                selection.setPosition(anchorBlock.position() +
                                      (cursor.blockNumber() <= extraAreaSelectionAnchorBlockNumber ?
                                       anchorBlock.length() - (anchorBlock.next().isValid()?0:1) : 0));
                selection.setPosition(cursor.block().position(), QTextCursor::KeepAnchor);
            }

            if (e->pos().y() >= 0 && e->pos().y() <= m_extraArea->height())
                autoScrollTimer.stop();
            else if (!autoScrollTimer.isActive())
                autoScrollTimer.start(100, this);

        } else {
            autoScrollTimer.stop();
            extraAreaSelectionAnchorBlockNumber = -1;
            return;
        }
        setTextCursor(selection);
    } else if (extraAreaToggleBreakpointBlockNumber >= 0) {
        if (e->type() == QEvent::MouseButtonRelease && e->button() == Qt::LeftButton) {
            if (cursor.blockNumber() == extraAreaToggleBreakpointBlockNumber) {
                bool b = false;
                if (TextBlockUserData *userData = TextEditDocumentLayout::testUserData(cursor.block()))
                    b = userData->breakpoint();
                setBreakpoint_helper(extraAreaToggleBreakpointBlockNumber, !b);
            }
            extraAreaToggleBreakpointBlockNumber = -1;
        }
    }
}

void TextEdit::slotCursorPositionChanged()
{

    QList<QTextEdit::ExtraSelection> extraSelections;

    if (true || !isReadOnly()){ // the line selection

        QTextEdit::ExtraSelection sel;

        QTextCursor cursor = textCursor();
        const QColor fg = palette().color(QPalette::Highlight);
        const QColor bg = palette().color(QPalette::Base);
        QColor col;
        const qreal ratio = 0.15;
        col.setRedF(fg.redF() * ratio + bg.redF() * (1 - ratio));
        col.setGreenF(fg.greenF() * ratio + bg.greenF() * (1 - ratio));
        col.setBlueF(fg.blueF() * ratio + bg.blueF() * (1 - ratio));
        sel.format.setBackground(col);
        sel.format.setProperty(QTextFormat::FullWidthSelection, true);
        sel.cursor = cursor;
        sel.cursor.clearSelection();
        extraSelections.append(sel);

        m_parenthesesMatchingTimer->start(50);
   }

    setExtraSelections(extraSelections);
}

void TextEdit::slotToggleBookmark()
{
    QTextBlock block = textCursor().block();
    bool b = false;
    if (TextBlockUserData *userData = TextEditDocumentLayout::testUserData(block))
        b = userData->bookmark();
    setBookmark_helper(block.blockNumber(), !b);
}

void TextEdit::slotToggleBreakpoint()
{
    QTextBlock block = textCursor().block();
    bool b = false;
    if (TextBlockUserData *userData = TextEditDocumentLayout::testUserData(block))
        b = userData->breakpoint();
    setBreakpoint_helper(block.blockNumber(), !b);
}

void TextEdit::slotTogglePendingBreakpoint()
{
    QTextBlock block = textCursor().block();
    bool b = false;
    if (TextBlockUserData *userData = TextEditDocumentLayout::testUserData(block))
        b = userData->pendingBreakpoint();
    setPendingBreakpoint_helper(block.blockNumber(), !b);
}

void TextEdit::slotSetExecutionBlock()
{
    setExecutionBlockNumber(textCursor().block().blockNumber());
}


void TextBlockUserData::setCollapse(const QTextBlock& block, bool visible)
{
    QTextBlock info = block;
    if (block.userData() && static_cast<TextBlockUserData*>(block.userData())->collapse() == CollapseAfter)
        ;
    else if (block.next().userData()
             && static_cast<TextBlockUserData*>(block.next().userData())->collapse()
             == TextBlockUserData::CollapseThis)
        info = block.next();
    else
        return;
    int pos = static_cast<TextBlockUserData*>(info.userData())->collapseAtPos();
    if (pos < 0)
        return;
    QTextCursor cursor(info);
    cursor.setPosition(cursor.position() + pos);
    if (matchCursorForward(&cursor) != Match) {
        if (visible) {
            // no match, at least unfold!
            QTextBlock b = block.next();
            while (b.isValid() && !b.isVisible()) {
                b.setVisible(true);
                b = b.next();
            }
        }
        return;
    }

    QTextBlock b = block.next();
    while (b < cursor.block()) {
        b.setVisible(visible);
        b = b.next();
    }

    Q_ASSERT(cursor.block() == b);
    int relPos = cursor.position() - b.position();
    QString s = b.text().mid(relPos).trimmed();
    bool collapseIncludesClosure = (s.isEmpty() || s == QLatin1String(";"));
    if (collapseIncludesClosure)
        b.setVisible(visible);
    static_cast<TextBlockUserData*>(info.userData())->setCollapseIncludesClosure(collapseIncludesClosure);
}

void TextEdit::toggleBlockVisible(const QTextBlock &block)
{
    TextEditDocumentLayout *documentLayout = qobject_cast<TextEditDocumentLayout*>(document()->documentLayout());
    Q_ASSERT(documentLayout);

    if (!TextBlockUserData::canCollapse(block))
        return;

    bool visible = block.next().isVisible();
    TextBlockUserData::setCollapse(block, !visible);
    documentLayout->requestUpdate();
}

void TextEdit::slotToggleBlockVisible()
{
    toggleBlockVisible(textCursor().block());
}


QChar TextEdit::charAt(int pos) const
{
    QTextCursor c = textCursor();

    if (pos < 0)
        return QChar();

    c.movePosition(QTextCursor::End);
    if (pos >= c.position())
        return QChar();

    c.setPosition(pos);
    c.setPosition(pos + 1, QTextCursor::KeepAnchor);

    return c.selectedText().at(0);
}


void TextEdit::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if (m_tabsettings.m_autoIndent) {
            QTextCursor cursor = textCursor();
            cursor.insertBlock();
            const QString leftText = cursor.block().text().left(cursor.position() - cursor.block().position());
            if (cursor.hasSelection() || leftText.simplified().isEmpty()) {
                indent(document(), cursor, QChar::Null);
            }
            e->accept();
            setTextCursor(cursor);
            return;
        }
        break;
    case Qt::Key_Tab:
        if (m_tabsettings.m_spacesForTabs) {
            indentSelection();
            e->accept();
            return;
        }
        break;
    case Qt::Key_Backtab:
        if (m_tabsettings.m_indentSelection) {
            unIndentSelection();
            e->accept();
            return;
        }
        break;
    case Qt::Key_Home:
        if (!(e == QKeySequence::MoveToStartOfDocument)) {
            handleHomeKey(e->modifiers() == Qt::ShiftModifier);
            e->accept();
            return;
        }
        break;
    case Qt::Key_Left:
    case Qt::Key_Right:
        if ((e->modifiers() == Qt::NoModifier) && textCursor().hasSelection()) {
            handleArrowKeys(e->key());
            e->accept();
            return;
        }
        break;
    case Qt::Key_BraceLeft:
    case Qt::Key_BraceRight: {
        const QString text = e->text();
        if (!text.isEmpty()) {
            const QChar c = text.at(0);
            if (c == QLatin1Char('}') || c == QLatin1Char('{')) {
                QTextCursor cursor = textCursor();
                cursor.insertText(QString(c));
                const QString leftText = cursor.block().text().left(cursor.position() - 1 - cursor.block().position());
                if (cursor.hasSelection() || leftText.simplified().isEmpty()) {
                    indent(document(), cursor, c);
                }
                TextEditDocumentLayout *documentLayout = qobject_cast<TextEditDocumentLayout*>(document()->documentLayout());
                Q_ASSERT(documentLayout);
                documentLayout->requestUpdate(); // a bit drastic
                e->accept();
                setTextCursor(cursor);
                return;
            }
        }
    } break;

    default:
        break;
    }
    QPlainTextEdit::keyPressEvent(e);
    if (e->key() == Qt::Key_Delete)
        slotCursorPositionChanged(); // parentheses matching
}

void TextEdit::unIndentSelection()
{
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();
    const QString indent(m_tabsettings.m_indentSize, QLatin1Char(' '));

    const int anchor = cursor.anchor();
    const int pos = cursor.position();
    int indents = 0;
    const bool reversed = (anchor > pos);
    int startPos = (reversed ? pos : anchor);
    int endPos = (reversed ? anchor : pos);
    const QTextBlock startBlock = document()->findBlock(reversed ? pos : anchor);
    QTextBlock lastBlock = document()->findBlock(reversed ? anchor : pos);
    if ((lastBlock.position() == pos || lastBlock.position() == anchor) && startBlock != lastBlock)
        lastBlock = lastBlock.previous();

    QTextBlock curBlock = startBlock;
    while(curBlock.previous() != lastBlock) {
        if (curBlock.text().startsWith(indent)) {
            cursor.setPosition(curBlock.position());
            cursor.setPosition(curBlock.position() + m_tabsettings.m_indentSize, QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            ++indents;
        }
        curBlock = curBlock.next();
    }

    startPos = startBlock.position();
    endPos -= m_tabsettings.m_indentSize * (indents);
    if (endPos < startPos)
        endPos = startPos;
    cursor.setPosition(reversed ? endPos : startPos);
    cursor.setPosition(reversed ? startPos : endPos, QTextCursor::KeepAnchor);

    cursor.endEditBlock();
    setTextCursor(cursor);
}

void TextEdit::indentSelection()
{
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();

    const QString indent(m_tabsettings.m_indentSize, QLatin1Char(' '));
    if (cursor.hasSelection() && m_tabsettings.m_indentSelection) {
        int anchor = cursor.anchor();
        int pos = cursor.position();
        int indents = 0;
        const bool reversed = (anchor > pos);
        const QTextBlock startBlock = document()->findBlock(reversed ? pos : anchor);
        QTextBlock lastBlock = document()->findBlock(reversed ? anchor : pos);
        if ((lastBlock.position() == pos || lastBlock.position() == anchor) && startBlock != lastBlock)
            lastBlock = lastBlock.previous();

        QTextBlock curBlock = startBlock;
        while(curBlock.previous() != lastBlock) {
            cursor.setPosition(curBlock.position());
            cursor.insertText(indent);
            curBlock = curBlock.next();
            ++indents;
        }

        if (reversed) {
            pos = startBlock.position();
            anchor += m_tabsettings.m_indentSize * (indents);
        } else {
            anchor = startBlock.position();
            pos += m_tabsettings.m_indentSize * (indents);
        }
        cursor.setPosition(anchor);
        cursor.setPosition(pos, QTextCursor::KeepAnchor);
    } else {
        cursor.insertText(indent);
    }
    cursor.endEditBlock();
    setTextCursor(cursor);
}

void TextEdit::handleHomeKey(bool anchor)
{
    QTextCursor cursor = textCursor();
    QTextCursor::MoveMode mode = QTextCursor::MoveAnchor;

    if (anchor)
        mode = QTextCursor::KeepAnchor;

    const int initpos = cursor.position();
    int pos = cursor.block().position();

    while (charAt(pos).category() == QChar::Separator_Space) {
        ++pos;
    }

    // Go to the start of the block when we're already at the start of the text
    if (pos == initpos)
        pos = cursor.block().position();

    cursor.setPosition(pos, mode);
    setTextCursor(cursor);
}

void TextEdit::handleArrowKeys(int key)
{
    QTextCursor cursor = textCursor();
    if (key == Qt::Key_Left)
        cursor.setPosition(cursor.selectionStart());
    else if (key == Qt::Key_Right)
        cursor.setPosition(cursor.selectionEnd());
    setTextCursor(cursor);
}


void TextEdit::format()
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
        return;

    const int start = cursor.selectionStart();
    int end = cursor.selectionEnd();

    const QTextBlock endblock = document()->findBlock(end - 1);
    indent(document(), cursor, QChar::Null);

    cursor.setPosition(start);
    end = endblock.position() + endblock.length();
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, end-start);
    setTextCursor(cursor);
}

void TextEdit::setTabSettings(const TabSettings &ts)
{
     m_tabsettings = ts;
}

void TextEdit::indentBlock(QTextDocument *, QTextBlock, QChar)
{
}

void TextEdit::indent(QTextDocument *doc, const QTextCursor &cursor, QChar typedChar)
{
    if (cursor.hasSelection()) {
        QTextBlock block = doc->findBlock(cursor.selectionStart());
        const QTextBlock end = doc->findBlock(cursor.selectionEnd()).next();
        do {
            indentBlock(doc, block, typedChar);
            block = block.next();
        } while (block.isValid() && block != end);
    } else {
        indentBlock(doc, cursor.block(), typedChar);
    }
}


QList<int> TextEdit::bookmarks() const
{
    QList<int> list;
    QTextBlock block = document()->begin();
    int blockNumber = 0;
    while (block.isValid()) {
        if (TextBlockUserData *userData = TextEditDocumentLayout::testUserData(block))
            if (userData->bookmark())
                list += blockNumber;
        block = block.next();
        ++blockNumber;
    }
    return list;
}

QList<int> TextEdit::breakpoints() const
{
    QList<int> list;
    QTextBlock block = document()->begin();
    int blockNumber = 0;
    while (block.isValid()) {
        if (TextBlockUserData *userData = TextEditDocumentLayout::testUserData(block))
            if (userData->breakpoint())
                list += blockNumber;
        block = block.next();
        ++blockNumber;
    }
    return list;
}

void TextEdit::setBookmark_helper(int blockNumber, bool b, bool update)
{
    QTextDocument *doc = document();
    TextEditDocumentLayout *documentLayout = qobject_cast<TextEditDocumentLayout*>(doc->documentLayout());
    Q_ASSERT(documentLayout);
    QTextBlock block = doc->findBlockByNumber(blockNumber);
    if (block.isValid()) {
        TextBlockUserData *userData = TextEditDocumentLayout::testUserData(block);
        bool wasSet = userData && userData->bookmark();
        if (b != wasSet) {
            if (!userData)
                userData = TextEditDocumentLayout::userData(block);
            userData->setBookmark(b);
            int diff = b ? 1 : -1;
            documentLayout->markCount += diff;
            if (userData->breakpoint())
                documentLayout->doubleMarkCount += diff;
        }
    }
    if (update)
        documentLayout->requestUpdate();
}

void TextEdit::setBreakpoint_helper(int blockNumber, bool b, bool update)
{
    QTextDocument *doc = document();
    TextEditDocumentLayout *documentLayout = qobject_cast<TextEditDocumentLayout*>(doc->documentLayout());
    Q_ASSERT(documentLayout);
    QTextBlock block = doc->findBlockByNumber(blockNumber);

    if (block.isValid()) {
        TextBlockUserData *userData = TextEditDocumentLayout::testUserData(block);
        bool wasSet = userData && userData->breakpoint();
        if (b != wasSet) {
            if (!userData)
                userData = TextEditDocumentLayout::userData(block);
            setPendingBreakpoint_helper(blockNumber, false, false);
            userData->setBreakpoint(b);
            int diff = b ? 1 : -1;
            documentLayout->markCount += diff;
            if (userData->bookmark())
                documentLayout->doubleMarkCount += diff;
            emit breakpointToggled(blockNumber + baseLine, b);
        }
    }
    if (update)
        documentLayout->requestUpdate();
}

void TextEdit::setPendingBreakpoint_helper(int blockNumber, bool b, bool update)
{
    QTextDocument *doc = document();
    TextEditDocumentLayout *documentLayout = qobject_cast<TextEditDocumentLayout*>(doc->documentLayout());
    Q_ASSERT(documentLayout);
    QTextBlock block = doc->findBlockByNumber(blockNumber);

    if (block.isValid()) {
        TextBlockUserData *userData = TextEditDocumentLayout::testUserData(block);
        bool wasSet = userData && userData->pendingBreakpoint();
        if (b != wasSet) {
            if (!userData)
                userData = TextEditDocumentLayout::userData(block);
            setBreakpoint_helper(blockNumber, false, false);
            userData->setPendingBreakpoint(b);
            int diff = b ? 1 : -1;
            documentLayout->markCount += diff;
            if (userData->bookmark())
                documentLayout->doubleMarkCount += diff;
        }
    }
    if (update)
        documentLayout->requestUpdate();
}

void TextEdit::setBookmarks(const QList<int> &blockNumbers)
{
    Q_ASSERT(qobject_cast<TextEditDocumentLayout*>(document()->documentLayout()));
    QTextBlock block = document()->begin();
    while (block.isValid()) {
        if (TextBlockUserData *userData = TextEditDocumentLayout::testUserData(block))
            userData->setBookmark(false);
        block = block.next();
    }
    foreach (int blockNumber, blockNumbers) {
        setBookmark_helper(blockNumber, true, blockNumber == blockNumbers.last());
    }
}


void TextEdit::setBreakpoints(const QList<int> &blockNumbers)
{
    Q_ASSERT(qobject_cast<TextEditDocumentLayout*>(document()->documentLayout()));
    QTextBlock block = document()->begin();
    while (block.isValid()) {
        if (TextBlockUserData *userData = TextEditDocumentLayout::testUserData(block))
            userData->setBreakpoint(false);
        block = block.next();
    }
    foreach (int blockNumber, blockNumbers) {
        setBreakpoint_helper(blockNumber, true, blockNumber == blockNumbers.last());
    }
}


void TextEdit::markBlocksAsChanged(QList<int> blockNumbers) {
    QTextBlock block = document()->begin();
    while (block.isValid()) {
        if (block.revision() < 0)
            block.setRevision(-block.revision() - 1);
        block = block.next();
    }
    foreach(int blockNumber, blockNumbers) {
        QTextBlock block = document()->findBlockByNumber(blockNumber);
        if (block.isValid())
            block.setRevision(-block.revision() - 1);
    }
}



TextBlockUserData::MatchType TextBlockUserData::checkOpenParenthesis(QTextCursor *cursor, QChar c)
{
    if (!TextEditDocumentLayout::hasParentheses(cursor->block()))
        return NoMatch;

    Parentheses parenList = TextEditDocumentLayout::parentheses(cursor->block());
    Parenthesis openParen, closedParen;
    QTextBlock closedParenParag = cursor->block();

    const int cursorPos = cursor->position() - closedParenParag.position();
    int i = 0;
    int ignore = 0;
    bool foundOpen = false;
    for (;;) {
        if (!foundOpen) {
            if (i >= parenList.count())
                return NoMatch;
            openParen = parenList.at(i);
            if (openParen.pos != cursorPos) {
                ++i;
                continue;
            } else {
                foundOpen = true;
                ++i;
            }
        }

        if (i >= parenList.count()) {
            for (;;) {
                closedParenParag = closedParenParag.next();
                if (!closedParenParag.isValid())
                    return NoMatch;
                if (TextEditDocumentLayout::hasParentheses(closedParenParag)) {
                    parenList = TextEditDocumentLayout::parentheses(closedParenParag);
                    break;
                }
            }
            i = 0;
        }

        closedParen = parenList.at(i);
        if (closedParen.type == Parenthesis::Opened) {
            ignore++;
            ++i;
            continue;
        } else {
            if (ignore > 0) {
                ignore--;
                ++i;
                continue;
            }

            cursor->clearSelection();
            cursor->setPosition(closedParenParag.position() + closedParen.pos + 1, QTextCursor::KeepAnchor);

            if (c == QLatin1Char('{') && closedParen.chr != QLatin1Char('}')
                || c == QLatin1Char('(') && closedParen.chr != QLatin1Char(')')
                || c == QLatin1Char('[') && closedParen.chr != QLatin1Char(']')
               )
                return Mismatch;

            return Match;
        }
    }
}

TextBlockUserData::MatchType TextBlockUserData::checkClosedParenthesis(QTextCursor *cursor, QChar c)
{

    if (!TextEditDocumentLayout::hasParentheses(cursor->block()))
        return NoMatch;

    Parentheses parenList = TextEditDocumentLayout::parentheses(cursor->block());
    Parenthesis openParen, closedParen;
    QTextBlock openParenParag = cursor->block();

    const int cursorPos = cursor->position() - openParenParag.position();
    int i = parenList.count() - 1;
    int ignore = 0;
    bool foundClosed = false;
    for (;;) {
        if (!foundClosed) {
            if (i < 0)
                return NoMatch;
            closedParen = parenList.at(i);
            if (closedParen.pos != cursorPos - 1) {
                --i;
                continue;
            } else {
                foundClosed = true;
                --i;
            }
        }

        if (i < 0) {
            for (;;) {
                openParenParag = openParenParag.previous();
                if (!openParenParag.isValid())
                    return NoMatch;

                if (TextEditDocumentLayout::hasParentheses(openParenParag)) {
                    parenList = TextEditDocumentLayout::parentheses(openParenParag);
                    break;
                }
            }
            i = parenList.count() - 1;
        }

        openParen = parenList.at(i);
        if (openParen.type == Parenthesis::Closed) {
            ignore++;
            --i;
            continue;
        } else {
            if (ignore > 0) {
                ignore--;
                --i;
                continue;
            }

            cursor->clearSelection();
            cursor->setPosition(openParenParag.position() + openParen.pos, QTextCursor::KeepAnchor);

            if ( c == '}' && openParen.chr != '{' ||
                    c == ')' && openParen.chr != '(' ||
                    c == ']' && openParen.chr != '[' )
                return Mismatch;

            return Match;
        }
    }
}
TextBlockUserData::MatchType TextBlockUserData::matchCursorBackward(QTextCursor *cursor)
{
    cursor->clearSelection();
    const QTextBlock block = cursor->block();

    if (!TextEditDocumentLayout::hasParentheses(block))
        return NoMatch;

    const int relPos = cursor->position() - block.position();

    Parentheses parentheses = TextEditDocumentLayout::parentheses(block);
    const Parentheses::const_iterator cend = parentheses.constEnd();
    for (Parentheses::const_iterator it = parentheses.constBegin();it != cend; ++it) {
        const Parenthesis &paren = *it;
        if (paren.pos == relPos - 1
            && paren.type == Parenthesis::Closed) {
            return checkClosedParenthesis(cursor, paren.chr);
        }
    }
    return NoMatch;
}

TextBlockUserData::MatchType TextBlockUserData::matchCursorForward(QTextCursor *cursor)
{
    cursor->clearSelection();
    const QTextBlock block = cursor->block();

    if (!TextEditDocumentLayout::hasParentheses(block))
        return NoMatch;

    const int relPos = cursor->position() - block.position();

    Parentheses parentheses = TextEditDocumentLayout::parentheses(block);
    const Parentheses::const_iterator cend = parentheses.constEnd();
    for (Parentheses::const_iterator it = parentheses.constBegin();it != cend; ++it) {
        const Parenthesis &paren = *it;
        if (paren.pos == relPos
            && paren.type == Parenthesis::Opened) {
            return checkOpenParenthesis(cursor, paren.chr);
        }
    }
    return NoMatch;
}

void TextEdit::matchParentheses()
{
    if (isReadOnly())
        return;

    QTextCursor backwardMatch = textCursor();
    QTextCursor forwardMatch = textCursor();
    const TextBlockUserData::MatchType backwardMatchType = TextBlockUserData::matchCursorBackward(&backwardMatch);
    const TextBlockUserData::MatchType forwardMatchType = TextBlockUserData::matchCursorForward(&forwardMatch);

    if (backwardMatchType == TextBlockUserData::NoMatch && forwardMatchType == TextBlockUserData::NoMatch)
        return;

    QList<QTextEdit::ExtraSelection> extraSelections = this->extraSelections();

    if (backwardMatch.hasSelection()) {
        QTextEdit::ExtraSelection sel;
        if (backwardMatchType == TextBlockUserData::Mismatch) {
            sel.cursor = backwardMatch;
            sel.format = m_mismatchFormat;
        } else {

            if (m_formatRange) {
                sel.cursor = backwardMatch;
                sel.format = m_rangeFormat;
                extraSelections.append(sel);
            }

            sel.cursor = backwardMatch;
            sel.format = m_matchFormat;

            sel.cursor.setPosition(backwardMatch.selectionStart());
            sel.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            extraSelections.append(sel);

            sel.cursor.setPosition(backwardMatch.selectionEnd());
            sel.cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        }
        extraSelections.append(sel);
    }

    if (forwardMatch.hasSelection()) {
        QTextEdit::ExtraSelection sel;
        if (forwardMatchType == TextBlockUserData::Mismatch) {
            sel.cursor = forwardMatch;
            sel.format = m_mismatchFormat;
        } else {

            if (m_formatRange) {
                sel.cursor = forwardMatch;
                sel.format = m_rangeFormat;
                extraSelections.append(sel);
            }

            sel.cursor = forwardMatch;
            sel.format = m_matchFormat;

            sel.cursor.setPosition(forwardMatch.selectionStart());
            sel.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            extraSelections.append(sel);

            sel.cursor.setPosition(forwardMatch.selectionEnd());
            sel.cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        }
        extraSelections.append(sel);
    }
    setExtraSelections(extraSelections);
}

