#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>
#include <QtCore>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QFile>
#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/core/core.hpp"

#include "util_global.h"

using namespace std;

#if defined(WIN32) || defined(_WIN32)
#define OS_WINDOWS
#elif defined(__APPLE__) || defined(APPLE)
#define OS_UNIX
#elif defined(__linux__) || defined(linux)
#define OS_LINUX
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) \
if ((p)) {              \
    delete (p);         \
    (p) = NULL;         \
}
#endif

class UTIL_EXPORT Utilities
{
public:
    Utilities();
    ~Utilities();

    static long getTimestamp();

    /*
     * Get file name from a given path
     * bool postfix: including the postfix
     */
    static QString getFileName(const QString &path, const bool postfix = false);

    /*
     * Split the given string into segements by a delimiter
     */
    static QStringList splitString(const QString &str, const char delimiter);

    /*
     * returns the smaller of the two numbers
     */
    template<typename T>
    static T min(const T &v1, const T &v2) {
        return (v1 < v2) ? v1 : v2;
    }

    /*
     * Get files from a given folder
     * all: including all sub-folders
     */
    static QStringList getFiles(const QString &folder, const bool all = true);

    /*
     * Print string lines to std::out from an array of const char*,
     * this function is used for displaying command tips.
     * lines: should be end with (const char*)NULL.
     */
    static void print_str_lines(const char** lines) {
        int index = 0;
        while (lines[index++]) {
            qDebug() << lines[index - 1];
        }
    }


    /*
     * Read and print by line.
     */
    static void print_file_lines(const QString &file)
    {
      QFile fs(file);
      if (fs.open(QIODevice::ReadOnly | QIODevice::Text))
      {
          QTextStream in(&fs);
          do
          {
              QString line = in.readLine();
              qDebug() << line;
          } while(!in.atEnd());
          fs.close();
      }
      else
      {
        qDebug() << "cannot open file: " << file;
      }
    }

    template<class T>
    static unsigned int levenshtein_distance(const T &s1, const T &s2) {
        const size_t len1 = s1.size(), len2 = s2.size();
        std::vector<unsigned int> col(len2 + 1), prevCol(len2 + 1);

        for (unsigned int i = 0; i < prevCol.size(); i++) prevCol[i] = i;
        for (unsigned int i = 0; i < len1; i++) {
            col[0] = i + 1;
            for (unsigned int j = 0; j < len2; j++)
                col[j + 1] = Utils::min(
                            easypr::Utils::min(prevCol[1 + j] + 1, col[j] + 1),
                        prevCol[j] + (s1[i] == s2[j] ? 0 : 1));
            col.swap(prevCol);
        }
        return prevCol[len2];
    }

    /*
     * Create multi-level directories by given folder.
     */
    static bool mkdir(const QString &folder);

    /*
     * Make sure the destination folder exists,
     * if not, create it, then call cv::imwrite.
     */
    static bool imwrite(const QString &file, const cv::Mat &image);

    static QString utf8_to_gbk(const char* utf8);


private:

};

#endif // UTILITIES_H
