/****************************************************************************
** Meta object code from reading C++ file 'vector.h'
**
** Created: Thu Oct 13 13:12:33 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "vector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Vgui[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_Vgui[] = {
    "Vgui\0"
};

const QMetaObject Vgui::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Vgui,
      qt_meta_data_Vgui, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Vgui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Vgui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Vgui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Vgui))
        return static_cast<void*>(const_cast< Vgui*>(this));
    return QWidget::qt_metacast(_clname);
}

int Vgui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_Vshader[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_Vshader[] = {
    "Vshader\0"
};

const QMetaObject Vshader::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Vshader,
      qt_meta_data_Vshader, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Vshader::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Vshader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Vshader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Vshader))
        return static_cast<void*>(const_cast< Vshader*>(this));
    return QThread::qt_metacast(_clname);
}

int Vshader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_Vectorize[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   10,   11,   10, 0x08,
      23,   10,   11,   10, 0x08,
      29,   10,   11,   10, 0x08,
      40,   10,   11,   10, 0x08,
      47,   10,   11,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Vectorize[] = {
    "Vectorize\0\0bool\0open()\0run()\0delaunay()\0"
    "test()\0save()\0"
};

const QMetaObject Vectorize::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Vectorize,
      qt_meta_data_Vectorize, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Vectorize::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Vectorize::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Vectorize::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Vectorize))
        return static_cast<void*>(const_cast< Vectorize*>(this));
    return QObject::qt_metacast(_clname);
}

int Vectorize::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = open();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = run();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = delaunay();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = test();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: { bool _r = save();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
