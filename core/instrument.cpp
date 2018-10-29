#include "instrument.h"
#include "checkstatusthread.h"
#include "workbench.h"
#include "checkstatusthread.h"
#include "paramheader.h"
#include "cameradata.h"

Instrument::Instrument(Workbench *parent)
    : QObject(parent)
    , m_workbench(parent)
{

}

Instrument::~Instrument()
{

}

bool Instrument::isConnected() const
{
    return state() >= ConnectedState;
}

bool Instrument::isValidParam() const
{
    return param() && param()->size;
}

bool Instrument::loadParam(QIODevice *device)
{
    QByteArray chunk = readChunk(device);
    if (chunk.isEmpty())
        return false;

    setParam((const ParamHeader *)chunk.constData());
    return true;
}

bool Instrument::saveParam(QIODevice *device) const
{
    const ParamHeader *p = param();
    if (!p)
        return false;

    QByteArray buf;
    buf.append((const char *)p, p->size);
    quint16 crc = qChecksum(buf.constData(), buf.size());
    buf.append((const char *)&crc, sizeof(quint16));

    if (device->write(buf) != buf.size())
        return false;

    return true;
}

bool Instrument::loadParam(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    return loadParam(&file);
}

bool Instrument::saveParam(const QString &filename) const
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    return saveParam(&file);
}

bool Instrument::loadParam()
{
	return true;
}

bool Instrument::saveParam() const
{
	return true;
}

QByteArray Instrument::readChunk(QIODevice *device)
{
	return QByteArray();
}

bool Instrument::writeChunk(const QByteArray &chunk, QIODevice *device)
{
	return true;
}

void Instrument::setUuid(const QUuid &uuid)
{
    Q_UNUSED(uuid);
}

void Instrument::end()
{

}

void Instrument::demo()
{

}

bool Instrument::reset()
{
	return false;
}

void Instrument::setDebug(bool flag)
{
    m_debugEnable = flag;
}

void Instrument::setRunning(bool running)
{
    m_isRunning = running;
}

void Instrument::enableUpdate(bool enable)
{
    if (enable)
        checkStatusThread()->start();
    else
        checkStatusThread()->pause();
}
