/****************************************************************************
** Meta object code from reading C++ file 'view.h'
**
** Created: Tue Dec 13 11:03:32 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "view.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RenderViewer[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   14,   13,   13, 0x0a,
      48,   39,   13,   13, 0x0a,
      65,   13,   13,   13, 0x0a,
      79,   13,   13,   13, 0x0a,
      92,   13,   13,   13, 0x0a,
     107,   13,   13,   13, 0x0a,
     121,   13,   13,   13, 0x0a,
     138,   13,   13,   13, 0x0a,
     154,   13,   13,   13, 0x0a,
     164,   13,   13,   13, 0x0a,
     176,   13,   13,   13, 0x0a,
     185,   13,   13,   13, 0x0a,
     194,   13,   13,   13, 0x0a,
     213,   13,   13,   13, 0x0a,
     233,   13,   13,   13, 0x0a,
     252,   13,   13,   13, 0x0a,
     272,   13,   13,   13, 0x0a,
     286,   13,   13,   13, 0x0a,
     298,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RenderViewer[] = {
    "RenderViewer\0\0func\0setRayFunction(int)\0"
    "penWidth\0setPenWidth(int)\0setAddPoint()\0"
    "setAddLine()\0setMovePoint()\0setMoveLine()\0"
    "setDeletePoint()\0setDeleteLine()\0"
    "setLock()\0setUnlock()\0setCPU()\0setGPU()\0"
    "setLineColorLeft()\0setLineColorRight()\0"
    "setLineSigmaLeft()\0setLineSigmaRight()\0"
    "engineStart()\0showImage()\0showData()\0"
};

const QMetaObject RenderViewer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RenderViewer,
      qt_meta_data_RenderViewer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RenderViewer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RenderViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RenderViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RenderViewer))
        return static_cast<void*>(const_cast< RenderViewer*>(this));
    return QWidget::qt_metacast(_clname);
}

int RenderViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setRayFunction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: setPenWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: setAddPoint(); break;
        case 3: setAddLine(); break;
        case 4: setMovePoint(); break;
        case 5: setMoveLine(); break;
        case 6: setDeletePoint(); break;
        case 7: setDeleteLine(); break;
        case 8: setLock(); break;
        case 9: setUnlock(); break;
        case 10: setCPU(); break;
        case 11: setGPU(); break;
        case 12: setLineColorLeft(); break;
        case 13: setLineColorRight(); break;
        case 14: setLineSigmaLeft(); break;
        case 15: setLineSigmaRight(); break;
        case 16: engineStart(); break;
        case 17: showImage(); break;
        case 18: showData(); break;
        default: ;
        }
        _id -= 19;
    }
    return _id;
}
static const uint qt_meta_data_RenderControler[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      29,   16,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_RenderControler[] = {
    "RenderControler\0\0okPressed()\0quitPressed()\0"
};

const QMetaObject RenderControler::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RenderControler,
      qt_meta_data_RenderControler, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RenderControler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RenderControler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RenderControler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RenderControler))
        return static_cast<void*>(const_cast< RenderControler*>(this));
    return QWidget::qt_metacast(_clname);
}

int RenderControler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: okPressed(); break;
        case 1: quitPressed(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void RenderControler::okPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void RenderControler::quitPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_RenderWidget[] = {

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

static const char qt_meta_stringdata_RenderWidget[] = {
    "RenderWidget\0"
};

const QMetaObject RenderWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RenderWidget,
      qt_meta_data_RenderWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RenderWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RenderWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RenderWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RenderWidget))
        return static_cast<void*>(const_cast< RenderWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int RenderWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
