#include <QDebug>
#include <QEvent>
#include <QChildEvent>
#include <QFocusEvent>
#include <QPaintEvent>
#include "QVstApplication.h"
#include "QVstHostedWidget.h"

struct QVstHostedWidget::Private
{
    HWND hParentWnd;
    bool reenableParent;
    HWND hPrevFocus;
};

QVstHostedWidget::QVstHostedWidget(HWND hParentWnd, QObject *pParent, Qt::WindowFlags f)
    : QWidget(0, f)
{
    m = new Private;
    if (pParent != nullptr) {
        QObject::setParent(pParent);
    }

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_NativeWindow, true);

    m->hParentWnd = hParentWnd;
    m->reenableParent = false;
    m->hPrevFocus = 0;

    initialize();
}

QVstHostedWidget::~QVstHostedWidget()
{
    delete m;
}

void QVstHostedWidget::setVisible(bool visible)
{
    if (visible) {
        saveFocus();
    }
    QWidget::setVisible(visible);
}

/*
void QVstHostedWidget::childEvent(QChildEvent *pEvent)
{
    QObject *pObj = pEvent->child();
    if (pObj->isWidgetType()) {
        if (pEvent->added()) {
            pObj->installEventFilter(this);
        } else if (pEvent->removed() && m->reenableParent) {
            m->reenableParent = false;
            ::EnableWindow(m->hParentWnd, true);
            pObj->removeEventFilter(this);
        }
    }
    QWidget::childEvent(pEvent);
}

bool QVstHostedWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *pMsg = reinterpret_cast<MSG*>(message);

    qDebug() << __FUNCTION__ << pMsg->message;


    if (pMsg->message == WM_SETFOCUS) {
        Qt::FocusReason reason = Qt::NoFocusReason;
        if (::GetKeyState(VK_SHIFT) < 0) {
            reason = Qt::BacktabFocusReason;
        } else {
            reason = Qt::TabFocusReason;
        }
        QFocusEvent evt(QEvent::FocusIn, reason);
        QVstApplication::sendEvent(this, &evt);
    }
    return QWidget::nativeEvent(eventType, message, result);
}

void QVstHostedWidget::focusInEvent(QFocusEvent *pEvent)
{
    qDebug() << __FUNCTION__;

    QWidget *pCandidate = this;

    switch (pEvent->reason()) {
    case Qt::TabFocusReason:
    case Qt::BacktabFocusReason:
        while (!(pCandidate->focusPolicy() & Qt::TabFocus)) {
            pCandidate = pCandidate->nextInFocusChain();
            if (pCandidate == this) {
                pCandidate = nullptr;
                break;
            }
        }
        if (pCandidate) {
            pCandidate->setFocus(pEvent->reason());
            if (pEvent->reason() == Qt::BacktabFocusReason
                    || pEvent->reason() == Qt::TabFocusReason) {
                pCandidate->setAttribute(Qt::WA_KeyboardFocusChange);
                pCandidate->window()->setAttribute(Qt::WA_KeyboardFocusChange);
            }
            if (pEvent->reason() == Qt::BacktabFocusReason) {
                QWidget::focusNextPrevChild(false);
            }
        }
        break;
    default:
        break;
    }
}

bool QVstHostedWidget::focusNextPrevChild(bool next)
{
    QWidget *pCurFocus = focusWidget();
    if (next) {
        QWidget *pNextFocus = pCurFocus;
        while (true) {
            pNextFocus = pNextFocus->nextInFocusChain();
            if (pNextFocus->isWindow())
                break;
            if (pNextFocus->focusPolicy() & Qt::TabFocus) {
                return QWidget::focusNextPrevChild(true);
            }
        }
    } else {
        if (!pCurFocus->isWindow()) {
            QWidget *pNextFocus = pCurFocus->nextInFocusChain();
            QWidget *pPrevFocus = nullptr;
            QWidget *pTopLevel = nullptr;
            while (pNextFocus != pCurFocus) {
                if (pNextFocus->focusPolicy() & Qt::TabFocus) {
                    pPrevFocus = pNextFocus;
                    pTopLevel = nullptr;
                } else if (pNextFocus->isWindow()) {
                    pTopLevel = pNextFocus;
                }
                pNextFocus = pNextFocus->nextInFocusChain();
            }

            if (pTopLevel != nullptr) {
                return QWidget::focusNextPrevChild(false);
            }
        }
    }

    ::SetFocus(m->hParentWnd);
    return true;
}

bool QVstHostedWidget::eventFilter(QObject *pObject, QEvent *pEvent)
{
    QWidget *pWidget = static_cast<QWidget*>(pObject);

    switch (pEvent->type()) {
    case QEvent::WindowDeactivate:
        if (pWidget->isModal() && pWidget->isHidden()) {
            ::BringWindowToTop(m->hParentWnd);
        }
        break;
    case QEvent::Hide:
        if (m->reenableParent) {
            ::EnableWindow(m->hParentWnd, true);
            m->reenableParent = false;
        }
        resetFocus();
        if (pWidget->testAttribute(Qt::WA_DeleteOnClose) && pWidget->isWindow()) {
            deleteLater();
        }
        break;
    case QEvent::Show:
        if (pWidget->isWindow()) {
            saveFocus();
            hide();
            if (pWidget->isModal() && !m->reenableParent) {
                ::EnableWindow(m->hParentWnd, false);
                m->reenableParent = true;
            }
        }
        break;
    case QEvent::Close:
        ::SetActiveWindow(m->hParentWnd);
        if (pWidget->testAttribute(Qt::WA_DeleteOnClose)) {
            deleteLater();
        }
        break;
    default:
        break;
    }
    return QWidget::eventFilter(pObject, pEvent);
}
*/

void QVstHostedWidget::adjustSize(const QSize &size)
{
    RECT rcParent;
    ::GetWindowRect(m->hParentWnd, &rcParent);

    HWND hWnd = reinterpret_cast<HWND>(winId());
    ::SetWindowPos(hWnd, HWND_TOP, 0, 0, size.width(), size.height(), SWP_SHOWWINDOW);
    setGeometry(rcParent.left, rcParent.top, size.width(), size.height());
    update();
}

void QVstHostedWidget::initialize()
{
    if (m->hParentWnd) {
        HWND hWnd = reinterpret_cast<HWND>(winId());

        RECT rcParent;
        ::GetWindowRect(m->hParentWnd, &rcParent);
        int width = rcParent.right - rcParent.left;
        int height = rcParent.bottom - rcParent.top;

        ::SetWindowLong(hWnd, GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
        ::SetParent(hWnd, m->hParentWnd);
        ::SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
        ::ShowWindow(hWnd, SW_SHOW);

        // Make the widget visible
        setVisible(true);

        QEvent evt(QEvent::EmbeddingControl);
        QVstApplication::sendEvent(this, &evt);

        // Adjust geometry
        adjustSize(QSize(width, height));
    }
}

void QVstHostedWidget::saveFocus()
{
    if (!m->hPrevFocus) {
        m->hPrevFocus = ::GetFocus();
    }
    if (!m->hPrevFocus) {
        m->hPrevFocus = m->hParentWnd;
    }
}

void QVstHostedWidget::resetFocus()
{
    if (m->hPrevFocus) {
        ::SetFocus(m->hPrevFocus);
    } else {
        ::SetFocus(m->hParentWnd);
    }
}
