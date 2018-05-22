#include "scanner.h"
#include <QCoreApplication>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Scanner scanner;
    scanner.sendRequest();

    return a.exec();
}
