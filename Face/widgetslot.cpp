#include "Wigdet.h"
#include <QDebug>

#include "arcsoft_face_sdk.h"
#include "amcomdef.h"
#include "asvloffscreen.h"
#include "merror.h"

#include "core.hpp"
#include "opencv.hpp"

void Wigdet::initSLot()
{
    connect(&m_loadPhotoBtn, SIGNAL(clicked()), this, SLOT(onLoadPhotoBtnClicked()));
    connect(&m_loadIdCardBtn, SIGNAL(clicked()), this, SLOT(onLoadIdCardBtnClicked()));
    connect(&m_compareBtn, SIGNAL(clicked()), this, SLOT(onCompareBtnClicked()));
}

void Wigdet::onLoadPhotoBtnClicked()
{
    QFileDialog fd(this);

    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setWindowTitle("图像选择：");

    if( fd.exec() == QFileDialog::Accepted )
    {
        QImage img;

        if( img.load(fd.selectedFiles()[0]) )
        {
            m_photo.img = img;

            update();
        }
        else
        {
            QMessageBox::critical(this, "错误", "请选择有效得图片文件 !");
        }
    }
}

void Wigdet::onLoadIdCardBtnClicked()
{
    QFileDialog fd(this);

    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setWindowTitle("图像选择：");

    if( fd.exec() == QFileDialog::Accepted )
    {
        QImage img;

        if( img.load(fd.selectedFiles()[0]) )
        {
            m_idcard.img = img;

            update();
        }
        else
        {
            QMessageBox::critical(this, "错误", "请选择有效得图片文件 !");
        }
    }
}

void Wigdet::onCompareBtnClicked()
{
    if( m_photo.img.isNull() )
    {
        QMessageBox::critical(this, "错误", "请选择有效得图片文件 !");

        return;
    }

    if( m_idcard.img.isNull() )
    {
        QMessageBox::critical(this, "错误", "请选择有效得图片文件 !");

        return;
    }

    QTime time;
    time.start();

    if( doCompare(m_photo, m_idcard, static_cast<float>(m_valSpinBox.value())) )
    {
        QString title_text;
        QString text;

        title_text += "经过 " + QString().number(time.elapsed()) + "ms 的观察";

        text += "这 是 同 一 个 人 !\n\n";

        text += "性别 ：  ";
        if( m_photo.gender == 0 )
            text += "男\n";
        else if( m_photo.gender == 1 )
            text += "女\n";
        else
            text += "你太难为我了\n";

        text += "年龄 ：  ";
        text += QString().number(m_photo.age);


        QMessageBox::information(this, title_text, text);
    }
    else
    {
        QString title_text;
        title_text += "经过 " + QString().number(time.elapsed()) + " ms的观察";

        QMessageBox::information(this, title_text, "这 不 是 同 一 个 人 !");
    }

    update();
}

void Wigdet::paintEvent(QPaintEvent*)
{
    if( !m_photo.img.isNull() )
    {
        QPainter painter;

        if( !m_photo.rect.isEmpty() )
        {
            QPen pen;

            pen.setColor(Qt::red);
            pen.setWidth(4);

            painter.begin(&m_photo.img);
            painter.setPen(pen);
            painter.drawRect(QRect(m_photo.rect.x(), m_photo.rect.y(), m_photo.rect.width(), m_photo.rect.height()));
            painter.end();

            m_photo.rect = QRect();
        }

        painter.begin(this);
        painter.drawImage(QRect(m_photoBox.x()+5, m_photoBox.y()+5, m_photoBox.width()-10, m_photoBox.height()-10), m_photo.img);
        painter.end();
    }

    if( !m_idcard.img.isNull() )
    {
        QPainter painter;

        if( !m_idcard.rect.isEmpty() )
        {
            QPen pen;

            pen.setColor(Qt::red);
            pen.setWidth(5);

            painter.begin(&m_idcard.img);
            painter.setPen(pen);
            painter.drawRect(QRect(m_idcard.rect.x(), m_idcard.rect.y(), m_idcard.rect.width(), m_idcard.rect.height()));
            painter.end();

            m_idcard.rect = QRect();
        }

        painter.begin(this);
        painter.drawImage(QRect(m_idCardBox.x()+5, m_idCardBox.y()+5, m_idCardBox.width()-10, m_idCardBox.height()-10), m_idcard.img);
        painter.end();
    }
}

