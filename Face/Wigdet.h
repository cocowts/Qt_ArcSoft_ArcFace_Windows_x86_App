#ifndef WIGDET_H
#define WIGDET_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QIcon>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QRect>
#include <QTime>

class Wigdet : public QWidget
{
    Q_OBJECT

private:

    struct Imag
    {
        QImage img;
        QRect rect;
        int age;
        int gender;
    };

    Imag m_photo;
    Imag m_idcard;

    QGroupBox      m_photoBox;
    QGroupBox      m_idCardBox;

    QLabel         m_valLab;
    QDoubleSpinBox m_valSpinBox;

    QPushButton    m_loadPhotoBtn;
    QPushButton    m_loadIdCardBtn;
    QPushButton    m_compareBtn;

    void initUI();
    void initSLot();

    bool doCompare(Imag& img_photo, Imag& img_idcard, float val);

protected:
    void paintEvent(QPaintEvent*);

public:
    Wigdet(QWidget *parent = 0);
    ~Wigdet();

protected slots:
    void onLoadPhotoBtnClicked();
    void onLoadIdCardBtnClicked();
    void onCompareBtnClicked();
};

#endif // WIGDET_H
