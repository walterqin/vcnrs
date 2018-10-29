#include "Serial.h"
#include <Windows.h>
#include <QDebug>
#include <qglobal.h>

#ifndef PRINTABLE
#define PRINTABLE(s) (s).toUtf8().constData()
#endif


QStringList Serial::EnumDevices()
{
	QStringList devices;
	HKEY hKey;
	if (ERROR_SUCCESS == RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"HARDWARE\\DEVICEMAP\\SERIALCOMM",
		0, KEY_READ, &hKey))
	{
		DWORD subkeys, maxSubKeyLen, values, maxValueNameLen, maxValueLen, i;
		if (ERROR_SUCCESS == RegQueryInfoKeyW(hKey, NULL, NULL, NULL, &subkeys, &maxSubKeyLen,
			NULL, &values, &maxValueNameLen, &maxValueLen, NULL, NULL))
		{
			WCHAR *pValue = new WCHAR[maxValueLen+1];
			WCHAR *pValueName = new WCHAR[maxValueNameLen+1];
			DWORD valueNameLen, valueLen, type;

			for(i=0; i<values; i++)
			{
				valueNameLen = maxValueNameLen+1;
				valueLen = (maxValueLen+1)*2;

				if (ERROR_SUCCESS == RegEnumValueW(hKey, i, pValueName, &valueNameLen, 
					NULL, &type, (LPBYTE)pValue, &valueLen))
				{
					if (type == REG_SZ)
						devices += QString::fromWCharArray(pValue);
				}
			}

			delete [] pValue;
			delete [] pValueName;
		}

		RegCloseKey(hKey);
	}

	return devices;
}

Serial::Serial(QObject *parent)
	: QIODevice(parent)
{
	m_handle = INVALID_HANDLE_VALUE;
}

Serial::~Serial()
{
	if (isOpen())
		close();
}

bool Serial::open()
{
	return open(ReadWrite | Unbuffered);
}

bool Serial::open(OpenMode mode)
{
	if (isOpen())
	{
		qWarning("Serial::open: Serial (%s) is already open", PRINTABLE(fileName()));
		return false;
	}

	QString filename = fileName();
	if (!filename.startsWith(QLatin1String("\\\\.\\")) ||
		!filename.startsWith(QLatin1String("//")))
		filename.prepend(QLatin1String("\\\\.\\"));

	DWORD shareMode = 0;

	int accessRights = 0;
	if (mode & QIODevice::ReadOnly)
		accessRights |= GENERIC_READ;
	if (mode & QIODevice::WriteOnly)
		accessRights |= GENERIC_WRITE;

	SECURITY_ATTRIBUTES securityAtts = { sizeof(SECURITY_ATTRIBUTES), NULL, FALSE };

    DWORD creationDisp = OPEN_EXISTING;

	m_handle = CreateFileW((WCHAR*)filename.unicode(), accessRights, 
		shareMode, &securityAtts, creationDisp, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_handle == INVALID_HANDLE_VALUE)
	{
		setErrorString(qt_error_string());
		return false;
	}

	QIODevice::open(mode);

	return true;
}

void Serial::close()
{
	QIODevice::close();
	CloseHandle(m_handle);
	m_handle = INVALID_HANDLE_VALUE;
}

bool Serial::setSettings(int baudrate, int byteSize, Parity parity, StopBits stopBits)
{
	if (!isOpen())
	{
		qWarning("Serial::setSettings: File is not open");
		return false;
	}

	DCB d = {sizeof(DCB)};
	if (!GetCommState(handle(), &d))
		return false;

	d.BaudRate = (DWORD)baudrate;
	d.ByteSize = (BYTE)byteSize;
	d.Parity   = (BYTE)parity;
	d.StopBits = (BYTE)stopBits;
	d.fBinary  = 1;

	// flow control
	d.fOutxCtsFlow = 0;
	d.fOutxDsrFlow = 0;
	d.fDsrSensitivity = 0;
	d.fOutX = 0;

	return SetCommState(handle(), &d);
}

bool Serial::setTimeouts(int readInterval, int readTimeouts, int writeTimeouts)
{
	COMMTIMEOUTS t = {readInterval, 1, readTimeouts, 1, writeTimeouts};

	return SetCommTimeouts(handle(), &t);
}

void Serial::clearInputBuffer()
{
	PurgeComm(handle(), PURGE_RXCLEAR);
}

qint64 Serial::readData(char *data, qint64 maxlen)
{
	DWORD read = 0;
	if (!ReadFile(handle(), data, (DWORD)maxlen, &read, NULL))
		setErrorString(qt_error_string());
	return read;
}

qint64 Serial::writeData(const char *data, qint64 len)
{
	DWORD written = 0;
	if (!WriteFile(handle(), data, (DWORD)len, &written, NULL))
		setErrorString(qt_error_string());
	return written;
}

qint64 Serial::bytesAvailable() const
{
	COMSTAT cs = {0};
	DWORD err;
	ClearCommError(handle(), &err, &cs);
	return cs.cbInQue + QIODevice::bytesAvailable();
}
