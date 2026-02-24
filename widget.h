#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"button.h"
#include<QLineEdit>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public slots:
    void digitClicked();
    void pointClicked();
    void backSpaceClicked();
    void clear();
    void clearAll();
    void muliplicativeOperatorClicked();
    void additiveOperatorClicked();
    void unaryOperatorClicked();
    void equalClicked();
    void changeClicked();
private:
    double sumSofar;
    double factorSoFar;
    QString pendingAdditiveOperator;
    QString pendingMultipcativeOperator;
    bool waitingForOperand;
    QLineEdit*display;
    Button*digitButtons[10];
private:
    Button*creatButton(const QString&text,const char*member);
    bool caculate(double rightOperand,const QString &pendingOperator);
    void abortOperator();
    Ui::Widget *ui;
};
#endif // WIDGET_H
