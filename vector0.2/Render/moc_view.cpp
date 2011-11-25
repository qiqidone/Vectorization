/****************************************************************************
** Meta object code from reading C++ file 'view.h'
**
** Created: Thu Nov 24 21:16:33 2011
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
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   14,   13,   13, 0x0a,
      40,   13,   13,   13, 0x0a,
      54,   13,   13,   13, 0x0a,
      67,   13,   13,   13, 0x0a,
      82,   13,   13,   13, 0x0a,
      96,   13,   13,   13, 0x0a,
     113,   13,   13,   13, 0x0a,
     129,   13,   13,   13, 0x0a,
     139,   13,   13,   13, 0x0a,
     151,   13,   13,   13, 0x0a,
     160,   13,   13,   13, 0x0a,
     169,   13,   13,   13, 0x0a,
     188,   13,   13,   13, 0x0a,
     208,   13,   13,   13, 0x0a,
     227,   13,   13,   13, 0x0a,
     247,   13,   13,   13, 0x0a,
     261,   13,   13,   13, 0x0a,
     273,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RenderViewer[] = {
    "RenderViewer\0\0penWidth\0setPenWidth(int)\0"
    "setAddPoint()\0setAddLine()\0setMovePoint()\0"
    "setMoveLine()\0setDeletePoint()\0"
    "setDeleteLine()\0setLock()\0setUnlock()\0"
    "setCPU()\0setGPU()\0setLineColorLeft()\0"
    "setLineColorRight()\0setLineSigmaLeft()\0"
    "setLineSigmaRight()\0engineStart()\0"
    "showImage()\0showData()\0"
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
        case 0: setPenWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: setAddPoint(); break;
        case 2: setAddLine(); break;
        case 3: setMovePoint(); break;
        case 4: setMoveLine(); break;
        case 5: setDeletePoint(); break;
        case 6: setDeleteLine(); break;
        case 7: setLock(); break;
        case 8: setUnlock(); break;
        case 9: setCPU(); break;
        case 10: setGPU(); break;
        case 11: setLineColorLeft(); break;
        case 12: setLineColorRight(); break;
        case 13: setLineSigmaLeft(); break;
        case 14: setLineSigmaRight(); break;
        case 15: engineStart(); break;
        case 16: showImage(); break;
        case 17: showData(); break;
        default: ;
        }
        _id -= 18;
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
