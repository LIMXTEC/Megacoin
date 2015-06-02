#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>

namespace Ui {
class Notification;
}

class Notification : public QWidget
{
    Q_OBJECT

public:
    explicit Notification(QWidget *parent = 0);
    ~Notification();

    void setText(const QString& text);
    void setClosable(bool isClosable);

protected:
    void changeEvent(QEvent *e);

private Q_SLOTS:
    void removeNotificationRequested();

private:
    Ui::Notification *_ui;
    bool _isClosable;
};

#endif // NOTIFICATION_H
