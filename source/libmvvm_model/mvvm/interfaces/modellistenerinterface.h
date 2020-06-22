// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_INTERFACES_MODELLISTENERINTERFACE_H
#define MVVM_INTERFACES_MODELLISTENERINTERFACE_H

#include <mvvm/signals/callback_types.h>

namespace ModelView
{

//! Interface to subscribe to various signals generated by SessionModel.

class MVVM_MODEL_EXPORT ModelListenerInterface
{
public:
    virtual ~ModelListenerInterface() = default;

    //! Sets callback to be notified on item's data change. The callback will be called
    //! with (SessionItem*, data_role).
    virtual void setOnDataChange(Callbacks::item_int_t f, Callbacks::slot_t client) = 0;

    //! Sets callback to be notified on item insert. The callback will be called with
    //! (SessionItem* parent, tagrow), where 'tagrow' denotes inserted child position.
    virtual void setOnItemInserted(Callbacks::item_tagrow_t f, Callbacks::slot_t client) = 0;

    //! Sets callback to be notified on item remove. The callback will be called with
    //! (SessionItem* parent, tagrow), where 'tagrow' denotes child position before the removal.
    virtual void setOnItemRemoved(Callbacks::item_tagrow_t f, Callbacks::slot_t client) = 0;

    //! Sets callback to be notified when the item is about to be removed. The callback will be called
    //! with (SessionItem* parent, tagrow), where 'tagrow' denotes child position being removed.
    virtual void setOnAboutToRemoveItem(Callbacks::item_tagrow_t f, Callbacks::slot_t client) = 0;

    //! Sets the callback for notifications on model destruction.
    virtual void setOnModelDestroyed(Callbacks::model_t f, Callbacks::slot_t client) = 0;

    //! Sets the callback to be notified just before the reset of the root item.
    virtual void setOnModelAboutToBeReset(Callbacks::model_t f, Callbacks::slot_t client) = 0;

    //! Sets the callback to be notified right after the root item recreation.
    virtual void setOnModelReset(Callbacks::model_t f, Callbacks::slot_t client) = 0;

    //! Removes given client from all subscriptions.
    virtual void unsubscribe(Callbacks::slot_t client) = 0;
};

} // namespace ModelView

#endif // MVVM_INTERFACES_MODELLISTENERINTERFACE_H
