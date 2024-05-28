// carwidget.h
#ifndef CARWIDGET_H
#define CARWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class CarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CarWidget(QWidget* parent = nullptr);

    void setCarInfo(const QString& registrationNumber, const QString& brand, const QString& color, int year);

private slots:
    void toggleExpanded();

private:
    QLabel* registrationNumberLabel;
    QLabel* brandLabel;
    QLabel* colorLabel;
    QLabel* yearLabel;
    QPushButton* expandButton;

    bool expanded;
    QVBoxLayout* mainLayout;
};

#endif // CARWIDGET_H
