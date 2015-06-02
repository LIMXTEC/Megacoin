#include "dialog_move_handler.h"
#include <QEvent>
#include <QMouseEvent>
#include <QWidget>

DialogMoveHandler::DialogMoveHandler(QWidget *target)
    : QObject(target)
    , _moving(false)
    , _lastPosition(0, 0)
{
}

bool DialogMoveHandler::eventFilter(QObject *obj, QEvent *event)
{
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    switch (event->type())
    {
    case QEvent::MouseButtonPress:
        if ( mouseEvent->button() == Qt::LeftButton)
        {
            _moving = true;
            _lastPosition = mouseEvent->globalPos();
            return true;
        }
        break;
    case QEvent::MouseMove:
        if (mouseEvent->buttons().testFlag(Qt::LeftButton) && _moving)
        {
            QWidget* target = qobject_cast<QWidget*>(parent());

            if (target)
            {
                target->move(target->pos() + (mouseEvent->globalPos() - _lastPosition));
                _lastPosition = mouseEvent->globalPos();
                return true;
            }
        }
        break;
    case QEvent::MouseButtonRelease:
        if (mouseEvent->button() == Qt::LeftButton)
        {
            _moving = false;
            return true;
        }
        break;
    default:
        break;
    }
    return QObject::eventFilter(obj, event);
}
