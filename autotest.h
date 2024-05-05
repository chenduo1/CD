#ifndef AUTOTEST_H
#define AUTOTEST_H

#endif // AUTOTEST_H

#include <QString>
#include <QVector>
#include <QRandomGenerator>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>
#include <QComboBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>

class Expression {
public:
    QString generate();
    QString generateOperand();
    QChar generateOperator();
    int maxNumber;
    QVector<QChar> operators;
    bool allowParentheses;
    bool allowDecimals;
};

//  生成括号及调用其他生成函数
QString Expression::generate() {
    int numOperands = QRandomGenerator::global()->bounded(2, 5);
    QString expr;
    int leftParenCount = 0;
    for (int i = 0; i < numOperands; i++) {
        if (i > 0 && allowParentheses && QRandomGenerator::global()->bounded(5) == 0) {
            expr += "(";
            leftParenCount++;
            expr += generateOperand();
            expr += generateOperator();
            expr += generateOperand();
            i += 2;
        } else {
            expr += generateOperand();
        }

        if (leftParenCount > 0 && (i == numOperands - 1 || QRandomGenerator::global()->bounded(3) == 0)) {
            expr += ")";
            leftParenCount--;
        }
        if (i < numOperands - 1) {
            expr += generateOperator();
        }
    }
    while (leftParenCount > 0) {
        expr += ")";
        leftParenCount--;
    }
    return expr;
}


// 生成操作数
QString Expression::generateOperand() {
    double operand = QRandomGenerator::global()->bounded(1, maxNumber + 1);
    if (allowDecimals && QRandomGenerator::global()->bounded(2) == 0) {
        operand += static_cast<double>(QRandomGenerator::global()->bounded(100)) / 100.0;
        return QString::number(operand, 'f', 2);
    }
    else {
        return QString::number(operand);
    }
}

// 生成操作符
QChar Expression::generateOperator() {
    int index = QRandomGenerator::global()->bounded(operators.size());
    return operators[index];
}

// 使用系统打印
void printDocument(QWidget *parent, const QString& expressions) {
    QPrinter printer;
    QPrintDialog printDialog(&printer, parent);

    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QFont font("Arial", 12);
        painter.setFont(font);

        QStringList lines = expressions.split("\n");
        int y = 100;
        for (const QString& line : lines) {
            painter.drawText(100, y, line);
            y += 20;
        }
    }
}
