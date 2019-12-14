#include "FindDialog.h"
#include "ui_FindDialog.h"
#include <QPushButton>

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoToCellDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
    ui->lineEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
}

void FindDialog::onTextChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->lineEdit->hasAcceptableInput());
}


FindDialog::~FindDialog()
{
    delete ui;
}
