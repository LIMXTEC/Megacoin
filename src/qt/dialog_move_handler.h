#ifndef DIALOG_MOVE_HANDLER_H
#define DIALOG_MOVE_HANDLER_H

#include <QObject>
#include <QPoint>

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QWidget)
QT_END_NAMESPACE

class DialogMoveHandler : public QObject
{
    Q_OBJECT
public:
    explicit DialogMoveHandler(QWidget* target);
protected:
     bool eventFilter(QObject *obj, QEvent *event);
private:

     bool _moving;
     QPoint _lastPosition;
};

#endif // DIALOG_MOVE_HANDLER_H
