#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "global.h"
#include <QObject>
#include <QStringList>
#include <QList>
#include <QMutex>
#include <QAtomicInt>
#include <QUuid>
#include <QReadWriteLock>

class ParamHeader;
class OptionFrame;
class QMainWindow;
class CheckStatusThread;
class Workbench;
class CameraData;

class CORE_EXPORT Instrument : public QObject, public QMutex
{
    Q_OBJECT
public:
    struct Message
    {
        int code;
        int type;
        QString text;

        Message() : code(0), type(QtDebugMsg) {}
        QString messageText() const;
        void report() const;
    };

    enum MessageCode
    {
        CodeOk = 0,
    };

    enum MessageChannel
    {
        ChannelLeft,
        ChannelRight,
        ChannelFront,
        ChannelBehind,
    };

    enum PauseReason
    {
        Crash,
        Other,
    };

    enum InstrumentState
    {
        UnconnectedState,
        ConnectingState,
        ConnectedState,
    };

    enum ErrorCode
    {
        ErrOk = 0,
        ErrTimeout,
        ErrDisconnected,
        ErrInstrumentRemoved,
    };


    explicit Instrument(Workbench *parent = 0);
    virtual ~Instrument();

    Workbench *workbench() const { return m_workbench; }

    InstrumentState state() const { return m_state; }
    bool isConnected() const;
    bool isRunning() const { return m_isRunning; }

    virtual bool hasFeature() const {return false;}
    virtual void setDebug(bool flag = false);
    virtual QString about() const { return QString(); }
    virtual void addOptionPage(OptionFrame *frame) {}
    virtual void manualTest() {}
    virtual QWidget *initDebugger() { return 0; }
    virtual void initMainWindow(QMainWindow *w) {}
    virtual void connectToHost() {}
    virtual void disconnectFromHost() {}
    virtual void waitForConnected() {}
    virtual bool canClose(bool &prompted) { return true; }

    // 仪器参数, 每个仪器有一个从InstrParamHeader派生出来的参数实例
    virtual const ParamHeader *param() const = 0;
    virtual void setParam(const ParamHeader *pa) = 0;
    bool isValidParam() const;

    // 打开仪器内部用于读写参数的文档
    virtual QIODevice *openParamFile(QIODevice::OpenMode mode) = 0;

    bool loadParam(QIODevice *device);
    bool saveParam(QIODevice *device) const;

    bool loadParam(const QString &filename);
    bool saveParam(const QString &filename) const;

    bool loadParam();
    bool saveParam() const;

    enum { MaxChunkSize = 4096 };
    static QByteArray readChunk(QIODevice *device);
    static bool writeChunk(const QByteArray &chunk, QIODevice *device);

    QUuid uuid() const { return m_uuid; }
    void setUuid(const QUuid &uuid);

    // 由CheckStatusThread调用
    virtual bool instantUpdate() { return false; }
    virtual void enableUpdate(bool enable);
    virtual void updateStatus() {}

    CheckStatusThread *checkStatusThread() const { return m_checkStatusThread; }


signals:
    void connected();
    void disconnected();
    void stateChanged(InstrumentState state);
    void fetalError(int code, const QString &detail);
    void uuidChanged();
    void runningStateChanged(bool running);
    void dataSampled(CameraData *data);

public slots:
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void clean() = 0;
    virtual void end();
    virtual void demo();
    virtual bool reset();

    void setRunning(bool running = true);

private:
    void loadMessage(const QString &filename, int type);

    Workbench *m_workbench;
    InstrumentState m_state;
    bool m_isRunning;
    QMap<int, Message> m_code;
    const QUuid m_uuid;
    CheckStatusThread *m_checkStatusThread;
    bool m_debugEnable;
};

#endif // INSTRUMENT_H
