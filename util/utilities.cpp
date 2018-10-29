#include "utilities.h"
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

#ifdef OS_WINDOWS
#include <Windows.h>
#include <direct.h>
#include <io.h>

#define PATH_DELIMITER '\\'

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#elif defined(OS_LINUX) || defined(OS_UNIX)

#include <cstring>
#include <direct.h>
#include <sys/stat.h>
#include <unistd.h>

#define PATH_DELIMITER '/'
#endif

#ifdef OS_UNIX
#include <sys/timeb.h>
#endif

#include <list>
#include "opencv2/highgui/highgui.hpp"

Utilities::Utilities()
{

}

Utilities::~Utilities()
{

}

long Utilities::getTimestamp()
{
#ifdef OS_WINDOWS
  return static_cast<long>(cv::getTickCount());
#endif

#ifdef OS_LINUX
  struct timespec ts;

  clock_gettime(CLOCK_MONOTONIC, &ts);

  return (ts.tv_sec * 1e3 + ts.tv_nsec / 1e6);
#endif

#ifdef OS_UNIX
  // there is no function provided by osx to get system tick count.
  // but considering the purpose by using this function,
  // we can simply return a millisecond since 1970/1/1 to calc the time elapse.
  struct timeb tb;
  ftime(&tb);
  return long(tb.time * 1e3 + tb.millitm);
#endif
}

QString Utilities::getFileName(const QString &path, const bool postfix)
{
    if (!path.isEmpty())
    {
        int last_slash = path.lastIndexOf(PATH_DELIMITER);
        int last_dot = path.lastIndexOf('.');
        if (last_dot < last_slash || last_dot == -1)
            return path.mid(last_slash + 1);
        else
        {
            if (postfix)
                return path.mid(last_slash + 1);

            return path.mid(last_slash + 1, last_dot - last_slash - 1);
        }
    }
    return QString("");
}

QStringList Utilities::splitString(const QString &str, const char delimiter)
{
    QStringList strList;
    strList = str.split(QRegExp(QString(&delimiter)));
    return strList;
}

QStringList Utilities::getFiles(const QString &folder, const bool all/* = true*/)
{
    QDir dir(folder);
    QStringList files;

    foreach(QFileInfo info, dir.entryInfoList())
    {
        if (info.isFile())
            files << info.fileName();
        else
        {
            if (all)
            {
                if (info.fileName() == "." || info.fileName() == "..")
                    continue;
                else
                    getFiles(info.absoluteFilePath());
            }
        }
    }
	return files;
}

bool Utilities::mkdir(const QString &folder)
{
    QDir dir;

    if (dir.exists(folder))
        return true;

    return (dir.mkdir(folder));
}

bool Utilities::imwrite(const QString &file, const cv::Mat &image)
{
    int pos = file.lastIndexOf(PATH_DELIMITER);
    QString path = file.left(pos + 1);
    mkdir(path);
    return cv::imwrite(file.toStdString(), image);
}

QString Utilities::utf8_to_gbk(const char* utf8)
{
    return QString(utf8);
}
