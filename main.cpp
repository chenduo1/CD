#include "autotest.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.resize(800, 400);
    window.setWindowTitle("四则运算生成器");
    QVBoxLayout *layout = new QVBoxLayout(&window);

    QLabel *numLabel = new QLabel("请输入要生成的题目数量:");
    QLineEdit *numInput = new QLineEdit;

    QLabel *maxNumLabel = new QLabel("请输入最大数值:");
    QLineEdit *maxNumInput = new QLineEdit;

    QLabel *operatorsLabel = new QLabel("请选择运算符 (+, -, *, /):");
    QLineEdit *operatorsInput = new QLineEdit;

    QLabel *parenthesesLabel = new QLabel("是否允许括号:");
    QCheckBox *parenthesesInput = new QCheckBox;

    QLabel *decimalsLabel = new QLabel("是否允许小数:");
    QCheckBox *decimalsInput = new QCheckBox;

    QLabel *outputMethodLabel = new QLabel("输出方式:");
    QComboBox *outputMethodComboBox = new QComboBox;
    outputMethodComboBox->addItem("显示在界面");
    outputMethodComboBox->addItem("输出到文件");
    outputMethodComboBox->addItem("打印");

    QPushButton *generateButton = new QPushButton("生成");
    QLabel *outputLabel = new QLabel("生成的表达式将显示在这里");

    layout->addWidget(numLabel);
    layout->addWidget(numInput);
    layout->addWidget(maxNumLabel);
    layout->addWidget(maxNumInput);
    layout->addWidget(operatorsLabel);
    layout->addWidget(operatorsInput);
    layout->addWidget(parenthesesLabel);
    layout->addWidget(parenthesesInput);
    layout->addWidget(decimalsLabel);
    layout->addWidget(decimalsInput);
    layout->addWidget(outputMethodLabel);
    layout->addWidget(outputMethodComboBox);
    layout->addWidget(generateButton);
    layout->addWidget(outputLabel);

    Expression exp;
    QObject::connect(generateButton, &QPushButton::clicked, [&]() {
        bool ok;
        int numExpressions = numInput->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(&window, "错误", "请输入有效的数字!");
            return;
        }
        exp.maxNumber = maxNumInput->text().toInt();
        QString ops = operatorsInput->text();

        if (ops.isEmpty()) {
            QMessageBox::warning(&window, "错误", "请输入至少一个操作符!");
            return;
        }

        for (QChar op : ops) {
            if (op != '+' && op != '-' && op != '*' && op != '/') {
                QMessageBox::warning(&window, "错误", "请输入合法的运算符 (+, -, *, /)!");
                return;
            }
        }

        exp.operators = QVector<QChar>(ops.begin(), ops.end());
        exp.allowParentheses = parenthesesInput->isChecked();
        exp.allowDecimals = decimalsInput->isChecked();
        QString expressions;
        for (int i = 0; i < numExpressions; i++) {
            expressions += exp.generate() + " =\n";
        }

        if (outputMethodComboBox->currentIndex() == 0) {
            outputLabel->setText(expressions);
        }
        else if (outputMethodComboBox->currentIndex() == 1) {
            QString fileName = QFileDialog::getSaveFileName(&window, "保存文件", "", "文本文件 (*.txt)");
            if (!fileName.isEmpty()) {
                QFile file(fileName);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&file);
                    out << expressions;
                    file.close();
                } else {
                    QMessageBox::warning(&window, "错误", "无法打开文件进行写入!");
                }
            }
        }
        else {
            printDocument(&window, expressions);
        }
    });

    window.show();
    return app.exec();
}
