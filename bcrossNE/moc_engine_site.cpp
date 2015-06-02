/****************************************************************************
** Meta object code from reading C++ file 'engine_site.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "engine_site.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'engine_site.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_engine_site_t {
    QByteArrayData data[12];
    char stringdata[191];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_engine_site_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_engine_site_t qt_meta_stringdata_engine_site = {
    {
QT_MOC_LITERAL(0, 0, 11), // "engine_site"
QT_MOC_LITERAL(1, 12, 8), // "finished"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 28), // "finished_loadInitSiteContent"
QT_MOC_LITERAL(4, 51, 27), // "map<string,vector<string> >"
QT_MOC_LITERAL(5, 79, 7), // "content"
QT_MOC_LITERAL(6, 87, 24), // "finished_gettingBookInfo"
QT_MOC_LITERAL(7, 112, 38), // "pair<string,map<string,search..."
QT_MOC_LITERAL(8, 151, 14), // "returnBookInfo"
QT_MOC_LITERAL(9, 166, 7), // "working"
QT_MOC_LITERAL(10, 174, 11), // "flushToSite"
QT_MOC_LITERAL(11, 186, 4) // "stop"

    },
    "engine_site\0finished\0\0"
    "finished_loadInitSiteContent\0"
    "map<string,vector<string> >\0content\0"
    "finished_gettingBookInfo\0"
    "pair<string,map<string,searchResult> >\0"
    "returnBookInfo\0working\0flushToSite\0"
    "stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_engine_site[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    1,   45,    2, 0x06 /* Public */,
       6,    1,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   51,    2, 0x0a /* Public */,
      10,    0,   52,    2, 0x0a /* Public */,
      11,    0,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void engine_site::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        engine_site *_t = static_cast<engine_site *>(_o);
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->finished_loadInitSiteContent((*reinterpret_cast< map<string,vector<string> >(*)>(_a[1]))); break;
        case 2: _t->finished_gettingBookInfo((*reinterpret_cast< pair<string,map<string,searchResult> >(*)>(_a[1]))); break;
        case 3: _t->working(); break;
        case 4: _t->flushToSite(); break;
        case 5: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (engine_site::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&engine_site::finished)) {
                *result = 0;
            }
        }
        {
            typedef void (engine_site::*_t)(map<string,vector<string>> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&engine_site::finished_loadInitSiteContent)) {
                *result = 1;
            }
        }
        {
            typedef void (engine_site::*_t)(pair<string,map<string,searchResult>> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&engine_site::finished_gettingBookInfo)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject engine_site::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_engine_site.data,
      qt_meta_data_engine_site,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *engine_site::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *engine_site::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_engine_site.stringdata))
        return static_cast<void*>(const_cast< engine_site*>(this));
    return QObject::qt_metacast(_clname);
}

int engine_site::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void engine_site::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void engine_site::finished_loadInitSiteContent(map<string,vector<string>> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void engine_site::finished_gettingBookInfo(pair<string,map<string,searchResult>> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
