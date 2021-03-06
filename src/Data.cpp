#include "Data.h"

#include <QTextCodec>

// Note that these aren't all possible BOMs. But they are probably the most common ones.
// The size is needed at least for the ones with character zero in them.
static const QByteArray bom3("\xEF\xBB\xBF", 3);
static const QByteArray bom2a("\xFE\xFF", 2);
static const QByteArray bom2b("\xFF\xFE", 2);
static const QByteArray bom4a("\x00\x00\xFE\xFF", 4);
static const QByteArray bom4b("\xFF\xFE\x00\x00", 4);

bool isTextOnly(QByteArray data, const QString& encoding, bool quickTest)
{
    // If the data starts with a Unicode BOM, we always assume it is text
    if(startsWithBom(data))
        return true;

    // Truncate to the first couple of bytes for quick testing
    if(quickTest)
        data = data.left(512);

    // Convert to Unicode if necessary
    if(!encoding.isEmpty())
        data = QTextCodec::codecForName(encoding.toUtf8())->toUnicode(data).toUtf8();

    // Perform check
    return QString(data).toUtf8() == data;
}

bool startsWithBom(const QByteArray& data)
{
    if(data.startsWith(bom3) ||
            data.startsWith(bom2a) || data.startsWith(bom2b) ||
            data.startsWith(bom4a) || data.startsWith(bom4b))
    return true;
    else
        return false;
}

QByteArray removeBom(QByteArray& data)
{
    if(data.startsWith(bom3))
    {
        QByteArray bom = data.left(3);
        data.remove(0, 3);
        return bom;
    } else if(data.startsWith(bom2a) || data.startsWith(bom2b)) {
        QByteArray bom = data.left(2);
        data.remove(0, 2);
        return bom;
    } else if(data.startsWith(bom4a) || data.startsWith(bom4b)) {
        QByteArray bom = data.left(4);
        data.remove(0, 4);
        return bom;
    } else {
        return QByteArray();
    }
}
