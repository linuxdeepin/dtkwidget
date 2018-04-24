#ifndef DPASSWDEDITANIMATED_H
#define DPASSWDEDITANIMATED_H

#include <DObject>
#include <QLineEdit>
#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE

class DPasswdEditAnimatedPrivate;
class LIBDTKWIDGETSHARED_EXPORT DPasswdEditAnimated : public QFrame, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DPasswdEditAnimated)

    Q_PROPERTY(bool alert READ alert NOTIFY alertChanged)
    Q_PROPERTY(bool editFocus READ hasFocus NOTIFY editFocusChanged)

public:
    DPasswdEditAnimated(QWidget *parent);

Q_SIGNALS:
    void submit(const QString &input);
    void abort();
    void alertChanged(bool alert);
    void editFocusChanged(bool focus);

public Q_SLOTS:
    void setEchoMode(QLineEdit::EchoMode mode);
    void setKeyboardButtonEnable(bool value);
    void setCapslockIndicatorEnable(bool value);
    void setEyeButtonEnable(bool value);
    void setSubmitButtonEnable(bool value);
    void setLoadAnimEnable(bool value);
    void setSubmitIcon(const QString &normalPic, const QString &hoverPic, const QString &pressPic);
    void showAlert(const QString &message);
    void hideAlert();
    void abortAuth();
    bool alert();

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

protected:
    D_PRIVATE_SLOT(void _q_resetKeyboardState())
    D_PRIVATE_SLOT(void _q_resetCapslockState())
    D_PRIVATE_SLOT(void _q_onEyeButtonClicked())
    D_PRIVATE_SLOT(void _q_showLoadSlider())
    D_PRIVATE_SLOT(void _q_hideLoadSlider())
    D_PRIVATE_SLOT(void _q_inputDone())

private:
    D_DECLARE_PRIVATE(DPasswdEditAnimated)
};

DWIDGET_END_NAMESPACE

#endif // DPASSWDEDITANIMATED_H
