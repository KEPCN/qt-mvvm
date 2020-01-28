// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "Segment.h"
#include "SegmentItem.h" 
#include "Handle.h"
#include "HandleItem.h"
#include "AxisObject.h"

#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneMouseEvent>

#include <cmath>
#include <mvvm/signals/itemmapper.h>
#include <mvvm/utils/numericutils.h>

#include <iostream>


Segment::Segment(SegmentItem* item) : 
    segment_item(item), 
    color(item->property(SegmentItem::P_COLOR).value<QColor>()),
    _left_handle(nullptr),
    _right_handle(nullptr)
{
    auto on_property_change = [this](ModelView::SessionItem*, std::string property_name) {
        if (property_name == SegmentItem::P_X_POS)
            update();
        if (property_name == SegmentItem::P_Y_POS)
            update();
        if (property_name == SegmentItem::P_HEIGHT)
            update();
        if (property_name == SegmentItem::P_WIDTH)
            update();
        if (property_name == SegmentItem::P_HORIZONTAL)
            update();
        if (property_name == SegmentItem::P_COLOR)
            update();
    };

    segment_item->mapper()->setOnPropertyChange(on_property_change, this);
    setFlag(QGraphicsItem::ItemIsMovable);
    setZValue(10);
}

void Segment::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    AxisObject* axis = getAxes();
    if (!axis) return;

    auto draw_rect = QRectF(
        axis->fromRealToSceneX(
            segment_item->property(SegmentItem::P_X_POS).toDouble()
            - segment_item->property(SegmentItem::P_WIDTH).toDouble()/2.),
        axis->fromRealToSceneY(
            segment_item->property(SegmentItem::P_Y_POS).toDouble()
            - segment_item->property(SegmentItem::P_HEIGHT).toDouble()/2.),
        axis->fromRealToSceneX(segment_item->property(SegmentItem::P_WIDTH).toDouble()),
        axis->fromRealToSceneY(segment_item->property(SegmentItem::P_HEIGHT).toDouble())
    );

    painter->setBrush(color);
    painter->drawRect(draw_rect);
}

QPainterPath Segment::shape() const
{
    QPainterPath path;

    AxisObject* axis = getAxes();
    if (!axis) return path;

    path.addRect(
        axis->fromRealToSceneX(
            segment_item->property(SegmentItem::P_X_POS).toDouble()
            - segment_item->property(SegmentItem::P_WIDTH).toDouble()/2),
        axis->fromRealToSceneY(
            segment_item->property(SegmentItem::P_Y_POS).toDouble()- segment_item->property(SegmentItem::P_HEIGHT).toDouble()/2),
        axis->fromRealToSceneX(segment_item->property(SegmentItem::P_WIDTH).toDouble()),
        axis->fromRealToSceneY(segment_item->property(SegmentItem::P_HEIGHT).toDouble())
    );

    return path;
}

QRectF Segment::boundingRect() const
{
    double epsilon = 10;

    AxisObject* axis = getAxes();
    if (!axis) return QRectF(0,0,1,1);

    return QRectF(
        axis->fromRealToSceneX(
            segment_item->property(SegmentItem::P_X_POS).toDouble()
            - segment_item->property(SegmentItem::P_WIDTH).toDouble()/2.)-epsilon,
        axis->fromRealToSceneY(
            segment_item->property(SegmentItem::P_Y_POS).toDouble()
            - segment_item->property(SegmentItem::P_HEIGHT).toDouble()/2.)-epsilon,
        axis->fromRealToSceneX(segment_item->property(SegmentItem::P_WIDTH).toDouble())+2*epsilon,
        axis->fromRealToSceneY(segment_item->property(SegmentItem::P_HEIGHT).toDouble())+2*epsilon
    );

}

void Segment::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    AxisObject* axis = getAxes();
    if (!axis) return ;

    if (segment_item->property(SegmentItem::P_HORIZONTAL).toBool())
        segment_item->setProperty(
            SegmentItem::P_Y_POS, axis->fromSceneToRealY(double(y()+ event->pos().y())));
    else
        segment_item->setProperty(
            SegmentItem::P_X_POS, axis->fromSceneToRealX(double(x()+ event->pos().x())));

    moveHandles();
}

