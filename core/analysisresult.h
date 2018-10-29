#ifndef ANALYSISRESULT_H
#define ANALYSISRESULT_H

#include "core_global.h"
#include "global.h"
#include <QVector>
#include <QAtomicInt>

class AnalysisResult
{
public:
    enum ErrorCode
    {
        NoError = 0,
        NullValue,
        InvalidValue,
        UnknownError,
    };

    AnalysisResult();
    AnalysisResult(ErrorCode e);
    ~AnalysisResult();

    QStringList value() const { return d->result; }
    void setValue(QStringList r);

    int flags() const { return d->flag; }
    void setFlags(int flags);
    void setFlag(vc::ResultFlag flag, bool set = true);
    bool testFlag(vc::ResultFlag flag) const;

    QString errorText() const;
    void clearErrorText();
    void setErrorText(const QString &e);

    ErrorCode error() const { return d->errorCode; }
    void setError(ErrorCode e);

    bool isValid() const { return d->errorCode == NoError; }
    bool isNull() const { return d->errorCode == NullValue; }

private:
    struct Data
    {
        QAtomicInt ref;
        QStringList result;
        int flag;
        ErrorCode errorCode;
        QString errorText;
    };
    Data *d;
    static Data shared_null;

    void detach();
};

Q_DECLARE_METATYPE(AnalysisResult)

#endif // ANALYSISRESULT_H