#define APPID   "AV3HLtNhNM98X8tGBB4GtG14PD34gouNHBQVY9E4bjEa"  //APPID
#define SDKKey  "6FSukJpCaon1ajHdSvmKXoGUmCoawcRNkUcAmBW8FG9a"  //SDKKey
#define MERR_ASF_BASE_ALREADY_ACTIVATED (0x16002)

bool Wigdet::doCompare(Imag& img_photo, Imag& img_idcard, float val)
{
    bool pResult = false;

    /** 1. 激活SDK */
    MRESULT res = ASFActivation(const_cast<char*>(APPID), const_cast<char*>(SDKKey));
    if (MOK == res || MERR_ASF_BASE_ALREADY_ACTIVATED == res)
    {
        qDebug() << "ALActivation sucess: " << res;
    }
    else
    {
        qDebug() << "ALActivation fail: " << res;
    }

    /** 2. 初始化引擎 */
    MHandle handle = NULL;
    MInt32 mask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE;
    res = ASFInitEngine(static_cast<MInt32>(ASF_DETECT_MODE_IMAGE), ASF_OP_0_ONLY, 16, 5, mask, &handle);
    if (res == MOK)
    {
        qDebug() << "ALInitEngine sucess: " << res;
    }
    else
    {
        qDebug() << "ALInitEngine fail: " << res;
    }

    /** 3. 人脸检测 */
    img_photo.img.scaled(img_photo.img.width()/4*4, img_photo.img.height()/4*4).save("img1.png");
    img_idcard.img.scaled(img_idcard.img.width()/4*4, img_idcard.img.height()/4*4).save("img2.png");

    IplImage* img1  = cvLoadImage("img1.png");
    IplImage* img2  = cvLoadImage("img2.png");

    if (img1 && img2)
    {
        /** 3.1 第一张人脸特征提取 */
        ASF_MultiFaceInfo  detectedFaces1       = { 0 };
        ASF_SingleFaceInfo SingleDetectedFaces1 = { 0 };
        ASF_FaceFeature    feature1             = { 0 };
        ASF_FaceFeature    copyfeature1         = { 0 };
        res = ASFDetectFaces(handle, img1->width, img1->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)img1->imageData, &detectedFaces1);
        if (MOK == res)
        {
            SingleDetectedFaces1.faceRect.left   = detectedFaces1.faceRect[0].left;
            SingleDetectedFaces1.faceRect.top    = detectedFaces1.faceRect[0].top;
            SingleDetectedFaces1.faceRect.right  = detectedFaces1.faceRect[0].right;
            SingleDetectedFaces1.faceRect.bottom = detectedFaces1.faceRect[0].bottom;
            SingleDetectedFaces1.faceOrient      = 0x05;

            qDebug() << detectedFaces1.faceNum;

            res = ASFFaceFeatureExtract(handle, img1->width, img1->height, ASVL_PAF_RGB24_B8G8R8, reinterpret_cast<MUInt8*>(img1->imageData), &SingleDetectedFaces1, &feature1);
            if (res == MOK)
            {
                /** 3.1.1 拷贝feature */
                copyfeature1.featureSize = feature1.featureSize;
                copyfeature1.feature = reinterpret_cast<MByte*>(malloc(static_cast<size_t>(feature1.featureSize)));
                memset(copyfeature1.feature, 0, static_cast<size_t>(feature1.featureSize));
                memcpy(copyfeature1.feature, feature1.feature, static_cast<size_t>(feature1.featureSize));  

                int x = SingleDetectedFaces1.faceRect.left;
                int y = SingleDetectedFaces1.faceRect.top;
                int w = SingleDetectedFaces1.faceRect.right - SingleDetectedFaces1.faceRect.left;
                int h = SingleDetectedFaces1.faceRect.bottom - SingleDetectedFaces1.faceRect.top;

                img_photo.rect = QRect(x, y, w, h);

                /** 3.1.2 人脸信息检测 */
                MInt32 processMask = ASF_AGE | ASF_GENDER;
                res = ASFProcess(handle, img1->width, img1->height, ASVL_PAF_RGB24_B8G8R8, reinterpret_cast<MUInt8*>(img1->imageData), &detectedFaces1, processMask);
                if (res == MOK)
                {
                    qDebug() << "ASFProcess sucess: " << res;
                }
                else
                {
                    qDebug() << "ASFProcess fail: " << res;
                }

                /** 3.1.3 获取年龄 */
                ASF_AgeInfo ageInfo = { 0 };
                res = ASFGetAge(handle, &ageInfo);
                if (res == MOK)
                {
                    img_photo.age = ageInfo.ageArray[0];

                    qDebug() << ageInfo.ageArray[0];
                    qDebug() << "ASFGetAge sucess: " << res;
                }
                else
                {
                    qDebug() << "ASFGetAge fail: " << res;
                }

                /** 3.1.4 获取性别 */
                ASF_GenderInfo genderInfo = { 0 };
                res = ASFGetGender(handle, &genderInfo);
                if (res == MOK)
                {
                    img_photo.gender = genderInfo.genderArray[0];

                    qDebug() << genderInfo.genderArray[0];
                    qDebug() << "ASFGetGender sucess: " << res;
                }
                else
                {
                    qDebug() << "ASFGetGender fail: " << res;
                }

                qDebug() << "ASFFaceFeatureExtract 1 Success";
            }
            else
            {
                qDebug() << "ASFFaceFeatureExtract 1 fail: " << res;
            }
        }
        else
        {
            qDebug() << "ASFDetectFaces 1 fail: " << res;
        }

        /** 3.2 第二张人脸特征提取 */
        ASF_MultiFaceInfo	detectedFaces2       = { 0 };
        ASF_SingleFaceInfo  SingleDetectedFaces2 = { 0 };
        ASF_FaceFeature     feature2             = { 0 };
        res = ASFDetectFaces(handle, img2->width, img2->height, ASVL_PAF_RGB24_B8G8R8, reinterpret_cast<MUInt8*>(img2->imageData), &detectedFaces2);
        if (MOK == res)
        {
            SingleDetectedFaces2.faceRect.left   = detectedFaces2.faceRect[0].left;
            SingleDetectedFaces2.faceRect.top    = detectedFaces2.faceRect[0].top;
            SingleDetectedFaces2.faceRect.right  = detectedFaces2.faceRect[0].right;
            SingleDetectedFaces2.faceRect.bottom = detectedFaces2.faceRect[0].bottom;
            SingleDetectedFaces2.faceOrient      = detectedFaces2.faceOrient[0];

            res = ASFFaceFeatureExtract(handle, img2->width, img2->height, ASVL_PAF_RGB24_B8G8R8, reinterpret_cast<MUInt8*>(img2->imageData), &SingleDetectedFaces2, &feature2);
            if (res == MOK)
            {
                int x = SingleDetectedFaces2.faceRect.left;
                int y = SingleDetectedFaces2.faceRect.top;
                int w = SingleDetectedFaces2.faceRect.right - SingleDetectedFaces2.faceRect.left;
                int h = SingleDetectedFaces2.faceRect.bottom - SingleDetectedFaces2.faceRect.top;

                img_idcard.rect = QRect(x, y, w, h);

                qDebug() << "ASFFaceFeatureExtract 2 Success";
            }
            else
            {
                qDebug() << "ASFFaceFeatureExtract 2 fail: " << res;
            }
        }
        else
        {
            qDebug() << "ASFDetectFaces 2 fail: " << res;
        }

        /** 3.3 单人脸特征比对 */
        MFloat confidenceLevel;
        res = ASFFaceFeatureCompare(handle, &copyfeature1, &feature2, &confidenceLevel);
        if (res == MOK)
        {
            qDebug() << "ASFFaceFeatureCompare sucess: " << confidenceLevel;

            if( confidenceLevel >= val ) pResult = true;
        }
        else
        {
            qDebug() << "ASFFaceFeatureCompare fail: " << res;
        }
    }

    /** 4. 反初始化 */
    res = ASFUninitEngine(handle);
    if (res != MOK)
    {
        qDebug() << "ALUninitEngine fail: " << res;
    }
    else
    {
        qDebug() << "ALUninitEngine sucess: " << res;
    }

    return pResult;
}

