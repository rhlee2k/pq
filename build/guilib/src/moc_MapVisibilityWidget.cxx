/****************************************************************************
** Meta object code from reading C++ file 'MapVisibilityWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../guilib/src/MapVisibilityWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapVisibilityWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_rtabmap__MapVisibilityWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      41,   30,   29,   29, 0x05,

 // slots: signature, parameters, type, tag, flags
      69,   29,   29,   29, 0x08,
      88,   29,   29,   29, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_rtabmap__MapVisibilityWidget[] = {
    "rtabmap::MapVisibilityWidget\0\0id,visible\0"
    "visibilityChanged(int,bool)\0"
    "signalVisibility()\0selectAll(bool)\0"
};

void rtabmap::MapVisibilityWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MapVisibilityWidget *_t = static_cast<MapVisibilityWidget *>(_o);
        switch (_id) {
        case 0: _t->visibilityChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->signalVisibility(); break;
        case 2: _t->selectAll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData rtabmap::MapVisibilityWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject rtabmap::MapVisibilityWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_rtabmap__MapVisibilityWidget,
      qt_meta_data_rtabmap__MapVisibilityWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &rtabmap::MapVisibilityWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *rtabmap::MapVisibilityWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *rtabmap::MapVisibilityWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_rtabmap__MapVisibilityWidget))
        return static_cast<void*>(const_cast< MapVisibilityWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int rtabmap::MapVisibilityWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void rtabmap::MapVisibilityWidget::visibilityChanged(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
