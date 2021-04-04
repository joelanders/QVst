#include <QDebug>
#include <QEvent>
#include <QChildEvent>
#include <QFocusEvent>
#include <QPaintEvent>
#include "QVstApplication.h"
#include "QVstHostedWidget.h"

QVstHostedWidget::QVstHostedWidget(QObject *pParent, Qt::WindowFlags f)
    : QWidget(0, f)
{
    if (pParent != nullptr) {
        QObject::setParent(pParent);
    }

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_NativeWindow, true);

    initialize();
}

QVstHostedWidget::~QVstHostedWidget()
{
}

void QVstHostedWidget::setVisible(bool visible)
{
    if (visible) {
        saveFocus();
    }
    QWidget::setVisible(visible);
}

void QVstHostedWidget::adjustSize(const QSize &size)
{
}

void QVstHostedWidget::initialize()
{
}

void QVstHostedWidget::saveFocus()
{
}

void QVstHostedWidget::resetFocus()
{
}
