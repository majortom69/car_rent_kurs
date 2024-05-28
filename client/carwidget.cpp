#include "carwidget.h"

CarWidget::CarWidget(QWidget* parent) : QWidget(parent), expanded(false)
{
    registrationNumberLabel = new QLabel(this);
    brandLabel = new QLabel(this);
    colorLabel = new QLabel(this);
    yearLabel = new QLabel(this);
    expandButton = new QPushButton("Expand", this);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(registrationNumberLabel);
    mainLayout->addWidget(brandLabel);
    mainLayout->addWidget(colorLabel);
    mainLayout->addWidget(yearLabel);
    mainLayout->addWidget(expandButton);

    connect(expandButton, &QPushButton::clicked, this, &CarWidget::toggleExpanded);
}

void CarWidget::setCarInfo(const QString& registrationNumber, const QString& brand, const QString& color, int year)
{
    registrationNumberLabel->setText("Registration Number: " + registrationNumber);
    brandLabel->setText("Brand: " + brand);
    colorLabel->setText("Color: " + color);
    yearLabel->setText("Year: " + QString::number(year));
}

void CarWidget::toggleExpanded()
{
    expanded = !expanded;
    if (expanded)
    {
        expandButton->setText("Collapse");
        // Add additional widgets/info for expanded view
    }
    else
    {
        expandButton->setText("Expand");
        // Remove additional widgets/info for collapsed view
    }
}