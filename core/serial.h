#ifndef SERIAL_H
#define SERIAL_H

#include "core_global.h"
#include <QIODevice>
#include <QStringList>

class CORE_EXPORT Serial : public QIODevice
{
	Q_OBJECT

public:
	enum Parity		{ No, Odd, Even, Mark, Space };
	enum StopBits	{ One, One5, Two };
	typedef void* HANDLE;

	explicit Serial(QObject *parent = 0);
	~Serial();

	void setFileName(const QString& filename) { m_filename = filename; }
	QString fileName() const { return m_filename; }

	bool open();
	bool open(OpenMode mode);
	void close();
	bool isSequential() const { return true; }

	bool setSettings(int baudrate = 9600, 
					 int byteSize = 8, 
					 Parity parity = No,
					 StopBits stopBits = One);
	bool setTimeouts(int readInterval, int readTimeouts = 0, int writeTimeouts = 0);
	void clearInputBuffer();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

	qint64 bytesAvailable() const;

	HANDLE handle() const { return m_handle; }

	static QStringList EnumDevices();

private:
	QString		m_filename;
	HANDLE		m_handle;
};

#endif // SERIAL_H