void Segment::addHandles(Handle* left_handle, Handle* right_handle)
{
    _left_handle = left_handle;
    _right_handle = right_handle;

    connectHandles();
    moveHandles();

}

void Segment::connectHandles()
{
    connect( 
        _left_handle, &Handle::moved,
        this, &Segment::refreshFromHandles);

    connect(
        _right_handle, &Handle::moved,
        this, &Segment::refreshFromHandles);
}

void Segment::disconnectHandles()
{
    disconnect( 
        _left_handle, &Handle::moved,
        this, &Segment::refreshFromHandles);

    disconnect(
        _right_handle, &Handle::moved,
        this, &Segment::refreshFromHandles);
}

void Segment::refreshFromHandles()
{
    if (!_left_handle)
        return;
    if (!_right_handle)
        return;

    AxisObject* axis = getAxes();
    if (!axis) return ;

    double left_x = _left_handle->handleItem()->property(HandleItem::P_XPOS).toDouble();
    double left_y = _left_handle->handleItem()->property(HandleItem::P_YPOS).toDouble();
    double right_x = _right_handle->handleItem()->property(HandleItem::P_XPOS).toDouble();
    double right_y = _right_handle->handleItem()->property(HandleItem::P_YPOS).toDouble();

    if (segment_item->property(SegmentItem::P_HORIZONTAL).toBool()){
        segment_item->setProperty(SegmentItem::P_X_POS, (right_x - left_x) / 2. + left_x);
        segment_item->setProperty(SegmentItem::P_Y_POS, left_y);
        segment_item->setProperty(SegmentItem::P_WIDTH,(right_x - left_x));
    }else{
        segment_item->setProperty(SegmentItem::P_X_POS, right_x);
        segment_item->setProperty(SegmentItem::P_Y_POS, (right_y - left_y)/2. + left_y );
        segment_item->setProperty(SegmentItem::P_HEIGHT,(right_y - left_y));
    }
}
void Segment::moveHandles()
{
    if (!_left_handle)
        return;
    if (!_right_handle)
        return;

    AxisObject* axis = getAxes();
    if (!axis) return ;

    // Put the handles in place
    double x_pos;
    double y_pos;
    HandleItem* item;

    disconnectHandles(); 

    if (segment_item->property(SegmentItem::P_HORIZONTAL).toBool()){

        x_pos = (
            segment_item->property(SegmentItem::P_X_POS).toDouble() 
            - segment_item->property(SegmentItem::P_WIDTH).toDouble()/2);

        y_pos = (
            segment_item->property(SegmentItem::P_Y_POS).toDouble());

        item = _left_handle->handleItem();
        item->setProperty(HandleItem::P_XPOS, x_pos);
        item->setProperty(HandleItem::P_YPOS, y_pos);
        
        x_pos = (
            segment_item->property(SegmentItem::P_X_POS).toDouble() 
            + segment_item->property(SegmentItem::P_WIDTH).toDouble()/2);

        y_pos = (
            segment_item->property(SegmentItem::P_Y_POS).toDouble());

        item = _right_handle->handleItem();
        item->setProperty(HandleItem::P_XPOS, x_pos);
        item->setProperty(HandleItem::P_YPOS, y_pos);

    }else{

        x_pos = (
            segment_item->property(SegmentItem::P_X_POS).toDouble());

        y_pos = (
            segment_item->property(SegmentItem::P_Y_POS).toDouble()
            - segment_item->property(SegmentItem::P_HEIGHT).toDouble()/2);

        item = _left_handle->handleItem();
        item->setProperty(HandleItem::P_XPOS, x_pos);
        item->setProperty(HandleItem::P_YPOS, y_pos);

        x_pos = (
            segment_item->property(SegmentItem::P_X_POS).toDouble());

        y_pos = (
            segment_item->property(SegmentItem::P_Y_POS).toDouble()
            + segment_item->property(SegmentItem::P_HEIGHT).toDouble()/2);

        item = _right_handle->handleItem();
        item->setProperty(HandleItem::P_XPOS, x_pos);
        item->setProperty(HandleItem::P_YPOS, y_pos);
    }
    
    connectHandles();
}

