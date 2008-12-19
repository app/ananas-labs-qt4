/****************************************************************************
**
** This file is part of the Qt Designer of the Qt Toolkit.
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

//#include "qdesigner.h"
#include "qdesigner_settings.h"
#include "qdesigner_widgetbox.h"
#include "qdesigner_workbench.h"
#include "qdesigner_propertyeditor.h"
#include "qdesigner_objectinspector.h"

#include <QtCore/QVariant>
#include <QtCore/QDir>

#include <QtGui/QDesktopWidget>
#include <QtGui/QStyle>
#include <QtGui/QApplication>

#include <QtCore/qdebug.h>

QDesignerSettings::QDesignerSettings()
    : QSettings()
{
    m_designerPath = QLatin1String("/.designer");

    QStringList paths = defaultFormTemplatePaths();
    foreach (QString path, paths) {
        if (!QDir::current().exists(path))
            QDir::current().mkpath(path);
    }
}

QDesignerSettings::~QDesignerSettings()
{
}

QStringList QDesignerSettings::formTemplatePaths() const
{
    return value(QLatin1String("FormTemplatePaths"),
                 defaultFormTemplatePaths()).toStringList();
}

void QDesignerSettings::setFormTemplatePaths(const QStringList &paths)
{
    setValue(QLatin1String("FormTemplatePaths"), paths);
}

QString QDesignerSettings::defaultUserWidgetBoxXml() const
{
    return QDir::homePath() + m_designerPath + QLatin1String("/widgetbox.xml");
}

QStringList QDesignerSettings::defaultFormTemplatePaths() const
{
    QStringList paths;

    QString templatePath = QLatin1String("/templates");

    paths.append(QDir::homePath() + m_designerPath + templatePath);
    paths.append(QApplication::applicationDirPath() + templatePath);

    return paths;
}

void QDesignerSettings::saveGeometryFor(const QWidget *w)
{
    Q_ASSERT(w && !w->objectName().isEmpty());
    saveGeometryHelper(w, w->objectName());
}

void QDesignerSettings::setGeometryFor(QWidget *w, const QRect &fallBack) const
{
    Q_ASSERT(w && !w->objectName().isEmpty());
    setGeometryHelper(w, w->objectName(),
                      fallBack.isNull() ? QRect(QPoint(0, 0), w->sizeHint()) : fallBack);
}

void QDesignerSettings::saveGeometryHelper(const QWidget *w, const QString &key)
{
    beginGroup(key);
    QPoint pos = w->pos();
    if (!w->isWindow()) // in workspace
        pos = w->parentWidget()->pos();

    setValue(QLatin1String("screen"), QApplication::desktop()->screenNumber(w));
    setValue(QLatin1String("geometry"), QRect(pos, w->size()));
    setValue(QLatin1String("visible"), w->isVisible());
    setValue(QLatin1String("maximized"), w->isMaximized());
    endGroup();
}

void QDesignerSettings::setGeometryHelper(QWidget *w, const QString &key,
                                          const QRect &fallBack) const
{
//    beginGroup();
    int screen = value(key + QLatin1String("/screen"), 0).toInt();
    QRect g = value(key + QLatin1String("/geometry"), fallBack).toRect();
    QRect screenRect = QApplication::desktop()->availableGeometry(screen);

    // Do geometry in a couple of steps
    // 1) Make sure the rect is within the specified geometry
    // 2) Make sure the bottom right and top left fit on the screen, move them in.
    // 3) Check again and resize.

    if (w->isWindow() && g.intersect(screenRect).isEmpty())
        g = fallBack;

    // Maybe use center?
    if (!screenRect.contains(g.bottomRight())) {
        g.moveRight(qMax(0 + g.width(), qMin(screenRect.right(), g.right())));
        g.moveBottom(qMax(0 + g.height(), qMin(screenRect.bottom(), g.bottom())));
    }

    if (!screenRect.contains(g.topLeft())) {
        g.moveLeft(qMin(screenRect.right() - g.width(), qMax(screenRect.left(), g.left())));
        g.moveTop(qMin(screenRect.bottom() - g.height(), qMax(screenRect.top(), g.top())));
    }

    if (!screenRect.contains(g.bottomRight())) {
        g.setRight(qMin(screenRect.right(), g.right()));
        g.moveBottom(qMin(screenRect.bottom(), g.bottom()));
    }

    if (!screenRect.contains(g.topLeft())) {
        g.setLeft(qMax(0, qMin(screenRect.left(), g.left())));
        g.moveTop(qMax(0, qMin(screenRect.top(), g.top())));
    }


    if (!w->isWindow()) // in workspace
        w->parentWidget()->move(g.topLeft());
    else
        w->move(g.topLeft());

    if (value(key + QLatin1String("/maximized"), false).toBool()) {
        w->setWindowState(w->windowState() | Qt::WindowMaximized);
    } else {
        w->resize(g.size());
    }

    if (value(key + QLatin1String("/visible"), true).toBool())
        w->show();
//    endGroup();
}

QStringList QDesignerSettings::recentFilesList() const
{
    return value(QLatin1String("recentFilesList")).toStringList();
}

void QDesignerSettings::setRecentFilesList(const QStringList &sl)
{
    setValue(QLatin1String("recentFilesList"), sl);
}

void QDesignerSettings::setShowNewFormOnStartup(bool showIt)
{
    setValue(QLatin1String("newFormDialog/ShowOnStartup"), showIt);
}

bool QDesignerSettings::showNewFormOnStartup() const
{
    return value(QLatin1String("newFormDialog/ShowOnStartup"), true).toBool();
}

void QDesignerSettings::setUIMode(int mode)
{
    setValue(QLatin1String("UI/currentMode"), mode);
}

int QDesignerSettings::uiMode() const
{
    return value(QLatin1String("UI/currentMode"), QDesignerWorkbench::TopLevelMode).toInt();
}

QByteArray QDesignerSettings::mainWindowState() const
{
    return value(QLatin1String("MainWindowState")).toByteArray();
}

void QDesignerSettings::setMainWindowState(const QByteArray &mainWindowState)
{
    setValue(QLatin1String("MainWindowState"), mainWindowState);
}

