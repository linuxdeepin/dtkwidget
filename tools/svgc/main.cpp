#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSvgRenderer>
#include <QImage>
#include <QPainter>

#include "DSvgRenderer"

static int String2Int(QString str, int defaultValue)
{
    bool ok = false;
    auto newValue = str.toInt(&ok);
    if (!ok || newValue <= 0) {
        return  defaultValue;
    }
    return newValue;
}

static QSize parseSize(QString sizeString, QSize origin = QSize(128, 128))
{
    if (sizeString.startsWith("x")) {
        auto newHeight = String2Int(sizeString.remove("x"), origin.height());
        auto newWidth = origin.width() * newHeight / origin.height();
        return QSize(newWidth, newHeight);
    } else {
        auto wh = sizeString.split("x");
        wh.removeAll("");
        if (1 == wh.length()) {
            auto newWidth = String2Int(wh.value(0), origin.width());
            auto newHeight = origin.height() * newWidth / origin.width();
            return QSize(newWidth, newHeight);
        }
        if (2 == wh.length()) {
            auto newWidth = String2Int(wh.value(0), origin.width());
            auto newHeight = String2Int(wh.value(1), origin.height());
            return QSize(newWidth, newHeight);
        }
    }
    return origin;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("dtk-scgc");

    QCommandLineParser parser;
    QCommandLineOption render = QCommandLineOption(QStringList() << "r" << "render",
                                "svg render, DSvgRenderer/QSvgRenderer, default QSvgRenderer",
                                "render",
                                "QSvgRenderer");
    QCommandLineOption factor = QCommandLineOption(QStringList() << "f" << "factor",
                                "float scale factor, must > 0, default 1.0",
                                "factor",
                                "1.0");
    QCommandLineOption size = QCommandLineOption(QStringList() << "s" << "size",
                              "output size, widthxheight, e.g., 100x200, 100x, x200",
                              "size",
                              "1.0");
    parser.addOption(render);
    parser.addOption(size);
    parser.addOption(factor);
    parser.addPositionalArgument("input", "input svg file");
    parser.addPositionalArgument("output", "output png file");
    parser.addHelpOption();

    parser.process(app);

    auto input = parser.positionalArguments().value(0);
    auto output = parser.positionalArguments().value(1);

    if (input.isEmpty() || output.isEmpty()) {
        parser.showHelp();
    }

    QString renderValue = parser.value("render");
    QString sizeValue = parser.value("size");
    double factorValue = parser.value("factor").toDouble();

    auto inputImage = QImage(input);
    auto originSize = inputImage.size();
    if (originSize.isEmpty()) {
        originSize = QSize(128, 128);
    }
    originSize = parseSize(sizeValue, originSize);
    auto outputSize = (QSizeF(originSize) * factorValue).toSize();
    QImage outPutimage(outputSize, QImage::Format_ARGB32);
    outPutimage.fill(Qt::transparent);
    QPainter outputPainter(&outPutimage);

    if ("DSvgRenderer" == renderValue) {
        Dtk::Widget::DSvgRenderer renderer(input);
        renderer.render(&outputPainter);
    } else {
        QSvgRenderer renderer(input);
        renderer.render(&outputPainter);
    }

    outPutimage.save(output);
    return 0;
}
