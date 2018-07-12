/****************************************************************************
** Meta object code from reading C++ file 'interactive_view.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/car_map/include/car_map/interactive_view.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'interactive_view.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_InteractiveView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      26,   16,   16,   16, 0x0a,
      48,   36,   16,   16, 0x0a,
      66,   60,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_InteractiveView[] = {
    "InteractiveView\0\0zoomIn()\0zoomOut()\0"
    "scaleFactor\0zoom(float)\0delta\0"
    "translate(QPointF)\0"
};

void InteractiveView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        InteractiveView *_t = static_cast<InteractiveView *>(_o);
        switch (_id) {
        case 0: _t->zoomIn(); break;
        case 1: _t->zoomOut(); break;
        case 2: _t->zoom((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->translate((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData InteractiveView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject InteractiveView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_InteractiveView,
      qt_meta_data_InteractiveView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &InteractiveView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *InteractiveView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *InteractiveView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_InteractiveView))
        return static_cast<void*>(const_cast< InteractiveView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int InteractiveView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
