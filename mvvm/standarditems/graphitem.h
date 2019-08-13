// ************************************************************************** //
//
//  Prototype of mini MVVM framework for bornagainproject.org
//
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher
//
// ************************************************************************** //

#ifndef MVVM_GRAPHITEM_H
#define MVVM_GRAPHITEM_H

#include "compounditem.h"

namespace ModelView
{


/*!
@class GraphItem
@brief Serves as one-dimensional graph representation of Data1DItem.

Contains plot properties (i.e. color, line type etc) and link to Data1DItem.
*/

class CORE_EXPORT GraphItem : public CompoundItem
{
public:
    static inline const std::string P_LINK = "P_LINK";
    static inline const std::string P_COLOR = "P_COLOR";
    GraphItem();
};

} // namespace ModelView

#endif // MVVM_GRAPHITEM_H