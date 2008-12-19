/****************************************************************************
**
** Copyright (C) 1992-2007 Trolltech ASA. All rights reserved.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of Qt Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef QDESIGNER_FORMBUILDER_H
#define QDESIGNER_FORMBUILDER_H

#include "shared_global_p.h"

#include <private/formscriptrunner_p.h>
#include <QtDesigner/formbuilder.h>

#include <QtCore/QMap>
#include <QtCore/QSet>

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;

namespace qdesigner_internal {

// Form builder used for previewing forms
class QDESIGNER_SHARED_EXPORT QDesignerFormBuilder: public QFormBuilder
{
public:
    enum Mode {
        // Use container extension to populate containers. Disable scripts.
        DisableScripts,
        // Use container extension to populate containers as well as scripts
        UseScriptAndContainerExtension,
        // Experimental: Use scripts to populate the container
        UseScriptForContainerExtension
    };

    QDesignerFormBuilder(QDesignerFormEditorInterface *core, Mode mode);

    QWidget *createWidgetFromContents(const QString &contents, QWidget *parentWidget = 0);

    virtual QWidget *createWidget(DomWidget *ui_widget, QWidget *parentWidget = 0)
    { return QFormBuilder::create(ui_widget, parentWidget); }

    inline QDesignerFormEditorInterface *core() const
    { return m_core; }

    typedef QFormScriptRunner::Errors ScriptErrors;
    // Create a preview widget (for integrations) or return 0. The widget has to be embedded into a main window.
    // Experimental, depending on script support.
    static QWidget *createPreview(const QDesignerFormWindowInterface *fw, const QString &styleName /* ="" */,
                                  ScriptErrors *scriptErrors, QString *errorMessage);
    // Convenience that pops up message boxes in case of failures.
    static QWidget *createPreview(const QDesignerFormWindowInterface *fw, const QString &styleName = QString());

protected:
    using QFormBuilder::createDom;
    using QFormBuilder::create;

    virtual QWidget *create(DomUI *ui, QWidget *parentWidget);
    virtual DomWidget *createDom(QWidget *widget, DomWidget *ui_parentWidget, bool recursive = true);
    virtual QWidget *create(DomWidget *ui_widget, QWidget *parentWidget);
    virtual QLayout *create(DomLayout *ui_layout, QLayout *layout, QWidget *parentWidget);

    virtual QWidget *createWidget(const QString &widgetName, QWidget *parentWidget, const QString &name);
    virtual bool addItem(DomWidget *ui_widget, QWidget *widget, QWidget *parentWidget);
    virtual bool addItem(DomLayoutItem *ui_item, QLayoutItem *item, QLayout *layout);

    virtual QIcon nameToIcon(const QString &filePath, const QString &qrcPath);
    virtual QPixmap nameToPixmap(const QString &filePath, const QString &qrcPath);

    virtual void applyProperties(QObject *o, const QList<DomProperty*> &properties);

    virtual void loadExtraInfo(DomWidget *ui_widget, QWidget *widget, QWidget *parentWidget);

private:
    bool addItemContainerExtension(QWidget *widget, QWidget *parentWidget);
    QDesignerFormEditorInterface *m_core;
    const Mode m_mode;
    
    typedef QSet<QWidget *> WidgetSet;
    WidgetSet m_customWidgetsWithScript;
};

} // namespace qdesigner_internal

#endif // QDESIGNER_FORMBUILDER_H
