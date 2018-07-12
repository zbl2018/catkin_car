/****************************************************************************
** Meta object code from reading C++ file 'main_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/car_map/include/car_map/main_window.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_car_map__MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      57,   20,   20,   20, 0x0a,
      90,   84,   20,   20, 0x0a,
     128,  122,   20,   20, 0x0a,
     174,   20,   20,   20, 0x0a,
     194,   20,   20,   20, 0x0a,
     218,   20,   20,   20, 0x0a,
     244,   20,   20,   20, 0x0a,
     272,  270,   20,   20, 0x0a,
     312,   20,   20,   20, 0x0a,
     336,   20,   20,   20, 0x0a,
     369,   20,   20,   20, 0x0a,
     395,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_car_map__MainWindow[] = {
    "car_map::MainWindow\0\0"
    "Signal_ContinueSendEndPoint(string)\0"
    "on_actionAbout_triggered()\0check\0"
    "on_button_connect_clicked(bool)\0state\0"
    "on_checkbox_use_environment_stateChanged(int)\0"
    "updateLoggingView()\0on_pushButton_clicked()\0"
    "Slot_ShowBrushStatus(int)\0"
    "on_pushButton_2_clicked()\0,\0"
    "Slot_DrawLocation(PointCoordinates,int)\0"
    "Slot_SelectNewMap(bool)\0"
    "Slot_GetEndPointFromItem(string)\0"
    "Slot_OpenHelpManual(bool)\0"
    "Slot_ShowProcessStatus(int)\0"
};

void car_map::MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->Signal_ContinueSendEndPoint((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 1: _t->on_actionAbout_triggered(); break;
        case 2: _t->on_button_connect_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_checkbox_use_environment_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->updateLoggingView(); break;
        case 5: _t->on_pushButton_clicked(); break;
        case 6: _t->Slot_ShowBrushStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_pushButton_2_clicked(); break;
        case 8: _t->Slot_DrawLocation((*reinterpret_cast< PointCoordinates(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->Slot_SelectNewMap((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->Slot_GetEndPointFromItem((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 11: _t->Slot_OpenHelpManual((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->Slot_ShowProcessStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData car_map::MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject car_map::MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_car_map__MainWindow,
      qt_meta_data_car_map__MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &car_map::MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *car_map::MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *car_map::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_car_map__MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int car_map::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void car_map::MainWindow::Signal_ContinueSendEndPoint(string _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
