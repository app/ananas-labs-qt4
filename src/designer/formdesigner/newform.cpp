/****************************************************************************
**
** Copyright (C) 1992-2006 Trolltech ASA. All rights reserved.
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

#include "newform.h"
//#include "qdesigner.h"
#include "qdesigner_workbench.h"
#include "qdesigner_actions.h"
#include "qdesigner_formwindow.h"
#include "qdesigner_settings.h"

#include "private/qdesigner_formbuilder_p.h"
#include "private/sheet_delegate_p.h"

#include <QtDesigner/abstractformwindow.h>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtGui/QHeaderView>
#include <QtGui/QPainter>

#include <QtCore/qdebug.h>

enum NewForm_CustomRole
{
    TemplateNameRole = Qt::UserRole + 100
};

NewForm::NewForm(QDesignerWorkbench *workbench, QWidget *parentWidget)
    : QDialog(parentWidget,
#ifdef Q_WS_MAC
            Qt::Tool |
#endif
            Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
      m_workbench(workbench)
{
    ui.setupUi(this);
    ui.treeWidget->setItemDelegate(new qdesigner_internal::SheetDelegate(ui.treeWidget, this));
    ui.treeWidget->header()->hide();
    ui.treeWidget->header()->setStretchLastSection(true);
    ui.lblPreview->setBackgroundRole(QPalette::Base);
    ui.chkShowOnStartup->setChecked(QDesignerSettings().showNewFormOnStartup());

    loadFrom(QLatin1String(":/trolltech/designer/templates/forms"), true);

    QDesignerSettings settings;
    foreach(QString path, settings.formTemplatePaths())
        loadFrom(path, false);
}

NewForm::~NewForm()
{
    QDesignerSettings().setShowNewFormOnStartup(ui.chkShowOnStartup->isChecked());
}

void NewForm::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    if (current && current->parent()) {
        QIcon icon = formPreviewIcon(current->data(0, TemplateNameRole).toString());
        if (icon.isNull()) {
            ui.createButton->setEnabled(false);
            ui.lblPreview->setText(tr("Error loading form"));
        } else {
            ui.createButton->setEnabled(true);
            ui.createButton->setDefault(true);
            ui.lblPreview->setPixmap(icon.pixmap(QSize(256, 256)));
        }
    } else {
        ui.createButton->setEnabled(false);
        ui.lblPreview->setText(tr("Choose a template for a preview"));
    }
}

void NewForm::on_treeWidget_itemActivated(QTreeWidgetItem *item)
{
    if (item->data(0, TemplateNameRole).isValid())
        ui.createButton->animateClick(0);
}

void NewForm::on_createButton_clicked()
{
    if (QTreeWidgetItem *item = ui.treeWidget->currentItem()) {
        close();

        int maxUntitled = 0;
        int totalWindows = m_workbench->formWindowCount();
        // This will cause some problems with i18n, but for now I need the string to be "static"
        QRegExp rx(QLatin1String("untitled( (\\d+))?"));
        for (int i = 0; i < totalWindows; ++i) {
            QString title = m_workbench->formWindow(i)->windowTitle();
            title = title.replace(QLatin1String("[*]"), QLatin1String(""));
            if (rx.indexIn(title) != 1) {
                if (maxUntitled == 0)
                    ++maxUntitled;
                if (rx.numCaptures() > 1)
                    maxUntitled = qMax(rx.cap(2).toInt(), maxUntitled);
            }
        }

        QDesignerFormWindow *formWindow = workbench()->createFormWindow();
        if (QDesignerFormWindowInterface *editor = formWindow->editor()) {
            QString formTemplateName = item->data(0, TemplateNameRole).toString();
            QFile f(formTemplateName);
            if (f.open(QFile::ReadOnly)) {
                editor->setContents(&f);
                f.close();
            } else {
                editor->setContents(QString());
            }

            if (QWidget *container = editor->mainContainer())
                formWindow->resize(container->size());
        }
        QString newTitle = QLatin1String("untitled");
        if (maxUntitled)
            newTitle += QLatin1String(" ") + QString::number(maxUntitled + 1);

        newTitle.append(QLatin1String("[*]"));
        formWindow->setWindowTitle(newTitle);
        formWindow->editor()->setFileName("");
        formWindow->show();
    }
}

void NewForm::on_closeButton_clicked()
{
    close();
}

QDesignerWorkbench *NewForm::workbench() const
{
    return m_workbench;
}

QIcon NewForm::formPreviewIcon(const QString &fileName)
{
    QIcon result;

    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        qdesigner_internal::QDesignerFormBuilder formBuilder(workbench()->core(),qdesigner_internal::QDesignerFormBuilder::DisableScripts);

        QWidget *fake = new QWidget(0);
        fake->setAttribute(Qt::WA_WState_Visible);

        if (QWidget *widget = formBuilder.load(&f, fake)) {
            widget->setParent(fake, 0);
            widget->show();
            f.close();

            QPixmap pix = QPixmap::grabWidget(widget);
            QImage image = pix.toImage();
            image = image.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            result = QPixmap::fromImage(image);
        }

        fake->deleteLater();
    }

    return result;
}

void NewForm::loadFrom(const QString &path, bool resourceFile)
{
    QDir dir(path);

    if (!dir.exists())
        return;

    // Iterate through the directory and add the templates
    QFileInfoList list = dir.entryInfoList(QStringList() << "*.ui", QDir::Files);

    if (list.isEmpty())
        return;

    QChar separator = resourceFile ? QChar(QLatin1Char('/'))
                                   : QChar(QDir::separator());
    QTreeWidgetItem *root = new QTreeWidgetItem(ui.treeWidget);
    // Try to get something that is easy to read.
    QString visiblePath = path;
    int index = visiblePath.lastIndexOf(separator);
    if (index != -1) {
        // try to find a second slash, just to be a bit better.
        int index2 = visiblePath.lastIndexOf(separator, index - 1);
        if (index2 != -1)
            index = index2;
        visiblePath = visiblePath.mid(index + 1);
        visiblePath = QDir::convertSeparators(visiblePath);
    }

    root->setText(0, visiblePath.replace(QLatin1String("_"), QLatin1String(" ")));
    root->setToolTip(0, path);

    foreach(QFileInfo fi, list) {
        if (!fi.isFile())
            continue;

        QTreeWidgetItem *item = new QTreeWidgetItem(root);
        item->setText(0, fi.baseName().replace(QLatin1String("_"), QLatin1String(" ")));
        item->setData(0, TemplateNameRole, fi.absoluteFilePath());

        QTreeWidgetItem *i = ui.treeWidget->currentItem();
        if (i == 0) {
            ui.treeWidget->setCurrentItem(item);
            ui.treeWidget->setItemSelected(item, true);
        }
    }
    ui.treeWidget->setItemExpanded(root, true);
}

void NewForm::on_openButton_clicked()
{
    hide();
    if (m_workbench->actionManager()->openForm())
        close();
    else
        show();
}

void NewForm::on_treeWidget_itemPressed(QTreeWidgetItem *item)
{
    if (item && !item->parent())
        ui.treeWidget->setItemExpanded(item, !ui.treeWidget->isItemExpanded(item));
}
