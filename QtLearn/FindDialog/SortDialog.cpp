#include "SortDialog.h"
#include "ui_SortDialog.h"

SortDialog::SortDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::SortDialog())
{
    ui->setupUi(this);

    ui->secondGroupBox->hide();
    ui->thirdGroupBox->hide();
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    connect(ui->moreButton, SIGNAL(toggled(bool)), this, SLOT(setAdvancedButtonText(bool)));
}

void SortDialog::setColumnRange(QChar first, QChar last)
{
    ui->primaryColumnComboBox->clear();
    ui->secondColumnComboBox->clear();
    ui->tertiaryColumnComboBox->clear();

    ui->secondColumnComboBox->addItem("None");
    ui->tertiaryColumnComboBox->addItem("None");
    ui->primaryColumnComboBox->setMinimumSize(ui->secondColumnComboBox->sizeHint());

    QChar ch = first;
    while (ch <= last) {
        ui->primaryColumnComboBox->addItem(ch);
        ui->secondColumnComboBox->addItem(ch);
        ui->tertiaryColumnComboBox->addItem(ch);
        ch = ch.unicode() + 1;
    }
}

void SortDialog::setAdvancedButtonText(bool toggled)
{
    if (toggled) {
        ui->moreButton->setText("More <<");
    } else {
        ui->moreButton->setText("More >>");
    }
}

SortDialog::~SortDialog()
{
    delete ui;
}

