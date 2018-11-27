#include "Wigdet.h"
#include <QTextCodec>

Wigdet::Wigdet(QWidget *parent)
    : QWidget(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));

    initUI();
    initSLot();
}

void Wigdet::initUI()
{
    m_photoBox.setParent(this);
    m_photoBox.move(8, 7);
    m_photoBox.resize(422, 278);

    m_idCardBox.setParent(this);
    m_idCardBox.move(440, 7);
    m_idCardBox.resize(276, 154);

    m_valLab.setParent(this);
    m_valLab.move(482, 171);
    m_valLab.resize(33, 16);
    m_valLab.setText("阈 值:");

    m_valSpinBox.setParent(this);
    m_valSpinBox.move(544, 166);
    m_valSpinBox.resize(159, 24);
    m_valSpinBox.setSingleStep(0.01);
    m_valSpinBox.setMinimum(0.01);
    m_valSpinBox.setMaximum(1.00);
    m_valSpinBox.setValue(0.82);

    m_loadPhotoBtn.setParent(this);
    m_loadPhotoBtn.move(454, 197);
    m_loadPhotoBtn.resize(249, 24);
    m_loadPhotoBtn.setText("图像导入");

    m_loadIdCardBtn.setParent(this);
    m_loadIdCardBtn.move(454, 228);
    m_loadIdCardBtn.resize(249, 24);
    m_loadIdCardBtn.setText("ID卡导入");

    m_compareBtn.setParent(this);
    m_compareBtn.move(454, 257);
    m_compareBtn.resize(249, 24);
    m_compareBtn.setText("面部识别");

    setWindowIcon(QIcon(":/pic/pic/icon.png"));
    setWindowTitle("AI Changes life [ 1508539502@qq.com -- TianSong ]");

    setFixedSize(726, 295);
}

Wigdet::~Wigdet()
{

}
