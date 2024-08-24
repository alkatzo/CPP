#include <QtTest>

// add necessary includes here

class Example : public QObject
{
    Q_OBJECT

public:
    Example();
    ~Example();

private slots:
    void test_case1();
};

Example::Example() {}

Example::~Example() {}

void Example::test_case1() {}

QTEST_APPLESS_MAIN(Example)

#include "tst_example.moc"
