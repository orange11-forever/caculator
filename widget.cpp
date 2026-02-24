#include "widget.h"
#include "./ui_widget.h"
#include<QFont>
#include<QGridLayout>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    display = new QLineEdit("0");
    display->setReadOnly(true);//只读
    display->setAlignment(Qt::AlignRight);//右对齐
    display->setMaxLength(15);//最大长度15
    QFont font=display->font();
    font.setPointSize(font.pointSize()+8);
    display->setFont(font);
    for(int i=0;i<10;++i){
        digitButtons[i]=creatButton(QString::number(i),SLOT(digitClicked()));
    }
    Button*pointBtn=creatButton(".",SLOT(pointClicked()));
    Button*backspaceBtn=creatButton("BackSpace",SLOT(backSpaceClicked()));
    Button*clearBtn=creatButton("clear",SLOT(clear()));
    Button*clearAlBtn=creatButton("clear all",SLOT(clearAll()));
    Button*divBtn=creatButton("\303\267",SLOT(muliplicativeOperatorClicked()));
    Button*timesBtn=creatButton("\303\227",SLOT(muliplicativeOperatorClicked()));
    Button*minusBtn=creatButton("-",SLOT(additiveOperatorClicked()));
    Button*plusBtn=creatButton("+",SLOT(additiveOperatorClicked()));
    Button*sqrtBtn=creatButton("Sqrt",SLOT(unaryOperatorClicked()));
    Button*powerBtn=creatButton("x\302\262",SLOT(unaryOperatorClicked()));
    Button*reciprocalBtn=creatButton("1/x",SLOT(unaryOperatorClicked()));
    Button*equalBtn=creatButton("=",SLOT(equalClicked()));
    Button*ChangeBtn=creatButton("\302\261",SLOT(changeClicked()));
    auto mainLayOut=new QGridLayout(this);
    mainLayOut->addWidget(display,0,0,1,7);
    mainLayOut->addWidget(backspaceBtn,1,0,1,2);//回退
    mainLayOut->addWidget(clearBtn,1,2,1,2);//清除
    mainLayOut->addWidget(clearAlBtn,1,4,1,3);//清除所有
    mainLayOut->addItem(new QSpacerItem(20,40,QSizePolicy::Expanding),2,0);
    for(int i=0;i<10;i++){
        int row=((9-i)/3)+2;//行
        int col=((i-1)%3)+1;//列
        mainLayOut->addWidget(digitButtons[i],row,col);
    }
    mainLayOut->addWidget(digitButtons[0],5,1);//数字
    mainLayOut->addWidget(pointBtn,5,2);//小数点
    mainLayOut->addWidget(ChangeBtn,5,3);//+-改变
    mainLayOut->addWidget(divBtn,2,4);//除
    mainLayOut->addWidget(timesBtn,3,4);
    mainLayOut->addWidget(minusBtn,4,4);
    mainLayOut->addWidget(plusBtn,5,4);//加
    mainLayOut->addWidget(sqrtBtn,2,5);//开方
    mainLayOut->addWidget(powerBtn,3,5);//次幂
    mainLayOut->addWidget(reciprocalBtn,4,5);
    mainLayOut->addWidget(equalBtn,5,5);//等于号
    mainLayOut->addItem(new QSpacerItem(20,40,QSizePolicy::Expanding),3,6);

}
Widget::~Widget()
{
    delete ui;
}

void Widget::digitClicked()
{
    Button*btn=qobject_cast<Button*>(sender());
    int digitValue=btn->text().toInt();// 将按钮文本转换为整数
    // 特殊情况：如果当前显示是 "0" 并且用户又按下了 0，则忽略此次点击
    if(display->text()=="0"&&digitValue==0){
        return;
    }
    // 如果处于等待操作数状态，说明要开始输入新数字，先清空显示
    if(waitingForOperand){
        display->clear();
        waitingForOperand=false;
    }
    // 将按下的数字追加到显示末尾
    display->setText(display->text()+QString::number(digitValue));
}

void Widget::pointClicked()
{
    if(waitingForOperand){
        display->setText("0");
    }
    if(!display->text().contains("."))//检查是否已包含小数点
    {
        display->setText((display->text()+"."));//追加小数点
        waitingForOperand=false;
    }
}

void Widget::backSpaceClicked()
{
    if(waitingForOperand){
        return;
    }
    else{
        QString txt=display->text();
        txt.chop(1);// 删除最后一个字符
        if(txt.isEmpty()){
            txt="0";
            waitingForOperand=true;
        }//如果为空显示0
        display->setText(txt);
    }
}

void Widget::clear()
{
    if(waitingForOperand){
        return;
    }
    else{
        display->setText("0");
        waitingForOperand=true;
    }
}

