#include "autotest.h"
#include <QtTest/QtTest>

class TestExpression : public QObject
{
    Q_OBJECT

private slots:
    void testGenerateOperand_data();
    void testGenerateOperand();
    void testGenerateOperator_data();
    void testGenerateOperator();
    void testGenerate_data();
    void testGenerate();
};

void TestExpression::testGenerateOperand_data()
{
    QTest::addColumn<int>("maxNumber");
    QTest::addColumn<bool>("allowDecimals");

    QTest::newRow("max 10, no decimals") << 10 << false;
    QTest::newRow("max 100, no decimals") << 100 << false;
    QTest::newRow("max 10, allow decimals") << 10 << true;
    QTest::newRow("max 100, allow decimals") << 100 << true;
}

void TestExpression::testGenerateOperand()
{
    QFETCH(int, maxNumber);
    QFETCH(bool, allowDecimals);

    Expression exp;
    exp.maxNumber = maxNumber;
    exp.allowDecimals = allowDecimals;

    QString operand = exp.generateOperand();

    bool ok;
    double value = operand.toDouble(&ok);

    QVERIFY2(ok, "生成的操作数不是有效的数字");
    QVERIFY2(value >= 1 && value <= maxNumber, "生成的操作数超出范围");

    if (!allowDecimals) {
        QVERIFY2(operand.indexOf('.') == -1, "不允许小数但生成了小数");
    }
}

void TestExpression::testGenerateOperator_data()
{
    QTest::addColumn<QVector<QChar>>("operators");

    QTest::newRow("+ only") << QVector<QChar>{'+'};
    QTest::newRow("+, -") << QVector<QChar>{'+', '-'};
    QTest::newRow("+, -, *, /") << QVector<QChar>{'+', '-', '*', '/'};
}

void TestExpression::testGenerateOperator()
{
    QFETCH(QVector<QChar>, operators);

    Expression exp;
    exp.operators = operators;

    QChar op = exp.generateOperator();

    QVERIFY2(operators.contains(op), "生成的运算符不在列表中");
}

void TestExpression::testGenerate_data()
{
    QTest::addColumn<int>("maxNumber");
    QTest::addColumn<QVector<QChar>>("operators");
    QTest::addColumn<bool>("allowParentheses");
    QTest::addColumn<bool>("allowDecimals");

    QTest::newRow("simple") << 10 << QVector<QChar>{'+', '-'} << false << false;
    QTest::newRow("with parentheses") << 20 << QVector<QChar>{'+', '-', '*'} << true << false;
    QTest::newRow("with decimals") << 100 << QVector<QChar>{'+'} << false << true;
    QTest::newRow("complex") << 100 << QVector<QChar>{'+', '-', '*', '/'} << true << true;
}

void TestExpression::testGenerate()
{
    QFETCH(int, maxNumber);
    QFETCH(QVector<QChar>, operators);
    QFETCH(bool, allowParentheses);
    QFETCH(bool, allowDecimals);

    Expression exp;
    exp.maxNumber = maxNumber;
    exp.operators = operators;
    exp.allowParentheses = allowParentheses;
    exp.allowDecimals = allowDecimals;

    QString expr = exp.generate();

    QVERIFY2(expr.length() > 0, "生成的表达式为空");

    // 可以添加更多对生成表达式格式和内容的断言
}

QTEST_MAIN(TestExpression)
#include "autotest.moc"
