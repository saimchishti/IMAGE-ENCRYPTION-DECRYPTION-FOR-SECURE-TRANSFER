#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadImageButton_clicked();
    void on_encryptButton_clicked();
    void on_decryptButton_clicked();
    void on_saveImageButton_clicked();

private:
    Ui::MainWindow* ui;
    cv::Mat image;
    cv::Mat encryptedImage;
    cv::Mat noise;

    void displayImage(const cv::Mat& img, QLabel* label);
    void addNoise(cv::Mat& image, cv::Mat& noise, unsigned int seed);
    void decryptImage(cv::Mat& encryptedImage, const cv::Mat& noise);
};

#endif // MAINWINDOW_H
