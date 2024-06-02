#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <random>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty())
    {
        image = cv::imread(fileName.toStdString());
        if (image.empty()) {
            QMessageBox::critical(this, "Error", "Could not open or find the image");
            return;
        }
        displayImage(image, ui->originalImageLabel);
    }
}

void MainWindow::on_encryptButton_clicked()
{
    if (image.empty()) {
        QMessageBox::warning(this, "Warning", "Load an image first");
        return;
    }

    encryptedImage = image.clone();
    unsigned int seed = 12345;  // You can change this to any value for different noise patterns
    addNoise(encryptedImage, noise, seed);
    displayImage(encryptedImage, ui->encryptedImageLabel);
}

void MainWindow::on_decryptButton_clicked()
{
    if (encryptedImage.empty() || noise.empty()) {
        QMessageBox::warning(this, "Warning", "Encrypt an image first");
        return;
    }

    cv::Mat decryptedImage = encryptedImage.clone();
    decryptImage(decryptedImage, noise);
    displayImage(decryptedImage, ui->decryptedImageLabel);
}

void MainWindow::on_saveImageButton_clicked()
{
    if (encryptedImage.empty()) {
        QMessageBox::warning(this, "Warning", "Encrypt an image first");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        cv::imwrite(fileName.toStdString(), encryptedImage);
    }
}

void MainWindow::displayImage(const cv::Mat& img, QLabel* label)
{
    cv::Mat rgb;
    cv::cvtColor(img, rgb, cv::COLOR_BGR2RGB);
    QImage qimg((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(qimg).scaled(label->size(), Qt::KeepAspectRatio));
}

void MainWindow::addNoise(cv::Mat& image, cv::Mat& noise, unsigned int seed)
{
    noise = cv::Mat::zeros(image.size(), CV_8UC3);
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, 255);

    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            for (int c = 0; c < 3; ++c) {
                int noise_value = distribution(generator);
                noise.at<cv::Vec3b>(y, x)[c] = noise_value;
                image.at<cv::Vec3b>(y, x)[c] = image.at<cv::Vec3b>(y, x)[c] ^ noise_value;
            }
        }
    }
}

void MainWindow::decryptImage(cv::Mat& encryptedImage, const cv::Mat& noise)
{
    for (int y = 0; y < encryptedImage.rows; ++y) {
        for (int x = 0; x < encryptedImage.cols; ++x) {
            for (int c = 0; c < 3; ++c) {
                encryptedImage.at<cv::Vec3b>(y, x)[c] = encryptedImage.at<cv::Vec3b>(y, x)[c] ^ noise.at<cv::Vec3b>(y, x)[c];
            }
        }
    }
}
