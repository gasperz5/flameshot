// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Gašper Žitko

#include "copytranslatetool.h"
#include "src/utils/screenshotsaver.h"
#include "src/widgets/notificationwidget.h"
#include <QPainter>
#include <QDesktopServices>

CopyTranslateTool::CopyTranslateTool(QObject* parent)
  : AbstractActionTool(parent)
{
    urlString = QStringLiteral("https://translate.google.com/?sl=auto&tl=en&op=images");
}

bool CopyTranslateTool::closeOnButtonPressed() const
{
    return true;
}

QIcon CopyTranslateTool::icon(const QColor& background, bool inEditor) const
{
    Q_UNUSED(inEditor)
    return QIcon(iconPath(background) + "translate.svg");
}
QString CopyTranslateTool::name() const
{
    return tr("Copy and Translate");
}

CaptureTool::Type CopyTranslateTool::type() const
{
    return CaptureTool::TYPE_COPYTRANSLATE;
}

QString CopyTranslateTool::description() const
{
    return tr("Copy selection to clipboard and open google translate in image mode.");
}

CaptureTool* CopyTranslateTool::copy(QObject* parent)
{
    return new CopyTranslateTool(parent);
}

void CopyTranslateTool::pressed(CaptureContext& context)
{
    emit requestAction(REQ_CLEAR_SELECTION);
    context.request.addTask(CaptureRequest::COPY);
    emit requestAction(REQ_CAPTURE_DONE_OK);
    emit requestAction(REQ_CLOSE_GUI);

    CopyTranslateTool::openURL();
}

void CopyTranslateTool::openURL()
{
    bool successful = QDesktopServices::openUrl(urlString);
    if (!successful) {
        m_notification = new NotificationWidget();
        m_notification->showMessage(tr("Unable to open the URL."));
    }
}