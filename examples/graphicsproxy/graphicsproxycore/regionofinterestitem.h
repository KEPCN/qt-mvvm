// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef GRAPHICSPROXY_REGIONOFINTERESTITEM_H
#define GRAPHICSPROXY_REGIONOFINTERESTITEM_H

#include "mvvm/model/compounditem.h"

namespace GraphicsProxy {

//! Item to represent region of interest on top of heat map.

class RegionOfInterestItem : public ModelView::CompoundItem {
public:
    static inline const std::string P_XLOW = "P_XLOW";
    static inline const std::string P_YLOW = "P_YLOW";
    static inline const std::string P_XUP = "P_XUP";
    static inline const std::string P_YUP = "P_YUP";

    RegionOfInterestItem();

    double xLow() const;
    void setXLow(double value);

    double yLow() const;
    void setYLow(double value);

    double xUp() const;
    void setXUp(double value);

    double yUp() const;
    void setYUp(double value);
};

} // namespace GraphicsProxy

#endif // GRAPHICSPROXY_REGIONOFINTERESTITEM_H
