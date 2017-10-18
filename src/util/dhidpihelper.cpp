#include "dhidpihelper.h"

#include <QPixmap>
#include <QImageReader>
#include <QApplication>
#include <QIcon>

DTK_BEGIN_NAMESPACE

/*!
 * \class DHiDPIHelper
 * \brief The DHiDPIHelper class provides helper to help applications support hiDPI.
 */

/*!
 * \brief DHiDPIHelper::loadNxPixmap loads the suitable @Nx image.
 * \param fileName is the original resource file name.
 * \return the hiDPI ready QPixmap.
 */
QPixmap DHiDPIHelper::loadNxPixmap(const QString &fileName)
{
    qreal sourceDevicePixelRatio = 1.0;
    qreal devicePixelRatio = qApp->devicePixelRatio();
    QPixmap pixmap;

    if (!qFuzzyCompare(sourceDevicePixelRatio, devicePixelRatio)) {
        QImageReader reader;
        reader.setFileName(qt_findAtNxFile(fileName, devicePixelRatio, &sourceDevicePixelRatio));
        if (reader.canRead()) {
            reader.setScaledSize(reader.size() * (devicePixelRatio / sourceDevicePixelRatio));
            pixmap = QPixmap::fromImage(reader.read());
            pixmap.setDevicePixelRatio(devicePixelRatio);
        }
    } else {
        pixmap.load(fileName);
    }

    return pixmap;
}

DTK_END_NAMESPACE
