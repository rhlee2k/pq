/****************************************************************************
** Meta object code from reading C++ file 'PreferencesDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../guilib/include/rtabmap/gui/PreferencesDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PreferencesDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_rtabmap__PreferencesDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      52,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   27,   27,   27, 0x05,
      76,   27,   27,   27, 0x05,

 // slots: signature, parameters, type, tag, flags
     122,  116,   27,   27, 0x0a,
     143,  116,   27,   27, 0x0a,
     168,  116,   27,   27, 0x0a,
     196,  188,   27,   27, 0x0a,
     218,  214,   27,   27, 0x0a,
     242,   27,   27,   27, 0x0a,
     254,   27,   27,   27, 0x0a,
     279,  272,   27,   27, 0x08,
     309,  272,   27,   27, 0x08,
     350,  338,   27,   27, 0x08,
     369,   27,   27,   27, 0x08,
     391,   27,  386,   27, 0x08,
     406,   27,   27,   27, 0x08,
     420,   27,   27,   27, 0x08,
     447,   27,   27,   27, 0x08,
     481,   27,   27,   27, 0x08,
     508,   27,   27,   27, 0x08,
     551,  534,   27,   27, 0x08,
     584,  116,   27,   27, 0x08,
     602,  116,   27,   27, 0x08,
     621,  116,   27,   27, 0x08,
     642,  116,   27,   27, 0x08,
     664,   27,   27,   27, 0x08,
     687,   27,   27,   27, 0x08,
     701,   27,   27,   27, 0x08,
     723,   27,   27,   27, 0x08,
     741,   27,   27,   27, 0x08,
     766,   27,   27,   27, 0x08,
     789,   27,   27,   27, 0x08,
     822,   27,   27,   27, 0x08,
     840,   27,   27,   27, 0x08,
     856,   27,   27,   27, 0x08,
     879,   27,   27,   27, 0x08,
     900,   27,   27,   27, 0x08,
     923,   27,   27,   27, 0x08,
     947,   27,   27,   27, 0x08,
     974,   27,   27,   27, 0x08,
    1006,   27,   27,   27, 0x08,
    1040,   27,   27,   27, 0x08,
    1070,   27,   27,   27, 0x08,
    1097,   27,   27,   27, 0x08,
    1132,   27,   27,   27, 0x08,
    1168,   27,   27,   27, 0x08,
    1193,   27,   27,   27, 0x08,
    1217,   27,   27,   27, 0x08,
    1247,   27,   27,   27, 0x08,
    1269,   27,   27,   27, 0x08,
    1292,   27,   27,   27, 0x08,
    1320,   27,   27,   27, 0x08,
    1335,   27,   27,   27, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_rtabmap__PreferencesDialog[] = {
    "rtabmap::PreferencesDialog\0\0"
    "settingsChanged(PreferencesDialog::PANEL_FLAGS)\0"
    "settingsChanged(rtabmap::ParametersMap)\0"
    "value\0setInputRate(double)\0"
    "setDetectionRate(double)\0setTimeLimit(float)\0"
    "enabled\0setSLAMMode(bool)\0src\0"
    "selectSourceDriver(Src)\0calibrate()\0"
    "calibrateSimple()\0button\0"
    "closeDialog(QAbstractButton*)\0"
    "resetApply(QAbstractButton*)\0panelNumber\0"
    "resetSettings(int)\0loadConfigFrom()\0"
    "bool\0saveConfigTo()\0resetConfig()\0"
    "makeObsoleteGeneralPanel()\0"
    "makeObsoleteCloudRenderingPanel()\0"
    "makeObsoleteLoggingPanel()\0"
    "makeObsoleteSourcePanel()\0current,previous\0"
    "clicked(QModelIndex,QModelIndex)\0"
    "addParameter(int)\0addParameter(bool)\0"
    "addParameter(double)\0addParameter(QString)\0"
    "updatePredictionPlot()\0updateKpROI()\0"
    "updateG2oVisibility()\0useOdomFeatures()\0"
    "changeWorkingDirectory()\0"
    "changeDictionaryPath()\0"
    "changeOdomBowFixedLocalMapPath()\0"
    "readSettingsEnd()\0setupTreeView()\0"
    "updateBasicParameter()\0openDatabaseViewer()\0"
    "selectSourceDatabase()\0selectCalibrationPath()\0"
    "selectSourceImagesStamps()\0"
    "selectSourceRGBDImagesPathRGB()\0"
    "selectSourceRGBDImagesPathDepth()\0"
    "selectSourceImagesPathScans()\0"
    "selectSourceImagesPathGt()\0"
    "selectSourceStereoImagesPathLeft()\0"
    "selectSourceStereoImagesPathRight()\0"
    "selectSourceImagesPath()\0"
    "selectSourceVideoPath()\0"
    "selectSourceStereoVideoPath()\0"
    "selectSourceOniPath()\0selectSourceOni2Path()\0"
    "updateSourceGrpVisibility()\0testOdometry()\0"
    "testCamera()\0"
};

void rtabmap::PreferencesDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PreferencesDialog *_t = static_cast<PreferencesDialog *>(_o);
        switch (_id) {
        case 0: _t->settingsChanged((*reinterpret_cast< PreferencesDialog::PANEL_FLAGS(*)>(_a[1]))); break;
        case 1: _t->settingsChanged((*reinterpret_cast< rtabmap::ParametersMap(*)>(_a[1]))); break;
        case 2: _t->setInputRate((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setDetectionRate((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setTimeLimit((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->setSLAMMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->selectSourceDriver((*reinterpret_cast< Src(*)>(_a[1]))); break;
        case 7: _t->calibrate(); break;
        case 8: _t->calibrateSimple(); break;
        case 9: _t->closeDialog((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 10: _t->resetApply((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 11: _t->resetSettings((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->loadConfigFrom(); break;
        case 13: { bool _r = _t->saveConfigTo();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 14: _t->resetConfig(); break;
        case 15: _t->makeObsoleteGeneralPanel(); break;
        case 16: _t->makeObsoleteCloudRenderingPanel(); break;
        case 17: _t->makeObsoleteLoggingPanel(); break;
        case 18: _t->makeObsoleteSourcePanel(); break;
        case 19: _t->clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 20: _t->addParameter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->addParameter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: _t->addParameter((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 23: _t->addParameter((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 24: _t->updatePredictionPlot(); break;
        case 25: _t->updateKpROI(); break;
        case 26: _t->updateG2oVisibility(); break;
        case 27: _t->useOdomFeatures(); break;
        case 28: _t->changeWorkingDirectory(); break;
        case 29: _t->changeDictionaryPath(); break;
        case 30: _t->changeOdomBowFixedLocalMapPath(); break;
        case 31: _t->readSettingsEnd(); break;
        case 32: _t->setupTreeView(); break;
        case 33: _t->updateBasicParameter(); break;
        case 34: _t->openDatabaseViewer(); break;
        case 35: _t->selectSourceDatabase(); break;
        case 36: _t->selectCalibrationPath(); break;
        case 37: _t->selectSourceImagesStamps(); break;
        case 38: _t->selectSourceRGBDImagesPathRGB(); break;
        case 39: _t->selectSourceRGBDImagesPathDepth(); break;
        case 40: _t->selectSourceImagesPathScans(); break;
        case 41: _t->selectSourceImagesPathGt(); break;
        case 42: _t->selectSourceStereoImagesPathLeft(); break;
        case 43: _t->selectSourceStereoImagesPathRight(); break;
        case 44: _t->selectSourceImagesPath(); break;
        case 45: _t->selectSourceVideoPath(); break;
        case 46: _t->selectSourceStereoVideoPath(); break;
        case 47: _t->selectSourceOniPath(); break;
        case 48: _t->selectSourceOni2Path(); break;
        case 49: _t->updateSourceGrpVisibility(); break;
        case 50: _t->testOdometry(); break;
        case 51: _t->testCamera(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData rtabmap::PreferencesDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject rtabmap::PreferencesDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_rtabmap__PreferencesDialog,
      qt_meta_data_rtabmap__PreferencesDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &rtabmap::PreferencesDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *rtabmap::PreferencesDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *rtabmap::PreferencesDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_rtabmap__PreferencesDialog))
        return static_cast<void*>(const_cast< PreferencesDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int rtabmap::PreferencesDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 52)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 52;
    }
    return _id;
}

// SIGNAL 0
void rtabmap::PreferencesDialog::settingsChanged(PreferencesDialog::PANEL_FLAGS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void rtabmap::PreferencesDialog::settingsChanged(rtabmap::ParametersMap _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
