// ************************************************************************** //
//
//  Prototype of mini MVVM framework for bornagainproject.org
//
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher
//
// ************************************************************************** //

#ifndef DATAROLE_H
#define DATAROLE_H

#include <QVariant>

//! Represents pair of data,role for SessionItemData.

class DataRole
{
public:
    DataRole(const QVariant& data = QVariant(), int role = -1);
    QVariant m_data;
    int m_role;
    bool operator==(const DataRole& other) const;
};

#endif