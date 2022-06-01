#include "tobject.h"

TObject::TObject(QObject *parent) : QObject(parent)
{

}

void TObject::UpdateConfig(Configuration c)
{
    m_configuration = c;
    emit configsUpdated(c);
}
