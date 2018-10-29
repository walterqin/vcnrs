#include "charsrecogniseinterface.h"
#include "workbench.h"

CharsRecogniseInterface::CharsRecogniseInterface(NumberRecognition *ma, QObject *parent)
    : QObject(parent)
    , m_owner(ma)
{

}

CharsRecogniseInterface::~CharsRecogniseInterface()
{

}