void Widget::clearAll()
{
    sumSofar=0;
    factorSoFar=0;
    pendingAdditiveOperator.clear();
    pendingMultipcativeOperator.clear();
    display->setText("0");
    waitingForOperand=true;
}

void Widget::muliplicativeOperatorClicked()
{
    Button*btn=qobject_cast<Button*>(sender());
    if(!btn){
        return;
    }
    QString oprator=btn->text();
    double operand = display->text().toDouble();//显示当前数字
    // 如果存在未处理的乘除运算符，则先完成上一次运算
    if(!pendingMultipcativeOperator.isEmpty()){
        if(!caculate(operand,oprator)){
            abortOperator();
            return;
        }
        // 显示乘除运算的中间结果
        display->setText(QString::number(factorSoFar));
    }
    else{
        // 没有未处理的乘除运算，则将当前操作数作为累积初始值
        factorSoFar=operand;
    }
    pendingMultipcativeOperator=oprator;
    waitingForOperand=true;
}

void Widget::additiveOperatorClicked()
{
    Button*btn=qobject_cast<Button*>(sender());
    QString oprator=btn->text();// 获取运算符文本
    double operand=display->text().toDouble();// 当前显示的数字
    // 处理优先级,先完成所有未完成的乘除运算
    if(!pendingMultipcativeOperator.isEmpty()){
        if(!caculate(operand,oprator)){
            abortOperator();
            return;
        }
        display->setText(QString::number(factorSoFar));// 显示乘除结果
        operand=factorSoFar;//更新操作数为结果
        factorSoFar=0;//清除中间量
        pendingMultipcativeOperator.clear();//清除运算符
    }
    if(!pendingAdditiveOperator.isEmpty()){
        if(!caculate(operand,oprator)){
          abortOperator();//报错
         return;
        }
        // 显示累积的加减结果
    display->setText(QString::number(sumSofar));
    }
    else{
        sumSofar=operand;
    }
    pendingAdditiveOperator=oprator;
    waitingForOperand=true;
}

void Widget::unaryOperatorClicked()
{
    Button*btn=qobject_cast<Button*>(sender());
    QString oprator=btn->text();
    // 将当前显示文本转换为双精度浮点数作为操作数
    double operand=display->text().toDouble();
    double result=0;
    // 根据运算符执行相应的运算
    if(oprator==("Sqrt")){
        if(operand<0){
            abortOperator();
            return;
        }
        result=std::sqrt(operand);
    }
   // 平方
    else if(oprator=="x\302\262"){
        result=std::pow(operand,2);
    }
    //倒数
    else if(oprator=="1/x"){
        if(operand==0){
            abortOperator();
            return;
        }
        result=1.0/operand;
    }
    display->setText(QString::number(result));
    waitingForOperand=true;

}

void Widget::equalClicked()//等于处理
{
    double operand=display->text().toDouble();
    //乘除
    if(!pendingMultipcativeOperator.isEmpty()){
        if(!caculate(operand,pendingMultipcativeOperator))
            {
            abortOperator();
            return;
        }
        operand=factorSoFar;
        factorSoFar=0;
        pendingMultipcativeOperator.clear();
    }
    //加减
    if(!pendingAdditiveOperator.isEmpty()){
        if(!caculate(operand,pendingAdditiveOperator)){
            abortOperator();
            return;
        }
        pendingAdditiveOperator.clear();
    }
    else{
        sumSofar=operand;
    }
    display->setText(QString::number(sumSofar));
    sumSofar=0;
    waitingForOperand=true;
}

void Widget::changeClicked()//改变正负号
{
    QString txt=display->text();
    double v =txt.toDouble();
    if(v>0){
        txt.append("-");
    }
    else if(v<0){
        txt.remove(0,1);// 负数移除开头的负号即可
    }
    display->setText(txt);
}

Button *Widget::creatButton(const QString &text, const char *member)
{
    Button*btn=new Button(text);
    connect(btn,SIGNAL(clicked()),this,member);
    return btn;
}

bool Widget::caculate(double rightOperand, const QString &pendingOperator)
{
    if(pendingOperator=="+"){
        sumSofar+=rightOperand;
    }
    else if(pendingOperator=="-"){
        sumSofar-=rightOperand;
    }
    else if(pendingOperator=="\303\227")//乘法
    {
        factorSoFar*=rightOperand;
    }
    else if(pendingOperator=="\303\267")//除法
    {
        if(rightOperand==0.0){
            return false;
        }
        else{
            factorSoFar/=rightOperand;
        }
    }
    return true;
}

void Widget::abortOperator()//错误报告
{
    clearAll();
    display->setText("######");
}

