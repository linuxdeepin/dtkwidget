#ifndef DWIDGETSTYPE_H
#define DWIDGETSTYPE_H

#include <dtkwidget_global.h>
#include <QFrame>

QT_BEGIN_NAMESPACE

class QScrollBar;
class QPushButton;
class QToolButton;
class QRadioButton;
class QDialogButtonBox;
class QListWidget;
class QTreeWidget;
class QTableWidget;
class QGroupBox;
class QScrollArea;
class QToolBox;
class QTableWidget;
class QStackedWidget;
class QFrame;
class QWidget;
class QMDIArea;
class QDockWidget;
class QComboBox;
class QFontComboBox;
class QTextEdit;
class QPlainTextEdit;
class QSpinBox;
class QDoubleSpinBox;
class QTimeEdit;
class QDateEdit;
class QDateTimeEdit;
class QDial;
class QHorizontalSlider;
class QVerticalSlider;
class QKeySequenceEdit;
class QTextBrowser;
class QGraphicsView;
class QCalendarWidget;
class QLCDNumber;
class QProgressBar;
class QHorizontalLine;
class QVerticalLine;
class QOpenGLWidget;
class QQuickWidget;
class QWebView;
class QAccessibleWidget;
class QCheckBox;
class QColorDialog;
class QColumnView;
class QDataWidgetMapper;
class QFocusFrame;
class QHeaderView;
class QInputDialog;
class QMdiArea;
class QMdiSubWindow;
class QErrorMessage;
class QFontDialog;
class QMenu;
class QMenuBar;
class QMessageBox;
class QRubberBand;
class QSlider;
class QSplitter;
class QStatusBar;
class QTabWidget;
class QTableView;
class QTileRules;
class QToolBar;
class QToolTip;
class QTreeView;
class QUndoView;
class QWhatsThis;
class QWizard;
class QWizardPage;
class QLabel;

QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

typedef QScrollBar DScrollBar;
typedef QPushButton DPushButton;
typedef QToolButton DToolButton;
typedef QRadioButton  DRadioButton;
typedef QDialogButtonBox DDialogButtonBox;
typedef QListWidget DListWidget;
typedef QTreeWidget DTreeWidget;
typedef QTableWidget DTableWidget;
typedef QGroupBox DGroupBox;
typedef QScrollArea DScrollArea;
typedef QToolBox DToolBox;
typedef QTableWidget DTableWidget;
typedef QStackedWidget DStackedWidget;
typedef QFrame DFrame;
typedef QWidget DWidget;
typedef QMDIArea DMDIArea;
typedef QDockWidget DDockWidget;
typedef QComboBox DComboBox;
typedef QFontComboBox DFontComboBox;
typedef QTextEdit DTextEdit;
typedef QPlainTextEdit DPlainTextEdit;
typedef QSpinBox DSpinBox;
typedef QDoubleSpinBox DDoubleSpinBox;
typedef QTimeEdit DTimeEdit;
typedef QDateEdit DDateEdit;
typedef QDateTimeEdit DDateTimeEdit;
typedef QDial DDial;
typedef QHorizontalSlider DHorizontalSlider;
typedef QVerticalSlider DVerticalSlider;
typedef QKeySequenceEdit DKeySequenceEdit;
typedef QTextBrowser DTextBrowser;
typedef QGraphicsView DGraphicsView;
typedef QCalendarWidget DCalendarWidget;
typedef QLCDNumber DLCDNumber;
typedef QProgressBar DProgressBar;
typedef QOpenGLWidget DOpenGLWidget;
typedef QQuickWidget DQuickWidget;
typedef QWebView DWebView;
typedef QAccessibleWidget DAccessibleWidget;
typedef QCheckBox DCheckBox;
typedef QColorDialog DColorDialog;
typedef QColumnView DColumnView;
typedef QDataWidgetMapper DDataWidgetMapper;
typedef QFocusFrame DFocusFrame;
typedef QHeaderView DHeaderView;
typedef QInputDialog DInputDialog;
typedef QMdiArea DMdiArea;
typedef QMdiSubWindow DMdiSubWindow;
typedef QErrorMessage DErrorMessage;
typedef QFontDialog DFontDialog;
typedef QMenu DMenu;
typedef QMenuBar DMenuBar;
typedef QMessageBox DMessageBox;
typedef QRubberBand DRubberBand;
typedef QSplitter DSplitter;
typedef QStatusBar DStatusBar;
typedef QTabWidget DTabWidget;
typedef QTableView DTableView;
typedef QTileRules DTileRules;
typedef QToolBar DToolBar;
typedef QToolTip DToolTip;
typedef QTreeView DTreeView;
typedef QUndoView DUndoView;
typedef QWhatsThis DWhatsThis;
typedef QWizard DWizard;
typedef QWizardPage DWizardPage;
typedef QLabel DLabel;

class DHorizontalLine : public QFrame
{
public:
    explicit DHorizontalLine(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QFrame(parent, f)
    {
        setFrameShape(HLine);
    }
};

class DVerticalLine : public QFrame
{
public:
    explicit DVerticalLine(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QFrame(parent, f)
    {
        setFrameShape(VLine);
    }
};

DWIDGET_END_NAMESPACE

#endif // DWIDGETSTYPE_H
