/****************************************************************************
** Meta object code from reading C++ file 'gamescreen.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../gamescreen.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gamescreen.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10GameScreenE_t {};
} // unnamed namespace

template <> constexpr inline auto GameScreen::qt_create_metaobjectdata<qt_meta_tag_ZN10GameScreenE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GameScreen",
        "0$Ba2xCXcEeCf5B9p+EHv7PnfRJR8",
        "backToHomeClicked",
        "",
        "updateTimerDisplay",
        "onUndoClicked",
        "onClearAllClicked",
        "onNewGameClicked",
        "onSolveClicked",
        "onSolverCellUpdated",
        "row",
        "col",
        "value",
        "onSolverFinished",
        "solved",
        "timeMs",
        "onNewGameFromOverlay",
        "onBackClicked",
        "onNumberButtonClicked",
        "number",
        "onCellFocused"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'backToHomeClicked'
        QtMocHelpers::SignalData<void()>(2, 3, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateTimerDisplay'
        QtMocHelpers::SlotData<void()>(4, 3, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onUndoClicked'
        QtMocHelpers::SlotData<void()>(5, 3, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onClearAllClicked'
        QtMocHelpers::SlotData<void()>(6, 3, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNewGameClicked'
        QtMocHelpers::SlotData<void()>(7, 3, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSolveClicked'
        QtMocHelpers::SlotData<void()>(8, 3, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSolverCellUpdated'
        QtMocHelpers::SlotData<void(int, int, int)>(9, 3, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 10 }, { QMetaType::Int, 11 }, { QMetaType::Int, 12 },
        }}),
        // Slot 'onSolverFinished'
        QtMocHelpers::SlotData<void(bool, int)>(13, 3, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 14 }, { QMetaType::Int, 15 },
        }}),
        // Slot 'onNewGameFromOverlay'
        QtMocHelpers::SlotData<void()>(16, 3, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBackClicked'
        QtMocHelpers::SlotData<void()>(17, 3, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNumberButtonClicked'
        QtMocHelpers::SlotData<void(int)>(18, 3, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 19 },
        }}),
        // Slot 'onCellFocused'
        QtMocHelpers::SlotData<void(int, int)>(20, 3, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 10 }, { QMetaType::Int, 11 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    int qt_metaObjectHashIndex = 1;
    return QtMocHelpers::metaObjectData<GameScreen, qt_meta_tag_ZN10GameScreenE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_metaObjectHashIndex);
}
Q_CONSTINIT const QMetaObject GameScreen::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GameScreenE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GameScreenE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10GameScreenE_t>.metaTypes,
    nullptr
} };

void GameScreen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GameScreen *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->backToHomeClicked(); break;
        case 1: _t->updateTimerDisplay(); break;
        case 2: _t->onUndoClicked(); break;
        case 3: _t->onClearAllClicked(); break;
        case 4: _t->onNewGameClicked(); break;
        case 5: _t->onSolveClicked(); break;
        case 6: _t->onSolverCellUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 7: _t->onSolverFinished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->onNewGameFromOverlay(); break;
        case 9: _t->onBackClicked(); break;
        case 10: _t->onNumberButtonClicked((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->onCellFocused((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GameScreen::*)()>(_a, &GameScreen::backToHomeClicked, 0))
            return;
    }
}

const QMetaObject *GameScreen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameScreen::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GameScreenE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GameScreen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void GameScreen::backToHomeClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
