/****************************************************************************
** Meta object code from reading C++ file 'custommask.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Proyecto_3/custommask.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'custommask.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CustomMask_t {
    QByteArrayData data[9];
    char stringdata0[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CustomMask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CustomMask_t qt_meta_stringdata_CustomMask = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CustomMask"
QT_MOC_LITERAL(1, 11, 23), // "on_CrearFiltro_accepted"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 23), // "on_CrearFiltro_rejected"
QT_MOC_LITERAL(4, 60, 18), // "on_SetData_clicked"
QT_MOC_LITERAL(5, 79, 18), // "on_Tam_textChanged"
QT_MOC_LITERAL(6, 98, 4), // "arg1"
QT_MOC_LITERAL(7, 103, 16), // "on_X_textChanged"
QT_MOC_LITERAL(8, 120, 16) // "on_Y_textChanged"

    },
    "CustomMask\0on_CrearFiltro_accepted\0\0"
    "on_CrearFiltro_rejected\0on_SetData_clicked\0"
    "on_Tam_textChanged\0arg1\0on_X_textChanged\0"
    "on_Y_textChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CustomMask[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    1,   47,    2, 0x08 /* Private */,
       7,    1,   50,    2, 0x08 /* Private */,
       8,    1,   53,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,

       0        // eod
};

void CustomMask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CustomMask *_t = static_cast<CustomMask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_CrearFiltro_accepted(); break;
        case 1: _t->on_CrearFiltro_rejected(); break;
        case 2: _t->on_SetData_clicked(); break;
        case 3: _t->on_Tam_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->on_X_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->on_Y_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CustomMask::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CustomMask.data,
      qt_meta_data_CustomMask,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CustomMask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CustomMask::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CustomMask.stringdata0))
        return static_cast<void*>(const_cast< CustomMask*>(this));
    return QDialog::qt_metacast(_clname);
}

int CustomMask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
